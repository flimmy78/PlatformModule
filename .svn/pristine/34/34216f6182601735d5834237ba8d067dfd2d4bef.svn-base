#include "USBBBE_CJParaSetGUIDlg.h"
#include "ui_USBBBE_CJParaSetGUIDlg.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDateTime>
#include "../../Common/Log.h"

extern quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
                     uchar ucSn);
USBBBE_CJParaSetGUIDlg::USBBBE_CJParaSetGUIDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::USBBBE_CJParaSetGUIDlg)
{
    ui->setupUi(this);
    m_pStateMgr = NULL;
    m_pILxTsspMessageManager = NULL;
    m_pILxTsspLogManager = NULL;
    m_pPublicInfo = NULL;
    m_pHAManager = NULL;

    m_MCSUSBCjStatus = NULL;
    setAttribute(Qt::WA_DeleteOnClose);
}

USBBBE_CJParaSetGUIDlg::~USBBBE_CJParaSetGUIDlg()
{
    delete ui;
}

int USBBBE_CJParaSetGUIDlg::msgBoxFunction(QMessageBox &msgBox, QString msg, BOOL ret)
{
    msgBox.setText(msg);
    msgBox.exec();
    return ret;
}
void USBBBE_CJParaSetGUIDlg::setText(const QString& strTitle, ParaStruct xmlsetText[3][60], QString MapMean[50][30], int MapNum[50], uchar Type, ILxTsspParameterMacro* pMacroObj)
{
    m_strTitle = strTitle;
    setWindowTitle(strTitle);

    memcpy(m_DlgParaSetStruct, xmlsetText, sizeof(ParaStruct) * 50 * 3);

    for(int i=0;i<50;i++)
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
void USBBBE_CJParaSetGUIDlg::InitDLG(int Type, ILxTsspParameterMacro* pMacroObj)
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
            DWORD stLen = sizeof(MCSUSBCjStatusStruct);
            m_MCSUSBCjStatus = (MCSUSBCjStatusStruct*)m_pStateMgr->FindOneItem(m_StatusParaMCSUSBCjParamID, stLen);
            if(m_MCSUSBCjStatus == NULL || stLen != sizeof(MCSUSBCjStatusStruct))
                return;
        }
        //启动定时器
        m_TimerID = startTimer(500);
        //不使能
        EnabledControls(false);
        EnabledControlsDown(false);

    }
    else if(Type == 1)//取宏里参数
    {
        ui->pushButton_Modify->setText("");
        if(pMacroObj!=NULL)
        {
            QByteArray byteArray;
            pMacroObj->GetParameterBlock(m_ParaMCSUSBCjParamID, byteArray);
            memcpy(&m_MCSUSBCjParam, byteArray.data(), sizeof(MCSUSBCjParamStruct));
        }
        ui->pushButton_Modify->setVisible(false);
        ui->pushButton_Set->setVisible(false);
        ui->pushButton_Modify_down->setVisible(false);
        ui->pushButton_Set_down->setVisible(false);
        //显示参数
        ShowPara(1);
    }
}

//设置输入格式
void USBBBE_CJParaSetGUIDlg::InitInput()
{
    //正整数
    QRegExp regExp("[0-9\\.]{1,16}");
    QRegExp regExpSideT("[0-9]{1,8}");
    //侧音等待延时
    QRegExp regExp_SidetoneDelay("[0-9]{1,4}");
    ui->lineEdit_SidetoneDelay->setValidator(new QRegExpValidator(regExp_SidetoneDelay, this));
    //距离预报值
    QRegExp regExp_DisForV("[0-9]{1,6}(\\.)?([0-9]{1,3})?");
    ui->lineEdit_DisForV->setValidator(new QRegExpValidator(regExp_DisForV, this));
    //应答机零值
    QRegExp regExp_ResponsorZero("[0-9]{1,6}(\\.)?([0-9]{1,2})?");
    ui->lineEdit_ResponsorZero->setValidator(new QRegExpValidator(regExp_ResponsorZero, this));

    //字母数字
    /*QRegExp regExp2("[A-Fa-f0-9]{1,8}");
    //帧同步码组[HEX]
    ui->lineEdit_ZTMZ->setValidator(new QRegExpValidator(regExp2, this));
    //副帧同步码组[HEX]
    ui->lineEdit_FZTBMZ->setValidator(new QRegExpValidator(regExp2, this));*/
}

