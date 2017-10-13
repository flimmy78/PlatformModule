#include "RecvDataThread_tmremote.h"
#include <time.h>
#include <sys/time.h>
#include "Global.h"
#include <QtEndian>
#include "../../Common/Log.h"
#include "DataStructTTCRemoteInterface.h"
#include "TMRemoteInterface_global.h"
#include "util.h"
#include "../../Common/const.h"
#include <QDataStream>
#pragma pack(1)

RecvDataThread::RecvDataThread(QObject *parent)
    : QThread(parent)
{
    m_pSocket = NULL;
    m_pDataBuffer = new UCHAR[MAX_RECV_BUFFER];
    m_pTC = NULL;
    m_bStop = false;
    m_devConnectID = 0;
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
            processDatagram(datagram, chlId);
        }
        usleep(100);
    }
}

void RecvDataThread::setStop()
{
    m_bStop = false;
}

void RecvDataThread::processDatagram(QByteArray& datagram, QString chlID)
{

    if(chlID == "TMRemote")
    {
        char tmp = datagram[datagram.size() - 1];
        datagram.remove(datagram.size() - 1, 1);
        struct {TLxTsspMessageHeader header;TLxTsspSubMsgHeader subHeader;}protocol;
        memcpy(&protocol, datagram.data(), sizeof(protocol));
        if (sizeof(SAllDataSource) == datagram.size())
            setDevs(datagram,datagram[datagram.size() - 1]);
        else if (tmp == STATUS)
        {
            DATASOURCE_PRO ds;
            memset(&ds, 0, sizeof(ds));
            int sz = datagram.size() > sizeof(ds) ? sizeof(ds) : datagram.size();
            memcpy(&ds, datagram.data(), sz);
            if (m_DSMap.find(ds.id) == m_DSMap.end())
                m_DSMap[ds.id] = m_pPlatformRes->m_pStatMgr->FindOneItem(ds.id, ds.len);
            if (m_DSMap[ds.id] == NULL)
            {
                sz = ds.len;
                m_DSMap[ds.id] = m_pPlatformRes->m_pStatMgr->ApplyOneItem(ds.id, sz);
                if(m_DSMap[ds.id] && sz == ds.len)
                    memcpy(m_DSMap[ds.id], ds.data, ds.len);
                    //m_pPlatformRes->m_pStatMgr->UpdateOneItem(ds.id, );
            } else {
                memcpy(m_DSMap[ds.id], ds.data, ds.len);
                updataTimeTag(ds);
            }
            m_pPlatformRes->m_pStatMgr->IncreaseTimeTag(m_devConnectID);
            if (g_showDatagram.m_id == ds.id){
                g_showDatagram.m_datagramStatuRev.clear();
                g_showDatagram.m_datagramStatuRev.resize(ds.len);
                memcpy(g_showDatagram.m_datagramStatuRev.data(), ds.data, ds.len);
            }
        }
        else if (tmp == MACRO)//1宏
        {
            setDatagram(datagram);
            if (macroTransferFinish())
            {
                updateMacroList();
            }
        }
    }
}

bool RecvDataThread::macroTransferFinish()
{
    //ILxTsspParamMacroManager* pMacroMgr = m_pPlatformRes->m_pMacroMgr;
    QString filePath = QDir::currentPath() + "/TM/System/MacroManager/";
    QFile file(filePath + "MacroList.mac");
    if (!file.exists())return false;
    if(!file.open(QIODevice::ReadOnly))return false;
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_7);
    in >> m_macroList;
    qDebug() << m_macroList.size();
    //QList fileList;
    foreach (QByteArray v, m_macroList.values())
    {
        TLxTsspParamMacroInformation* pMacroInfo = (TLxTsspParamMacroInformation*)v.data();
        //getFileList(fileList, filePath);
        QFile datFile(getDATFullPath(pMacroInfo));
        if(!datFile.exists())return false;
    }
    return true;
}
int RecvDataThread::getSN(int devID)
{
    if(0x400 == devID)
    {
        return (int)m_bbe1.m_pBbeAbstract->dwSystemWorkMode;
    }
    else if(0x401 == devID)
    {
        return (int)m_bbe2.m_pBbeAbstract->dwSystemWorkMode;
    }
    else if(0x700 == devID)
    {
        return (int)m_safeCtrl1.m_pMCSSysTarget4Status1->MCSSysStatu.ucSystemMode;
    }
    else
    {
        return (int)m_safeCtrl2.m_pMCSSysTarget4Status1->MCSSysStatu.ucSystemMode;
    }
}

