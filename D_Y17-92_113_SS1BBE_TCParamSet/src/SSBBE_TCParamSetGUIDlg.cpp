#include "SSBBE_TCParamSetGUIDlg.h"
#include "ui_SSBBE_TCParamSetGUIDlg.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDateTime>
#include "../../Common/Log.h"
#include "../../Common/CommonFunction.h"
#include <QTimer>
#pragma pack(1)

extern quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
                     uchar ucSn);
SSBBE_TCParamSetGUIDlg::SSBBE_TCParamSetGUIDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SSBBE_TCParamSetGUIDlg)
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
    m_timer1 = new QTimer(this);
    m_targetNum_usedInMacro = -1;
    m_targetNum_usedInParam = -1;
    connect(m_timer1, SIGNAL(timeout()), this, SLOT(updateRadioButton()));
    setAttribute(Qt::WA_DeleteOnClose);
}

SSBBE_TCParamSetGUIDlg::~SSBBE_TCParamSetGUIDlg ()
{
    killTimer(m_TimerID);
    m_timer1->stop();
    delete m_timer1;
    delete ui;
}
int SSBBE_TCParamSetGUIDlg::msgBoxFunction(QMessageBox &msgBox, QString msg, BOOL ret)
{
    msgBox.setText(msg);
    msgBox.exec();
    return ret;
}

void SSBBE_TCParamSetGUIDlg::setText(const QString& strTitle, ParaStruct xmlsetText[5][60], QString MapMean[50][30], int MapNum[50], uchar Type, ILxTsspParameterMacro* pMacroObj)
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
void SSBBE_TCParamSetGUIDlg::InitDLG(int Type, ILxTsspParameterMacro* pMacroObj)
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
            DWORD stLen = sizeof(SMCU::MCSSSTCStatusStruct4);
            m_MCSSSTCStatus = (SMCU::MCSSSTCStatusStruct4*)m_pStateMgr->FindOneItem(m_StatusParaMCSSSTCID, stLen);
            if(m_MCSSSTCStatus == NULL || stLen != sizeof(SMCU::MCSSSTCStatusStruct4))
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
            memcpy(&m_MCSSSTCParam, byteArray.data(), sizeof(SMCU::MCSSSTCStatusStruct4));
        }
        ui->pushButton_Modify->setVisible(false);
        ui->pushButton_Set->setVisible(false);
        ui->lineEdit_ExeCodeLen->setDisabled(true);
        m_timer1->start(500);
        m_pCommFunc = CCommFunc::getInstance();
        //显示参数
        ShowPara(1);
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
    }
}

//设置输入格式
void SSBBE_TCParamSetGUIDlg::InitInput()
{
    //正整数
    QRegExp regExp("[0-9]{1,8}");
    //码速率
    QRegExp regExp_Len("[0-9]{0,4}");
    ui->lineEdit_CodeRate->setValidator(new QRegExpValidator(regExp_Len, this));
    //前补序列长度
    ui->lineEdit_BeforeFillLen->setValidator(new QRegExpValidator(regExp_Len, this));
    //执行序列长度
    ui->lineEdit_ExeCodeLen->setValidator(new QRegExpValidator(regExp, this));
    //后补序列长度
    ui->lineEdit_LateFillLen->setValidator(new QRegExpValidator(regExp_Len, this));

    //保护间隙
    QRegExp regExp_BufferGap("[0-9]{3,5}");
    ui->lineEdit_BufferGap->setValidator(new QRegExpValidator(regExp_BufferGap, this));
    //应急填充长度
    QRegExp regExp_EmergencyFillLen("[0-9]{0,3}");
    ui->lineEdit_EmergencyFillLen->setValidator(new QRegExpValidator(regExp_EmergencyFillLen, this));

    //字母数字
    QRegExp regExp2("[A-Fa-f0-9]{1,8}");
    QRegExp regExp1("[A-Fa-f0-9]{1,2}");
    QRegExp regExp3("[A-Fa-f0-9]{1,4}");
    //前补序列
    ui->lineEdit_BeforeFillCode->setValidator(new QRegExpValidator(regExp1, this));
    //后补序列
    ui->lineEdit_LateFillCode->setValidator(new QRegExpValidator(regExp1, this));
    //空闲序列
    ui->lineEdit_FreeCode->setValidator(new QRegExpValidator(regExp1, this));
    //起始序列图样
    ui->lineEdit_BeginPattern->setValidator(new QRegExpValidator(regExp3, this));
    //结尾序列图样
    QRegExp regExpHex16("[A-Fa-f0-9]{1,16}");
    ui->lineEdit_LatePattern->setValidator(new QRegExpValidator(regExpHex16, this));
    //执行序列
    ui->lineEdit_ExeCode->setValidator(new QRegExpValidator(regExpHex16, this));
    //加扰多项式
    ui->lineEdit_RandPoly->setValidator(new QRegExpValidator(regExp2, this));
    //加扰初相
    ui->lineEdit_RandPhase->setValidator(new QRegExpValidator(regExp2, this));

}

