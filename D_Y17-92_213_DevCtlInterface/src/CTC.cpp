#include "CTC.h"
#include "../../Common/Log.h"
#include "DataStructTTCDevCtlInterface.h"
#include "Global.h"
CTC::CTC()
{
    m_pTC4_4 = NULL;
    m_pAkGlobalStatu = NULL;
    m_connectOld = m_connect = 0;
    m_change  = false;
}


bool CTC::init(USHORT deviceID, PlatformResource* pRes, SMCU2::MCSSSTCStatus4* pTC)
{
    m_devID = deviceID;
    m_pTC4_4 = pTC;
    m_pPlatformRes = pRes;
    DATA_SOURCE_ALLOC(SMCU2::MCSSSB2PNCODEStatusStruct4, m_p2MCSSSPNCODEStatu4_9, "单遥控扩频码参数设置", deviceID, 0)
    DATA_SOURCE_ALLOC(SMCU2::MCSSSTCStatusStruct4, m_pMCSSSB2TCStatu4_5, "单遥控遥控参数设置", deviceID,0)
    DATA_SOURCE_ALLOC(SMCU2::MCSUpModuleStatusStruct4, m_pMCSUpModuleStatu4_3, "单遥控调制参数设置", deviceID,0)
//            DATA_SOURCE_ALLOC(SMCU2::MCSSSB2PNCODEStatusStruct4, m_p2MCSSSPNCODEStatu4_9, "模式2扩频码设置", deviceID, 0)
//            DATA_SOURCE_ALLOC(SMCU2::MCSSSB2TMSimulStatusStruct4, m_pMCSSSB2TMSimulStatu4_8, "扩频遥测模拟源参数设置",deviceID, 1)
//            DATA_SOURCE_ALLOC(SMCU2::MCSSSB2TMStatusStruct4, m_pMCSSSB2TMStatu4_6, "扩频遥测解调参数设置", deviceID,1)
//            DATA_SOURCE_ALLOC(SMCU2::MCSSSB2TCStatusStruct4, m_pMCSSSB2TCStatu4_5, "扩频遥控参数设置", deviceID,1)
//    DATA_SOURCE_ALLOC(SMCU2::MCSSSB2CjStatusStruct4, m_pMCSSSCjStatu4_4, "模式2测距测速参数设置",deviceID, 0)
//            DATA_SOURCE_ALLOC(SMCU2::MCSUpModuleStatusStruct4, m_pMCSUpModuleStatu4_3, "模式2调制参数设置", deviceID,0)
//            DATA_SOURCE_ALLOC(SMCU2::MCSRevStatusStruct4, m_pMCSRevStatusStruct4_2, "模式2接收参数设置", deviceID,0)
    DATA_SOURCE_ALLOC(SMCU2::MCSSysTarget4StatusStruct, m_pMCSSysTarget4Status1, "单遥控系统工作参数设置", deviceID, 0)
    if (0x400 == m_devID || 0x401 == m_devID)
    {
        DATA_SOURCE_ALLOC_PRO_CTRL(PCRSS2, m_pcrSS, "过程控制命令", deviceID, 8)
    }
    if (0x700 == m_devID || 0x701 == m_devID)
    {
        DATA_SOURCE_ALLOC(AK_GLOBALSTATUS, m_pAkGlobalStatu, "安控全局信息", deviceID, 0)
        m_timeTagID = m_pPlatformRes->m_nIDCurrent;
        DATA_SOURCE_ALLOC_PRO_CTRL(PCRSS2, m_pcrSS, "过程控制命令", deviceID, 10)
    }
    return true;
}

void CTC::processBaseBound_0xF100(char *pData)
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

void CTC::processBaseBound_0xF104(char *pData, const int dataLen)
{
    BYTE* pDataField = new BYTE[dataLen];
    memcpy(pDataField, pData, dataLen);
    QMap<int, int> mapFB_FE;
    getMap(pDataField, dataLen, mapFB_FE);

    QMap<int, int>::iterator mpIter = mapFB_FE.begin();
    QMap<int, int>::const_iterator mpIterEnd = mapFB_FE.end();
    for(; mpIter != mpIterEnd; ++mpIter)
    {
        BYTE unitNum = pDataField[mpIter.key() + 1];
        BYTE* pBuff = pDataField + mpIter.key() + 2;
        processBaseBoundData(pBuff, unitNum);
    }
    delete pDataField;
}

