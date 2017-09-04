#include "CTTC.h"
#include "../../Common/Log.h"
#include "DataStructTTCDevCtlInterface.h"
#include "Global.h"
CTTC::CTTC()
{
}


bool CTTC::init(USHORT deviceID, PlatformResource* pRes, SMCU2::MCSSSTCStatus4* pTC)
{
    m_devID = deviceID;
    m_pPlatformRes = pRes;
    m_pTC4_4 = pTC;
    DATA_SOURCE_ALLOC(MCSSysDataStatusStruct, m_pMCSSysDataStatusTTC1, "USBBBE系统参数设置", deviceID, 0)
    DATA_SOURCE_ALLOC(MCSUSBTCStatusStruct, m_pMCSUSBTCStatus7, "USBBBE遥控参数设置", deviceID, 0)
    DATA_SOURCE_ALLOC(MCSUSBUpModulStatusStruct, m_pMCSUSBUpModulStatus4, "USBBBE调制参数设置", deviceID, 0)
    DATA_SOURCE_ALLOC(MCSUSBCjStatusStruct, m_pMCSUSBCjStatus6, "USBBBE测距参数设置", deviceID, 1)
    DATA_SOURCE_ALLOC(MCSUSBCjStatusStruct, m_pMCSUSBCjStatus5, "USBBBE测距参数设置", deviceID, 0)
    DATA_SOURCE_ALLOC(MCSUSBTMSimulStatusStruct, m_pMCSUSBTMSimulStatus12, "USBBBE模拟源参数设置", deviceID, 0)
    DATA_SOURCE_ALLOC(MCSUSBTMStatusStruct, m_pMCSUSBTMStatus8, "USBBBE遥测参数设置", deviceID, 0)//左旋
    //DATA_SOURCE_ALLOC(MCSUSBTMStatusStruct, m_pMCSUSBTMStatus8, "USBBBE遥测参数设置", deviceID, 0)//右旋
    DATA_SOURCE_ALLOC(MCSUSBRevStatusStruct, m_pMCSUSBRevStatu3, "USBBBE中频接收机参数设置", deviceID, 1)
    DATA_SOURCE_ALLOC(MCSUSBRevStatusStruct, m_pMCSUSBRevStatu2, "USBBBE中频接收机参数设置", deviceID, 0)
    DATA_SOURCE_ALLOC_PRO_CTRL(PCRDATATRANS_TTC, m_usbCtlRsltRpt, "过程控制命令", deviceID, 2)
    DATA_SOURCE_ALLOC(MCSUSBTraceStatusStruct, m_pTrace9, "USBBBE主跟踪单元", deviceID, 0)
    DATA_SOURCE_ALLOC(MCSUSBTraceStatusStruct, m_pTrace10, "USBBBE主跟踪单元", deviceID, 1)
    return true;
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
    pData += 2;//过程命令标识与序列号两个字节.
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
        m_usbCtlRsltRpt->eZero4.valid = 3;
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
    {
        STRCTST  *pTemVal=(STRCTST*)pData;
        if(pTemVal->tstStatu==2)
        {
            memcpy(&m_usbCtlRsltRpt->strcTst23, pData, sizeof(STRCTST));
        }
    }
        break;
    case 31:
    {
        STDRTST  *pTemVal=(STDRTST*)pData;
        if(pTemVal->tstStatus==2)
        {
                memcpy(&m_usbCtlRsltRpt->stdrTst31, pData, sizeof(STDRTST));
        }
    }
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
        m_usbCtlRsltRpt->bzcrInquire57.valid = 1;
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
        tmp.ucSystemMode = 7;
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
    case 9://USBBBE主跟踪
        memcpy(m_pTrace9, pBuff, sizeof(MCSUSBTraceStatusStruct));
        break;
    case 10://USBBBE主跟踪
        memcpy(m_pTrace10, pBuff, sizeof(MCSUSBTraceStatusStruct));
        break;
    case 12://USBBBE模拟源参数设置
        memcpy(m_pMCSUSBTMSimulStatus12, pBuff, sizeof(MCSUSBTMSimulStatusStruct));
        break;
    default:
        break;
    }
}

