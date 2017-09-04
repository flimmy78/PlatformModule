#include "ProcessModule.h"
#include "Global.h"
#include "XmlReader.h"
#include "../Common/Log.h"

void ProcessBasebandMonitorDataTrans(DATACONTENT& msg)
{
    char* tmpBuff = new char[msg.iDataLen + 3];
    //进行数据内容的封装
    //单元号
    int packLen = 0;
    memcpy(tmpBuff, (UCHAR*)&msg.subItem.usUintNo, 1);
    packLen += 1;
    //顺序号
    int indexLen = sizeof(unsigned short);
    unsigned short usIndex = (unsigned short)msg.seq;
    memcpy(tmpBuff + packLen, &usIndex, indexLen);
    ++usIndex;
    packLen += indexLen;
    memcpy(tmpBuff + packLen, msg.pData, msg.iDataLen);
    msg.iDataLen += 3;
    msg.pData = tmpBuff;
    g_showDatagram.m_datagramCommSend = CHelper::sendBaseBandPro(msg);
    delete tmpBuff;
}

//jc 中频开关矩阵处理函数
void ProcessSwitchMatrix(DATACONTENT& msg)
{
    g_showDatagram.m_datagramCommSend = CHelper::sendSwMatPro(msg);
}

//组参数处理
void ProcessGroupCMDBaseBandCommon(TLxTsspParamMacroInformation macroInfo,
                                   ILxTsspParameterMacro* macroObj,DATACONTENT& msg,
                                   QString baseBandName, int baseBandIndex)
{
    QString _strTemp;
    DevicesConnectStatus& devLinkStatu = *(g_context.m_pDeviceLinkStatus);
    if(devLinkStatu.bbe[baseBandIndex] == 0)
    {
        CLog::addLog(baseBandName + "不在线无法进行组参数设置！", 3);
        return;
    }
    if(g_context.m_pBBBGlobalStatus[baseBandIndex].DestMCSSysStatus.ucMonitorMode !=1)
    {
        _strTemp = baseBandName + "处于分控无法进行组参数设置！";
        CLog::addLog(_strTemp, 3);
        return;
    }
    uint tmpModu = g_context.m_pBBBGlobalStatus[baseBandIndex].DestMCSSysStatus.dwSystemMode;
    if(false == CHelper::updateSubitemOfMsgForGroupCmd(msg, baseBandIndex))//设置channel
        return;
    // 基带
    if(macroInfo.aDevModeIdx[baseBandIndex] == 3)  // 数传
    {
        if(tmpModu != 4)
        {
            _strTemp = baseBandName + "当前模式与下发宏参数模式不对应，无法进行组参数设置！";
            CLog::addLog(_strTemp, 3);
            return;
        }
        ProcessGroupDataTrans(macroObj,msg);
        _strTemp = baseBandName + "加载数传组参数！";
        CLog::addLog(_strTemp);
    }
    else if(macroInfo.aDevModeIdx[baseBandIndex] == 4)  // TC
    {
        if(tmpModu != 5)
        {
            _strTemp = baseBandName + "当前模式与下发宏参数模式不对应，无法进行组参数设置！";
            CLog::addLog(_strTemp, 3);
            return;
        }
        ProcessGroupTC(macroObj,msg);
        _strTemp = baseBandName + "加载TC组参数！";
        CLog::addLog(_strTemp);
    }
    else if(macroInfo.aDevModeIdx[baseBandIndex] == 5)//5 TTC
    {
        if(tmpModu != 6)
        {
            _strTemp = baseBandName + "当前模式与下发宏参数模式不对应，无法进行组参数设置！";
            CLog::addLog(_strTemp, 3);
            return;
        }
        ProcessGroupTTC(macroObj,msg);
        _strTemp = baseBandName + "加载TTC组参数！";
        CLog::addLog(_strTemp);
    }
    else if(macroInfo.aDevModeIdx[baseBandIndex] == 6)// 6 ss1
    {
        if(tmpModu != 7)
        {
            _strTemp = baseBandName + "当前模式与下发宏参数模式不对应，无法进行组参数设置！";
            CLog::addLog(_strTemp, 3);
            return;
        }
        ProcessGroupSMCU1(macroObj,msg);
        _strTemp = baseBandName + "加载ss1组参数！";
        CLog::addLog(_strTemp);
    }
    else if(macroInfo.aDevModeIdx[baseBandIndex] == 7)//7 ss2
    {
        if(tmpModu != 8)
        {
            _strTemp = baseBandName + "当前模式与下发宏参数模式不对应，无法进行组参数设置！";
            CLog::addLog(_strTemp, 3);
            return;
        }
        ProcessGroupSMCU2(macroObj,msg);
        _strTemp = baseBandName + "加载ss2组参数！";
        CLog::addLog(_strTemp);
    }
}

