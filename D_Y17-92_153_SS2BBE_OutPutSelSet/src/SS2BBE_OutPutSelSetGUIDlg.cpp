#include "SS2BBE_OutPutSelSetGUIDlg.h"
#include "ui_SS2BBE_OutPutSelSetGUIDlg.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDateTime>
#include "../../Common/Log.h"
#include <QTimer>
#pragma pack(1)
extern quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
              uchar ucSn);
SS2BBE_OutPutSelSetGUIDlg::SS2BBE_OutPutSelSetGUIDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SS2BBE_OutPutSelSetGUIDlg)
{
    ui->setupUi(this);
    m_pStateMgr = NULL;
    m_pILxTsspMessageManager = NULL;
    m_pILxTsspLogManager = NULL;
    m_pPublicInfo = NULL;
    m_pHAManager = NULL;
    m_MCSSysStatus = NULL;
    m_timer1 = new QTimer(this);
    connect(m_timer1, SIGNAL(timeout()), this, SLOT(updateTarget()));
    m_targetNum = 4;
    setAttribute(Qt::WA_DeleteOnClose);
}
void SS2BBE_OutPutSelSetGUIDlg::updateTarget()
{
    if(!m_pCommFunc)return;
    int target = m_pCommFunc->getTargetNum();
    if(target == m_targetNum)return;
    m_targetNum = target;
    int index = ui->comboBox_OutputTarget->currentIndex();
    ui->comboBox_OutputTarget->clear();
    switch (target) {
    case 2:
        ui->comboBox_OutputTarget->addItem(m_strListOutTargets[0]);
        ui->comboBox_OutputTarget->addItem(m_strListOutTargets[1]);
        break;
    case 3:
        ui->comboBox_OutputTarget->addItem(m_strListOutTargets[0]);
        ui->comboBox_OutputTarget->addItem(m_strListOutTargets[1]);
        ui->comboBox_OutputTarget->addItem(m_strListOutTargets[2]);
        break;
    case 4:
        ui->comboBox_OutputTarget->addItem(m_strListOutTargets[0]);
        ui->comboBox_OutputTarget->addItem(m_strListOutTargets[1]);
        ui->comboBox_OutputTarget->addItem(m_strListOutTargets[2]);
        ui->comboBox_OutputTarget->addItem(m_strListOutTargets[3]);
        break;
    default:
        ui->comboBox_OutputTarget->addItem(m_strListOutTargets[0]);
        break;
    }
    ui->comboBox_OutputTarget->setCurrentIndex(target - index >= 1 ? index : 0);
}
SS2BBE_OutPutSelSetGUIDlg::~SS2BBE_OutPutSelSetGUIDlg()
{
    killTimer(m_TimerID);
    m_timer1->stop();
    delete m_timer1;
    delete ui;
}

void SS2BBE_OutPutSelSetGUIDlg::setText(const QString& strTitle, ParaStruct xmlsetText[3][60], QString MapMean[50][30], int MapNum[50], uchar Type, ILxTsspParameterMacro* pMacroObj)
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
void SS2BBE_OutPutSelSetGUIDlg::InitDLG(int Type, ILxTsspParameterMacro* pMacroObj)
{
    ui->groupBox->hide();
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
            DWORD stLen = sizeof(SMCU2::MCSSysTarget4StatusStruct);
            m_MCSSysStatus = (SMCU2::MCSSysTarget4StatusStruct*)m_pStateMgr->FindOneItem(m_StatusParaSysID, stLen);
            if(m_MCSSysStatus == NULL || stLen != sizeof(SMCU2::MCSSysTarget4StatusStruct))
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
            pMacroObj->GetParameterBlock(m_ParaSysID, byteArray);
            memcpy(&m_MCSSysParam, byteArray.data(), sizeof(SMCU2::MCSSysParamStruct4));
        }
        ui->pushButton_Modify->setVisible(false);
        ui->pushButton_Set->setVisible(false);

        on_comboBox_OutputMd_currentIndexChanged(ui->comboBox_OutputMd->currentIndex());
        m_pCommFunc = CCommFunc::getInstance();
        updateOutTargets();
        m_timer1->start(500);
        //显示参数
        ShowPara(1);
    }
}

