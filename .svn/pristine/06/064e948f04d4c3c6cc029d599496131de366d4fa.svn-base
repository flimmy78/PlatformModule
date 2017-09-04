#include "RCLOCKDcuGUIDlg.h"
#include "ui_RCLOCKDcuGUIDlg.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDateTime>
#include "../../Common/Log.h"
#include "../../Common/CommonFunction.h"

extern quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
              uchar ucSn);

RCLOCKDcuGUIDlg::RCLOCKDcuGUIDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RCLOCKDcuGUIDlg)
{
    ui->setupUi(this);
    m_pStateMgr = NULL;
    m_pILxTsspMessageManager = NULL;
    m_pILxTsspLogManager = NULL;
    m_pPublicInfo = NULL;
    m_pHAManager = NULL;
    m_pComStatus = NULL;

    m_bOnLine = false;

    setAttribute(Qt::WA_DeleteOnClose);

    //setMinimumSize(800, 600);

    EnabledControls(false);
}

RCLOCKDcuGUIDlg::~RCLOCKDcuGUIDlg()
{
    killTimer(m_TimerID);
    delete ui;
}

void RCLOCKDcuGUIDlg::setText(const QString& strTitle, ParaStruct xmlsetText[3][60], QString MapMean[50][30],
                            int MapNum[50], uchar Type, ILxTsspParameterMacro* pMacroObj)
{
    m_strTitle = strTitle;
    setWindowTitle(strTitle);

    memcpy(m_DlgParaSetStruct, xmlsetText, sizeof(ParaStruct)*50*3);

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

    //获取要发送的C和TID
    initCmdInfo(MODULENAME);

    //初始化界面
    InitDLG(Type, pMacroObj);
    ui->groupBox_hide->hide();
}

//初始化对话框
void RCLOCKDcuGUIDlg::InitDLG(int Type, ILxTsspParameterMacro* pMacroObj)
{
    //设置界面的灯的默认颜色
    setLight(ui->label_NorthLight, Light_empty);
    setLight(ui->label_TwoIPPSInput, Light_empty);
    setLight(ui->label_TwoInput, Light_empty);
    setLight(ui->label_SyncLight, Light_empty);
    setLight(ui->label_RCLOCKLight, Light_empty);
    setLight(ui->label_FourInput, Light_empty);
    setLight(ui->label_OneIPPSInput, Light_empty);
    setLight(ui->label_PowerLight, Light_empty);
    setLight(ui->label_WarningLight, Light_empty);
    setLight(ui->label_OneInput, Light_empty);
    setLight(ui->label_GPSLight, Light_empty);
    setLight(ui->label_ThirdInput, Light_empty);
    //

    iParaSetFlag = Type;
    //设置输入格式
    InitInput();
    //显示变量名字和范围
    ShowControls();

    if(Type == 0)//取当前参数
    {
        //取当前设置的参数
       /* QByteArray byteArray;
        if(m_pHAManager != NULL)
        {
            m_pHAManager->GetParameter(m_ParaDDTBBECodeSynID,byteArray);
            memcpy(&m_DDTBBECodeSynPara,byteArray.data(),sizeof(m_DDTBBECodeSynPara));

            m_pHAManager->GetParameter(m_ParaDDTBBEFrameSynID,byteArray);
            memcpy(&m_DDTBBEFrameSynPara,byteArray.data(),sizeof(m_DDTBBEFrameSynPara));

            m_pHAManager->GetParameter(m_ParaDDTBBESubFrameSynID,byteArray);
            memcpy(&m_DDTBBESubFrameSynPara,byteArray.data(),sizeof(m_DDTBBESubFrameSynPara));
        }
      */
        if(m_pStateMgr != NULL)
        {
            DWORD stLen = sizeof(RCLOCKDCUStatu);
            m_pComStatus = (RCLOCKDCUStatu*)m_pStateMgr->FindOneItem(m_StatusComSynID, stLen);
            if(m_pComStatus == NULL || stLen != sizeof(RCLOCKDCUStatu))
                return;
        }
        //启动定时器
        m_TimerID = startTimer(500);
        //不使能
        EnabledControls(false);
    }
    else if(Type == 1)//取宏里参数
    {
        if(pMacroObj != NULL)
        {
            QByteArray byteArray;
            pMacroObj->GetParameterBlock(m_ParaComSynID, byteArray);
            memcpy(&m_ComPara, byteArray.data(), sizeof(RCLOCKDCUStatu));
        }
        ui->pushButton_Modify->setVisible(false);
        ui->pushButton_Set->setVisible(false);

        //显示参数
        ShowPara(1);
    }
}

