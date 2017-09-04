
#include "headers.h"
#include "RecvDataThread_ttcSrvr.h"
#include <time.h>
#include <sys/time.h>
#include "Global.h"
#include <QtEndian>
#include "../../Common/Log.h"
#include "DataStructTTCServerInterface.h"
#include "TTCServerInterface_global.h"
#include "util.h"
namespace TTCSrvr {


RecvDataThread::RecvDataThread(QObject *parent)
    : QThread(parent)
{
    m_pSocket = NULL;
    m_pDataBuffer = new UCHAR[MAX_RECV_BUFFER];
    m_bStop = false;
}

RecvDataThread::~RecvDataThread()
{
    m_bStop = true;

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

void RecvDataThread::run()
{
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
            processDatagram(datagram, chlId, m_ComChannelVec[i]);
        }
        usleep(10);
    }
}


void RecvDataThread::setStop()
{
    m_bStop = false;
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


void RecvDataThread::processDatagram(QByteArray& datagram, QString chlID, ICommunicationChannel* pChl)
{

    if(chlID == "server")
    {
        if(datagram.size() == 1 && datagram[0] == 0)
        {
            QByteArray tmpBuff;
            getDevs(tmpBuff);
            pChl->SendData((uchar*)tmpBuff.data(), tmpBuff.size());
        }
        else
        {
            struct {TLxTsspMessageHeader header;TLxTsspSubMsgHeader subHeader;}protocol;
            memcpy(&protocol, datagram.data()+4, sizeof(protocol));
            protocol.header.S = m_pPlatformRes->m_pPublicInfo->GetLocalDeviceID();
            protocol.header.D = m_pPlatformRes->m_pPublicInfo->GetLocalDeviceID();
            protocol.subHeader.SID = m_pPlatformRes->m_pPublicInfo->GetLocalDeviceID();
            memcpy(datagram.data()+4,&protocol, sizeof(protocol));
            TLxTsspMessage msg;
            memcpy(&msg.iDataLen, datagram.data(), 4);
            msg.pData = datagram.data()+4;
            m_pPlatformRes->m_pMsgMgr->FireMessage(msg);
        }
    }
    //    else if (chlID == "DOWNNetSwitch")
    //    {
    //        m_netSwitchDown.processNetSwitch(datagram);
    //    }
    //    else if(chlID == "UPNetSwitch")
    //    {
    //        //m_netSwitchUp.processNetSwitch(datagram);
    //    }
    //    else if(chlID == "SMCU" || chlID == "USBTTC" || chlID == "SS1BBE")
    //    {
    //        m_bbe1.processBaseBand(datagram);
    //    }
    //    else if(chlID == "SMCU1")
    //    {
    //        //m_bbe2.processBaseBand(datagram);
    //    }
    //    else if(chlID == "DownAlter1")
    //    {
    //        //m_alterDown1.process(datagram);

    //    }
    //    else if(chlID == "DownAlter2")
    //    {
    //        //m_alterDown1.process(datagram);

    //    }
    //    else if(chlID == "UpAlter1")
    //    {
    //        //m_alterUp1.process(datagram);

    //    }
    //    else if(chlID == "UpAlter2")
    //    {
    //        //m_alterUp1.process(datagram);
    //    }
    //    else if (chlID == "UpAlter") {
    //        m_adjustAlter.process(datagram);
    //    }
    //    else
    //    {
    //        //m_devs.proceProcotol(datagram);
    //    }
}


bool RecvDataThread::init(PlatformResource& res)
{
    m_pPlatformRes = &res;
    if(false == m_bbe1.init((USHORT)0x400, m_pPlatformRes))
    {
        CLog::addLog("设备bbe1初始化失败。", 1);
        return false;
    }
    //    if(false == m_bbe2.init((USHORT)0x401, m_pPlatformRes))
    //    {
    //        CLog::addLog("设备bbe2初始化失败。", 1);
    //        return false;
    //    }
    //    if(false == m_bbe3.init((USHORT)0x402, m_pPlatformRes))
    //    {
    //        CLog::addLog("设备bbe3初始化失败。", 1);
    //        return false;
    //    }
    //    if(false == m_netSwitchUp.init((USHORT)0x401, m_pPlatformRes))
    //    {
    //        CLog::addLog("上行开关矩阵初始化失败。", 1);
    //        return false;
    //    }
    //    if(false == m_netSwitchDown.init((USHORT)0x1700, m_pPlatformRes))
    //    {
    //        CLog::addLog("下行开关矩阵初始化失败。", 1);
    //        return false;
    //    }
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
    //    if(false == m_devs.init(m_pPlatformRes))
    //    {
    //        CLog::addLog("设备初始化失败。", 1);
    //        return false;
    //    }

    //读取所有配置信息生存通道内容
    QList<ICommunicationChannel*> ChannelList;
    m_pPlatformRes->m_pChlMgr->GetAllChannel(ChannelList);
    for(int i = 0; i < ChannelList.size(); ++i)
    {
        ICommunicationChannel* pChannal = ChannelList.at(i);
        if(pChannal == NULL)
        {
            QString strChlID;
            pChannal->GetChannelID(strChlID);
            QString strInfo = QString("%1 通道句柄为空，创建接收失败!").arg(strChlID);
            qDebug()<<strInfo;
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

}
