#ifndef _GLOBAL_H_
#define _GLOBAL_H_
//宏定义

//包含头文件
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <QString>
#include <QThread>
#include <QDebug>
#include <QMutex>
#include <QWaitCondition>
#include <QFile>
#include <QDir>
#include "../Common/LxTsspCommonDefinition.h"
#include "../Common/TypeDefine.h"
#include "RecvDataThread.h"
#include "DataStructTTCDevCtlInterface.h"
#include "../Common/SMCUHeadFile.h"
#include "../Common/DeviceMap.h"
#include "SearchStatusThread.h"
#include "DevCtlInterface_global.h"
#include "cshowdatagram.h"
#include "../Common/SMCU_BBE_CDMAStruct.h"
#include "../Common/SMCUSSB2_BBE_CDMAStruct.h"
#include "../Common/Dcu_common_DataStruct.h"
#include "connectstatus.h"
#include <QMap>
#include "../Common/ddtTTCCtrlResultReport.h"
struct CGlobalContext
 {
    CGlobalContext(){m_pDeviceLinkStatus = NULL;m_deviceIndex = NODEV;}
    CDeviceMap m_devMap;
    TLxTsspObjectInfo m_lsTsspObjInfo;
    USHORT m_usDeviceID;
    int m_deviceIndex;
    const static int NODEV;
    DevicesConnectStatus* m_pDeviceLinkStatus;
    BBE_GlobalStatusStruct m_pBBBGlobalStatus[3];
    enum {BBE_1, BBE_2, BBE_3, DDT, TTC, SS1, SS2};
    enum {FM_BBE_1, FM_BBE_2, FM_BBE_3};
    enum {BPSK_BBE_1, BPSK_BBE_2, BPSK_BBE_3};
    enum {SS_BBE_1, SS_BBE_2, SS_BBE_3};
    enum {FM_AK_1, FM_AK_2};
    enum {ZZM_AK_1, ZZM_AK_2};
    void initIDs();
    QMap<int, quint64> m_ddtId;
};
#ifdef __ONCE_TEMP_

LxTsspObjectManager* g_pObjectManager = NULL;
ILxTsspStateManager* g_pILxTsspStateManager = NULL;
ILxTsspMessageManager *g_MessageManager=NULL;
TLxTsspObjectInfo g_ObjectInfo;

LxTsspObjectManager* m_pObjectManager = NULL;
ILxTsspCommunicationManager *mpCommunicationMgr = NULL;

RecvDataThread *s_link = NULL;
SearchStatusThread *g_pSearchThread = NULL;
DevCtl::ConnectStatusCollect* g_pDeviceConnectStatu = NULL;
BOOL bLinkThread = false;

WORD g_wSID = 0x0100;
WORD g_wDID = 0x0100;

//命令中心
DWORD g_dwCmcHandle = 0;
//本模块发出命令管理中心命令序号
DWORD g_dwSequence = 0;

RECORD_RESPONSE * g_pRecoedResponse = NULL;

STATUS_RESPONSE * g_pStatusResponse = NULL;
HK_IFSWMatPara* g_pIFSWMatParaResponse = NULL;
HK_IFSWMatStatu* g_pIFSWMatStatuResponse = NULL;
HK_IFSWMatResponse *g_pIFSWMatResponse = NULL;  //添加状态响应变量