//设置输入格式
void RCLOCKDcuGUIDlg::InitInput()
{
    QRegExp regExp("[A-Fa-f0-9]{1,8}");
    //帧同步码组
//    ui->lineEdit_FrmSyncCode->setValidator(new QRegExpValidator(regExp, this));

//    //正整数
    QRegExp regExp2("[0-9]{1,16}");
    //输入射频频率
    //ui->lineEdit_InputFreq->setValidator(new QRegExpValidator(regExp2, this));
}

void RCLOCKDcuGUIDlg::ShowControls()
{
    //向下拉框控件中写入从xml中读取的内容
    //时间区域
    for (int i=0; i<DlgMapLineNum[0]; i++)
    {
        ui->comboBox_TimeZone->addItem(m_DlgMapMean[0][i]);
    }
    //时间信息
    QString strVal = QString(m_DlgParaSetStruct[0][1].cParaName);
    //ui->lineEdit_InputFreq->setText(strVal);
    //参考有效状态
    for (int i=0; i<DlgMapLineNum[1]; i++)
    {
        //ui->comboBox_ReferentInvalid->addItem(m_DlgMapMean[1][i]);
    }
    //参考选择状态
    for (int i=0; i<DlgMapLineNum[2]; i++)
    {
        ui->comboBox_ReferentSource->addItem(m_DlgMapMean[2][i]);
    }
    //同步状态
    for (int i=0; i<DlgMapLineNum[3]; i++)
    {
        //ui->comboBox_SyncStatus->addItem(m_DlgMapMean[3][i]);
    }
    //铷钟锁定状态
    for (int i=0; i<DlgMapLineNum[4]; i++)
    {
        //ui->comboBox_RCLOCK->addItem(m_DlgMapMean[4][i]);
    }
    //解调秒状态
    for (int i=0; i<DlgMapLineNum[5]; i++)
    {
        //ui->comboBox_DemodulateSec->addItem(m_DlgMapMean[5][i]);
    }
    //守时状态
    for (int i=0; i<DlgMapLineNum[6]; i++)
    {
        //ui->comboBox_Punctual->addItem(m_DlgMapMean[6][i]);
    }
    //告警状态
    for (int i=0; i<DlgMapLineNum[7]; i++)
    {
        //ui->comboBox_Warning->addItem(m_DlgMapMean[7][i]);
    }
    //电源状态
    for (int i=0; i<DlgMapLineNum[8]; i++)
    {
        //ui->comboBox_Power->addItem(m_DlgMapMean[8][i]);
    }
    //分本控状态
    for (int i=0; i<DlgMapLineNum[9]; i++)
    {
        //ui->comboBox_VolCtrl->addItem(m_DlgMapMean[9][i]);
    }
    //驯控状态
    for (int i=0; i<DlgMapLineNum[10]; i++)
    {
        ui->comboBox_TameCtrl->addItem(m_DlgMapMean[10][i]);
    }
    //同步方式
    for (int i=0; i<DlgMapLineNum[11]; i++)
    {
        ui->comboBox_SyncCtrl->addItem(m_DlgMapMean[11][i]);
    }

    //设置状态为只读形式
    //时间显示
    ui->lineEdit_TimeInfo->setReadOnly(true);
    //守时状态
    ui->lineEdit_Punctual->setReadOnly(true);
    //分本控状态
    ui->lineEdit_VolCtrl->setReadOnly(true);
    //解调秒状态
    ui->lineEdit_DemodulateSec->setReadOnly(true);
}

