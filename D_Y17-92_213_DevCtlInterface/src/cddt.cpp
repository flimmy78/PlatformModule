#include "cddt.h"
#include "../../Common/Log.h"
#include "DataStructTTCDevCtlInterface.h"
#include "Global.h"

CDDT::CDDT()
{
}


bool CDDT::init(USHORT deviceID, PlatformResource* pRes)
{
    m_devID = deviceID;
    m_pPlatformRes = pRes;
    DATA_SOURCE_ALLOC(MCSSysDataStatusStruct, m_pMCSSysStatus, "DDT_BBE系统参数设置", deviceID, 0);
    DATA_SOURCE_ALLOC(MCSDivComStatusStruct, m_pMCSDivComStatu, "分集合成参数设置", deviceID, 0);
    DATA_SOURCE_ALLOC(MCSDDTRevStatusStruct, m_pMCSDDTRevStatus, "数传接收机参数设置", deviceID, 0)
    DATA_SOURCE_ALLOC(MCSDDTTMIStatusStruct, m_pMCSDDTTMQStatusI, "数传解调参数设置", deviceID, 0)
    DATA_SOURCE_ALLOC(MCSDDTTMQStatusStruct, m_pMCSDDTTMQStatusQ, "数传解调参数设置", deviceID, 1)
    DATA_SOURCE_ALLOC(MCSDDTTMSimulStatusStruct, m_pMCSDDTTMSimulStatus, "数传信号源参数设置", deviceID, 0)
    DATA_SOURCE_ALLOC(MCSDDTReplayStatusStruct, m_pMCSDDTReplayStatus, "数传存储转发单元数据元素", deviceID, 0)
    DATA_SOURCE_ALLOC_PRO_CTRL(PCRDATATRANS_TTC, m_ddtCtlRsltRpt, "过程控制命令", deviceID, 0)
    return true;
}

void CDDT::processBaseBound_0xF100(char *pData)
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

void CDDT::processBaseBound_0xF104(char *pData, const int dataLen)
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

