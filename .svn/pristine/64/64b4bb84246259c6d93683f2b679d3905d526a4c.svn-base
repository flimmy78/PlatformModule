#include "RecvDataThread_tmremote.h"
#include <time.h>
#include <sys/time.h>
#include "Global.h"
#include <QtEndian>
#include "../../Common/Log.h"
#include "DataStructTTCRemoteInterface.h"
#include "TMRemoteInterface_global.h"
#include "util.h"
#include "devices.h"

bool CNetSwitch::init(USHORT devID, PlatformResource* res, bool bUp)
{
    QString strDeviceName = bUp ? ("上行") : ("下行");
    strDeviceName += "中频开关矩阵监控";
    m_pPlatformRes = res;
    DATA_SOURCE_ALLOC(HK_IFSWMatStatu, m_pIFSWMatStatuResponse, strDeviceName, devID, 0);
    return true;
}

void CNetSwitch::set(SNetSwitch& ns)
{
    memcpy(m_pIFSWMatStatuResponse, &ns.m_IFSWMatStatuResponse, sizeof(ns.m_IFSWMatStatuResponse));
}

QString CNetSwitch::getAnswerFlag(UCHAR ucAnsFlag)
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

void CNetSwitch::processNetSwitch(QByteArray& datagram, const QString &strName)
{
    ++m_connect;
    PROCOTOL_PACK proPack;
    memcpy(&proPack, datagram.data(), datagram.size());
    QString strInfo;
    //工作状态上报
    //上行开关矩阵 处理程序
    if(0x0701 == proPack.C && 0xEA0D == proPack.Data.T1)
    {
        memcpy(m_pIFSWMatStatuResponse, &proPack.Data.Dat1.ifSwMatParaStatus, proPack.Data.L1);
    }
    //处理中频开关矩阵响应信息
    else if (0x200 == proPack.C && 0xEA0C == proPack.Data.T1)
    {
        UCHAR ucResponse = (UCHAR)proPack.Data.Dat1.answer;
        processResponse(strName, ucResponse);
    }
    //下行开关矩阵 处理程序
    else if(0x0701 == proPack.C && 0xEA0B == proPack.Data.T1)
    {
        memcpy(m_pIFSWMatStatuResponse, &proPack.Data.Dat1.ifSwMatParaStatus, proPack.Data.L1);
    }
    else if (0x200 == proPack.C && 0xEA0E == proPack.Data.T1)
    {
        UCHAR ucResponse = (UCHAR)proPack.Data.Dat1.answer;
        processResponse(strName, ucResponse);
    }
    //
    else
    {
        strInfo = QString("%1: 收到未知协议报文!").arg(strName);
        CLog::addLog(strInfo);
    }
}

void CNetSwitch::processResponse(QString strDeviceName, UCHAR ucAnswerFlag)
{
    QString strType("返回结果");
    QString strAnswer = getAnswerFlag(ucAnswerFlag);
    QString strInfo = QString("%1 %2: %3!").arg(strDeviceName, strType, strAnswer);
    CLog::addLog(strInfo);
}
void CNetSwitch::getDevInfo(char& connect)
{
    connect = m_connect;
    m_connect = char(0);
}

CDevices::CDevices():
    m_const_0(char(0)),
    m_const_1(char(1))
{
    zeroConn();
}

void CDevices::zeroConn()
{
    m_connStdRSP = m_const_0;
    m_connSSRSP = m_const_0;
    m_connZEROSTD = m_const_0;
    m_connSLDC = m_const_0;
    m_connSswitch = m_const_0;
    m_connTestNetSwitch = m_const_0;
    m_connFREQB = m_const_0;
    m_connRCLOCK = m_const_0;
}