void ProcessGroupCMD(DATACONTENT& msg)
{
    uint macro = 0;
    memcpy(&macro, msg.pData, 4);
    LxTsspObjectManager* pObjMgr = LxTsspObjectManager::Instance();
    if(!pObjMgr)
    {
        CLog::addLog("处理组参数命令时, LxTsspObjectManager 获取失败.");
        return;
    }
    ILxTsspParamMacroManager* pMacroMgr = pObjMgr->GetParamMacroManager();
    if(!pMacroMgr)
    {
        CLog::addLog("处理组参数命令时, ILxTsspParamMacroManager获取失败.");
        return;
    }
    if(!CHelper::HasDevicesConnectStatus())
    {
        CLog::addLog("处理组参数命令时, 设备状态获取失败.");
        return;
    }
    TLxTsspParamMacroInformation macroInfo;
    pMacroMgr->GetMacroInformation(macro, macroInfo);
    ILxTsspParameterMacro* macroObj = NULL;
    pMacroMgr->GetMacro(macro, macroObj);
    ProcessGroupCMDBaseBandCommon(macroInfo, macroObj, msg, "基带一", 0);
    ProcessGroupCMDBaseBandCommon(macroInfo, macroObj, msg, "基带二", 1);
    ProcessGroupCMDBaseBandCommon(macroInfo, macroObj, msg, "基带三", 2);
    //ak基带 todo
}


#define INI_GROUP_STRUCT(MODULENAME, INDEX, STRUCT) \
    do{\
    tmpId = CHelper::getMacroIDFromModuleNameAndIndex(MODULENAME, INDEX);\
    paramBlock.clear();\
    if(-1 == macroObj->GetParameterBlock(tmpId, paramBlock))\
    {\
        CLog::addLog(QString(MODULENAME) + ", 宏参数获取失败.");\
        return;\
    }\
    memcpy(&STRUCT, paramBlock.data(), paramBlock.size());\
    }while(false);

void ProcessGroupTC(ILxTsspParameterMacro* /*macroObj*/, DATACONTENT /*msg*/)
{
//数据不知如何获取, 所以注释之.
//    TCGroup tc;
//    CHelper::initDataTrans(tc);
//    QByteArray paramBlock;
//    qint64 tmpId = 0;
//    INI_GROUP_STRUCT("单遥控扩频码参数设置", 0, tc.mSSPNCode1)
//    INI_GROUP_STRUCT("单遥控调制参数设置", 0, tc.UpModu2.)
//    INI_GROUP_STRUCT("单遥控遥控参数设置", 0, tc.MCSSSTCParam3)
//    msg.pData = (char*)&tc;
//    msg.iDataLen = sizeof(tc);
//    g_showDatagram.m_datagramCommSend = CHelper::sendBaseBandPro(msg);

}
void ProcessGroupDataTrans(ILxTsspParameterMacro* macroObj, DATACONTENT msg)
{
    MSCDataTransGroup dt;
    CHelper::initDataTrans(dt);
    QByteArray paramBlock;
    qint64 tmpId = 0;
    INI_GROUP_STRUCT("DDT_BBE系统参数设置", 0, dt.MCSSysParam)
    INI_GROUP_STRUCT("数传接收机参数设置", 0, dt.MCSDDTRev)
    INI_GROUP_STRUCT("数传解调参数设置", 0, dt.MCSDDTTMI)
    INI_GROUP_STRUCT("数传解调参数设置", 0, dt.MCSDDTTMQ)
    INI_GROUP_STRUCT("数传信号源参数设置", 0, dt.MCSDDTTMSimul)
    INI_GROUP_STRUCT("分集合成参数设置", 0, dt.MCSDivCom)
    //INI_GROUP_STRUCT("存储转发单元设置", 0, dt.MCSDDTReplay)
    msg.pData = (char*)&dt;
    msg.iDataLen = sizeof(dt);
    g_showDatagram.m_datagramCommSend = CHelper::sendBaseBandPro(msg);
}