void SSBBE_TCParamSetGUIDlg::ShowControls()
{
    showTargetParam(0, 0);
    //默认选择目标1
    ui->radioButton_Target1->setChecked(true);
}

//显示参数
void SSBBE_TCParamSetGUIDlg::ShowPara(int type)
{
    if(type == 0)       //参数设置
    {
        //码同步
        if(m_MCSSSTCStatus != NULL)
        {
            for (int nIndex = 0; nIndex < MAX_TARGET_COUNT; nIndex++)
            {
                m_MCSSSTCParam.SSTCParamStruct[nIndex] = m_MCSSSTCStatus->SSTCStatusStruct[nIndex].tParams;
            }

        }
        else
        {
            DWORD stLen = sizeof(SMCU::MCSSSTCStatusStruct4);
            if(m_pStateMgr != NULL)
                m_MCSSSTCStatus  = (SMCU::MCSSSTCStatusStruct4*)m_pStateMgr->FindOneItem(m_StatusParaMCSSSTCID, stLen);
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
void SSBBE_TCParamSetGUIDlg::SetFrameLenRange(int iWordLen)
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

void SSBBE_TCParamSetGUIDlg::SetObjInfo(TLxTsspObjectInfo ObjectInfo)
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
    ITEM *pItemCom = m_DeviceMap.getItem(COMM_SS1BBE_NAME);
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
void SSBBE_TCParamSetGUIDlg::EnabledControls(bool bFlag)
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
int SSBBE_TCParamSetGUIDlg::SaveToMacro(ILxTsspParameterMacro* pMacroObj)
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
void SSBBE_TCParamSetGUIDlg::GetPara()
{
    getParamTarget();
}

//验证参数范围
int SSBBE_TCParamSetGUIDlg::CheckPara()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(MODULENAME);
    /***********************************码同步****************************/
    //码速率
    if((ui->lineEdit_CodeRate->text().toInt()) < m_DlgParaSetStruct[0][1].MinVal.lmin
            || (ui->lineEdit_CodeRate->text().toInt()) > m_DlgParaSetStruct[0][1].MaxVal.lmax)
    {
        msgBox.setText("码速率正确范围\n:[" + QString::number(m_DlgParaSetStruct[0][1].MinVal.lmin) +
                "～" + QString::number(m_DlgParaSetStruct[0][1].MaxVal.lmax) + "bps]，请重新输入!");
        msgBox.exec();
        return -1;
    }
    //前补序列长度
    if((ui->lineEdit_BeforeFillLen->text().toUInt()) < m_DlgParaSetStruct[0][8].MinVal.lmin
            || (ui->lineEdit_BeforeFillLen->text().toUInt()) > m_DlgParaSetStruct[0][8].MaxVal.lmax)
    {
        msgBox.setText("前补序列长度正确范围\n:[" + QString::number(m_DlgParaSetStruct[0][8].MinVal.lmin) +
                "～" + QString::number(m_DlgParaSetStruct[0][8].MaxVal.lmax ) + "]，请重新输入!");
        msgBox.exec();
        return -1;
    }
    //后补序列长度
    if((ui->lineEdit_LateFillLen->text().toUInt()) < m_DlgParaSetStruct[0][9].MinVal.lmin
            || (ui->lineEdit_LateFillLen->text().toUInt()) > m_DlgParaSetStruct[0][9].MaxVal.lmax)
    {
        msgBox.setText("后补序列长度正确范围\n:[" + QString::number(m_DlgParaSetStruct[0][9].MinVal.lmin)+
                "～"+QString::number(m_DlgParaSetStruct[0][9].MaxVal.lmax) + "]，请重新输入!");
        msgBox.exec();
        return -1;
    }
    //执行序列长度
    if((ui->lineEdit_ExeCodeLen->text().toUInt()) < m_DlgParaSetStruct[0][10].MinVal.lmin
            || (ui->lineEdit_ExeCodeLen->text().toUInt()) > m_DlgParaSetStruct[0][10].MaxVal.lmax)
    {
        msgBox.setText("执行序列长度正确范围\n:[" + QString::number(m_DlgParaSetStruct[0][10].MinVal.lmin) +
                "～" + QString::number(m_DlgParaSetStruct[0][10].MaxVal.lmax) + "]，请重新输入!");
        msgBox.exec();
        return -1;
    }
    //保护间隙
    if((ui->lineEdit_BufferGap->text().toUInt()) < m_DlgParaSetStruct[0][11].MinVal.lmin
            || (ui->lineEdit_BufferGap->text().toUInt()) > m_DlgParaSetStruct[0][11].MaxVal.lmax)
    {
        msgBox.setText("保护间隙正确范围\n:[" + QString::number(m_DlgParaSetStruct[0][11].MinVal.lmin) +
                "～" + QString::number(m_DlgParaSetStruct[0][11].MaxVal.lmax) + "]，请重新输入!");
        msgBox.exec();
        return -1;
    }
    //应急填充长度
    if((ui->lineEdit_EmergencyFillLen->text().toUInt()) < m_DlgParaSetStruct[0][12].MinVal.lmin
            || (ui->lineEdit_EmergencyFillLen->text().toUInt()) > m_DlgParaSetStruct[0][12].MaxVal.lmax)
    {
        msgBox.setText("应急填充长度正确范围\n:[" + QString::number(m_DlgParaSetStruct[0][12].MinVal.lmin) +
                "～" + QString::number(m_DlgParaSetStruct[0][12].MaxVal.lmax) + "]，请重新输入!");
        msgBox.exec();
        return -1;
    }
    if(ui->lineEdit_BeforeFillCode->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"前补序列输入为空，请重新输入!", FALSE);
    }
    if(ui->lineEdit_LateFillCode->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"后补序列输入为空，请重新输入!", FALSE);
    }
    if(ui->lineEdit_ExeCode->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"执行序列输入为空，请重新输入!", FALSE);
    }
    if(ui->lineEdit_BeforeFillLen->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"前补序列长度输入为空，请重新输入!", FALSE);
    }
    if(ui->lineEdit_LateFillLen->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"后补序列长度输入为空，请重新输入!", FALSE);
    }
    if(ui->lineEdit_EmergencyFillLen->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"应急填充长度输入为空，请重新输入!", FALSE);
    }
    if(ui->lineEdit_FreeCode->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"空闲序列输入为空，请重新输入!", FALSE);
    }
    if(ui->lineEdit_BeginPattern->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"起始序列图样输入为空，请重新输入!", FALSE);
    }
    if(ui->lineEdit_LatePattern->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"结尾序列图样输入为空，请重新输入!", FALSE);
    }
    if(ui->lineEdit_RandPoly->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"加扰多项式输入为空，请重新输入!", FALSE);
    }
    if(ui->lineEdit_RandPhase->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"加扰初项输入为空，请重新输入!", FALSE);
    }
    int numOfByte = ui->comboBox_RandPolyLen->currentIndex() + 1;
    QString sTmp = ui->lineEdit_RandPoly->text();
    if(sTmp.size() > numOfByte * 2)
    {
        return msgBoxFunction(msgBox, QString("加扰多项式字符数应小于或等于%1，请重新输入!").arg(numOfByte * 2), FALSE);
    }
    sTmp = ui->lineEdit_RandPhase->text();
    if(sTmp.size() > numOfByte * 2)
    {
        return msgBoxFunction(msgBox, QString("加扰初项字符数应小于或等于%1，请重新输入!").arg(numOfByte * 2), FALSE);
    }

    return 1;
}