void CDDT::processBaseBound_0xF101(char * pData)
{

    //过程命令标识，在上报时，对低字节进行取反，高字节不变。
    //逆操作上述过程得到原来的过程命令标识.
    USHORT id = 0;
    memcpy(&id, pData, sizeof(USHORT));
    UCHAR ucCode = id;
    ucCode = 0xFF-ucCode;
    pData += 2;//过程命令标识与序列号共4个字节,各两个字节.
    switch(ucCode)
    {
    case 1://任务代号下发
        memcpy(&m_ddtCtlRsltRpt->tcIssue1, pData, sizeof(TCISSUE));
        break;
    case 2://跟踪通道选择
        m_ddtCtlRsltRpt->tcSel2.sel = (UCHAR)pData[0];
        break;
    case 3://应答机零值装订
        memcpy(&m_ddtCtlRsltRpt->tzBound3, pData, sizeof(TZBOUND));
        break;
    case 4:
        memcpy(&m_ddtCtlRsltRpt->eZero4, pData, sizeof(EQUIPMENTZERO));
        m_ddtCtlRsltRpt->eZero4.valid = 3;
        break;
    case 8:
        memcpy(&m_ddtCtlRsltRpt->ntIssue8, pData, sizeof(NTISSUE));
        break;
    case 17:
        memcpy(&m_ddtCtlRsltRpt->dmStone17, pData, sizeof(DMSTONE));
        break;
    case 18:
        memcpy(&m_ddtCtlRsltRpt->tcsTone18, pData, sizeof(TCSTONE));
        break;
    case 20:
        memcpy(&m_ddtCtlRsltRpt->tsssTone20, pData, sizeof(TSSSTONE));
        break;
    case 21:
        memcpy(&m_ddtCtlRsltRpt->drtTst21, pData, sizeof(DRTTST));
        break;
    case 22:
        memcpy(&m_ddtCtlRsltRpt->ttcTst22, pData, sizeof(TTCTST));
        break;
    case 23:
        memcpy(&m_ddtCtlRsltRpt->strcTst23, pData, sizeof(STRCTST));
        break;
    case 31:
        memcpy(&m_ddtCtlRsltRpt->stdrTst31, pData, sizeof(STDRTST));
        break;
    case 32:
        memcpy(&m_ddtCtlRsltRpt->dtrctTst32, pData, sizeof(DTRCTTST));
        break;
    case 37://数传频率动态范围测试
        memcpy(&m_ddtCtlRsltRpt->dfdrTst37, pData, sizeof(DFDRTST));
        break;
    case 38://实时误码帧统计
        memcpy(&m_ddtCtlRsltRpt->rtefStat38, pData, sizeof(RTEFSTAT));
        break;
    case 39://误码率测试
        memcpy(&m_ddtCtlRsltRpt->berTst39, pData, sizeof(BERTST));
        break;
    case 40://误帧率测试
        memcpy(&m_ddtCtlRsltRpt->ferTst40, pData, sizeof(FERTST));
        break;
    case 41://遥控小环误指令测试
        memcpy(&m_ddtCtlRsltRpt->rcsreiTst41, pData, sizeof(RCSREITST));
        break;
    case 43:
        memcpy(&m_ddtCtlRsltRpt->dmreTst43, pData, sizeof(DMRETST));
        break;
    case 44:
        memcpy(&m_ddtCtlRsltRpt->smreTst44, pData, sizeof(SMRETST));
        break;
    case 49:
        memcpy(&m_ddtCtlRsltRpt->dSimu49, pData, sizeof(DOPPLERSIMU));
        break;
    case 50:
        memcpy(&m_ddtCtlRsltRpt->bbdtsSel50, pData, sizeof(BBDTSSEL));
        break;
    case 53:
        memcpy(&m_ddtCtlRsltRpt->dmdrrPort53, pData, sizeof(DMDRRPORT));
        break;
    case 54:
        memcpy(&m_ddtCtlRsltRpt->smdrePort54, pData, sizeof(SMDREPORT));
        break;
    case 55:
        memcpy(&m_ddtCtlRsltRpt->ttcdSave55, pData, sizeof(TTCDSAVE));
        break;
    case 56:
        memcpy(&m_ddtCtlRsltRpt->ddtdSave56, pData, sizeof(DDTDSAVE));
        break;
    case 57:
        memcpy(&m_ddtCtlRsltRpt->bzcrInquire57, pData, sizeof(BZCRINQUIRE));
        m_ddtCtlRsltRpt->bzcrInquire57.valid = 1;
        break;
    case 60:
        memcpy(&m_ddtCtlRsltRpt->r0Decuction60, pData, sizeof(R0DEDUCTION));
        break;
    case 61:
        memcpy(&m_ddtCtlRsltRpt->dfvBind61, pData, sizeof(DFVBINDING));
        break;
    case 63:
        memcpy(&m_ddtCtlRsltRpt->dcIDIssue63, pData, sizeof(DCIDISSUE));
        break;
    case 65:
        memcpy(&m_ddtCtlRsltRpt->tcpPeriod65, pData, sizeof(TCPPERIOD));
        break;
    case 66:
        memcpy(&m_ddtCtlRsltRpt->dpfpcRange66, pData, sizeof(DPFPCRANGE));
        break;
    case 67:
        memcpy(&m_ddtCtlRsltRpt->AGCcBind67, pData, sizeof(AGCCBIND));
        break;
    default:
        CLog::addLog("单元序号错误.\n");
    }
}

void CDDT::getMap(BYTE* pDataField, const int dataLen, QMap<int, int>& mapFB_FE)
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

