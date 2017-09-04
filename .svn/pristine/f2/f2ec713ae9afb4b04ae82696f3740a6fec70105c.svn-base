#include "CSS2.h"
#include "../../Common/Log.h"
#include "DataStructTMServerInterface.h"
#include "Global.h"
#include "../../Common/interfaces/cdatasource.h"
CSS2::CSS2()
{
}

bool CSS2::init(USHORT deviceID, PlatformResource* pRes)
{
    m_pPlatformRes = pRes;
    DATA_SOURCE_ALLOC(SMCU2::MCSSSB2PNCODEStatusStruct4, m_p2MCSSSPNCODEStatu4_9, "模式2扩频码设置", deviceID, 0)
    DATA_SOURCE_ALLOC(SMCU2::MCSSSB2TMSimulStatusStruct4, m_pMCSSSB2TMSimulStatu4_8, "扩频遥测模拟源参数设置",deviceID, 1)
    DATA_SOURCE_ALLOC(SMCU2::MCSSSB2TMStatusStruct4, m_pMCSSSB2TMStatu4_6, "扩频遥测解调参数设置", deviceID,1)
    DATA_SOURCE_ALLOC(SMCU2::MCSSSTCStatusStruct4, m_pMCSSSB2TCStatu4_5, "模式2遥控参数设置", deviceID,0)
    DATA_SOURCE_ALLOC(SMCU2::MCSSSB2CjStatusStruct4, m_pMCSSSCjStatu4_4, "模式2测距测速参数设置",deviceID, 0)
    DATA_SOURCE_ALLOC(SMCU2::MCSUpModuleStatusStruct4, m_pMCSUpModuleStatu4_3, "模式2调制参数设置", deviceID,0)
    DATA_SOURCE_ALLOC(SMCU2::MCSRevStatusStruct4, m_pMCSRevStatusStruct4_2, "模式2接收参数设置", deviceID,0)
    DATA_SOURCE_ALLOC(SMCU2::MCSSysTarget4StatusStruct, m_pMCSSysTarget4Status1, "扩频2系统工作参数设置", deviceID,0)
    DATA_SOURCE_ALLOC_PRO_CTRL(PCRSS2, m_pcrSS, "过程控制命令", deviceID, 6)
    return true;
}

void CSS2::set(SSS2& ss2)
{
    memcpy(m_p2MCSSSPNCODEStatu4_9, &ss2.m_2MCSSSPNCODEStatu4_9,sizeof(ss2.m_2MCSSSPNCODEStatu4_9));
    memcpy(m_pMCSRevStatusStruct4_2, &ss2.m_MCSRevStatusStruct4_2,sizeof(ss2.m_MCSRevStatusStruct4_2));
    memcpy(m_pMCSSSB2TCStatu4_5, &ss2.m_MCSSSB2TCStatu4_5,sizeof(ss2.m_MCSSSB2TCStatu4_5));
    memcpy(m_pMCSSSB2TMSimulStatu4_8, &ss2.m_MCSSSB2TMSimulStatu4_8,sizeof(ss2.m_MCSSSB2TMSimulStatu4_8));
    memcpy(m_pMCSSSB2TMStatu4_6, &ss2.m_MCSSSB2TMStatu4_6,sizeof(ss2.m_MCSSSB2TMStatu4_6));
    memcpy(m_pMCSSSCjStatu4_4, &ss2.m_MCSSSCjStatu4_4,sizeof(ss2.m_MCSSSCjStatu4_4));
    memcpy(m_pMCSSysTarget4Status1, &ss2.m_MCSSysTarget4Status1,sizeof(ss2.m_MCSSysTarget4Status1));
    memcpy(m_pMCSUpModuleStatu4_3, &ss2.m_MCSUpModuleStatu4_3,sizeof(ss2.m_MCSUpModuleStatu4_3));
    memcpy(m_pcrSS, &ss2.m_pcrSS, sizeof(ss2.m_pcrSS));
}
void CSS2::processBaseBound_0xF100(char *pData)
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

void CSS2::processBaseBound_0xF104(char *pData, const int dataLen)
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

