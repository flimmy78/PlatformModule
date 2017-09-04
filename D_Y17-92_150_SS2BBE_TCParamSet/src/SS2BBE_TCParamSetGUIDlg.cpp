#include "SS2BBE_TCParamSetGUIDlg.h"
#include "ui_SS2BBE_TCParamSetGUIDlg.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDateTime>
#include "../../Common/Log.h"
#include "../../Common/CommonFunction.h"
#include <QTimer>
#pragma pack(1)

extern quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
                     uchar ucSn);
SS2BBE_TCParamSetGUIDlg::SS2BBE_TCParamSetGUIDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SS2BBE_TCParamSetGUIDlg)
{
    ui->setupUi(this);
    m_pStateMgr = NULL;
    m_pILxTsspMessageManager = NULL;
    m_pILxTsspLogManager = NULL;
    m_pPublicInfo = NULL;
    m_pHAManager = NULL;
    m_nTargetNum = 0;
    m_pMCSTargetSysStatus = NULL;
    m_MCSSSTCStatus = NULL;
    m_currentNumber_usedByMacro = -1;
    m_currentNumber_usedByParam = -1;
    m_timer1 = new QTimer(this);
    connect(m_timer1, SIGNAL(timeout()), this, SLOT(updateRadioButton()));
    setAttribute(Qt::WA_DeleteOnClose);
}

SS2BBE_TCParamSetGUIDlg::~SS2BBE_TCParamSetGUIDlg ()
{
    killTimer(m_TimerID);
    m_timer1->stop();
    delete m_timer1;
    delete ui;
}

void SS2BBE_TCParamSetGUIDlg::setText(const QString& strTitle, ParaStruct xmlsetText[5][60], QString MapMean[50][30], int MapNum[50], uchar Type, ILxTsspParameterMacro* pMacroObj)
{
    m_strTitle = strTitle;
    setWindowTitle(strTitle);

    memcpy(m_DlgParaSetStruct, xmlsetText, sizeof(ParaStruct) * 60 * 5);

    for(int i = 0;i < 50;i++)
    {
        for(int j = 0; j < 30; j++)
        {
            m_DlgMapMean[i][j] = MapMean[i][j];
        }
    }

    memset(DlgMapLineNum, 0, 50);
    for(int i = 0; i < 50; i++)
    {
        DlgMapLineNum[i] = MapNum[i];
    }
    //初始化界面
    InitDLG(Type, pMacroObj);
}

//初始化对话框
void SS2BBE_TCParamSetGUIDlg::InitDLG(int Type, ILxTsspParameterMacro* pMacroObj)
{
    iParaSetFlag = Type;
    //设置输入格式
    InitInput();
    //显示变量名字和范围
    ShowControls();

    if(Type == 0)//取当前参数
    {
        //取当前设置的参数
        if(m_pStateMgr != NULL)
        {
            DWORD stLen = sizeof(SMCU2::MCSSSTCStatusStruct4);
            m_MCSSSTCStatus = (SMCU2::MCSSSTCStatusStruct4*)m_pStateMgr->FindOneItem(m_StatusParaMCSSSTCID, stLen);
            if(m_MCSSSTCStatus == NULL || stLen != sizeof(SMCU2::MCSSSTCStatusStruct4))
                return;
        }
        //启动定时器
        m_TimerID = startTimer(500);
        //不使能
        EnabledControls(false);
    }
    else if(Type == 1)//取宏里参数
    {
        ui->pushButton_Modify->setText("");
        ui->pushButton_Set->setEnabled(false);
        if(pMacroObj != NULL)
        {
            QByteArray byteArray;
            pMacroObj->GetParameterBlock(m_ParaMCSSSTCID, byteArray);
            memcpy(&m_MCSSSTCParam, byteArray.data(), sizeof(SMCU2::MCSSSTCStatusStruct4));
        }
        ui->pushButton_Modify->setVisible(false);
        ui->pushButton_Set->setVisible(false);
        ui->lineEdit_ExeCodeLen->setEnabled(false);
        m_timer1->start(500);
        //遥控转发方式
        on_comboBox_TeleSendType_currentIndexChanged(ui->comboBox_TeleSendType->currentIndex());
        //编码控制
        on_comboBox_BCHEncode_currentIndexChanged(ui->comboBox_BCHEncode->currentIndex());
        //加扰控制
        on_comboBox_RandCtrl_currentIndexChanged(ui->comboBox_RandCtrl->currentIndex());
        //起始序列控制
        on_comboBox_BeginCtrl_currentIndexChanged(ui->comboBox_BeginCtrl->currentIndex());
        //结尾序列控制
        on_comboBox_LateCtrl_currentIndexChanged(ui->comboBox_LateCtrl->currentIndex());
        m_pCommFunc = CCommFunc::getInstance();
        //显示参数
        ShowPara(1);
    }
}

//设置输入格式
void SS2BBE_TCParamSetGUIDlg::InitInput()
{
    //正整数

    QRegExp regExp("[0-9]{1,8}");
    //码速率
    QRegExp regExp_Len("[0-9]{0,4}");
    //保护间隙
    QRegExp regExp_BufferGap("[0-9]{3,5}");
    //应急填充长度
    QRegExp regExp_EmergencyFillLen("[0-9]{0,3}");
    //码速率
    ui->lineEdit_CodeRate->setValidator(new QRegExpValidator(regExp_Len, this));
    //前补序列长度
    ui->lineEdit_BeforeFillLen->setValidator(new QRegExpValidator(regExp_Len, this));
    //后补序列长度
    ui->lineEdit_LateFillLen->setValidator(new QRegExpValidator(regExp_Len, this));
    //执行序列长度
    ui->lineEdit_ExeCodeLen->setValidator(new QRegExpValidator(regExp_Len, this));
    //保护间隙
    ui->lineEdit_BufferGap->setValidator(new QRegExpValidator(regExp_BufferGap, this));
    //应急填充长度
    ui->lineEdit_EmergencyFillLen->setValidator(new QRegExpValidator(regExp_EmergencyFillLen, this));

    //字母数字
    QRegExp regExp2("[A-Fa-f0-9]{1,8}");
    QRegExp regExp3("[A-Fa-f0-9]{1,2}");
    QRegExp regExp4("[A-Fa-f0-9]{1,4}");
    QRegExp regExp5("[A-Fa-f0-9]{1,16}");
    //前补序列
    ui->lineEdit_BeforeFillCode->setValidator(new QRegExpValidator(regExp3, this));
    //后补序列
    ui->lineEdit_LateFillCode->setValidator(new QRegExpValidator(regExp3, this));
    //执行序列
    ui->lineEdit_ExeCode->setValidator(new QRegExpValidator(regExp5, this));
    //空闲序列
    ui->lineEdit_FreeCode->setValidator(new QRegExpValidator(regExp3, this));
    //起始序列图样
    ui->lineEdit_BeginPattern->setValidator(new QRegExpValidator(regExp4, this));
    //结尾序列图样
    ui->lineEdit_LatePattern->setValidator(new QRegExpValidator(regExp5, this));
    //加扰多项式
    ui->lineEdit_RandPoly->setValidator(new QRegExpValidator(regExp2, this));
    //加扰初相
    ui->lineEdit_RandPhase->setValidator(new QRegExpValidator(regExp2, this));

}

