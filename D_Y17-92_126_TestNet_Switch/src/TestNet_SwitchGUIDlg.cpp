#include "TestNet_SwitchGUIDlg.h"
#include "ui_TestNet_SwitchGUIDlg.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDateTime>
#include <QDir>
#include <QDebug>
#include <QHBoxLayout>
#include <QPushButton>

#include "../../Common/CommonFunction.h"

extern quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
              uchar ucSn);

TestNet_SwitchGUIDlg::TestNet_SwitchGUIDlg(QWidget *parent) : QDialog(parent),
    ui(new Ui::TestNet_SwitchGUIDlg)
{
    ui->setupUi(this);
    m_pStateMgr = NULL;
    m_pILxTsspMessageManager = NULL;
    m_pILxTsspLogManager = NULL;
    m_pPublicInfo = NULL;
    m_pHAManager = NULL;

    m_pTestSWMatStatus = NULL;

    m_bOnLine = false;

    setAttribute(Qt::WA_DeleteOnClose);
    //setMinimumSize(800, 550);
    init();

    ui->lineEdit_CrrlMode->setReadOnly(true);
    ui->lineEdit_SelCheckStatus->setReadOnly(true);

    EnabledControls(false);
}

TestNet_SwitchGUIDlg::~TestNet_SwitchGUIDlg()
{
    killTimer(m_TimerID);
    delete ui;
}

void TestNet_SwitchGUIDlg::setText(const QString& strTitle,ParaStruct xmlsetText[3][60],QString MapMean[50][30],int MapNum[50],uchar Type,ILxTsspParameterMacro* pMacroObj)
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
    //初始化界面
    InitDLG(Type, pMacroObj);
}

//初始化对话框
void TestNet_SwitchGUIDlg::InitDLG(int Type, ILxTsspParameterMacro* pMacroObj)
{
    if(Type == 0)//取当前参数
    {
        //取当前设置的参数
       /* QByteArray byteArray;
        if(m_pHAManager!=NULL)
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
            DWORD dwLen = sizeof(TEST_SWMATStatu);
            m_pTestSWMatStatus  = (TEST_SWMATStatu*)m_pStateMgr->FindOneItem(m_StatusSwitchCodeSynID, dwLen);
            if(m_pTestSWMatStatus == NULL || dwLen != sizeof(TEST_SWMATStatu))
                return;
        }
        //启动定时器 用于刷新显示使用
        m_TimerID = startTimer(500);

        EnabledControls(false);
    }
    else if(Type == 1)//取宏里参数
    {
        if(pMacroObj != NULL)
        {
            QByteArray byteArray;
            pMacroObj->GetParameterBlock(m_ParaSwitchCodeSynID, byteArray);
            memcpy(&m_TestSWMatPara, byteArray.data(), sizeof(TEST_SWMATPara));
        }
        ui->pushButton_Modify->setVisible(false);
        ui->pushButton_Set->setVisible(false);

        //显示参数
        show();
    }
}

void TestNet_SwitchGUIDlg::SetObjInfo(TLxTsspObjectInfo ObjectInfo)
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
        qDebug() << strInfo;
        CLog::addLog(strInfo);
        return;
    }
    ITEM* pItem = m_DeviceMap.getItem(MODULENAME);
    if (pItem != NULL && pItem->vSubItem.size() > 0)
    {
        SUBITEM sItem = pItem->vSubItem.at(0);
        /**************************状态信息设置的ID************************/
        m_ParaSwitchCodeSynID = GetID(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID,
                                                               sItem.usTID, sItem.ucSN);
        /*************************状态信息设置的ID*************************/

        /**************************状态信息设置的ID************************/
        m_StatusSwitchCodeSynID = GetID(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID,
                                                               sItem.usStateType, sItem.ucSN);
        /*************************状态信息设置的ID*************************/
    }

    //日志
    m_pILxTsspLogManager = pObjectManager->GetLogManager();
    m_pStateMgr = pObjectManager->GetStateManager();
    m_pILxTsspMessageManager = pObjectManager->GetMessageManager();
    m_pPublicInfo = pObjectManager->GetPublicInfoManager();
}