//显示参数
void RCLOCKDcuGUIDlg::ShowPara(int type)
{
    if(type == 0)       //参数设置
    {
        if(m_pComStatus != NULL)
        {
            //m_ComPara = m_pComStatus->tParam;
            memcpy(&m_ComPara, m_pComStatus, sizeof(RCLOCKDCUStatu));
        }
        else
        {
            DWORD stLen = sizeof(RCLOCKDCUStatu);
            if(m_pStateMgr != NULL)
                m_pComStatus  = (RCLOCKDCUStatu*)m_pStateMgr->FindOneItem(m_StatusComSynID, stLen);
        }
    }
    else
    {    
        if (ui->pushButton_Modify->text() != "更改")
        {
            //EnabledControlsSubFrmType(ui->comboBox_SubFrmType->currentIndex());
        }
    }

    if (m_pComStatus == NULL)
        return;

    /********************设备是否在线************************/
    m_bOnLine = CCommFunc::getInstance()->getrClockStatus();
    if (!m_bOnLine)
    {
        setOfflineStatusValue();
        return;
    }
    /********************设备是否在线************************/

    //时间区域
    ui->comboBox_TimeZone->setCurrentIndex(m_ComPara.ucTimeZone + 12);
    //时间信息
    QString strDateTime ;
    /*= QString("%1-%2-%3 %4:%5:%6").arg(m_ComPara.sYear)
            .arg(m_ComPara.ucMonth, 2, 10, QChar('0')).arg(m_ComPara.ucDay, 2, 10, QChar('0'))
            .arg(m_ComPara.ucHour, 2, 10, QChar('0'))
            .arg(m_ComPara.ucMin, 2, 10, QChar('0')).arg(m_ComPara.ucSec, 2, 10, QChar('0'));
            */
    strDateTime.sprintf("%04d-%02d-%02d %02d:%02d:%02d",m_ComPara.sYear,m_ComPara.ucMonth,m_ComPara.ucDay,m_ComPara.ucHour,\
                        m_ComPara.ucMin,m_ComPara.ucSec);
    //QDateTime dt;
    //dt.fromString(strDateTime);
    //ui->dateTimeEdit_TimeInfo->setDateTime(dt);
    ui->lineEdit_TimeInfo->setText(strDateTime);
    //参考有效状态
    //ui->comboBox_ReferentInvalid->setCurrentIndex(m_ComPara.ucReferentInvalid);
    //GPS状态
    setLight(ui->label_GPSLight, (m_ComPara.ucGPSStatus == 1 ? Light_green : Light_empty));
    //北斗状态
    setLight(ui->label_NorthLight, (m_ComPara.ucNorthStatus == 2 ? Light_green : Light_empty));
    //参考选择状态
    ui->comboBox_ReferentSource->setCurrentIndex(m_ComPara.ucReferentSel == 1 ? 0 : 1);
    //同步状态
    setLight(ui->label_SyncLight, (m_ComPara.ucSyncStatus == 1 ? Light_green : Light_red));
    //铷钟锁定状态
    setLight(ui->label_RCLOCKLight, (m_ComPara.ucRCLOCK == 1 ? Light_green : Light_red));
    //解调秒状态
    //ui->comboBox_DemodulateSec->setCurrentIndex(m_ComPara.ucDemodulateSec);
    ui->lineEdit_DemodulateSec->setText(m_DlgMapMean[5][m_ComPara.ucDemodulateSec]);
    //守时状态
    //ui->comboBox_Punctual->setCurrentIndex(m_ComPara.ucPunctual);
    ui->lineEdit_Punctual->setText(m_DlgMapMean[6][m_ComPara.ucPunctual]);
    //告警状态
    setLight(ui->label_WarningLight, (m_ComPara.ucWarning == 1 ? Light_red : Light_empty));
    //电源状态
    setLight(ui->label_PowerLight, (m_ComPara.ucPower == 1 ? Light_green : Light_red));
    //分本控状态
    //ui->comboBox_VolCtrl->setCurrentIndex(m_ComPara.ucConMode);
    ui->lineEdit_VolCtrl->setText(m_DlgMapMean[9][m_ComPara.ucConMode]);
    //驯控状态
    ui->comboBox_TameCtrl->setCurrentIndex(m_ComPara.ucTameCtrl);
    //同步方式
    ui->comboBox_SyncCtrl->setCurrentIndex(m_ComPara.ucSyncCtrl);
    /********************显示后面灯的状态***********************/
    setLight(ui->label_OneInput, (m_ComPara.ucOne10MHzInput == 1 ? Light_green : Light_red));
    setLight(ui->label_TwoInput, (m_ComPara.ucTwo10MHzInput == 2 ? Light_green : Light_red));
    setLight(ui->label_ThirdInput, m_ComPara.ucThird10MHzInput == 4 ? Light_green : Light_red);
    setLight(ui->label_FourInput, m_ComPara.ucFour10MHzInput == 8 ? Light_green : Light_red);
    setLight(ui->label_OneIPPSInput, m_ComPara.ucOne1PPSInput == 1 ? Light_green : Light_red);
    setLight(ui->label_TwoIPPSInput, m_ComPara.ucTwo1PPSInput == 2 ? Light_green : Light_red);
    /********************显示后面灯的状态***********************/
}

