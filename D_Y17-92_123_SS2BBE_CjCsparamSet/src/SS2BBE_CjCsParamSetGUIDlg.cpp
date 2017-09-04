#include "SS2BBE_CjCsParamSetGUIDlg.h"
#include "ui_SS2BBE_CjCsParamSetGUIDlg.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDateTime>
#include "../../Common/Log.h"
#include <QDebug>
#include "../../Common/CommonFunction.h"
#include <QTimer>
#pragma pack(1)
extern quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
                     uchar ucSn);
SS2BBE_CjCsParamSetGUIDlg::SS2BBE_CjCsParamSetGUIDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SS2BBE_CjCsParamSetGUIDlg)
{
    ui->setupUi(this);
    m_pStateMgr = NULL;
    m_pILxTsspMessageManager = NULL;
    m_pILxTsspLogManager = NULL;
    m_pPublicInfo = NULL;
    m_pHAManager = NULL;
    m_targetNum_usedInMacro = -1;
    m_targetNum_usedInParam = -1;
    m_nTargetNum = 0;
    m_MCSSSCjStatus = NULL;
    m_pMCSTargetSysStatus = NULL;
    m_timer1 = new QTimer(this);
    connect(m_timer1, SIGNAL(timeout()), this, SLOT(updateRadioButton()));
    setAttribute(Qt::WA_DeleteOnClose);
}

SS2BBE_CjCsParamSetGUIDlg::~SS2BBE_CjCsParamSetGUIDlg()
{
    killTimer(m_TimerID);
    m_timer1->stop();
    delete m_timer1;
    delete ui;
}

void SS2BBE_CjCsParamSetGUIDlg::setText(const QString& strTitle, ParaStruct xmlsetText[3][60], QString MapMean[50][30], int MapNum[50], uchar Type, ILxTsspParameterMacro* pMacroObj)
{
    m_strTitle = strTitle;
    setWindowTitle(strTitle);

    memcpy(m_DlgParaSetStruct, xmlsetText, sizeof(ParaStruct) * 50 * 3);

    for(int i=0; i<50; i++)
    {
        for(int j=0; j<30; j++)
        {
            m_DlgMapMean[i][j] = MapMean[i][j];
        }
    }

    memset(DlgMapLineNum, 0, 50);
    for(int i=0; i<49; i++)
    {
        DlgMapLineNum[i] = MapNum[i];
    }
    //初始化界面
    InitDLG(Type, pMacroObj);
}

