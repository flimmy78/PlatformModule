#include "CTTC.h"
#include "../../Common/Log.h"
#include "DataStructTTCRemoteInterface.h"
#include "Global.h"
CTTC::CTTC()
{
}

bool CTTC::init(USHORT deviceID, PlatformResource* pRes)
{
    m_pPlatformRes = pRes;
    DATA_SOURCE_ALLOC(MCSSysDataStatusStruct, m_pMCSSysDataStatusTTC1, "USBBBE系统参数设置", deviceID, 0)
    DATA_SOURCE_ALLOC(MCSUSBTCStatusStruct, m_pMCSUSBTCStatus7, "USBBBE遥控参数设置", deviceID, 0)
    DATA_SOURCE_ALLOC(MCSUSBUpModulStatusStruct, m_pMCSUSBUpModulStatus4, "USBBBE调制参数设置", deviceID, 0)
    DATA_SOURCE_ALLOC(MCSUSBCjStatusStruct, m_pMCSUSBCjStatus6, "USBBBE测距参数设置", deviceID, 1)
    DATA_SOURCE_ALLOC(MCSUSBCjStatusStruct, m_pMCSUSBCjStatus5, "USBBBE测距参数设置", deviceID, 0)
    DATA_SOURCE_ALLOC(MCSUSBTMSimulStatusStruct, m_pMCSUSBTMSimulStatus12, "USBBBE模拟源参数设置", deviceID, 0)
    DATA_SOURCE_ALLOC(MCSUSBTMStatusStruct, m_pMCSUSBTMStatus8, "USBBBE遥测参数设置", deviceID, 0)
    DATA_SOURCE_ALLOC(MCSUSBRevStatusStruct, m_pMCSUSBRevStatu3, "USBBBE中频接收机参数设置", deviceID, 1)
    DATA_SOURCE_ALLOC(MCSUSBRevStatusStruct, m_pMCSUSBRevStatu2, "USBBBE中频接收机参数设置", deviceID, 0)
    DATA_SOURCE_ALLOC_PRO_CTRL(PCRDATATRANS_TTC, m_usbCtlRsltRpt, "过程控制命令", deviceID, 2)
    return true;
}

void CTTC::set(STTC &ttc)
{
    memcpy(m_pMCSSysDataStatusTTC1, &ttc.m_MCSSysDataStatusTTC1,sizeof(ttc.m_MCSSysDataStatusTTC1));
    memcpy(m_pMCSUSBCjStatus5, &ttc.m_MCSUSBCjStatus5,sizeof(ttc.m_MCSUSBCjStatus5));
    memcpy(m_pMCSUSBCjStatus6, &ttc.m_MCSUSBCjStatus6,sizeof(ttc.m_MCSUSBCjStatus6));
    memcpy(m_pMCSUSBRevStatu2, &ttc.m_MCSUSBRevStatu2,sizeof(ttc.m_MCSUSBRevStatu2));
    memcpy(m_pMCSUSBRevStatu3, &ttc.m_MCSUSBRevStatu3,sizeof(ttc.m_MCSUSBRevStatu3));
    memcpy(m_pMCSUSBTCStatus7, &ttc.m_MCSUSBTCStatus7,sizeof(ttc.m_MCSUSBTCStatus7));
    memcpy(m_pMCSUSBTMSimulStatus12, &ttc.m_MCSUSBTMSimulStatus12,sizeof(ttc.m_MCSUSBTMSimulStatus12));
    memcpy(m_pMCSUSBTMStatus8, &ttc.m_MCSUSBTMStatus8,sizeof(ttc.m_MCSUSBTMStatus8));
    memcpy(m_pMCSUSBUpModulStatus4, &ttc.m_MCSUSBUpModulStatus4,sizeof(ttc.m_MCSUSBUpModulStatus4));
    memcpy(m_usbCtlRsltRpt, &ttc.m_ctlRsltRpt, sizeof(ttc.m_ctlRsltRpt));
}