//定时器
void SSBBE_TCParamSetGUIDlg::timerEvent(QTimerEvent *event)
{
    ShowPara(0);
}

//更改参数按钮休息处理函数
void SSBBE_TCParamSetGUIDlg::on_pushButton_Modify_clicked()
{
    if(ui->pushButton_Modify->text() == "更改")
    {
        EnabledControls(true);
        killTimer(m_TimerID);
        on_comboBox_BCHEncode_currentIndexChanged(ui->comboBox_BCHEncode->currentIndex());
        on_comboBox_RandCtrl_currentIndexChanged(ui->comboBox_RandCtrl->currentIndex());
        on_comboBox_TeleSendType_currentIndexChanged(ui->comboBox_TeleSendType->currentIndex());
        return;
    }

    if(ui->pushButton_Modify->text() == "恢复")
    {
        EnabledControls(false);
        m_TimerID = startTimer(500);
        return;
    }
}

void SSBBE_TCParamSetGUIDlg::on_pushButton_Set_clicked()
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
                struct{USHORT tmp;
                       SMCU::MCSSSTCParamStruct m_MCSSSTCParam;
                      }param[MAX_TARGET_COUNT];
            }data;
            data.target = 0x02 | 0x04 | 0x08 | 0x10;
            for(int i = 0; i < MAX_TARGET_COUNT; ++i)
            {
                data.param[i].m_MCSSSTCParam = m_MCSSSTCParam.SSTCParamStruct[i];
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

void SSBBE_TCParamSetGUIDlg::closeEvent(QCloseEvent *event)
{
    event->accept();
}

QSize SSBBE_TCParamSetGUIDlg::sizeHint() const
{
    return QSize(this->width(), this->height());
}

/*
//副帧方式响应函数
void SSBBE_TCParamSetGUIDlg::on_comboBox_SubFrmType_currentIndexChanged(int index)
{
//    //副帧方式控件使能
//    if (ui->pushButton_Modify->text() != "更改" || iParaSetFlag != 0)
//    {
//        EnabledControlsSubFrmType(index);   //副帧方式控件使能
//    }
}

//根据副帧类型，使能副帧控件
void SSBBE_TCParamSetGUIDlg::EnabledControlsSubFrmType(int iSubFrm)
{
//    switch(iSubFrm)
//    {
//        case 0:     //无副帧
//            SubFrmEn(false);            //副帧通用
//            IDSubFrmEn(false);          //ID副帧
//            CycSubFrmEn(false);         //循环副帧
//            ReserveSubFrmEn(false);     //反码副帧
//            break;
//        case 1:     //ID副帧
//            SubFrmEn(true);             //副帧通用
//            IDSubFrmEn(true);           //ID副帧
//            CycSubFrmEn(false);         //循环副帧
//            ReserveSubFrmEn(false);     //反码副帧
//            break;
//        case 2:     //循环副帧comboBox_RandCtrl
//            SubFrmEn(true);             //副帧通用
//            IDSubFrmEn(false);          //ID副帧
//            CycSubFrmEn(true);          //循环副帧
//            ReserveSubFrmEn(false);     //反码副帧
//            break;
//        case 3:     //反码副帧
//            SubFrmEn(true);         //副帧通用
//            IDSubFrmEn(false);      //ID副帧
//            CycSubFrmEn(false);     //循环副帧
//            ReserveSubFrmEn(true);  //反码副帧
//            break;
//        default:
//            break;
//    }
}*/

void SSBBE_TCParamSetGUIDlg::SubFrmEn(bool bFlag)
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
void SSBBE_TCParamSetGUIDlg::IDSubFrmEn(bool bFlag)
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
void SSBBE_TCParamSetGUIDlg::CycSubFrmEn(bool bFlag)
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
void SSBBE_TCParamSetGUIDlg::ReserveSubFrmEn(bool bFlag)
{

}

void SSBBE_TCParamSetGUIDlg::SendMessage(char* pData, unsigned short  usLen, unsigned short usC, unsigned short usTID)
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

void SSBBE_TCParamSetGUIDlg::showTargetParam(int nIndex, int nType)
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
        ui->lineEdit_BeforeFillCode->setText(QString::number(m_DlgParaSetStruct[nIndex][4].InitVal.lval,16).toUpper());
        //后补序列
        ui->lineEdit_LateFillCode->setText(QString::number(m_DlgParaSetStruct[nIndex][5].InitVal.lval,16).toUpper());
        //执行序列
        ui->lineEdit_ExeCode->setText(QString::number(m_DlgParaSetStruct[nIndex][6].InitVal.lval,16).toUpper());
        //空闲序列
        ui->lineEdit_FreeCode->setText(QString::number(m_DlgParaSetStruct[nIndex][7].InitVal.lval,16).toUpper());
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
        ui->lineEdit_BeginPattern->setText(QString::number(m_DlgParaSetStruct[nIndex][15].InitVal.lval,16).toUpper());
        //结尾序列图样
        ui->lineEdit_LatePattern->setText(QString::number(m_DlgParaSetStruct[nIndex][18].InitVal.lval,16).toUpper());
        //加扰多项式
        ui->lineEdit_RandPoly->setText(QString::number(m_DlgParaSetStruct[nIndex][20].InitVal.lval,16).toUpper());
        //结尾序列图样
        ui->lineEdit_RandPhase->setText(QString::number(m_DlgParaSetStruct[nIndex][21].InitVal.lval,16).toUpper());
    }
    else if (nType == 1)
    {
        //码型
        ui->comboBox_CodeType->setCurrentIndex(m_MCSSSTCParam.SSTCParamStruct[nIndex].ucCodeStyle - 1);
        //码率
        ui->lineEdit_CodeRate->setText(QString::number(m_MCSSSTCParam.SSTCParamStruct[nIndex].uiCodeRate));
        //遥控发送方式
        ui->comboBox_TeleSendType->setCurrentIndex(m_MCSSSTCParam.SSTCParamStruct[nIndex].ucSWaveMd - 1);
        //工作期启动方式
        ui->comboBox_StartingMethod->setCurrentIndex(m_MCSSSTCParam.SSTCParamStruct[nIndex].ucTCWorkCyc - 1);
        //前补序列
        ui->lineEdit_BeforeFillCode->setText(QString::number(m_MCSSSTCParam.SSTCParamStruct[nIndex].ucFCodeMinU,16).toUpper());
        //后补序列
        ui->lineEdit_LateFillCode->setText(QString::number(m_MCSSSTCParam.SSTCParamStruct[nIndex].ucBCodeMinU,16).toUpper());
        //执行序列
        ui->lineEdit_ExeCode->setText(QString::number(m_MCSSSTCParam.SSTCParamStruct[nIndex].ucECodeMinU,16).toUpper());
        //前补序列长度
        ui->lineEdit_BeforeFillLen->setText(QString::number(m_MCSSSTCParam.SSTCParamStruct[nIndex].usFQueueLen));
        //后补序列长度
        ui->lineEdit_LateFillLen->setText(QString::number(m_MCSSSTCParam.SSTCParamStruct[nIndex].usBQueueLen));
        //执行序列长度
        ui->lineEdit_ExeCodeLen->setText(QString::number(m_MCSSSTCParam.SSTCParamStruct[nIndex].ucECodeLen));
        //保护间隙
        ui->lineEdit_BufferGap->setText(QString::number(m_MCSSSTCParam.SSTCParamStruct[nIndex].usTCProctC));
        //应急填充长度
        ui->lineEdit_EmergencyFillLen->setText(QString::number(m_MCSSSTCParam.SSTCParamStruct[nIndex].ucC8Data));
        //编码控制
        ui->comboBox_BCHEncode->setCurrentIndex(m_MCSSSTCParam.SSTCParamStruct[nIndex].ucBCHCode - 1);
        //起始序列控制
        ui->comboBox_BeginCtrl->setCurrentIndex(m_MCSSSTCParam.SSTCParamStruct[nIndex].ucBCHBeginSeriesCtrl - 1);
        //起始序列图样
        ui->lineEdit_BeginPattern->setText(QString::number(m_MCSSSTCParam.SSTCParamStruct[nIndex].usBCHBeginSeries, 16).toUpper());
        //加扰使能
        ui->comboBox_RandCtrl->setCurrentIndex(m_MCSSSTCParam.SSTCParamStruct[nIndex].ucDisturboCtrl - 1);
        //结尾序列控制
        ui->comboBox_LateCtrl->setCurrentIndex(m_MCSSSTCParam.SSTCParamStruct[nIndex].ucBCHEndCtrl - 1);
        //结尾序列图样
        ui->lineEdit_LatePattern->setText(QString::number(m_MCSSSTCParam.SSTCParamStruct[nIndex].dwlBCHEndSeries, 16).toUpper());
        //加扰多项式长度
        ui->comboBox_RandPolyLen->setCurrentIndex(m_MCSSSTCParam.SSTCParamStruct[nIndex].ucDisturboLen - 1);
        //加扰多项式
        ui->lineEdit_RandPoly->setText(QString::number(m_MCSSSTCParam.SSTCParamStruct[nIndex].uiDisPoly, 16).toUpper());
        //加扰初相
        ui->lineEdit_RandPhase->setText(QString::number(m_MCSSSTCParam.SSTCParamStruct[nIndex].uiDisPhase, 16).toUpper());
        //空闲序列
        ui->lineEdit_FreeCode->setText(QString::number(m_MCSSSTCParam.SSTCParamStruct[nIndex].ucIndleCode, 16).toUpper());

        bool bTmp;
        bTmp=m_MCSSSTCParam.SSTCParamStruct[nIndex].ucC9FQueueCheck==1?true:false;
        ui->checkBox_5->setChecked (bTmp);
        bTmp=m_MCSSSTCParam.SSTCParamStruct[nIndex].ucC9BQueueCheck==1?true:false;
        ui->checkBox_6->setChecked(bTmp);

        bTmp=m_MCSSSTCParam.SSTCParamStruct[nIndex].ucC7FQueueCheck==1?true:false;
        ui->checkBox_7->setChecked(bTmp);
        bTmp=m_MCSSSTCParam.SSTCParamStruct[nIndex].ucC7BQueueCheck==1?true:false;
        ui->checkBox_8->setChecked(bTmp);
    }
}