void RCLOCKDcuGUIDlg::SetObjInfo(TLxTsspObjectInfo ObjectInfo)
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
        SUBITEM sItem = pItem->vSubItem.at(0);
        /*************************框架**********************************/
        m_ParaComSynID = GetID(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID,
                                     sItem.usTID, sItem.ucSN);
        /*************************框架**********************************/

        /*************************状态**********************************/
        m_StatusComSynID = GetID(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID,
                                       sItem.usStateType, sItem.ucSN);
        /*************************状态**********************************/
    }

    //日志
    m_pILxTsspLogManager = pObjectManager->GetLogManager();
    m_pStateMgr = pObjectManager->GetStateManager();
    m_pILxTsspMessageManager = pObjectManager->GetMessageManager();
    m_pPublicInfo = pObjectManager->GetPublicInfoManager();
}

//使能或不使能控件
void RCLOCKDcuGUIDlg::EnabledControls(bool bFlag)
{
    //时区设置
    ui->comboBox_TimeZone->setEnabled(bFlag);
    ui->pushButton_TimeZone->setEnabled(bFlag);
    //同步设置
    ui->comboBox_SyncCtrl->setEnabled(bFlag);
    ui->pushButton_SyncCtrl->setEnabled(bFlag);
    //参考源设置
    ui->comboBox_ReferentSource->setEnabled(bFlag);
    ui->pushButton_ReferentSource->setEnabled(bFlag);
    //驯服设置
    ui->comboBox_TameCtrl->setEnabled(bFlag);
    ui->pushButton_TameCtrl->setEnabled(bFlag);
    //时间设置
//    ui->dateTimeEdit_TimeInfo->setEnabled(bFlag);
//    ui->pushButton_TimeInfo->setEnabled(bFlag);
    //ui->dateTimeEdit_TimeInfo->setEnabled(false);
    ui->pushButton_TimeInfo->setEnabled(false);

    //将状态界面显示为不可用
    //同步状态
    //ui->comboBox_SyncStatus->setEditable(false);
    //本分控状态
    //ui->comboBox_VolCtrl->setEnabled(false);
    //告警状态
    //ui->comboBox_Warning->setEnabled(false);
    //守时状态
    //ui->comboBox_Punctual->setEnabled(false);
    //铷钟锁定状态
    //ui->comboBox_RCLOCK->setEnabled(false);
    //电源状态
    //ui->comboBox_Power->setEnabled(false);
    //解调秒状态
    //ui->comboBox_DemodulateSec->setEnabled(false);
    //参考有效状态
    //ui->comboBox_ReferentInvalid->setEnabled(false);
    //同步状态
    //ui->comboBox_SyncStatus->setEnabled(false);

    //时间设置按钮不可用 暂时不用设置
    ui->pushButton_TimeInfo->hide();
    //界面上以前预留的设置按钮不可用
    ui->pushButton_Set->hide();
}