void USBBBE_CJParaSetGUIDlg::ShowControls()
{
    //向下拉框控件中写入从xml中读取的内容
    int i;
    //最低侧音选择
    for(i=0; i<DlgMapLineNum[0];i++)
    {
        ui->comboBox_LowestSidetone->addItem(m_DlgMapMean[0][i]);
    }
    ui->comboBox_LowestSidetone->setCurrentIndex(m_DlgParaSetStruct[0][0].InitVal.lval);
    //距离预报方式
    for(i=0; i<DlgMapLineNum[1];i++)
    {
        ui->comboBox_DisForType->addItem(m_DlgMapMean[1][i]);
    }
    ui->comboBox_DisForType->setCurrentIndex(m_DlgParaSetStruct[0][3].InitVal.lval);
    //距离解模糊
    for(i=0; i<DlgMapLineNum[2];i++)
    {
        ui->comboBox_FuzDis->addItem(m_DlgMapMean[2][i]);
    }
    ui->comboBox_FuzDis->setCurrentIndex(m_DlgParaSetStruct[0][4].InitVal.lval);
    //往静态控件中写入缺省值
    //侧音等待延时
    ui->lineEdit_SidetoneDelay->setText(QString::number(m_DlgParaSetStruct[0][2].InitVal.lval));
    ui->label_SidetoneDelay->setText("侧音等待延时\n[" + QString::number(m_DlgParaSetStruct[0][2].MinVal.lmin) +
            "~" + QString::number(m_DlgParaSetStruct[0][2].MaxVal.lmax) + "]");
    //距离预报值
    ui->lineEdit_DisForV->setText(QString::number(m_DlgParaSetStruct[0][5].InitVal.lval));
    //应答机零值位置
    ui->lineEdit_ResponsorZero->setText(QString::number(m_DlgParaSetStruct[0][6].InitVal.lval));
}

//显示参数
void USBBBE_CJParaSetGUIDlg::ShowPara(int type)
{
    if(type == 0)       //参数设置
    {
        //副帧同步器
        if(m_MCSUSBCjStatus != NULL)
        {
            m_MCSUSBCjParam = m_MCSUSBCjStatus->tParams;
        }
        else
        {
            DWORD stLen = sizeof(MCSUSBCjStatusStruct);
            if(m_pStateMgr != NULL)
                m_MCSUSBCjStatus  = (MCSUSBCjStatusStruct*)m_pStateMgr->FindOneItem(m_StatusParaMCSUSBCjParamID, stLen);
        }
    }
    else
    {
        //解扰使能或禁止
        //副帧方式控件使能
        // if (ui->pushButton_Change->text() != "更改")
        //        {
        //EnabledControlsSubFrmType(ui->comboBox_SubFrmType->currentIndex());
        // }
    }
    /********************************参数*********************************/

    //最低侧音选择
    ui->comboBox_LowestSidetone->setCurrentIndex(m_MCSUSBCjParam.ucLSToneCheck - 1);
    //侧音等待延时
    ui->lineEdit_SidetoneDelay->setText(QString::number(m_MCSUSBCjParam.uiSToneWait));
    //距离预报方式
    if (m_MCSUSBCjParam.ucPreRangDataMd > 0)
    {
        if (m_MCSUSBCjParam.ucPreRangDataMd == 4)
            ui->comboBox_DisForType->setCurrentText("引导数据");
        else
        {
            ui->comboBox_DisForType->setCurrentIndex(m_MCSUSBCjParam.ucPreRangDataMd - 1);
        }
    }

    //距离解模糊
    if (m_MCSUSBCjParam.ucRMHCheck > 0)
        ui->comboBox_FuzDis->setCurrentIndex(m_MCSUSBCjParam.ucRMHCheck - 1);
    //距离预报值
    if (m_MCSUSBCjParam.ulPridictR != 0)
        ui->lineEdit_DisForV->setText(QString::number(m_MCSUSBCjParam.ulPridictR / 1000.0, 'f', 3));
    else
        ui->lineEdit_DisForV->setText("0");
    //应答机零值
    if (m_MCSUSBCjParam.uiRSPZero != 0)
        ui->lineEdit_ResponsorZero->setText(QString::number(m_MCSUSBCjParam.uiRSPZero / 100.0, 'f', 2));
    else
        ui->lineEdit_ResponsorZero->setText("0");

    on_comboBox_FuzDis_currentIndexChanged(ui->comboBox_FuzDis->currentIndex());
    /********************************参数*********************************/
}

//设置帧长范围
void USBBBE_CJParaSetGUIDlg::SetFrameLenRange(int iWordLen)
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