void CTTC::get(STTC &ttc)
{
    memcpy(&ttc.m_MCSSysDataStatusTTC1, m_pMCSSysDataStatusTTC1,sizeof(ttc.m_MCSSysDataStatusTTC1));
    memcpy(&ttc.m_MCSUSBCjStatus5, m_pMCSUSBCjStatus5,sizeof(ttc.m_MCSUSBCjStatus5));
    memcpy(&ttc.m_MCSUSBCjStatus6, m_pMCSUSBCjStatus6,sizeof(ttc.m_MCSUSBCjStatus6));
    memcpy(&ttc.m_MCSUSBRevStatu2, m_pMCSUSBRevStatu2,sizeof(ttc.m_MCSUSBRevStatu2));
    memcpy(&ttc.m_MCSUSBRevStatu3, m_pMCSUSBRevStatu3,sizeof(ttc.m_MCSUSBRevStatu3));
    memcpy(&ttc.m_MCSUSBTCStatus7, m_pMCSUSBTCStatus7,sizeof(ttc.m_MCSUSBTCStatus7));
    memcpy(&ttc.m_MCSUSBTMSimulStatus12, m_pMCSUSBTMSimulStatus12,sizeof(ttc.m_MCSUSBTMSimulStatus12));
    memcpy(&ttc.m_MCSUSBTMStatus8, m_pMCSUSBTMStatus8,sizeof(ttc.m_MCSUSBTMStatus8));
    memcpy(&ttc.m_MCSUSBUpModulStatus4, m_pMCSUSBUpModulStatus4,sizeof(ttc.m_MCSUSBUpModulStatus4));
    memcpy(&ttc.m_ctlRsltRpt, m_usbCtlRsltRpt, sizeof(ttc.m_ctlRsltRpt));
}

void CTTC::processBaseBound_0xF100(char *pData)
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

void CTTC::processBaseBound_0xF104(char *pData, const int dataLen)
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

void CTTC::processBaseBound_0xF101(char * pData)
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
    case 1://任务代号下发
        memcpy(&m_usbCtlRsltRpt->tcIssue1, pData, sizeof(TCISSUE));
        break;
    case 2://跟踪通道选择
        m_usbCtlRsltRpt->tcSel2.sel = (UCHAR)pData[0];
        break;
    case 3://应答机零值装订
        memcpy(&m_usbCtlRsltRpt->tzBound3, pData, sizeof(TZBOUND));
        break;
    case 4:
        memcpy(&m_usbCtlRsltRpt->eZero4, pData, sizeof(EQUIPMENTZERO));
        break;
    case 8:
        memcpy(&m_usbCtlRsltRpt->ntIssue8, pData, sizeof(NTISSUE));
        break;
    case 17:
        memcpy(&m_usbCtlRsltRpt->dmStone17, pData, sizeof(DMSTONE));
        break;
    case 18:
        memcpy(&m_usbCtlRsltRpt->tcsTone18, pData, sizeof(TCSTONE));
        break;
    case 20:
        memcpy(&m_usbCtlRsltRpt->tsssTone20, pData, sizeof(TSSSTONE));
        break;
    case 21:
        memcpy(&m_usbCtlRsltRpt->drtTst21, pData, sizeof(DRTTST));
        break;
    case 22:
        memcpy(&m_usbCtlRsltRpt->ttcTst22, pData, sizeof(TTCTST));
        break;
    case 23:
        memcpy(&m_usbCtlRsltRpt->strcTst23, pData, sizeof(STRCTST));
        break;
    case 31:
        memcpy(&m_usbCtlRsltRpt->stdrTst31, pData, sizeof(STDRTST));
        break;
    case 32:
        memcpy(&m_usbCtlRsltRpt->dtrctTst32, pData, sizeof(DTRCTTST));
        break;
    case 37://数传频率动态范围测试
        memcpy(&m_usbCtlRsltRpt->dfdrTst37, pData, sizeof(DFDRTST));
        break;
    case 38://实时误码帧统计
        memcpy(&m_usbCtlRsltRpt->rtefStat38, pData, sizeof(RTEFSTAT));
        break;
    case 39://误码率测试
        memcpy(&m_usbCtlRsltRpt->berTst39, pData, sizeof(BERTST));
        break;
    case 40://误帧率测试
        memcpy(&m_usbCtlRsltRpt->ferTst40, pData, sizeof(FERTST));
        break;
    case 41://遥控小环误指令测试
        memcpy(&m_usbCtlRsltRpt->rcsreiTst41, pData, sizeof(RCSREITST));
        break;
    case 43:
        memcpy(&m_usbCtlRsltRpt->dmreTst43, pData, sizeof(DMRETST));
        break;
    case 44:
        memcpy(&m_usbCtlRsltRpt->smreTst44, pData, sizeof(SMRETST));
        break;
    case 49:
        memcpy(&m_usbCtlRsltRpt->dSimu49, pData, sizeof(DOPPLERSIMU));
        break;
    case 50:
        memcpy(&m_usbCtlRsltRpt->bbdtsSel50, pData, sizeof(BBDTSSEL));
        break;
    case 53:
        memcpy(&m_usbCtlRsltRpt->dmdrrPort53, pData, sizeof(DMDRRPORT));
        break;
    case 54:
        memcpy(&m_usbCtlRsltRpt->smdrePort54, pData, sizeof(SMDREPORT));
        break;
    case 55:
        memcpy(&m_usbCtlRsltRpt->ttcdSave55, pData, sizeof(TTCDSAVE));
        break;
    case 56:
        memcpy(&m_usbCtlRsltRpt->ddtdSave56, pData, sizeof(DDTDSAVE));
        break;
    case 57:
        memcpy(&m_usbCtlRsltRpt->bzcrInquire57, pData, sizeof(BZCRINQUIRE));
        break;
    case 60:
        memcpy(&m_usbCtlRsltRpt->r0Decuction60, pData, sizeof(R0DEDUCTION));
        break;
    case 61:
        memcpy(&m_usbCtlRsltRpt->dfvBind61, pData, sizeof(DFVBINDING));
        break;
    case 63:
        memcpy(&m_usbCtlRsltRpt->dcIDIssue63, pData, sizeof(DCIDISSUE));
        break;
    case 65:
        memcpy(&m_usbCtlRsltRpt->tcpPeriod65, pData, sizeof(TCPPERIOD));
        break;
    case 66:
        memcpy(&m_usbCtlRsltRpt->dpfpcRange66, pData, sizeof(DPFPCRANGE));
        break;
    case 67:
        memcpy(&m_usbCtlRsltRpt->AGCcBind67, pData, sizeof(AGCCBIND));
        break;
    default:
        CLog::addLog("单元序号错误.\n");
    }
}

