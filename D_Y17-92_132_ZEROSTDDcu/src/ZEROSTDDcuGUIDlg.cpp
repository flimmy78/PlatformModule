#include "ZEROSTDDcuGUIDlg.h"
#include "ui_ZEROSTDDcuGUIDlg.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDateTime>

#include "../../Common/CommonFunction.h"

extern quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
              uchar ucSn);
ZEROSTDDcuGUIDlg::ZEROSTDDcuGUIDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ZEROSTDDcuGUIDlg)
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
}

ZEROSTDDcuGUIDlg::~ZEROSTDDcuGUIDlg()
{
    killTimer(m_TimerID);
    delete ui;
}

void ZEROSTDDcuGUIDlg::setText(const QString& strTitle,ParaStruct xmlsetText[3][60],QString MapMean[50][30],int MapNum[50],uchar Type,ILxTsspParameterMacro* pMacroObj)
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

    //获取要发送的C和TID
    initCmdInfo(MODULENAME);

    //初始化界面
    InitDLG(Type,pMacroObj);
}

//初始化对话框
void ZEROSTDDcuGUIDlg::InitDLG(int Type,ILxTsspParameterMacro* pMacroObj)
{
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
            DWORD stLen = sizeof(ZEROCALIBBEACONStatu);
            m_pComStatus = (ZEROCALIBBEACONStatu*)m_pStateMgr->FindOneItem(m_StatusComSynID, stLen);
            if(m_pComStatus == NULL || stLen != sizeof(ZEROCALIBBEACONStatu))
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
            memcpy(&m_ComPara, byteArray.data(), sizeof(ZEROCALIBBEACONPara));
        }
        ui->pushButton_Modify->setVisible(false);
        ui->pushButton_Set->setVisible(false);

        //显示参数
        ShowPara(1);
    }
}

//设置输入格式
void ZEROSTDDcuGUIDlg::InitInput()
{
    QRegExp regExp("[0-9]{4,4}");
    //帧同步码组
//    ui->lineEdit_FrmSyncCode->setValidator(new QRegExpValidator(regExp, this));

//    //正整数
    QRegExp regExp2("[0-9]{4,4}");
    //输入射频频率
    ui->lineEdit_InputFreq->setValidator(new QRegExpValidator(regExp2, this));
    //输出射频频率
    ui->lineEdit_OutputFreq->setValidator(new QRegExpValidator(regExp2, this));
    //信标输出频率
    ui->lineEdit_BeaconFreq->setValidator(new QRegExpValidator(regExp2, this));
    //数控衰减器1
    ui->lineEdit_NCAtten1->setValidator(new QRegExpValidator(regExp2, this));
    //数控衰减器2
    ui->lineEdit_NCAtten2->setValidator(new QRegExpValidator(regExp2, this));
}