void SS2BBE_TCParamSetGUIDlg::ShowControls()
{
    showTargetParam(0, 0);
    //默认选择目标1
    ui->radioButton_Target1->setChecked(true);
}

//显示参数
void SS2BBE_TCParamSetGUIDlg::ShowPara(int type)
{
    if(type == 0)       //参数设置
    {
        //码同步
        if(m_MCSSSTCStatus != NULL)
        {
            for (int nIndex = 0; nIndex < MAX_TARGET_COUNT; nIndex++)
                m_MCSSSTCParam.MCSSSB2TCPara[nIndex] = m_MCSSSTCStatus->MCSSSB2TCStatu[nIndex].tParams;
        }
        else
        {
            DWORD stLen = sizeof(SMCU2::MCSSSTCStatusStruct4);
            if(m_pStateMgr != NULL)
                m_MCSSSTCStatus  = (SMCU2::MCSSSTCStatusStruct4*)m_pStateMgr->FindOneItem(m_StatusParaMCSSSTCID, stLen);
        }
    }
    else
    {
        //解扰使能或禁止
        //副帧方式控件使能
        if (ui->pushButton_Modify->text() != "更改")
        {
            //EnabledControlsSubFrmType(ui->comboBox_SubFrmType->currentIndex());
        }
    }
    /********************************参数*********************************/
    showTargetParam(m_nTargetNum, 1);
    showRadioButton();
}

//设置帧长范围
void SS2BBE_TCParamSetGUIDlg::SetFrameLenRange(int iWordLen)
{
    /* if(iWordLen ==8)
    {
        m_DlgParaSetStruct[1][3].MinVal.lmin = 4;
        m_DlgParaSetStruct[1][3].MaxVal.lmax = 4096;
    }
    else
    {
        m_DlgParaSetStruct[1][3].MinVal.lmin = 4;
        m_DlgParaSetStruct[1][3].MaxVal.lmax = 2048;
    }
    QString strTemp = "帧长["+QString::number(m_DlgParaSetStruct[1][3].MinVal.lmin)+"~"+
            QString::number(m_DlgParaSetStruct[1][3].MaxVal.lmax)+"]字";
    ui->label_FrameLen->setText(strTemp);*/
}

void SS2BBE_TCParamSetGUIDlg::SetObjInfo(TLxTsspObjectInfo ObjectInfo)
{
    memcpy(&m_ObjectInfo, &ObjectInfo, sizeof(TLxTsspObjectInfo));
    //对象初始化
    //查找对象管理器
    LxTsspObjectManager* pObjectManager = LxTsspObjectManager::Instance();

    if(pObjectManager == NULL){
        return;
    }

    m_pHAManager = pObjectManager->GetHAManager();

    bool bRet = m_DeviceMap.loadXML();
    if (!bRet)
    {
        QString strInfo = QString("load device map failed %1");
        CLog::addLog(strInfo);
        return;
    }

    ITEM *pItem = m_DeviceMap.getItem(MODULENAME);
    if (pItem->vSubItem.size() > 0)
    {
        SUBITEM& sItem = *(pItem->getSubItemByDeviceID_SN(m_ObjectInfo.usLocalDID, m_ObjectInfo.ucSN));
        m_usC = sItem.usCMD;
        m_usTID = sItem.usTID;
        /*************************框架**********************************/
        m_ParaMCSSSTCID = GetID(sItem.usStationID,
                                sItem.ucSubSysID, sItem.usDeviceID,sItem.usTID,
                                sItem.ucSN);
        /*************************框架**********************************/

        /*************************状态**********************************/
        m_StatusParaMCSSSTCID = GetID(sItem.usStationID,
                                      sItem.ucSubSysID, sItem.usDeviceID,sItem.usStateType,
                                      sItem.ucSN);
        /*************************状态**********************************/
    }
    //获取目标参数的ID
    ITEM *pItemCom = m_DeviceMap.getItem(COMM_SS2BBE_NAME);
    if (pItemCom != NULL && pItemCom->vSubItem.size() > 0)
    {
        SUBITEM& sItem = *(pItemCom->getSubItemByDeviceID_SN(m_ObjectInfo.usLocalDID, m_ObjectInfo.ucSN));
        /*************************状态**********************************/
        m_CommTargetID = GetID(sItem.usStationID,
                               sItem.ucSubSysID, sItem.usDeviceID,sItem.usStateType,
                               sItem.ucSN);
        /*************************状态**********************************/
    }
    //日志
    m_pILxTsspLogManager = pObjectManager->GetLogManager();
    m_pStateMgr = pObjectManager->GetStateManager();
    m_pILxTsspMessageManager = pObjectManager->GetMessageManager();
    m_pPublicInfo = pObjectManager->GetPublicInfoManager();
}

//使能或不使能控件
void SS2BBE_TCParamSetGUIDlg::EnabledControls(bool bFlag)
{
    /*************************Group1*************************/
    //工作期启动方式
    ui->comboBox_StartingMethod->setEnabled(bFlag);
    //码型
    ui->comboBox_CodeType->setEnabled(bFlag);
    //码速率
    ui->lineEdit_CodeRate->setEnabled(bFlag);
    //遥控发送方式
    ui->comboBox_TeleSendType->setEnabled(bFlag);
    /*************************Group1*************************/

    /*************************Group2*************************/
    //前补序列码样
    ui->lineEdit_BeforeFillCode->setEnabled(bFlag);
    //后补序列码样
    ui->lineEdit_LateFillCode->setEnabled(bFlag);
    //帧同步码组
    ui->lineEdit_ExeCode->setEnabled(bFlag);
    //空闲序列码样
    ui->lineEdit_FreeCode->setEnabled(bFlag);
    //前补序列长度
    ui->lineEdit_BeforeFillLen->setEnabled(bFlag);
    //后补序列长度
    ui->lineEdit_LateFillLen->setEnabled(bFlag);
    //执行编码长度
    ui->lineEdit_ExeCodeLen->setEnabled(false);
    //解扰初相
    ui->lineEdit_BufferGap->setEnabled(bFlag);
    //BCH编码
    ui->comboBox_BCHEncode->setEnabled(bFlag);
    //其实序列控制
    ui->comboBox_BeginCtrl->setEnabled(bFlag);
    //其实序列图样
    ui->lineEdit_BeginPattern->setEnabled(bFlag);
    //加扰控制
    ui->comboBox_RandCtrl->setEnabled(bFlag);
    //结尾序列控制
    ui->comboBox_LateCtrl->setEnabled(bFlag);
    //结尾序列图样
    ui->lineEdit_LatePattern->setEnabled(bFlag);
    //加扰多项式长度
    ui->comboBox_RandPolyLen->setEnabled(bFlag);
    //加扰多项式
    ui->lineEdit_RandPoly->setEnabled(bFlag);
    //加扰多项式
    ui->lineEdit_RandPhase->setEnabled(bFlag);
    //应急填充长度
    ui->lineEdit_EmergencyFillLen->setEnabled(bFlag);

    /*************************Group2*************************/

    ui->checkBox_5->setEnabled(bFlag);
    ui->checkBox_6->setEnabled(bFlag);
    ui->checkBox_7->setEnabled(bFlag);
    ui->checkBox_8->setEnabled(bFlag);


    /*************************帧同步器*************************/
    if (bFlag)
    {
        //副帧方式控件使能
        //EnabledControlsSubFrmType(ui->comboBox_SubFrmType->currentIndex());
        ui->pushButton_Modify->setText("恢复");
        ui->pushButton_Set->setEnabled(true);
    }
    else
    {
        ui->pushButton_Modify->setText("更改");
        ui->pushButton_Set->setEnabled(false);
    }
}

