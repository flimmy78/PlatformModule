#include "TCPNCodeSetGUIDlg.h"
#include "ui_TCPNCodeSetGUIDlg.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDateTime>
#include "../../Common/Log.h"
#include <QDebug>
#include "../../Common/CommonFunction.h"
#include <QTimer>
#pragma pack (1)
extern quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
                     uchar ucSn);
TCPNCodeSetGUIDlg::TCPNCodeSetGUIDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TCPNCodeSetGUIDlg)
{
    ui->setupUi(this);
    m_pStateMgr = NULL;
    m_pILxTsspMessageManager = NULL;
    m_pILxTsspLogManager = NULL;
    m_pPublicInfo = NULL;
    m_pHAManager = NULL;
    m_nTargetNum = 0;
    m_MCSSSB2PNCODEStatus = NULL;
    m_timer1 = new QTimer(this);
    m_currentNumber_usedByMacro = -1;
    m_currentNumber_usedByParam = -1;
    connect(m_timer1, SIGNAL(timeout()), this, SLOT(updateRadioButton()));
    setAttribute(Qt::WA_DeleteOnClose);
}

int TCPNCodeSetGUIDlg::msgBoxFunction(QMessageBox &msgBox, QString msg, BOOL ret)
{
    msgBox.setText(msg);
    msgBox.exec();
    return ret;
}
TCPNCodeSetGUIDlg::~TCPNCodeSetGUIDlg()
{
    killTimer(m_TimerID);
    m_timer1->stop();
    delete m_timer1;
    delete ui;
}

void TCPNCodeSetGUIDlg::setText(const QString& strTitle, ParaStruct xmlsetText[5][60], QString MapMean[50][30], int MapNum[50], uchar Type, ILxTsspParameterMacro* pMacroObj)
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
    for(int i=0; i<49; i++)
    {
        DlgMapLineNum[i] = MapNum[i];
    }
    //初始化界面
    InitDLG(Type, pMacroObj);
}

//初始化对话框
void TCPNCodeSetGUIDlg::InitDLG(int Type, ILxTsspParameterMacro* pMacroObj)
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
            DWORD stLen = sizeof(SMCU2::MCSSSB2PNCODEStatusStruct4);
            m_MCSSSB2PNCODEStatus = (SMCU2::MCSSSB2PNCODEStatusStruct4*)m_pStateMgr->
                    FindOneItem(m_StatusParaMCSSSB2PNCodeID, stLen);
            if(m_MCSSSB2PNCODEStatus == NULL || stLen != sizeof(SMCU2::MCSSSB2PNCODEStatusStruct4))
                return;
        }
        //启动定时器
        m_TimerID = startTimer(500);
        //不使能
        EnabledControls(false);
    }
    else if(Type == 1)//取宏里参数
    {
        ui->pushButton_Set->setEnabled(false);
        if(pMacroObj != NULL)
        {
            QByteArray byteArray;
            pMacroObj->GetParameterBlock(m_ParaMCSSSB2PNCodeID, byteArray);
            memcpy(&m_MCSSSB2PNCode, byteArray.data(), sizeof(SMCU2::MCSSSB2PNCodeStruct4));
        }
        ui->pushButton_Modify->setVisible(false);
        ui->pushButton_Set->setVisible(false);

        m_timer1->start(500);
        //显示参数
        ShowPara(1);
        m_pCommFunc = CCommFunc::getInstance();
    }
}

//设置输入格式
void TCPNCodeSetGUIDlg::InitInput()
{
    //字母数字
    QRegExp regExp2("[A-Fa-f0-9]{1,3}");
    //上行遥控支路伪码本源多项式1
    ui->lineEdit_UpTCtrlPseOriginPoly1->setValidator(new QRegExpValidator(regExp2, this));
    //上行遥控支路伪码本源多项式2
    ui->lineEdit_UpTCtrlPseOriginPoly2->setValidator(new QRegExpValidator(regExp2, this));
    //上行遥控支路伪码初相1
    ui->lineEdit_UpTCtrlPsePhase1->setValidator(new QRegExpValidator(regExp2, this));
    //上行遥控支路伪码初相2
    ui->lineEdit_UpTCtrlPsePhase2->setValidator(new QRegExpValidator(regExp2, this));
}

void TCPNCodeSetGUIDlg::ShowControls()
{
    showTargetParam(0);
    //默认选择目标1
    ui->radioButton_Target1->setChecked(true);
}

