#include "CSS1.h"
#include "../../Common/Log.h"
#include "DataStructTMServerInterface.h"
#include "Global.h"
#include "../../Common/interfaces/cdatasource.h"
CSS1::CSS1()
{
}

bool CSS1::init(USHORT deviceID, PlatformResource* pRes)
{
    m_pPlatformRes = pRes;
    DATA_SOURCE_ALLOC(SMCU::MCSSSPNCODEStatusStruct4, m_pMCSSSPNCODEStatu4_9, "模式1扩频码设置", deviceID, 0)
    DATA_SOURCE_ALLOC(SMCU::MCSSSTMSimulStatusStruct4, m_pMCSSSTMSimulStatu4_8, "扩频遥测模拟源参数设置", deviceID, 0)
    DATA_SOURCE_ALLOC(SMCU::MCSSSTMStatusStruct4, m_pMCSSSTMStatusStruct4_6, "扩频遥测解调参数设置", deviceID, 0)
    DATA_SOURCE_ALLOC(SMCU::MCSSSTCStatusStruct4, m_pMCSSSTCStatusStruct4_5, "扩频遥控参数设置", deviceID, 0)
    DATA_SOURCE_ALLOC(SMCU::MCSSSCjStatusStruct4, m_pMCSSSCjStatu4_4, "模式1测距测速参数设置", deviceID, 0)
    DATA_SOURCE_ALLOC(SMCU::MCSSSUTargetTCSSUpModulStatusStruct, m_pMCSSSUTargetTCSSUpModulStatu3, "模式1调制参数设置", deviceID, 0)
    DATA_SOURCE_ALLOC(SMCU::MCSSSRevStatusStruct4, m_pMCSSSRev4Statu4_2, "模式1接收参数设置", deviceID, 0)
    DATA_SOURCE_ALLOC(SMCU::MCSSysTarget4StatusStruct, m_pMCSSysTarget4Status1, "扩频1系统工作参数设置", deviceID, 0)
    DATA_SOURCE_ALLOC_PRO_CTRL(PCRSS1, m_pcrSS, "过程控制命令", deviceID, 4)
    return true;
}

void CSS1::set(SSS1& ss1)
{
    memcpy(m_pMCSSSCjStatu4_4, &ss1.m_MCSSSCjStatu4_4,sizeof(ss1.m_MCSSSCjStatu4_4));
    memcpy(m_pMCSSSPNCODEStatu4_9, &ss1.m_MCSSSPNCODEStatu4_9,sizeof(ss1.m_MCSSSPNCODEStatu4_9));
    memcpy(m_pMCSSSRev4Statu4_2, &ss1.m_MCSSSRev4Statu4_2,sizeof(ss1.m_MCSSSRev4Statu4_2));
    memcpy(m_pMCSSSTCStatusStruct4_5, &ss1.m_MCSSSTCStatusStruct4_5,sizeof(ss1.m_MCSSSTCStatusStruct4_5));
    memcpy(m_pMCSSSTMSimulStatu4_8, &ss1.m_MCSSSTMSimulStatu4_8,sizeof(ss1.m_MCSSSTMSimulStatu4_8));
    memcpy(m_pMCSSSTMStatusStruct4_6, &ss1.m_MCSSSTMStatusStruct4_6,sizeof(ss1.m_MCSSSTMStatusStruct4_6));
    memcpy(m_pMCSSysTarget4Status1, &ss1.m_MCSSysTarget4Status1,sizeof(ss1.m_MCSSysTarget4Status1));
    memcpy(m_pMCSSSUTargetTCSSUpModulStatu3, &ss1.m_pMCSSSUTargetTCSSUpModulStatu3,sizeof(ss1.m_pMCSSSUTargetTCSSUpModulStatu3));
    memcpy(m_pcrSS, &ss1.m_pcrSS, sizeof(ss1.m_pcrSS));
}
void CSS1::processBaseBound_0xF100(char *pData)
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

void CSS1::processBaseBound_0xF104(char *pData, const int dataLen)
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

