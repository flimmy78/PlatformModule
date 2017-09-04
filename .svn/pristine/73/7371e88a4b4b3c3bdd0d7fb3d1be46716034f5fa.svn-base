#ifndef  DATASTRUCTTTCDEVCTLINTERFACE_H
#define DATASTRUCTTTCDEVCTLINTERFACE_H
#include "../Common/TypeDefine.h"
#include "../Common/MCSDataStruct.h"
#include "../Common/DataStruct2311.h"
#define MaxDataLen 4096
#define L_BUFFER_LENGTH		(1024 * 8)

#pragma pack (1)

//下行中频开关矩阵监控接口协议 v1.0
//定义具体的业务数据段
typedef union _data_context_
{
    //0xFF,正确执行
    //0x61,非法码；
    //0x62, 帧校验出错；（相关设备无帧校验则不上报该信息）
    //0x63, 条件不具备不执行；（分控状态下不执行本控命令或其它情况）
    //0x64, 超限码
    BYTE answer;
    HK_IFSWMatPara ifSwMatPara;
    HK_IFSWMatStatu ifSwMatParaStatus;
    SLDCDCUPara sldDcuPara;
    SLDCDCUStatu sldDcuParaStatu;   //射频分路开关监控
    SSWITCHDCUPara ssDcuPara;
    SSWITCHDCUStatu ssDcuStatu;     //射频切换开关监控
    TEST_SWMATPara tstSwMatPara;
    TEST_SWMATStatu tstSwMatStatu;
    FREQBDCUPara freDcuPara;
    FREQBDCUStatu freDcuStatu;
    ZEROCALIBBEACONPara zcBeaconPara;
    ZEROCALIBBEACONStatu zcBeaconStatu;
    COMBTRANSPara svSysPara;  //将其注释 防止发送数据量出错
    COMBTRANSStatu svSysStatu;
    UCHAR ucTmp[L_BUFFER_LENGTH];              //临时缓存
}DATACONTEXT;

//协议包的数据段
typedef struct pack_data_struct_
{
    USHORT S1; //为源分系统编号
    USHORT D1; //为目的分系统编号
    USHORT T1; //为命令类型
    USHORT I1; //为同类命令的序号(从0开始)，针对设备板的命令(如参数加载)，为T类型板的板号
    USHORT O1; //为操作字由设备驱动接口使用， O=0表示对设备进行实际操作，O=1表示不操作
    USHORT L1; //为Dat1数据所占的字节数

    DATACONTEXT Dat1;
}PACK_DATA;

//协议数据包格式
typedef struct _procotol_pack_struct_
{
    UINT H;    //应用标识号
    UINT SEQ;  //信息报文帧序号
    UCHAR M;   //任务代号
    UCHAR T;   //信息报文帧类型
    USHORT L;  //信息报文帧长度
    UCHAR P;   //优先级
    USHORT C;  //信息代号
    UCHAR A;   //附加字
    USHORT S;  //信源，通常为发送方设备号
    USHORT D;  //信源，通常为接收方设备号
    USHORT R;  //保留字段，填充为0000H

    PACK_DATA Data; //协议具体数据段

}PROCOTOL_PACK;

//基带网络自定义帧格式
typedef struct
{
    INT SID;//信源地址
    INT DID;//目的地址
    INT BID;//信息类别
    SHORT Ver;//版本
    SHORT Reserve;//保留
    INT Date;//日期
    INT Time;//时间
    INT DataLen;//数据域长度
    //DataField 下面定义
}NETSDFRM;//net self defined frame.

//扩频组参数设置
typedef struct _SSBBEGROUPPARA
{
    WORD wUnit1Head;
    SMCU::MCSSysTarget4ParamStruct GlobalSetPara1;
    WORD wUnit1End;
    WORD wUnit2Head;
    SMCU::MCSSSRevParamStruct4 mcsSSRec2;
    WORD wUnit2End;
    WORD wUnit3Head;
    SMCU::MCSSSUTargetTCSSUpModulParamsStruct UpModul3;
    WORD wUnit3End;
    WORD wUnit4Head;
    SMCU::MCSSSCjParamStruct4 MCSSSCjParam4;
    WORD wUnit4End;
    WORD wUnit5Head;
    SMCU::MCSSSTCParamStruct4 MCSSSTCParam5;
    WORD wUnit5End;
    WORD wUnit6Head;
    SMCU::MCSSSTMParamStruct4 MCSSSTMParam6;
    WORD wUnit6End;
//    WORD wUnit7Head;
//    SSBBECTMEmulatorSetStruct mcsSSTMEmulator;
//    WORD wUnit7End;
    WORD wUnit8Head;
    SMCU::MCSSSTMSimulParamStruct4 MCSSSTMSimulParam8;
    WORD wUnit8End;
    WORD wUnit9Head;
    SMCU::MCSSSPNCODEParamStruct4 mSSPNCode9;
    WORD wUnit9End;
}SSBBEGROUPPARA, *PSSBBEGROUPPARA;