void ProcessGroupTTC(ILxTsspParameterMacro* macroObj, DATACONTENT msg)
{
    USBTTCGroup ttc;
    CHelper::initTTC(ttc);
    QByteArray paramBlock;
    qint64 tmpId = 0;
    INI_GROUP_STRUCT("USBBBE系统参数设置", 0, ttc.MCSSysDataTTC1)
    INI_GROUP_STRUCT("USBBBE中频接收机参数设置", 0, ttc.MCSUSBRev2)
    INI_GROUP_STRUCT("USBBBE中频接收机参数设置", 1, ttc.MCSUSBRev3)
    INI_GROUP_STRUCT("USBBBE调制参数设置", 0, ttc.MCSUSBUpModulParam4)
    INI_GROUP_STRUCT("USBBBE测距参数设置", 0, ttc.MCSUSBCjParam5)
    INI_GROUP_STRUCT("USBBBE测距参数设置", 1, ttc.MCSUSBCjParam6)
    INI_GROUP_STRUCT("USBBBE遥控参数设置", 0, ttc.MCSUSBTCParam7)
    INI_GROUP_STRUCT("USBBBE遥测参数设置", 0, ttc.MCSUSBTMParam8)
    INI_GROUP_STRUCT("USBBBE模拟源参数设置", 0, ttc.MCSUSBTMSimulParamC)
    msg.pData = (char*)&ttc;
    msg.iDataLen = sizeof(ttc);
    g_showDatagram.m_datagramCommSend = CHelper::sendBaseBandPro(msg);
}
void ProcessGroupSMCU1(ILxTsspParameterMacro* macroObj, DATACONTENT msg)
{
    SSBBEGROUPPARA smcu;
    CHelper::initSMCU(smcu);
    QByteArray paramBlock;
    qint64 tmpId = 0;
    INI_GROUP_STRUCT("扩频系统工作参数设置", 0, smcu.GlobalSetPara1)
    INI_GROUP_STRUCT("模式1接收参数设置", 0, smcu.mcsSSRec2)
    INI_GROUP_STRUCT("模式1调制参数设置", 0, smcu.UpModul3)
    INI_GROUP_STRUCT("模式1测距测速参数设置", 0, smcu.MCSSSCjParam4)
    INI_GROUP_STRUCT("扩频遥控参数设置", 0, smcu.MCSSSTCParam5)
    INI_GROUP_STRUCT("扩频遥测解调参数设置", 0, smcu.MCSSSTMParam6)
    INI_GROUP_STRUCT("扩频遥测模拟源参数设置", 0, smcu.MCSSSTMSimulParam8)
    INI_GROUP_STRUCT("模式1扩频码设置", 0, smcu.mSSPNCode9)
    msg.pData = (char*)&smcu;
    msg.iDataLen = sizeof(smcu);
    g_showDatagram.m_datagramCommSend = CHelper::sendBaseBandPro(msg);
}
void ProcessGroupSMCU2(ILxTsspParameterMacro* macroObj, DATACONTENT msg)
{
    SSBBEGROUPPARA smcu;
    CHelper::initSMCU(smcu);
    QByteArray paramBlock;
    qint64 tmpId = 0;
    INI_GROUP_STRUCT("扩频系统工作参数设置", 1, smcu.GlobalSetPara1)
    INI_GROUP_STRUCT("模式2接收参数设置", 0, smcu.mcsSSRec2)
    INI_GROUP_STRUCT("模式2调制参数设置", 0, smcu.UpModul3)
    INI_GROUP_STRUCT("模式2测距测速参数设置", 0, smcu.MCSSSCjParam4)
    INI_GROUP_STRUCT("扩频遥控参数设置", 1, smcu.MCSSSTCParam5)
    INI_GROUP_STRUCT("扩频遥测解调参数设置", 1, smcu.MCSSSTMParam6)
    INI_GROUP_STRUCT("扩频遥测模拟源参数设置", 1, smcu.MCSSSTMSimulParam8)
    INI_GROUP_STRUCT("模式2扩频码设置", 0, smcu.mSSPNCode9)
    msg.pData = (char*)&smcu;
    msg.iDataLen = sizeof(smcu);
    g_showDatagram.m_datagramCommSend = CHelper::sendBaseBandPro(msg);
}

