#include "cbaseband.h"
#include "../../Common/Log.h"
#include "DataStructTTCDevCtlInterface.h"
#include "Global.h"

CBaseband::CBaseband()
{
    m_connect = 0;
    m_connectOld = m_connect;
    m_sysMod = UNKNOWN;
    m_pUSBAbstract = NULL;
    m_pBbeAbstract = NULL;
    m_pSS1Abstract = NULL;
    m_pSS2Abstract = NULL;
}

bool CBaseband::init(USHORT devID, PlatformResource* pRes, SMCU2::MCSSSTCStatus4* pTC)
{
    m_devID = devID;
    m_pPlatformRes = pRes;
    bool isOK = false;
    DATA_SOURCE_ALLOC(DSZ_BBE_GlobalStatusBlk, m_pBbeAbstract, "基带全局信息元", m_devID, 0);
    m_ddtTcID = m_pPlatformRes->m_nIDCurrent;
    DATA_SOURCE_ALLOC(USBBBE_GlobalStatusStruct, m_pUSBAbstract, "基带全局信息元", m_devID, 1);
    m_usbID = m_pPlatformRes->m_nIDCurrent;
    DATA_SOURCE_ALLOC(SS1BBE_GlobalStatusStruct, m_pSS1Abstract, "基带全局信息元", m_devID, 2);
    m_ss1ID = m_pPlatformRes->m_nIDCurrent;
    DATA_SOURCE_ALLOC(SS2BBE_GlobalStatusStruct, m_pSS2Abstract, "基带全局信息元", m_devID, 3);
    m_ss2ID = m_pPlatformRes->m_nIDCurrent;
    do
    {
        if(!m_ddt.init(m_devID, m_pPlatformRes))
            break;
        if(!m_ttc.init(m_devID,m_pPlatformRes, pTC))
            break;
        if(!m_ss1.init(m_devID, m_pPlatformRes, pTC))
            break;
        if(!m_ss2.init(m_devID, m_pPlatformRes, pTC))
            break;
        if(!m_tc.init(m_devID, m_pPlatformRes, pTC))
            break;
        isOK = true;
    }while(false);
    return isOK;
}

void CBaseband::processBaseBand(QByteArray &datagram, const QString& chlID)
{

    if(chlID == "DDTSMCU1" || chlID == "DDTSMCU2")
    {
        m_sysMod = DDT;
        m_ddt.processBaseBand(datagram);
        m_ddt.updateAbstract(m_pBbeAbstract);
        m_pPlatformRes->m_pStatMgr->IncreaseTimeTag(m_ddtTcID);

    }
    else if(chlID == "USBSMCU1" || chlID == "USBSMCU2")
    {
        m_sysMod = USB;
        m_ttc.processBaseBand(datagram);
        m_ttc.updateAbstract(m_pUSBAbstract);
        m_pBbeAbstract->ucBBEWorkID = m_pUSBAbstract->ucBBEWorkID;
        m_pBbeAbstract->DestMCSSysStatus.dwSystemMode = m_pUSBAbstract->ucSystemWorkMode;
        m_pBbeAbstract->DestMCSSysStatus.ucSystemWorkMode = m_pUSBAbstract->ucSystemMode;
        m_pPlatformRes->m_pStatMgr->IncreaseTimeTag(m_usbID);
    }
    else if(chlID == "SS1SMCU1" || chlID == "SS1SMCU2")
    {
        m_sysMod = SS1;
        m_ss1.processBaseBand(datagram);
        m_ss1.updateAbstract(m_pSS1Abstract);
        m_pBbeAbstract->ucBBEWorkID = m_pSS1Abstract->ucBBEWorkID;
        m_pBbeAbstract->DestMCSSysStatus.dwSystemMode = m_pSS1Abstract->ucSystemWorkMode;
        m_pBbeAbstract->DestMCSSysStatus.ucSystemWorkMode = m_pSS1Abstract->ucSystemMode;
        m_pPlatformRes->m_pStatMgr->IncreaseTimeTag(m_ss1ID);
    }
    else if(chlID == "SS2SMCU1" || chlID == "SS2SMCU2")
    {
        m_sysMod = SS2;
        m_ss2.processBaseBand(datagram);
        m_ss2.updateAbstract(m_pSS2Abstract);
        m_pBbeAbstract->ucBBEWorkID = m_pSS2Abstract->ucBBEWorkID;
        m_pBbeAbstract->DestMCSSysStatus.dwSystemMode = m_pSS2Abstract->ucSystemWorkMode;
        m_pBbeAbstract->DestMCSSysStatus.ucSystemWorkMode = m_pSS2Abstract->ucSystemMode;
        m_pPlatformRes->m_pStatMgr->IncreaseTimeTag(m_ss2ID);
    }
    else if(chlID == "TCSMCU1" || chlID == "TCSMCU2")
    {
        m_sysMod = TC;
        m_tc.processBaseBand(datagram);
        m_tc.updateAbstract(m_pBbeAbstract);
        m_pPlatformRes->m_pStatMgr->IncreaseTimeTag(m_ddtTcID);
    }

    ++m_connect;
}