//显示参数
void TCPNCodeSetGUIDlg::ShowPara(int type)
{
    if(type == 0)       //参数设置
    {
        //码同步
        if(m_MCSSSB2PNCODEStatus != NULL)
        {
            for(int i = 0; i < MAX_TARGET_COUNT; ++i)
                m_MCSSSB2PNCode.PNCode[i] = m_MCSSSB2PNCODEStatus->PNCodeStatu[i].tParams;
        }
        else
        {
            DWORD stLen = sizeof(SMCU2::MCSSSB2PNCODEStatusStruct4);
            if(m_pStateMgr != NULL)
                m_MCSSSB2PNCODEStatus  = (SMCU2::MCSSSB2PNCODEStatusStruct4*)m_pStateMgr->FindOneItem(m_StatusParaMCSSSB2PNCodeID, stLen);
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
    showRadioButton();
    showTargetParam(m_nTargetNum, 1);

}

void TCPNCodeSetGUIDlg::showTargetParam(int nIndex, int nType)
{
    if(0 == nType)
    {


        //上行遥控支路伪码本源多项式1
        ui->lineEdit_UpTCtrlPseOriginPoly1->setText(QString::number(m_DlgParaSetStruct[nIndex][2].InitVal.lval, 16).toUpper());
        //上行遥控支路伪码本源多项式2
        ui->lineEdit_UpTCtrlPseOriginPoly2->setText(QString::number(m_DlgParaSetStruct[nIndex][3].InitVal.lval, 16).toUpper());
        //上行遥控支路伪码初相1
        ui->lineEdit_UpTCtrlPsePhase1->setText(QString::number(m_DlgParaSetStruct[nIndex][4].InitVal.lval, 16).toUpper());
        //上行遥控支路伪码初相2
        ui->lineEdit_UpTCtrlPsePhase2->setText(QString::number(m_DlgParaSetStruct[nIndex][5].InitVal.lval, 16).toUpper());


    }
    else if (1 == nType)
    {

        ui->lineEdit_UpTCtrlPseOriginPoly1->setText(QString::number(m_MCSSSB2PNCode.PNCode[nIndex].usUpTCSPNPolyA, 16).toUpper());
        //上行遥控支路伪码本源多项式2
        ui->lineEdit_UpTCtrlPseOriginPoly2->setText(QString::number(m_MCSSSB2PNCode.PNCode[nIndex].usUpTCSPNPolyB, 16).toUpper());
        //上行遥控支路伪码初相1
        ui->lineEdit_UpTCtrlPsePhase1->setText(QString::number(m_MCSSSB2PNCode.PNCode[nIndex].usUpTCSPNInitPhaseA, 16).toUpper());
        //上行遥控支路伪码初相2
        ui->lineEdit_UpTCtrlPsePhase2->setText(QString::number(m_MCSSSB2PNCode.PNCode[nIndex].usUpTCSPNInitPhaseB, 16).toUpper());



    }
}


void TCPNCodeSetGUIDlg::SetObjInfo(TLxTsspObjectInfo ObjectInfo)
{
    memcpy(&m_ObjectInfo, &ObjectInfo, sizeof(TLxTsspObjectInfo));
    //查找对象管理器
    LxTsspObjectManager* pObjectManager = LxTsspObjectManager::Instance();

    if(pObjectManager == NULL){
        return;
    }

    m_pHAManager = pObjectManager->GetHAManager();

    bool bRet = m_DeviceMap.loadXML();
    if (!bRet)
    {
        QString strInfo = QString("Load device map failed %1");
        CLog::addLog(strInfo);
        return;
    }

    ITEM *pItem = m_DeviceMap.getItem(MODULENAME);
    if (pItem->vSubItem.size() > 0)
    {
        SUBITEM& sItem = *(pItem->getSubItemByDeviceID_SN(m_ObjectInfo.usLocalDID, m_ObjectInfo.ucSN));
        m_usC = sItem.usCMD;
        m_usTID = sItem.usTID;
        m_ucSN = sItem.ucSN;
        /*************************框架**********************************/
        //码同步
        m_ParaMCSSSB2PNCodeID = GetID(sItem.usStationID,
                                      sItem.ucSubSysID, sItem.usDeviceID,sItem.usTID,
                                      sItem.ucSN);
        /*************************框架**********************************/

        /*************************状态**********************************/
        //码同步
        m_StatusParaMCSSSB2PNCodeID = GetID(sItem.usStationID,
                                            sItem.ucSubSysID, sItem.usDeviceID,sItem.usStateType,
                                            sItem.ucSN);
        /*************************状态**********************************/
    }
    ITEM *pItemCom = m_DeviceMap.getItem(COMM_TCBBE_NAME);
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
void TCPNCodeSetGUIDlg::EnabledControls(bool bFlag)
{
    /*************************Group1*************************/

    //上行遥控支路伪码本源多项式1
    ui->lineEdit_UpTCtrlPseOriginPoly1->setEnabled(bFlag);
    //上行遥控支路伪码本源多项式2
    ui->lineEdit_UpTCtrlPseOriginPoly2->setEnabled(bFlag);
    //上行遥控支路伪码初相1
    ui->lineEdit_UpTCtrlPsePhase1->setEnabled(bFlag);
    //上行遥控支路伪码初相2
    ui->lineEdit_UpTCtrlPsePhase2->setEnabled(bFlag);




    /*************************Group1*************************/

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
int TCPNCodeSetGUIDlg::SaveToMacro(ILxTsspParameterMacro* pMacroObj)
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
        byteArray.resize(sizeof(m_MCSSSB2PNCode));
        memcpy(byteArray.data(), &m_MCSSSB2PNCode, sizeof(m_MCSSSB2PNCode));
        pMacroObj->UpdateParameterBlock(m_ParaMCSSSB2PNCodeID, byteArray);

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
void TCPNCodeSetGUIDlg::getParamTarget()
{
    bool ok;
    //上行遥控支路伪码本源多项式1
    m_MCSSSB2PNCode.PNCode[m_nTargetNum].usUpTCSPNPolyA = ui->lineEdit_UpTCtrlPseOriginPoly1->text().toInt(&ok, 16);
    //上行遥控支路伪码本源多项式2
    m_MCSSSB2PNCode.PNCode[m_nTargetNum].usUpTCSPNPolyB = ui->lineEdit_UpTCtrlPseOriginPoly2->text().toInt(&ok, 16);
    //上行遥控支路伪码初相1
    m_MCSSSB2PNCode.PNCode[m_nTargetNum].usUpTCSPNInitPhaseA = ui->lineEdit_UpTCtrlPsePhase1->text().toInt(&ok, 16);
    //上行遥控支路伪码初相2
    m_MCSSSB2PNCode.PNCode[m_nTargetNum].usUpTCSPNInitPhaseB = ui->lineEdit_UpTCtrlPsePhase2->text().toInt(&ok, 16);
}
//从界面取参数
void TCPNCodeSetGUIDlg::GetPara()
{
    getParamTarget();
}

bool TCPNCodeSetGUIDlg::CheckParaValue(QLineEdit* pLE)
{
    bool ok = false;
    int v = pLE->text().toInt(&ok, 16);
    if(!ok)return false;
    return v >= 0 && v <= 0x3FF;
}

//验证参数范围
int TCPNCodeSetGUIDlg::CheckPara()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(MODULENAME);
    if(ui->lineEdit_UpTCtrlPseOriginPoly1->text().isEmpty() || !CheckParaValue(ui->lineEdit_UpTCtrlPseOriginPoly1))
    {
        return msgBoxFunction(msgBox,"上行伪码,遥控支路伪码本源多项式1输入范围0～3FF，请重新输入!", FALSE);
    }
    if(ui->lineEdit_UpTCtrlPseOriginPoly2->text().isEmpty() || !CheckParaValue(ui->lineEdit_UpTCtrlPseOriginPoly2))
    {
        return msgBoxFunction(msgBox,"上行伪码,遥控支路伪码本源多项式2输入范围0～3FF，请重新输入!", FALSE);
    }

    if(ui->lineEdit_UpTCtrlPsePhase1->text().isEmpty() || !CheckParaValue(ui->lineEdit_UpTCtrlPsePhase1))
    {
        return msgBoxFunction(msgBox,"上行伪码,遥控支路伪码初相1输入范围0～3FF，请重新输入!", FALSE);
    }

    if(ui->lineEdit_UpTCtrlPsePhase2->text().isEmpty() || !CheckParaValue(ui->lineEdit_UpTCtrlPsePhase2))
    {
        return msgBoxFunction(msgBox,"上行伪码,遥控支路伪码初相2输入范围0～3FF，请重新输入!", FALSE);
    }
    return 1;
}


//定时器
void TCPNCodeSetGUIDlg::timerEvent(QTimerEvent *event)
{
    ShowPara(0);
}

//更改参数按钮休息处理函数
void TCPNCodeSetGUIDlg::on_pushButton_Modify_clicked()
{
    if(ui->pushButton_Modify->text() == "更改")
    {
        EnabledControls(true);
        killTimer(m_TimerID);
        return;
    }

    if(ui->pushButton_Modify->text() == "恢复")
    {
        EnabledControls(false);
        m_TimerID = startTimer(500);
        return;
    }
}

void TCPNCodeSetGUIDlg::on_pushButton_Set_clicked()
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
                    SMCU2::MCSSSB2PNCodeStruct SSPNCodeParam;
                }param[MAX_TARGET_COUNT];
            }data;
            data.target = 1 | 0x02 | 0x04 | 0x08 | 0x10;
            for (int i = 0; i < MAX_TARGET_COUNT; ++i)
            {
                data.param[i].SSPNCodeParam = m_MCSSSB2PNCode. PNCode[i];
            }
            SendMessage((char*)&data, sizeof(data), m_usC, m_usTID, m_ucSN);
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

void TCPNCodeSetGUIDlg::closeEvent(QCloseEvent *event)
{
    event->accept();
}

QSize TCPNCodeSetGUIDlg::sizeHint() const
{
    return QSize(this->width(), this->height());
}

void TCPNCodeSetGUIDlg::SubFrmEn(bool bFlag)
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
void TCPNCodeSetGUIDlg::IDSubFrmEn(bool bFlag)
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
void TCPNCodeSetGUIDlg::CycSubFrmEn(bool bFlag)
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
void TCPNCodeSetGUIDlg::ReserveSubFrmEn(bool bFlag)
{

}

void TCPNCodeSetGUIDlg::SendMessage(char* pData, unsigned short  usLen, unsigned short usC, unsigned short usTID, unsigned char ucSN)
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
    subHeader.O = ucSN;
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
    if(0x700 == subHeader.DID){
        subHeader.DID = 0x701;
        ITEM *pItem = m_DeviceMap.getItem(MODULENAME);
        if (pItem->vSubItem.size() > 0)
        {
            SUBITEM& sItem = *(pItem->getSubItemByDeviceID_SN(subHeader.DID, m_ObjectInfo.ucSN));
            header.C = sItem.usCMD;
            subHeader.TID = sItem.usTID;

        }
        memcpy(message.pData, &header, sizeof(TLxTsspMessageHeader));
        memcpy(message.pData + sizeof(TLxTsspMessageHeader),
               &subHeader, sizeof(TLxTsspSubMsgHeader));
        m_pILxTsspMessageManager->FireMessage(message);
    }
    else if(0x701 == subHeader.DID){
        subHeader.DID = 0x700;
        ITEM *pItem = m_DeviceMap.getItem(MODULENAME);
        if (pItem->vSubItem.size() > 0)
        {
            SUBITEM& sItem = *(pItem->getSubItemByDeviceID_SN(subHeader.DID, m_ObjectInfo.ucSN));
            header.C = sItem.usCMD;
            subHeader.TID = sItem.usTID;
        }
        memcpy(message.pData, &header, sizeof(TLxTsspMessageHeader));
        memcpy(message.pData + sizeof(TLxTsspMessageHeader),
               &subHeader, sizeof(TLxTsspSubMsgHeader));
        m_pILxTsspMessageManager->FireMessage(message);
    }
    delete[] message.pData;
}

