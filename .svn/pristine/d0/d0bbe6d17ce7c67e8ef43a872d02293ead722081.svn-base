#include "RecvDataThread.h"
#include <time.h>
#include <sys/time.h>
#include "Global.h"
#include <QtEndian>
#include "../Common/Log.h"
#include "DataStructTTCDevCtlInterface.h"
#include "DevCtlInterface_global.h"
#include "util.h"

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

void RecvDataThread::updateDev(ICommunicationChannel* pChl)
{
    if(NULL == g_context.m_pDeviceLinkStatus)
        return;
    QString chlId;
    pChl->GetChannelID(chlId);
    if(chlId == "SMCU")
        g_context.m_deviceIndex = 0;
    else if(chlId == "SMCU1")
        g_context.m_deviceIndex = 1;
    else
        g_context.m_deviceIndex = g_context.NODEV;
}
#define USE_CHANNEL
void RecvDataThread::run()
{
#ifdef USE_CHANNEL
    bool isOK = initChlProcessor();
    if(!isOK)
    {
        QString strInfo = QString("初始化通道处理失败，线程退出!");
        CLog::addLog(strInfo, 1);
        return;
    }
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
            updateDev(m_ComChannelVec[i]);
            datagram.clear();
            datagram.resize(iDataLen);
            memcpy(datagram.data(), m_pDataBuffer, datagram.size());
            qDebug() << "Recv len: " << datagram.size();
            processDatagram(datagram);
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

int RecvDataThread::checkProcotol(QByteArray &datagram)
{
    //中频开关矩阵
    PROCOTOL_PACK fwMatPro;
    memcpy(&fwMatPro, datagram.data(), sizeof(fwMatPro));
    return  (0 == fwMatPro.H || 0x1 == fwMatPro.H || 0x2 == fwMatPro.H || 0x4 == fwMatPro.H);
}

void RecvDataThread::processDatagram(QByteArray& datagram)
{
    if (checkProcotol(datagram))
    {
        proceProcotol(datagram);
        g_showDatagram.m_datagramCommRev = datagram;
    }
    else
    {
        processBaseBound(datagram);
    }
}

void RecvDataThread::proceProcotol(QByteArray &datagram)
{
    PROCOTOL_PACK proPack;
    //int nSize = sizeof(PROCOTOL_PACK);
    memcpy(&proPack, datagram.data(), datagram.size());

    //如果数据源为标准TTC联试应答机监控
    if (0x1400 == proPack.S)
    {
        processCombTrans(proPack);
    }
    //非相干扩频联试应答机监控
    else if (0x1401 == proPack.S)
    {
        processCombTrans(proPack, false);
    }
    //校零信表一体机
    else if (0xEA14 == proPack.S)
    {
        processZEROSTDDcu(proPack);
    }
    //射频分路组合监控
    else if (0x2C00 == proPack.S)
    {
        processSLDDcu(proPack);
    }
    //射频切换开关监控
    else if (0x2C01 == proPack.S)
    {
        processSswitch(proPack);
    }
    //测试开关矩阵
    else if (0x2C07 == proPack.S)
    {
        processTestSwitch(proPack);
    }
    //频率基准源监控
    else if (0x2C08 == proPack.S)
    {
        processFreqBDcu(proPack);
    }
    //铷钟机箱监控
    else if (0x2C20 == proPack.S)
    {
        processRCLOCKDcu(proPack);
    }
    //中频开关矩阵
    else if (0x1700 == proPack.S)
    {
        processNetSwitch(proPack);
    }
}

bool RecvDataThread::initChlProcessor()
{
    m_pObjectManager = LxTsspObjectManager::Instance();
    if (m_pObjectManager == NULL)
    {
        QString strInfo = QString("获取通道管理者实例化失败!");
        CLog::addLog(strInfo, 1);
        return false;
    }
    m_pCommunicationMgr = (ILxTsspCommunicationManager *)m_pObjectManager->GetCommunicationManager();
    if (m_pCommunicationMgr == NULL)
    {
        QString strInfo = QString("通道管理者句柄为空!");
        CLog::addLog(strInfo, 1);
        return false;
    }

    //读取所有配置信息生存通道内容
    QList<ICommunicationChannel*> ChannelList;
    m_pCommunicationMgr->GetAllChannel(ChannelList);
    for(int i = 0; i < ChannelList.size(); ++i)
    {
        ICommunicationChannel* pChannal = ChannelList.at(i);
        if(pChannal == NULL)
        {
            QString strChlID;
            pChannal->GetChannelID(strChlID);
            QString strInfo;
            strInfo = QString("%1 通道句柄为空，创建接收失败!").arg(strChlID);
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

//bool RecvDataThread::isSwMat(QByteArray& datagram)
//{
//    //中频开关矩阵
//    PROCOTOL_PACK fwMatPro;
//    memcpy(&fwMatPro, datagram.data(), sizeof(fwMatPro));
//    return  (0 == fwMatPro.H || 0x1 == fwMatPro.H || 0x2 == fwMatPro.H || 0x4 == fwMatPro.H);
//}

void RecvDataThread::processNetSwitch(QByteArray& datagram)
{
    //1、相关具体变量使用可配置的？
    //2、解析中使用的T不是开始的T 应该为数据段中的T1来判断？
    PROCOTOL_PACK fwMatPro;
    memcpy(&fwMatPro, datagram.data(), sizeof(fwMatPro));
    if(0x1700 == fwMatPro.S)//中频开关矩阵
    {
        if(0x02 == fwMatPro.T  && 0x200 == fwMatPro.C)
        {
            QString ans = "中频开关矩阵应答: 0x";
            ans += QString::number(fwMatPro.Data.Dat1.answer, 16);
            ans += ".\n";
            CLog::addLog(ans);
        }
        else if(0x04 == fwMatPro.T && 0x701 == fwMatPro.C)
            memcpy(g_pIFSWMatStatuResponse, &fwMatPro.Data.Dat1.ifSwMatParaStatus, fwMatPro.Data.L1);
        else
            CLog::addLog("接收到的中频开关矩阵协议报文有错.");
    }
    else if(0x1400 == fwMatPro.S)//sl频段
    {
        if(0x02 == fwMatPro.T && 0x046E == fwMatPro.C)
        {
            QString ans = "sl频段应答: 0x";
            ans += QString::number(fwMatPro.Data.Dat1.answer, 16);
            ans += ".\n";
            CLog::addLog(ans);
        }
        //处理标准TTC应答工作上报状态信息
        else if(0x1200 == fwMatPro.C && 0x18A3 == fwMatPro.Data.T1)
            //memcpy(g_pCombTransParaFXG, &fwMatPro.Data.Dat1.svSysStatu, fwMatPro.Data.L1);
            ;
        //处理标准TTC应答参数宏信息
        else if (0x04 == fwMatPro.T && 0x701 == fwMatPro.C)
            //memcpy(g_pCombTransParaFXG, &fwMatPro.Data.Dat1.svSysPara, fwMatPro.Data.L1);
            ;
        else
            CLog::addLog("sl频段协议报文有错.");
    }
    else if(0x3000 == fwMatPro.S)//射频分路组合监控 //fix me
    {
        if(0x02 == fwMatPro.T && 0x200 == fwMatPro.C)
        {
            QString ans = "射频分路组合监控应答: 0x";
            ans += QString::number(fwMatPro.Data.Dat1.answer, 16);
            ans += ".\n";
            CLog::addLog(ans);
        }
        else if(0x04 == fwMatPro.T && 0x701 == fwMatPro.C)
            memcpy(g_pSLDDcuParaStatu, &fwMatPro.Data.Dat1.sldDcuParaStatu, fwMatPro.Data.L1);
        else
            CLog::addLog("射频分路组合监控协议报文有错.");
    }
    else if(0x4000 == fwMatPro.S)//频段切换开关.//fix me
    {
        if (0x02 == fwMatPro.T && 0x200 == fwMatPro.C)
        {
            QString ans = "频段切换开关应答: 0x";
            ans += QString::number(fwMatPro.Data.Dat1.answer, 16);
            ans += ".\n";
            CLog::addLog(ans);
        }
        else if (0x04 == fwMatPro.T && 0x701 == fwMatPro.C)
            memcpy(g_pSswitchDcuStatu, &fwMatPro.Data.Dat1.ssDcuStatu, fwMatPro.Data.L1);
        else
            CLog::addLog("频段切换开关协议报文有错.");
    }
    else if(0x5000 == fwMatPro.S)//测试开关矩阵//fix me
    {
        if(0x02 == fwMatPro.T && 0x200 == fwMatPro.C)
        {
            QString ans = "测试开关矩阵应答: 0x";
            ans += QString::number(fwMatPro.Data.Dat1.answer, 16);
            ans += ".\n";
            CLog::addLog(ans);
        }
        else if(0x04 == fwMatPro.T && 0x701 == fwMatPro.C)
            memcpy(g_pTstSwMatStatu, &fwMatPro.Data.Dat1.tstSwMatStatu, fwMatPro.Data.L1);
        else
            CLog::addLog("测试开关矩阵协议报文有错.");
    }
    else if(0x6000 == fwMatPro.S)//频率基准源
    {
        if(0x02 == fwMatPro.T && 0x200 == fwMatPro.C)
        {
            QString ans = "频率基准源应答: 0x";
            ans += QString::number(fwMatPro.Data.Dat1.answer, 16);
            ans += ".\n";
            CLog::addLog(ans);
        }
        else if(0x04 == fwMatPro.T && 0x701 == fwMatPro.C)
            memcpy(g_pFreDcuStatu, &fwMatPro.Data.Dat1.freDcuStatu, fwMatPro.Data.L1);
        else
            CLog::addLog("频率基准源协议报文有错.");
    }
    else if(0x7000 == fwMatPro.S)//一体机
    {
        if(0x02 == fwMatPro.T && 0x200 == fwMatPro.C)
        {
            QString ans = "一体机应答: 0x";
            ans += QString::number(fwMatPro.Data.Dat1.answer, 16);
            ans += ".\n";
            CLog::addLog(ans);
        }
        else if(0x04 == fwMatPro.T && 0x701 == fwMatPro.C)
            memcpy(g_pZCBeaconStatu, &fwMatPro.Data.Dat1.zcBeaconStatu, fwMatPro.Data.L1);
        else
            CLog::addLog("一体机协议报文有错.");
    }
}

bool RecvDataThread::processFuncion(const NETSDFRM& netSDFrm)
{
    QMap<int, QPair<QString, QString> >& sidSys = g_context.m_devMap.m_mpSysSid;
    QMap<int, QPair<QString, QString> >::iterator iter = sidSys.find(netSDFrm.SID);
    if(iter == sidSys.end())
        return false;
    if(iter.value().first == "DDT")
        m_pfProcessBaseboundDTSS = processBaseBoundDataTrans;
    else if(iter.value().first == "USB")
        m_pfProcessBaseboundDTSS = processBaseBoundTTC;
    else if(iter.value().first == "SS1")
        m_pfProcessBaseboundDTSS = processBaseBoundSMCU1;
    else if(iter.value().first == "SS2")
        m_pfProcessBaseboundDTSS = processBaseBoundSMCU2;
    else
        return false;
    return true;
}
void RecvDataThread::processBaseBound(QByteArray& datagram)
{
    // 基带
    NETSDFRM netSDFrm;
    int nNetSDFRMLen = sizeof(NETSDFRM);
    if (datagram.size() <= nNetSDFRMLen)
    {
        CLog::addLog("接收到的基带协议报文有错.");
        return;
    }
    memcpy(&netSDFrm, datagram.data(), nNetSDFRMLen);
//    if(netSDFrm.DataLen != datagram.size() - nNetSDFRMLen)
//    {
//        CLog::addLog("接收到的基带协议报文有错.");
//        return;
//    }状态查询命令报文长度不对, 所以无法用上述方法检测
    if(!processFuncion(netSDFrm))
    {
        CLog::addLog("状态上报报文有误.");
        return;
    }
    switch(netSDFrm.BID)
    {
    case 0xF100:
        //控制命令响应
        processBaseBound_0xF100(datagram.data() + nNetSDFRMLen);
        g_showDatagram.m_datagramCommRev =datagram;
        break;
    case 0xF104:
        //分机状态信息上报
        processBaseBound_0xF104(datagram.data()  + nNetSDFRMLen, netSDFrm.DataLen);
        g_showDatagram.m_datagramStatuRev =datagram;
        break;
    case 0xF101:
        //控制结果上报
        processBaseBound_0xF101(datagram.data()  + nNetSDFRMLen);
        g_showDatagram.m_datagramCommRev =datagram;
        break;
    default:
        break;
    }
}

void RecvDataThread::processBaseBound_0xF100(char *pData)
{
    CCRESPONSE ccResponse;
    memcpy(&ccResponse, pData, sizeof(CCRESPONSE));

    if(0xF002 == ccResponse.TYPE)
    {//单元参数设置命令
        QString log = "单元参数设置命令,单元0x";
        log = log + QString::number(ccResponse.ID, 16);
        log = log + "的序号: 0x" + QString::number(ccResponse.SEQ, 16)+ ". ";
        log = log + "控制结果: " + QString::number(ccResponse.CRESULT) + ".\n";
        CLog::addLog(log);
    }  
    else if(0xF003 == ccResponse.TYPE)
    {//组参数设置命令
        QString log = "组参数设置命令";
        log = log + "的序号: 0x" + QString::number(ccResponse.SEQ, 16)+ ". ";
        log = log + "控制结果: " + QString::number(ccResponse.CRESULT) + ".\n";
        CLog::addLog(log);
    }
    else if(0xF001 == ccResponse.TYPE)
    {//过程控制命令
        QString log = "过程控制命令0x";
        log = log + QString::number(ccResponse.ID, 16);
        log = log + "的序号: 0x" + QString::number(ccResponse.SEQ, 16)+ ". ";
        log = log + "控制结果: " + QString::number(ccResponse.CRESULT) + ".\n";
        CLog::addLog(log);
    }
}

void RecvDataThread::processBaseBoundDataTrans(BYTE *pBuff, const BYTE &unitNum)
{
    switch(unitNum)
    {
    case 1:     //DDT_BBE系统参数设置
        MCSSysDataStatusStruct tmp;
        memcpy(&tmp, pBuff, sizeof(MCSSysDataStatusStruct));
        tmp.ucSystemMode = 4;
        memcpy(g_pMCSSysStatus, &tmp, sizeof(MCSSysDataStatusStruct));
        if(g_context.NODEV != g_context.m_deviceIndex)
        {
            g_context.m_pBBBGlobalStatus[g_context.m_deviceIndex].DestMCSSysStatus.ucMonitorMode =
                    g_pMCSSysStatus->ucMonitorMode;
            g_context.m_pBBBGlobalStatus[g_context.m_deviceIndex].DestMCSSysStatus.dwSystemMode =
                    g_pMCSSysStatus->ucSystemMode;
            g_pILxTsspStateManager->IncreaseTimeTag(g_context.m_ddtId[g_context.m_deviceIndex]);
        }
        break;
    case 8:     //数传检测输出参数设置
        memcpy(g_pSCBBETestOutStatu, pBuff, sizeof(SCBBETestOutStatusStruct));
        break;
    case 15:    //数传接收机单元数据元素
        memcpy(g_pMCSDDTRevStatus, pBuff, sizeof(MCSDDTRevStatusStruct));
        break;
    case 16:    //数传解调器单元(I路)数据元素
        memcpy(g_pMCSDDTTMQStatusI, pBuff, sizeof(MCSDDTTMQStatusStruct));
        break;
    case 17:    //数传解调器单元(Q路)数据元素
        memcpy(g_pMCSDDTTMQStatusQ, pBuff, sizeof(MCSDDTTMQStatusStruct));
        break;
    case 18:    //数传模拟源单元数据元素
        memcpy(g_pMCSDDTTMSimulStatus, pBuff, sizeof(MCSDDTTMSimulStatusStruct));
        break;
    case 19:    //数传存储转发单元数据元素
        //memcpy(g_pMCSDDTReplayStatus, pBuff, sizeof(MCSDDTReplayStatusStruct));
        break;
    case 20:    //分集合成
        memcpy(g_pMCSDivComStatus, pBuff, sizeof(MCSDivComStatusStruct));
        break;
    }
}

void RecvDataThread::processBaseBoundTTC(BYTE * pBuff, const BYTE& unitNum)
{
    switch(unitNum)
    {
    case 1://USBBBE系统参数设置
        MCSSysDataStatusStruct tmp;
        memcpy(&tmp, pBuff, sizeof(MCSSysDataStatusStruct));
        tmp.ucSystemMode = 6;
        memcpy(g_pMCSSysDataStatusTTC1, &tmp, sizeof(MCSSysDataStatusStruct));
        if(g_context.NODEV != g_context.m_deviceIndex)
        {
            g_context.m_pBBBGlobalStatus[g_context.m_deviceIndex].DestMCSSysStatus.ucMonitorMode =
                    g_pMCSSysDataStatusTTC1->ucMonitorMode;
            g_context.m_pBBBGlobalStatus[g_context.m_deviceIndex].DestMCSSysStatus.dwSystemMode =
                    g_pMCSSysDataStatusTTC1->ucSystemMode;
            g_pILxTsspStateManager->IncreaseTimeTag(g_context.m_ddtId[g_context.m_deviceIndex]);
        }
        break;
    case 2://USBBBE中频接收机参数设置
        memcpy(g_pMCSUSBRevStatu2, pBuff, sizeof(MCSUSBRevStatusStruct));
        break;
    case 3://USBBBE中频接收机参数设置
        memcpy(g_pMCSUSBRevStatu3, pBuff, sizeof(MCSUSBRevStatusStruct));
        break;
    case 4://USBBBE调制参数设置
        memcpy(g_pMCSUSBUpModulStatus4, pBuff, sizeof(MCSUSBUpModulStatusStruct));
        break;
    case 5://USBBBE测距参数设置
        memcpy(g_pMCSUSBCjStatus5, pBuff, sizeof(MCSUSBCjStatusStruct));
        break;
    case 6://USBBBE测距参数设置
        memcpy(g_pMCSUSBCjStatus6, pBuff, sizeof(MCSUSBCjStatusStruct));
        break;
    case 7://USBBBE遥控参数设置
        memcpy(g_pMCSUSBTCStatus7, pBuff, sizeof(MCSUSBTCStatusStruct));//MCSUSBTCStatusStruct
        break;
    case 8://USBBBE遥测参数设置
        memcpy(g_pMCSUSBTMStatus8, pBuff, sizeof(MCSUSBTMStatusStruct));
        break;      
    case 12://USBBBE模拟源参数设置
        memcpy(g_pMCSUSBTMSimulStatus12, pBuff, sizeof(MCSUSBTMSimulStatusStruct));
        break;
    default:
        break;
    }
}

void RecvDataThread::processBaseBoundSMCU1(BYTE *pBuff, const BYTE &unitNum)
{
    switch(unitNum)
    {
    case 1:
        SMCU::MCSSysTarget4StatusStruct tmp;
        memcpy(&tmp, pBuff, sizeof(SMCU::MCSSysTarget4StatusStruct));
        tmp.sysStatu.ucSystemMode = 7;
        memcpy(g_pMCSSysTarget4Status1, &tmp, sizeof(SMCU::MCSSysTarget4StatusStruct));
        if(g_context.NODEV != g_context.m_deviceIndex)
        {
            g_context.m_pBBBGlobalStatus[g_context.m_deviceIndex].DestMCSSysStatus.ucMonitorMode =
                    g_pMCSSysTarget4Status1->sysStatu.ucMonitorMode;
            g_context.m_pBBBGlobalStatus[g_context.m_deviceIndex].DestMCSSysStatus.dwSystemMode =
                    g_pMCSSysTarget4Status1->sysStatu.ucSystemMode;
            g_pILxTsspStateManager->IncreaseTimeTag(g_context.m_ddtId[g_context.m_deviceIndex]);
        }
        break;
    case 2://模式1接收参数设置
        memcpy(g_pMCSSSRev4Statu4_2, pBuff, sizeof(SMCU::MCSSSRevStatusStruct4));
        break;
    case 3://模式1调制参数设置
        memcpy(g_pMCSSSUTargetTCSSUpModulStatu3, pBuff, sizeof(SMCU::MCSSSUTargetTCSSUpModulStatusStruct));
        break;
    case 5://扩频遥控参数设置
        memcpy(g_pMCSSSTCStatusStruct4_5, pBuff, sizeof(SMCU::MCSSSTCStatusStruct4));
        break;
    case 4://模式1测距测速参数设置
        memcpy(g_pMCSSSCjStatu4_4, pBuff, sizeof(SMCU::MCSSSCjStatusStruct4));
        break;
    case 6://扩频遥测解调参数设置
        memcpy(g_pMCSSSTMStatusStruct4_6, pBuff, sizeof(SMCU::MCSSSTMStatusStruct4));
        break;
    case 7://
        //memcp);
        break;
    case 8://扩频遥测模拟源参数设置
        memcpy(g_pMCSSSTMSimulStatu4_8, pBuff, sizeof(SMCU::MCSSSTMSimulStatusStruct4));
        break;
    case 9:
        memcpy(g_pMCSSSPNCODEStatu4_9, pBuff, sizeof(SMCU::MCSSSPNCODEStatusStruct4));
        break;
    default:
        break;
    }
}

void RecvDataThread::processBaseBoundSMCU2(BYTE *pBuff, const BYTE &unitNum)
{
    switch(unitNum)
    {
    case 1:
//        SMCU::MCSSysTarget4StatusStruct tmp;
//        memcpy(&tmp, pBuff, sizeof(SMCU::MCSSysTarget4StatusStruct));
//        tmp.sysStatu.ucSystemMode = 8;
//        memcpy(g_p2MCSSysTarget4Status1, &tmp, sizeof(SMCU::MCSSysTarget4StatusStruct));
//        if(g_context.NODEV != g_context.m_deviceIndex)
//        {
//            g_context.m_pBBBGlobalStatus[g_context.m_deviceIndex].DestMCSSysStatus.ucMonitorMode =
//                    g_pMCSSysTarget4Status1->sysStatu.ucMonitorMode;
//            g_context.m_pBBBGlobalStatus[g_context.m_deviceIndex].DestMCSSysStatus.ucSystemMode =
//                    g_pMCSSysTarget4Status1->sysStatu.ucSystemMode;
//            g_pILxTsspStateManager->IncreaseTimeTag(g_context.m_ddtId[g_context.m_deviceIndex]);
//        }
        break;
    case 2://模式2接收参数设置
        //memcpy(g_p2MCSSSRev4Statu4_2, pBuff, sizeof(SMCU::MCSSSRev4StatusStruct));
        break;
    case 3://模式2调制参数设置
        memcpy(g_p2MCSSSUTargetTCSSUpModulStatu3, pBuff, sizeof(SMCU::MCSSSUTargetTCSSUpModulStatusStruct));
        break;
    case 4://模式2测距测速参数设置
        //memcpy(g_p2MCSSSCjStatu4_4, pBuff, sizeof(SMCU::MCSSSCjStatusStruct4));
        break;
    case 5://扩频遥控参数设置
        //memcpy(g_pMCSSSTCStatusStruct4_5, pBuff, sizeof(SMCU::MCSSSTCStatusStruct4));
        break;
    case 6://扩频遥测解调参数设置
        //memcpy(g_p2MCSSSTMStatusStruct4_6, pBuff, sizeof(MCSSSTMStatusStruct4));
        break;
    case 7:
        //memcpy();
        break;
    case 8://扩频遥测模拟源参数设置
        //memcpy(g_pMCSSSTMSimulStatu4_8, pBuff, sizeof(MCSSSTMSimulStatusStruct4));
        break;
    case 9://模式2扩频码设置
        memcpy(g_p2MCSSSPNCODEStatu4_9, pBuff, sizeof(SMCU2::MCSSSB2PNCODEStatusStruct4));
        break;
    default:
        break;
    }
}
void RecvDataThread::processBaseBound_0xF104(char *pData, const int dataLen)
{
    BYTE* pDataField = new BYTE[dataLen];
    memcpy(pDataField, pData, dataLen);
    QMap<int, int> mapFB_FE;
    for(int i = 0; i < dataLen; ++i)
    {
        int tmp = pDataField[i];
        QPair<int, int> pairFB_FE;
        if(0xFB != tmp)continue;
        pairFB_FE.first = i;
        for(int j = i; j < dataLen; ++j)
        {
            tmp = pDataField[j];
            if(0xFE != tmp)continue;
            i = pairFB_FE.second = j;
            break;
        }
        mapFB_FE[pairFB_FE.first] = pairFB_FE.second;
    }
    QMap<int, int>::iterator mpIter = mapFB_FE.begin();
    QMap<int, int>::const_iterator mpIterEnd = mapFB_FE.end();
    for(; mpIter != mpIterEnd; ++mpIter)
    {
        BYTE unitNum = pDataField[mpIter.key() + 1];
        BYTE* pBuff = pDataField + mpIter.key() + 2;
        m_pfProcessBaseboundDTSS(pBuff, unitNum);
    }
    delete pDataField;
}

void RecvDataThread::processBaseBound_0xF101(char * pData)
{
    if(!g_pPCReport)
    {
        CLog::addLog(PC_INFO_SOURCE_ERR);
        return;
    }
    //过程命令标识，在上报时，对低字节进行取反，高字节不变。
    //逆操作上述过程得到原来的过程命令标识.
    USHORT id = 0;
    memcpy(&id, pData, sizeof(USHORT));
    UCHAR ucCode = id;
    ucCode = 0xFF-ucCode;
    pData += 4;//过程命令标识与序列号共4个字节,各两个字节.
    switch(ucCode)
    {
    case 1://任务代号下发
        memcpy(&g_pPCReport->tcIssue1, pData, sizeof(TCISSUE));
        break;
    case 2://跟踪通道选择
        g_pPCReport->tcSel2.sel = (UCHAR)pData[0];
        break;
    case 3://应答机零值装订
        memcpy(&g_pPCReport->tzBound3, pData, sizeof(TZBOUND));
        break;
    case 37://数传频率动态范围测试
        memcpy(&g_pPCReport->dfdrTst37, pData, sizeof(DFDRTST));
        break;
    case 38://实时误码帧统计
        memcpy(&g_pPCReport->rtefStat38, pData, sizeof(RTEFSTAT));
        break;
    case 39://误码率测试
        memcpy(&g_pPCReport->berTst39, pData, sizeof(BERTST));
        break;
    case 40://误帧率测试
        memcpy(&g_pPCReport->ferTst40, pData, sizeof(FERTST));
        break;
    case 41://遥控小环误指令测试
        memcpy(&g_pPCReport->rcsreiTst41, pData, sizeof(RCSREITST));
        break;
    default:
        CLog::addLog(tr("单元序号错误.\n"));
    }
}

void RecvDataThread::processCombTrans(PROCOTOL_PACK &proPack, bool bTTC)
{
    QString strDeviceName = bTTC ? "标准TTC联试应答机监控" : "非相干扩频联试应答机监控";
    if(0x046E == proPack.C) //命令应答帧
    {
        //模式类型
        QString strType("未知命令");
        if (0x046D == proPack.Data.T1)
            strType = QString("模式切换命令");
        else if (0x046E == proPack.Data.T1)
            strType = QString("仿真控制命令");
        else if (0x046F == proPack.Data.T1)
            strType = QString("存盘控制命令");
        else if (0x0470 == proPack.Data.T1)
            strType = QString("切换控制命令");

        //响应的内容
        QString strAnswer = getAnswerFlag(proPack.Data.Dat1.answer);
        QString strInfo = QString("%1 %2: %3!").arg(strDeviceName, strType, strAnswer);
        CLog::addLog(strInfo);
    }
    //处理联试应答机监控工作上报状态信息
    else if(0x1200 == proPack.C && 0x18A3 == proPack.Data.T1)
    {
        if (bTTC)
            memcpy(g_pCombTransStatuTTC, &proPack.Data.Dat1.svSysStatu, proPack.Data.L1);
        else
            memcpy(g_pCombTransStatuFXG, &proPack.Data.Dat1.svSysStatu, proPack.Data.L1);
    }
    //处理联试应答机监控参数宏查询信息
    else if (0x200 == proPack.C && 0x0106 == proPack.Data.T1)
    {
        //对宏参数应答数据进行处理
        int nMarcoLen = sizeof(MARCOSEARCHPara);
        int nMarcoCount = proPack.Data.L1 / nMarcoLen;
        int i = 0;
        //将数据值进行解析
        if (bTTC)
        {
            g_pCombTransParaTTC->nParaCount = nMarcoCount;
            for (; i < nMarcoCount; i++)
            {
                memcpy(&g_pCombTransParaTTC->paras[i], (void *)(proPack.Data.Dat1.ucTmp + i * nMarcoLen), nMarcoLen);
            }
            //拷贝标志到响应中 从缓存中取数据
            memcpy(&g_pCombTransParaTTC->ucAnswerFlag, (void *)(proPack.Data.Dat1.ucTmp + i * nMarcoLen), 1);
        }
        else
        {
            g_pCombTransParaFXG->nParaCount = nMarcoCount;
            for (; i < nMarcoCount; i++)
            {
                memcpy(&g_pCombTransParaFXG->paras[i], (void *)(proPack.Data.Dat1.ucTmp + i * nMarcoLen), nMarcoLen);
            }
            //拷贝标志到响应中 从缓存中取数据
            memcpy(&g_pCombTransParaFXG->ucAnswerFlag, (void *)(proPack.Data.Dat1.ucTmp + i * nMarcoLen), 1);
        }
    }
    //处理联试应答机监控参数宏调用信息
    else if (0x200 == proPack.C && 0x0206 == proPack.Data.T1)
    {
        QString strType("宏参数调用返回结果");
        int nMarcoLen = sizeof(MARCOSEARCHPara);
        int nMarcoCount = proPack.Data.L1 / nMarcoLen;
        UCHAR ucResponse;
        memcpy(&ucResponse, (void *)(proPack.Data.Dat1.ucTmp + nMarcoCount * nMarcoLen), 1);
        processResponse(strDeviceName, ucResponse);
    }
    else
    {
        QString strInfo = QString("%1: 收到未知协议报文!").arg(strDeviceName);
        CLog::addLog(strInfo);
    }
}

void RecvDataThread::processZEROSTDDcu(PROCOTOL_PACK &proPack)
{
    QString strInfo;
    QString strDeviceName = QString("校零信标一体机");
    //工作状态上报
    if(0x0701 == proPack.C && 0xEA13 == proPack.Data.T1)
    {
        memcpy(g_pZCBeaconStatu, &proPack.Data.Dat1.zcBeaconStatu, proPack.Data.L1);
    }
    //处理校零信标一体机参数设置响应信息
    else if (0x200 == proPack.C && 0xEA14 == proPack.Data.T1)
    {
        UCHAR ucResponse;
        memcpy(&ucResponse, (void *)(proPack.Data.Dat1.answer), 1);
        processResponse(strDeviceName, ucResponse);
    }
    else
    {
        strInfo = QString("%1: 收到未知协议报文!").arg(strDeviceName);
        CLog::addLog(strInfo);
    }
}

void RecvDataThread::processSLDDcu(PROCOTOL_PACK &proPack)
{
    QString strInfo;
    QString strDeviceName = QString("射频分路组合监控");
    //工作状态上报
    if(0x0701 == proPack.C && 0xEA01 == proPack.Data.T1)
    {
        memcpy(g_pSLDDcuParaStatu, &proPack.Data.Dat1.sldDcuParaStatu, proPack.Data.L1);
    }
    //处理校零信标一体机参数设置响应信息
    else if (0x200 == proPack.C && 0xEA02 == proPack.Data.T1)
    {
        UCHAR ucResponse;
        memcpy(&ucResponse, (void *)(proPack.Data.Dat1.answer), 1);
        processResponse(strDeviceName, ucResponse);
    }
    else
    {
        strInfo = QString("%1: 收到未知协议报文!").arg(strDeviceName);
        CLog::addLog(strInfo);
    }
}

void RecvDataThread::processSswitch(PROCOTOL_PACK &proPack)
{
    QString strInfo;
    QString strDeviceName = QString("射频切换开关监控");
    //工作状态上报
    if(0x0701 == proPack.C && 0xEA03 == proPack.Data.T1)
    {
        memcpy(g_pSswitchDcuStatu, &proPack.Data.Dat1.ssDcuStatu, proPack.Data.L1);
    }
    //处理校射频切换开关监控响应信息
    else if (0x200 == proPack.C && 0xEA04 == proPack.Data.T1)
    {
        UCHAR ucResponse;
        memcpy(&ucResponse, (void *)(proPack.Data.Dat1.answer), 1);
        processResponse(strDeviceName, ucResponse);
    }
    else
    {
        strInfo = QString("%1: 收到未知协议报文!").arg(strDeviceName);
        CLog::addLog(strInfo);
    }
}

void RecvDataThread::processTestSwitch(PROCOTOL_PACK &proPack)
{
    QString strInfo;
    QString strDeviceName = QString("测试开关矩阵");
    //工作状态上报
    if(0x0701 == proPack.C && 0xEA0F == proPack.Data.T1)
    {
        memcpy(g_pTstSwMatStatu, &proPack.Data.Dat1.tstSwMatStatu, proPack.Data.L1);
    }
    //处理测试开关矩阵响应信息
    else if (0x200 == proPack.C && 0xEA10 == proPack.Data.T1)
    {
        UCHAR ucResponse;
        memcpy(&ucResponse, (void *)(proPack.Data.Dat1.answer), 1);
        processResponse(strDeviceName, ucResponse);
    }
    else
    {
        strInfo = QString("%1: 收到未知协议报文!").arg(strDeviceName);
        CLog::addLog(strInfo);
    }
}

void RecvDataThread::processFreqBDcu(PROCOTOL_PACK &proPack)
{
    QString strInfo;
    QString strDeviceName = QString("频率基准源监控");
    //工作状态上报
    if(0x0701 == proPack.C && 0xEA11 == proPack.Data.T1)
    {
        memcpy(g_pFreDcuStatu, &proPack.Data.Dat1.freDcuStatu, proPack.Data.L1);
    }
    //处理频率基准源监控响应信息
    else if (0x200 == proPack.C && 0xEA12 == proPack.Data.T1)
    {
        UCHAR ucResponse;
        memcpy(&ucResponse, (void *)(proPack.Data.Dat1.answer), 1);
        processResponse(strDeviceName, ucResponse);
    }
    else
    {
        strInfo = QString("%1: 收到未知协议报文!").arg(strDeviceName);
        CLog::addLog(strInfo);
    }
}

void RecvDataThread::processRCLOCKDcu(PROCOTOL_PACK &proPack)
{
    //判断数据长度是否
    QString strInfo;
    QString strDeviceName = QString("铷钟机箱监控");
    //工作状态上报
    if(0x0701 == proPack.C && 0xEA20 == proPack.Data.T1)
    {
        //判断数据长度是否大于 21字节
        if (proPack.Data.L1 < 21)
        {
            strInfo = QString("%1: 数据报文长度小于21!").arg(strDeviceName);
            CLog::addLog(strInfo);
            return;
        }
        analyseRCLOCKDcu(proPack.Data.Dat1.ucTmp, proPack.Data.L1);
    }
    //处理频率基准源监控响应信息
    else if (0x200 == proPack.C && 0xEA19 == proPack.Data.T1)
    {
        UCHAR ucResponse;
        memcpy(&ucResponse, (void *)(proPack.Data.Dat1.answer), 1);
        processResponse(strDeviceName, ucResponse);
    }
    else
    {
        strInfo = QString("%1: 收到未知协议报文!").arg(strDeviceName);
        CLog::addLog(strInfo);
    }
}

void RecvDataThread::processNetSwitch(PROCOTOL_PACK &proPack)
{
    QString strInfo;
    QString strDeviceName = QString("中频开关矩阵");
    //工作状态上报
    if(0x0701 == proPack.C && 0xE499 == proPack.Data.T1)
    {
        memcpy(g_pIFSWMatStatuResponse, &proPack.Data.Dat1.ifSwMatParaStatus, proPack.Data.L1);
    }
    //处理中频开关矩阵响应信息
    else if (0x200 == proPack.C && 0xE498 == proPack.Data.T1)
    {
        UCHAR ucResponse;
        memcpy(&ucResponse, (void *)(proPack.Data.Dat1.answer), 1);
        processResponse(strDeviceName, ucResponse);
    }
    else
    {
        strInfo = QString("%1: 收到未知协议报文!").arg(strDeviceName);
        CLog::addLog(strInfo);
    }
}

void RecvDataThread::analyseRCLOCKDcu(UCHAR *pData, unsigned int nLen)
{
    if (pData == NULL || nLen <= 0)
        return;

    int nIndex = 0;
    int nTmpLen = 0;
    //时间区间
    nTmpLen = sizeof(g_pRCLOCKDCUStatu->sTimeZone);
    memcpy(&g_pRCLOCKDCUStatu->sTimeZone, pData, nTmpLen);
    nIndex += nTmpLen;
    //时间信息
    //年
    nTmpLen = sizeof(g_pRCLOCKDCUStatu->sYear);
    memcpy(&g_pRCLOCKDCUStatu->sYear, pData + nIndex, nTmpLen);
    nIndex += nTmpLen;
    //月
    nTmpLen = sizeof(g_pRCLOCKDCUStatu->ucMonth);
    memcpy(&g_pRCLOCKDCUStatu->ucMonth, pData + nIndex, nTmpLen);
    nIndex += nTmpLen;
    //日
    nTmpLen = sizeof(g_pRCLOCKDCUStatu->ucDay);
    memcpy(&g_pRCLOCKDCUStatu->ucDay, pData + nIndex, nTmpLen);
    nIndex += nTmpLen;
    //时
    nTmpLen = sizeof(g_pRCLOCKDCUStatu->ucHour);
    memcpy(&g_pRCLOCKDCUStatu->ucHour, pData + nIndex, nTmpLen);
    nIndex += nTmpLen;
    //分
    nTmpLen = sizeof(g_pRCLOCKDCUStatu->ucMin);
    memcpy(&g_pRCLOCKDCUStatu->ucMin, pData + nIndex, nTmpLen);
    nIndex += nTmpLen;
    //秒
    nTmpLen = sizeof(g_pRCLOCKDCUStatu->ucSec);
    memcpy(&g_pRCLOCKDCUStatu->ucSec, pData + nIndex, nTmpLen);
    nIndex += nTmpLen;
    //获取参考有效状态数据
    UCHAR ucTmp;
    nTmpLen = sizeof(UCHAR);
    memcpy(&ucTmp, pData + nIndex, nTmpLen);
    nIndex += nTmpLen;
    //解析gps状态 和 Bit1北斗状态
    g_pRCLOCKDCUStatu->ucGPSStatus = ucTmp & 0x01;
    g_pRCLOCKDCUStatu->ucNorthStatus = ucTmp &0x02;
    //获取10MHz输出状态数据
    memcpy(&ucTmp, pData + nIndex, nTmpLen);
    nIndex += nTmpLen;
    //1路10MHz输出状态
    g_pRCLOCKDCUStatu->ucOne10MHzInput = ucTmp & 0x01;
    //2路10MHz输出状态
    g_pRCLOCKDCUStatu->ucTwo10MHzInput = ucTmp & 0x02;
    //3路10MHz输出状态
    g_pRCLOCKDCUStatu->ucThird10MHzInput = ucTmp & 0x04;
    //4路10MHz输出状态
    g_pRCLOCKDCUStatu->ucFour10MHzInput = ucTmp & 0x08;
    //获取IPP输出状态数据
    memcpy(&ucTmp, pData + nIndex, nTmpLen);
    nIndex += nTmpLen;
    //1路1PPS输出状态
    g_pRCLOCKDCUStatu->ucOne1PPSInput = ucTmp & 0x01;
    //2路1PPS输出状态
    g_pRCLOCKDCUStatu->ucTwo1PPSInput = ucTmp &0x02;
    //同步状态  0-未同步 1-同步
    g_pRCLOCKDCUStatu->ucSyncStatus = pData[nIndex];
    nIndex += nTmpLen;
    //铷钟锁定状态 0-未锁定 1-锁定
    g_pRCLOCKDCUStatu->ucRCLOCK = pData[nIndex];
    nIndex += nTmpLen;
    //解调秒状态 0-无  1-有
    g_pRCLOCKDCUStatu->ucDemodulateSec = pData[nIndex];
    nIndex += nTmpLen;
    //守时状态 0-未守时 1-守时
    g_pRCLOCKDCUStatu->ucPunctual = pData[nIndex];
    nIndex += nTmpLen;
    //告警状态 0-不告警 1-告警
    g_pRCLOCKDCUStatu->ucWarning = pData[nIndex];
    nIndex += nTmpLen;
    //电源状态 0-异常 1-正常
    g_pRCLOCKDCUStatu->ucPower = pData[nIndex];
    nIndex += nTmpLen;
    //控制模式：0-本控 1-分控
    g_pRCLOCKDCUStatu->ucConMode = pData[nIndex];
    nIndex += nTmpLen;
    //驯服模式 0-驯服 1-不驯服
    g_pRCLOCKDCUStatu->ucTameCtrl = pData[nIndex];
    nIndex += nTmpLen;
    //同步方式：0-手动 1-自动
    g_pRCLOCKDCUStatu->ucSyncCtrl = pData[nIndex];
    nIndex += nTmpLen;
}

void RecvDataThread::processResponse(QString strDeviceName, UCHAR ucAnswerFlag)
{
    QString strType("返回结果");
    QString strAnswer = getAnswerFlag(ucAnswerFlag);
    QString strInfo = QString("%1 %2: %3!").arg(strDeviceName, strType, strAnswer);
    CLog::addLog(strInfo);
}

QString RecvDataThread::getAnswerFlag(UCHAR ucAnsFlag)
{
    QString strAnswer("未知应答标识");
    switch (ucAnsFlag)
    {
    case 0xFF:
        strAnswer = QString("正常执行");
        break;
    case 0x61:
        strAnswer = QString("非法码");
        break;
    case 0x62:
        strAnswer = QString("帧校验出错");
        break;
    case 0x63:
        strAnswer = QString("条件不具备不执行");
        break;
    case 0x64:
        strAnswer = QString("超限码，参数超限");
        break;
    default:
        break;
    }
    return strAnswer;
}
