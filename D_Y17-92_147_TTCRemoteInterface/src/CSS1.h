#ifndef CSS1_H
#define CSS1_H
#include "../../Common/MCSDataStruct.h"
#include "../../Common/MCSUSBDataStruct.h"
#include "../../Common/DataStruct2311.h"
#include "../../Common/SMCUHeadFile.h"
#include "../../Common/SMCUSSB2_BBE_CDMAStruct.h"
#include "../../Common/SMCU_BBE_CDMAStruct.h"
#include "../../Common/DeviceMap.h"
#include "../../Common/ss1trlResultReport.h"
#include "../../Common/interfaces/cdatasource.h"
#include "platformresource.h"

class CSS1
{
public:
    CSS1();

    bool init(USHORT deviceID, PlatformResource* pRes);
    PlatformResource* m_pPlatformRes;
    //case 1:
    SMCU::MCSSysTarget4StatusStruct* m_pMCSSysTarget4Status1;
    //case 2://模式1接收参数设置
    SMCU::MCSSSRevStatusStruct4* m_pMCSSSRev4Statu4_2;
    //case 3://模式1调制参数设置
    SMCU::MCSSSUTargetTCSSUpModulStatusStruct* m_pMCSSSUTargetTCSSUpModulStatu3;
    //case 5://扩频遥控参数设置
    SMCU::MCSSSTCStatusStruct4* m_pMCSSSTCStatusStruct4_5;
    //case 4://模式1测距测速参数设置
    SMCU::MCSSSCjStatusStruct4* m_pMCSSSCjStatu4_4;
    //case 6://扩频遥测解调参数设置
    SMCU::MCSSSTMStatusStruct4* m_pMCSSSTMStatusStruct4_6;
    //case 8://扩频遥测模拟源参数设置
    SMCU::MCSSSTMSimulStatusStruct4* m_pMCSSSTMSimulStatu4_8;
    //case 9:
    SMCU::MCSSSPNCODEStatusStruct4* m_pMCSSSPNCODEStatu4_9;

    //过程控制结果上报
    PCRSS1* m_pcrSS;
    void processBaseBand(QByteArray &datagram);
    void processBaseBound_0xF100(char *pData);
    void processBaseBound_0xF104(char *pData, const int dataLen);
    void processBaseBound_0xF101(char * pData);
    void processBaseBoundData(BYTE *pBuff, const BYTE &unitNum);
    void getMap(BYTE *, const int dataLen, QMap<int, int>& mapFB_FE);
    void set(SSS1 &ss1);
    void get(SSS1 &ss1);
};

#endif // CSS1_H
