#include <QByteArray>
#include "../ddtTTCCtrlResultReport.h"
#include "../ss1trlResultReport.h"
#include "../ss2trlResultReport.h"
#include "alter.h"
#ifndef CDATASOURCE_H
#define CDATASOURCE_H
const int c_buff_size = 1024*50;
#pragma pack(1)
struct DATASOURCE_PRO
{
    quint64 id;
    int len;
    uint timeTag;
    uchar data[c_buff_size];
        //                virtual bool GetOneItemByIndex(int iIndex, quint64& ui64StatusID, uchar* pData,
        //                                               int &iDataLen, uint& uiTimeTag) = 0;
};

struct DS_EX{
    uchar* pContent;
    DATASOURCE_PRO ds;
    ~DS_EX(){pContent = NULL;}
};
struct SDDT
{//ddt
    //DDT_BBE系统参数设置
    MCSSysDataStatusStruct m_MCSSysStatus;
    //数传检测输出参数设置
    SCBBETestOutStatusStruct m_SCBBETestOutStatu;
    //数传接收机单元数据元素
    MCSDDTRevStatusStruct m_MCSDDTRevStatus;
    //数传解调器单元(I路)数据元素
    MCSDDTTMIStatusStruct m_MCSDDTTMQStatusI;
    //数传解调器单元(Q路)数据元素
    MCSDDTTMQStatusStruct m_MCSDDTTMQStatusQ;
    //数传模拟源单元数据元素
    MCSDDTTMSimulStatusStruct m_MCSDDTTMSimulStatus;
    //数传存储转发单元数据元素
    MCSDDTReplayStatusStruct m_MCSDDTReplayStatus;
    //分集合成
    MCSDivComStatusStruct m_MCSDivComStatu;
    //过程控制结果上报
    PCRDATATRANS_TTC m_ctlRsltRpt;
};
struct STTC
{//ttc
    //case 1://USBBBE系统参数设置
    MCSSysDataStatusStruct m_MCSSysDataStatusTTC1;
    //case 2://USBBBE中频接收机参数设置
    MCSUSBRevStatusStruct m_MCSUSBRevStatu2;
    //case 3://USBBBE中频接收机参数设置
    MCSUSBRevStatusStruct m_MCSUSBRevStatu3;
    //case 4://USBBBE调制参数设置
    MCSUSBUpModulStatusStruct m_MCSUSBUpModulStatus4;
    //case 5://USBBBE测距参数设置
    MCSUSBCjStatusStruct m_MCSUSBCjStatus5;
    //case 6://USBBBE测距参数设置
    MCSUSBCjStatusStruct m_MCSUSBCjStatus6;
    //case 7://USBBBE遥控参数设置
    MCSUSBTCStatusStruct m_MCSUSBTCStatus7;
    //case 8://USBBBE遥测参数设置
    MCSUSBTMStatusStruct m_MCSUSBTMStatus8;
    //case 12://USBBBE模拟源参数设置
    MCSUSBTMSimulStatusStruct m_MCSUSBTMSimulStatus12;
    //过程控制结果上报
    PCRDATATRANS_TTC m_ctlRsltRpt;
};