//保存参数宏
int SS2BBE_TCParamSetGUIDlg::SaveToMacro(ILxTsspParameterMacro* pMacroObj)
{
    //判断合法性
    int iRet = CheckPara();
    if(iRet == -1)
    {
        return -1;
    }
    else
    {
        GetPara();
        //取当前设置的参数
        QByteArray byteArray;
        byteArray.resize(sizeof(m_MCSSSTCParam));
        memcpy(byteArray.data(),&m_MCSSSTCParam,sizeof(m_MCSSSTCParam));
        pMacroObj->UpdateParameterBlock(m_ParaMCSSSTCID,byteArray);

        //日志信息
        TLxTsspLog tmpLog;
        QString strLog;
        memset(&tmpLog, 0, sizeof(TLxTsspLog));
        tmpLog.ucWarningLevel  = 1;
        strLog += "数传解调参数设置: 保存宏成功！";
        strcpy(tmpLog.szContent, strLog.toUtf8().data());
        tmpLog.time = QDateTime::currentDateTime().toTime_t();

        m_pILxTsspLogManager->Add(tmpLog);
    }
}
//从界面取参数
void SS2BBE_TCParamSetGUIDlg::GetPara()
{
    getParamTarget();
}

//验证参数范围
int SS2BBE_TCParamSetGUIDlg::CheckPara()
{
    QMessageBox msgBox;
    QString strTmp;
    int randPolyLen = (ui->comboBox_RandPolyLen->currentIndex() + 1) * 8;
    QString randPoly = ui->lineEdit_RandPoly->text();
    QString randPhase = ui->lineEdit_RandPhase->text();
    UI64 randPolyCode = 0;
    randPolyCode = StrToSynCode(randPoly.toStdString().c_str());
    UI64 randPhaseCode = 0;
    randPhaseCode = StrToSynCode(randPhase.toStdString().c_str());
    UI64 tempSynCode = 0;
    UI64 tempSynCode1 = 0;
    tempSynCode = randPolyCode<<(64 - randPolyLen);
    tempSynCode = tempSynCode>>(64 - randPolyLen);
    tempSynCode1 = randPhaseCode<<(64 - randPolyLen);
    tempSynCode1 = tempSynCode1>>(64 - randPolyLen);
    msgBox.setWindowTitle(MODULENAME);
    //码速率
    if (ui->lineEdit_CodeRate->text().isEmpty())
    {
        strTmp = ("码速率不能为空，请重新输入！");
        goto flag;
    }
    if((ui->lineEdit_CodeRate->text().toInt()) < m_DlgParaSetStruct[0][1].MinVal.lmin
            || (ui->lineEdit_CodeRate->text().toInt()) > m_DlgParaSetStruct[0][1].MaxVal.lmax)
    {
        strTmp = QString("码速率正确范围\n:[%1～%2bps]，请重新输入!")
                .arg(m_DlgParaSetStruct[0][1].MinVal.lmin).arg(m_DlgParaSetStruct[0][1].MaxVal.lmax);
        goto flag;
    }
    if (ui->lineEdit_BeforeFillCode->text().isEmpty())
    {
        strTmp = ("前补序列不能为空，请重新输入！");
        goto flag;
    }
    if (ui->lineEdit_LateFillCode->text().isEmpty())
    {
        strTmp = ("后补序列不能为空，请重新输入！");
        goto flag;
    }
    if (ui->lineEdit_ExeCode->text().isEmpty())
    {
        strTmp = ("执行序列不能为空，请重新输入！");
        goto flag;
    }
    if (ui->lineEdit_FreeCode->text().isEmpty())
    {
        strTmp = ("空闲序列不能为空，请重新输入！");
        goto flag;
    }
    //前补序列长度
    if((ui->lineEdit_BeforeFillLen->text().toInt()) < m_DlgParaSetStruct[0][8].MinVal.lmin
            || (ui->lineEdit_BeforeFillLen->text().toInt()) > m_DlgParaSetStruct[0][8].MaxVal.lmax)
    {
        strTmp = QString("前补序列长度正确范围\n:[%1～%2]，请重新输入!")
                .arg(m_DlgParaSetStruct[0][8].MinVal.lmin).arg(m_DlgParaSetStruct[0][8].MaxVal.lmax);
        goto flag;
    }
    if (ui->lineEdit_BeforeFillLen->text().isEmpty())
    {
        strTmp = ("前补序列长度不能为空，请重新输入！");
        goto flag;
    }
    //后补序列长度
    if((ui->lineEdit_LateFillLen->text().toInt()) < m_DlgParaSetStruct[0][9].MinVal.lmin
            || (ui->lineEdit_LateFillLen->text().toInt()) > m_DlgParaSetStruct[0][9].MaxVal.lmax)
    {
        strTmp = QString("后补序列长度正确范围\n:[%1～%2]，请重新输入!")
                .arg(m_DlgParaSetStruct[0][9].MinVal.lmin).arg(m_DlgParaSetStruct[0][9].MaxVal.lmax);
        goto flag;
    }
    if (ui->lineEdit_LateFillLen->text().isEmpty())
    {
        strTmp = ("后补序列长度不能为空，请重新输入！");
        goto flag;
    }
    //执行序列长度
    if((ui->lineEdit_ExeCodeLen->text().toInt()) < m_DlgParaSetStruct[0][10].MinVal.lmin
            || (ui->lineEdit_ExeCodeLen->text().toInt()) > m_DlgParaSetStruct[0][10].MaxVal.lmax)
    {
        strTmp = QString("执行序列长度正确范围\n:[%1～%2]，请重新输入!")
                .arg(m_DlgParaSetStruct[0][10].MinVal.lmin).arg(m_DlgParaSetStruct[0][10].MaxVal.lmax);
        goto flag;
    }
    if (ui->lineEdit_ExeCodeLen->text().isEmpty())
    {
        strTmp = ("执行序列长度不能为空，请重新输入！");
        goto flag;
    }
    //保护间隙
    if (ui->lineEdit_BufferGap->text().isEmpty())
    {
        strTmp = ("保护间隙不能为空，请重新输入！");
        goto flag;
    }
    if((ui->lineEdit_BufferGap->text().toInt()) < m_DlgParaSetStruct[0][11].MinVal.lmin
            || (ui->lineEdit_BufferGap->text().toInt()) > m_DlgParaSetStruct[0][11].MaxVal.lmax)
    {
        strTmp = QString("保护间隙正确范围\n:[%1～%2]，请重新输入!")
                .arg(m_DlgParaSetStruct[0][11].MinVal.lmin).arg(m_DlgParaSetStruct[0][11].MaxVal.lmax);
        goto flag;
    }
    //应急填充长度
    if (ui->lineEdit_EmergencyFillLen->text().isEmpty())
    {
        strTmp = ("应急填充长度不能为空，请重新输入！");
        goto flag;
    }
    if((ui->lineEdit_EmergencyFillLen->text().toInt()) < m_DlgParaSetStruct[0][12].MinVal.lmin
            || (ui->lineEdit_EmergencyFillLen->text().toInt()) > m_DlgParaSetStruct[0][12].MaxVal.lmax)
    {
        strTmp = QString("应急填充长度正确范围\n:[%1～%2]，请重新输入!")
                .arg(m_DlgParaSetStruct[0][12].MinVal.lmin).arg(m_DlgParaSetStruct[0][12].MaxVal.lmax);
        goto flag;
    }
    //控制部分
    if (ui->lineEdit_BeginPattern->text().isEmpty())
    {
        strTmp = ("起始序列图样不能为空，请重新输入！");
        goto flag;
    }
    if (ui->lineEdit_LatePattern->text().isEmpty())
    {
        strTmp = ("结尾序列图样不能为空，请重新输入！");
        goto flag;
    }
    if (randPoly.isEmpty())
    {
        strTmp = ("输入的加扰多项式不能为空，请重新输入！");
        goto flag;
    }
    if (randPhase.isEmpty())
    {
        strTmp = ("输入的加扰初项不能为空，请重新输入！");
        goto flag;
    }
    if (tempSynCode != randPolyCode)
    {
        strTmp = ("输入的加扰多项式长度大于设置的加扰多项式长度，请重新输入！");
        goto flag;
    }
    if (tempSynCode1 != randPhaseCode)
    {
        strTmp = ("输入的加扰多项式长度大于设置的加扰多项式长度，请重新输入！");
        goto flag;
    }
    return 1;
flag:
    msgBox.setText(strTmp);
    msgBox.exec();
    return -1;
}