void CDDT::updateAbstract(DSZ_BBE_GlobalStatusBlk* pBbeAbstract)
{
    if(!pBbeAbstract)return;
    //
    pBbeAbstract->ucMonitorMode = m_pMCSSysStatus->ucMonitorMode;
    pBbeAbstract->ucBBEWorkID = m_devID == 0x400 ? 1 : 2;
    pBbeAbstract->ucBBEID = m_devID == 0x400 ? 1 : 2;//fix me
    pBbeAbstract->ucBBEWorkStatus = m_pMCSSysStatus->ucBBEWorkStatus;
    pBbeAbstract->ucSystemWorkMode = m_pMCSSysStatus->ucSystemMode;//按注释来的
    pBbeAbstract->ucSystemMode = m_pMCSSysStatus->ucSystemWorkMode;//按注释来的
    //pBbeAbstract->ucDataTransStatus =
    pBbeAbstract->uiMID = m_pMCSSysStatus->tParams.uiM;
    //pBbeAbstract->nBID = m_pMCSSysStatus->
    //pBbeAbstract->ucSatEncryptionFlag
    //pBbeAbstract->ucTaskID
    //pBbeAbstract->nSubCarFreq =
    pBbeAbstract->nCodeRate = m_pMCSDDTTMQStatusI->tParams.usCodeRate;//模拟元也有
    //pBbeAbstract->ucWordLen = m_pMCSDDTTMQStatusI->tParams.
    pBbeAbstract->usFrmLen = m_pMCSDDTTMQStatusI->tParams.usFrameLen;
    pBbeAbstract->dwlSynCode = m_pMCSDDTTMQStatusI->tParams.uiFrameSynG;
    pBbeAbstract->ucFspLen = m_pMCSDDTTMQStatusI->tParams.ucFrameSynGLen;
    pBbeAbstract->ucSubFrmType = m_pMCSDDTTMQStatusI->tParams.ucSFrameSynMd - 1;
    pBbeAbstract->usSubFrmLen = m_pMCSDDTTMQStatusI->tParams.ucSframeLen;
    //pBbeAbstract->ucCodeLock = m_pMCSDDTTMQStatusI->tParams.ucSframeLen;
    //pBbeAbstract->ucFrmLock = m_pMCSDDTTMQStatusI->tParams.ucSFrameSynMd;
    //pBbeAbstract->ucSubFrmLock = m_pMCSDDTTMQStatusI->tParams.ucSFrameSynMd;
    memcpy(pBbeAbstract->szTaskName, m_pMCSSysStatus->tParams.ucTaskDescribe, 6);
    //pBbeAbstract->byStaID = m_pMCSDDTTMQStatusI->tParams.ucSFrameSynMd;
    pBbeAbstract->i64DeviceAssembleID = m_pMCSSysStatus->tParams.dwlComboID;
    //pBbeAbstract->byRcvType
    pBbeAbstract->ucCombType = m_pMCSDivComStatu->tParams.ucCombType;
    //pBbeAbstract->ucRcvBand
    //pBbeAbstract->usAveBw
    //pBbeAbstract->usAEBw
    //pBbeAbstract->usErrorChaPhase
    pBbeAbstract->i64DownRF = m_pMCSSysStatus->tParams.dwlAnswerRevDownFreq;
    pBbeAbstract->dwSystemWorkMode = m_pMCSSysStatus->ucSystemMode;

    pBbeAbstract->DestMCSSysStatus.ucMonitorMode = m_pMCSSysStatus->ucMonitorMode;;
    pBbeAbstract->DestMCSSysStatus.dwSystemMode = m_pMCSSysStatus->ucSystemMode;
    pBbeAbstract->DestMCSSysStatus.ucSystemWorkMode = m_pMCSSysStatus->ucSystemWorkMode;

    pBbeAbstract->DestMCSSysStatus.ucTimeCodeStatus = m_pMCSSysStatus->ucTimeCodeStatus;
    //pBbeAbstract->DestMCSSysStatus.ucBBETimeMode = ->;
    pBbeAbstract->DestMCSSysStatus.ucDTETranStatus = m_pMCSSysStatus->ucBBEWorkStatus;

    //pBbeAbstract->DestMCSSysStatus.FMTMBEEBoardWorkState;
    //pBbeAbstract->DestMCSSysStatus.SupplyElectricity;
    //pBbeAbstract->DestMCSSysStatus.HardWareTemperature;

    //pBbeAbstract->DestMCSSysStatus.Power_L;
    //pBbeAbstract->DestMCSSysStatus.Power_R;
    //pBbeAbstract->DestMCSSysStatus.TMDataTranStatus;
    //m_pMCSDDTReplayStatus UCHAR  ucSaveStatus;    //存储状态 0：无效，1：正在存盘，2：未存盘
    //pBbeAbstract UCHAR TMDataSaveStatus; //遥测数据存盘状态 0：无此参数，1：正在存盘，2：未存盘
    pBbeAbstract->DestMCSSysStatus.TMDataSaveStatus = m_pMCSDDTReplayStatus->ucSaveStatus;
    //pBbeAbstract->DestMCSSysStatus.ucMemStoreStatus;
    //pBbeAbstract->DestMCSSysStatus.ucOuterTimeWorkStatus;

    //pBbeAbstract->DestMCSSysStatus.ucChannelPowerFull;
    //pBbeAbstract->DestMCSSysStatus.ucBaseSignal;
    //pBbeAbstract->DestMCSSysStatus.ucCarrierDoppl;

    //pBbeAbstract->DestMCSSysStatus.ucCodeDoppl;
    //pBbeAbstract->DestMCSSysStatus.ucFMTMOnline;

    memcpy(pBbeAbstract->DestMCSSysStatus.tParams.ucTaskDescribe, m_pMCSSysStatus->tParams.ucTaskDescribe, 6);
    pBbeAbstract->DestMCSSysStatus.tParams.uiM = m_pMCSSysStatus->tParams.uiM;
   // pBbeAbstract->DestMCSSysStatus.tParams.ucSendMode = m_pMCSSysStatus->tParams.ucSendMode;
    pBbeAbstract->DestMCSSysStatus.tParams.dwlAnswerRevDownFreq = m_pMCSSysStatus->tParams.dwlAnswerRevDownFreq;

    //pBbeAbstract->DestMCSSysStatus.tParams.uiInfoTransferFormat =  ;//fix me
    //pBbeAbstract->DestMCSSysStatus.tParams.ucDDTOutFormat = m_pMCSSysStatus->tParams.ucDDTOutFormat;
    //pBbeAbstract->DestMCSSysStatus.tParams.ucDDTSendSwitchI = m_pMCSSysStatus->ucDDTSendSwitchI;
    //pBbeAbstract->DestMCSSysStatus.tParams.ucDDTSendSwitchQ = m_pMCSSysStatus->ucDDTSendSwitchQ;
    //UCHAR  ucLockStatus;    //载波锁定指示 0：无效，1：锁定，2：失锁。
    if(m_pMCSDDTRevStatus->ucLockStatus == 1)
        pBbeAbstract->ucCarrLk = 1;
    else pBbeAbstract->ucCarrLk = 0;
    if(m_pMCSDivComStatu->usDiffPllLock == 1)
        pBbeAbstract->ucDivCarrLk = 1;
    else pBbeAbstract->ucDivCarrLk = 0;
    pBbeAbstract->codeSyn = m_pMCSDDTTMQStatusI->ucCodeLock;
    if(m_pMCSDDTTMQStatusI->tParams.ucBitOutEn ==1){//是位流
        pBbeAbstract->frmSyn = 3;
        pBbeAbstract->subfrmSyn = 3;
    }else
    {
        pBbeAbstract->frmSyn = m_pMCSDDTTMQStatusI->ucFrmLock;
        pBbeAbstract->subfrmSyn = m_pMCSDDTTMQStatusI->ucSubFrmLock;
    }
    int tmp = (SHORT)m_pMCSDDTTMQStatusI->usEbN0;
    pBbeAbstract->fSnrC = tmp/10.0; // 合成信噪比

    pBbeAbstract->codeSynQ = m_pMCSDDTTMQStatusQ->ucCodeLock;

    if(m_pMCSDDTTMQStatusQ->tParams.ucBitOutEn ==1){//是位流
        pBbeAbstract->frmSynQ = 3;
        pBbeAbstract->subfrmSynQ = 3;
    }else
    {
        pBbeAbstract->frmSynQ = m_pMCSDDTTMQStatusQ->ucFrmLock;
        pBbeAbstract->subfrmSynQ = m_pMCSDDTTMQStatusQ->ucSubFrmLock;
    }
    tmp = (SHORT)m_pMCSDDTTMQStatusQ->usEbN0;
    pBbeAbstract->fSnrCQ = tmp/10.0; // 合成信噪比
    pBbeAbstract->ucSubFrmTypeQ = m_pMCSDDTTMQStatusQ->tParams.ucSFrameSynMd - 1;			//副帧方式，8位无符号整型数。D7 ~ D0位：00H表示无副帧；01H表示ID副帧；02H表示循环副帧；03H表示反码副帧。缺省值为无副帧。

}