struct SSS1
{
    //case 1:
    SMCU::MCSSysTarget4StatusStruct m_MCSSysTarget4Status1;
    //case 2://模式1接收参数设置
    SMCU::MCSSSRevStatusStruct4 m_MCSSSRev4Statu4_2;
    //case 3://模式1调制参数设置
    SMCU::MCSSSUTargetTCSSUpModulStatusStruct m_pMCSSSUTargetTCSSUpModulStatu3;
    //case 5://扩频遥控参数设置
    SMCU::MCSSSTCStatusStruct4 m_MCSSSTCStatusStruct4_5;
    //case 4://模式1测距测速参数设置
    SMCU::MCSSSCjStatusStruct4 m_MCSSSCjStatu4_4;
    //case 6://扩频遥测解调参数设置
    SMCU::MCSSSTMStatusStruct4 m_MCSSSTMStatusStruct4_6;
    //case 8://扩频遥测模拟源参数设置
    SMCU::MCSSSTMSimulStatusStruct4 m_MCSSSTMSimulStatu4_8;
    //case 9:
    SMCU::MCSSSPNCODEStatusStruct4 m_MCSSSPNCODEStatu4_9;
    //过程控制结果上报
    PCRSS1 m_pcrSS;
};
struct SSS2
{
    //ss2
    //case 1:
    SMCU2::MCSSysTarget4StatusStruct m_MCSSysTarget4Status1;
    //case 2://模式2接收参数设置
    SMCU2::MCSRevStatusStruct4 m_MCSRevStatusStruct4_2;
    //case 3://模式2调制参数设置
    SMCU2::MCSUpModuleStatusStruct4 m_MCSUpModuleStatu4_3;
    //case 4://模式2测距测速参数设置
    SMCU2::MCSSSB2CjStatusStruct4 m_MCSSSCjStatu4_4;
    //case 5://扩频遥控参数设置
    SMCU2::MCSSSTCStatusStruct4 m_MCSSSB2TCStatu4_5;
    //case 6://扩频遥测解调参数设置
    SMCU2::MCSSSB2TMStatusStruct4 m_MCSSSB2TMStatu4_6;
    //case 8://扩频遥测模拟源参数设置
    SMCU2::MCSSSB2TMSimulStatusStruct4 m_MCSSSB2TMSimulStatu4_8;
    //case 9://模式2扩频码设置
    SMCU2::MCSSSB2PNCODEStatusStruct4 m_2MCSSSPNCODEStatu4_9;
    //过程控制结果上报
    PCRDATATRANS_TTC m_ctlRsltRpt;
    //过程控制结果上报
    PCRSS2 m_pcrSS;
};
struct STC
{
    //tc
    //case 3://模式2调制参数设置
    SMCU2::MCSUpModuleStatusStruct4  m_MCSUpModuleStatu4_3;
    //case 5://扩频遥控参数设置
    SMCU2::MCSSSTCStatusStruct4 m_MCSSSB2TCStatu4_5;
    //case 9://模式2扩频码设置
    SMCU2::MCSSSB2PNCODEStatusStruct4 m_2MCSSSPNCODEStatu4_9;

};
struct SBBE
{
    SDDT ddt;
    STTC ttc;
    SSS1 ss1;
    SSS2 ss2;
    STC tc;
    DSZ_BBE_GlobalStatusBlk m_bbeAbstract;
    USBBBE_GlobalStatusStruct m_USBAbstract;
    SS1BBE_GlobalStatusStruct m_SS1Abstract;
    SS2BBE_GlobalStatusStruct m_SS2Abstract;
};
struct SAlter
{
     ALTER m_Alter;
};
struct SAdjstAlter
{
    ADJUST_ALTER m_Alter;
};
struct SNetSwitch
{
    HK_IFSWMatStatu m_IFSWMatStatuResponse;
};

struct SDevs
{
    COMBTRANSStatu m_CombTransStatuTTC;//标准TTC联试应答机监控
    COMBTRANSStatu m_CombTransStatuFXG;//非相干扩频联试应答机监控
    ZEROCALIBBEACONStatu m_ZCBeaconStatu;//校零信标一体机监控
    SLDCDCUStatu m_SLDDcuParaStatu;//射频分路组合监控
    SSWITCHDCUStatu m_SswitchDcuStatu;//射频切换开关监控
    TEST_SWMATStatu m_TstSwMatStatu;//测试开关矩阵监控
    FREQBDCUStatu m_FreDcuStatu;//频率基准源监控
    RCLOCKDCUStatu m_RCLOCKDCUStatu;//铷钟机箱监控
    COMBTRANSPara m_CombTransParaTTC;
    COMBTRANSPara m_CombTransParaFXG;
};

struct SRM
{
    StatusFindResStruct m_StatusWorkRes;
    //文件信息列表查询结构体
    RECORDFILELIST m_FileListRes;
    //查询单个文件信息结构体
    RECORDFILEINFOCOMM m_FileInfoRes;
};
struct SAllDataSource
{
    SBBE bbe1;
    SBBE bbe2;
    SNetSwitch nsUp;
    SNetSwitch nsDown;
    SAlter alterUp1;
    SAlter alterUp2;
    SAlter alterDown1;
    SAlter alterDown2;
    SAdjstAlter adjstAlter;
    SDevs devs;
    SMCU2::MCSSSTCStatus4 m_tc;
    DevicesConnectStatus m_connectSt;
    STC safeTC1;
    STC safeTC2;
    SRM rm1;
    SRM rm2;
};
#pragma pack()

#endif // CDATASOURCE_H