void SSBBE_TCParamSetGUIDlg::getParamTarget()
{
    bool ok;
    /***********************************码同步****************************/
    //码型
    m_MCSSSTCParam.SSTCParamStruct[m_nTargetNum].ucCodeStyle = ui->comboBox_CodeType->currentIndex() + 1;
    //码率
    m_MCSSSTCParam.SSTCParamStruct[m_nTargetNum].uiCodeRate = ui->lineEdit_CodeRate->text().toInt();
    //遥控发送方式
    m_MCSSSTCParam.SSTCParamStruct[m_nTargetNum].ucSWaveMd = ui->comboBox_TeleSendType->currentIndex() + 1;
    //工作期启动方式
    m_MCSSSTCParam.SSTCParamStruct[m_nTargetNum].ucTCWorkCyc = ui->comboBox_StartingMethod->currentIndex() + 1;
    //前补序列
    m_MCSSSTCParam.SSTCParamStruct[m_nTargetNum].ucFCodeMinU = ui->lineEdit_BeforeFillCode->text().toInt(&ok, 16);
    //后补序列
    m_MCSSSTCParam.SSTCParamStruct[m_nTargetNum].ucBCodeMinU = ui->lineEdit_LateFillCode->text().toInt(&ok, 16);
    //执行序列
    m_MCSSSTCParam.SSTCParamStruct[m_nTargetNum].ucECodeMinU = ui->lineEdit_ExeCode->text().toULongLong(&ok, 16);
    //空闲序列
    m_MCSSSTCParam.SSTCParamStruct[m_nTargetNum].ucIndleCode = ui->lineEdit_FreeCode->text().toInt(&ok, 16);
    //前补序列长度
    m_MCSSSTCParam.SSTCParamStruct[m_nTargetNum].usFQueueLen = ui->lineEdit_BeforeFillLen->text().toInt();
    //后补序列长度
    m_MCSSSTCParam.SSTCParamStruct[m_nTargetNum].usBQueueLen = ui->lineEdit_LateFillLen->text().toInt();
    //执行序列长度
    m_MCSSSTCParam.SSTCParamStruct[m_nTargetNum].ucECodeLen = ui->lineEdit_ExeCodeLen->text().toInt();
    //保护间隙
    m_MCSSSTCParam.SSTCParamStruct[m_nTargetNum].usTCProctC = ui->lineEdit_BufferGap->text().toInt();
    //应急填充长度
    m_MCSSSTCParam.SSTCParamStruct[m_nTargetNum].ucC8Data = ui->lineEdit_EmergencyFillLen->text().toInt();
    //编码控制
    m_MCSSSTCParam.SSTCParamStruct[m_nTargetNum].ucBCHCode = ui->comboBox_BCHEncode->currentIndex() + 1;
    //起始序列控制
    m_MCSSSTCParam.SSTCParamStruct[m_nTargetNum].ucBCHBeginSeriesCtrl = ui->comboBox_BeginCtrl->currentIndex() + 1;
    //起始序列图样
    m_MCSSSTCParam.SSTCParamStruct[m_nTargetNum].usBCHBeginSeries = ui->lineEdit_BeginPattern->text().toInt(&ok, 16);
    //加扰使能
    m_MCSSSTCParam.SSTCParamStruct[m_nTargetNum].ucDisturboCtrl = ui->comboBox_RandCtrl->currentIndex() + 1;
    //结尾序列控制
    m_MCSSSTCParam.SSTCParamStruct[m_nTargetNum].ucBCHEndCtrl = ui->comboBox_LateCtrl->currentIndex() + 1;
    //结尾序列图样
    m_MCSSSTCParam.SSTCParamStruct[m_nTargetNum].dwlBCHEndSeries = ui->lineEdit_LatePattern->text().toULongLong(&ok, 16);
    //加扰多项式长度
    m_MCSSSTCParam.SSTCParamStruct[m_nTargetNum].ucDisturboLen = ui->comboBox_RandPolyLen->currentIndex() + 1;
    //加扰多项式
    m_MCSSSTCParam.SSTCParamStruct[m_nTargetNum].uiDisPoly = ui->lineEdit_RandPoly->text().toUInt(&ok, 16);
    //加扰初相
    m_MCSSSTCParam.SSTCParamStruct[m_nTargetNum].uiDisPhase = ui->lineEdit_RandPhase->text().toUInt(&ok, 16);

    bool bTmp;
    bTmp=ui->checkBox_5->checkState();
    m_MCSSSTCParam.SSTCParamStruct[m_nTargetNum].ucC9FQueueCheck=bTmp==true ? 1 : 2;

    bTmp=ui->checkBox_6->checkState();
    m_MCSSSTCParam.SSTCParamStruct[m_nTargetNum].ucC9BQueueCheck=bTmp==true ? 1 : 2;

    bTmp=ui->checkBox_7->checkState();
    m_MCSSSTCParam.SSTCParamStruct[m_nTargetNum].ucC7FQueueCheck=bTmp==true ? 1 : 2;

    bTmp=ui->checkBox_8->checkState();
    m_MCSSSTCParam.SSTCParamStruct[m_nTargetNum].ucC7BQueueCheck=bTmp==true ? 1 : 2;

}