bool CDevices::init(PlatformResource *res)
{
    USHORT devID = 0x400;
    m_pPlatformRes = res;
    DATA_SOURCE_ALLOC(COMBTRANSStatu, m_pCombTransStatuTTC, "标准TTC联试应答机监控", 0x700, 0);
    DATA_SOURCE_ALLOC(COMBTRANSStatu, m_pCombTransStatuFXG, "非相干扩频联试应答机监控", 0x700, 1);
    DATA_SOURCE_ALLOC(ZEROCALIBBEACONStatu, m_pZCBeaconStatu, "校零信标一体机监控", 0x400, 0);
    DATA_SOURCE_ALLOC(SLDCDCUStatu, m_pSLDDcuParaStatu, "射频分路组合监控", 0x400, 0);
    DATA_SOURCE_ALLOC(SSWITCHDCUStatu, m_pSswitchDcuStatu, "射频切换开关监控", 0x400, 0);
    DATA_SOURCE_ALLOC(TEST_SWMATStatu, m_pTstSwMatStatu, "测试开关矩阵监控", 0x3D00, 0);
    DATA_SOURCE_ALLOC(FREQBDCUStatu, m_pFreDcuStatu, "频率基准源监控", 0x3E00, 0);
    DATA_SOURCE_ALLOC(RCLOCKDCUStatu, m_pRCLOCKDCUStatu, "铷钟机箱监控", 0x2500, 0);
    DATA_SOURCE_ALLOC(COMBTRANSPara, m_pCombTransParaTTC, RSP_MACRO_SEARCH, 0x500, 0);
    DATA_SOURCE_ALLOC(COMBTRANSPara, m_pCombTransParaFXG, RSP_MACRO_SEARCH, 0x500, 1);
    return true;
}

void CDevices::set(SDevs& devs)
{
    memcpy(m_pCombTransStatuTTC, &devs.m_CombTransStatuTTC, sizeof(devs.m_CombTransStatuTTC));
    memcpy(m_pCombTransStatuFXG,&devs.m_CombTransStatuFXG, sizeof(devs.m_CombTransStatuFXG));
    memcpy(m_pZCBeaconStatu, &devs.m_ZCBeaconStatu, sizeof(devs.m_ZCBeaconStatu));
    memcpy(m_pSLDDcuParaStatu, &devs.m_SLDDcuParaStatu, sizeof(devs.m_SLDDcuParaStatu));
    memcpy(m_pSswitchDcuStatu, &devs.m_SswitchDcuStatu, sizeof(devs.m_SswitchDcuStatu));
    memcpy(m_pTstSwMatStatu, &devs.m_TstSwMatStatu, sizeof(devs.m_TstSwMatStatu));
    memcpy(m_pFreDcuStatu, &devs.m_FreDcuStatu, sizeof(devs.m_FreDcuStatu));
    memcpy(m_pRCLOCKDCUStatu, &devs.m_RCLOCKDCUStatu, sizeof(devs.m_RCLOCKDCUStatu));
    memcpy(m_pCombTransParaTTC, &devs.m_CombTransParaTTC, sizeof(devs.m_CombTransParaTTC));
    memcpy(m_pCombTransParaFXG, &devs.m_CombTransParaFXG, sizeof(devs.m_CombTransParaFXG));   
}

void CDevices::get(SDevs& devs)
{
    memcpy(&devs.m_CombTransStatuTTC, m_pCombTransStatuTTC, sizeof(devs.m_CombTransStatuTTC));
    memcpy(&devs.m_CombTransStatuFXG, m_pCombTransStatuFXG, sizeof(devs.m_CombTransStatuFXG));
    memcpy(&devs.m_ZCBeaconStatu, m_pZCBeaconStatu, sizeof(devs.m_ZCBeaconStatu));
    memcpy(&devs.m_SLDDcuParaStatu, m_pSLDDcuParaStatu, sizeof(devs.m_SLDDcuParaStatu));
    memcpy(&devs.m_SswitchDcuStatu, m_pSswitchDcuStatu, sizeof(devs.m_SswitchDcuStatu));
    memcpy(&devs.m_TstSwMatStatu, m_pTstSwMatStatu, sizeof(devs.m_TstSwMatStatu));
    memcpy(&devs.m_FreDcuStatu, m_pFreDcuStatu, sizeof(devs.m_FreDcuStatu));
    memcpy(&devs.m_RCLOCKDCUStatu, m_pRCLOCKDCUStatu, sizeof(devs.m_RCLOCKDCUStatu));
    memcpy(&devs.m_CombTransParaTTC, m_pCombTransParaTTC, sizeof(devs.m_CombTransParaTTC));
    memcpy(&devs.m_CombTransParaFXG, m_pCombTransParaFXG, sizeof(devs.m_CombTransParaFXG));
}