//单遥控模式组参数
typedef struct _tag_TCGroup
{
    WORD wUnit1Head;
    SMCU::MCSSSPNCODEParamStruct4 mSSPNCode1;
    WORD wUnit1End;
    WORD wUnit2Head;
    SMCU::MCSSSUTargetTCSSUpModulParamsStruct UpModu2;
    WORD wUnit2End;
    WORD wUnit3Head;
    SMCU::MCSSSTCParamStruct4 MCSSSTCParam3;
    WORD wUnit3End;
}TCGroup;

//数传组参数设置
typedef struct _tag_MSCDataTRans{
    WORD wUnit1Head;
    MCSSysParamStruct MCSSysParam;
    WORD wUnit1End;
    WORD wUnit15Head;
    MCSDDTRevParamStruct MCSDDTRev;
    WORD wUnit15End;
    WORD wUnit16Head;
    MCSDDTTMIParamStruct MCSDDTTMI;
    WORD wUnit16End;
    WORD wUnit17Head;
    MCSDDTTMQParamStruct MCSDDTTMQ;
    WORD wUnit17End;
    WORD wUnit18Head;
    MCSDDTTMSimulParamStruct MCSDDTTMSimul;
    WORD wUnit18End;
    WORD wUnit19Head;
    MCSDDTReplayParamStruct MCSDDTReplay;
    WORD wUnit19End;
    WORD wUnit20Head;
    MCSDivComParamStruct MCSDivCom;
    WORD wUnit20End;
}MSCDataTransGroup;

//TTC组参数设置
typedef struct _tag_USBTTC{
    WORD wUnit1Head;
    MCSSysDataStruct MCSSysDataTTC1;
    WORD wUnit1End;
    WORD wUnit2Head;
    MCSUSBRevParamStruct MCSUSBRev2;
    WORD wUnit2End;
    WORD wUnit3Head;
    MCSUSBRevParamStruct MCSUSBRev3;
    WORD wUnit3End;
    WORD wUnit4Head;
    MCSUSBUpModulParamStruct MCSUSBUpModulParam4;
    WORD wUnit4End;
    WORD wUnit5Head;
    MCSUSBCjParamStruct MCSUSBCjParam5;
    WORD wUnit5End;
    WORD wUnit6Head;
    MCSUSBCjParamStruct MCSUSBCjParam6;
    WORD wUnit6End;
    WORD wUnit7Head;
    MCSUSBTCParamStruct MCSUSBTCParam7;
    WORD wUnit7End;
    WORD wUnit8Head;
    MCSUSBTMParamStruct MCSUSBTMParam8;
    WORD wUnit8End;
//    WORD wUnitBHead;
//    MCSUSBAudioParamStruct MCSUSBAudioParamB;
//    WORD wUnitBEnd;
    WORD wUnitCHead;
    MCSUSBTMSimulParamStruct MCSUSBTMSimulParamC;
    WORD wUnitCEnd;
//    WORD wUnitFHead;
//    MCSDDTRevParamStruct MCSDDTRevParamF;
//    WORD wUnitFEnd;
//    WORD wUnit10Head;
//    MCSDDTTMIParamStruct MCSDDTTMIParam10;
//    WORD wUnit10End;
//    WORD wUnit11Head;
//    MCSDDTTMQParamStruct MCSDDTTMQParam11;
//    WORD wUnit11End;
//    WORD wUnit12Head;
//    MCSDDTTMSimulParamStruct MCSDDTTMSimulParam12;
//    WORD wUnit12End;
//    WORD wUnit13Head;
//    MCSDDTReplayParamStruct MCSDDTReplayParam13;
//    WORD wUnit13End;
}USBTTCGroup;

typedef struct
{
    UINT TYPE;//信息类别
    UINT ID;//标识
    USHORT SEQ;//顺序号
    UCHAR CRESULT;//控制结果
}CCRESPONSE;//控制命令响应

typedef struct
{
    USHORT NUM;//顺序号
}SINQUIRY;// status inquiry　状态查询



#pragma pack()

#endif // DATASTRUCTTTCDEVCTLINTERFACE_H