//定时器
void SS2BBE_TCParamSetGUIDlg::timerEvent(QTimerEvent *event)
{
    ShowPara(0);
}

//更改参数按钮休息处理函数
void SS2BBE_TCParamSetGUIDlg::on_pushButton_Modify_clicked()
{
    if(ui->pushButton_Modify->text() == "更改")
    {
        EnabledControls(true);
        killTimer(m_TimerID);
        on_comboBox_BCHEncode_currentIndexChanged(ui->comboBox_BCHEncode->currentIndex());
        on_comboBox_RandCtrl_currentIndexChanged(ui->comboBox_RandCtrl->currentIndex());
        on_comboBox_TeleSendType_currentIndexChanged(ui->comboBox_TeleSendType->currentIndex());
        on_lineEdit_ExeCode_textChanged(ui->lineEdit_ExeCode->text());
        return;
    }

    if(ui->pushButton_Modify->text() == "恢复")
    {
        EnabledControls(false);
        m_TimerID = startTimer(500);
        return;
    }
}

void SS2BBE_TCParamSetGUIDlg::on_pushButton_Set_clicked()
{
    //验证参数范围
    if(CheckPara() == 1)
    {
        //从界面取参数
        GetPara();

        //设置参数
        //取当前设置的参数
        if(m_pHAManager != NULL)
        {
            struct{
                USHORT target;
                struct{
                    USHORT tmp;
                    SMCU2::MCSSSB2TCParamStruct m_MCSSSTCParam;
                }param[MAX_TARGET_COUNT];
            }data;
            data.target = 1 | 2 | 4 | 8 | 16;
            for (int i = 0; i < MAX_TARGET_COUNT; i++)
            {
                data.param[i].m_MCSSSTCParam = m_MCSSSTCParam.MCSSSB2TCPara[i];
            }
            QByteArray byteArray;
            byteArray.resize(sizeof(data));
            memcpy(byteArray.data(), &data, sizeof(data));
            SendMessage(byteArray.data(), byteArray.size(), m_usC ,m_usTID);
        }

        //日志信息
        TLxTsspLog tmpLog;
        QString strLog;
        memset(&tmpLog, 0, sizeof(TLxTsspLog));
        tmpLog.ucWarningLevel  = 1;
        strLog += "解调参数设置: 设置参数成功！";
        strcpy(tmpLog.szContent, strLog.toUtf8().data());
        tmpLog.time = QDateTime::currentDateTime().toTime_t();

        m_pILxTsspLogManager->Add(tmpLog);

        //不使能控件
        EnabledControls(false);
        m_TimerID = startTimer(500);
    }
}

void SS2BBE_TCParamSetGUIDlg::closeEvent(QCloseEvent *event)
{
    event->accept();
}

QSize SS2BBE_TCParamSetGUIDlg::sizeHint() const
{
    return QSize(this->width(), this->height());
}

void SS2BBE_TCParamSetGUIDlg::SubFrmEn(bool bFlag)
{
    //副帧长度
    //    ui->label_SubFrmLen->setEnabled(bFlag);
    //    ui->lineEdit_SubFrmLen->setEnabled(bFlag);

    //    ui->label_CFSearchErrBits->setEnabled(bFlag);
    //    ui->comboBox_CFSearchErrBits->setEnabled(bFlag);
    //    ui->label_CFVerifyErrBits->setEnabled(bFlag);
    //    ui->comboBox_CFVerifyErrBits->setEnabled(bFlag);
    //    ui->label_CFLockErrBits->setEnabled(bFlag);
    //    ui->comboBox_CFLockErrBits->setEnabled(bFlag);
    //    ui->label_CFVerifyCheckFrame->setEnabled(bFlag);
    //    ui->comboBox_SubFVerifyFrame->setEnabled(bFlag);
    //    ui->label_CFLockCheckFrame->setEnabled(bFlag);
    //    ui->comboBox_SubFLockFrame->setEnabled(bFlag);
}

//有无ID副帧
void SS2BBE_TCParamSetGUIDlg::IDSubFrmEn(bool bFlag)
{
    //    ui->groupBox_IDSubFrm->setEnabled(bFlag);
    //    //ID副帧位置
    //    ui->label_IDSubFrmPos->setEnabled(bFlag);
    //    ui->lineEdit_IDSubFrmPos->setEnabled(bFlag);

    //    //ID基值
    //    ui->label_IdBase->setEnabled(bFlag);
    //    ui->comboBox_IdBase->setEnabled(bFlag);

    //    //ID字方向
    //    ui->label_IdDirect->setEnabled(bFlag);
    //    ui->comboBox_IdDirect->setEnabled(bFlag);
}

//有无循环副帧
void SS2BBE_TCParamSetGUIDlg::CycSubFrmEn(bool bFlag)
{
    //循环副帧
    //    ui->groupBox_CycSubFrm_->setEnabled(bFlag);
    //    ui->label_CycFrmSyncCode->setEnabled(bFlag);
    //    ui->lineEdit_CycFrmSyncCode->setEnabled(bFlag);
    //    ui->label_CycFrmSynCodePos->setEnabled(bFlag);
    //    ui->lineEdit_CycFrmSynCodePos->setEnabled(bFlag);
    //    ui->label_CycFrmSynCodeLen->setEnabled(bFlag);
    //    ui->lineEdit_CycFrmSynCodeLen->setEnabled(bFlag);
}

//有无反码副帧
void SS2BBE_TCParamSetGUIDlg::ReserveSubFrmEn(bool bFlag)
{

}

