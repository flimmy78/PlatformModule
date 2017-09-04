#include "USBBBE_TMCJRecSetGUIDlg.h"
#include "ui_USBBBE_TMCJRecSetGUIDlg.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDateTime>

extern quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
              uchar ucSn);
USBBBE_TMCJRecSetGUIDlg::USBBBE_TMCJRecSetGUIDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::USBBBE_TMCJRecSetGUIDlg)
{
    ui->setupUi(this);
    m_pStateMgr = NULL;
    m_pILxTsspMessageManager = NULL;
    m_pILxTsspLogManager = NULL;
    m_pPublicInfo = NULL;
    m_pHAManager = NULL;

    m_MCSUSBRevStatus = NULL;
    setAttribute(Qt::WA_DeleteOnClose);
}

USBBBE_TMCJRecSetGUIDlg::~USBBBE_TMCJRecSetGUIDlg()
{
    killTimer(m_TimerID);
    delete ui;
}

void USBBBE_TMCJRecSetGUIDlg::setText(const QString& strTitle, ParaStruct xmlsetText[3][60], QString MapMean[50][30], int MapNum[50], uchar Type, ILxTsspParameterMacro* pMacroObj)
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
    for(int i=0; i < 49; i++)
    {
        DlgMapLineNum[i] = MapNum[i];
    }
    //初始化界面
    InitDLG(Type, pMacroObj);
}

//初始化对话框
void USBBBE_TMCJRecSetGUIDlg::InitDLG(int Type, ILxTsspParameterMacro* pMacroObj)
{
    iParaSetFlag = Type;
    //设置输入格式
    InitInput();
    //显示变量名字和范围
    ShowControls();

    if(Type == 0)//取当前参数
    {
        //取当前设置的参数
        QByteArray byteArray;
        if(m_pHAManager != NULL)
        {
            m_pHAManager->GetParameter(m_ParaMCSUSBRevID, byteArray);
            memcpy(&m_MCSUSBRevParam, byteArray.data(), sizeof(MCSUSBRevParamStruct));
        }
        if(m_pStateMgr != NULL)
        {
            DWORD stLen = sizeof(MCSUSBRevStatusStruct);
            m_MCSUSBRevStatus = (MCSUSBRevStatusStruct*)m_pStateMgr->FindOneItem(m_StatusParaMCSUSBRevID, stLen);
            if(m_MCSUSBRevStatus == NULL || stLen != sizeof(MCSUSBRevStatusStruct))
                return;
        }
        //启动定时器
        m_TimerID = startTimer(500);
        //不使能
        EnabledControls(false);
    }
    else if(Type == 1)//取宏里参数
    {
        if(pMacroObj!=NULL)
        {
            QByteArray byteArray;
            pMacroObj->GetParameterBlock(m_ParaMCSUSBRevID, byteArray);
            memcpy(&m_MCSUSBRevParam, byteArray.data(), sizeof(MCSUSBRevParamStruct));
        }
        ui->pushButton_Modify->setVisible(false);
        ui->pushButton_Set->setVisible(false);

        //显示参数
        ShowPara(1);
    }
}

//设置输入格式
void USBBBE_TMCJRecSetGUIDlg::InitInput()
{
    //正整数
    QRegExp regExp("([6-7][0-9])(\\.[0-9]{1,3})?");// 小数点后保留3位
    //输入载波中频
    ui->lineEdit_InputWavF->setValidator(new QRegExpValidator(regExp, this));
}