//保存参数宏
int TestNet_SwitchGUIDlg::SaveToMacro(ILxTsspParameterMacro* pMacroObj)
{
    //获取设置的通道
    QByteArray ba;
    ba.resize(sizeof(TEST_SWMATPara));
    int nCount = m_GroupSwitchList.size();
    for (int i = 0; i < nCount; i ++)
    {
        GroupSwitch * pSwitch = m_GroupSwitchList.at(i);
        if (pSwitch != NULL)
        {
            //将数字转化成16进制数字
            int index = pSwitch->getCurrentIndex();
            UCHAR ucValue = 0;
            QMap<int, UCHAR>::const_iterator itr = m_reader.m_GroupList.at(i).paramValueMap.find(index);
            if (itr != m_reader.m_GroupList.at(i).paramValueMap.end())
                ucValue = itr.value();
            ba.append(ucValue);
        }
    }
    pMacroObj->UpdateParameterBlock(m_ParaSwitchCodeSynID, ba);

    //日志信息
    QString strLog;
    strLog += QString("%1: 保存宏成功！").arg(MODULENAME);
    CLog::addLog(strLog);

    EnabledControls(false);
}

//定时器
void TestNet_SwitchGUIDlg::timerEvent(QTimerEvent *event)
{
    if (m_TimerID == event->timerId())
        show();
}

