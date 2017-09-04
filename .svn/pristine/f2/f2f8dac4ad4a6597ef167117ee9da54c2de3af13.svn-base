#include "RecvDataThread.h"
#include <time.h>
#include <sys/time.h>
#include "Global.h"
#include <QtEndian>
#include "../../Common/Log.h"
#include "DataStructTTCDevCtlInterface.h"
#include "DevCtlInterface_global.h"
#include "util.h"
#include "../../Common/CommonFunction.h"
#include "../../Common/const.h"
RecvDataThread::RecvDataThread(QObject *parent)
    : QThread(parent)
{
    m_pSocket = NULL;
    m_pDataBuffer = new UCHAR[MAX_RECV_BUFFER];
    m_pTC = NULL;
    m_bStop = false;
}

RecvDataThread::~RecvDataThread()
{
    if (m_pDataBuffer != NULL)
    {
        delete[] m_pDataBuffer;
        m_pDataBuffer = NULL;
    }
    if (m_pSocket != NULL)
    {
        m_pSocket->close();
        delete m_pSocket;
        m_pSocket = NULL;
    }
}

#define USE_CHANNEL
void RecvDataThread::run()
{
#ifdef USE_CHANNEL
    int iDataLen = MAX_RECV_BUFFER;
    while(1)
    {
        if(m_bStop)
            break;
        for (int i = 0; i < m_ComChannelVec.size(); i++)
        {
            QByteArray datagram;
            uint uiPackageCount = 0;
            uchar* aIP = NULL;
            int iPort = 0;
            int iDataHeader = 0;
            int nRet = m_ComChannelVec.at(i)->ReadReceivedData(m_pDataBuffer, iDataLen, uiPackageCount, aIP, iPort, iDataHeader);
            if (nRet == -1)
            {
                continue;
            }
            QString chlId;
            m_ComChannelVec[i]->GetChannelID(chlId);
            datagram.clear();
            datagram.resize(iDataLen);
            memcpy(datagram.data(), m_pDataBuffer, datagram.size());
            qDebug() << "Recv len: " << datagram.size();
            processDatagram(datagram, chlId);
        }
        usleep(10);
    }
#else
       m_pSocket = new QUdpSocket(this);
#if 0
    //m_pSocket->bind(QHostAddress::AnyIPv4, 5062, QAbstractSocket::ShareAddress);
    m_pSocket->bind(QHostAddress::AnyIPv4, 1177, QAbstractSocket::ShareAddress);
    QHostAddress mcast_addr("225.0.2.51");
    m_pSocket->setSocketOption(QAbstractSocket::MulticastLoopbackOption, 0);
    m_pSocket->joinMulticastGroup(mcast_addr);
    connect(m_pSocket, SIGNAL(readyRead()), this, SLOT(ReadData()), Qt::DirectConnection);
#else
    m_pSocket->bind(QHostAddress::AnyIPv4, 5050, QAbstractSocket::ShareAddress);
    //m_pSocket->bind(QHostAddress::AnyIPv4, 5062, QAbstractSocket::ShareAddress);
    //m_pSocket->bind(QHostAddress::AnyIPv4, 5062);
    /*QHostAddress mcast_addr("225.0.0.2");
    m_pSocket->setSocketOption(QAbstractSocket::MulticastLoopbackOption, 0);
    m_pSocket->joinMulticastGroup(mcast_addr);*/
    connect(m_pSocket, SIGNAL(readyRead()), this, SLOT(ReadData()));
#endif
    exec();
#endif // USE_CHANNEL
}


void RecvDataThread::setStop()
{
    m_bStop = true;
    wait();
}

void RecvDataThread::ReadData()
{
    if (m_pSocket == NULL)
        return;

    while(m_pSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.clear();
        datagram.resize(m_pSocket->pendingDatagramSize());
        m_pSocket->readDatagram(datagram.data(), datagram.size());
        //processDatagram(datagram);
    }
}