int RCLOCKDcuGUIDlg::SaveToMacro(ILxTsspParameterMacro* pMacroObj)
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

        //参数
        byteArray.resize(sizeof(m_ComPara));
        memcpy(byteArray.data(), &m_ComPara, sizeof(m_ComPara));
        pMacroObj->UpdateParameterBlock(m_ParaComSynID, byteArray);

        //日志信息
        QString strLog = QString(tr("%1: 保存宏成功！")).arg(MODULENAME);
        CLog::addLog(strLog);
    }
}

//从界面取参数
void RCLOCKDcuGUIDlg::GetPara()
{
    //在线标记
    /*m_ComPara.ucOnlineMark = ui->comboBox_OnlineMark->currentIndex();
    //输入射频频率
    m_ComPara.ucInputFreq = ui->lineEdit_InputFreq->text().toUInt();
    //输出射频频率
    m_ComPara.ucOutputFreq = ui->lineEdit_OutputFreq->text().toUInt();
    //信标输出频率：
    m_ComPara.ucBeaconFreq = ui->lineEdit_BeaconFreq->text().toUInt();
    //数控衰减器1
    m_ComPara.sAttenuation1 = ui->lineEdit_NCAtten1->text().toShort();
    //数控衰减器2
    m_ComPara.sAttenuation2 = ui->lineEdit_NCAtten2->text().toShort();
    //参考源选择
    m_ComPara.usReferentSel = ui->comboBox_ReferentSel->currentIndex();*/
}

//验证参数范围
int RCLOCKDcuGUIDlg::CheckPara()
{
    QMessageBox msgBox;
    //输入射频频率
    /*UINT uVal = ui->lineEdit_InputFreq->text().toUInt();
    if (uVal < m_DlgParaSetStruct[0][1].MinVal.lmin ||uVal > m_DlgParaSetStruct[0][1].MaxVal.lmax)
    {
        msgBox.setText("输入射频频率超出范围，请重新输入!");
        msgBox.exec();
        return -1;
    }
    //输出射频频率
    uVal = ui->lineEdit_OutputFreq->text().toUInt();
    if (uVal < m_DlgParaSetStruct[0][2].MinVal.lmin ||uVal > m_DlgParaSetStruct[0][2].MaxVal.lmax)
    {
        msgBox.setText("输出射频频率超出范围，请重新输入!");
        msgBox.exec();
        return -1;
    }
    //信标输出频率
    uVal = ui->lineEdit_BeaconFreq->text().toUInt();
    if (uVal < m_DlgParaSetStruct[0][3].MinVal.lmin ||uVal > m_DlgParaSetStruct[0][3].MaxVal.lmax)
    {
        msgBox.setText("信标输出频率超出范围，请重新输入!");
        msgBox.exec();
        return -1;
    }
    //数控衰减器1
    USHORT usVal = ui->lineEdit_NCAtten1->text().toShort();
    if (usVal < m_DlgParaSetStruct[0][4].MinVal.lmin ||usVal > m_DlgParaSetStruct[0][4].MaxVal.lmax)
    {
        msgBox.setText("数控衰减器1超出范围，请重新输入!");
        msgBox.exec();
        return -1;
    }
    //数控衰减器2
    usVal = ui->lineEdit_NCAtten2->text().toShort();
    if (usVal < m_DlgParaSetStruct[0][5].MinVal.lmin ||usVal > m_DlgParaSetStruct[0][5].MaxVal.lmax)
    {
        msgBox.setText("数控衰减器2超出范围，请重新输入!");
        msgBox.exec();
        return -1;
    }*/

    return 1;
}

//定时器
void RCLOCKDcuGUIDlg::timerEvent(QTimerEvent *event)
{
    ShowPara(0);
}

void RCLOCKDcuGUIDlg::on_pushButton_Modify_clicked()
{
    if (!m_bOnLine)
    {
        QString strMsg = QString(tr("%1处于离线状态，无法进行更改设置！")).arg(MODULENAME);
        QMessageBox::information(this, tr("信息提示"), strMsg);
        return;
    }

    if(ui->pushButton_Modify->text() == "更改")
    {
        EnabledControls(true);
        killTimer(m_TimerID);
        ui->pushButton_Modify->setText("恢复");
    }
    else if(ui->pushButton_Modify->text() == "恢复")
    {
        EnabledControls(false);
        m_TimerID = startTimer(500);
        ui->pushButton_Modify->setText("更改");
    }
}