DWORDLONG TCPNCodeSetGUIDlg::StrToSynCode(const char * pStr)
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


int TCPNCodeSetGUIDlg::getSharedMemoryTargetNum()
{
    return 1;
    DWORD stLen = sizeof(SMCU2::MCSSysTarget4StatusStruct);
    if(m_pStateMgr != NULL)
        m_pMCSTargetSysStatus  = (SMCU2::MCSSysTarget4StatusStruct*)m_pStateMgr->FindOneItem(m_CommTargetID, stLen);

    if (m_pMCSTargetSysStatus != NULL)
        return m_pMCSTargetSysStatus->MCSSysStatu.tParams.ucTargetNum;

    return 0;
}

void TCPNCodeSetGUIDlg::showRadioButton()
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
    switch(nNum)
    {
    case 1:
        ui->radioButton_Target1->show();
        break;
    case 2:
        ui->radioButton_Target1->show();
        ui->radioButton_Target2->show();
        break;
    case 3:
        ui->radioButton_Target1->show();
        ui->radioButton_Target2->show();
        ui->radioButton_Target3->show();
        break;
    case 4:
        ui->radioButton_Target1->show();
        ui->radioButton_Target2->show();
        ui->radioButton_Target3->show();
        ui->radioButton_Target4->show();
        break;
    default:
        break;
    }
}

void TCPNCodeSetGUIDlg::set_radioButton_Target()
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

void TCPNCodeSetGUIDlg::on_radioButton_Target1_clicked()
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
}

void TCPNCodeSetGUIDlg::on_radioButton_Target2_clicked()
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
}

void TCPNCodeSetGUIDlg::on_radioButton_Target3_clicked()
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

void TCPNCodeSetGUIDlg::on_radioButton_Target4_clicked()
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

void TCPNCodeSetGUIDlg::updateRadioButton()
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
    switch(m_pCommFunc->getTargetNum() - 1)
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
#pragma pack ()