CGlobalContext g_context;
UPCTLCMD* g_pUnitParamControlCmd = NULL;
PCTLCMD* g_pProcessControlCmd = NULL;
GPSETCMD* g_pGroupParamSetCmd = NULL;
PMCSDDTRevStatusStruct g_pMCSDDTRevStatus = NULL;
PMCSDDTTMQStatusStruct g_pMCSDDTTMQStatusI = NULL;
PMCSDDTTMQStatusStruct g_pMCSDDTTMQStatusQ = NULL;
PMCSDDTTMSimulStatusStruct g_pMCSDDTTMSimulStatus = NULL;
PMCSDDTReplayStatusStruct g_pMCSDDTReplayStatus = NULL;
PMCSSysDataStatusStruct g_pMCSSysStatus = NULL;
PMCSDivComStatusStruct g_pMCSDivComStatus = NULL;      //分集合成状态参数
PCRDATATRANS_TTC* g_pPCReport = NULL;//过程控制上报
RCLOCKDCUStatu *g_pRCLOCKDCUStatu = NULL;           //铷钟机箱监控工作状态结构体
SLDCDCUStatu* g_pSLDDcuParaStatu = NULL;            //射频分路组合监控工作状态结构体
SSWITCHDCUStatu* g_pSswitchDcuStatu = NULL;         //射频切换开关监控工作状态上报结构体
TEST_SWMATStatu* g_pTstSwMatStatu = NULL;           //测试开关矩阵工作状态结构体
FREQBDCUStatu* g_pFreDcuStatu = NULL;               //频率基准源监控工作状态结构体
ZEROCALIBBEACONStatu* g_pZCBeaconStatu = NULL;      //校零信标一体机工作状态上报结构体指针
COMBTRANSStatu *g_pCombTransStatuTTC = NULL;        //标准TTC联试应答机工作状态上报的结构体指针
COMBTRANSPara *g_pCombTransParaTTC = NULL;          //标准TTC联试应答机宏查询之后参数返回的结构体指针
COMBTRANSStatu *g_pCombTransStatuFXG = NULL;        //非相干扩频联试应答机工作状态上报的结构体指针
COMBTRANSPara *g_pCombTransParaFXG = NULL;          //非相干扩频联试应答机宏查询之后参数返回的结构体指针
SCBBETestOutStatusStruct* g_pSCBBETestOutStatu = NULL;
MCSUSBRevStatusStruct *g_pMCSUSBRevStatu2 = NULL;
MCSUSBRevStatusStruct *g_pMCSUSBRevStatu3 = NULL;
MCSUSBTMStatusStruct* g_pMCSUSBTMStatus8 = NULL;
MCSUSBTMSimulStatusStruct* g_pMCSUSBTMSimulStatus12 = NULL;
MCSUSBCjStatusStruct* g_pMCSUSBCjStatus5 = NULL;
MCSUSBCjStatusStruct* g_pMCSUSBCjStatus6 = NULL;
MCSUSBUpModulStatusStruct* g_pMCSUSBUpModulStatus4 = NULL;
MCSUSBTCStatusStruct* g_pMCSUSBTCStatus7 = NULL;
MCSSysDataStatusStruct* g_pMCSSysDataStatusTTC1 = NULL;
SMCU::MCSSysTarget4StatusStruct* g_pMCSSysTarget4Status1 = NULL;
SMCU::MCSSysTarget4StatusStruct* g_p2MCSSysTarget4Status1 = NULL;
SMCU::MCSSSUTargetTCSSUpModulStatusStruct* g_pMCSSSUTargetTCSSUpModulStatu3 = NULL;
SMCU::MCSSSTCStatusStruct4* g_pMCSSSTCStatusStruct4_5 = NULL;
SMCU::MCSSSTCStatusStruct4* g_p2MCSSSTCStatusStruct4_5 = NULL;
SMCU::MCSSSTMStatusStruct4* g_pMCSSSTMStatusStruct4_6 = NULL;
SMCU::MCSSSTMStatusStruct4* g_p2MCSSSTMStatusStruct4_6 = NULL;
SMCU::MCSSSCjStatusStruct4* g_pMCSSSCjStatu4_4 = NULL;
SMCU::MCSSSTMSimulStatusStruct4* g_pMCSSSTMSimulStatu4_8 = NULL;
SMCU::MCSSSTMSimulStatusStruct4* g_p2MCSSSTMSimulStatu4_8 = NULL;
SMCU::MCSSSPNCODEStatusStruct4* g_pMCSSSPNCODEStatu4_9 = NULL;
SMCU2::MCSSSB2PNCODEStatusStruct4* g_p2MCSSSPNCODEStatu4_9 = NULL;
SMCU::MCSSSUTargetTCSSUpModulStatusStruct* g_p2MCSSSUTargetTCSSUpModulStatu3 = NULL;
SMCU::MCSSSCjStatusStruct4* g_p2MCSSSCjStatu4_4 = NULL;
CShowDatagram g_showDatagram;//用于调试, 为显示发送报文与接收报文提的界面提供数据.
SMCU::MCSSSRevStatusStruct4* g_pMCSSSRev4Statu4_2 = NULL;//模式1接收参数设置
SMCU::MCSSSRevStatusStruct4* g_p2MCSSSRev4Statu4_2 = NULL;//模式2接收参数设置