void CSS2::processBaseBound_0xF101(char * pData)
{
    //过程命令标识，在上报时，对低字节进行取反，高字节不变。
    //逆操作上述过程得到原来的过程命令标识.
    USHORT id = 0;
    memcpy(&id, pData, sizeof(USHORT));
    UCHAR ucCode = id;
    ucCode = 0xFF-ucCode;
    pData += 4;//过程命令标识与序列号共4个字节,各两个字节.
    switch(ucCode)
    {
    case 1:
        memcpy(&m_pcrSS->tcIssue1, pData, sizeof(ss2TCISSUE));
        break;
    case 3:
        memcpy(&m_pcrSS->tzBound3, pData, sizeof(ss2TZBOUND));
        break;
    case 4:
        memcpy(&m_pcrSS->eZero4, pData, sizeof(ss2EQUIPMENTZERO));
        break;
    case 8:
        memcpy(&m_pcrSS->ntIssue8, pData, sizeof(ss2NTISSUE));
        break;
    case 11:
        memcpy(&m_pcrSS->rctTst11, pData, sizeof(ss2SSTTCRCTTST));
        break;
    case 18:
        memcpy(&m_pcrSS->ssTone18, pData, sizeof(ss2SSTDRTST));
        break;
    case 19:
        memcpy(&m_pcrSS->drtTst19, pData, sizeof(ss2RTEFSTAT));
        break;
    case 20:
        memcpy(&m_pcrSS->ttcTst20, pData, sizeof(ss2BERTST));
        break;
    case 21:
        memcpy(&m_pcrSS->strcTst21, pData, sizeof(ss2FERTST));
        break;
    case 22:
        memcpy(&m_pcrSS->rcsreiTst22, pData, sizeof(ss2RCSREITST));
        break;
    case 23:
        memcpy(&m_pcrSS->dmreTst23, pData, sizeof(ss2DMRETST));
        break;
    case 24:
        memcpy(&m_pcrSS->smreTst24, pData, sizeof(ss2SMRETST));
        break;
    case 26:
        memcpy(&m_pcrSS->dSimu26, pData, sizeof(ss2DOPPLERSIMU));
        break;
    case 27:
        memcpy(&m_pcrSS->bbdtsSel27, pData, sizeof(ss2BBDTSSEL));
        break;
    case 29:
        memcpy(&m_pcrSS->dmdrrPort29, pData, sizeof(ss2DMDRRPORT));
        break;
    case 30:
        memcpy(&m_pcrSS->smdrePort30, pData, sizeof(ss2SMDREPORT));
        break;
    case 31:
        memcpy(&m_pcrSS->ttcdSave31, pData, sizeof(ss2TTCDSAVE));
        break;
    case 32:
        memcpy(&m_pcrSS->bzcrInquire32, pData, sizeof(ss2BZCRINQUIRE));
        break;
    case 35:
        memcpy(&m_pcrSS->r0Decuction35, pData, sizeof(ss2R0DEDUCTION));
        break;
    case 38:
        memcpy(&m_pcrSS->dpfpcRange38, pData, sizeof(ss2DPFPCRANGE));
        break;
    case 39:
        memcpy(&m_pcrSS->AGCcBind39, pData, sizeof(ss2AGCCBIND));
        break;
    case 40:
        memcpy(&m_pcrSS->dcCtrl40, pData, sizeof(DCCTRL));
        break;
    default:
        CLog::addLog("单元序号错误.\n");
    }
}

void CSS2::getMap(BYTE* pDataField, const int dataLen, QMap<int, int>& mapFB_FE)
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

void CSS2::processBaseBand(QByteArray &datagram)
{
    NETSDFRM netSDFrm;
    int nNetSDFRMLen = sizeof(NETSDFRM);
    memcpy(&netSDFrm, datagram.data(), nNetSDFRMLen);
    switch(netSDFrm.BID)
    {
    case 0xF100:
        //控制命令响应
        processBaseBound_0xF100(datagram.data() + nNetSDFRMLen);
        break;
    case 0xF104:
        //分机状态信息上报
        processBaseBound_0xF104(datagram.data()  + nNetSDFRMLen, netSDFrm.DataLen);
        break;
    case 0xF101:
        //控制结果上报
        processBaseBound_0xF101(datagram.data()  + nNetSDFRMLen);
        break;
    default:
        break;
    }
}