//初始化对话框
void SS2BBE_CjCsParamSetGUIDlg::InitDLG(int Type, ILxTsspParameterMacro* pMacroObj)
{
    iParaSetFlag = Type;
    //设置输入格式
    InitInput();
    //显示变量名字和范围
    ShowControls();

    if(Type == 0)//取当前参数
    {
        if(m_pStateMgr != NULL)
        {
            DWORD stLen = sizeof(SMCU2::MCSSSB2CjStatusStruct4);
            m_MCSSSCjStatus = (SMCU2::MCSSSB2CjStatusStruct4*)m_pStateMgr->FindOneItem(m_StatusParaMCSSSCjID, stLen);
            if(m_MCSSSCjStatus == NULL || stLen != sizeof(SMCU2::MCSSSB2CjStatusStruct4))
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
        if(pMacroObj != NULL)
        {
            QByteArray byteArray;
            pMacroObj->GetParameterBlock(m_ParaMCSSSCjID, byteArray);
            memcpy(&m_MCSSSCjParam, byteArray.data(), sizeof(SMCU2::MCSSSB2CjParamStruct4));
        }
        ui->pushButton_Modify->setVisible(false);
        ui->pushButton_Set->setVisible(false);
        on_comboBox_DisAmbigous_currentIndexChanged(ui->comboBox_DisAmbigous->currentIndex());
        on_comboBox_StationNum_currentIndexChanged(ui->comboBox_StationNum->currentIndex());
        on_comboBox_TargetType_currentIndexChanged(ui->comboBox_TargetType->currentIndex() );
        on_comboBox_DisForType_currentIndexChanged(ui->comboBox_DisForType->currentIndex());
        m_pCommFunc = CCommFunc::getInstance();
        m_timer1->start(500);
        //显示参数
        ShowPara(1);
    }
}

//设置输入格式
void SS2BBE_CjCsParamSetGUIDlg::InitInput()
{
    //正整数
    QRegExp regExp_DisFor("[0-9]{0,6}(\\.[0-9]{1,3})");
    QRegExp regExp_ResponsorZeroPos("[0-9]{0,7}(\\.[0-9]{1,3})");

    QRegExp regExp("[0-9]{0,8}(\\.[0-9]{1,8})");
    //应答机相对时延
    ui->lineEdit_ResponsorDelay->setValidator(new QRegExpValidator(regExp, this));

    //距离预报值
    ui->lineEdit_DisForValue->setValidator(new QRegExpValidator(regExp_DisFor, this));
    //应答机距离零值
    ui->lineEdit_ResponsorZero->setValidator(new QRegExpValidator(regExp_ResponsorZeroPos, this));

    //字母数字
    QRegExp regExp2("[A-Fa-f0-9]{1,2}");
    //目标代号
    ui->lineEdit_TargetCode->setValidator(new QRegExpValidator(regExp2, this));
    //地面测站代号[HEX]
    QRegExp regExpStationCode("[A-Fa-f0-9]{1,3}");
    ui->lineEdit_StationCode->setValidator(new QRegExpValidator(regExpStationCode, this));
}

void SS2BBE_CjCsParamSetGUIDlg::ShowControls()
{
    showTargetParam(0);
    //默认选择目标1
    ui->radioButton_Target1->setChecked(true);
}

//显示参数
void SS2BBE_CjCsParamSetGUIDlg::ShowPara(int type)
{
    if(type == 0)       //参数设置
    {
        if(m_MCSSSCjStatus != NULL)
        {
            for (int i = 0; i < MAX_TARGET_COUNT; i++)
                m_MCSSSCjParam.cjParam[i] = m_MCSSSCjStatus->cjStatus[i].tParams;
        }
        else
        {
            DWORD stLen = sizeof(SMCU2::MCSSSB2CjStatusStruct4);
            if(m_pStateMgr != NULL)
                m_MCSSSCjStatus  = (SMCU2::MCSSSB2CjStatusStruct4*)m_pStateMgr->FindOneItem(m_StatusParaMCSSSCjID, stLen);
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
    //显示目标
    showRadioButton();

    //    if (m_MCSSSCjStatus == NULL)
    //        return;
    //显示目标的数据所
    //    for (int i = 0; i < MAX_TARGET_COUNT; i++)
    //        showTargetParam(i, 1);
    showTargetParam(m_nTargetNum, 1);
}

//设置帧长范围
void SS2BBE_CjCsParamSetGUIDlg::SetFrameLenRange(int iWordLen)
{

}

void SS2BBE_CjCsParamSetGUIDlg::SetObjInfo(TLxTsspObjectInfo ObjectInfo)
{
    memcpy(&m_ObjectInfo, &ObjectInfo, sizeof(TLxTsspObjectInfo));
    //对象初始化
    //查找对象管理器
    LxTsspObjectManager* pObjectManager = LxTsspObjectManager::Instance();
    if(pObjectManager == NULL)
    {
        return;
    }

    m_pHAManager = pObjectManager->GetHAManager();

    bool bRet = m_DeviceMap.loadXML();
    if (!bRet)
    {
        QString strInfo = QString("load device map failed %1").arg(MODULENAME);
        CLog::addLog(strInfo);
        return;
    }

    ITEM *pItem = m_DeviceMap.getItem(MODULENAME);
    if (pItem != NULL && pItem->vSubItem.size() > 0)
    {
        SUBITEM& sItem = *(pItem->getSubItemByDeviceID_SN(m_ObjectInfo.usLocalDID, m_ObjectInfo.ucSN));
        m_usC = sItem.usCMD;
        m_usTID = sItem.usTID;
        /*************************框架**********************************/
        m_ParaMCSSSCjID = GetID(sItem.usStationID,
                                sItem.ucSubSysID, sItem.usDeviceID,sItem.usTID,
                                sItem.ucSN);
        /*************************框架**********************************/

        /*************************状态**********************************/
        m_StatusParaMCSSSCjID = GetID(sItem.usStationID,
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
void SS2BBE_CjCsParamSetGUIDlg::EnabledControls(bool bFlag)
{
    /*************************Group*************************/
    //目标类型
    ui->comboBox_TargetType->setEnabled(bFlag);
    //目标代号
    ui->lineEdit_TargetCode->setEnabled(bFlag);
    //地面测站代号
    ui->lineEdit_StationCode->setEnabled(bFlag);
    //是否使用测站代号
    ui->comboBox_UseJudge->setEnabled(bFlag);
    //工作站数目
    ui->comboBox_StationNum->setEnabled(bFlag);
    //上行测量帧CRC校验
    ui->comboBox_UpCrcCgheck->setEnabled(bFlag);
    //下行测量帧RP
    ui->comboBox_DownMeasRp->setEnabled(bFlag);
    //下行测量帧CRC校验
    ui->comboBox_DownCrcCheck->setEnabled(bFlag);
    //距离预报方式
    ui->comboBox_DisForType->setEnabled(bFlag);
    //星上通道号
    ui->comboBox_SatalliteChannel->setEnabled(bFlag);
    //距离解模糊
    ui->comboBox_DisAmbigous->setEnabled(bFlag);
    //距离预报值
    ui->lineEdit_DisForValue->setEnabled(bFlag);
    //应答机零值(单程)
    ui->lineEdit_ResponsorZero->setEnabled(bFlag);
    //应答机相对时延
    ui->lineEdit_ResponsorDelay->setEnabled(bFlag);
    /*************************Group*************************/

    /*************************帧同步器*************************/
    if (bFlag)
    {
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
int SS2BBE_CjCsParamSetGUIDlg::SaveToMacro(ILxTsspParameterMacro* pMacroObj)
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

        byteArray.resize(sizeof(m_MCSSSCjParam));
        memcpy(byteArray.data(), &m_MCSSSCjParam, sizeof(m_MCSSSCjParam));
        pMacroObj->UpdateParameterBlock(m_ParaMCSSSCjID, byteArray);

        //日志信息
        QString strLog;
        strLog += QString("%1: 保存宏成功！").arg(MODULENAME);
        CLog::addLog(strLog);
    }
}

//从界面取参数
void SS2BBE_CjCsParamSetGUIDlg::GetPara()
{
    getParamTarget();
}

//验证参数范围
int SS2BBE_CjCsParamSetGUIDlg::CheckPara()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(MODULENAME);
    //目标代号
    if(ui->lineEdit_TargetCode->text().isEmpty())
    {
        msgBox.setText("目标代号不能为空，请重新输入!");
        msgBox.exec();
        return -1;
    }
    //地面测站代号
    bool ok = false;
    int tmp = ui->lineEdit_StationCode->text().toInt(&ok, 16);
    if(ui->lineEdit_StationCode->text().isEmpty())
    {
        msgBox.setText("地面测站代号不能为空!");
        msgBox.exec();
        return -1;
    }
    if(!ok || tmp < 0 || tmp > 0x3FF)
    {
        msgBox.setText("地面测站代号范围0～3FF，请重新输入!");
        msgBox.exec();
        return -1;
    }
    //距离预报值
    double lfTmp = ui->lineEdit_DisForValue->text().toDouble(&ok);
    if(ui->lineEdit_DisForValue->text().isEmpty())
    {
        msgBox.setText("距离预报值不能为空!");
        msgBox.exec();
        return -1;
    }
    if(!ok || lfTmp < 0 || lfTmp > 450000)
    {
        msgBox.setText("距离预报值范围0～450000Km，请重新输入!");
        msgBox.exec();
        return -1;
    }
    //应答机距离零值
    lfTmp = ui->lineEdit_ResponsorZero->text().toDouble(&ok);
    if(ui->lineEdit_ResponsorZero->text().isEmpty())
    {
        msgBox.setText("应答机距离零值不能为空!");
        msgBox.exec();
        return -1;
    }
    if(!ok || lfTmp < 0 || lfTmp > 1000000)
    {
        msgBox.setText("应答机距离零值范围0～1000000m，请重新输入!");
        msgBox.exec();
        return -1;
    }
    //应答机相对时延
    if(ui->lineEdit_ResponsorDelay->text().isEmpty())
    {
        msgBox.setText("应答机相对时延不能为空，请重新输入!");
        msgBox.exec();
        return -1;
    }
    if(ui->lineEdit_ResponsorDelay->text().toDouble()*100>0xFFFFFFFF)
    {
        msgBox.setText(QString("应答机相对时延不能大于%1!").arg(0xFFFFFFFF/100));
        msgBox.exec();
        return -1;
    }
    return 1;
}

//定时器
void SS2BBE_CjCsParamSetGUIDlg::timerEvent(QTimerEvent *event)
{
    ShowPara(0);
}

//更改参数按钮休息处理函数
void SS2BBE_CjCsParamSetGUIDlg::on_pushButton_Modify_clicked()
{
    if(ui->pushButton_Modify->text() == "更改")
    {
        EnabledControls(true);
        killTimer(m_TimerID);
        on_comboBox_DisAmbigous_currentIndexChanged(ui->comboBox_DisAmbigous->currentIndex());
        on_comboBox_StationNum_currentIndexChanged(ui->comboBox_StationNum->currentIndex());
        on_comboBox_TargetType_currentIndexChanged(ui->comboBox_TargetType->currentIndex() );
        return;
    }

    if(ui->pushButton_Modify->text() == "恢复")
    {
        EnabledControls(false);
        m_TimerID = startTimer(500);
        return;
    }
}

void SS2BBE_CjCsParamSetGUIDlg::on_pushButton_Set_clicked()
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
            struct{USHORT target;//公共和目标  1-5，第一位公共
                   struct{USHORT tmp1;//占位
                          SMCU2::MCSSSB2CjParamStruct CjStatusStruct;//目标参数
                         }param[MAX_TARGET_COUNT];
                  }data;

            QByteArray byteArray;
            byteArray.resize(sizeof(data));
            data.target = 1 | 0x02 | 0x04 | 0x08 | 0x10;
            for(int i = 0; i < MAX_TARGET_COUNT; ++i)
            {
                data.param[i].CjStatusStruct = m_MCSSSCjParam.cjParam[i];
            }
            memcpy(byteArray.data(), &data, sizeof(data));
            SendMessage((char*)byteArray.data(), byteArray.size(), m_usC, m_usTID);
        }

        //日志信息
        QString strLog;
        strLog += QString("%1: 设置参数成功！").arg(MODULENAME);
        CLog::addLog(strLog);

        //不使能控件
        EnabledControls(false);
        m_TimerID = startTimer(500);
    }
}

void SS2BBE_CjCsParamSetGUIDlg::closeEvent(QCloseEvent *event)
{
    event->accept();
}

QSize SS2BBE_CjCsParamSetGUIDlg::sizeHint() const
{
    return QSize(this->width(), this->height());
}

void SS2BBE_CjCsParamSetGUIDlg::SubFrmEn(bool bFlag)
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
void SS2BBE_CjCsParamSetGUIDlg::IDSubFrmEn(bool bFlag)
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
void SS2BBE_CjCsParamSetGUIDlg::CycSubFrmEn(bool bFlag)
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
void SS2BBE_CjCsParamSetGUIDlg::ReserveSubFrmEn(bool bFlag)
{

}

void SS2BBE_CjCsParamSetGUIDlg::SendMessage(char* pData, unsigned short  usLen, unsigned short usC, unsigned short usTID)
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
    header.C = usC;

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

int SS2BBE_CjCsParamSetGUIDlg::getSharedMemoryTargetNum()
{
    DWORD stLen = sizeof(SMCU2::MCSSysTarget4StatusStruct);
    if(m_pStateMgr != NULL)
        m_pMCSTargetSysStatus  = (SMCU2::MCSSysTarget4StatusStruct*)m_pStateMgr->FindOneItem(m_CommTargetID, stLen);

    if (m_pMCSTargetSysStatus != NULL)
        return m_pMCSTargetSysStatus->MCSSysStatu.tParams.ucTargetNum;

    return 0;
}

void SS2BBE_CjCsParamSetGUIDlg::showRadioButton()
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
    switch(nNum - 1)
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

void SS2BBE_CjCsParamSetGUIDlg::showTargetParam(int nIndex, int nType)
{
    int i;
    /*******************************目标参数*********************************/
    if (nType == 0)
    {
        //目标类型
        for(i=0; i<DlgMapLineNum[nIndex * 9 + 0]; i++)
        {
            ui->comboBox_TargetType->addItem(m_DlgMapMean[nIndex * 9 + 0][i]);
            qDebug()<<m_DlgMapMean[nIndex * 9 + 0][i];
        }
        ui->comboBox_TargetType->setCurrentIndex(m_DlgParaSetStruct[nIndex][0].InitVal.lval);
        //目标代号
        ui->lineEdit_TargetCode->setText(QString::number(m_DlgParaSetStruct[nIndex][1].InitVal.lval, 16).toUpper());
        //地面测站代号
        ui->lineEdit_StationCode->setText(QString::number(m_DlgParaSetStruct[0][2].InitVal.lval, 16).toUpper());
        //是否使用测站代号
        for (i = 0; i < DlgMapLineNum[nIndex * 9 + 1]; i++)
        {
            ui->comboBox_UseJudge->addItem(m_DlgMapMean[nIndex * 9 + 1][i]);
        }
        ui->comboBox_UseJudge->setCurrentIndex(m_DlgParaSetStruct[nIndex][3].InitVal.lval);
        //工作站数目
        for (i = 0; i < DlgMapLineNum[nIndex * 9 + 2]; i++)
        {
            ui->comboBox_StationNum->addItem(m_DlgMapMean[nIndex * 9 + 2][i]);
        }
        ui->comboBox_StationNum->setCurrentIndex(m_DlgParaSetStruct[nIndex][4].InitVal.lval);
        //上行测量帧CRC校验
        for (i = 0; i < DlgMapLineNum[nIndex * 9 + 3]; i++)
        {
            ui->comboBox_UpCrcCgheck->addItem(m_DlgMapMean[nIndex * 9 + 3][i]);
        }
        ui->comboBox_UpCrcCgheck->setCurrentIndex(m_DlgParaSetStruct[nIndex][5].InitVal.lval);
        //下行测量帧RP
        for (i = 0; i < DlgMapLineNum[nIndex * 9 + 4]; i++)
        {
            ui->comboBox_DownMeasRp->addItem(m_DlgMapMean[nIndex * 9 + 4][i]);
        }
        ui->comboBox_DownMeasRp->setCurrentIndex(m_DlgParaSetStruct[nIndex][6].InitVal.lval);
        //下行测量帧CRC校验
        for (i = 0; i < DlgMapLineNum[nIndex * 9 + 5]; i++)
        {
            ui->comboBox_DownCrcCheck->addItem(m_DlgMapMean[nIndex * 9 + 5][i]);
        }
        ui->comboBox_DownCrcCheck->setCurrentIndex(m_DlgParaSetStruct[nIndex][7].InitVal.lval);
        //距离预报方式
        for (i = 0; i < DlgMapLineNum[nIndex * 9 + 6]; i++)
        {
            ui->comboBox_DisForType->addItem(m_DlgMapMean[nIndex * 9 + 6][i]);
        }
        ui->comboBox_DisForType->setCurrentIndex(m_DlgParaSetStruct[nIndex][8].InitVal.lval);
        //星上通道号
        for (i = 0; i < DlgMapLineNum[nIndex * 9 + 7]; i++)
        {
            ui->comboBox_SatalliteChannel->addItem(m_DlgMapMean[nIndex * 9 + 7][i]);
        }
        ui->comboBox_SatalliteChannel->setCurrentIndex(m_DlgParaSetStruct[nIndex][9].InitVal.lval);
        //距离解模糊
        for (i = 0; i < DlgMapLineNum[nIndex * 9 + 8]; i++)
        {
            ui->comboBox_DisAmbigous->addItem(m_DlgMapMean[nIndex * 9 + 8][i]);
        }
        ui->comboBox_DisAmbigous->setCurrentIndex(m_DlgParaSetStruct[nIndex][10].InitVal.lval);
        //距离预报值
        ui->lineEdit_DisForValue->setText(QString::number(m_DlgParaSetStruct[nIndex][11].InitVal.lval / 1000.0, 'f', 3));
        //应答机零值(单程)
        ui->lineEdit_ResponsorZero->setText(QString::number(m_DlgParaSetStruct[nIndex][12].InitVal.lval / 1000.0, 'f', 3));
        //应答机相对时延
        ui->lineEdit_ResponsorDelay->setText(QString::number(m_DlgParaSetStruct[nIndex][13].InitVal.lval / 100.0, 'f', 2));
    }
    else if (nType == 1)
    {
        //目标类型
        if (m_MCSSSCjParam.cjParam[nIndex].ucTargetType > 0)
            ui->comboBox_TargetType->setCurrentIndex(m_MCSSSCjParam.cjParam[nIndex].ucTargetType - 1);
        //目标代号
        ui->lineEdit_TargetCode->setText(QString::number(m_MCSSSCjParam.cjParam[nIndex].ucTargetCode, 16).toUpper());
        //地面测站代号
        ui->lineEdit_StationCode->setText(QString::number(m_MCSSSCjParam.cjParam[0].ucStationID, 16).toUpper());
        //是否使用测站代号
        if (m_MCSSSCjParam.cjParam[nIndex].ucStationIDIsUsed > 0)
            ui->comboBox_UseJudge->setCurrentIndex(m_MCSSSCjParam.cjParam[nIndex].ucStationIDIsUsed - 1);
        //工作站数目
        if (m_MCSSSCjParam.cjParam[nIndex].ucStationCount > 0)
            ui->comboBox_StationNum->setCurrentIndex(m_MCSSSCjParam.cjParam[nIndex].ucStationCount - 1);
        //上行测量帧CRC校验
        if (m_MCSSSCjParam.cjParam[nIndex].ucCRC > 0)
            ui->comboBox_UpCrcCgheck->setCurrentIndex(m_MCSSSCjParam.cjParam[nIndex].ucCRC - 1);
        //下行测量帧RP
        if (m_MCSSSCjParam.cjParam[nIndex].ucRP > 0)
            ui->comboBox_DownMeasRp->setCurrentIndex(m_MCSSSCjParam.cjParam[nIndex].ucRP - 1);
        //下行测量帧CRC校验
        if (m_MCSSSCjParam.cjParam[nIndex].ucDownCRC > 0)
            ui->comboBox_DownCrcCheck->setCurrentIndex(m_MCSSSCjParam.cjParam[nIndex].ucDownCRC - 1);
        //距离预报方式
        if (m_MCSSSCjParam.cjParam[nIndex].ucPreRangDataMd > 0)
            ui->comboBox_DisForType->setCurrentIndex(m_MCSSSCjParam.cjParam[nIndex].ucPreRangDataMd - 1);
        //星上通道号
        if (m_MCSSSCjParam.cjParam[nIndex].ucSatChnelNo > 0)
            ui->comboBox_SatalliteChannel->setCurrentIndex(m_MCSSSCjParam.cjParam[nIndex].ucSatChnelNo - 1);
        //距离解模糊
        if (m_MCSSSCjParam.cjParam[nIndex].ucRMHCheck > 0)
            ui->comboBox_DisAmbigous->setCurrentIndex(m_MCSSSCjParam.cjParam[nIndex].ucRMHCheck - 1);
        //距离预报值
        ui->lineEdit_DisForValue->setText(QString::number(m_MCSSSCjParam.cjParam[nIndex].ulPridictR / 1000.0, 'f', 3));
        //应答机零值(单程)
        ui->lineEdit_ResponsorZero->setText(QString::number(m_MCSSSCjParam.cjParam[nIndex].uiRSPDisZero / 1000.0, 'f', 3));
        //应答机相对时延
        ui->lineEdit_ResponsorDelay->setText(QString::number(m_MCSSSCjParam.cjParam[nIndex].uiRSPDifTime / 100.0, 'f', 2));
    }
    /*******************************目标参数*********************************/
}

void SS2BBE_CjCsParamSetGUIDlg::getParamTarget()
{
    bool ok;
    //目标类型
    m_MCSSSCjParam.cjParam[m_nTargetNum].ucTargetType
            = ui->comboBox_TargetType->currentIndex() + 1;
    //目标代号
    m_MCSSSCjParam.cjParam[m_nTargetNum].ucTargetCode
            = ui->lineEdit_TargetCode->text().toInt(&ok, 16);
    //地面测站代号
    m_MCSSSCjParam.cjParam[m_nTargetNum].ucStationID
            = ui->lineEdit_StationCode->text().toInt(&ok, 16);
    //是否使用测站代号
    m_MCSSSCjParam.cjParam[m_nTargetNum].ucStationIDIsUsed
            = ui->comboBox_UseJudge->currentIndex() + 1;
    //工作站数目
    m_MCSSSCjParam.cjParam[m_nTargetNum].ucStationCount
            = ui->comboBox_StationNum->currentIndex() + 1;
    //上行测量帧CRC校验
    m_MCSSSCjParam.cjParam[m_nTargetNum].ucCRC
            = ui->comboBox_UpCrcCgheck->currentIndex() + 1;
    //下行测量帧RP
    m_MCSSSCjParam.cjParam[m_nTargetNum].ucRP
            = ui->comboBox_DownMeasRp->currentIndex() + 1;
    //下行测量帧CRC校验
    m_MCSSSCjParam.cjParam[m_nTargetNum].ucDownCRC
            = ui->comboBox_DownCrcCheck->currentIndex() + 1;
    //距离预报方式
    m_MCSSSCjParam.cjParam[m_nTargetNum].ucPreRangDataMd
            = ui->comboBox_DisForType->currentIndex() + 1;
    //星上通道号
    m_MCSSSCjParam.cjParam[m_nTargetNum].ucSatChnelNo
            = ui->comboBox_SatalliteChannel->currentIndex() + 1;
    //距离解模糊
    m_MCSSSCjParam.cjParam[m_nTargetNum].ucRMHCheck
            = ui->comboBox_DisAmbigous->currentIndex() + 1;
    //距离预报值
    m_MCSSSCjParam.cjParam[m_nTargetNum].ulPridictR
            = qRound(ui->lineEdit_DisForValue->text().toDouble() * 1000);
    //应答机零值(单程)
    m_MCSSSCjParam.cjParam[m_nTargetNum].uiRSPDisZero
            = qRound(ui->lineEdit_ResponsorZero->text().toDouble() * 1000);
    //应答机相对时延
    m_MCSSSCjParam.cjParam[m_nTargetNum].uiRSPDifTime
            = qRound64(ui->lineEdit_ResponsorDelay->text().toDouble() * 100);
}

void SS2BBE_CjCsParamSetGUIDlg::set_radioButton_Target()
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

DWORDLONG SS2BBE_CjCsParamSetGUIDlg::StrToSynCode(const char * pStr)
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

void SS2BBE_CjCsParamSetGUIDlg::on_radioButton_Target1_clicked()
{
    if(1 != CheckPara())
    {
        set_radioButton_Target();
        return;
    }
    GetPara();
    m_nTargetNum = 0;
    //int nType = ui->pushButton_Set->isEnabled()  ? 0 : 1;
    showTargetParam(0, 1);
    if (ui->pushButton_Modify->text() != "更改")
        ui->lineEdit_StationCode->setEnabled(true);
}

void SS2BBE_CjCsParamSetGUIDlg::on_radioButton_Target2_clicked()
{
    if(1 != CheckPara())
    {
        set_radioButton_Target();
        return;
    }
    GetPara();
    m_nTargetNum = 1;
    //int nType = ui->pushButton_Set->isEnabled()  ? 0 : 1;
    showTargetParam(1, 1);
    ui->lineEdit_StationCode->setEnabled(false);
}

void SS2BBE_CjCsParamSetGUIDlg::on_radioButton_Target3_clicked()
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
    ui->lineEdit_StationCode->setEnabled(false);
}

void SS2BBE_CjCsParamSetGUIDlg::on_radioButton_Target4_clicked()
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
    ui->lineEdit_StationCode->setEnabled(false);
}
//距离模糊解
void SS2BBE_CjCsParamSetGUIDlg::on_comboBox_DisAmbigous_currentIndexChanged(int index)
{
    if(ui->pushButton_Modify->text() == "更改")
        return;
    if(index ==0){
        ui->comboBox_DisForType->setEnabled(true);
    }else{
        ui->comboBox_DisForType->setEnabled(false);
        ui->lineEdit_DisForValue->setEnabled(false);
    }
    on_comboBox_DisForType_currentIndexChanged(ui->comboBox_DisForType->currentIndex());
}
//距离预报方式
void SS2BBE_CjCsParamSetGUIDlg::on_comboBox_DisForType_currentIndexChanged(int index)
{
    if(ui->pushButton_Modify->text() == "更改")
        return;
    if(index ==0 && ui->comboBox_DisAmbigous->currentIndex()==0)
        ui->lineEdit_DisForValue->setEnabled(true);
    else
        ui->lineEdit_DisForValue->setEnabled(false);

}