#else
extern SMCU::MCSSSRevStatusStruct4* g_p2MCSSSRev4Statu4_2;
extern SMCU::MCSSSRevStatusStruct4* g_pMCSSSRev4Statu4_2;
extern SearchStatusThread* g_pSearchThread;
extern DevCtl::ConnectStatusCollect* g_pDeviceConnectStatu;
extern RecvDataThread *s_link;
extern CShowDatagram g_showDatagram;
extern SMCU2::MCSSSB2PNCODEStatusStruct4* g_p2MCSSSPNCODEStatu4_9;
extern SMCU::MCSSSPNCODEStatusStruct4* g_pMCSSSPNCODEStatu4_9;
extern SMCU::MCSSSTMSimulStatusStruct4* g_pMCSSSTMSimulStatu4_8;
extern SMCU::MCSSSTMSimulStatusStruct4* g_p2MCSSSTMSimulStatu4_8;
extern SMCU::MCSSSCjStatusStruct4* g_pMCSSSCjStatu4_4;
extern SMCU::MCSSSCjStatusStruct4* g_p2MCSSSCjStatu4_4;
extern SMCU::MCSSSTMStatusStruct4* g_pMCSSSTMStatusStruct4_6;
extern SMCU::MCSSSTMStatusStruct4* g_p2MCSSSTMStatusStruct4_6;
extern SMCU::MCSSSTCStatusStruct4* g_pMCSSSTCStatusStruct4_5;
extern SMCU::MCSSSTCStatusStruct4* g_p2MCSSSTCStatusStruct4_5;
extern SMCU::MCSSSUTargetTCSSUpModulStatusStruct* g_pMCSSSUTargetTCSSUpModulStatu3;
extern SMCU::MCSSSUTargetTCSSUpModulStatusStruct* g_p2MCSSSUTargetTCSSUpModulStatu3;
extern SMCU::MCSSysTarget4StatusStruct* g_pMCSSysTarget4Status1;
extern SMCU::MCSSysTarget4StatusStruct* g_p2MCSSysTarget4Status1;
extern MCSSysDataStatusStruct* g_pMCSSysDataStatusTTC1; //USBBBE系统参数设置
extern MCSUSBTCStatusStruct* g_pMCSUSBTCStatus7;
extern MCSUSBUpModulStatusStruct* g_pMCSUSBUpModulStatus4;//USBBBE调制参数设置
extern MCSUSBCjStatusStruct* g_pMCSUSBCjStatus5;
extern MCSUSBCjStatusStruct* g_pMCSUSBCjStatus6;
extern MCSUSBTMSimulStatusStruct* g_pMCSUSBTMSimulStatus12;
extern MCSUSBTMStatusStruct* g_pMCSUSBTMStatus8;
extern MCSUSBRevStatusStruct *g_pMCSUSBRevStatu2;//USBBBE中频接收机参数设置
extern MCSUSBRevStatusStruct *g_pMCSUSBRevStatu3;
extern SCBBETestOutStatusStruct* g_pSCBBETestOutStatu;
extern COMBTRANSStatu *g_pCombTransStatuTTC;        //标准TTC联试应答机工作状态上报的结构体指针
extern COMBTRANSPara *g_pCombTransParaTTC;          //标准TTC联试应答机宏查询之后参数返回的结构体指针
extern COMBTRANSStatu *g_pCombTransStatuFXG;        //非相干扩频联试应答机工作状态上报的结构体指针
extern COMBTRANSPara *g_pCombTransParaFXG;          //非相干扩频联试应答机宏查询之后参数返回的结构体指针
extern ZEROCALIBBEACONStatu* g_pZCBeaconStatu;      //校零信标一体机工作状态上报结构体指针
extern FREQBDCUStatu* g_pFreDcuStatu;               //频率基准源监控工作状态结构体
extern TEST_SWMATStatu* g_pTstSwMatStatu;           //测试开关矩阵工作状态结构体
extern SSWITCHDCUStatu* g_pSswitchDcuStatu;         //射频切换开关监控工作状态上报结构体
extern SLDCDCUStatu* g_pSLDDcuParaStatu;            //射频分路组合监控工作状态结构体
extern RCLOCKDCUStatu *g_pRCLOCKDCUStatu;           //铷钟机箱监控工作状态结构体
extern PCRDATATRANS_TTC* g_pPCReport;
extern LxTsspObjectManager* m_pObjectManager;
extern ILxTsspCommunicationManager *mpCommunicationMgr;
extern LxTsspObjectManager* g_pObjectManager;
extern ILxTsspStateManager* g_pILxTsspStateManager;
extern ILxTsspMessageManager *g_MessageManager;
extern UPCTLCMD* g_pUnitParamControlCmd;
extern PCTLCMD* g_pProcessControlCmd;
extern GPSETCMD* g_pGroupParamSetCmd;
extern PMCSDDTRevStatusStruct g_pMCSDDTRevStatus;//数传接收机参数设置
extern PMCSDDTTMQStatusStruct g_pMCSDDTTMQStatusI;
extern PMCSDDTTMQStatusStruct g_pMCSDDTTMQStatusQ;
extern PMCSDDTTMSimulStatusStruct g_pMCSDDTTMSimulStatus;
extern PMCSDDTReplayStatusStruct g_pMCSDDTReplayStatus;
extern PMCSSysDataStatusStruct g_pMCSSysStatus; //公共参数单元
//extern MCSSysDataStatusStruct g_baseBoundCommonUnitData;
extern PMCSDivComStatusStruct g_pMCSDivComStatus;      //分集合成状态参数
//对象信息
extern TLxTsspObjectInfo g_ObjectInfo;