void SS2BBE_TCParamSetGUIDlg::SendMessage(char* pData, unsigned short  usLen, unsigned short usC, unsigned short usTID)
{
    static uint seq = 0;
    TLxTsspMessageHeader header = {0};
    TLxTsspSubMsgHeader subHeader = {0};
    header.uiSequence = seq++;
    header.uiHandle = 1;
    header.S = m_pPublicInfo->GetLocalDeviceID();
    header.L = sizeof(TLxTsspMessageHeader) +sizeof(TLxTsspSubMsgHeader) + usLen;
    header.D = m_pPublicInfo->GetLocalDeviceID();//0x09;
    header.T = 1;
    header.M = 1;
    header.P = 1;
    header.A = 0;
    header.R = 0;
    header.C = 0x200;

    subHeader.Len = usLen;
    subHeader.SID = m_pPublicInfo->GetLocalDeviceID();//m_DObjectInfo.usStationID;
    subHeader.DID = m_ObjectInfo.usLocalDID;
    subHeader.TID = usTID;
    subHeader.SN = m_ObjectInfo.ucSubSysID;
    subHeader.O = 0;
    TLxTsspMessage message;
    message.pData =
            new char[sizeof(TLxTsspMessageHeader) + sizeof(TLxTsspSubMsgHeader) + usLen ];
    memcpy(message.pData, &header, sizeof(TLxTsspMessageHeader));
    memcpy(message.pData + sizeof(TLxTsspMessageHeader),
           &subHeader, sizeof(TLxTsspSubMsgHeader));
    memcpy(message.pData + sizeof(TLxTsspMessageHeader) + sizeof(TLxTsspSubMsgHeader),
           pData, usLen);
    message.iDataLen = sizeof(TLxTsspMessageHeader) + sizeof(TLxTsspSubMsgHeader) + usLen;

    int iRet = m_pILxTsspMessageManager->FireMessage(message);
    delete[] message.pData;
}

