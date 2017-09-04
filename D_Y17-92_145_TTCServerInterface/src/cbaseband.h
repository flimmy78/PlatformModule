#ifndef CBASEBAND_H
#define CBASEBAND_H
#include "../../Common/MCSDataStruct.h"
#include "../../Common/MCSUSBDataStruct.h"
#include "../../Common/DataStruct2311.h"
#include "../../Common/SMCUHeadFile.h"
#include "../../Common/SMCUSSB2_BBE_CDMAStruct.h"
#include "../../Common/SMCU_BBE_CDMAStruct.h"
#include "../../Common/DeviceMap.h"
class CBaseband
{
public:
    CBaseband();
    void getDevInfo(char& bbe, char& bbeControl);//bbe体制ddt ttc ss1 ss2等
    //bbeControl 本控1，分控0;
    bool init(USHORT, PlatformResource* );
    void processBaseBand(QByteArray &datagram);
    CDDT m_ddt;
    CTTC m_ttc;
    CSS1 m_ss1;
    CSS2 m_ss2;
    CTC  m_tc;
    USHORT m_devID;
    enum SYS_MOD{DDT, USB, SS1, SS2, TC};
    SYS_MOD m_sysMod;
    PlatformResource* m_pPlatformRes;
    char m_connect;//0未连接，1连接
    void set(SBBE&);
    void get(SBBE&);
};

#endif // CBASEBAND_H