void CDevices::getDevInfo(DevicesConnectStatus *pConnectSt)
{
    if(!pConnectSt)return;
//    pConnectSt->usbRSP = m_connStdRSP;
//    pConnectSt->ss2RSP = m_connSSRSP;
    pConnectSt->zeroSTD = m_connZEROSTD;
    pConnectSt->slDivcom = m_connSLDC;
    pConnectSt->sSwitch = m_connSswitch;
    pConnectSt->testnet_switch = m_connTestNetSwitch;
    pConnectSt->freqB = m_connFREQB;
    pConnectSt->rClock = m_connRCLOCK;
    zeroConn();
}

void CDevices::proceProcotol(QByteArray &datagram)
{
    PROCOTOL_PACK proPack;
    //int nSize = sizeof(PROCOTOL_PACK);
    memcpy(&proPack, datagram.data(), datagram.size());

    //如果数据源为标准TTC联试应答机监控
    if (0x1400 == proPack.S)
    {
        m_connStdRSP = m_const_1;
        processCombTrans(proPack);
    }
    //非相干扩频联试应答机监控
    else if (0x1401 == proPack.S)
    {
        m_connSSRSP = m_const_1;
        processCombTrans(proPack, false);
    }
    //校零信表一体机
    else if (0xEA14 == proPack.S)
    {
        m_connZEROSTD = m_const_1;
        processZEROSTDDcu(proPack);
    }
    //射频分路组合监控
    else if (0x2C00 == proPack.S)
    {
        m_connSLDC = m_const_1;
        processSLDDcu(proPack);
    }
    //射频切换开关监控
    else if (0x2C01 == proPack.S)
    {
        m_connSswitch = m_const_1;
        processSswitch(proPack);
    }
    //测试开关矩阵
    else if (0x2C07 == proPack.S)
    {
        m_connTestNetSwitch = m_const_1;
        processTestSwitch(proPack);
    }
    //频率基准源监控
    else if (0x2C08 == proPack.S)
    {
        m_connFREQB = m_const_1;
        processFreqBDcu(proPack);
    }
    //铷钟机箱监控
    else if (0x2C20 == proPack.S)
    {
        m_connRCLOCK = m_const_1;
        processRCLOCKDcu(proPack);
    }
}