void SS2BBE_CjCsParamSetGUIDlg::on_comboBox_StationNum_currentIndexChanged(int index)
{
    if(ui->pushButton_Modify->text() == "更改")
        return;
    switch(index){
    case 0:
        ui->comboBox_SatalliteChannel->clear();
        ui->comboBox_SatalliteChannel->addItem("1");
        ui->comboBox_SatalliteChannel->setCurrentIndex(m_MCSSSCjParam.cjParam[m_nTargetNum].ucSatChnelNo - 1);
        break;
    case 1:
        ui->comboBox_SatalliteChannel->clear();
        ui->comboBox_SatalliteChannel->addItem("1");
        ui->comboBox_SatalliteChannel->addItem("2");
        ui->comboBox_SatalliteChannel->setCurrentIndex(m_MCSSSCjParam.cjParam[m_nTargetNum].ucSatChnelNo - 1);
        break;
    case 2:
        ui->comboBox_SatalliteChannel->clear();
        ui->comboBox_SatalliteChannel->addItem("1");
        ui->comboBox_SatalliteChannel->addItem("2");
        ui->comboBox_SatalliteChannel->addItem("3");
        ui->comboBox_SatalliteChannel->setCurrentIndex(m_MCSSSCjParam.cjParam[m_nTargetNum].ucSatChnelNo - 1);
        break;
    case 3:
        ui->comboBox_SatalliteChannel->clear();
        ui->comboBox_SatalliteChannel->addItem("1");
        ui->comboBox_SatalliteChannel->addItem("2");
        ui->comboBox_SatalliteChannel->addItem("3");
        ui->comboBox_SatalliteChannel->addItem("4");
        ui->comboBox_SatalliteChannel->setCurrentIndex(m_MCSSSCjParam.cjParam[m_nTargetNum].ucSatChnelNo - 1);
        break;
    }
}
//目标类型
void SS2BBE_CjCsParamSetGUIDlg::on_comboBox_TargetType_currentIndexChanged(int index)
{
    if(ui->pushButton_Modify->text() == "更改")
        return;
    switch(index){
    case 0:
        ui->lineEdit_TargetCode->setEnabled(true);
        break;
    case 1:
        ui->lineEdit_TargetCode->setText("1A");
        ui->lineEdit_TargetCode->setEnabled(false);
        break;
    case 2:
        ui->lineEdit_TargetCode->setText("2B");
        ui->lineEdit_TargetCode->setEnabled(false);
        break;
    case 3:
        ui->lineEdit_TargetCode->setText("3C");
        ui->lineEdit_TargetCode->setEnabled(false);
        break;
    }
}

void SS2BBE_CjCsParamSetGUIDlg::updateRadioButton()
{
    if(!m_pCommFunc)return;
    if (m_targetNum_usedInMacro == m_pCommFunc->getTargetNum())
        return;
    m_targetNum_usedInMacro = m_pCommFunc->getTargetNum();
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
#pragma pack()