void ZEROSTDDcuGUIDlg::ShowControls()
{
    //向下拉框控件中写入从xml中读取的内容
    //在线标记
    for (int i=0; i<DlgMapLineNum[0];i++)
    {
        ui->comboBox_OnlineMark->addItem(m_DlgMapMean[0][i]);
    }
    //输入射频频率
    QString strVal = QString::number(m_DlgParaSetStruct[0][1].InitVal.ival);
    ui->lineEdit_InputFreq->setText(strVal);
    //输出射频频率
    strVal = QString::number(m_DlgParaSetStruct[0][2].InitVal.ival);
    ui->lineEdit_OutputFreq->setText(strVal);
    //信标输出频率
    strVal = QString::number(m_DlgParaSetStruct[0][3].InitVal.ival);
    ui->lineEdit_BeaconFreq->setText(strVal);
    //数控衰减器1
    strVal = QString::number(m_DlgParaSetStruct[0][4].InitVal.ival);
    ui->lineEdit_NCAtten1->setText(strVal);
    //数控衰减器2
    strVal = QString::number(m_DlgParaSetStruct[0][5].InitVal.ival);
    ui->lineEdit_NCAtten2->setText(strVal);
    //参考源选择
    for (int i=0; i<DlgMapLineNum[1];i++)
    {
        ui->comboBox_ReferentSel->addItem(m_DlgMapMean[1][i]);
    }

    //往静态控件中写入参数范围,设置界面参数项显示范围
    QString strTemp;
    //差模环路带宽
    strTemp = "差模环路带宽\n["+QString::number(m_DlgParaSetStruct[0][1].MinVal.lmin)+"~"+
            QString::number(m_DlgParaSetStruct[0][1].MaxVal.lmax)+"]"+ "Hz";
    //ui->label_CodeRate->setText(strTemp);

    //输入射频频率
    strTemp = "输入射频频率\n["+QString::number(m_DlgParaSetStruct[0][1].MinVal.lmin)+"~"+
            QString::number(m_DlgParaSetStruct[0][1].MaxVal.lmax)+"]"+ "MHz";
    ui->label_InputFreq->setText(strTemp);
    //输出射频频率
    strTemp = "输出射频频率\n["+QString::number(m_DlgParaSetStruct[0][2].MinVal.lmin)+"~"+
            QString::number(m_DlgParaSetStruct[0][2].MaxVal.lmax)+"]"+ "MHz";
    ui->label_OutputFreq->setText(strTemp);
    //信标输出频率
    strTemp = "信标输出频率\n["+QString::number(m_DlgParaSetStruct[0][3].MinVal.lmin)+"~"+
            QString::number(m_DlgParaSetStruct[0][3].MaxVal.lmax)+"]"+ "MHz";
    ui->label_BeaconFreq->setText(strTemp);
    //数控衰减器1
    strTemp = "数控衰减器1\n["+QString::number(m_DlgParaSetStruct[0][4].MinVal.lmin)+"~"+
        QString::number(m_DlgParaSetStruct[0][4].MaxVal.lmax)+"]"+ "dB";
    ui->label_NCAtten1->setText(strTemp);
    //数控衰减器2
    strTemp = "数控衰减器2\n["+QString::number(m_DlgParaSetStruct[0][5].MinVal.lmin)+"~"+
        QString::number(m_DlgParaSetStruct[0][5].MaxVal.lmax)+"]"+ "dB";
    ui->label_NCAtten2->setText(strTemp);
}

