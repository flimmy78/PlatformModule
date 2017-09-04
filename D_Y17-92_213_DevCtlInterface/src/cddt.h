#ifndef CDDT_H
#define CDDT_H
#include "../../Common/MCSDataStruct.h"
#include "../../Common/MCSUSBDataStruct.h"
#include "../../Common/DataStruct2311.h"
#include "../../Common/SMCUHeadFile.h"
#include "../../Common/SMCUSSB2_BBE_CDMAStruct.h"
#include "../../Common/SMCU_BBE_CDMAStruct.h"
#include "../../Common/DeviceMap.h"
#include "platformresource.h"
#include "../../Common/ddtTTCCtrlResultReport.h"
class CDDT
{
public:
    CDDT();

    bool init(USHORT deviceID, PlatformResource* pRes);
    PlatformResource* m_pPlatformRes;
    //case 1:     //DDT_BBE系统参数设置
    MCSSysDataStatusStruct* m_pMCSSysStatus;
    //case 8:     //数传检测输出参数设置
    SCBBETestOutStatusStruct* m_pSCBBETestOutStatu;
    //case 15:    //数传接收机单元数据元素
    MCSDDTRevStatusStruct* m_pMCSDDTRevStatus;
    //case 16:    //数传解调器单元(I路)数据元素
    MCSDDTTMIStatusStruct* m_pMCSDDTTMQStatusI;
    //case 17:    //数传解调器单元(Q路)数据元素
    MCSDDTTMQStatusStruct* m_pMCSDDTTMQStatusQ;
    //case 18:    //数传模拟源单元数据元素
    MCSDDTTMSimulStatusStruct* m_pMCSDDTTMSimulStatus;
    //case 19:    //数传存储转发单元数据元素
    MCSDDTReplayStatusStruct* m_pMCSDDTReplayStatus;
    //case 20:    //分集合成
    MCSDivComStatusStruct* m_pMCSDivComStatu;

    USHORT m_devID;
    //过程控制结果上报
    PCRDATATRANS_TTC* m_ddtCtlRsltRpt;
    void processBaseBand(QByteArray &datagram);
    void processBaseBound_0xF100(char *pData);
    void processBaseBound_0xF104(char *pData, const int dataLen);
    void processBaseBound_0xF101(char * pData);
    void processBaseBoundData(BYTE *pBuff, const BYTE &unitNum);
    void getMap(BYTE *, const int dataLen, QMap<int, int>& mapFB_FE);
    void updateAbstract(DSZ_BBE_GlobalStatusBlk* pBbeAbstract);
};

#endif // CDDT_H