void USBBBE_CJParaSetGUIDlg::SetObjInfo(TLxTsspObjectInfo ObjectInfo)
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
        m_ParaMCSUSBCjParamID = GetID(sItem.usStationID,
                                      sItem.ucSubSysID, sItem.usDeviceID,sItem.usTID,
                                      sItem.ucSN);
        /*************************框架**********************************/

        /*************************状态**********************************/
        m_StatusParaMCSUSBCjParamID = GetID(sItem.usStationID,
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
void USBBBE_CJParaSetGUIDlg::EnabledControls(bool bFlag)
{
    /*************************Group1*************************/
    //最低侧音选择
    ui->comboBox_LowestSidetone->setEnabled(bFlag);
    //侧音等待延时
    ui->lineEdit_SidetoneDelay->setEnabled(bFlag);
    //距离预报延时
    ui->comboBox_DisForType->setEnabled(bFlag);
    //距离解模糊
    ui->comboBox_FuzDis->setEnabled(bFlag);
    //预报距离值
    ui->lineEdit_DisForV->setEnabled(bFlag);

    //应答机零值
    // ui->lineEdit_ResponsorZero->setEnabled(bFlag);

    /*************************Group1*************************/


    /*************************帧同步器*************************/
    if (bFlag)
    {
        //副帧方式控件使能
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
int USBBBE_CJParaSetGUIDlg::SaveToMacro(ILxTsspParameterMacro* pMacroObj)
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

        byteArray.resize(sizeof(m_MCSUSBCjParam));
        memcpy(byteArray.data(),&m_MCSUSBCjParam,sizeof(m_MCSUSBCjParam));
        pMacroObj->UpdateParameterBlock(m_ParaMCSUSBCjParamID,byteArray);

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
void USBBBE_CJParaSetGUIDlg::GetPara()
{

    //最低侧音选择
    m_MCSUSBCjParam.ucLSToneCheck = ui->comboBox_LowestSidetone->currentIndex() + 1;
    //侧音等待延时
    m_MCSUSBCjParam.uiSToneWait = ui->lineEdit_SidetoneDelay->text().toInt();
    //距离解模糊
    m_MCSUSBCjParam.ucRMHCheck = ui->comboBox_FuzDis->currentIndex() + 1;
    //距离预报方式
    if (ui->comboBox_DisForType->currentText() == "引导数据")
        m_MCSUSBCjParam.ucPreRangDataMd = 4;
    else
    {
        m_MCSUSBCjParam.ucPreRangDataMd = ui->comboBox_DisForType->currentIndex() + 1;
    }
    //距离预报值
    m_MCSUSBCjParam.ulPridictR = qRound(ui->lineEdit_DisForV->text().toDouble() * 1000);
    //应答机距离零值
    m_MCSUSBCjParam.uiRSPZero = qRound(ui->lineEdit_ResponsorZero->text().toDouble() * 100.0);

}

//验证参数范围
int USBBBE_CJParaSetGUIDlg::CheckPara()
{
    QMessageBox msgBox;
    QString strTmp;
    QString disPreV = ui->lineEdit_DisForV->text();
    QString responsorZero = ui->lineEdit_ResponsorZero->text();
    msgBox.setWindowTitle(MODULENAME);
    //侧音等待延时
    if((ui->lineEdit_SidetoneDelay->text().toInt())<m_DlgParaSetStruct[0][2].MinVal.lmin
            || (ui->lineEdit_SidetoneDelay->text().toInt())>m_DlgParaSetStruct[0][2].MaxVal.lmax)
    {
        msgBox.setText("侧音等待延时范围: ["+QString::number(m_DlgParaSetStruct[0][2].MinVal.lmin) +
                "~" + QString::number(m_DlgParaSetStruct[0][2].MaxVal.lmax)+"], 请重新输入！");
        msgBox.exec();
        return -1;
    }
    if(ui->lineEdit_SidetoneDelay->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"侧音等待延时输入为空，请重新输入!", FALSE);
    }
    if(ui->lineEdit_DisForV->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"预报距离值输入为空，请重新输入!", FALSE);
    }
    if(ui->lineEdit_ResponsorZero->text().isEmpty())
    {
        return msgBoxFunction(msgBox,"应答机零值(单向)输入为空，请重新输入!", FALSE);
    }
    if (qRound(disPreV.toDouble() * 1000) < m_DlgParaSetStruct[0][5].MinVal.lmin * 1000
            || qRound(disPreV.toDouble() * 1000) > m_DlgParaSetStruct[0][5].MaxVal.lmax * 1000)
    {
        strTmp = QString("预报距离值[%1～%2]km超出范围，请重新输入!")
                .arg(m_DlgParaSetStruct[0][5].MinVal.lmin).arg(m_DlgParaSetStruct[0][5].MaxVal.lmax);
        return msgBoxFunction(msgBox, strTmp, FALSE);
    }
    if (qRound(responsorZero.toDouble() * 100) < m_DlgParaSetStruct[0][6].MinVal.lmin * 100
            || qRound(responsorZero.toDouble() * 100) > m_DlgParaSetStruct[0][6].MaxVal.lmax * 100)
    {
        strTmp = QString("应答机零值(单向)[%1～%2]m超出范围，请重新输入!")
                .arg(m_DlgParaSetStruct[0][6].MinVal.lmin).arg(m_DlgParaSetStruct[0][6].MaxVal.lmax);
        return msgBoxFunction(msgBox, strTmp, FALSE);
    }
    return 1;
}

//定时器
void USBBBE_CJParaSetGUIDlg::timerEvent(QTimerEvent *event)
{
    ShowPara(0);
}


void USBBBE_CJParaSetGUIDlg::on_pushButton_Set_clicked()
{
    //验证参数范围
    if(CheckPara()== 1)
    {
        //从界面取参数
        GetPara();

        //设置参数
        //取当前设置的参数
        if(m_pHAManager != NULL)
        {
            QByteArray byteArray;
            byteArray.resize(sizeof(m_MCSUSBCjParam));
            memcpy(byteArray.data(),&m_MCSUSBCjParam,sizeof(MCSUSBCjParamStruct));
            m_pHAManager->SetParameter(m_ParaMCSUSBCjParamID, byteArray);
            SendMessage((char*)&m_MCSUSBCjParam, sizeof(MCSUSBCjParamStruct), m_usC, m_usTID);
        }

        //日志信息
        TLxTsspLog tmpLog;
        QString strLog;
        tmpLog.ucWarningLevel  = 1;
        strLog += QString("%1: 设置参数成功！").arg(MODULENAME);
        CLog::addLog(strLog, 1);
        //不使能控件
        EnabledControls(false);
        ui->pushButton_Modify_down->setEnabled(true);
        m_TimerID = startTimer(500);
    }
}

void USBBBE_CJParaSetGUIDlg::closeEvent(QCloseEvent *event)
{
    event->accept();
}

QSize USBBBE_CJParaSetGUIDlg::sizeHint() const
{
    return QSize(this->width(), this->height());
}


void USBBBE_CJParaSetGUIDlg::SubFrmEn(bool bFlag)
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
void USBBBE_CJParaSetGUIDlg::IDSubFrmEn(bool bFlag)
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
void USBBBE_CJParaSetGUIDlg::CycSubFrmEn(bool bFlag)
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
void USBBBE_CJParaSetGUIDlg::ReserveSubFrmEn(bool bFlag)
{

}

void USBBBE_CJParaSetGUIDlg::SendMessage(char* pData, unsigned short  usLen, unsigned short usC, unsigned short usTID)
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

DWORDLONG USBBBE_CJParaSetGUIDlg::StrToSynCode(const char * pStr)
{
    char c,C;
    const char*  p;
    DWORDLONG dwlValue = 0;
    char   DigVal = 0;
    USHORT nDigCount = 0,MAX_DIGITS = 16; //The synchronizing code should be not longer  than 16 digitals

    p = pStr;     //Point to the first char of the string
    c = *p++;   // read the first char
    while ((c != NULL) && (nDigCount <= MAX_DIGITS))//  Not the terminator of a string && total digitals is less than 16
    {
        if(isspace((int)(unsigned char)c))    c = *p++;// skip whitespace
        else if(c >= '0' && c <= '9')                        //the char is a number between 1 and 9
        {
            DigVal=c-'0';
            dwlValue=dwlValue*16+DigVal;               //Accumulate the value of the hexal number so far to the current digital
            nDigCount++;
            c = *p++;
        }
        else if((C=toupper(c)) >= 'A' && C<='F')           //the char is a letter could be a digital of a hexal number
        {
            DigVal = C-'A' + 10;
            dwlValue = dwlValue * 16 + DigVal;               //Accumulate the value of the hexal number so far to the current digital
            nDigCount++;
            c = *p++;
        }
        else    break;                                 // An invalid char that is neither whitespace nor a hexal digital is encounted
        // Conclude the scanning process
    }
    return dwlValue;
}


//使能或不使能控件
void USBBBE_CJParaSetGUIDlg::EnabledControlsDown(bool bFlag)
{
    /*************************Group1*************************/

    //应答机零值
    ui->lineEdit_ResponsorZero->setEnabled(bFlag);

    /*************************Group1*************************/


    /*************************帧同步器*************************/
    if (bFlag)
    {
        //副帧方式控件使能
        ui->pushButton_Modify_down->setText("恢复");

        ui->pushButton_Set_down->setEnabled(true);
    }
    else
    {
        ui->pushButton_Modify_down->setText("更改");

        ui->pushButton_Set_down->setEnabled(false);
    }
}
//距离预报方式
void USBBBE_CJParaSetGUIDlg::on_comboBox_DisForType_currentIndexChanged(int index)
{
    if(ui->pushButton_Modify->text() == "更改")
        return;
    if(index== 0 && ui->comboBox_FuzDis->currentIndex() == 0)
        ui->lineEdit_DisForV->setEnabled(true);
    if(index== 1 ){
        ui->lineEdit_DisForV->setEnabled(false);

    }
}


//距离解模糊
void USBBBE_CJParaSetGUIDlg::on_comboBox_FuzDis_currentIndexChanged(int index)
{
    if(ui->pushButton_Modify->text() == "更改")
        return;
    if(index==1){
        ui->comboBox_DisForType->setEnabled(false);
        ui->lineEdit_DisForV->setEnabled(false);
        //on_comboBox_DisForType_currentIndexChanged(ui->comboBox_DisForType->currentIndex());
    }
    if(index==0){
        ui->comboBox_DisForType->setEnabled(true);
        on_comboBox_DisForType_currentIndexChanged(ui->comboBox_DisForType->currentIndex());
    }

}



void USBBBE_CJParaSetGUIDlg::on_pushButton_Set_down_clicked()
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
            byteArray.resize(sizeof(m_MCSUSBCjParam));
            memcpy(byteArray.data(),&m_MCSUSBCjParam,sizeof(MCSUSBCjParamStruct));
            m_pHAManager->SetParameter(m_ParaMCSUSBCjParamID, byteArray);
            SendMessage((char*)&m_MCSUSBCjParam, sizeof(MCSUSBCjParamStruct), m_usC, m_usTID);
        }

        //日志信息
        TLxTsspLog tmpLog;
        QString strLog;
        tmpLog.ucWarningLevel  = 1;
        strLog += QString("%1: 设置参数成功！").arg(MODULENAME);
        CLog::addLog(strLog, 1);
        //不使能控件
        EnabledControlsDown(false);
        ui->pushButton_Modify->setEnabled(true);
        m_TimerID = startTimer(500);
    }
}
//更改参数按钮休息处理函数
void USBBBE_CJParaSetGUIDlg::on_pushButton_Modify_clicked()
{
    if(ui->pushButton_Modify->text() == "更改")
    {
        EnabledControls(true);
        on_comboBox_FuzDis_currentIndexChanged(ui->comboBox_FuzDis->currentIndex());
        on_comboBox_DisForType_currentIndexChanged(ui->comboBox_DisForType->currentIndex());
        killTimer(m_TimerID);
        ui->pushButton_Modify_down->setEnabled(false);
        ui->pushButton_Set_down->setEnabled(false);
        ui->lineEdit_ResponsorZero->setEnabled(false);
        return;
    }

    if(ui->pushButton_Modify->text() == "恢复")
    {
        EnabledControls(false);
        ui->pushButton_Modify_down->setEnabled(true);
        ui->pushButton_Set_down->setEnabled(true);
        m_TimerID = startTimer(500);
        return;
    }
}
//down
void USBBBE_CJParaSetGUIDlg::on_pushButton_Modify_down_clicked()
{

    if(ui->pushButton_Modify_down->text() == "更改")
    {
        EnabledControlsDown(true);
        ui->pushButton_Modify->setEnabled(false);
        ui->pushButton_Set->setEnabled(false);
        killTimer(m_TimerID);
        return;
    }

    if(ui->pushButton_Modify_down->text() == "恢复")
    {
        EnabledControlsDown(false);
        ui->pushButton_Modify->setEnabled(true);
        ui->pushButton_Set->setEnabled(true);
        m_TimerID = startTimer(500);
        return;
    }
}