bool RecvDataThread::setDatagram(QByteArray datagram)
{
    TMTCMACRO data;
    memcpy(&data, datagram.data(), qMin(int(sizeof(data)), datagram.size()));
    QDir dir;
    QString strPath = dir.currentPath();
    QString filePath;
    qDebug() << QString(data.fileName);
    if (QString(data.fileName) == "IFMCfgMacro.dat")
        filePath = strPath + "/System/LxTsspConfig/";
    else
        filePath = strPath + "/TM/System/MacroManager/";
    QByteArray byteArry;
    byteArry.resize(data.fileNameLen);
    memcpy(byteArry.data(),data.fileName,data.fileNameLen);
    QFile file(filePath + byteArry.data());
    if (!file.open(QIODevice::ReadWrite))
    {
        CLog::addLog(QString(QString(byteArry.data()) + ":文件写入失败!"), 1);
        return false;
    }
    file.write(data.fileData, data.fileDataLen);
    file.close();
    m_searchMarc->setStop();
}

QString RecvDataThread::getDATFullPath(TLxTsspParamMacroInformation* pMacroInfo)
{
    QString filePath = QDir::currentPath() + "/TM/System/MacroManager/";
    return filePath + QString::number(pMacroInfo->uiID, 16) + ".dat";
}

void RecvDataThread::updataTimeTag(DATASOURCE_PRO &ds)
{
    QMap<quint64,uint>::iterator iter = m_tags.find(ds.id);
    if(iter != m_tags.end())
    {
        if(m_tags[ds.id] == ds.timeTag)return;
        m_tags[ds.id] = ds.timeTag;
        m_pPlatformRes->m_pStatMgr->IncreaseTimeTag(ds.id);
    }
    else
        m_tags[ds.id] = ds.timeTag;
}

bool RecvDataThread::updateMacroList()
{
    ILxTsspParamMacroManager* pMacroMgr = m_pPlatformRes->m_pMacroMgr;
    QString filePath = QDir::currentPath() + "/TM/System/MacroManager/MacroList.mac";
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))return false;
    QDataStream in(&file);
    in >> m_macroList;
    foreach (QByteArray v, m_macroList.values()) {
        TLxTsspParamMacroInformation* pMacroInfo = (TLxTsspParamMacroInformation*)v.data();
        QFile datFile(getDATFullPath(pMacroInfo));
        if(!datFile.open(QIODevice::ReadOnly))continue;
        QDataStream datIn(&datFile);
        TLxTsspParamMacroInformation tmp;
        QByteArray buff;
        buff.resize(sizeof(tmp));
        datIn >> buff >> m_paramList;
        memcpy(&tmp, buff.data(), sizeof(tmp));
        ILxTsspParameterMacro* pMacroObj = NULL;
        pMacroMgr->CreateMacro(pMacroObj,tmp);
        QMapIterator<quint64, QByteArray> i(m_paramList);
        while (i.hasNext()) {
            i.next();
            pMacroObj->UpdateParameterBlock(i.key(), i.value());
        }
        pMacroMgr->AddMacro(pMacroObj);
        delete pMacroObj;
    }
    return false;
}

bool RecvDataThread::init(PlatformResource& res)
{
    m_pPlatformRes = &res;
    DATA_SOURCE_ALLOC(SMCU2::MCSSSTCStatus4, m_pTC, "单遥控发令状态", 0x100, 0);
    DATA_SOURCE_ALLOC(DevicesConnectStatus, m_pConnectSt, "设备联接状态", 0x100, 0);
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
bool RecvDataThread::init(PlatformResource& res,SearchMarc *searchMarc)
{
    m_searchMarc=searchMarc;
    m_pPlatformRes = &res;
    DATA_SOURCE_ALLOC(SMCU2::MCSSSTCStatus4, m_pTC, "单遥控发令状态", 0x100, 0);
    DATA_SOURCE_ALLOC(DevicesConnectStatus, m_pConnectSt, "设备联接状态", 0x100, 0);
    m_devConnectID = m_pPlatformRes->m_nIDCurrent;
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
#pragma pack()