bool TestNet_SwitchGUIDlg::initCmdInfo(QString strModuleName)
{
    bool bRet = m_DeviceMap.loadXML();
    if (!bRet)
    {
        QString strInfo = QString("load device map failed %1").arg(MODULENAME);
        qDebug() << strInfo;
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

void TestNet_SwitchGUIDlg::show()
{
    if(m_pTestSWMatStatus != NULL)
    {
        m_TestSWMatPara = m_pTestSWMatStatus->tParam;
    }
    else
    {
        DWORD stLen = sizeof(TEST_SWMATStatu);
        if(m_pStateMgr != NULL)
            m_pTestSWMatStatus  = (TEST_SWMATStatu*)m_pStateMgr->FindOneItem(m_StatusSwitchCodeSynID, stLen);
    }

    if (m_pTestSWMatStatus == NULL)
        return;

    /********************设备是否在线************************/
    m_bOnLine = CCommFunc::getInstance()->getTestNet_switchStatus();
    if (!m_bOnLine)
    {
        setOfflineStatusValue();
        return;
    }
    /********************设备是否在线************************/

    //显示主控还是分控
    bool bCtrlMode = m_pTestSWMatStatus->ucConMode == 0x00 ? true : false;
    bool bNormal = m_pTestSWMatStatus->ucSelfCheckStatu == 0x00 ? true : false;
    setType(bCtrlMode, bNormal);
    UCHAR *pChar = (UCHAR*)&m_pTestSWMatStatus->tParam;
    int nCount = m_GroupSwitchList.size();
    for (int i = 0; i < nCount; i ++)
    {
        UCHAR ucValue = static_cast<UCHAR>(*(pChar + i));
        GroupSwitch * pSwitch = m_GroupSwitchList.at(i);
        if (pSwitch == NULL)
        {
            continue;
        }
        int nCurIndex = 0;
        int nGroupCount = m_reader.m_GroupList.at(i).paramValueMap.size();
        for (int j = 0; j < nGroupCount; j++)
        {
            UCHAR ucTmp = m_reader.m_GroupList.at(i).paramValueMap.value(j);
            if (ucValue == ucTmp)
            {
                nCurIndex = j;
                break;
            }
        }

        pSwitch->setCurrentIndex(nCurIndex);
    }
}

void TestNet_SwitchGUIDlg::EnabledControls(bool bFlag)
{
    //设置Combox是否可用
    int nCount = m_GroupSwitchList.size();
    for (int i = 0; i < nCount; i ++)
    {
        GroupSwitch * pSwitch = m_GroupSwitchList.at(i);
        if (pSwitch != NULL)
        {
            pSwitch->enabledCtrl(bFlag);
        }
    }
    ui->pushButton_Set->setEnabled(bFlag);
}

void TestNet_SwitchGUIDlg::setType(bool bMain, bool bNormal)
{
    QString strType = bMain ? QString(tr(LOCAL_CTRL)) : QString(tr(REMOTE_CTRL));
    QString strStatus = bNormal ? QString(tr(NORMA)) : QString(tr(FAULT));

    ui->lineEdit_CrrlMode->setText(strType);
    ui->lineEdit_SelCheckStatus->setText(strStatus);
}

void TestNet_SwitchGUIDlg::setOfflineStatusValue()
{
    QString strEmpty("--");
    ui->lineEdit_CrrlMode->setText(strEmpty);
    ui->lineEdit_SelCheckStatus->setText(strEmpty);
}

void TestNet_SwitchGUIDlg::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void TestNet_SwitchGUIDlg::init()
{
    QDir dir;
    QString strPath = dir.currentPath();
    QString strConfigPath = strPath + "/TM/System/ModuleConfig/TestNet_SwitchConfig.xml";
    bool bRet = m_reader.loadXML(strConfigPath);
    if (!bRet)
    {
        QString strInfo = QString("read xml config failed %1").arg(strConfigPath);
        qDebug() << strInfo;
        CLog::addLog(strInfo);
        return;
    }

    //初始化指令信息
    initCmdInfo(MODULENAME);

    //初始化界面上显示的控件
    initCtrl();
}

void TestNet_SwitchGUIDlg::initCtrl()
{
    QVBoxLayout *hLayout = new QVBoxLayout;
    int nGroupCount = m_reader.m_GroupList.size();
    for (int i = 0; i < nGroupCount;  i++)
    {
        int nParamSize = m_reader.m_GroupList.at(i).paramNameList.size();
        QRectF rect(30, 10, 800, nParamSize * 30);
        QString strOut = m_reader.m_GroupList.at(i).strName;
        GroupSwitch * pSwitch = new GroupSwitch(rect, m_reader.m_GroupList.at(i).paramNameList, strOut, ui->widget);
        m_GroupSwitchList.push_back(pSwitch);
        hLayout->addWidget(pSwitch);
    }
    hLayout->addStretch();
    //this->setLayout(hLayout);
    ui->widget->setLayout(hLayout);
}

QSize TestNet_SwitchGUIDlg::sizeHint() const
{
    return QSize(this->width(), this->height());
}

void TestNet_SwitchGUIDlg::SendMessage(char* pData, unsigned short  usLen, unsigned short usC, unsigned short usTID)
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
    memcpy(message.pData + sizeof(TLxTsspMessageHeader), &subHeader, sizeof(TLxTsspSubMsgHeader));
    memcpy(message.pData + sizeof(TLxTsspMessageHeader) + sizeof(TLxTsspSubMsgHeader),
           pData, usLen);
    message.iDataLen = sizeof(TLxTsspMessageHeader) + sizeof(TLxTsspSubMsgHeader) + usLen;

    int iRet = m_pILxTsspMessageManager->FireMessage(message);
    delete[] message.pData;
}

void TestNet_SwitchGUIDlg::on_pushButton_Modify_clicked()
{
    if (!m_bOnLine)
    {
        QString strMsg = QString(tr("%1处于离线状态，无法进行更改设置！")).arg(MODULENAME);
        QMessageBox::information(this, tr("信息提示"), strMsg);
        return;
    }

    QString strText = ui->pushButton_Modify->text();
    if (strText == tr("更改"))
    {
        ui->pushButton_Modify->setText(tr("恢复"));
        EnabledControls(true);
        killTimer(m_TimerID);
    }
    else
    {
        ui->pushButton_Modify->setText(tr("更改"));
        EnabledControls(false);
        m_TimerID = startTimer(500);
    }
}

void TestNet_SwitchGUIDlg::on_pushButton_Set_clicked()
{
    //获取设置的通道
    QByteArray ba;
    int nCount = m_GroupSwitchList.size();
    for (int i = 0; i < nCount; i ++)
    {
        GroupSwitch * pSwitch = m_GroupSwitchList.at(i);
        if (pSwitch != NULL)
        {
            //将数字转化成16进制数字
            int index = pSwitch->getCurrentIndex();
            UCHAR ucValue = 0;
            QMap<int, UCHAR>::const_iterator itr = m_reader.m_GroupList.at(i).paramValueMap.find(index);
            if (itr != m_reader.m_GroupList.at(i).paramValueMap.end())
                ucValue = itr.value();
            ba.append(ucValue);
        }
    }

    //取当前设置的参数
    if(m_pHAManager != NULL)
    {
        QByteArray byteArray;
        byteArray.resize(sizeof(TEST_SWMATPara));
        memcpy(byteArray.data(), &m_TestSWMatPara, sizeof(TEST_SWMATPara));
        m_pHAManager->SetParameter(m_ParaSwitchCodeSynID, byteArray);
        SendMessage(ba.data(), ba.length(), m_usC, m_usTID);
    }
    //日志信息
    QString strLog;
    strLog = QString("%1: 设置参数成功！").arg(MODULENAME);
    CLog::addLog(strLog);
    m_TimerID = startTimer(500);

    on_pushButton_Modify_clicked();
}