//设置输入格式
void SS2BBE_OutPutSelSetGUIDlg::InitInput()
{

    QRegExp regExp("[0-9]{1,16}");
    //保持门限
    ui->lineEdit_KeepThresold->setValidator(new QRegExpValidator(regExp, this));
    //判决时间
    ui->lineEdit_ProPeriod->setValidator(new QRegExpValidator(regExp, this));
    //输出门限
    ui->lineEdit_OutputThresold->setValidator(new QRegExpValidator(regExp, this));

    //字母数字
    QRegExp regExp2("[A-Fa-f0-9]{1,8}");
    /*//帧同步码组[HEX]
    ui->lineEdit_ZTMZ->setValidator(new QRegExpValidator(regExp2, this));
    //副帧同步码组[HEX]
    ui->lineEdit_FZTBMZ->setValidator(new QRegExpValidator(regExp2, this));*/
}

void SS2BBE_OutPutSelSetGUIDlg::ShowControls()
{
    int i;
    //输出方式
    for(i=0; i<DlgMapLineNum[0]; i++)
    {
        ui->comboBox_OutputMd->addItem(m_DlgMapMean[0][i]);
    }
    ui->comboBox_OutputMd->setCurrentIndex(m_DlgParaSetStruct[0][0].InitVal.lval);
    //输出目标
    for(i=0; i<DlgMapLineNum[1]; i++)
    {
        m_strListOutTargets << m_DlgMapMean[1][i];

    }
    ui->comboBox_OutputTarget->addItems(m_strListOutTargets);
    ui->comboBox_OutputTarget->setCurrentIndex(m_DlgParaSetStruct[0][1].InitVal.lval);
    //输出通道
    for(i=0; i<DlgMapLineNum[2]; i++)
    {
        ui->comboBox_OutputChannel->addItem(m_DlgMapMean[2][i]);
    }
    ui->comboBox_OutputChannel->setCurrentIndex(m_DlgParaSetStruct[0][2].InitVal.lval);
    //保持门限
    ui->lineEdit_KeepThresold->setText(QString::number(m_DlgParaSetStruct[0][3].InitVal.lval));
    //判决时间
    ui->lineEdit_ProPeriod->setText(QString::number(m_DlgParaSetStruct[0][4].InitVal.lval));
    //输出门限
    ui->lineEdit_OutputThresold->setText(QString::number(m_DlgParaSetStruct[0][5].InitVal.lval));
}

