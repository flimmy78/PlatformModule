#include "SLDCDcuGUIDlg.h"
#include "ui_SLDCDcuGUIDlg.h"
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

SLDCDcuGUIDlg::SLDCDcuGUIDlg(QWidget *parent) : QDialog(parent),
    ui(new Ui::SLDCDcuGUIDlg)
{
    ui->setupUi(this);
    m_pStateMgr = NULL;
    m_pILxTsspMessageManager = NULL;
    m_pILxTsspLogManager = NULL;
    m_pPublicInfo = NULL;
    m_pHAManager = NULL;

    m_pSLDCDcuStatus = NULL;

    m_bOnLine = false;

    setAttribute(Qt::WA_DeleteOnClose);
    //setMinimumSize(800, 550);
    init();

    EnabledControls(false);
}

SLDCDcuGUIDlg::~SLDCDcuGUIDlg()
{
    killTimer(m_TimerID);
    delete ui;
}

void SLDCDcuGUIDlg::setText(const QString& strTitle,ParaStruct xmlsetText[3][60],QString MapMean[50][30],int MapNum[50],uchar Type,ILxTsspParameterMacro* pMacroObj)
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

    for(int i=0;i<49;i++)
    {
        DlgMapLineNum[i] = MapNum[i];
    }
    //初始化界面
    InitDLG(Type,pMacroObj);
}

//初始化对话框
void SLDCDcuGUIDlg::InitDLG(int Type, ILxTsspParameterMacro* pMacroObj)
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
            DWORD dwLen = sizeof(SLDCDCUStatu);
            m_pSLDCDcuStatus  = (SLDCDCUStatu*)m_pStateMgr->FindOneItem(m_StatusCodeSynID, dwLen);
            if(m_pSLDCDcuStatus == NULL || dwLen != sizeof(SLDCDCUStatu))
                return;
        }
        //启动定时器 用于显示二维矩阵中的灯光使用
        m_TimerID = startTimer(1000);
        //
        EnabledControls(false);
    }
    else if(Type == 1)//取宏里参数
    {
        if(pMacroObj != NULL)
        {
            QByteArray byteArray;
            pMacroObj->GetParameterBlock(m_ParaCodeSynID, byteArray);
            memcpy(&m_SLDCDcuPara, byteArray.data(), sizeof(SLDCDCUPara));
        }
        ui->pushButton_Modify->setVisible(false);
        ui->pushButton_Set->setVisible(false);

        //显示参数
        show();
    }
}

void SLDCDcuGUIDlg::SetObjInfo(TLxTsspObjectInfo ObjectInfo)
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
        m_ParaCodeSynID = GetID(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID,
                                                               sItem.usTID, sItem.ucSN);
        /*************************状态信息设置的ID*************************/

        /**************************状态信息设置的ID************************/
        m_StatusCodeSynID = GetID(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID,
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
int SLDCDcuGUIDlg::SaveToMacro(ILxTsspParameterMacro* pMacroObj)
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
    pMacroObj->UpdateParameterBlock(m_ParaCodeSynID, ba);

    //日志信息
    QString strLog;
    strLog += QString("%1: 保存宏成功！").arg(MODULENAME);
    CLog::addLog(strLog);
}

//定时器
void SLDCDcuGUIDlg::timerEvent(QTimerEvent *event)
{
    if (m_TimerID == event->timerId())
        show();
}

bool SLDCDcuGUIDlg::initCmdInfo(QString strModuleName)
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
    if (item != NULL && item->vSubItem.size() > 0)
    {
        m_usC = item->vSubItem[0].usCMD;
        m_usTID = item->vSubItem[0].usTID;
    }
    return true;
}