extern WORD g_wSID;
extern WORD g_wDID;

//命令中心
extern DWORD g_dwCmcHandle;
//本模块发出命令管理中心命令序号
extern DWORD g_dwSequence;

extern RECORD_RESPONSE * g_pRecoedResponse;

extern STATUS_RESPONSE *g_pStatusResponse;
extern PROCOTOL_PACK *g_pProctolPack;           //协议包1的数据结构变量
extern HK_IFSWMatPara *g_pIFSWMatParaResponse;
extern HK_IFSWMatStatu *g_pIFSWMatStatuResponse;
extern HK_IFSWMatResponse *g_pIFSWMatResponse;
extern CGlobalContext g_context;
#endif

//1.获取对象管理器和状态管理器
BOOL GetNeedManager();

//2. 从信息元管理器中申请一个信息元（由信息元管理器申请一个节点和相应的数据空间）
//参数：wSrcSubSys:源分系统号；wDstSubSys:目的分系统号；wType:信息元类型号；
//      wID:信息元ID；
BYTE*  ApplyOneItem(ushort usStationID, WORD wSrcSubSys,WORD wDstSubSys,WORD wType,\
                                         WORD wID, int &dwLen);
//3. 从信息元管理器中释放一个信息元（由信息元管理器申请一个节点和相应的数据空间）
//返回值：TRUE--释放成功；FALSE:释放失败；
//参数：wSrcSubSys:源分系统号；wDstSubSys:目的分系统号；wType:信息元类型号；
//      wID:信息元ID；
BOOL  RemoveOneItem(ushort usStationID,  WORD wSrcSubSys, WORD wDstSubSys, WORD wType,\
                                        WORD wID);

//4. 从信息元管理器中查找一个信息元（由信息元管理器申请一个节点和相应的数据空间）
//返回值：NULL--找不到；其它--指向信息元节点数据区的指针；dwLen:返回实际的长度
//参数：wSrcSubSys:源分系统号；wDstSubSys:目的分系统号；wType:信息元类型号；
//      wID:信息元ID；dwLen:申请数据的长度（信息元数据区长度）
BYTE*  FindOneItem(ushort usStationID,  WORD wSrcSubSys, WORD wDstSubSys, WORD wType,\
                                        WORD wID, int &dwLen);

//5. 更新信息元管理器中的一个节点内容，并通过DM发送出去(数据长度不能超过8K<8000>字节)
//返回值：NULL--更新失败；其它--指向信息元节点数据区的指针；dwSrcDataLen:返回实际的长度
//参数：wSrcSubSys:源分系统号；wDstSubSys:目的分系统号；wType:信息元类型号；
//      wID:信息元ID；pSrcData:新的数据；
//      dwSrcDataLen:新数据的长度（信息元数据区长度）
BYTE*  UpdateOneItem(ushort usStationID,  WORD wSrcSubSys, WORD wDstSubSys, WORD wType,\
                                  WORD wID, LPBYTE pSrcData, int &dwSrcDataLen);

BOOL  MsgRevRegister(QString strName);
BOOL  MsgRevUnregister(QString strName);

void MsgRev(TLxTsspMessage msg);

int   SendCmdMessage(WORD SID, WORD DID,WORD SN, WORD ItemC, \
                   WORD ItemT, BYTE *pData, DWORD dwDataLen, DWORD &Sreq, UINT Handle);
int   SendCmdMessage(WORD SID, WORD DID,WORD SN, WORD T, WORD ItemC, \
                   WORD ItemT, BYTE *pData, DWORD dwDataLen, DWORD Sreq, UINT Handle);
int   SendCmdMessage(WORD SID, WORD DID,WORD SubSID, WORD SubDID,  WORD SN, WORD ItemC, \
                   WORD ItemT, BYTE *pData, DWORD dwDataLen, DWORD &Sreq, UINT Handle);
int   SendCmdMessageNoSubPack(WORD SID, WORD DID, WORD SN, WORD ItemC, \
                   WORD ItemT, BYTE *pData, DWORD dwDataLen, DWORD &Sreq, UINT Handle);
int   SendCmdMessage(char* pData, unsigned short  usLen, unsigned usC, unsigned short usTID);

#endif // _GLOBAL_H_