void CSS2::processBaseBoundData(BYTE *pBuff, const BYTE &unitNum)
{
    switch(unitNum)
    {
    case 1:
        SMCU2::MCSSysTarget4StatusStruct tmp;
        memcpy(&tmp, pBuff, sizeof(SMCU2::MCSSysTarget4StatusStruct));
        tmp.MCSSysStatu.ucSystemMode = 8;
        memcpy(m_pMCSSysTarget4Status1, &tmp, sizeof(SMCU2::MCSSysTarget4StatusStruct));
        break;
    case 2://模式2接收参数设置
        memcpy(m_pMCSRevStatusStruct4_2, pBuff, sizeof(SMCU2::MCSRevStatusStruct4));
        break;
    case 3://模式2调制参数设置
        memcpy(m_pMCSUpModuleStatu4_3, pBuff, sizeof(SMCU2::MCSUpModuleStatusStruct4));
        break;
    case 4://模式2测距测速参数设置
        memcpy(m_pMCSSSCjStatu4_4, pBuff, sizeof(SMCU2::MCSSSB2CjStatusStruct4));
        break;
    case 5://扩频遥控参数设置
        memcpy(m_pMCSSSB2TCStatu4_5, pBuff, sizeof(SMCU2::MCSSSTCStatusStruct4));
        break;
    case 6://扩频遥测解调参数设置
        memcpy(m_pMCSSSB2TMStatu4_6, pBuff, sizeof(SMCU2::MCSSSB2TMStatusStruct4));
        break;
    case 7:
        //memcpy();
        break;
    case 8://扩频遥测模拟源参数设置
        memcpy(m_pMCSSSB2TMSimulStatu4_8, pBuff, sizeof(SMCU2::MCSSSB2TMSimulStatusStruct4));
        break;
    case 9://模式2扩频码设置
        memcpy(m_p2MCSSSPNCODEStatu4_9, pBuff, sizeof(SMCU2::MCSSSB2PNCODEStatusStruct4));
        break;
    default:
        break;
    }
}
void CSS2::get(SSS2 &ss2){
    memcpy(&ss2.m_2MCSSSPNCODEStatu4_9, m_p2MCSSSPNCODEStatu4_9, sizeof(ss2.m_2MCSSSPNCODEStatu4_9));
    memcpy(&ss2.m_MCSRevStatusStruct4_2 ,m_pMCSRevStatusStruct4_2 ,sizeof(ss2.m_MCSRevStatusStruct4_2));
    memcpy(&ss2.m_MCSSSB2TCStatu4_5 ,m_pMCSSSB2TCStatu4_5,sizeof(ss2.m_MCSSSB2TCStatu4_5));
    memcpy(&ss2.m_MCSSSB2TMSimulStatu4_8 ,m_pMCSSSB2TMSimulStatu4_8,sizeof(ss2.m_MCSSSB2TMSimulStatu4_8));
    memcpy(&ss2.m_MCSSSB2TMStatu4_6 ,m_pMCSSSB2TMStatu4_6 ,sizeof(ss2.m_MCSSSB2TMStatu4_6));
    memcpy(&ss2.m_MCSSSCjStatu4_4 ,m_pMCSSSCjStatu4_4 ,sizeof(ss2.m_MCSSSCjStatu4_4));
    memcpy(&ss2.m_MCSSysTarget4Status1 ,m_pMCSSysTarget4Status1 ,sizeof(ss2.m_MCSSysTarget4Status1));
    memcpy(&ss2.m_MCSUpModuleStatu4_3 , m_pMCSUpModuleStatu4_3,sizeof(ss2.m_MCSUpModuleStatu4_3));
    memcpy(&ss2.m_pcrSS , m_pcrSS, sizeof(ss2.m_pcrSS));

}