void CTC::processBaseBound_0xF101(char * pData)
{
    //过程命令标识，在上报时，对低字节进行取反，高字节不变。
    //逆操作上述过程得到原来的过程命令标识.
    USHORT id = 0;
    memcpy(&id, pData, sizeof(USHORT));
    UCHAR ucCode = id;
    ucCode = 0xFF - ucCode;
    pData += 2;//过程命令标识与序列号各两个字节.
    switch(ucCode)
    {
//    case 1:
//        memcpy(&m_pcrSS->tcIssue1, pData, sizeof(ss2TCISSUE));
//        break;
//    case 3:
//        memcpy(&m_pcrSS->tzBound3, pData, sizeof(ss2TZBOUND));
//        break;
//    case 4:
//        memcpy(&m_pcrSS->eZero4, pData, sizeof(ss2EQUIPMENTZERO));
//        m_pcrSS->eZero4.qz0.valid = 3;
//        break;
//    case 8:
//        memcpy(&m_pcrSS->ntIssue8, pData, sizeof(ss2NTISSUE));
//        break;
//    case 11:
//        memcpy(&m_pcrSS->rctTst11, pData, sizeof(ss2SSTTCRCTTST));
//        break;
//    case 18:
//        memcpy(&m_pcrSS->ssTone18, pData, sizeof(ss2SSTDRTST));
//        break;
//    case 19:
//        memcpy(&m_pcrSS->drtTst19, pData, sizeof(ss2RTEFSTAT));
//        break;
//    case 20:
//        memcpy(&m_pcrSS->ttcTst20, pData, sizeof(ss2BERTST));
//        break;
//    case 21:
//        memcpy(&m_pcrSS->strcTst21, pData, sizeof(ss2FERTST));
//        break;
    case 22:
        memcpy(&m_pcrSS->rcsreiTst22, pData, sizeof(ss2RCSREITST));
        break;
//    case 23:
//        memcpy(&m_pcrSS->dmreTst23, pData, sizeof(ss2DMRETST));
//        break;
//    case 24:
//        memcpy(&m_pcrSS->smreTst24, pData, sizeof(ss2SMRETST));
//        break;
//    case 26:
//        memcpy(&m_pcrSS->dSimu26, pData, sizeof(ss2DOPPLERSIMU));
//        break;
//    case 27:
//        memcpy(&m_pcrSS->bbdtsSel27, pData, sizeof(ss2BBDTSSEL));
//        break;
//    case 29:
//        memcpy(&m_pcrSS->dmdrrPort29, pData, sizeof(ss2DMDRRPORT));
//        break;
//    case 30:
//        memcpy(&m_pcrSS->smdrePort30, pData, sizeof(ss2SMDREPORT));
//        break;
//    case 31:
//        memcpy(&m_pcrSS->ttcdSave31, pData, sizeof(ss2TTCDSAVE));
//        break;
//    case 32:
//        memcpy(&m_pcrSS->bzcrInquire32, pData, sizeof(ss2BZCRINQUIRE));
//        break;
//    case 35:
//        memcpy(&m_pcrSS->r0Decuction35, pData, sizeof(ss2R0DEDUCTION));
//        break;
//    case 38:
//        memcpy(&m_pcrSS->dpfpcRange38, pData, sizeof(ss2DPFPCRANGE));
//        break;
//    case 39:
//        memcpy(&m_pcrSS->AGCcBind39, pData, sizeof(ss2AGCCBIND));
//        break;
//    case 40:
//        memcpy(&m_pcrSS->dcCtrl40, pData, sizeof(DCCTRL));
//        break;
    default:
        CLog::addLog("单元序号错误.\n");
    }
}

void CTC::getMap(BYTE* pDataField, const int dataLen, QMap<int, int>& mapFB_FE)
{
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
}

void CTC::processBaseBand(QByteArray &datagram)
{
    NETSDFRM netSDFrm;
    int nNetSDFRMLen = sizeof(NETSDFRM);
    memcpy(&netSDFrm, datagram.data(), nNetSDFRMLen);
    switch(netSDFrm.BID)
    {
    case 0xF100:
        //控制命令响应
        processBaseBound_0xF100(datagram.data() + nNetSDFRMLen);
        g_showDatagram.m_datagramCommRev = datagram;
        break;
    case 0xF104:
        //分机状态信息上报
        processBaseBound_0xF104(datagram.data()  + nNetSDFRMLen, netSDFrm.DataLen);
        g_showDatagram.m_datagramStatuRev = datagram;
        break;
    case 0xF101:
        //控制结果上报
        processBaseBound_0xF101(datagram.data()  + nNetSDFRMLen);
        g_showDatagram.m_datagramCommRev = datagram;
        break;
    default:
        break;
    }
}