void CDevices::processCombTrans(PROCOTOL_PACK &proPack, bool bTTC)
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
            memcpy(m_pCombTransStatuTTC, &proPack.Data.Dat1.svSysStatu, proPack.Data.L1);
        else
            memcpy(m_pCombTransStatuFXG, &proPack.Data.Dat1.svSysStatu, proPack.Data.L1);
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
            m_pCombTransParaTTC->nParaCount = nMarcoCount;
            for (; i < nMarcoCount; i++)
            {
                memcpy(&m_pCombTransParaTTC->paras[i], (void *)(proPack.Data.Dat1.ucTmp + i * nMarcoLen), nMarcoLen);
            }
            //拷贝标志到响应中 从缓存中取数据
            memcpy(&m_pCombTransParaTTC->ucAnswerFlag, (void *)(proPack.Data.Dat1.ucTmp + i * nMarcoLen), 1);
        }
        else
        {
            m_pCombTransParaFXG->nParaCount = nMarcoCount;
            for (; i < nMarcoCount; i++)
            {
                memcpy(&m_pCombTransParaFXG->paras[i], (void *)(proPack.Data.Dat1.ucTmp + i * nMarcoLen), nMarcoLen);
            }
            //拷贝标志到响应中 从缓存中取数据
            memcpy(&m_pCombTransParaFXG->ucAnswerFlag, (void *)(proPack.Data.Dat1.ucTmp + i * nMarcoLen), 1);
        }
    }
    //处理联试应答机监控参数宏调用信息
    else if (0x200 == proPack.C && 0x0206 == proPack.Data.T1)
    {
        //QString strType("宏参数调用返回结果");
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

void CDevices::processZEROSTDDcu(PROCOTOL_PACK &proPack)
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
        UCHAR ucResponse = (UCHAR)proPack.Data.Dat1.answer;
        processResponse(strDeviceName, ucResponse);
    }
    else
    {
        strInfo = QString("%1: 收到未知协议报文!").arg(strDeviceName);
        CLog::addLog(strInfo);
    }
}

void CDevices::processSLDDcu(PROCOTOL_PACK &proPack)
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
        UCHAR ucResponse = (UCHAR)proPack.Data.Dat1.answer;
        processResponse(strDeviceName, ucResponse);
    }
    else
    {
        strInfo = QString("%1: 收到未知协议报文!").arg(strDeviceName);
        CLog::addLog(strInfo);
    }
}

void CDevices::processSswitch(PROCOTOL_PACK &proPack)
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
        UCHAR ucResponse = (UCHAR)proPack.Data.Dat1.answer;
        processResponse(strDeviceName, ucResponse);
    }
    else
    {
        strInfo = QString("%1: 收到未知协议报文!").arg(strDeviceName);
        CLog::addLog(strInfo);
    }
}

void CDevices::processTestSwitch(PROCOTOL_PACK &proPack)
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
        UCHAR ucResponse = (UCHAR)proPack.Data.Dat1.answer;
        processResponse(strDeviceName, ucResponse);
    }
    else
    {
        strInfo = QString("%1: 收到未知协议报文!").arg(strDeviceName);
        CLog::addLog(strInfo);
    }
}

void CDevices::processFreqBDcu(PROCOTOL_PACK &proPack)
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
        UCHAR ucResponse = (CHAR)proPack.Data.Dat1.answer;
        processResponse(strDeviceName, ucResponse);
    }
    else
    {
        strInfo = QString("%1: 收到未知协议报文!").arg(strDeviceName);
        CLog::addLog(strInfo);
    }
}

void CDevices::processRCLOCKDcu(PROCOTOL_PACK &proPack)
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
        UCHAR ucResponse = (CHAR)proPack.Data.Dat1.answer;
        processResponse(strDeviceName, ucResponse);
    }
    else
    {
        strInfo = QString("%1: 收到未知协议报文!").arg(strDeviceName);
        CLog::addLog(strInfo);
    }
}


void CDevices::analyseRCLOCKDcu(UCHAR *pData, unsigned int nLen)
{
    if (pData == NULL || nLen <= 0)
        return;

    int nIndex = 0;
    int nTmpLen = 0;
    //时间区间
    nTmpLen = sizeof(g_pRCLOCKDCUStatu->ucTimeZone);
    memcpy(&g_pRCLOCKDCUStatu->ucTimeZone, pData, nTmpLen);
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

void CDevices::processResponse(QString strDeviceName, UCHAR ucAnswerFlag)
{
    QString strType("返回结果");
    QString strAnswer = getAnswerFlag(ucAnswerFlag);
    QString strInfo = QString("%1 %2: %3!").arg(strDeviceName, strType, strAnswer);
    CLog::addLog(strInfo);
}

QString CDevices::getAnswerFlag(UCHAR ucAnsFlag)
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