void CTTC::updateAbstract(USBBBE_GlobalStatusStruct* pBbeAbstract)
{
    if(!pBbeAbstract)return;
    //
    pBbeAbstract->ucMonitorMode = m_pMCSSysDataStatusTTC1->ucMonitorMode;
    if(m_pMCSSysDataStatusTTC1->ucBBEOnline == 1){
        if(m_pTC4_4)
        {
            memcpy((char*)m_pTC4_4 + 4, m_pMCSUSBTCStatus7, sizeof(MCSUSBTCStatusStruct) - sizeof(m_pMCSUSBTCStatus7->tParams));

            memcpy(m_pTC4_4, m_pMCSUSBTCStatus7, 16);
            memcpy((char*)&m_pTC4_4->MCSSSB2TCStatu[0].tParams + 5, (char*)&m_pMCSUSBTCStatus7->tParams + 22,
                    sizeof(m_pTC4_4->MCSSSB2TCStatu[0].tParams) - 5);
            m_pTC4_4->MCSSSB2TCStatu[0].tParams.ucCodeStyle = m_pMCSUSBTCStatus7->tParams.ucCodeStyle;
            m_pTC4_4->MCSSSB2TCStatu[0].tParams.uiCodeRate = m_pMCSUSBTCStatus7->tParams.uiCodeRate;
            m_pTC4_4->MCSSSB2TCStatu[0].uiAllCmdSendTotal = m_pMCSSysDataStatusTTC1->usTCSendNum;
            m_pTC4_4->MCSSSB2TCStatu[0].uiAllSmallLoopRight = m_pMCSSysDataStatusTTC1->usTCSendNum - m_pMCSSysDataStatusTTC1->usTCSmallLoopErrNum;
            m_pTC4_4->valid = 1;
        }
        if(m_devID == 0x400)
            pBbeAbstract->ucBBEWorkID = 1;
        else pBbeAbstract->ucBBEWorkID = 2;
    }
    else
    {
        if(m_devID == 0x401)
            pBbeAbstract->ucBBEWorkID = 1;
        else pBbeAbstract->ucBBEWorkID = 2;
    }
    pBbeAbstract->ucBBEID = m_devID == 0x400 ? 1 : 2;
    pBbeAbstract->ucBBEWorkStatus = m_pMCSSysDataStatusTTC1->ucBBEWorkStatus;
    pBbeAbstract->ucSystemWorkMode = m_pMCSSysDataStatusTTC1->ucSystemMode;//按注释来的
    //pBbeAbstract->ucDataTransStatus = fix me
    //pBbeAbstract->usMID = m_pMCSSysDataStatusTTC1->tParams.uiM; 不使用
    //pBbeAbstract->nBID = fix me
    //pBbeAbstract->ucSatEncryptionFlag 不使用
    pBbeAbstract->ucSystemMode = m_pMCSSysDataStatusTTC1->ucSystemWorkMode;//按注释来的
    //pBbeAbstract->ucDeviceAssembleID = m_pMCSSysDataStatusTTC1->tParams.dwlComboID; fix me下边还有一个
    //pBbeAbstract->ucTaskID fix me
    memcpy(pBbeAbstract->szTaskName, m_pMCSSysDataStatusTTC1->tParams.ucTaskDescribe, 6);
    //pBbeAbstract->byStaID = fix me
    pBbeAbstract->i64DeviceAssembleID = m_pMCSSysDataStatusTTC1->tParams.dwlComboID;
    // fix me
    pBbeAbstract->ucTimeCodeStatus = m_pMCSSysDataStatusTTC1->ucTimeCodeStatus;
    //pBbeAbstract->ucSaveStatus = fix me
    pBbeAbstract->ucCSSendSwitch = m_pMCSSysDataStatusTTC1->ucCSSendSwitch;
    pBbeAbstract->ucCJSendSwitch = m_pMCSSysDataStatusTTC1->ucCJSendSwitch;
    pBbeAbstract->ucTMSendSwitch = m_pMCSSysDataStatusTTC1->ucTMSendSwitch;

    if(m_pMCSUSBRevStatu2->tParams.ucRevChannel < 1 || m_pMCSUSBRevStatu2->tParams.ucRevChannel > 3)return;
    //if(m_pMCSUSBRevStatu2->tParams.ucRevChannel == 1 || m_pMCSUSBRevStatu2->tParams.ucRevChannel == 3)

    updateAbstractIndexLeft(pBbeAbstract->IndexStatus[0]);
    //if(m_pMCSUSBRevStatu2->tParams.ucRevChannel == 2 || m_pMCSUSBRevStatu2->tParams.ucRevChannel == 3)
    updateAbstractIndexRight(pBbeAbstract->IndexStatus[1]);
    //UCHAR  ucSignalPolar;   //当前跟踪通道	0：无效， 1：左旋， 2：右旋， 3：不跟踪
    pBbeAbstract->byRcvType = m_pMCSSysDataStatusTTC1->ucSignalPolar;
    int index = m_pMCSSysDataStatusTTC1->ucSignalPolar == 2 ? 1 : 0;
    USBBBE_IndexStatusStruct& ref = pBbeAbstract->IndexStatus[index];
    ref.nSubCarFreq = m_pMCSUSBTMStatus8->tParams.uiSWFreq;
    ref.nCodeRate = m_pMCSUSBTMStatus8->tParams.uiTM_CodeRate;
    ref.ucSubCarLock = m_pMCSUSBTMStatus8->ucRevCarrLock;
    ref.ucCodeLock = m_pMCSUSBTMStatus8->ucCodeLock;
    ref.ucFrmLock = m_pMCSUSBTMStatus8->ucFrmLock;
    ref.ucSubFrmLock = m_pMCSUSBTMStatus8->ucSubFrmLock;
    ref.ucDobbleCap = m_pMCSSysDataStatusTTC1->ucDoubleAqu;
}

