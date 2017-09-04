#include "CTC.h"
#include "../../Common/Log.h"
#include "DataStructTMServerInterface.h"
#include "Global.h"
CTC::CTC()
{
}


bool CTC::init(USHORT deviceID, PlatformResource* pRes)
{
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
    return true;
            return true;
}

void CTC::set(STC & tc)
{
    memcpy(m_p2MCSSSPNCODEStatu4_9, &tc.m_2MCSSSPNCODEStatu4_9,sizeof(tc.m_2MCSSSPNCODEStatu4_9));
    memcpy(m_pMCSSSB2TCStatu4_5, &tc.m_MCSSSB2TCStatu4_5,sizeof(tc.m_MCSSSB2TCStatu4_5));
    memcpy(m_pMCSUpModuleStatu4_3, &tc.m_MCSUpModuleStatu4_3,sizeof(tc.m_MCSUpModuleStatu4_3));
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

void CTC::processBaseBound_0xF101(char * /*pData*/)
{
//    if(!g_pPCReport)
//    {
//        CLog::addLog(PC_INFO_SOURCE_ERR);
//        return;
//    }
//    //过程命令标识，在上报时，对低字节进行取反，高字节不变。
//    //逆操作上述过程得到原来的过程命令标识.
//    USHORT id = 0;
//    memcpy(&id, pData, sizeof(USHORT));
//    UCHAR ucCode = id;
//    ucCode = 0xFF-ucCode;
//    pData += 4;//过程命令标识与序列号共4个字节,各两个字节.
//    switch(ucCode)
//    {
//    case 1://任务代号下发
//        memcpy(&g_pPCReport->tcIssue1, pData, sizeof(TCISSUE));
//        break;
//    case 2://跟踪通道选择
//        g_pPCReport->tcSel2.sel = (UCHAR)pData[0];
//        break;
//    case 3://应答机零值装订
//        memcpy(&g_pPCReport->tzBound3, pData, sizeof(TZBOUND));
//        break;
//    case 37://数传频率动态范围测试
//        memcpy(&g_pPCReport->dfdrTst37, pData, sizeof(DFDRTST));
//        break;
//    case 38://实时误码帧统计
//        memcpy(&g_pPCReport->rtefStat38, pData, sizeof(RTEFSTAT));
//        break;
//    case 39://误码率测试
//        memcpy(&g_pPCReport->berTst39, pData, sizeof(BERTST));
//        break;
//    case 40://误帧率测试
//        memcpy(&g_pPCReport->ferTst40, pData, sizeof(FERTST));
//        break;
//    case 41://遥控小环误指令测试
//        memcpy(&g_pPCReport->rcsreiTst41, pData, sizeof(RCSREITST));
//        break;
//    default:
//        CLog::addLog(tr("单元序号错误.\n"));
//    }
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

void CTC::processBaseBoundData(BYTE *pBuff, const BYTE &unitNum)
{
    switch(unitNum)
    {
    case 3://模式2调制参数设置
        memcpy(m_pMCSUpModuleStatu4_3, pBuff, sizeof(SMCU2::MCSUpModuleStatusStruct4));
        break;
    case 5://扩频遥控参数设置
        memcpy(m_pMCSSSB2TCStatu4_5, pBuff, sizeof(SMCU2::MCSSSTCStatusStruct4));
        break;
    case 9://模式2扩频码设置
        memcpy(m_p2MCSSSPNCODEStatu4_9, pBuff, sizeof(SMCU2::MCSSSB2PNCODEStatusStruct4));
        break;
    }
}
void CTC::get(STC &tc){
    memcpy( &tc.m_2MCSSSPNCODEStatu4_9 ,m_p2MCSSSPNCODEStatu4_9 ,sizeof(tc.m_2MCSSSPNCODEStatu4_9));
    memcpy(&tc.m_MCSSSB2TCStatu4_5 , m_pMCSSSB2TCStatu4_5,sizeof(tc.m_MCSSSB2TCStatu4_5));
    memcpy(&tc.m_MCSUpModuleStatu4_3 , m_pMCSUpModuleStatu4_3,sizeof(tc.m_MCSUpModuleStatu4_3));
}
