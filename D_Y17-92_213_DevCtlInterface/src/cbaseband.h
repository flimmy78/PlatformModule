#ifndef CBASEBAND_H
#define CBASEBAND_H
#include "../../Common/MCSDataStruct.h"
#include "../../Common/MCSUSBDataStruct.h"
#include "../../Common/DataStruct2311.h"
#include "../../Common/SMCUHeadFile.h"
#include "../../Common/SMCUSSB2_BBE_CDMAStruct.h"
#include "../../Common/SMCU_BBE_CDMAStruct.h"
#include "../../Common/DeviceMap.h"
#include "cddt.h"
#include "CTTC.h"
#include "CSS1.h"
#include "CSS2.h"
#include "CTC.h"
class CBaseband
{
public:
    CBaseband();
    void getDevInfo(char& bbe, char& bbeControl);//bbe体制ddt ttc ss1 ss2等
    //bbeControl 本控1，分控0;
    bool init(USHORT, PlatformResource*, SMCU2::MCSSSTCStatus4*);
    CDDT m_ddt;
    CTTC m_ttc;
    CSS1 m_ss1;
    CSS2 m_ss2;
    CTC  m_tc;
    USHORT m_devID;
    DSZ_BBE_GlobalStatusBlk* m_pBbeAbstract;
    quint64 m_ddtTcID;
    USBBBE_GlobalStatusStruct* m_pUSBAbstract;
    quint64 m_usbID;
    SS1BBE_GlobalStatusStruct* m_pSS1Abstract;
    quint64 m_ss1ID;
    SS2BBE_GlobalStatusStruct* m_pSS2Abstract;
    quint64 m_ss2ID;
    enum SYS_MOD{DDT, USB, SS1, SS2, TC, UNKNOWN};
    SYS_MOD m_sysMod;
    PlatformResource* m_pPlatformRes;
    int m_connect;//不断增加
    int m_connectOld;//保存m_connect;
    void processBaseBand(QByteArray &datagram, const QString &chlID);
};

#endif // CBASEBAND_H