void CBaseband::getDevInfo(char &bbe, char &bbeControl)
{
    //m_connect == m_connectOld  m_sysMod == UNKNOWN为机带未连接状态
    //当有状态上报时，m_connect != m_connectOld  m_sysMod != UNKNOWN
    //当连接断开时应回复未连接状态
    //只有连接状态和由连接转到未连接时需要对bbe bbeControl进行处理。
    if(m_connect == m_connectOld)
    {
        if(UNKNOWN != m_sysMod)
        {
            bbe = (char)0;
            bbeControl = (char)0;
            if(TC == m_sysMod && m_tc.m_pTC4_4)
            {
                memset(m_tc.m_pTC4_4, 0, sizeof(SMCU2::MCSSSTCStatus4));
            }
            m_sysMod = UNKNOWN;
            if(m_pBbeAbstract)
                memset(m_pBbeAbstract, 0, sizeof(DSZ_BBE_GlobalStatusBlk));
            if(m_pUSBAbstract)
                memset(m_pUSBAbstract, 0, sizeof(USBBBE_GlobalStatusStruct));
            if(m_pSS1Abstract)
                memset(m_pSS1Abstract, 0, sizeof(SS1BBE_GlobalStatusStruct));
            if(m_pSS2Abstract)
                memset(m_pSS2Abstract, 0, sizeof(SS2BBE_GlobalStatusStruct));
        }
        return;
    }
    m_connectOld = m_connect;
    bbe = (char)0;
    bbeControl = (char)0;
    switch (m_sysMod) {
    case DDT:

        bbe = (char)m_ddt.m_pMCSSysStatus->ucSystemMode;
        bbeControl = (char)m_ddt.m_pMCSSysStatus->ucMonitorMode;
        break;
    case USB:
        bbe = (char)m_ttc.m_pMCSSysDataStatusTTC1->ucSystemMode;
        bbeControl = (char)m_ttc.m_pMCSSysDataStatusTTC1->ucMonitorMode;
        break;
    case SS1:
        bbe = (char)m_ss1.m_pMCSSysTarget4Status1->sysStatu.ucSystemMode;
        bbeControl = (char)m_ss1.m_pMCSSysTarget4Status1->sysStatu.ucMonitorMode;
        break;
    case SS2:
        bbe = (char)m_ss2.m_pMCSSysTarget4Status1->MCSSysStatu.ucSystemMode;
        bbeControl = (char)m_ss2.m_pMCSSysTarget4Status1->MCSSysStatu.ucMonitorMode;
        break;
    case TC:
        bbe = m_tc.m_pMCSSysTarget4Status1->MCSSysStatu.ucSystemMode;
        bbeControl = (char)m_tc.m_pMCSSysTarget4Status1->MCSSysStatu.ucMonitorMode;
        break;
    default:
        ;
    }
}