void SS2BBE_TCParamSetGUIDlg::showTargetParam(int nIndex, int nType)
{
    if (nType == 0)
    {
        //向下拉框控件中写入从xml中读取的内容
        int i;
        //码型
        for(i = 0; i < DlgMapLineNum[nIndex * 8 + 0];i++)
        {
            ui->comboBox_CodeType->addItem(m_DlgMapMean[nIndex * 8 + 0][i]);
        }
        ui->comboBox_CodeType->setCurrentIndex(m_DlgParaSetStruct[nIndex][0].InitVal.lval);
        //遥控发送方式
        for(i = 0; i < DlgMapLineNum[nIndex * 8 + 1];i++)
        {
            ui->comboBox_TeleSendType->addItem(m_DlgMapMean[nIndex * 8 + 1][i]);
        }
        ui->comboBox_TeleSendType->setCurrentIndex(m_DlgParaSetStruct[nIndex][2].InitVal.lval);
        //工作期启动方式
        for(i=0; i<DlgMapLineNum[nIndex * 8 + 2];i++)
        {
            ui->comboBox_StartingMethod->addItem(m_DlgMapMean[nIndex * 8 + 2][i]);
        }
        ui->comboBox_StartingMethod->setCurrentIndex(m_DlgParaSetStruct[nIndex][3].InitVal.lval);
        //编码控制
        for(i = 0; i < DlgMapLineNum[nIndex * 8 + 3]; i++)
        {
            ui->comboBox_BCHEncode->addItem(m_DlgMapMean[nIndex * 8 + 3][i]);
        }
        ui->comboBox_BCHEncode->setCurrentIndex(m_DlgParaSetStruct[nIndex][13].InitVal.lval);
        //起始序列控制
        for(i = 0; i < DlgMapLineNum[nIndex * 8 + 4];i++)
        {
            ui->comboBox_BeginCtrl->addItem(m_DlgMapMean[nIndex * 8 + 4][i]);
        }
        ui->comboBox_BeginCtrl->setCurrentIndex(m_DlgParaSetStruct[nIndex][14].InitVal.lval);
        //加扰使能
        for(i = 0; i < DlgMapLineNum[nIndex * 8 + 5];i++)
        {
            ui->comboBox_RandCtrl->addItem(m_DlgMapMean[nIndex * 8 + 5][i]);
        }
        ui->comboBox_RandCtrl->setCurrentIndex(m_DlgParaSetStruct[nIndex][16].InitVal.lval);
        //结尾序列控制
        for(i=0; i<DlgMapLineNum[nIndex * 8 + 6];i++)
        {
            ui->comboBox_LateCtrl->addItem(m_DlgMapMean[nIndex * 8 + 6][i]);
        }
        ui->comboBox_LateCtrl->setCurrentIndex(m_DlgParaSetStruct[nIndex][17].InitVal.lval);
        //加扰多项式长度
        for(i=0; i<DlgMapLineNum[nIndex * 8 + 7];i++)
        {
            ui->comboBox_RandPolyLen->addItem(m_DlgMapMean[nIndex * 8 + 7][i]);
        }
        ui->comboBox_RandPolyLen->setCurrentIndex(m_DlgParaSetStruct[nIndex][19].InitVal.lval);

        //往静态控件中写入参数缺省值
        //码速率
        ui->lineEdit_CodeRate->setText(QString::number(m_DlgParaSetStruct[nIndex][1].InitVal.lval));
        ui->label_CodeRate->setText("码速率[" + QString::number(m_DlgParaSetStruct[0][1].MinVal.lmin) +
                "~" + QString::number(m_DlgParaSetStruct[0][1].MaxVal.lmax) + "]");
        //前补序列
        ui->lineEdit_BeforeFillCode->setText(QString::number(m_DlgParaSetStruct[nIndex][4].InitVal.lval, 16).toUpper());
        //后补序列
        ui->lineEdit_LateFillCode->setText(QString::number(m_DlgParaSetStruct[nIndex][5].InitVal.lval, 16).toUpper());
        //执行序列
        ui->lineEdit_ExeCode->setText(QString::number(m_DlgParaSetStruct[nIndex][6].InitVal.lval, 16).toUpper());
        //空闲序列
        ui->lineEdit_FreeCode->setText(QString::number(m_DlgParaSetStruct[nIndex][7].InitVal.lval, 16).toUpper());
        //前补序列长
        ui->lineEdit_BeforeFillLen->setText(QString::number(m_DlgParaSetStruct[nIndex][8].InitVal.lval));
        ui->label_BeforeFillLen->setText("前补序列长度\n[" + QString::number(m_DlgParaSetStruct[0][8].MinVal.lmin) +
                "~" + QString::number(m_DlgParaSetStruct[0][8].MaxVal.lmax) + "]");
        //后补序列长
        ui->lineEdit_LateFillLen->setText(QString::number(m_DlgParaSetStruct[nIndex][9].InitVal.lval));
        ui->label_LateFillLen->setText("后补序列长度\n[" + QString::number(m_DlgParaSetStruct[0][9].MinVal.lmin) +
                "~" + QString::number(m_DlgParaSetStruct[0][9].MaxVal.lmax) + "]");
        //执行序列长
        ui->lineEdit_ExeCodeLen->setText(QString::number(m_DlgParaSetStruct[nIndex][10].InitVal.lval));
        ui->label_ExeCodeLen->setText("执行序列长度\n[" + QString::number(m_DlgParaSetStruct[0][10].MinVal.lmin) +
                "~" + QString::number(m_DlgParaSetStruct[0][10].MaxVal.lmax) + "]");
        //保护间隙
        ui->lineEdit_BufferGap->setText(QString::number(m_DlgParaSetStruct[nIndex][11].InitVal.lval));
        ui->label_BufferGap->setText("保护间隙\n[" + QString::number(m_DlgParaSetStruct[0][11].MinVal.lmin) +
                "~" + QString::number(m_DlgParaSetStruct[0][11].MaxVal.lmax) + "]");
        //应急填充长度
        ui->lineEdit_EmergencyFillLen->setText(QString::number(m_DlgParaSetStruct[nIndex][12].InitVal.lval));
        ui->label_EmergencyFillLen->setText("应急填充长度\n[" + QString::number(m_DlgParaSetStruct[0][12].MinVal.lmin) +
                "~" + QString::number(m_DlgParaSetStruct[0][12].MaxVal.lmax) + "]");
        //结尾序列图样
        ui->lineEdit_BeginPattern->setText(QString::number(m_DlgParaSetStruct[nIndex][15].InitVal.lval, 16).toUpper());
        //结尾序列图样
        ui->lineEdit_LatePattern->setText(QString::number(m_DlgParaSetStruct[nIndex][18].InitVal.lval, 16).toUpper());
        //加扰多项式
        ui->lineEdit_RandPoly->setText(QString::number(m_DlgParaSetStruct[nIndex][20].InitVal.lval, 16).toUpper());
        //结尾序列图样
        ui->lineEdit_RandPhase->setText(QString::number(m_DlgParaSetStruct[nIndex][21].InitVal.lval, 16).toUpper());
    }
    else if (nType == 1)
    {
        //码型
        if (m_MCSSSTCParam.MCSSSB2TCPara[nIndex].ucCodeStyle > 0)
            ui->comboBox_CodeType->setCurrentIndex(m_MCSSSTCParam.MCSSSB2TCPara[nIndex].ucCodeStyle - 1);
        //码率
        ui->lineEdit_CodeRate->setText(QString::number(m_MCSSSTCParam.MCSSSB2TCPara[nIndex].uiCodeRate));
        //遥控发送方式
        if (m_MCSSSTCParam.MCSSSB2TCPara[nIndex].ucSWaveMd > 0)
            ui->comboBox_TeleSendType->setCurrentIndex(m_MCSSSTCParam.MCSSSB2TCPara[nIndex].ucSWaveMd - 1);
        //工作期启动方式
        if (m_MCSSSTCParam.MCSSSB2TCPara[nIndex].ucTCWorkCyc > 0)
            ui->comboBox_StartingMethod->setCurrentIndex(m_MCSSSTCParam.MCSSSB2TCPara[nIndex].ucTCWorkCyc - 1);
        //前补序列
        ui->lineEdit_BeforeFillCode->setText(QString::number(m_MCSSSTCParam.MCSSSB2TCPara[nIndex].ucFCodeMinU, 16).toUpper());
        //后补序列
        ui->lineEdit_LateFillCode->setText(QString::number(m_MCSSSTCParam.MCSSSB2TCPara[nIndex].ucBCodeMinU, 16).toUpper());
        //执行序列
        ui->lineEdit_ExeCode->setText(QString::number(m_MCSSSTCParam.MCSSSB2TCPara[nIndex].ucECodeMinU, 16).toUpper());
        //前补序列长度
        ui->lineEdit_BeforeFillLen->setText(QString::number(m_MCSSSTCParam.MCSSSB2TCPara[nIndex].usFQueueLen));
        //后补序列长度
        ui->lineEdit_LateFillLen->setText(QString::number(m_MCSSSTCParam.MCSSSB2TCPara[nIndex].usBQueueLen));
        //执行序列长度
        ui->lineEdit_ExeCodeLen->setText(QString::number(m_MCSSSTCParam.MCSSSB2TCPara[nIndex].ucECodeLen));
        //保护间隙
        ui->lineEdit_BufferGap->setText(QString::number(m_MCSSSTCParam.MCSSSB2TCPara[nIndex].usTCProctC));
        //应急填充长度
        ui->lineEdit_EmergencyFillLen->setText(QString::number(m_MCSSSTCParam.MCSSSB2TCPara[nIndex].ucC8Data));
        //编码控制
        if (m_MCSSSTCParam.MCSSSB2TCPara[nIndex].ucBCHCode > 0)
            ui->comboBox_BCHEncode->setCurrentIndex(m_MCSSSTCParam.MCSSSB2TCPara[nIndex].ucBCHCode - 1);
        //起始序列控制
        if (m_MCSSSTCParam.MCSSSB2TCPara[nIndex].ucBCHBeginSeriesCtrl > 0)
            ui->comboBox_BeginCtrl->setCurrentIndex(m_MCSSSTCParam.MCSSSB2TCPara[nIndex].ucBCHBeginSeriesCtrl - 1);
        //起始序列图样
        ui->lineEdit_BeginPattern->setText(QString::number(m_MCSSSTCParam.MCSSSB2TCPara[nIndex].usBCHBeginSeries, 16).toUpper());
        //加扰使能
        if (m_MCSSSTCParam.MCSSSB2TCPara[nIndex].ucDisturboCtrl > 0)
            ui->comboBox_RandCtrl->setCurrentIndex(m_MCSSSTCParam.MCSSSB2TCPara[nIndex].ucDisturboCtrl - 1);
        //结尾序列控制
        if (m_MCSSSTCParam.MCSSSB2TCPara[nIndex].ucBCHEndCtrl > 0)
            ui->comboBox_LateCtrl->setCurrentIndex(m_MCSSSTCParam.MCSSSB2TCPara[nIndex].ucBCHEndCtrl - 1);
        //结尾序列图样
        ui->lineEdit_LatePattern->setText(QString::number(m_MCSSSTCParam.MCSSSB2TCPara[nIndex].dwlBCHEndSeries, 16).toUpper());
        //加扰多项式长度
        if (m_MCSSSTCParam.MCSSSB2TCPara[nIndex].ucDisturboLen > 0)
            ui->comboBox_RandPolyLen->setCurrentIndex(m_MCSSSTCParam.MCSSSB2TCPara[nIndex].ucDisturboLen - 1);
        //加扰多项式
        ui->lineEdit_RandPoly->setText(QString::number(m_MCSSSTCParam.MCSSSB2TCPara[nIndex].uiDisPoly, 16).toUpper());
        //加扰初相
        ui->lineEdit_RandPhase->setText(QString::number(m_MCSSSTCParam.MCSSSB2TCPara[nIndex].uiDisPhase, 16).toUpper());
        //空闲序列
        ui->lineEdit_FreeCode->setText(QString::number(m_MCSSSTCParam.MCSSSB2TCPara[nIndex].ucIndleCode, 16).toUpper());

        bool bTmp;
        bTmp=m_MCSSSTCParam.MCSSSB2TCPara[nIndex].ucC9FQueueCheck==1 ? true : false;
        ui->checkBox_5->setChecked (bTmp);
        bTmp=m_MCSSSTCParam.MCSSSB2TCPara[nIndex].ucC9BQueueCheck==1 ? true : false;
        ui->checkBox_6->setChecked(bTmp);

        bTmp=m_MCSSSTCParam.MCSSSB2TCPara[nIndex].ucC7FQueueCheck==1 ? true : false;
        ui->checkBox_7->setChecked(bTmp);
        bTmp=m_MCSSSTCParam.MCSSSB2TCPara[nIndex].ucC7BQueueCheck==1 ? true : false;
        ui->checkBox_8->setChecked(bTmp);
    }
}