void CTTC::getMap(BYTE* pDataField, const int dataLen, QMap<int, int>& mapFB_FE)
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

void CTTC::processBaseBand(QByteArray &datagram)
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

void CTTC::processBaseBoundData(BYTE *pBuff, const BYTE &unitNum)
{
    switch(unitNum)
    {
    case 1://USBBBE系统参数设置
        MCSSysDataStatusStruct tmp;
        memcpy(&tmp, pBuff, sizeof(MCSSysDataStatusStruct));
        tmp.ucSystemMode = 6;
        memcpy(m_pMCSSysDataStatusTTC1, &tmp, sizeof(MCSSysDataStatusStruct));
        break;
    case 2://USBBBE中频接收机参数设置
        memcpy(m_pMCSUSBRevStatu2, pBuff, sizeof(MCSUSBRevStatusStruct));
        break;
    case 3://USBBBE中频接收机参数设置
        memcpy(m_pMCSUSBRevStatu3, pBuff, sizeof(MCSUSBRevStatusStruct));
        break;
    case 4://USBBBE调制参数设置
        memcpy(m_pMCSUSBUpModulStatus4, pBuff, sizeof(MCSUSBUpModulStatusStruct));
        break;
    case 5://USBBBE测距参数设置
        memcpy(m_pMCSUSBCjStatus5, pBuff, sizeof(MCSUSBCjStatusStruct));
        break;
    case 6://USBBBE测距参数设置
        memcpy(m_pMCSUSBCjStatus6, pBuff, sizeof(MCSUSBCjStatusStruct));
        break;
    case 7://USBBBE遥控参数设置
        memcpy(m_pMCSUSBTCStatus7, pBuff, sizeof(MCSUSBTCStatusStruct));//MCSUSBTCStatusStruct
        break;
    case 8://USBBBE遥测参数设置
        memcpy(m_pMCSUSBTMStatus8, pBuff, sizeof(MCSUSBTMStatusStruct));
        break;
    case 12://USBBBE模拟源参数设置
        memcpy(m_pMCSUSBTMSimulStatus12, pBuff, sizeof(MCSUSBTMSimulStatusStruct));
        break;
    default:
        break;
    }
}