void CTC::processBaseBoundData(BYTE *pBuff, const BYTE &unitNum)
{
    switch(unitNum)
    {
    case 1:
        SMCU2::MCSSysTarget4StatusStruct tmp;
        memcpy(&tmp, pBuff, sizeof(SMCU2::MCSSysTarget4StatusStruct));
        tmp.MCSSysStatu.ucSystemMode = 6;
        memcpy(m_pMCSSysTarget4Status1, &tmp, sizeof(SMCU2::MCSSysTarget4StatusStruct));
        break;
    case 2://模式2接收参数设置
        //memcpy(m_pMCSRevStatusStruct4_2, pBuff, sizeof(SMCU2::MCSRevStatusStruct4));
        break;
    case 3://模式2调制参数设置
        memcpy(m_pMCSUpModuleStatu4_3, pBuff, sizeof(SMCU2::MCSUpModuleStatusStruct4));
        break;
//    case 4://单遥控发令状态
//        if(m_pTC4_4)
//        {
//            memcpy(m_pTC4_4, pBuff, sizeof(SMCU2::MCSSSTCStatus4));
//            m_pTC4_4->valid = 1;
//        }
//        break;
    case 5://扩频遥控参数设置
        memcpy(m_pMCSSSB2TCStatu4_5, pBuff, sizeof(SMCU2::MCSSSTCStatusStruct4));
        break;
    case 6://扩频遥测解调参数设置
        //memcpy(m_pMCSSSB2TMStatu4_6, pBuff, sizeof(SMCU2::MCSSSB2TMStatusStruct4));
        break;
    case 7:
        //memcpy();
        break;
    case 8://扩频遥测模拟源参数设置
        //memcpy(m_pMCSSSB2TMSimulStatu4_8, pBuff, sizeof(SMCU2::MCSSSB2TMSimulStatusStruct4));
        break;
    case 9://模式2扩频码设置
        memcpy(m_p2MCSSSPNCODEStatu4_9, pBuff, sizeof(SMCU2::MCSSSB2PNCODEStatusStruct4));
        break;
    }
}
void CTC::updateAbstractAK()
{
    if(!m_pAkGlobalStatu)return;
    m_pPlatformRes->m_pStatMgr->IncreaseTimeTag(m_timeTagID);
    ++m_connect;
    m_change = true;
//    DWORDLONG   dwlAnswerRevUpFreq;//上行工作频率，范围20250000~21200000，步进1Hz
//    DWORDLONG   dwlAnswerRevDownFreq;//下行工作频率，范围22000000~23000000，步进1Hz
    //m_pAkGlobalStatu->ucDownAlterLockStatuc
//    m_pMCSSysTarget4Status1->MCSSSB2TargetSysStatu[0].tParams.dwlAnswerRevDownFreq;
//    m_pMCSSysTarget4Status1->MCSSSB2TargetSysStatu[0].tParams.dwlAnswerRevUpFreq;
    m_pAkGlobalStatu->ucDownAlterLockStatuc = m_pMCSSysTarget4Status1->MCSSysStatu.ucDownFreqLock;
    m_pAkGlobalStatu->ucUpAlterLockStatuc = m_pMCSSysTarget4Status1->MCSSysStatu.ucUpFreqLock;
    //00H表示失锁；01H表示锁定            UCHAR  ucTimeCodeStatus;//外时码状态	0：无效，1：故障，2：正常
    //m_pAkGlobalStatu->ucTimeBoardLock = m_pMCSSysTarget4Status1->MCSSysStatu.ucTimeCodeStatus == 2 ? 1 : 0;
    m_pAkGlobalStatu->ucTimeBoardLock = m_pMCSSysTarget4Status1->MCSSysStatu.ucTimeCodeStatus;
    m_pAkGlobalStatu->byModu = m_pMCSUpModuleStatu4_3->m_MCSSSUpModulStatus.tParams.ucModulate - 1;
    m_pAkGlobalStatu->byOnline = m_pMCSUpModuleStatu4_3->m_MCSSSUpModulStatus.tParams.ucWaveOutCtrl == 1 ? 1 : 0;
    m_pAkGlobalStatu->ucSystemMode = 3;//按注释来的
    m_pAkGlobalStatu->ucCtrlMode = m_pMCSSysTarget4Status1->MCSSysStatu.ucMonitorMode - 1;
    m_pAkGlobalStatu->ucWorkMode = m_pMCSSysTarget4Status1->MCSSysStatu.ucSystemMode;//按注释来的
    //ucDevOnlineStatus; //设备在线状态：00H离线  01HA主B备 02B主A备
    //ucBBEOnline;		//本机在线状态	0：无效，1：在线，2：备份
    if(m_devID == 0x700){
        m_pAkGlobalStatu->ucDevOnlineStatus = m_pMCSSysTarget4Status1->MCSSysStatu.ucBBEOnline;
    }else{
        int tmp = m_pMCSSysTarget4Status1->MCSSysStatu.ucBBEOnline;
        if(tmp == 0){
            m_pAkGlobalStatu->ucDevOnlineStatus = 0;
        }else if(tmp == 1){
            m_pAkGlobalStatu->ucDevOnlineStatus = 2;
        }else if(tmp == 2){
            m_pAkGlobalStatu->ucDevOnlineStatus = 1;
        }
    }

    if(m_pAkGlobalStatu->ucDevOnlineStatus == 1 && m_devID == 0x700)
    {
        if(m_pTC4_4)
        {
            memcpy(m_pTC4_4, m_pMCSSSB2TCStatu4_5, sizeof(SMCU2::MCSSSTCStatusStruct4));
            m_pTC4_4->valid = 1;
        }
    }
    else if(m_pAkGlobalStatu->ucDevOnlineStatus == 2 && m_devID == 0x701)
    {
        if(m_pTC4_4)
        {
            memcpy(m_pTC4_4, m_pMCSSSB2TCStatu4_5, sizeof(SMCU2::MCSSSTCStatusStruct4));
            m_pTC4_4->valid = 1;
        }
    }

}
void CTC::updateAbstract(DSZ_BBE_GlobalStatusBlk* pBbeAbstract)
{
    if(!pBbeAbstract)return;
    //
    pBbeAbstract->ucMonitorMode = m_pMCSSysTarget4Status1->MCSSysStatu.ucMonitorMode - 1;
    pBbeAbstract->ucBBEWorkID = m_devID == 0x400 ? 1 : 2;
    pBbeAbstract->ucBBEID = m_devID == 0x400 ? 1 : 2;//fix me
    pBbeAbstract->ucBBEWorkStatus = m_pMCSSysTarget4Status1->MCSSysStatu.ucBBEWorkStatus;
    pBbeAbstract->ucSystemWorkMode = m_pMCSSysTarget4Status1->MCSSysStatu.ucSystemMode;//按注释来的
    pBbeAbstract->ucSystemMode = m_pMCSSysTarget4Status1->MCSSysStatu.ucSystemWorkMode;//按注释来的
    pBbeAbstract->uiMID = m_pMCSSysTarget4Status1->MCSSSB2TargetSysStatu[0].tParams.uiM;
    memcpy(pBbeAbstract->szTaskName, m_pMCSSysTarget4Status1->MCSSSB2TargetSysStatu[0].tParams.ucTaskDescribe, 6);

    pBbeAbstract->i64DownRF = m_pMCSSysTarget4Status1->MCSSSB2TargetSysStatu[0].tParams.dwlAnswerRevDownFreq;
    pBbeAbstract->dwSystemWorkMode = m_pMCSSysTarget4Status1->MCSSysStatu.ucSystemMode;

    pBbeAbstract->DestMCSSysStatus.ucMonitorMode = m_pMCSSysTarget4Status1->MCSSysStatu.ucMonitorMode;;
    pBbeAbstract->DestMCSSysStatus.dwSystemMode = m_pMCSSysTarget4Status1->MCSSysStatu.ucSystemMode;
    pBbeAbstract->DestMCSSysStatus.ucSystemWorkMode = m_pMCSSysTarget4Status1->MCSSysStatu.ucSystemWorkMode;

    pBbeAbstract->DestMCSSysStatus.ucTimeCodeStatus = m_pMCSSysTarget4Status1->MCSSysStatu.ucTimeCodeStatus;
    pBbeAbstract->DestMCSSysStatus.ucDTETranStatus = m_pMCSSysTarget4Status1->MCSSysStatu.ucBBEWorkStatus;

    memcpy(pBbeAbstract->DestMCSSysStatus.tParams.ucTaskDescribe, m_pMCSSysTarget4Status1->MCSSSB2TargetSysStatu[0].tParams.ucTaskDescribe, 6);
    pBbeAbstract->DestMCSSysStatus.tParams.uiM = m_pMCSSysTarget4Status1->MCSSSB2TargetSysStatu[0].tParams.uiM;
    pBbeAbstract->DestMCSSysStatus.tParams.dwlAnswerRevDownFreq = m_pMCSSysTarget4Status1->MCSSSB2TargetSysStatu[0].tParams.dwlAnswerRevDownFreq;
}

void CTC::getDevInfo(char &bbe)
{
    if(m_connect == m_connectOld)
    {
        if(m_change)
        {
            bbe = 0;
            m_change = false;
        }
        return;
    }
    m_connectOld = m_connect;
    bbe = 3;
}