void SS2BBE_TCParamSetGUIDlg::getParamTarget()
{
    bool ok;
    /***********************************码同步****************************/
    //码型
    m_MCSSSTCParam.MCSSSB2TCPara[m_nTargetNum].ucCodeStyle = ui->comboBox_CodeType->currentIndex() + 1;
    //码率
    m_MCSSSTCParam.MCSSSB2TCPara[m_nTargetNum].uiCodeRate = ui->lineEdit_CodeRate->text().toInt();
    //遥控发送方式
    m_MCSSSTCParam.MCSSSB2TCPara[m_nTargetNum].ucSWaveMd = ui->comboBox_TeleSendType->currentIndex() + 1;
    //工作期启动方式
    m_MCSSSTCParam.MCSSSB2TCPara[m_nTargetNum].ucTCWorkCyc = ui->comboBox_StartingMethod->currentIndex() + 1;
    //前补序列
    m_MCSSSTCParam.MCSSSB2TCPara[m_nTargetNum].ucFCodeMinU = ui->lineEdit_BeforeFillCode->text().toInt(&ok, 16);
    //后补序列
    m_MCSSSTCParam.MCSSSB2TCPara[m_nTargetNum].ucBCodeMinU = ui->lineEdit_LateFillCode->text().toInt(&ok, 16);
    //执行序列
    m_MCSSSTCParam.MCSSSB2TCPara[m_nTargetNum].ucECodeMinU = ui->lineEdit_ExeCode->text().toULongLong(&ok, 16);
    //空闲序列
    m_MCSSSTCParam.MCSSSB2TCPara[m_nTargetNum].ucIndleCode = ui->lineEdit_FreeCode->text().toInt(&ok, 16);
    //前补序列长度
    m_MCSSSTCParam.MCSSSB2TCPara[m_nTargetNum].usFQueueLen = ui->lineEdit_BeforeFillLen->text().toInt();
    //后补序列长度
    m_MCSSSTCParam.MCSSSB2TCPara[m_nTargetNum].usBQueueLen = ui->lineEdit_LateFillLen->text().toInt();
    //执行序列长度
    m_MCSSSTCParam.MCSSSB2TCPara[m_nTargetNum].ucECodeLen = ui->lineEdit_ExeCodeLen->text().toInt();
    //保护间隙
    m_MCSSSTCParam.MCSSSB2TCPara[m_nTargetNum].usTCProctC = ui->lineEdit_BufferGap->text().toInt();
    //应急填充长度
    m_MCSSSTCParam.MCSSSB2TCPara[m_nTargetNum].ucC8Data = ui->lineEdit_EmergencyFillLen->text().toInt();
    //编码控制
    m_MCSSSTCParam.MCSSSB2TCPara[m_nTargetNum].ucBCHCode = ui->comboBox_BCHEncode->currentIndex() + 1;
    //起始序列控制
    m_MCSSSTCParam.MCSSSB2TCPara[m_nTargetNum].ucBCHBeginSeriesCtrl = ui->comboBox_BeginCtrl->currentIndex() + 1;
    //起始序列图样
    m_MCSSSTCParam.MCSSSB2TCPara[m_nTargetNum].usBCHBeginSeries = ui->lineEdit_BeginPattern->text().toInt(&ok, 16);
    //加扰使能
    m_MCSSSTCParam.MCSSSB2TCPara[m_nTargetNum].ucDisturboCtrl = ui->comboBox_RandCtrl->currentIndex() + 1;
    //结尾序列控制
    m_MCSSSTCParam.MCSSSB2TCPara[m_nTargetNum].ucBCHEndCtrl = ui->comboBox_LateCtrl->currentIndex() + 1;
    //结尾序列图样
    m_MCSSSTCParam.MCSSSB2TCPara[m_nTargetNum].dwlBCHEndSeries = ui->lineEdit_LatePattern->text().toULongLong(&ok, 16);
    //加扰多项式长度
    m_MCSSSTCParam.MCSSSB2TCPara[m_nTargetNum].ucDisturboLen = ui->comboBox_RandPolyLen->currentIndex() + 1;
    //加扰多项式
    m_MCSSSTCParam.MCSSSB2TCPara[m_nTargetNum].uiDisPoly = ui->lineEdit_RandPoly->text().toUInt(&ok, 16);
    //加扰初相
    m_MCSSSTCParam.MCSSSB2TCPara[m_nTargetNum].uiDisPhase = ui->lineEdit_RandPhase->text().toUInt(&ok, 16);

    bool bTmp;
    bTmp=ui->checkBox_5->checkState();
    m_MCSSSTCParam.MCSSSB2TCPara[m_nTargetNum].ucC9FQueueCheck=bTmp==true?1:2;

    bTmp=ui->checkBox_6->checkState();
    m_MCSSSTCParam.MCSSSB2TCPara[m_nTargetNum].ucC9BQueueCheck=bTmp==true?1:2;

    bTmp=ui->checkBox_7->checkState();
    m_MCSSSTCParam.MCSSSB2TCPara[m_nTargetNum].ucC7FQueueCheck=bTmp==true?1:2;

    bTmp=ui->checkBox_8->checkState();
    m_MCSSSTCParam.MCSSSB2TCPara[m_nTargetNum].ucC7BQueueCheck=bTmp==true?1:2;

}

DWORDLONG SS2BBE_TCParamSetGUIDlg::StrToSynCode(const char * pStr)
{
    char c,C;
    const char*  p;
    DWORDLONG dwlValue=0;
    char   DigVal=0;
    USHORT nDigCount=0,MAX_DIGITS=16; //The synchronizing code should be not longer  than 16 digitals

    p=pStr;     //Point to the first char of the string
    c = *p++;   // read the first char
    while ((c!=NULL) && (nDigCount<=MAX_DIGITS))//  Not the terminator of a string && total digitals is less than 16
    {
        if(isspace((int)(unsigned char)c))    c = *p++;// skip whitespace
        else if(c>='0'&&c<='9')                        //the char is a number between 1 and 9
        {
            DigVal=c-'0';
            dwlValue=dwlValue*16+DigVal;               //Accumulate the value of the hexal number so far to the current digital
            nDigCount++;
            c = *p++;
        }
        else if((C=toupper(c))>='A'&&C<='F')           //the char is a letter could be a digital of a hexal number
        {
            DigVal=C-'A'+10;
            dwlValue=dwlValue*16+DigVal;               //Accumulate the value of the hexal number so far to the current digital
            nDigCount++;
            c = *p++;
        }
        else    break;                                 // An invalid char that is neither whitespace nor a hexal digital is encounted
        // Conclude the scanning process
    }
    return dwlValue;
}




void SS2BBE_TCParamSetGUIDlg::on_radioButton_Target1_clicked()
{
    if (CheckPara() != 1)
    {
        set_radioButton_Target();
        return;
    }
    GetPara();
    //界面显示对应的参数数据
    m_nTargetNum = 0;
    //int nType = ui->pushButton_Set->isEnabled() ? 0 : 1;
    showTargetParam(0, 1);
}

void SS2BBE_TCParamSetGUIDlg::on_radioButton_Target2_clicked()
{
    if (CheckPara() != 1)
    {
        set_radioButton_Target();
        return;
    }
    GetPara();
    m_nTargetNum = 1;
    //int nType = ui->pushButton_Set->isEnabled() ? 0 : 1;
    showTargetParam(1, 1);
}