//显示参数
void SS2BBE_OutPutSelSetGUIDlg::ShowPara(int type)
{
    if(type == 0)       //参数设置
    {
        if(m_MCSSysStatus != NULL)
        {
            m_MCSSysParam.MCSSysParam = m_MCSSysStatus->MCSSysStatu.tParams;
        }
        else
        {
            DWORD stLen = sizeof(SMCU2::MCSSysTarget4StatusStruct);
            if(m_pStateMgr != NULL)
                m_MCSSysStatus  = (SMCU2::MCSSysTarget4StatusStruct*)m_pStateMgr->FindOneItem(m_StatusParaSysID, stLen);
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
    //输出方式
    ui->comboBox_OutputMd->setCurrentIndex(m_MCSSysParam.MCSSysParam.ucTrackOutType - 1);
    //输出目标
    ui->comboBox_OutputTarget->setCurrentIndex(m_MCSSysParam.MCSSysParam.ucTrackDefCH - 1);
    //输出通道
    ui->comboBox_OutputChannel->setCurrentIndex(m_MCSSysParam.MCSSysParam.ucTrackType - 1);//comboBox_OutputChannel
    updateOutTargets();
}

//设置帧长范围
void SS2BBE_OutPutSelSetGUIDlg::SetFrameLenRange(int iWordLen)
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

void SS2BBE_OutPutSelSetGUIDlg::SetObjInfo(TLxTsspObjectInfo ObjectInfo)
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
        m_ParaSysID = GetID(sItem.usStationID,
                                         sItem.ucSubSysID, sItem.usDeviceID,sItem.usTID,
                                          sItem.ucSN);
        /*************************框架**********************************/

        /*************************状态**********************************/
        m_StatusParaSysID = GetID(sItem.usStationID,
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
void SS2BBE_OutPutSelSetGUIDlg::EnabledControls(bool bFlag)
{
    /*************************Group1*************************/
    //输出方式
    ui->comboBox_OutputMd->setEnabled(bFlag);
    //输出通道
    ui->comboBox_OutputChannel->setEnabled(bFlag);
    //输出目标
    ui->comboBox_OutputTarget->setEnabled(bFlag);
    //保持门限
    ui->lineEdit_KeepThresold->setEnabled(false);
    //判决时间
    ui->lineEdit_ProPeriod->setEnabled(false);
    //输出门限
    ui->lineEdit_OutputThresold->setEnabled(false);
    /*************************Group1*************************/

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
int SS2BBE_OutPutSelSetGUIDlg::SaveToMacro(ILxTsspParameterMacro* pMacroObj)
{
    //判断合法性
    int iRet = CheckPara();
    if(iRet == -1)
    {
        return -1;
    }
    else
    {
        QByteArray byteArray;
        byteArray.resize(sizeof(m_MCSSysParam));
        if(1 == pMacroObj->GetParameterBlock(m_ParaSysID, byteArray))
            memcpy(&m_MCSSysParam, byteArray.data(), sizeof(m_MCSSysParam));
        GetPara();
        //取当前设置的参数
        memcpy(byteArray.data(), &m_MCSSysParam, sizeof(m_MCSSysParam));
        pMacroObj->UpdateParameterBlock(m_ParaSysID, byteArray);

        //日志信息
        QString strLog;
        strLog += QString("%1: 保存宏成功！").arg(MODULENAME);
        CLog::addLog(strLog);
    }
}

//从界面取参数
void SS2BBE_OutPutSelSetGUIDlg::GetPara()
{
    //输出方式
    m_MCSSysParam.MCSSysParam.ucTrackOutType = ui->comboBox_OutputMd->currentIndex() + 1;
    //输出目标
    m_MCSSysParam.MCSSysParam.ucTrackDefCH = ui->comboBox_OutputTarget->currentIndex() + 1;
    //输出通道
    m_MCSSysParam.MCSSysParam.ucTrackType = ui->comboBox_OutputChannel->currentIndex() + 1;
}

//验证参数范围
int SS2BBE_OutPutSelSetGUIDlg::CheckPara()
{
    QMessageBox msgBox;
    /*//距离预报值
    if(ui->lineEdit_DisFor->text() == "")
    {
        msgBox.setText("距离预报值不能为空，请重新输入!");
        msgBox.exec();
        return -1;
    }
    //应答机距离零值
    if(ui->lineEdit_ResponsorZeroPos->text() == "")
    {
        msgBox.setText("应答机距离零值不能为空，请重新输入!");
        msgBox.exec();
        return -1;
    }*/
    return 1;
}

////定时器
void SS2BBE_OutPutSelSetGUIDlg::timerEvent(QTimerEvent *event)
{
    ShowPara(0);
}

//更改参数按钮休息处理函数
void SS2BBE_OutPutSelSetGUIDlg::on_pushButton_Modify_clicked()
{
    if(ui->pushButton_Modify->text() == "更改")
    {
        EnabledControls(true);
        on_comboBox_OutputMd_currentIndexChanged(ui->comboBox_OutputMd->currentIndex());
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

void SS2BBE_OutPutSelSetGUIDlg::on_pushButton_Set_clicked()
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
                   USHORT tmp;//占位
                   SMCU2::MCSSysParamStruct sys;//公共参数
                  }data;
            QByteArray byteArray;
            byteArray.resize(sizeof(data));
            data.target = 1;
            data.sys=m_MCSSysParam.MCSSysParam;
            memcpy(byteArray.data(),&data,sizeof(data));
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

void SS2BBE_OutPutSelSetGUIDlg::closeEvent(QCloseEvent *event)
{
    event->accept();
}

QSize SS2BBE_OutPutSelSetGUIDlg::sizeHint() const
{
    return QSize(this->width(), this->height());
}

/*
//副帧方式响应函数
void SS2BBE_OutPutSelSetGUIDlg::on_comboBox_SubFrmType_currentIndexChanged(int index)
{
//    //副帧方式控件使能
//    if (ui->pushButton_Modify->text() != "更改" || iParaSetFlag != 0)
//    {
//        EnabledControlsSubFrmType(index);   //副帧方式控件使能
//    }
}

//根据副帧类型，使能副帧控件
void SS2BBE_OutPutSelSetGUIDlg::EnabledControlsSubFrmType(int iSubFrm)
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
}*/

void SS2BBE_OutPutSelSetGUIDlg::SubFrmEn(bool bFlag)
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
void SS2BBE_OutPutSelSetGUIDlg::IDSubFrmEn(bool bFlag)
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
void SS2BBE_OutPutSelSetGUIDlg::CycSubFrmEn(bool bFlag)
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
void SS2BBE_OutPutSelSetGUIDlg::ReserveSubFrmEn(bool bFlag)
{

}

void SS2BBE_OutPutSelSetGUIDlg::SendMessage(char* pData, unsigned short  usLen, unsigned short usC, unsigned short usTID)
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

DWORDLONG SS2BBE_OutPutSelSetGUIDlg::StrToSynCode(const char * pStr)
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


void SS2BBE_OutPutSelSetGUIDlg::on_comboBox_OutputMd_currentIndexChanged(int index)
{
    if(ui->pushButton_Modify->text() == "更改")return;
    bool enable = false;
    if(index == 0)enable = true;
    else enable = false;

    ui->comboBox_OutputTarget->setEnabled(enable);
    ui->comboBox_OutputChannel->setEnabled(enable);
}

void SS2BBE_OutPutSelSetGUIDlg::updateOutTargets()
{
    int target = m_MCSSysStatus ? m_MCSSysStatus->MCSSysStatu.tParams.ucTargetNum : 4;
    ui->comboBox_OutputTarget->clear();
    switch (target) {
    case 2:
        ui->comboBox_OutputTarget->addItem(m_strListOutTargets[0]);
        ui->comboBox_OutputTarget->addItem(m_strListOutTargets[1]);
        break;
    case 3:
        ui->comboBox_OutputTarget->addItem(m_strListOutTargets[0]);
        ui->comboBox_OutputTarget->addItem(m_strListOutTargets[1]);
        ui->comboBox_OutputTarget->addItem(m_strListOutTargets[2]);
        break;
    case 4:
        ui->comboBox_OutputTarget->addItem(m_strListOutTargets[0]);
        ui->comboBox_OutputTarget->addItem(m_strListOutTargets[1]);
        ui->comboBox_OutputTarget->addItem(m_strListOutTargets[2]);
        ui->comboBox_OutputTarget->addItem(m_strListOutTargets[3]);
        break;
    default:
        ui->comboBox_OutputTarget->addItem(m_strListOutTargets[0]);
        break;
    }
    target = m_MCSSysParam.MCSSysParam.ucTrackDefCH;
    ui->comboBox_OutputTarget->setCurrentIndex(target ? target - 1 : 0);
}
#pragma pack()

