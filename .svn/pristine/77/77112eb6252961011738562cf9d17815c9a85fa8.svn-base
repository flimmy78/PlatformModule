#include "cbaseband.h"
#include "../../Common/Log.h"
#include "DataStructTTCRemoteInterface.h"
#include "Global.h"

CBaseband::CBaseband()
{
    m_connect = false;
    m_pBbeAbstract = NULL;
    m_pUSBAbstract = NULL;
    m_pSS1Abstract = 0;
    m_pSS2Abstract = 0;
}
void CBaseband::set(SBBE& bbe)
{
    m_ddt.set(bbe.ddt);
    m_ttc.set(bbe.ttc);
    m_ss1.set(bbe.ss1);
    m_ss2.set(bbe.ss2);
    m_tc.set(bbe.tc);
    if(!m_pBbeAbstract || !m_pUSBAbstract)return;
    memcpy(m_pBbeAbstract, &bbe.m_bbeAbstract, sizeof(bbe.m_bbeAbstract));
    memcpy(m_pUSBAbstract, &bbe.m_USBAbstract, sizeof(bbe.m_USBAbstract));
    memcpy(m_pSS1Abstract, &bbe.m_SS1Abstract, sizeof(SS1BBE_GlobalStatusStruct));
    memcpy(m_pSS2Abstract, &bbe.m_SS2Abstract, sizeof(SS2BBE_GlobalStatusStruct));
}
bool CBaseband::init(USHORT devID, PlatformResource* pRes, SMCU2::MCSSSTCStatus4* pTC)
{
    m_devID = devID;
    m_pPlatformRes = pRes;
    bool isOK = false;
    DATA_SOURCE_ALLOC(DSZ_BBE_GlobalStatusBlk, m_pBbeAbstract, "基带全局信息元", m_devID, 0);
    DATA_SOURCE_ALLOC(USBBBE_GlobalStatusStruct, m_pUSBAbstract, "基带全局信息元", m_devID, 1);
    DATA_SOURCE_ALLOC(SS1BBE_GlobalStatusStruct, m_pSS1Abstract, "基带全局信息元", m_devID, 2);
    DATA_SOURCE_ALLOC(SS2BBE_GlobalStatusStruct, m_pSS2Abstract, "基带全局信息元", m_devID, 3);
    do
    {
        if(!m_ddt.init(m_devID, m_pPlatformRes))
            break;
        if(!m_ttc.init(m_devID,m_pPlatformRes))
            break;
        if(!m_ss1.init(m_devID, m_pPlatformRes))
            break;
        if(!m_ss2.init(m_devID, m_pPlatformRes))
            break;
        if(!m_tc.init(m_devID, m_pPlatformRes, pTC))
            break;
        isOK = true;
    }while(false);
    return isOK;
}

void CBaseband::processBaseBand(QByteArray &datagram)
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
    QMap<int, QPair<QString, QString> >& sidSys = m_pPlatformRes->m_devMap.m_mpSysSid;
    QMap<int, QPair<QString, QString> >::iterator iter = sidSys.find(netSDFrm.SID);
    if(iter == sidSys.end())
        return;
    if(iter.value().first == "DDT")
    {
        m_sysMod = DDT;
        m_ddt.processBaseBand(datagram);
    }
    else if(iter.value().first == "USB")
    {
        m_sysMod = USB;
        m_ttc.processBaseBand(datagram);
    }
    else if(iter.value().first == "SS1")
    {
        m_sysMod = SS1;
        m_ss1.processBaseBand(datagram);
    }
    else if(iter.value().first == "SS2")
    {
        m_sysMod = SS2;
        m_ss2.processBaseBand(datagram);
    }
    else if(iter.value().first == "TC")
    {
        m_sysMod = TC;
        m_tc.processBaseBand(datagram);
    }
    m_connect = (char)1;
}

void CBaseband::getDevInfo(char &bbe)
{
}