void CSS1::processBaseBound_0xF101(char * pData)
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
        memcpy(&m_pcrSS->tcIssue1, pData, sizeof(ss1TCISSUE));
        break;
    case 3:
        memcpy(&m_pcrSS->tzBound3, pData, sizeof(ss1TZBOUND));
        break;
    case 4:
        memcpy(&m_pcrSS->eZero4, pData, sizeof(ss1EQUIPMENTZERO));
        break;
    case 8:
        memcpy(&m_pcrSS->ntIssue8, pData, sizeof(ss1NTISSUE));
        break;
    case 13:
        memcpy(&m_pcrSS->rctTst13, pData, sizeof(SSTTCRCTTST));
        break;
    case 16:
        memcpy(&m_pcrSS->clBandwidth16, pData, sizeof(SSTTCTCLBANDWITH));
        break;
    case 20:
        memcpy(&m_pcrSS->ssTone20, pData, sizeof(SSTDRTST));
        break;
    case 21:
        memcpy(&m_pcrSS->drtTst21, pData, sizeof(ss1RTEFSTAT));
        break;
    case 22:
        memcpy(&m_pcrSS->ttcTst22, pData, sizeof(ss1BERTST));
        break;
    case 23:
        memcpy(&m_pcrSS->strcTst23, pData, sizeof(ss1FERTST));
        break;
    case 24:
        memcpy(&m_pcrSS->rcsreiTst24, pData, sizeof(ss1RCSREITST));
        break;
    case 25:
        memcpy(&m_pcrSS->dmreTst25, pData, sizeof(ss1DMRETST));
        break;
    case 26:
        memcpy(&m_pcrSS->smreTst26, pData, sizeof(ss1SMRETST));
        break;
    case 27:
        memcpy(&m_pcrSS->dSimu27, pData, sizeof(ss1DOPPLERSIMU));
        break;
    case 28:
        memcpy(&m_pcrSS->bbdtsSel28, pData, sizeof(ss1BBDTSSEL));
        break;
    case 30:
        memcpy(&m_pcrSS->dmdrrPort30, pData, sizeof(ss1DMDRRPORT));
        break;
    case 31:
        memcpy(&m_pcrSS->smdrePort31, pData, sizeof(ss1SMDREPORT));
        break;
    case 32:
        memcpy(&m_pcrSS->ttcdSave32, pData, sizeof(ss1TTCDSAVE));
        break;
    case 33:
        memcpy(&m_pcrSS->bzcrInquire33, pData, sizeof(ss1BZCRINQUIRE));
        break;
    case 36:
        memcpy(&m_pcrSS->r0Decuction36, pData, sizeof(ss1R0DEDUCTION));
        break;
    case 39:
        memcpy(&m_pcrSS->dpfpcRange39, pData, sizeof(ss1DPFPCRANGE));
        break;
    case 40:
        memcpy(&m_pcrSS->AGCcBind40, pData, sizeof(ss1AGCCBIND));
        break;
    case 41:
        memcpy(&m_pcrSS->dcCtrl41, pData, sizeof(DCCTRL));
        break;
    default:
        CLog::addLog("单元序号错误.\n");
    }
}

void CSS1::getMap(BYTE* pDataField, const int dataLen, QMap<int, int>& mapFB_FE)
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

void CSS1::processBaseBand(QByteArray &datagram)
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

void CSS1::processBaseBoundData(BYTE *pBuff, const BYTE &unitNum)
{
    switch(unitNum)
    {
    case 1:
        SMCU::MCSSysTarget4StatusStruct tmp;
        memcpy(&tmp, pBuff, sizeof(SMCU::MCSSysTarget4StatusStruct));
        tmp.sysStatu.ucSystemMode = 7;
        memcpy(m_pMCSSysTarget4Status1, &tmp, sizeof(SMCU::MCSSysTarget4StatusStruct));
        break;
    case 2://模式1接收参数设置
        memcpy(m_pMCSSSRev4Statu4_2, pBuff, sizeof(SMCU::MCSSSRevStatusStruct4));
        break;
    case 3://模式1调制参数设置
        memcpy(m_pMCSSSUTargetTCSSUpModulStatu3, pBuff, sizeof(SMCU::MCSSSUTargetTCSSUpModulStatusStruct));
        break;
    case 5://扩频遥控参数设置
        memcpy(m_pMCSSSTCStatusStruct4_5, pBuff, sizeof(SMCU::MCSSSTCStatusStruct4));
        break;
    case 4://模式1测距测速参数设置
        memcpy(m_pMCSSSCjStatu4_4, pBuff, sizeof(SMCU::MCSSSCjStatusStruct4));
        break;
    case 6://扩频遥测解调参数设置
        memcpy(m_pMCSSSTMStatusStruct4_6, pBuff, sizeof(SMCU::MCSSSTMStatusStruct4));
        break;
    case 7://
        //memcp);
        break;
    case 8://扩频遥测模拟源参数设置
        memcpy(m_pMCSSSTMSimulStatu4_8, pBuff, sizeof(SMCU::MCSSSTMSimulStatusStruct4));
        break;
    case 9://模式1扩频码设置
        memcpy(m_pMCSSSPNCODEStatu4_9, pBuff, sizeof(SMCU::MCSSSPNCODEStatusStruct4));
        break;
    default:
        break;
    }
}
void CSS1:: get(SSS1 &ss1){
    memcpy( &ss1.m_MCSSSCjStatu4_4,m_pMCSSSCjStatu4_4,sizeof(ss1.m_MCSSSCjStatu4_4));
    memcpy(&ss1.m_MCSSSPNCODEStatu4_9 ,m_pMCSSSPNCODEStatu4_9 ,sizeof(ss1.m_MCSSSPNCODEStatu4_9));
    memcpy(&ss1.m_MCSSSRev4Statu4_2 ,m_pMCSSSRev4Statu4_2 ,sizeof(ss1.m_MCSSSRev4Statu4_2));
    memcpy(&ss1.m_MCSSSTCStatusStruct4_5,  m_pMCSSSTCStatusStruct4_5,sizeof(ss1.m_MCSSSTCStatusStruct4_5));
    memcpy(&ss1.m_MCSSSTMSimulStatu4_8 , m_pMCSSSTMSimulStatu4_8,sizeof(ss1.m_MCSSSTMSimulStatu4_8));
    memcpy(&ss1.m_MCSSSTMStatusStruct4_6 ,m_pMCSSSTMStatusStruct4_6 ,sizeof(ss1.m_MCSSSTMStatusStruct4_6));
    memcpy(&ss1.m_MCSSysTarget4Status1 ,m_pMCSSysTarget4Status1,sizeof(ss1.m_MCSSysTarget4Status1));
    memcpy(&ss1.m_pMCSSSUTargetTCSSUpModulStatu3 ,m_pMCSSSUTargetTCSSUpModulStatu3 ,sizeof(ss1.m_pMCSSSUTargetTCSSUpModulStatu3));
    memcpy(&ss1.m_pcrSS ,m_pcrSS , sizeof(ss1.m_pcrSS));

}