void CTTC::updateAbstractIndexLeft(USBBBE_IndexStatusStruct& index)
{
    // index.ucCarrLk: 载波锁定 00H：失锁，01H：锁定。
    //m_pMCSUSBRevStatu2->ucLockStatus; 载波锁定指示 0：无效，1：锁定，2：失锁。
    index.ucCarrLk = m_pMCSUSBRevStatu2->ucLockStatus;
    index.ucSweep = m_pMCSUSBRevStatu2->ucFollow;
    index.usAgcVolt = m_pMCSUSBRevStatu2->usAgcVolt;
    index.usSqDir = m_pMCSUSBRevStatu2->usSqDir;

    index.ucWordLen = m_pMCSUSBTMSimulStatus12->tParams.ucWordLen;
    index.usFrmLen = m_pMCSUSBTMSimulStatus12->tParams.usFrameLen;
    index.dwlSynCode = m_pMCSUSBTMSimulStatus12->tParams.uiFrameSynG;
    index.ucFspLen = m_pMCSUSBTMSimulStatus12->tParams.ucFrameSynGLen;
    index.ucSubFrmType = m_pMCSUSBTMSimulStatus12->tParams.ucSFrameSynMd;
    index.usSubFrmLen = m_pMCSUSBTMSimulStatus12->tParams.ucSframeLen;

    index.usRangeCap = m_pMCSSysDataStatusTTC1->ucDistanceAqu;
    index.ucMainTone = m_pMCSUSBCjStatus5->ucLock;
    //侧音锁定 00H：失锁，01H：锁定。
    index.ucSideTone = m_pMCSUSBCjStatus5->ucCurTone;//侧音锁定
    index.iDistance = m_pMCSUSBCjStatus5->iDistance;
    index.iDopRate = m_pMCSUSBRevStatu2->iDopRate;
    index.iSpeed = m_pMCSUSBRevStatu2->iSpeed;
}

void CTTC::updateAbstractIndexRight(USBBBE_IndexStatusStruct& index)
{
    // index.ucCarrLk: 载波锁定 00H：失锁，01H：锁定。
    //m_pMCSUSBRevStatu2->ucLockStatus; 载波锁定指示 0：无效，1：锁定，2：失锁。
    index.ucCarrLk = m_pMCSUSBRevStatu3->ucLockStatus;
    index.ucSweep = m_pMCSUSBRevStatu3->ucFollow;
    index.usAgcVolt = m_pMCSUSBRevStatu3->usAgcVolt;
    index.usSqDir = m_pMCSUSBRevStatu3->usSqDir;
    index.ucWordLen = m_pMCSUSBTMSimulStatus12->tParams.ucWordLen;
    index.usFrmLen = m_pMCSUSBTMSimulStatus12->tParams.usFrameLen;
    index.dwlSynCode = m_pMCSUSBTMSimulStatus12->tParams.uiFrameSynG;
    index.ucFspLen = m_pMCSUSBTMSimulStatus12->tParams.ucFrameSynGLen;
    index.ucSubFrmType = m_pMCSUSBTMSimulStatus12->tParams.ucSFrameSynMd;
    index.usSubFrmLen = m_pMCSUSBTMSimulStatus12->tParams.ucSframeLen;
    index.usRangeCap = m_pMCSSysDataStatusTTC1->ucDistanceAqu;
    index.ucMainTone = m_pMCSUSBCjStatus6->ucLock;
    index.ucSideTone = m_pMCSUSBCjStatus6->ucCurTone;//侧音锁定
    index.iDistance = m_pMCSUSBCjStatus6->iDistance;
    index.iDopRate = m_pMCSUSBRevStatu3->iDopRate;
    index.iSpeed = m_pMCSUSBRevStatu3->iSpeed;
}
