#include "RecvDataThread_tmSrvr.h"
#include <time.h>
#include <sys/time.h>
#include "Global.h"
#include <QtEndian>
#include <QString>
#include <QList>
#include <QCoreApplication>
#include "../../Common/Log.h"
#include "DataStructTMServerInterface.h"
#include "TMServerInterface_global.h"
#include "util.h"
#include "../../Common/CommonFunction.h"
#include "../../Common/const.h"
#include <string>
#include <QRegularExpression>
using namespace std;
#pragma pack(1)
RecvDataThread::RecvDataThread(QObject *parent)
    : QThread(parent)
{
    m_pSocket = NULL;
    m_pDataBuffer = new UCHAR[MAX_RECV_BUFFER];
    m_pConnectSt = NULL;
    m_pTC = NULL;
    m_bStop = false;
    m_pPlatformRes = NULL;
    m_listDsex.clear();
}

RecvDataThread::~RecvDataThread()
{
    m_bStop = true;
    this->wait();
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

void RecvDataThread::catchDs()
{
    m_listDsex.clear();
    DS_EX dsex;
    memset(&dsex, 0, sizeof(dsex));
    DATASOURCE_PRO& ds = dsex.ds;
    ILxTsspStateManager* pStatMgr = m_pPlatformRes->m_pStatMgr;
    const int itemCounts = pStatMgr->GetItemCounts();
    for(int i = 0; i < itemCounts; ++i)
    {
        if (pStatMgr->GetOneItemByIndex(i, ds.id, dsex.pContent, ds.len, ds.timeTag))
        {
            dsex.pContent = pStatMgr->FindOneItem(ds.id, ds.len);
            ds.data[ds.len] = STATUS;
            m_listDsex.append(dsex);
        }
    }
}

void RecvDataThread::processDatagram(QByteArray& datagram, QString chlID, ICommunicationChannel* /*pChl*/)
{
    if(chlID != "TMServer")return;
    char tmp = datagram[datagram.size() - 1];
    datagram.remove(datagram.size() - 1, 1);
    struct {TLxTsspMessageHeader header;TLxTsspSubMsgHeader subHeader;}protocol;
    memcpy(&protocol, datagram.data(), sizeof(protocol));
    if(tmp == PARAM)
    {
        protocol.header.S = m_pPlatformRes->m_pPublicInfo->GetLocalDeviceID();
        protocol.header.D = m_pPlatformRes->m_pPublicInfo->GetLocalDeviceID();
        protocol.subHeader.SID = m_pPlatformRes->m_pPublicInfo->GetLocalDeviceID();
        memcpy(datagram.data(), &protocol, sizeof(protocol));
        TLxTsspMessage msg;
        msg.iDataLen = datagram.size();
        msg.pData = datagram.data();
        m_pPlatformRes->m_pMsgMgr->FireMessage(msg);
    }
    else if (tmp == STATUS)
    {
        if(m_listDsex.size() != m_pPlatformRes->m_pStatMgr->GetItemCounts())catchDs();
        QList<ICommunicationChannel*> chls;
        bool ok = m_pPlatformRes->getChlsUsedByDevCtl(chls);
        if(!ok || !chls.size())
        {
            CLog::addLog("状态上报时，通道获取失败。");
            return;
        }
        QList<ICommunicationChannel*>::iterator iter = chls.begin(), iterEnd = chls.end();
        for (; iter != iterEnd; ++iter)
        {
            for (int i = 0; i < m_listDsex.size(); ++i)
            {
                DATASOURCE_PRO& ds = m_listDsex[i].ds;
                if(ds.len > c_buff_size)
                {
                    continue;
                }
                memcpy(ds.data, m_listDsex[i].pContent, ds.len);
                int ret1_1 = (*iter)->SendData((uchar*)&ds, ds.len + sizeof(ds) - c_buff_size + 1);
                if (-1 == ret1_1)
                {
                    CLog::addLog("状态上报消息发送失败.\n");
                    continue;
                }
            }
        }
    }
    else if (tmp == MACRO)
    {
        QDir dir;
        QString strPath = dir.currentPath();
        QByteArray macro;
        QList<QFileInfo> fileList;
        getFileList(fileList, strPath);
        QList<QFileInfo>::iterator iter = fileList.begin(), end = fileList.end();
        for (; iter != end; ++iter)
        {
            if (iter->suffix() == "dat" || iter->suffix() == "mac")
            {
                if(getDatagram(*iter, macro, strPath))
                    sendDatagram(macro);
            }
        }
    }

}

bool RecvDataThread::init(PlatformResource& res)
{
    m_pPlatformRes = &res;
    DATA_SOURCE_ALLOC(SMCU2::MCSSSTCStatus4, m_pTC, "单遥控发令状态", 0x100, 0);
    DATA_SOURCE_ALLOC(DevicesConnectStatus, m_pConnectSt, "设备联接状态", 0x100, 0);
    if(false == m_bbe1.init((USHORT)0x400, m_pPlatformRes))
    {
        CLog::addLog("设备bbe1初始化失败。", 1);
        return false;
    }
    if(false == m_bbe2.init((USHORT)0x401, m_pPlatformRes))
    {
        CLog::addLog("设备bbe2初始化失败。", 1);
        return false;
    }
    if(false == m_safeCtrl1.init((USHORT)0x700, m_pPlatformRes))
    {
        CLog::addLog("设备安控1初始化失败。", 1);
        return false;
    }
    if(false == m_safeCtrl2.init((USHORT)0x701, m_pPlatformRes))
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
    if(false == m_alterUp2.init("上变频器", 0xEA05, (USHORT)0x401, m_pPlatformRes))
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
    if(false == m_rm1.init(0x1200,m_pPlatformRes))
    {
        CLog::addLog("检前1初始化失败。", 1);
        return false;
    }
    if(false == m_rm2.init(0x1201,m_pPlatformRes))
    {
        CLog::addLog("检前2初始化失败。", 1);
        return false;
    }
    //读取所有配置信息生存通道内容
    QList<ICommunicationChannel*> ChannelList;
    m_pPlatformRes->getChlsUsedByDevCtl(ChannelList);
    for(int i = 0; i < ChannelList.size(); ++i)
    {
        ICommunicationChannel* pChannal = ChannelList.at(i);
        QString strChlID;
        pChannal->GetChannelID(strChlID);
        if(strChlID != "TMServer")continue;
        if(pChannal == NULL)
        {

            QString strInfo = QString("%1 通道句柄为空，创建接收失败!").arg(strChlID);
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

bool RecvDataThread::getFileList(QFileInfoList &fileList, QString strPath)
{
    QString filePath = strPath + "/TM/System/MacroManager/";
    fileList = QDir(filePath).entryInfoList();
    if (fileList.isEmpty())
    {
        return false;
    }
    return true;
}

bool RecvDataThread::getDatagram(QFileInfo fileInfo, QByteArray& macro, QString strPath)
{
    TMTCMACRO data;
    QString filePath = strPath + "/TM/System/MacroManager/";
    QString fileName = fileInfo.fileName();
    QFile file(filePath + fileName);
    if(!file.open(QFile::ReadOnly))
    {
        CLog::addLog(fileName + ":打开失败!", 1);
        return false;
    }
    QByteArray byteArr;
    byteArr = fileName.toLatin1();
    memcpy(data.fileName, byteArr.data(), byteArr.size());
    data.fileNameLen = byteArr.size();
    byteArr.clear();
    byteArr = file.readAll();
    if(byteArr.isNull())return false;
    if(byteArr.size() > c_buff_size_100)
    {
        CLog::addLog(fileName + ":文件超过100K，无法处理!", 1);
        return false;
    }
    memcpy(data.fileData, byteArr.data(), byteArr.size());
    const int size = sizeof(data) - c_buff_size_100 + byteArr.size();
    data.fileDataLen = byteArr.size();
    macro.resize(size);
    memcpy(macro.data(), &data, size);
    file.close();
    return true;
}

bool RecvDataThread::sendDatagram(QByteArray& macro)
{
    QList<ICommunicationChannel*> chls;
    bool ok = m_pPlatformRes->getChlsUsedByDevCtl(chls);
    if (!ok || !chls.size())
    {
        CLog::addLog("宏同步，通道获取失败！");
        return false;
    }
    QList<ICommunicationChannel*>::iterator iter = chls.begin(), end = chls.end();
    for (; iter != end; ++iter)
    {
        macro.append(1);
        int ret = (*iter)->SendData((uchar*)macro.data(), macro.size());
        if (-1 == ret)
        {
            CLog::addLog("宏同步命令消息发送失败.\n");
            continue;
        }
    }
}
bool RecvDataThread::getDevs(QByteArray &datagram)
{

    m_bbe1.get(m_allDS.bbe1);
    m_bbe2.get(m_allDS.bbe2);
    m_netSwitchUp.get(m_allDS.nsUp);
    m_netSwitchDown.get(m_allDS.nsDown);
    m_alterDown1.get(m_allDS.alterDown1);
    m_alterDown2.get(m_allDS.alterDown2);
    m_alterUp1.get(m_allDS.alterUp1);
    m_alterUp2.get(m_allDS.alterUp2);
    m_adjustAlter.get(m_allDS.adjstAlter);
    m_devs.get(m_allDS.devs);
    m_safeCtrl1.get(m_allDS.safeTC1);
    m_safeCtrl2.get(m_allDS.safeTC2);
    m_rm1.get(m_allDS.rm1);
    m_rm2.get(m_allDS.rm2);
    if(!m_pConnectSt || !m_pTC)return false;
    memcpy(&m_allDS.m_connectSt, m_pConnectSt, sizeof(m_allDS.m_connectSt));
    memcpy(&m_allDS.m_tc, m_pTC, sizeof(m_allDS.m_tc));
    datagram.clear();
    datagram.resize(sizeof(m_allDS));
    memcpy(datagram.data(), &m_allDS, sizeof(m_allDS));
    return true;
}

bool RecvDataThread::getDevs(QByteArray &datagram, int nIndex)
{
    switch(nIndex)
    {
    case 0:
        m_bbe1.get(m_allDS.bbe1);
        datagram.clear();
        datagram.append((const char*)&m_allDS.bbe1,sizeof(m_allDS.bbe1));
        break;
    case 1:
        m_bbe2.get(m_allDS.bbe2);
        datagram.clear();
        datagram.append((const char*)&m_allDS.bbe2,sizeof(m_allDS.bbe2));
        break;
    case 2:
        m_netSwitchUp.get(m_allDS.nsUp);
        datagram.clear();
        datagram.append((const char*)&m_allDS.nsUp,sizeof(m_allDS.nsUp));
        break;
    case 3:
        m_netSwitchDown.get(m_allDS.nsDown);
        datagram.clear();
        datagram.append((const char*)&m_allDS.nsDown,sizeof(m_allDS.nsDown));
        break;
    case 4:
        m_alterDown1.get(m_allDS.alterDown1);
        datagram.clear();
        datagram.append((const char*)&m_allDS.alterDown1,sizeof(m_allDS.alterDown1));
        break;
    case 5:
        m_alterDown2.get(m_allDS.alterDown2);
        datagram.clear();
        datagram.append((const char*)&m_allDS.alterDown2,sizeof(m_allDS.alterDown2));
        break;
    case 6:
        m_alterUp1.get(m_allDS.alterUp1);
        datagram.clear();
        datagram.append((const char*)&m_allDS.alterUp1,sizeof(m_allDS.alterUp1));
        break;
    case 7:
        m_alterUp2.get(m_allDS.alterUp2);
        datagram.clear();
        datagram.append((const char*)&m_allDS.alterUp2,sizeof(m_allDS.alterUp2));
        break;
    case 8:
        m_adjustAlter.get(m_allDS.adjstAlter);
        datagram.clear();
        datagram.append((const char*)&m_allDS.adjstAlter,sizeof(m_allDS.adjstAlter));
        break;
    case 9:
        m_devs.get(m_allDS.devs);
        datagram.clear();
        datagram.append((const char*)&m_allDS.devs,sizeof(m_allDS.devs));
        break;
    case 10:
        if(!m_pConnectSt)return false;
        memcpy(&m_allDS.m_connectSt, m_pConnectSt, sizeof(m_allDS.m_connectSt));
        datagram.clear();
        datagram.append((const char*)&m_allDS.m_connectSt,sizeof(m_allDS.m_connectSt));
        break;
    case 11:
        if(!m_pTC)return false;
        memcpy(&m_allDS.m_tc, m_pTC, sizeof(m_allDS.m_tc));
        datagram.clear();
        datagram.append((const char*)&m_allDS.m_tc,sizeof(m_allDS.m_tc));
        break;
    case 12:
        m_safeCtrl1.get(m_allDS.safeTC1);
        datagram.clear();
        datagram.append((const char*)&m_allDS.safeTC1,sizeof(m_allDS.safeTC1));
        break;
    case 13:
        m_safeCtrl2.get(m_allDS.safeTC2);
        datagram.clear();
        datagram.append((const char*)&m_allDS.safeTC2,sizeof(m_allDS.safeTC2));
        break;
    case 14:
        m_rm1.get(m_allDS.rm1);
        datagram.clear();
        datagram.append((const char*)&m_allDS.rm1,sizeof(m_allDS.rm1));
        break;
    case 15:
        m_rm2.get(m_allDS.rm2);
        datagram.clear();
        datagram.append((const char*)&m_allDS.rm2,sizeof(m_allDS.rm2));
        break;
    default:
        return false;
    }
    return true;
}
#pragma pack()