void ProcessControlCMD(DATACONTENT& msg)
{
    static USHORT usSeq = 0;
    char* tmpBuff = new char[msg.iDataLen + 4];
    tmpBuff[0] = msg.subItem.usUintNo;//过程命令标识
    tmpBuff[1] = 'M';
    memcpy(tmpBuff+2, &usSeq, 2);//序号
    ++usSeq;
    memcpy(tmpBuff + 4, msg.pData, msg.iDataLen);
    msg.pData = tmpBuff;
    msg.iDataLen += 4;
    g_showDatagram.m_datagramCommSend = CHelper::sendBaseBandPro(msg);
    delete tmpBuff;
}

void ProcessBaseBandStatusCheck(DATACONTENT& msg)
{
    g_showDatagram.m_datagramStatuSend = CHelper::sendBaseBandPro(msg);
}

void ProcessSswitchDcu(DATACONTENT& msg)
{
    g_showDatagram.m_datagramCommSend = CHelper::sendSwMatPro(msg);
}

void ProcessRSPMarcoSearch(DATACONTENT& msg)
{
    g_showDatagram.m_datagramCommSend = CHelper::sendRSPData(msg);
}

void ProcessRSPMarcoCall(DATACONTENT& msg)
{
    g_showDatagram.m_datagramCommSend = CHelper::sendRSPData(msg);
}

void ProcessRSPMarcoCmdCtrl(DATACONTENT& msg)
{
    g_showDatagram.m_datagramCommSend = CHelper::sendRSPData(msg);
}

PFProcessMsg GetProcessFunction(const QString& objName)
{
    QMap<QString, PFProcessMsg> namePF;
    namePF["DDT_BBE系统参数设置"] = ProcessBasebandMonitorDataTrans;//
    namePF["数传接收机参数设置"] = ProcessBasebandMonitorDataTrans;//
    namePF["数传解调参数设置"] = ProcessBasebandMonitorDataTrans;//
    namePF["分集合成参数设置"] = ProcessBasebandMonitorDataTrans;//
    namePF["数传信号源参数设置"] = ProcessBasebandMonitorDataTrans;//
    namePF["数传检测输出参数设置"] = ProcessBasebandMonitorDataTrans;//6
    namePF["USBBBE中频接收机参数设置"] = ProcessBasebandMonitorDataTrans;//
    namePF["USBBBE遥测参数设置"] = ProcessBasebandMonitorDataTrans;//
    namePF["USBBBE模拟源参数设置"] = ProcessBasebandMonitorDataTrans;//
    namePF["USBBBE测距参数设置"] = ProcessBasebandMonitorDataTrans;//
    namePF["USBBBE调制参数设置"] = ProcessBasebandMonitorDataTrans;//
    namePF["USBBBE遥控参数设置"] = ProcessBasebandMonitorDataTrans;//6
    namePF["USBBBE系统参数设置"] = ProcessBasebandMonitorDataTrans;//
    namePF["扩频系统工作参数设置"] = ProcessBasebandMonitorDataTrans;//
    namePF["模式1接收参数设置"] = ProcessBasebandMonitorDataTrans;//找不到结构，延后。
    namePF["模式1调制参数设置"] = ProcessBasebandMonitorDataTrans;//
    namePF["扩频遥控参数设置"] = ProcessBasebandMonitorDataTrans;//
    namePF["扩频遥测解调参数设置"] = ProcessBasebandMonitorDataTrans;//6
    namePF["模式1测距测速参数设置"] = ProcessBasebandMonitorDataTrans;//
    namePF["扩频遥测模拟源参数设置"] = ProcessBasebandMonitorDataTrans;//
    namePF["扩频角误差输出参数设置"] = ProcessBasebandMonitorDataTrans;//找不到结构，延后
    namePF["模式1扩频码设置"] = ProcessBasebandMonitorDataTrans;//
    namePF["扩频检测输出控制"] = ProcessBasebandMonitorDataTrans;//找不到结构，延后
    namePF["模式2接收参数设置"] = ProcessBasebandMonitorDataTrans;//6找不到结构，延后。
    namePF["模式2调制参数设置"] = ProcessBasebandMonitorDataTrans;//
    namePF["模式2测距测速参数设置"] = ProcessBasebandMonitorDataTrans;//
    namePF["模式2扩频码设置"] = ProcessBasebandMonitorDataTrans;//
    namePF["检前记录设备控制"] = ProcessBasebandMonitorDataTrans;//找不到结构，延后
    namePF["测试开关矩阵监控"] = ProcessSwitchMatrix;//
    namePF["中频开关矩阵监控"] = ProcessSwitchMatrix;//
    namePF["射频切换开关监控"] = ProcessSwitchMatrix;//
    namePF["射频分路组合监控"] = ProcessSswitchDcu;//
    namePF["标准TTC联试应答机监控"] = ProcessSwitchMatrix;//
    namePF["非相干扩频联试应答机监控"] = ProcessSwitchMatrix;//
    namePF["校零信标一体机监控"] = ProcessBasebandMonitorDataTrans;//
    namePF["铷钟机箱监控"] = ProcessBasebandMonitorDataTrans;//没实现
    namePF["频率基准源监控"] = ProcessBasebandMonitorDataTrans;//
    namePF["状态查询命令"] = ProcessBaseBandStatusCheck;
    //wl add 联试应答机相关处理函数
    namePF[RSP_MACRO_SEARCH] = ProcessRSPMarcoSearch;
    namePF[RSP_MACRO_CALL] = ProcessRSPMarcoCall;
    namePF[RSPDCU_CMD] = ProcessRSPMarcoCmdCtrl;
    //wl end
    namePF["过程控制命令"] = ProcessControlCMD;//
    namePF["组参数设置命令"] = ProcessGroupCMD;
    QMap<QString, PFProcessMsg>::iterator it = namePF.find(objName);
    if (it == namePF.end())
        return NULL;
    return *it;
}