DWORDLONG SSBBE_TCParamSetGUIDlg::StrToSynCode(const char * pStr)
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


void SSBBE_TCParamSetGUIDlg::set_radioButton_Target()
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

void SSBBE_TCParamSetGUIDlg::on_radioButton_Target1_clicked()
{
    if(1 != CheckPara())
    {
        set_radioButton_Target();
        return;
    }
    GetPara();
    //界面显示对应的参数数据
    m_nTargetNum = 0;
    //int nType = ui->pushButton_Set->isEnabled()  ? 0 : 1;
    showTargetParam(0, 1);
}

void SSBBE_TCParamSetGUIDlg::on_radioButton_Target2_clicked()
{
    if(1 != CheckPara())
    {
        set_radioButton_Target();
        return;
    }
    GetPara();
    m_nTargetNum = 1;
    //int nType = ui->pushButton_Set->isEnabled() ? 0 : 1;
    showTargetParam(1, 1);
}

void SSBBE_TCParamSetGUIDlg::on_radioButton_Target3_clicked()
{
    if(1 != CheckPara())
    {
        set_radioButton_Target();
        return;
    }
    GetPara();
    m_nTargetNum = 2;
    //int nType = ui->pushButton_Set->isEnabled()  ? 0 : 1;
    showTargetParam(2, 1);
}