void CDDT::processBaseBand(QByteArray &datagram)
{
    NETSDFRM netSDFrm;
    int nNetSDFRMLen = sizeof(NETSDFRM);
    memcpy(&netSDFrm, datagram.data(), nNetSDFRMLen);
    switch(netSDFrm.BID)
    {
    case 0xF100:
        g_showDatagram.m_datagramCommRev = datagram;
        //控制命令响应
        processBaseBound_0xF100(datagram.data() + nNetSDFRMLen);
        break;
    case 0xF104:
        g_showDatagram.m_datagramStatuRev = datagram;
        //分机状态信息上报
        processBaseBound_0xF104(datagram.data()  + nNetSDFRMLen, netSDFrm.DataLen);
        break;
    case 0xF101:
        //控制结果上报
        g_showDatagram.m_datagramCommRev = datagram;
        processBaseBound_0xF101(datagram.data()  + nNetSDFRMLen);
        break;
    default:
        break;
    }
}

void CDDT::processBaseBoundData(BYTE *pBuff, const BYTE &unitNum)
{
    switch(unitNum)
    {
    case 1:     //DDT_BBE系统参数设置
        MCSSysDataStatusStruct tmp;
        memcpy(&tmp, pBuff, sizeof(MCSSysDataStatusStruct));
        tmp.ucSystemMode = 5;
        memcpy(m_pMCSSysStatus, &tmp, sizeof(MCSSysDataStatusStruct));
        break;
    case 8:     //数传检测输出参数设置
        //memcpy(m_pSCBBETestOutStatu, pBuff, sizeof(SCBBETestOutStatusStruct));
        break;
    case 15:    //数传接收机单元数据元素
        memcpy(m_pMCSDDTRevStatus, pBuff, sizeof(MCSDDTRevStatusStruct));
        break;
    case 16:    //数传解调器单元(I路)数据元素
        memcpy(m_pMCSDDTTMQStatusI, pBuff, sizeof(MCSDDTTMQStatusStruct));
        break;
    case 17:    //数传解调器单元(Q路)数据元素
        memcpy(m_pMCSDDTTMQStatusQ, pBuff, sizeof(MCSDDTTMQStatusStruct));
        break;
    case 18:    //数传模拟源单元数据元素
        memcpy(m_pMCSDDTTMSimulStatus, pBuff, sizeof(MCSDDTTMSimulStatusStruct));
        break;
    case 19:    //数传存储转发单元数据元素
        memcpy(m_pMCSDDTReplayStatus, pBuff, sizeof(MCSDDTReplayStatusStruct));
        break;
    case 20:    //分集合成
        memcpy(m_pMCSDivComStatu, pBuff, sizeof(MCSDivComStatusStruct));
        break;
    }
}