UINT CHelper::getBCD(int iYear, int iMonth, int iDay)
{
    //转换年
    int tmp = iYear % 10;
    int bcd = 0;
    bcd = bcd | tmp << 16;
    iYear = iYear / 10;
    tmp = iYear % 10;
    bcd = bcd | tmp << 20;
    iYear = iYear / 10;
    tmp = iYear % 10;
    bcd = bcd | tmp << 24;
    iYear = iYear / 10;
    tmp = iYear % 10;
    bcd = bcd | tmp << 28;

    //转换月
    tmp = iMonth % 10;
    bcd = bcd | tmp << 8;
    tmp = (iMonth / 10) % 10;
    bcd = bcd | tmp << 12;

    //转换日
    tmp = iDay % 10;
    bcd = bcd | tmp ;
    tmp = (iDay / 10) % 10;
    bcd = bcd | tmp << 4;

    return bcd;
}

UINT CHelper::getDate()
{
    QDate date = QDate::currentDate();
    int year(0), month(0), day(0);
    date.getDate(&year, &month, &day);
    return getBCD(year, month, day);
}

UINT CHelper::getTime()
{
    QTime time = QTime::currentTime();
    QTime time0(0, 0);
    UINT tmp = (UINT)(time0.msecsTo(time));
    return tmp;
}

void CHelper::sendData(uchar* pPack, int packLen, QString strID)
{
    m_pObjectManager = LxTsspObjectManager::Instance();
    if (!m_pObjectManager)
        return;
    mpCommunicationMgr = (ILxTsspCommunicationManager *)m_pObjectManager->GetCommunicationManager();
    if (!mpCommunicationMgr)
        return;
    ICommunicationChannel *pChannel = NULL;
    mpCommunicationMgr->GetChannel(strID, pChannel);
    if (NULL == pChannel)
    {
        CLog::addLog("get channel failed.........." );
        return;
    }
    if(-1 == pChannel->SendData(pPack, packLen))
        CLog::addLog("channel send pack failed.........." );
}