//显示参数
void ZEROSTDDcuGUIDlg::ShowPara(int type)
{
    if(type == 0)       //参数设置
    {
        if(m_pComStatus != NULL)
        {
            m_ComPara = m_pComStatus->tParam;
        }
        else
        {
            DWORD stLen = sizeof(ZEROCALIBBEACONStatu);
            if(m_pStateMgr != NULL)
                m_pComStatus  = (ZEROCALIBBEACONStatu*)m_pStateMgr->FindOneItem(m_StatusComSynID, stLen);
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

    if (m_pComStatus == NULL)
        return;

    /********************设备是否在线************************/
    m_bOnLine = CCommFunc::getInstance()->getZeroSTDStatus();
    if (!m_bOnLine)
    {
        setOfflineStatusValue();
        return;
    }
    /********************设备是否在线************************/

    //在线标记
    ui->comboBox_OnlineMark->setCurrentIndex(m_ComPara.ucOnlineMark);
    //输入射频频率
    ui->lineEdit_InputFreq->setText(QString::number(m_ComPara.ucInputFreq/1000000));
    //输出射频频率
    ui->lineEdit_OutputFreq->setText(QString::number(m_ComPara.ucOutputFreq/1000000));
    //信标输出频率：
    ui->lineEdit_BeaconFreq->setText(QString::number(m_ComPara.ucBeaconFreq/1000000));
    //数控衰减器1
    ui->lineEdit_NCAtten1->setText(QString::number(m_ComPara.sAttenuation1));
    //数控衰减器2
    ui->lineEdit_NCAtten2->setText(QString::number(m_ComPara.sAttenuation2));
    //参考源选择
    ui->comboBox_ReferentSel->setCurrentIndex(m_ComPara.usReferentSel);
    //UCHAR ucConMode;                //控制模式：0-分控 1-本控
    if(0 == m_pComStatus->ucConMode)
        ui->lineEdit_ConMode->setText("分控");
    else if(1 == m_pComStatus->ucConMode)
        ui->lineEdit_ConMode->setText("本控");
    else
        ui->lineEdit_ConMode->setText("Error");
    //UCHAR ucSelfCheckStatu;       //自检状态：00H-正常，FFH-故障
    if(0 == m_pComStatus->ucSelfCheckStatu)
        ui->lineEdit_SelfCheckStatu->setText("正常");
    else
        ui->lineEdit_SelfCheckStatu->setText("故障");
    //UCHAR ucLockStatu;                //锁定指示：00H-失锁，01H-锁定
    if(0 == m_pComStatus->ucLockStatu)
        ui->lineEdit_LockStatu->setText("失锁");
    else
        ui->lineEdit_LockStatu->setText("锁定");
}

void ZEROSTDDcuGUIDlg::SetObjInfo(TLxTsspObjectInfo ObjectInfo)
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
void ZEROSTDDcuGUIDlg::EnabledControls(bool bFlag)
{
    //在线标记
    ui->comboBox_OnlineMark->setEnabled(bFlag);
    //输入射频频率
    ui->lineEdit_InputFreq->setEnabled(bFlag);
    //输出射频频率
    ui->lineEdit_OutputFreq->setEnabled(bFlag);
    //信标输出频率：
    ui->lineEdit_BeaconFreq->setEnabled(bFlag);
    //数控衰减器1
    ui->lineEdit_NCAtten1->setEnabled(bFlag);
    //数控衰减器2
    ui->lineEdit_NCAtten2->setEnabled(bFlag);
    //参考源选择
    ui->comboBox_ReferentSel->setEnabled(bFlag);
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

int ZEROSTDDcuGUIDlg::SaveToMacro(ILxTsspParameterMacro* pMacroObj)
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
void ZEROSTDDcuGUIDlg::GetPara()
{
    //在线标记
    m_ComPara.ucOnlineMark = ui->comboBox_OnlineMark->currentIndex();
    //输入射频频率
    m_ComPara.ucInputFreq = ui->lineEdit_InputFreq->text().toUInt()*1000000;
    //输出射频频率
    m_ComPara.ucOutputFreq = ui->lineEdit_OutputFreq->text().toUInt()*1000000;
    //信标输出频率：
    m_ComPara.ucBeaconFreq = ui->lineEdit_BeaconFreq->text().toUInt()*1000000;
    //数控衰减器1
    m_ComPara.sAttenuation1 = ui->lineEdit_NCAtten1->text().toShort();
    //数控衰减器2
    m_ComPara.sAttenuation2 = ui->lineEdit_NCAtten2->text().toShort();
    //参考源选择
    m_ComPara.usReferentSel = ui->comboBox_ReferentSel->currentIndex();
}

//验证参数范围
int ZEROSTDDcuGUIDlg::CheckPara()
{
    QMessageBox msgBox;
    //输入射频频率
    UINT uVal = ui->lineEdit_InputFreq->text().toUInt();
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
    }

    return 1;
}

//定时器
void ZEROSTDDcuGUIDlg::timerEvent(QTimerEvent *event)
{
    ShowPara(0);
}

void ZEROSTDDcuGUIDlg::on_pushButton_Modify_clicked()
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
        return;
    }
    if(ui->pushButton_Modify->text() == "恢复")
    {
        EnabledControls(false);
        m_TimerID = startTimer(500);
        return;
    }
}

void ZEROSTDDcuGUIDlg::on_pushButton_Set_clicked()
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
            //
            byteArray.resize(sizeof(m_ComPara));
            memcpy(byteArray.data(), &m_ComPara, sizeof(m_ComPara));
            m_pHAManager->SetParameter(m_ParaComSynID, byteArray);
            SendMessage((char*)&m_ComPara, sizeof(m_ComPara), m_usC, m_usTID);
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

void ZEROSTDDcuGUIDlg::closeEvent(QCloseEvent *event)
{
    event->accept();
}

QSize ZEROSTDDcuGUIDlg::sizeHint() const
{
    return QSize(this->width(), this->height());
}

void ZEROSTDDcuGUIDlg::SendMessage(char* pData, unsigned short  usLen, unsigned short usC, unsigned short usTID)
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

bool ZEROSTDDcuGUIDlg::initCmdInfo(QString strModuleName)
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

void ZEROSTDDcuGUIDlg::setOfflineStatusValue()
{
    QString strEmpty("--");
    //在线标记
    ui->comboBox_OnlineMark->setCurrentText(strEmpty);
    //输入射频频率
    ui->lineEdit_InputFreq->setText(strEmpty);
    //输出射频频率
    ui->lineEdit_OutputFreq->setText(strEmpty);
    //信标输出频率：
    ui->lineEdit_BeaconFreq->setText(strEmpty);
    //数控衰减器1
    ui->lineEdit_NCAtten1->setText(strEmpty);
    //数控衰减器2
    ui->lineEdit_NCAtten2->setText(strEmpty);
    //参考源选择
    ui->comboBox_ReferentSel->setCurrentText(strEmpty);
}

DWORDLONG ZEROSTDDcuGUIDlg::StrToSynCode(const char * pStr)
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