void SLDCDcuGUIDlg::show()
{
    if(m_pSLDCDcuStatus != NULL)
    {
        m_SLDCDcuPara = m_pSLDCDcuStatus->tParam;
    }
    else
    {
        DWORD stLen = sizeof(SLDCDCUStatu);
        if(m_pStateMgr != NULL)
            m_pSLDCDcuStatus  = (SLDCDCUStatu*)m_pStateMgr->FindOneItem(m_StatusCodeSynID, stLen);
    }

    if (m_pSLDCDcuStatus == NULL)
        return;

    /********************设备是否在线************************/
    m_bOnLine = CCommFunc::getInstance()->getSLDivcomStatus();
    if (!m_bOnLine)
    {
        setOfflineStatusValue();
        return;
    }
    /********************设备是否在线************************/

    //显示主控还是分控
    bool bCtrlMode = m_pSLDCDcuStatus->ucConMode == 0x00 ? true : false;
    bool bNormal = m_pSLDCDcuStatus->ucSelfCheckStatu == 0x00 ? true : false;
    setType(bCtrlMode, bNormal);
    UCHAR * pChar = (UCHAR*)&m_pSLDCDcuStatus->tParam;
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

void SLDCDcuGUIDlg::EnabledControls(bool bFlag)
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

void SLDCDcuGUIDlg::setType(bool bMain, bool bNormal)
{
    QString strType = bMain ? QString(tr("主控")) : QString(tr("分控"));
    QString strStatus = bNormal ? QString(tr("正常")) : QString(tr("故障"));

    ui->lineEdit_CrrlMode->setText(strType);
    ui->lineEdit_SelCheckStatus->setText(strStatus);
}

void SLDCDcuGUIDlg::setOfflineStatusValue()
{
    QString strEmpty("--");
    ui->lineEdit_CrrlMode->setText(strEmpty);
    ui->lineEdit_SelCheckStatus->setText(strEmpty);
}

void SLDCDcuGUIDlg::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void SLDCDcuGUIDlg::init()
{
    QDir dir;
    QString strPath = dir.currentPath();
    QString strConfigPath = strPath + "/TM/System/ModuleConfig/SLDCDcuConfig.xml";
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

void SLDCDcuGUIDlg::initCtrl()
{
    QVBoxLayout *hLayout = new QVBoxLayout;
    int nGroupCount = m_reader.m_GroupList.size();
    for (int i = 0; i < nGroupCount;  i++)
    {
        int nParamSize = m_reader.m_GroupList.at(i).paramNameList.size();
        QRectF rect(20, 10, 800, nParamSize * 50);
        QString strOut = m_reader.m_GroupList.at(i).strName;
        GroupSwitch * pSwitch = new GroupSwitch(rect, m_reader.m_GroupList.at(i).paramNameList, strOut, ui->widget);
        pSwitch->setFont(11);
        pSwitch->setLineDistance(20);
        m_GroupSwitchList.push_back(pSwitch);
        hLayout->addWidget(pSwitch);
    }
    //hLayout->addStretch();
    hLayout->addStretch();
    //this->setLayout(hLayout);
    ui->widget->setLayout(hLayout);
}

QSize SLDCDcuGUIDlg::sizeHint() const
{
    return QSize(this->width(), this->height());
}

void SLDCDcuGUIDlg::SendMessage(char* pData, unsigned short  usLen, unsigned short usC, unsigned short usTID)
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

void SLDCDcuGUIDlg::on_pushButton_Modify_clicked()
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

void SLDCDcuGUIDlg::on_pushButton_Set_clicked()
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
        byteArray.resize(sizeof(SLDCDCUPara));
        memcpy(byteArray.data(), ba, sizeof(SLDCDCUPara));
        m_pHAManager->SetParameter(m_ParaCodeSynID, byteArray);
        SendMessage(ba.data(), ba.length(), m_usC, m_usTID);
    }
    //日志信息
    QString strLog;
    strLog = QString("%1: 设置参数成功！").arg(MODULENAME);
    CLog::addLog(strLog);

    on_pushButton_Modify_clicked();
}
