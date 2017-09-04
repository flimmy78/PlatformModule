#include "SK_BBEDivCombaraSetGUIDlg.h"
#include "ui_SK_BBEDivCombaraSetGUIDlg.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDateTime>
#include "../../Common/Log.h"

extern quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
                     uchar ucSn);
SK_BBEDivCombaraSetGUIDlg::SK_BBEDivCombaraSetGUIDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SK_BBEDivCombaraSetGUIDlg)
{
    ui->setupUi(this);
    m_pStateMgr = NULL;
    m_pILxTsspMessageManager = NULL;
    m_pILxTsspLogManager = NULL;
    m_pPublicInfo = NULL;
    m_pHAManager = NULL;
    m_pMCSDivComStatus = NULL;

    setAttribute(Qt::WA_DeleteOnClose);

    //setMinimumSize(800, 600);
}

SK_BBEDivCombaraSetGUIDlg::~SK_BBEDivCombaraSetGUIDlg()
{
    killTimer(m_TimerID);
    delete ui;
}

void SK_BBEDivCombaraSetGUIDlg::setText(const QString& strTitle,ParaStruct xmlsetText[3][60],QString MapMean[50][30],int MapNum[50],uchar Type,ILxTsspParameterMacro* pMacroObj)
{
    m_strTitle = strTitle;
    setWindowTitle(strTitle);

    memcpy(m_DlgParaSetStruct,xmlsetText,sizeof(ParaStruct)*50*3);

    for(int i=0;i<50;i++)
    {
        for(int j=0;j<30;j++)
        {
            m_DlgMapMean[i][j] = MapMean[i][j];
        }
    }

    memset(DlgMapLineNum,0,50);
    for(int i=0;i<49;i++)
    {
        DlgMapLineNum[i] = MapNum[i];
    }

    //初始化界面
    InitDLG(Type,pMacroObj);
}

//初始化对话框
void SK_BBEDivCombaraSetGUIDlg::InitDLG(int Type,ILxTsspParameterMacro* pMacroObj)
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
            DWORD stLen = sizeof(MCSDivComStatusStruct);
            m_pMCSDivComStatus =(MCSDivComStatusStruct*)m_pStateMgr->FindOneItem(m_StatusMCSDivComSynID, stLen);
            if(m_pMCSDivComStatus == NULL || stLen != sizeof(MCSDivComStatusStruct))
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
            pMacroObj->GetParameterBlock(m_ParaMCSDivComSynID, byteArray);
            memcpy(&m_MCSDivComPara, byteArray.data(), sizeof(MCSDivComParamStruct));
        }
        ui->pushButton_Modify->setVisible(false);
        ui->pushButton_Set->setVisible(false);

        //显示参数
        ShowPara(1);
    }
}

//设置输入格式
void SK_BBEDivCombaraSetGUIDlg::InitInput()
{
    QRegExp regExp("[0-9]{1,4}(\\.[1-9])?");
    ui->lineEdit_CodeRate->setValidator(new QRegExpValidator(regExp,this));
}

void SK_BBEDivCombaraSetGUIDlg::ShowControls()
{
    //向下拉框控件中写入从xml中读取的内容
    int i;
    //合成方式
    for(i=0; i<DlgMapLineNum[0];i++)
    {
        ui->comboBox_codeType->addItem(m_DlgMapMean[0][i]);
    }
    //差模环路带宽
    QString strVal = QString::number(m_DlgParaSetStruct[0][1].InitVal.ival);
    ui->lineEdit_CodeRate->setText(strVal);

    //往静态控件中写入参数范围,设置界面参数项显示范围
    QString strTemp;
    /********************************差模环路带宽*********************************/
    //差模环路带宽
    strTemp = "差模环路带宽\n[" + QString::number(m_DlgParaSetStruct[0][1].MinVal.fmin) + "~" +
            QString::number(m_DlgParaSetStruct[0][1].MaxVal.fmax) + "]" + "Hz";
    ui->label_CodeRate->setText(strTemp);
    /********************************差模环路带宽*********************************/
}

//显示参数
void SK_BBEDivCombaraSetGUIDlg::ShowPara(int type)
{
    if(type == 0)       //参数设置
    {
        if(m_pMCSDivComStatus != NULL)
        {
            m_MCSDivComPara = m_pMCSDivComStatus->tParams;
        }
        else
        {
            DWORD stLen = sizeof(MCSDivComStatusStruct);
            if(m_pStateMgr != NULL)
                m_pMCSDivComStatus  = (MCSDivComStatusStruct*)m_pStateMgr->FindOneItem(m_StatusMCSDivComSynID, stLen);
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
    /********************************分集合成参数*********************************/
    //合成方式
    ui->comboBox_codeType->setCurrentIndex(m_MCSDivComPara.ucCombType - 1);
    if (m_MCSDivComPara.ucCombType == 8)
    {
        ui->comboBox_codeType->setCurrentIndex(6);
    }
    //差模环路带宽
    ui->lineEdit_CodeRate->setText(QString::number(m_MCSDivComPara.usDiffPllBW / 10.0, 'f', 1));
    /********************************分集合成参数*********************************/
}

//设置帧长范围
void SK_BBEDivCombaraSetGUIDlg::SetFrameLenRange(int iWordLen)
{  
}

void SK_BBEDivCombaraSetGUIDlg::SetObjInfo(TLxTsspObjectInfo ObjectInfo)
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
        QString strInfo = QString("load device map failed %1");
        CLog::addLog(strInfo);
        return;
    }

    ITEM *pItem = m_DeviceMap.getItem(MODULENAME);
    if (pItem->vSubItem.size() > 0)
    {
        int index;
        switch (m_ObjectInfo.usLocalDID) {
        case 0x400:
            index = 0;
            break;
        case 0x401:
            index = 1;
            break;
        default:
            break;
        }
        SUBITEM sItem = pItem->vSubItem.at(index);
        m_usC = sItem.usCMD;
        m_usTID = sItem.usTID;
        m_ucSN = sItem.ucSN;
        /*************************框架**********************************/
        m_ParaMCSDivComSynID = GetID(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID,
                                     sItem.usTID, sItem.ucSN);
        /*************************框架**********************************/

        /*************************状态**********************************/
        m_StatusMCSDivComSynID = GetID(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID,
                                       sItem.usStateType, sItem.ucSN);
        /*************************状态**********************************/
    }

    //日志
    m_pILxTsspLogManager = pObjectManager->GetLogManager();
    m_pStateMgr = pObjectManager->GetStateManager();
    m_pILxTsspMessageManager = pObjectManager->GetMessageManager();
    m_pPublicInfo = pObjectManager->GetPublicInfoManager();
    on_comboBox_codeType_currentIndexChanged(ui->comboBox_codeType->currentIndex());
}