void RCLOCKDcuGUIDlg::on_pushButton_Set_clicked()
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
            QByteArray byteArray;
            //参数
            byteArray.resize(sizeof(m_ComPara));
            memcpy(byteArray.data(), &m_ComPara, sizeof(m_ComPara));
            m_pHAManager->SetParameter(m_ParaComSynID, byteArray);
            SendMessage((char*)&m_ComPara, sizeof(RCLOCKDCUStatu), m_usC, m_usTID);
        }

        //日志信息
        QString strLog;
        strLog = QString("%1: 设置参数成功！").arg(MODULENAME);
        CLog::addLog(strLog);

        //不使能控件
        EnabledControls(false);
        m_TimerID = startTimer(500);
    }
}

void RCLOCKDcuGUIDlg::closeEvent(QCloseEvent *event)
{
    event->accept();
}

QSize RCLOCKDcuGUIDlg::sizeHint() const
{
    return QSize(this->width(), this->height());
}

void RCLOCKDcuGUIDlg::SendMessage(char* pData, unsigned short  usLen, unsigned short usC, unsigned short usTID)
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
    subHeader.O = m_ObjectInfo.ucSN;
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

bool RCLOCKDcuGUIDlg::initCmdInfo(QString strModuleName)
{
    bool bRet = m_DeviceMap.loadXML();
    if (!bRet)
    {
        QString strInfo = QString("load device map failed %1").arg(MODULENAME);
        //qDebug() << strInfo;
        CLog::addLog(strInfo);
        return false;
    }
    ITEM* item = m_DeviceMap.getItem(strModuleName);
    if (item != NULL)
    {
        m_usC = item->vSubItem[0].usCMD;
        m_usTID = item->vSubItem[0].usTID;
    }
    return true;
}

DWORDLONG RCLOCKDcuGUIDlg::StrToSynCode(const char * pStr)
{
    char c, C;
    const char*  p;
    DWORDLONG dwlValue = 0;
    char  DigVal = 0;
    USHORT nDigCount = 0, MAX_DIGITS = 16; //The synchronizing code should be not longer  than 16 digitals

    p = pStr;     //Point to the first char of the string
    c = *p++;   // read the first char
    while ((c != NULL) && (nDigCount <= MAX_DIGITS))//  Not the terminator of a string && total digitals is less than 16
    {
         if(isspace((int)(unsigned char)c))    c = *p++;// skip whitespace
         else if(c >= '0' && c <= '9')                        //the char is a number between 1 and 9
         {
             DigVal = c - '0';
             dwlValue = dwlValue*16 + DigVal;               //Accumulate the value of the hexal number so far to the current digital
             nDigCount++;
             c = *p++;
         }
         else if((C=toupper(c)) >= 'A' && C <= 'F')           //the char is a letter could be a digital of a hexal number
         {
             DigVal = C-'A'+10;
             dwlValue = dwlValue*16 + DigVal;               //Accumulate the value of the hexal number so far to the current digital
             nDigCount++;
             c = *p++;
         }
         else    break;                                 // An invalid char that is neither whitespace nor a hexal digital is encounted
                                                        // Conclude the scanning process
    }
    return dwlValue;
}

void RCLOCKDcuGUIDlg::sendCMD(QByteArray ba, QString strInfo)
{
    SendMessage(ba.data(), ba.size(), m_usC, m_usTID);
    //日志信息
    QString strLog;
    strLog = QString("%1: 设置%2成功！").arg(MODULENAME).arg(strInfo);
    CLog::addLog(strLog);

    //不使能控件
    on_pushButton_Modify_clicked();
}

void RCLOCKDcuGUIDlg::setLight(QLabel *pLabel, int nState)
{
    QPixmap light;
    bool bRet = false;
    switch(nState)
    {
    case Light_empty:
        bRet = light.load(":/empty.png");
        break;
    case Light_green:
        bRet = light.load(QString(":/safe.png"));
        break;
    case Light_red:
        bRet = light.load(QString(":/denter.png"));
        break;
    default:
        bRet = light.load(":/empty.png");
        break;
    }
    pLabel->setPixmap(light);
    //pLabel->resize(24, 24);
}