void SSBBE_TCParamSetGUIDlg::on_radioButton_Target4_clicked()
{
    if(1 != CheckPara())
    {
        set_radioButton_Target();
        return;
    }
    GetPara();
    m_nTargetNum = 3;
    //int nType = ui->pushButton_Set->isEnabled()  ? 0 : 1;
    showTargetParam(3, 1);
}

int SSBBE_TCParamSetGUIDlg::getSharedMemoryTargetNum()
{
    DWORD stLen = sizeof(SMCU::MCSSysTarget4StatusStruct);
    if(m_pStateMgr != NULL)
        m_pMCSTargetSysStatus  = (SMCU::MCSSysTarget4StatusStruct*)m_pStateMgr->FindOneItem(m_CommTargetID, stLen);

    if (m_pMCSTargetSysStatus != NULL)
        return m_pMCSTargetSysStatus->sysStatu.tParams.ucTargetNum;

    return 0;
}

void SSBBE_TCParamSetGUIDlg::showRadioButton()
{
    int nNum = getSharedMemoryTargetNum();
    if (m_targetNum_usedInParam == nNum)
        return;
    m_targetNum_usedInParam = nNum;
    m_nTargetNum = 0;
    set_radioButton_Target();
    ui->radioButton_Target1->hide();
    ui->radioButton_Target2->hide();
    ui->radioButton_Target3->hide();
    ui->radioButton_Target4->hide();
    switch(m_targetNum_usedInParam - 1)
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
void SSBBE_TCParamSetGUIDlg::on_comboBox_TeleSendType_currentIndexChanged(int index)
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

void SSBBE_TCParamSetGUIDlg::on_comboBox_BCHEncode_currentIndexChanged(int index)
{
    if(ui->pushButton_Modify->text() == "更改")
        return ;
    if(index==0){
        ui->comboBox_BeginCtrl->setEnabled(false);
        ui->comboBox_LateCtrl->setEnabled(false);
        ui->lineEdit_BeginPattern->setEnabled(false);
        ui->lineEdit_LatePattern->setEnabled(false);
    }
    else{
        ui->comboBox_BeginCtrl->setEnabled(true);
        ui->comboBox_LateCtrl->setEnabled(true);

    }
    on_comboBox_LateCtrl_currentIndexChanged(ui->comboBox_LateCtrl->currentIndex());
    on_comboBox_BeginCtrl_currentIndexChanged(ui->comboBox_BeginCtrl->currentIndex());

}
//加扰控制
void SSBBE_TCParamSetGUIDlg::on_comboBox_RandCtrl_currentIndexChanged(int index)
{
    if(ui->pushButton_Modify->text() == "更改")
        return ;
    if(index==0){
        ui->comboBox_RandPolyLen->setEnabled(false);
        ui->lineEdit_RandPoly->setEnabled(false);
        ui->lineEdit_RandPhase->setEnabled(false);
    }else
    {
        ui->comboBox_RandPolyLen->setEnabled(true);
        ui->lineEdit_RandPoly->setEnabled(true);
        ui->lineEdit_RandPhase->setEnabled(true);
    }

}
//起始序列控制
void SSBBE_TCParamSetGUIDlg::on_comboBox_BeginCtrl_currentIndexChanged(int index)
{
    if(ui->pushButton_Modify->text() == "更改")
        return ;
    if(index==1 && ui->comboBox_BCHEncode->currentIndex()==1){

        ui->lineEdit_BeginPattern->setEnabled(true);

    }
    else{
        ui->lineEdit_BeginPattern->setEnabled(false);

    }

}
//结尾序列控制
void SSBBE_TCParamSetGUIDlg::on_comboBox_LateCtrl_currentIndexChanged(int index)
{
    if(ui->pushButton_Modify->text() == "更改")
        return ;
    if(index==1 && ui->comboBox_BCHEncode->currentIndex()==1){

        ui->lineEdit_LatePattern->setEnabled(true);
    }
    else{
        ui->lineEdit_LatePattern->setEnabled(false);
    }
}

void SSBBE_TCParamSetGUIDlg::updateRadioButton()
{
    if(!m_pCommFunc) return;
    if (m_targetNum_usedInMacro == m_pCommFunc->getTargetNum())
        return;
    m_targetNum_usedInMacro = m_pCommFunc->getTargetNum();
    m_nTargetNum = 0;
    set_radioButton_Target();
    ui->radioButton_Target1->hide();
    ui->radioButton_Target2->hide();
    ui->radioButton_Target3->hide();
    ui->radioButton_Target4->hide();
    switch( m_targetNum_usedInMacro - 1)
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
#pragma pack()

void SSBBE_TCParamSetGUIDlg::on_lineEdit_ExeCode_textChanged(const QString &arg1)
{
    if (arg1.toLatin1().size() > 1)
        ui->lineEdit_ExeCodeLen->setText(QString::number(arg1.size()*4));
    else
        ui->lineEdit_ExeCodeLen->setText(QString::number(8));
}