void USBBBE_TMCJRecSetGUIDlg::ShowControls()
{
    //向下拉框控件中写入从xml中读取的内容
    int i;
    //接收通道
    for(i=0; i<DlgMapLineNum[0];i++)
    {
        ui->comboBox_RecieveChannel->addItem(m_DlgMapMean[0][i]);
    }
    ui->comboBox_RecieveChannel->setCurrentIndex(m_DlgParaSetStruct[0][0].InitVal.lval);
    //载波环路带宽
    for(i=0; i<DlgMapLineNum[1];i++)
    {
        ui->comboBox_CodeBn->addItem(m_DlgMapMean[1][i]);
    }
    ui->comboBox_CodeBn->setCurrentIndex(m_DlgParaSetStruct[0][1].InitVal.lval);
    //AGC时间常数
    for(i=0; i<DlgMapLineNum[2];i++)
    {
        ui->comboBox_AGCTime->addItem(m_DlgMapMean[2][i]);
    }
    ui->comboBox_AGCTime->setCurrentIndex(m_DlgParaSetStruct[0][2].InitVal.lval);
    //输入载波中频
    ui->lineEdit_InputWavF->setText(QString::number(m_DlgParaSetStruct[0][3].InitVal.lval / 1000.0));

    QString strTemp;

    /********************************输入载波中频*********************************/
    strTemp = "输入载波中频\n[" + QString::number(m_DlgParaSetStruct[0][3].MinVal.lmin / 1000000.0) + "~" +
            QString::number(m_DlgParaSetStruct[0][3].MaxVal.lmax / 1000000.0)+"]"+"MHz";
    ui->label_InputWavF->setText(strTemp);
    /********************************输入载波中频*********************************/
}

