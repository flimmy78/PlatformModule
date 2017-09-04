#ifndef CSS2_H
#define CSS2_H
#include "../../Common/MCSDataStruct.h"
#include "../../Common/MCSUSBDataStruct.h"
#include "../../Common/DataStruct2311.h"
#include "../../Common/SMCUHeadFile.h"
#include "../../Common/SMCUSSB2_BBE_CDMAStruct.h"
#include "../../Common/SMCU_BBE_CDMAStruct.h"
#include "../../Common/DeviceMap.h"
#include "../../Common/ss2trlResultReport.h"
#include "platformresource_tmSrvr.h"
#include "../../Common/interfaces/cdatasource.h"
class CSS2
{
public:
    CSS2();
    bool init(USHORT deviceID, PlatformResource* pRes);
    PlatformResource* m_pPlatformRes;
    //case 1:
    SMCU2::MCSSysTarget4StatusStruct* m_pMCSSysTarget4Status1;
    //case 2://模式2接收参数设置
    SMCU2::MCSRevStatusStruct4* m_pMCSRevStatusStruct4_2;
    //case 3://模式2调制参数设置
    SMCU2::MCSUpModuleStatusStruct4* m_pMCSUpModuleStatu4_3;
    //case 4://模式2测距测速参数设置
    SMCU2::MCSSSB2CjStatusStruct4* m_pMCSSSCjStatu4_4;
    //case 5://扩频遥控参数设置
    SMCU2::MCSSSTCStatusStruct4* m_pMCSSSB2TCStatu4_5;
    //case 6://扩频遥测解调参数设置
    SMCU2::MCSSSB2TMStatusStruct4* m_pMCSSSB2TMStatu4_6;
    //case 8://扩频遥测模拟源参数设置
    SMCU2::MCSSSB2TMSimulStatusStruct4* m_pMCSSSB2TMSimulStatu4_8;
    //case 9://模式2扩频码设置
    SMCU2::MCSSSB2PNCODEStatusStruct4* m_p2MCSSSPNCODEStatu4_9;

    //过程控制结果上报
    PCRSS2* m_pcrSS;
    void processBaseBand(QByteArray &datagram);
    void processBaseBound_0xF100(char *pData);
    void processBaseBound_0xF104(char *pData, const int dataLen);
    void processBaseBound_0xF101(char * pData);
    void processBaseBoundData(BYTE *pBuff, const BYTE &unitNum);
    void getMap(BYTE *, const int dataLen, QMap<int, int>& mapFB_FE);
    void set(SSS2 &ss2);
    void get(SSS2 &ss2);
};

#endif // CSS2_H