void SS2BBE_TCParamSetGUIDlg::on_radioButton_Target3_clicked()
{
    if (CheckPara() != 1)
    {
        set_radioButton_Target();
        return;
    }
    GetPara();
    m_nTargetNum = 2;
    //int nType = ui->pushButton_Set->isEnabled() ? 0 : 1;
    showTargetParam(2, 1);
}

void SS2BBE_TCParamSetGUIDlg::on_radioButton_Target4_clicked()
{
    if (CheckPara() != 1)
    {
        set_radioButton_Target();
        return;
    }
    GetPara();
    m_nTargetNum = 3;
    //int nType = ui->pushButton_Set->isEnabled() ? 0 : 1;
    showTargetParam(3, 1);
}

int SS2BBE_TCParamSetGUIDlg::getSharedMemoryTargetNum()
{
    DWORD stLen = sizeof(SMCU2::MCSSysTarget4StatusStruct);
    if(m_pStateMgr != NULL)
        m_pMCSTargetSysStatus  = (SMCU2::MCSSysTarget4StatusStruct*)m_pStateMgr->FindOneItem(m_CommTargetID, stLen);

    if (m_pMCSTargetSysStatus != NULL)
        return m_pMCSTargetSysStatus->MCSSysStatu.tParams.ucTargetNum;

    return 0;
}

void SS2BBE_TCParamSetGUIDlg::showRadioButton()
{
    int nNum = getSharedMemoryTargetNum();
    if (m_currentNumber_usedByParam == nNum)
        return;
    m_currentNumber_usedByParam = nNum;
    m_nTargetNum = 0;
    set_radioButton_Target();
    ui->radioButton_Target1->hide();
    ui->radioButton_Target2->hide();
    ui->radioButton_Target3->hide();
    ui->radioButton_Target4->hide();
    switch(nNum - 1)
    {
    case 1:
        ui->radioButton_Target1->show();
        ui->radioButton_Target2->show();
        break;
    case 2:
        ui->radioButton_Target1->show();
        ui->radioButton_Target2->show();
        ui->radioButton_Target3->show();
        break;
    case 3:
        ui->radioButton_Target1->show();
        ui->radioButton_Target2->show();
        ui->radioButton_Target3->show();
        ui->radioButton_Target4->show();
        break;
    default:
        ui->radioButton_Target1->show();
        break;
    }
}
//遥控转发方式
void SS2BBE_TCParamSetGUIDlg::on_comboBox_TeleSendType_currentIndexChanged(int index)
{
    if(ui->pushButton_Modify->text() == "更改")
        return ;
    if(index==0){
        ui->comboBox_StartingMethod->setEnabled(true);
        ui->comboBox_StartingMethod->setCurrentIndex(ui->comboBox_StartingMethod->currentIndex());
    }
    else{
        ui->comboBox_StartingMethod->setEnabled(false);
        ui->comboBox_StartingMethod->setCurrentIndex(0);

    }

}

//编码控制

void SS2BBE_TCParamSetGUIDlg::on_comboBox_BCHEncode_currentIndexChanged(int index)
{
    if(ui->pushButton_Modify->text() == "更改")
        return ;
    if(index==0)
    {
        ui->comboBox_BeginCtrl->setEnabled(false);
        ui->comboBox_LateCtrl->setEnabled(false);
        ui->lineEdit_BeginPattern->setEnabled(false);
        ui->lineEdit_LatePattern->setEnabled(false);
    }
    else
    {
        ui->comboBox_BeginCtrl->setEnabled(true);
        ui->comboBox_LateCtrl->setEnabled(true);

    }
    on_comboBox_LateCtrl_currentIndexChanged(ui->comboBox_LateCtrl->currentIndex());
    on_comboBox_BeginCtrl_currentIndexChanged(ui->comboBox_BeginCtrl->currentIndex());

}
//加扰控制
void SS2BBE_TCParamSetGUIDlg::on_comboBox_RandCtrl_currentIndexChanged(int index)
{
    if(ui->pushButton_Modify->text() == "更改")
        return ;
    if(index==0)
    {
        ui->comboBox_RandPolyLen->setEnabled(false);
        ui->lineEdit_RandPoly->setEnabled(false);
        ui->lineEdit_RandPhase->setEnabled(false);
    }
    else
    {
        ui->comboBox_RandPolyLen->setEnabled(true);
        ui->lineEdit_RandPoly->setEnabled(true);
        ui->lineEdit_RandPhase->setEnabled(true);
    }

}
//起始序列控制
void SS2BBE_TCParamSetGUIDlg::on_comboBox_BeginCtrl_currentIndexChanged(int index)
{
    if(ui->pushButton_Modify->text() == "更改")
        return ;
    if(index==1 && ui->comboBox_BCHEncode->currentIndex()==1)
    {

        ui->lineEdit_BeginPattern->setEnabled(true);

    }
    else
    {
        ui->lineEdit_BeginPattern->setEnabled(false);

    }

}
//结尾序列控制
void SS2BBE_TCParamSetGUIDlg::on_comboBox_LateCtrl_currentIndexChanged(int index)
{
    if(ui->pushButton_Modify->text() == "更改")
        return ;
    if(index==1 && ui->comboBox_BCHEncode->currentIndex() == 1)
    {

        ui->lineEdit_LatePattern->setEnabled(true);
    }
    else
    {
        ui->lineEdit_LatePattern->setEnabled(false);
    }
}
void SS2BBE_TCParamSetGUIDlg::updateRadioButton()
{
    if(!m_pCommFunc)return;
    if (m_currentNumber_usedByMacro == m_pCommFunc->getTargetNum())
        return;
    m_currentNumber_usedByMacro = m_pCommFunc->getTargetNum();
    m_nTargetNum = 0;
    set_radioButton_Target();
    ui->radioButton_Target1->hide();
    ui->radioButton_Target2->hide();
    ui->radioButton_Target3->hide();
    ui->radioButton_Target4->hide();
    switch( m_pCommFunc->getTargetNum() - 1)
    {
    case 0:
        ui->radioButton_Target1->show();
        break;
    case 1:
        ui->radioButton_Target1->show();
        ui->radioButton_Target2->show();
        break;
    case 2:
        ui->radioButton_Target1->show();
        ui->radioButton_Target2->show();
        ui->radioButton_Target3->show();
        break;
    case 3:
        ui->radioButton_Target1->show();
        ui->radioButton_Target2->show();
        ui->radioButton_Target3->show();
        ui->radioButton_Target4->show();
        break;
    default:
        break;
    }
}

void SS2BBE_TCParamSetGUIDlg::set_radioButton_Target()
{
    switch(m_nTargetNum)
    {
    case 0:
        ui->radioButton_Target1->setChecked(true);
        break;
    case 1:
        ui->radioButton_Target2->setChecked(true);
        break;
    case 2:
        ui->radioButton_Target3->setChecked(true);
        break;
    case 3:
        ui->radioButton_Target4->setChecked(true);
        break;
    default:
        ui->radioButton_Target1->setChecked(true);
    }
}

#pragma pack()

void SS2BBE_TCParamSetGUIDlg::on_lineEdit_ExeCode_textChanged(const QString &arg1)
{
    int len = arg1.toLatin1().size();
    if (len > 1)
        ui->lineEdit_ExeCodeLen->setText(QString::number(len * 4));
    else
        ui->lineEdit_ExeCodeLen->setText("8");
}
