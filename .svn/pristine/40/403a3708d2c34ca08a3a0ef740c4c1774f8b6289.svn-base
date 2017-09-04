#ifndef CTTC_H
#define CTTC_H
#include "../../Common/MCSDataStruct.h"
#include "../../Common/MCSUSBDataStruct.h"
#include "../../Common/DataStruct2311.h"
#include "../../Common/SMCUHeadFile.h"
#include "../../Common/SMCUSSB2_BBE_CDMAStruct.h"
#include "../../Common/SMCU_BBE_CDMAStruct.h"
#include "../../Common/DeviceMap.h"
#include "platformresource.h"
#include "../../Common/ddtTTCCtrlResultReport.h"
#include "../../Common/interfaces/cdatasource.h"
class CTTC
{
public:
    CTTC();

    bool init(USHORT deviceID, PlatformResource* pRes);
    void set(STTC&);
    PlatformResource* m_pPlatformRes;
    //case 1://USBBBE系统参数设置
    MCSSysDataStatusStruct* m_pMCSSysDataStatusTTC1;
    //case 2://USBBBE中频接收机参数设置
    MCSUSBRevStatusStruct* m_pMCSUSBRevStatu2;
    //case 3://USBBBE中频接收机参数设置
    MCSUSBRevStatusStruct* m_pMCSUSBRevStatu3;
    //case 4://USBBBE调制参数设置
    MCSUSBUpModulStatusStruct* m_pMCSUSBUpModulStatus4;
    //case 5://USBBBE测距参数设置
    MCSUSBCjStatusStruct* m_pMCSUSBCjStatus5;
    //case 6://USBBBE测距参数设置
    MCSUSBCjStatusStruct* m_pMCSUSBCjStatus6;
    //case 7://USBBBE遥控参数设置
    MCSUSBTCStatusStruct* m_pMCSUSBTCStatus7;
    //case 8://USBBBE遥测参数设置
    MCSUSBTMStatusStruct* m_pMCSUSBTMStatus8;
    //case 12://USBBBE模拟源参数设置
    MCSUSBTMSimulStatusStruct* m_pMCSUSBTMSimulStatus12;

    PCRDATATRANS_TTC* m_usbCtlRsltRpt;
    void processBaseBand(QByteArray &datagram);
    void processBaseBound_0xF100(char *pData);
    void processBaseBound_0xF104(char *pData, const int dataLen);
    void processBaseBound_0xF101(char * pData);
    void processBaseBoundData(BYTE *pBuff, const BYTE &unitNum);
    void getMap(BYTE *, const int dataLen, QMap<int, int>& mapFB_FE);
    void get(STTC &ttc);
};

#endif // CDDT_H