//显示参数
void USBBBE_TMCJRecSetGUIDlg::ShowPara(int type)
{
    if(type == 0)       //参数设置
    {
        //码同步
        if(m_MCSUSBRevStatus != NULL)
        {
            m_MCSUSBRevParam = m_MCSUSBRevStatus->tParams;
        }
        else
        {
            DWORD stLen = sizeof(MCSUSBRevStatusStruct);
            if(m_pStateMgr != NULL)
                m_MCSUSBRevStatus  = (MCSUSBRevStatusStruct*)m_pStateMgr->FindOneItem(m_StatusParaMCSUSBRevID, stLen);
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
    /********************************参数设置*********************************/
    //接收通道
    if (m_MCSUSBRevParam.ucRevChannel > 0)
        ui->comboBox_RecieveChannel->setCurrentIndex(m_MCSUSBRevParam.ucRevChannel - 1);
    //载波环路带宽
    if (m_MCSUSBRevParam.ucPllBw > 0)
        ui->comboBox_CodeBn->setCurrentIndex(m_MCSUSBRevParam.ucPllBw - 1);
    //AGC时间常数
    if (m_MCSUSBRevParam.ucAgcTime > 0)
        ui->comboBox_AGCTime->setCurrentIndex(m_MCSUSBRevParam.ucAgcTime - 1);
    //输入载波中频
    ui->lineEdit_InputWavF->setText(QString::number(m_MCSUSBRevParam.uiCarrIF / 1000000.0));
}

//设置帧长范围
void USBBBE_TMCJRecSetGUIDlg::SetFrameLenRange(int iWordLen)
{
   /* if(iWordLen == 8)
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

void USBBBE_TMCJRecSetGUIDlg::SetObjInfo(TLxTsspObjectInfo ObjectInfo)
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
    if (pItem->vSubItem.size() > 0)
    {
        SUBITEM& sItem = *(pItem->getSubItemByDeviceID_SN(m_ObjectInfo.usLocalDID, m_ObjectInfo.ucSN));
        m_usC = sItem.usCMD;
        m_usTID = sItem.usTID;
        /*************************框架**********************************/
        m_ParaMCSUSBRevID = GetID(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID,
                                     sItem.usTID, sItem.ucSN);
        /*************************框架**********************************/

        /*************************状态**********************************/
        m_StatusParaMCSUSBRevID = GetID(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID,
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
void USBBBE_TMCJRecSetGUIDlg::EnabledControls(bool bFlag)
{
    /*************************Group1*************************/
    //接收通道
    ui->comboBox_RecieveChannel->setEnabled(bFlag);
    //载波环路带宽
    ui->comboBox_CodeBn->setEnabled(bFlag);
    //AGC时间常数
    ui->comboBox_AGCTime->setEnabled(bFlag);
    //输入载波中频
    ui->lineEdit_InputWavF->setEnabled(bFlag);
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
int USBBBE_TMCJRecSetGUIDlg::SaveToMacro(ILxTsspParameterMacro* pMacroObj)
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
        byteArray.resize(sizeof(m_MCSUSBRevParam));
        memcpy(byteArray.data(),&m_MCSUSBRevParam,sizeof(m_MCSUSBRevParam));
        pMacroObj->UpdateParameterBlock(m_ParaMCSUSBRevID,byteArray);

        //日志信息
        QString strLog;
        //tmpLog.ucWarningLevel  = 1;
        strLog += "数传解调参数设置: 保存宏成功！";
        CLog::addLog(strLog);
    }
}
//从界面取参数
void USBBBE_TMCJRecSetGUIDlg::GetPara()
{
    //接收通道
    m_MCSUSBRevParam.ucRevChannel = ui->comboBox_RecieveChannel->currentIndex() + 1;
    //AGC时间
    m_MCSUSBRevParam.ucAgcTime = ui->comboBox_AGCTime->currentIndex() + 1;
    //环路带宽s
    m_MCSUSBRevParam.ucPllBw = ui->comboBox_CodeBn->currentIndex() + 1;
    //输入中频
    m_MCSUSBRevParam.uiCarrIF = qRound(ui->lineEdit_InputWavF->text().toDouble() * 1000000);
}

//验证参数范围
int USBBBE_TMCJRecSetGUIDlg::CheckPara()
{
    QMessageBox msgBox;
    /***********************************输入载波中频****************************/
    //输入载波中频
    if(ui->lineEdit_InputWavF->text().isEmpty())
    {
        QString strTemp = QString("输入载波中频不能为空, 请重新输入!");
        msgBox.setText(strTemp);
        msgBox.exec();
        return -1;
    }
    if(qRound(ui->lineEdit_InputWavF->text().toDouble() * 1000000) < m_DlgParaSetStruct[0][3].MinVal.lmin
            || qRound(ui->lineEdit_InputWavF->text().toDouble() * 1000000) > m_DlgParaSetStruct[0][3].MaxVal.lmax)
    {
        QString strTemp = QString("输入载波中频范围为:[%1～%2]MHz, 请重新输入!")
                .arg(m_DlgParaSetStruct[0][3].MinVal.lmin / 1000000.0).arg(m_DlgParaSetStruct[0][3].MaxVal.lmax / 1000000.0);
        msgBox.setText(strTemp);
        msgBox.exec();
        return -1;
    }
    /***********************************输入载波中频****************************/
    return 1;
}

////定时器
void USBBBE_TMCJRecSetGUIDlg::timerEvent(QTimerEvent *event)
{
    ShowPara(0);
}

//更改参数按钮休息处理函数
void USBBBE_TMCJRecSetGUIDlg::on_pushButton_Modify_clicked()
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

void USBBBE_TMCJRecSetGUIDlg::on_pushButton_Set_clicked()
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
            byteArray.resize(sizeof(m_MCSUSBRevParam));
            memcpy(byteArray.data(), &m_MCSUSBRevParam, sizeof(MCSUSBRevParamStruct));
            SendMessage(byteArray.data(), byteArray.size(), m_usC, m_usTID);
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

void USBBBE_TMCJRecSetGUIDlg::closeEvent(QCloseEvent *event)
{
    event->accept();
}

QSize USBBBE_TMCJRecSetGUIDlg::sizeHint() const
{
    return QSize(this->width(), this->height());
}

//根据副帧类型，使能副帧控件
void USBBBE_TMCJRecSetGUIDlg::EnabledControlsSubFrmType(int iSubFrm)
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
//        case 2:     //循环副帧
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
}

void USBBBE_TMCJRecSetGUIDlg::SubFrmEn(bool bFlag)
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
void USBBBE_TMCJRecSetGUIDlg::IDSubFrmEn(bool bFlag)
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
void USBBBE_TMCJRecSetGUIDlg::CycSubFrmEn(bool bFlag)
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
void USBBBE_TMCJRecSetGUIDlg::ReserveSubFrmEn(bool bFlag)
{

}

void USBBBE_TMCJRecSetGUIDlg::SendMessage(char* pData, unsigned short  usLen, unsigned short usC,unsigned short usTID)
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

DWORDLONG USBBBE_TMCJRecSetGUIDlg::StrToSynCode(const char * pStr)
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
