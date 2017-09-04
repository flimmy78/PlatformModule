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
#include "../../Common/interfaces/cdatasource.h"
#include "platformresource.h"
class CTC
{
public:
    CTC();
    bool init(USHORT deviceID, PlatformResource* pRes, SMCU2::MCSSSTCStatus4* pTC);
    USHORT m_devID;
    void set(STC&);
    void get(STC&);
    PlatformResource* m_pPlatformRes;
    //单遥控系统参数1
    SMCU2::MCSSysTarget4StatusStruct* m_pMCSSysTarget4Status1;
    //case 3://模式2调制参数设置
    SMCU2::MCSUpModuleStatusStruct4* m_pMCSUpModuleStatu4_3;
    //case 5://扩频遥控参数设置
    SMCU2::MCSSSTCStatusStruct4* m_pMCSSSB2TCStatu4_5;
    //case 9://模式2扩频码设置
    SMCU2::MCSSSB2PNCODEStatusStruct4* m_p2MCSSSPNCODEStatu4_9;
    AK_GLOBALSTATUS* m_pAkGlobalStatu;
    //case 4://单遥控发令状态
    SMCU2::MCSSSTCStatus4* m_pTC4_4;
    void processBaseBand(QByteArray &datagram);
    void processBaseBound_0xF100(char *pData);
    void processBaseBound_0xF104(char *pData, const int dataLen);
    void processBaseBound_0xF101(char * pData);
    void processBaseBoundData(BYTE *pBuff, const BYTE &unitNum);
    void getMap(BYTE *, const int dataLen, QMap<int, int>& mapFB_FE);
};

#endif // CSS2_H