void RCLOCKDcuGUIDlg::setOfflineStatusValue()
{
    QString strEmpty("--");
    //时间信息
    ui->lineEdit_TimeInfo->setText(strEmpty);
    //分本控状态
    ui->lineEdit_VolCtrl->setText(strEmpty);
    //GPS状态
    setLight(ui->label_GPSLight, Light_empty);
    //北斗状态
    setLight(ui->label_NorthLight, Light_empty);
    //同步状态
    setLight(ui->label_SyncLight, Light_empty);
    //铷钟锁定状态
    setLight(ui->label_RCLOCKLight, Light_empty);
    //解调秒状态
    ui->lineEdit_DemodulateSec->setText(strEmpty);
    //守时状态
    ui->lineEdit_Punctual->setText(strEmpty);
    //告警状态
    setLight(ui->label_WarningLight, Light_empty);
    //电源状态
    setLight(ui->label_PowerLight, Light_empty);
    /********************显示后面灯的状态***********************/
    setLight(ui->label_OneInput, Light_empty);
    setLight(ui->label_TwoInput, Light_empty);
    setLight(ui->label_ThirdInput, Light_empty);
    setLight(ui->label_FourInput, Light_empty);
    setLight(ui->label_OneIPPSInput, Light_empty);
    setLight(ui->label_TwoIPPSInput, Light_empty);
    /********************显示后面灯的状态***********************/
}

void RCLOCKDcuGUIDlg::on_pushButton_TimeZone_clicked()
{
    //设置时间区域
    int nCurIndex = ui->comboBox_TimeZone->currentIndex();
    //设置参数
    QByteArray byteArray;
    byteArray.append(0x34);
    int val = nCurIndex - 12;
    CHAR c = (val < 0) ? 0x01 : 0x00;
    byteArray.append(c);
    byteArray.append(abs(val));
    sendCMD(byteArray, "时间区域命令");
}

void RCLOCKDcuGUIDlg::on_pushButton_SyncCtrl_clicked()
{
    //设置同步控制
    int nCurIndex = ui->comboBox_SyncCtrl->currentIndex();
    //设置参数
    QByteArray byteArray;
    byteArray.append(0x32);
    CHAR c = nCurIndex;
    byteArray.append(c);
    sendCMD(byteArray, "同步控制命令");
}

void RCLOCKDcuGUIDlg::on_pushButton_ReferentSource_clicked()
{
    //参考源设置
    int nCurIndex = ui->comboBox_ReferentSource->currentIndex();
    //设置参数
    QByteArray byteArray;
    byteArray.append(0x33);
    CHAR c = nCurIndex;
    byteArray.append(c);
    sendCMD(byteArray, "参考源控制命令");
}

void RCLOCKDcuGUIDlg::on_pushButton_TameCtrl_clicked()
{
    //驯服控制
    int nCurIndex = ui->comboBox_TameCtrl->currentIndex();
    //设置参数
    QByteArray byteArray;
    byteArray.append(0x31);
    CHAR c = nCurIndex;
    byteArray.append(c);
    sendCMD(byteArray, "驯服控制命令");
}

void RCLOCKDcuGUIDlg::on_pushButton_TimeInfo_clicked()
{
    //时间设置
    QDateTime dt;// = ui->dateTimeEdit_TimeInfo->dateTime();
    QDate date = dt.date();
    QTime time = dt.time();
    //设置参数
    QByteArray byteArray;
    byteArray.append(0x35);
    UCHAR year = abs(date.year() - 2000);
    UCHAR month = date.month();
    UCHAR day = date.day();
    UCHAR hour = time.hour();
    UCHAR minute = time.minute();
    UCHAR sec = time.second();
    byteArray.append(year);
    byteArray.append(month);
    byteArray.append(day);
    byteArray.append(hour);
    byteArray.append(minute);
    byteArray.append(sec);
    sendCMD(byteArray, "时间设置命令");
}