//使能或不使能控件
void SK_BBEDivCombaraSetGUIDlg::EnabledControls(bool bFlag)
{
    /*************************分集合成参数*************************/
    //码型
    ui->comboBox_codeType->setEnabled(bFlag);
    //码率
    ui->lineEdit_CodeRate->setEnabled(bFlag);
    /*************************分集合成参数*************************/
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
int SK_BBEDivCombaraSetGUIDlg::SaveToMacro(ILxTsspParameterMacro* pMacroObj)
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

        //分集合成参数
        byteArray.resize(sizeof(m_MCSDivComPara));
        memcpy(byteArray.data(), &m_MCSDivComPara, sizeof(m_MCSDivComPara));
        pMacroObj->UpdateParameterBlock(m_ParaMCSDivComSynID, byteArray);

        //日志信息
        QString strLog = QString(tr("调制参数设置: 保存宏成功！"));
        CLog::addLog(strLog);
    }
}

//从界面取参数
void SK_BBEDivCombaraSetGUIDlg::GetPara()
{
    /***********************************分集合成参数****************************/
    //
    m_MCSDivComPara.ucCombType = ui->comboBox_codeType->currentIndex() + 1;
    //
    m_MCSDivComPara.usDiffPllBW = qRound(ui->lineEdit_CodeRate->text().toDouble() * 10);
    /***********************************分集合成参数****************************/
}

//验证参数范围
int SK_BBEDivCombaraSetGUIDlg::CheckPara()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(MODULENAME);
    /***********************************码同步****************************/
    if(qRound(ui->lineEdit_CodeRate->text().toDouble() * 10) < m_DlgParaSetStruct[0][1].MinVal.fmin * 10
            || qRound(ui->lineEdit_CodeRate->text().toDouble() * 10) > m_DlgParaSetStruct[0][1].MaxVal.fmax * 10)
    {
        msgBox.setText(QString("差模环路带宽范围是%1～%2!")
                       .arg(m_DlgParaSetStruct[0][1].MinVal.fmin).arg(m_DlgParaSetStruct[0][1].MaxVal.fmax));
        msgBox.exec();
        return -1;
    }
    /***********************************码同步****************************/
    return 1;
}

//定时器
void SK_BBEDivCombaraSetGUIDlg::timerEvent(QTimerEvent *event)
{
    ShowPara(0);
}

void SK_BBEDivCombaraSetGUIDlg::on_pushButton_Modify_clicked()
{
    if(ui->pushButton_Modify->text() == "更改")
    {
        EnabledControls(true);
        on_comboBox_codeType_currentIndexChanged(ui->comboBox_codeType->currentIndex());
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

void SK_BBEDivCombaraSetGUIDlg::on_pushButton_Set_clicked()
{
    //验证参数范围
    if(CheckPara() == 1)
    {
        //从界面取参数
        GetPara();

        //设置参数
        //取当前设置的参数
        if(m_pHAManager!=NULL)
        {
            QByteArray byteArray;
            //分集合成参数
            byteArray.resize(sizeof(m_MCSDivComPara));
            memcpy(byteArray.data(), &m_MCSDivComPara, sizeof(m_MCSDivComPara));
            m_pHAManager->SetParameter(m_ParaMCSDivComSynID, byteArray);
            SendMessage((char*)&m_MCSDivComPara, sizeof(MCSDivComParamStruct), m_usC, m_usTID, m_ucSN);
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

void SK_BBEDivCombaraSetGUIDlg::closeEvent(QCloseEvent *event)
{
    event->accept();
}

QSize SK_BBEDivCombaraSetGUIDlg::sizeHint() const
{
    return QSize(this->width(), this->height());
}

void SK_BBEDivCombaraSetGUIDlg::SendMessage(char* pData, unsigned short  usLen, unsigned short usC, unsigned short usTID, unsigned char ucSN)
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
    delete[] message.pData;
}

DWORDLONG SK_BBEDivCombaraSetGUIDlg::StrToSynCode(const char * pStr)
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
        else if(c>='0' && c<='9')                        //the char is a number between 1 and 9
        {
            DigVal=c-'0';
            dwlValue=dwlValue*16+DigVal;               //Accumulate the value of the hexal number so far to the current digital
            nDigCount++;
            c = *p++;
        }
        else if((C=toupper(c))>='A' && C<='F')           //the char is a letter could be a digital of a hexal number
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

void SK_BBEDivCombaraSetGUIDlg::on_comboBox_codeType_currentIndexChanged(int index)
{
    if (ui->pushButton_Modify->text() == "更改")
        return;
    ui->lineEdit_CodeRate->setEnabled(false);
    if(index==0)
        ui->lineEdit_CodeRate->setEnabled(true);

}