void RecvDataThread::processDatagram(QByteArray& datagram, QString chlID)
{
    DEV chl = DEV_UNKOW;
    if (chlID == "DOWNNetSwitch")
    {
        chl = DOWNNetSwitch;
        m_netSwitchDown.processNetSwitch(datagram, "下行开关矩阵");
    }
    else if(chlID == "UPNetSwitch")
    {
        chl = UPNetSwitch;
        m_netSwitchUp.processNetSwitch(datagram, "上行开关矩阵");
    }
    else if(chlID == "DDTSMCU1" || chlID == "TCSMCU1" || chlID == "SS1SMCU1" || chlID == "SS2SMCU1" || chlID == "USBSMCU1")
    {
        if(chlID == "DDTSMCU1")chl = DDTSMCU1;
        else if(chlID == "TCSMCU1")chl = TCSMCU1;
        else if(chlID == "SS1SMCU1")chl = SS1SMCU1;
        else if(chlID == "SS2SMCU1")chl = SS2SMCU1;
        else if(chlID == "USBSMCU1")chl = USBSMCU1;
        m_bbe1.processBaseBand(datagram, chlID);
    }
    else if(chlID == "DDTSMCU2" || chlID == "TCSMCU2" || chlID == "SS1SMCU2" || chlID == "SS2SMCU2" || chlID == "USBSMCU2")
    {
        if(chlID == "DDTSMCU2")chl = DDTSMCU2;
        else if(chlID == "TCSMCU2")chl = TCSMCU2;
        else if(chlID == "SS1SMCU2")chl = SS1SMCU2;
        else if(chlID == "SS2SMCU2")chl = SS2SMCU2;
        else if(chlID == "USBSMCU2")chl = USBSMCU2;
        m_bbe2.processBaseBand(datagram, chlID);
    }
    else if(chlID == "DownAlter1")
    {
        chl = DownAlter1;
        //m_alterDown1.process(datagram);
    }
    else if(chlID == "DownAlter2")
    {
        chl = DownAlter2;
        //m_alterDown1.process(datagram);
    }
    else if(chlID == "UpAlter1")
    {
        chl = UpAlter1;
        //m_alterUp1.process(datagram);
    }
    else if(chlID == "UpAlter2")
    {
        chl = UpAlter2;
        //m_alterUp1.process(datagram);
    }
    else if (chlID == "UpAlter")
    {
        chl = UpAlter;
        m_adjustAlter.process(datagram);
    }
    else if(chlID == "LTCSMCU1")
    {
        chl = LTCSMCU1;
        m_safeCtrl1.processBaseBand(datagram);
        m_safeCtrl1.updateAbstractAK();
    }
    else if(chlID == "LTCSMCU2")
    {
        chl = LTCSMCU2;
        m_safeCtrl2.processBaseBand(datagram);
        m_safeCtrl2.updateAbstractAK();
    }
    else if(chlID == "Record0")
    {
        m_crm1.processRecordMonitor(datagram);
       // m_crm.updateAbstractAK();
    }
    else if(chlID == "Record1")
    {
        m_crm2.processRecordMonitor(datagram);
    }
    else
    {
        chl = DEV_SMCU;
        m_devs.proceProcotol(datagram);
    }
}

bool RecvDataThread::init(PlatformResource& res)
{    
    m_pPlatformRes = &res;
    DATA_SOURCE_ALLOC(SMCU2::MCSSSTCStatus4, m_pTC, "单遥控发令状态", 0x100, 0);
    if(false == m_bbe1.init((USHORT)0x400, m_pPlatformRes, m_pTC))
    {
        CLog::addLog("设备bbe1初始化失败。", 1);
        return false;
    }
    if(false == m_bbe2.init((USHORT)0x401, m_pPlatformRes, m_pTC))
    {
        CLog::addLog("设备bbe2初始化失败。", 1);
        return false;
    }
    if(false == m_safeCtrl1.init((USHORT)0x700, m_pPlatformRes, m_pTC))
    {
        CLog::addLog("设备安控1初始化失败。", 1);
        return false;
    }
    if(false == m_safeCtrl2.init((USHORT)0x701, m_pPlatformRes, m_pTC))
    {
        CLog::addLog("设备安控2初始化失败。", 1);
        return false;
    }
    //上行中频开关矩阵
    if(false == m_netSwitchUp.init((USHORT)0x3B00, m_pPlatformRes))
    {
        CLog::addLog("上行开关矩阵初始化失败。", 1);
        return false;
    }
    //下行中频开关矩阵
    if(false == m_netSwitchDown.init((USHORT)0x3C00, m_pPlatformRes, false))
    {
        CLog::addLog("下行开关矩阵初始化失败。", 1);
        return false;
    }
    if(false == m_alterDown1.init("下变频器", 0xEA07, (USHORT)0x400, m_pPlatformRes))
    {
        CLog::addLog("变频器初始化失败。", 1);
        return false;
    }
    if(false == m_alterDown2.init("下变频器", 0xEA07, (USHORT)0x401, m_pPlatformRes))
    {
        CLog::addLog("变频器初始化失败。", 1);
        return false;
    }
    if(false == m_alterUp1.init("上变频器", 0xEA05, (USHORT)0x400, m_pPlatformRes))
    {
        CLog::addLog("变频器初始化失败。", 1);
        return false;
    }
    if(false == m_alterUp1.init("上变频器", 0xEA05, (USHORT)0x401, m_pPlatformRes))
    {
        CLog::addLog("变频器初始化失败。", 1);
        return false;
    }
    if(false == m_adjustAlter.init((USHORT)0x400, m_pPlatformRes))
    {
        CLog::addLog("变频器初始化失败。", 1);
        return false;
    }
    if(false == m_devs.init(m_pPlatformRes))
    {
        CLog::addLog("设备初始化失败。", 1);
        return false;
    }
    if(false == m_crm1.init(0x1200,m_pPlatformRes))
    {
        CLog::addLog("检前1初始化失败。", 1);
        return false;
    }
    if(false == m_crm2.init(0x1201,m_pPlatformRes))
    {
        CLog::addLog("检前2初始化失败。", 1);
        return false;
    }

    //读取所有配置信息生存通道内容
    QList<ICommunicationChannel*> ChannelList;
    if(!m_pPlatformRes->getChlsUsedByDevCtl(ChannelList))
    {
        CLog::addLog("通道获取失败。", 1);
        return false;;
    }
    for(int i = 0; i < ChannelList.size(); ++i)
    {
        ICommunicationChannel* pChannal = ChannelList.at(i);
        if(pChannal == NULL)
        {
            QString strInfo = QString("第%1 通道句柄为空，创建接收失败!").arg(i);
            CLog::addLog(strInfo, 1);
            continue;
        }
        //下面增加接收参数信息 内部实现是为了接收参数数据内容使用
        uchar ucLocalIP[] = "127.0.0.1";
        int iLocalPort = 0;
        pChannal->ReceivedData(ucLocalIP, iLocalPort);
        m_ComChannelVec.push_back(pChannal);
    }
    return true;
}