QByteArray CHelper::sendSwMatPro(DATACONTENT& msg)
{
    PROCOTOL_PACK swMatPro;
    memset(&swMatPro, 0, sizeof(swMatPro));
    swMatPro.H = 0x01;
    swMatPro.SEQ = msg.seq;
    swMatPro.A = 0x02;
    swMatPro.C = msg.subItem.usCMD;
    swMatPro.Data.T1 = msg.subItem.usTID;
    swMatPro.S = swMatPro.Data.S1 = msg.subItem.uPDXPSMCU;
    swMatPro.D = swMatPro.Data.D1 = msg.subItem.uPDXPDEVICES;
    memcpy(&swMatPro.Data.Dat1, msg.pData, msg.iDataLen);
    swMatPro.L = sizeof(PROCOTOL_PACK) - sizeof(swMatPro.Data) + msg.iDataLen;
    swMatPro.Data.L1 = msg.iDataLen;
    CHelper::sendData((uchar*)&swMatPro, swMatPro.L, msg.subItem.szChannel);
    QByteArray datagram;
    datagram.resize(sizeof(swMatPro));
    memcpy(datagram.data(), &swMatPro, datagram.size());
    return datagram;
}

QByteArray CHelper::sendRSPData(DATACONTENT &msg)
{
    PROCOTOL_PACK pack;
    memset(&pack, 0, sizeof(pack));
    pack.H = 0x00;
    pack.SEQ = msg.seq;
    pack.M = 0x00;
    pack.T = 0x01;
    pack.L = sizeof(PROCOTOL_PACK) - sizeof(pack.Data.Dat1) + msg.iDataLen;
    pack.C = msg.subItem.usCMD;
    pack.A = 0x02;
    pack.S = pack.Data.S1 = msg.subItem.uPDXPSMCU;
    pack.D = pack.Data.D1 = msg.subItem.uPDXPDEVICES;
    pack.R = 0x00;
    pack.Data.T1 = msg.subItem.usTID;
    memcpy(&pack.Data.Dat1, msg.pData, msg.iDataLen);
    pack.Data.L1 = msg.iDataLen;
    CHelper::sendData((uchar*)&pack, pack.L, msg.subItem.szChannel);
    QByteArray datagram;
    datagram.resize(sizeof(pack));
    memcpy(datagram.data(), &pack, datagram.size());
    return datagram;
}

QByteArray CHelper::sendBaseBandPro(DATACONTENT& msg)
{
    //业务数据的长度为 序号长度 + 顺序号长度 + 数据包长度
    //进行网络协议数据帧封装
    NETSDFRM netSDF;
    memset(&netSDF, 0, sizeof(netSDF));
    netSDF.Date = CHelper::getDate();
    netSDF.Time = CHelper::getTime();
    netSDF.DataLen = msg.iDataLen;
    netSDF.BID = msg.iObjType;
    netSDF.DID = msg.subItem.uPDXPDEVICES;
    netSDF.SID = msg.subItem.uPDXPSMCU;
    unsigned char szBuf[MAX_SEND_BUFFER_LEN] = { 0 };
    int packLen = 0;
    int nHeadLen = sizeof(NETSDFRM);
    memcpy(szBuf + packLen, &netSDF, nHeadLen);
    packLen += nHeadLen;
    //封装具体的数据内容
    memcpy(szBuf + packLen, msg.pData, msg.iDataLen);
    packLen += msg.iDataLen;
    CHelper::sendData(szBuf, packLen, msg.subItem.szChannel);
    QByteArray datagram = (char*)szBuf;
    datagram.resize(packLen);
    memcpy(datagram.data(), szBuf, packLen);
    return datagram;
}

QString CHelper::getXmlPath(QString xmlFileName)
{
    QDir dir;
    QString strPath = dir.currentPath();
    QString filePath = strPath + "/TM/System/ModuleConfig/" + xmlFileName;
    return filePath;
}

void CHelper::initDataTrans(MSCDataTransGroup &dt)
{
    dt.wUnit1Head  = 0x01FB;
    dt.wUnit1End   = 0x01FE;
    dt.wUnit15Head = 0x0FFB;
    dt.wUnit15End  = 0x0FFE;
    dt.wUnit16Head = 0x10FB;
    dt.wUnit16End  = 0x10FE;
    dt.wUnit17Head = 0x11FB;
    dt.wUnit17End  = 0x11FE;
    dt.wUnit18Head = 0x12FB;
    dt.wUnit18End  = 0x12FE;
    dt.wUnit19Head = 0x13FB;
    dt.wUnit19End  = 0x13FE;
    dt.wUnit20Head = 0x14FB;
    dt.wUnit20End  = 0x14FE;
}

