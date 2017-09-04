#ifndef CTC_H
#define CTC_H
#include "../../Common/MCSDataStruct.h"
#include "../../Common/MCSUSBDataStruct.h"
#include "../../Common/DataStruct2311.h"
#include "../../Common/SMCUHeadFile.h"
#include "../../Common/SMCUSSB2_BBE_CDMAStruct.h"
#include "../../Common/SMCU_BBE_CDMAStruct.h"
#include "../../Common/DeviceMap.h"
#include "../../Common/struct_header/Dcu_2311_AK_DataStruct.h"
#include "../../Common/ss2trlResultReport.h"
#include "platformresource.h"
//安控也是用这个结构。
class CTC
{
public:
    CTC();
    bool init(USHORT deviceID, PlatformResource* pRes, SMCU2::MCSSSTCStatus4* pTC);
    PlatformResource* m_pPlatformRes;
    USHORT m_devID;
    quint64 m_timeTagID;
    //单遥控系统参数1
    SMCU2::MCSSysTarget4StatusStruct* m_pMCSSysTarget4Status1;
    //case 3://单遥控调制参数设置
    SMCU2::MCSUpModuleStatusStruct4* m_pMCSUpModuleStatu4_3;
    //case 5://单遥控遥控参数设置
    SMCU2::MCSSSTCStatusStruct4* m_pMCSSSB2TCStatu4_5;
    //case 9://单遥控扩频码设置
    SMCU2::MCSSSB2PNCODEStatusStruct4* m_p2MCSSSPNCODEStatu4_9;
    //case 4://单遥控发令状态
    SMCU2::MCSSSTCStatus4* m_pTC4_4;

    AK_GLOBALSTATUS* m_pAkGlobalStatu;

    //过程控制结果上报
    PCRSS2* m_pcrSS;

    //case 2://模式2接收参数设置
    SMCU2::MCSRevStatusStruct4* m_pMCSRevStatusStruct4_2;
    //case 6://扩频遥测解调参数设置
    SMCU2::MCSSSB2TMStatusStruct4* m_pMCSSSB2TMStatu4_6;
    //case 8://扩频遥测模拟源参数设置
    SMCU2::MCSSSB2TMSimulStatusStruct4* m_pMCSSSB2TMSimulStatu4_8;
    void processBaseBand(QByteArray &datagram);
    void processBaseBound_0xF100(char *pData);
    void processBaseBound_0xF104(char *pData, const int dataLen);
    void processBaseBound_0xF101(char * pData);
    void processBaseBoundData(BYTE *pBuff, const BYTE &unitNum);
    void getMap(BYTE *, const int dataLen, QMap<int, int>& mapFB_FE);
    void updateAbstract(DSZ_BBE_GlobalStatusBlk *pBbeAbstract);
    void updateAbstractAK();
    void getDevInfo(char &bbe);

    long m_connect;
    long m_connectOld;
    bool m_change;
};

#endif // CSS2_H