void CHelper::initTTC(USBTTCGroup& ttc)
{
    ttc.wUnit1Head = 0x01FB;
    ttc.wUnit1End  = 0x01FE;
    ttc.wUnit2Head = 0x02FB;
    ttc.wUnit2End  = 0x02FE;
    ttc.wUnit3Head = 0x03FB;
    ttc.wUnit3End  = 0x03FE;
    ttc.wUnit4Head = 0x04FB;
    ttc.wUnit4End  = 0x04FE;
    ttc.wUnit5Head = 0x05FB;
    ttc.wUnit5End  = 0x05FE;
    ttc.wUnit6Head = 0x06FB;
    ttc.wUnit6End  = 0x06FE;
    ttc.wUnit7Head = 0x07FB;
    ttc.wUnit7End  = 0x07FE;
    ttc.wUnit8Head = 0x08FB;
    ttc.wUnit8End  = 0x08FE;
    ttc.wUnitCHead = 0x0CFB;
    ttc.wUnitCEnd  = 0x0CFE;
}

void CHelper::initSMCU(SSBBEGROUPPARA& smcu)
{
    smcu.wUnit1Head = 0x01FB;
    smcu.wUnit1End  = 0x01FE;
    smcu.wUnit2Head = 0x02FB;
    smcu.wUnit2End  = 0x02FE;
    smcu.wUnit3Head = 0x03FB;
    smcu.wUnit3End  = 0x03FE;
    smcu.wUnit4Head = 0x04FB;
    smcu.wUnit4End  = 0x04FE;
    smcu.wUnit5Head = 0x05FB;
    smcu.wUnit5End  = 0x05FE;
    smcu.wUnit6Head = 0x06FB;
    smcu.wUnit6End  = 0x06FE;
    smcu.wUnit8Head = 0x08FB;
    smcu.wUnit8End  = 0x08FE;
    smcu.wUnit9Head = 0x09FB;
    smcu.wUnit9End  = 0x09FE;
}

void CHelper::initTC(TCGroup &tc)
{
    tc.wUnit1Head = 0x01FB;
    tc.wUnit1End  = 0x01FE;
    tc.wUnit2Head = 0x02FB;
    tc.wUnit2End  = 0x02FE;
    tc.wUnit3Head = 0x03FB;
    tc.wUnit3End  = 0x03FE;
}
extern quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
                     uchar ucSn);
qint64 CHelper::getMacroIDFromModuleNameAndIndex(QString moduleName, int index)
{
    ITEM* pItem = g_context.m_devMap.getItem(moduleName);
    if(!pItem || pItem->vSubItem.size() <= index)
        return 0;
    SUBITEM sItem = pItem->vSubItem[index];
    return GetID(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID, sItem.usTID, sItem.ucSN);
}
extern quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
              uchar ucSn);
bool CHelper::HasDevicesConnectStatus()
{
    if(g_context.m_pDeviceLinkStatus == NULL)
    {
        TLxTsspObjectInfo& tmpInfo = g_context.m_lsTsspObjInfo;
        quint64 _id_temp = g_pILxTsspStateManager->GetID(tmpInfo.usStationID,
                        tmpInfo.ucSubSysID, g_context.m_usDeviceID,0x9013, 0);
        int iLen = sizeof(DevicesConnectStatus);
        g_context.m_pDeviceLinkStatus =
                (DevicesConnectStatus*)g_pILxTsspStateManager->FindOneItem(_id_temp, iLen);
        if(iLen != sizeof(DevicesConnectStatus) || g_context.m_pDeviceLinkStatus == NULL)
            return false;
    }
    return true;
}

bool CHelper::updateSubitemOfMsgForGroupCmd(DATACONTENT& msg, const int& basebandIndex)
{
    ITEM* pItem = g_context.m_devMap.getItem("组参数设置命令");
    if(NULL == pItem || pItem->vSubItem.size() <= basebandIndex)
    {
        CLog::addLog("组参数设置命令, subitem获取失败, 请查看DeviceMapping.xml");
        return false;
    }
    msg.subItem = pItem->vSubItem[basebandIndex];
    return true;
}
