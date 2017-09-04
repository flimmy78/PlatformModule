#ifndef _PRAA_DEFINE_H
#define _PRAA_DEFINE_H

#include "TypeDefine.h"

#ifndef C_TCOPER_PARASET
//操控台参数设置命令命令字
#define C_TCOPER_PARASET 0x0200
#endif

#ifndef C_REMOTE_TCCMD
//远程安控发令命令命令字
#define C_REMOTE_TCCMD 0x0445
#endif

#ifndef C_TCOPER_KEYSET
//操控台按键操作命令命令字
#define C_TCOPER_KEYSET 0x0444
#endif

const WORD gc_wChangeObjCmd = 0x447;     //目标变换命令
//自环/小环比对
const  WORD   ClosedLoopResult_ItemID    =    0x0A15;

#pragma pack(1)

namespace ak {
//////////////////////////////////////////////////////////////////////////////////
//	改动时间: 2002-08-29
//--------------------------------------------------------------------------------
//	简要描述: 以下结构用于和指令编辑相关的信息元
//--------------------------------------------------------------------------------
//	   日志:
//////////////////////////////////////////////////////////////////////////////////
const  USHORT   TC_COMMAND_NAME_LENGTH  = 21;
const  USHORT   TC_COMMAND_LENGTH       = 128;
const  USHORT   TC_COMMAND_GROUP_LENGTH = 50;
const  USHORT   TC_COMMAND_OBJECT_NUM   = 10;
const  UINT     g_nMaxObjCount          = 10; //最大目标号
const  UINT     g_nMaxFreCnt = 10;

///////*每条指令验证逻辑参数结构*//////////
//本结构为42字节
typedef struct
{
	//**指令验证逻辑, 0--五判三,1--三判二,2--一判一 ,3--五判四,4--五判五,5--十判十,6--二十判二十,***1字节**//   
	//注：大环验证支持0~4，弹上关键参数参数验证支持0~6
	unsigned char           ucCheckLogic; 
	
	//**指令验证方式,0--不需要二次处理,1--需要二次处理***1字节**//
	unsigned char           ucCheckManner; 
	
	//**指令电平判决逻辑,0--低电平,1--高电平***1字节**// 
	unsigned char           ucHighLowLogic;
	
	//**最小电平分层值 ,0--20分层***1字节**// 
	unsigned short           ucLowAcceErrRange;
 

	//**指令所放的子帧波道号,***2字节**//
    USHORT                  ucPosition;

	//以下为2008.1.30添加//
	//**指令所放的子帧号(即全帧的第几个子帧) ,***2字节**// 
    USHORT                  ucFrameNum;

	//**挑路类型 ,0--从子帧挑,1--从全帧挑,2--从特殊副帧挑***1字节**// 
    unsigned char            Type;

	//------------------------------------------------------------
	//**最大电平分层值 ,0--20分层***1字节**// 
	unsigned short           ucHighAcceErrRange;

	//**是否选择位判方式 ,0:不需要，1:需要 ***1字节**// 
	unsigned char           ucIfBitJudage;

	//**输入位判的位置 ,  0~7位 ***1字节**// 
	unsigned char           ucBitPlace;
	
	//**保留字节 ,***28字节**//
	unsigned char			 ucDeServe[22];
	USHORT                   DeServe[3];
	
}TC_CHECK_COMMAND_LOGIC_STRUCT,*PTC_CHECK_COMMAND_LOGIC_STRUCT; 

typedef struct  
{
	//当前指令对应的波道数
	unsigned char            ucChnlNum; //为1时表示单组
	
	//本条指令逻辑关系
	unsigned char            CommandLogic;//0:或逻辑 1:与逻辑 2:单组 
	
	//**弹上关键参数指令代码,***1字节**//
    USHORT                   KeyCommandNo;
	
	//**弹上关键参数指令名称,***20字节****//
    unsigned char             KeyCommandName[21];
	
	//每一波道对应的参数
	TC_CHECK_COMMAND_LOGIC_STRUCT		   aChannelPara[5];
	
} TC_KEYPARA_COMMAND_LOGIC_STRUCT,*PTC_KEYPARA_COMMAND_LOGIC_STRUCT; 

//////*弹上验证设置参数结构*****大环设置参数结构*////////////  //20090317
//** 说明1:大环设置参数结构与弹上验证设置参数结构使用同一个结构          **//
//** 说明2:关键参数读取框架类型号0xA016;大环设置参数读取框架类型号0xA014 **//
const WORD gc_wBLConfigInfoUnit_TID = 0xA014; //大环验证设置
const WORD gc_wDanshangKeyConfigInfoUnit_TID = 0xA016; //弹上关键参数设置
const WORD gc_BLConfigInfoUnit_ItemID = 0x0A24;//大环验证参数信息元
const WORD gc_DanshangKeyConfigInfoUnit_ItemID = 0x0A23;//弹上关键参数信息元
typedef struct
{
	//**指令条数,范围0--127 ***1字节**//
	unsigned char                         ucCmdNum;   
	
	//流号;从0开始,0 代表流 1,依次类推
	unsigned char                         ucStreamNo;   	
	
	//**指令***42*127字节**//
	//TC_KEYPARA_COMMAND_LOGIC_STRUCT         ucCmdItem[127];
	TC_KEYPARA_COMMAND_LOGIC_STRUCT         ucCmdItem[30];
	
}TC_CHECK_SET_PARA_STRUCT; 

//////*关键参数设置每条指令结果结构*/////////
typedef struct
{
    //**验证结果,1：告警；0：无告警 ***1字节**//  
    BYTE                   ucCheckResult;
	//**参数代码 ***1字节**//
	BYTE                   ucCmdSymbol;
	//**时间 遥测时码格式***6字节**//
	BYTE                   ucCheckTime[6];
	
}KEY_CMD_ITEM_SAVE_STRUCT,*PKEY_CMD_ITEM_SAVE_STRUCT;

//////*大环每条指令结果结构*/////////
typedef struct
{
	//**指令代号,最大20***2字节**//
	USHORT                   ucCmdSymbol;

	//**验证时间 ***15字节**//
	unsigned char               ucCheckTime[15];
	
	//**验证结果,0--验证错误,1--验证正确***1字节**//                 
    unsigned char                   ucCheckResult;
	
}CMD_ITEM_SAVE_STRUCT,*PCMD_ITEM_SAVE_STRUCT;


////////*大环验证结果参数结构*/////////
const  WORD   BigLoopResult_ItemID       =    0x0A14;//大环
typedef struct
{	
	//**需要验证指令条数***1字节**//
	unsigned char               ucCheckCmdNum; 

	//**验证时间 ***15字节**//
	unsigned char               ucCheckTime[15];

	//**指令***40字节**//
	CMD_ITEM_SAVE_STRUCT	    ucCmdItems[20];

}TC_BIG_LOOP_CHECK_RESULT_STRUCT,*PTC_BIG_LOOP_CHECK_RESULT_STRUCT; 

////////*关键参数验证结果参数结构*/////////

//弹上结果信息元
const  WORD   DanShangResult_ItemID       =    0x0A22;//弹上验证结果

typedef struct
{	
	//**需要验证指令条数***1字节**//
	unsigned char               ucCheckCmdNum; 
	//**指令***40字节**//
	KEY_CMD_ITEM_SAVE_STRUCT	    ucCmdItems[126];
	
}TC_CHECK_RESULT_STRUCT;


//安控指令装订
const WORD gc_wSingleCommand_TID = 0x1C1A;//安控指令装订
struct  SinglePieceCommandStruct
{
	//0xDFDF
	CHAR    CommandName[TC_COMMAND_NAME_LENGTH];
	USHORT  CommandLength;
	CHAR    CommandData[TC_COMMAND_LENGTH];
	BYTE    CommandType;
} ;

////安控指令装订状态
const  WORD TC_EDITCOMMAND_ITEMID = 0x1C19;
struct  GroupCommandStruct
{
	//0x1C19
	SinglePieceCommandStruct  SingleCommand[TC_COMMAND_GROUP_LENGTH];
};

struct TC_SendFrequencyConfigStruct
{
	BYTE     ValidFlag;
	UINT     nFreqValue;
	CHAR     FreqName[100];
};

//频率设置参数
const WORD gc_wFreCfgTID = 0x1406;
struct TC_FRECFG_STRUCT
{
	//0x1406
	UINT nFreCnt;  //频率个数，最大不能超过g_nMaxFreCnt，暂时不用
	UINT nReloadCnt; //加载计数，频率变化时改变
	TC_SendFrequencyConfigStruct TcFreCfg[g_nMaxFreCnt];
};

//综合时间信息元  F0H
typedef struct
{
    short       SelfSize;           //本结构的大小--20
    UCHAR       AbsoluteTime[6];    //绝对时间
    long        RelativeTime;       //相对时间
    UCHAR       FlyStartTime[6];    //起飞零时间 
    UCHAR       FlayStartTimeFlag;  //起飞零有效标记  0-无效，1-有效
    UCHAR       TimeCodeSelect;     //时间选择标记  0-时码器，1-时码板，2-外测时间，3-系统时间
}GlobleTimeCodeCellStruct;

//遥测子帧时间
typedef struct
{	
	WORD tenthMillisecond : 4;  //0.1毫秒
	WORD aMillisecond     : 4;  // 1毫秒
	WORD tenMillisecond   : 4;  //10毫秒
	WORD hunMillisecond   : 4;  //100毫秒
	
	WORD aSecond          : 4;   //1秒
	WORD tenSecond        : 4;   //10秒
	WORD aMinute          : 4;   //1分
	WORD tenMinute        : 4;   //10分
	
	WORD aHour            : 4;   //1时
	WORD tenHour          : 2;   //10时  
	WORD aDay             : 4;   //1天
	WORD tenDay           : 4;   //10天
	WORD HunDay           : 2;   //100天
}TIMEDATA_STRUCT, *PTIMEDATA_STRUCT;

//L频段下变频器工作参数设置块结构
const WORD gc_wDC_TID = 0x13FF;//下变频器工作参数
typedef struct
{   //L下变频器：0x13FF
	USHORT usOnLine;			// 在线标志，0-在线 1-不在线
	USHORT usPowOut;			// 是否向LNA馈电，0-不馈电 1-馈电
	UINT   nIFOutFreq;			// 输出中频频率，L下变频器该值范围：70MHz±5MHz，缺省值为70 MHz，量化单位：Hz
	UINT   nRFInFreq;			// 输入射频频率，L下变频器该值范围：1750MHz～1850MHz，量化单位：Hz
	SHORT  sAtten1;				// 数控衰减器1衰减量，0~60dB
	SHORT  sAtten2;				// 数控衰减器2衰减量，0~44dB
	SHORT  sRefSel;				// 参考选择0x0000=内参考，0x0001=外参考
	UINT   nUpdateFlag;			// 设置有效标志，保留
	USHORT usAlterMode;			// 衰减模式，1--自动, 0--手动，此参数仅在实时处理衰减时用，不加载到硬件
}BBEDownAlterParaStruct, *LPBBEDownAlterParaStruct;

//L频段下变频器工作状态参数块结构
const WORD gc_wDC_ItemID = 0x1400;//下变频器工作状态
typedef struct
{   //L下变频器：0x1400
	UCHAR  ucControlMode;			// 控制模式，00-分控 01-本控
	UCHAR  ucSelfCheckState;		// 重置计数  0~255循环计数
	UCHAR  ucLockStatuc;			// 锁定指示，00-失锁 01-锁定
	USHORT usAlterMode;				// 两级下变频器的变频模式:0x0000＝两变频器均为高差，1＝两变频器均为低差，
	                                // 2＝第一级变频器为高差，第二级变频器为低差3＝第一级变频器为低差，第二级变频器为高差
	UINT   nRFInFreq;				// 实际输入射频信号频率，量化单位：Hz
	BBEDownAlterParaStruct ParaSet;	// 设置参数结构
}BBEDownAlterStatusStruct, *LPBBEDownAlterStatusStruct;

//L频段上变频器工作参数设置块结构
const WORD gc_wUC_TID = 0x13FD;//上变频器工作参数
typedef struct
{   //L上变频器：0x13FD
	USHORT usOnLine;				// 在线标志，0-在线 1-不在线
	UINT   nInFreq;					// 输入中频频率，L上变频器该值范围：70MHz±10MHz，缺省值为70 MHz，量化单位：Hz
	UINT   nRFOutFreq;				// 输出射频频率，L上变频器该值范围：1750～1850MHz，量化单位：Hz
	SHORT  sAtten1;				    // 数控衰减器衰减量，0~30dB
}BBEUpAlterParaStruct, *LPBBEUpAlterParaStruct;

//L频段上变频器工作状态参数块结构
const WORD gc_wUC_ItemID = 0x13FE;//上变频器工作状态
typedef struct
{   //L上变频器：0x13FE
	UCHAR  ucControlMode;			// 控制模式，00-分控 01-本控
	UCHAR  ucSelfCheckState;		// 重置计数  0~255循环计数
	UCHAR  ucLockStatuc;			// 锁定指示，00-失锁 01-锁定
	BBEUpAlterParaStruct ParaSet;	// 设置参数结构
}BBEUpAlterStatusStruct, *LPBBEUpAlterStatusStruct;

// 馈电耦合器工作参数设置块结构
typedef struct
{   // 0x1D04
	USHORT wKD;					// 馈电标志，0-不馈电 1-左旋馈电 2-右旋馈电 3:全馈电
	UINT   InFreq;					// 输入频率，量化单位：Hz
	UINT   RFOutFreq;				// 输出射频频率
	
}KDParaStruct, *PKDParaStruct;

//馈电耦合器工作状态参数块结构
typedef struct
{   // 0x1D05
	UCHAR  ControlMode;				// 控制模式，00-本控 01-远控
	KDParaStruct ParaSet;	// 设置参数结构
}KDStatusStruct, *PKDStatusStruct;

//指令数据信息元，此信息元为数据信息元（100块），由控制数据逻辑解释模块生成，安控发令模块读取使用
typedef struct
{   //0x1405
	UCHAR ucSendManner;             //发令方式 0：本地发令 1：远程发令
    UCHAR ucActType;                //动作类型 0：指令 1：停发
    WORD  wObjectNo;                //目标号
    UINT  nCmdIndex;                //指令索引，标识目标的第几个指令，从0开始
    UCHAR szCmdName[20];            //指令名称
    WORD  wCmdFrameLen;             //指令帧长度
    UCHAR szCmdFrameContent[128];   //指令帧内容
    UCHAR szRsvd[32];               //保留
}TC_PENDINGCMD_STRUCT, *PTC_PENDINGCMD_STRUCT;

//发令状态数据信息元，此信息元为数据信息元（500块），由安控发令模块生成，监控等模块读取使用
//////////////实时状态巡捡上报接口//////////////////
const  WORD   StatusReport_ItemID =  0x1403;//状态上报
typedef  struct CtledSendCmdInterface
{   //0x1403
	UCHAR ucSendManner;             //发令方式 0：本地发令 1：远程发令
    UINT  nTotalCmdNum;             //本次程序启动后发令总条数（含本次）
    UINT  nLocalCmdNum;             //本次程序启动后本地发令总条数（含本次）
    WORD  wObjectNo;                //目标号
    UINT  nCmdIndex;                //指令索引，标识目标的第几个指令，从0开始
    UCHAR szCmdName[20];            //指令名称
    WORD  wCmdFrameLen;             //指令帧长度
    UCHAR szCmdFrameContent[128];   //指令帧内容
	UCHAR ucSendStatus;             //发令状态 0：停止发令; 1：正在发令
    UCHAR szSendCmdTime[15];        //发令的开始时刻
    UCHAR ucBigLoopResult;          //大环比对结果 0：比对进行中; 1：比对正确； 2 比对超时
    UCHAR szBigLoopTime[15];        //大环比对结果时间
    UCHAR ucSmallLoopResult;        //小环比对结果 0：比对进行中; 1：比对正确； 2 比对超时
    UCHAR szSmallLoopTime[15];      //小环比对结果时间
    UCHAR szRsvd[32];               //保留
}TC_PROCESSCMD_STRUCT, *PTC_PROCESSCMD_STRUCT;

//BBE时码单元设置结构
const WORD gc_wTIMEUNIT_TID = 0xE626;//时码单元设置
typedef struct
{   //0xE626
	UCHAR	ucWorkMode;			    //内/外时统选择，8位无符号整型数。D7 ~ D0位：00H表示内时统；01H表示外时统。缺省值为外时统。
    UCHAR	ucReserved;             //保留
	UCHAR	ucLeap;				    //闰年,00H表示否；01H表示是,仅在"内时统"时有效
	UCHAR	ucReserved1;            //保留
	UCHAR	ucSecond;			    //秒，8位无符号整型数，量化单位为1s，取值范围：0 ~ 59，仅在内时统时有效。
	UCHAR	ucMinute;			    //分，8位无符号整型数，量化单位为1min，取值范围：0 ~ 59，仅在内时统时有效。
	UCHAR	ucHour;				    //时，8位无符号整型数，量化单位为1hour，取值范围：0 ~ 23。仅在内时统时有效。
    UCHAR	ucReserved2;            //保留
	USHORT	usDay;				    //天，16位无符号整型数，量化单位为1day，取值范围：1 ~ 365或366（闰年）。仅在内时统时有效。
	UCHAR   ucReserved3[2];         //保留
	ULONG	ulReserved[4];          //保留
}TC_TIMEUNIT_PARA_STRUCT, *PTC_TIMEUNIT_PARA_STRUCT;

//安控操控台状态结构
typedef struct
{   //0x13FC
	UCHAR ucControlMode;            //控制模式，00-本控 01-远控
    UCHAR ucCmdLampState;           //安控操控台发令灯状态，每个BIT代表含义如下：
                                    //BIT0-0 ：K1发令灯灭; 1 ：K1发令灯亮
                                    //BIT1-0 ：K2发令灯灭; 1 ：K2发令灯亮
                                    //BIT2-0 ：K3发令灯灭; 1 ：K3发令灯亮
                                    //BIT3-0 ：K4发令灯灭; 1 ：K4发令灯亮
                                    //BIT4-0 ：K5发令灯灭; 1 ：K5发令灯亮
	                                //其余BIT保留
    UCHAR ucReturnLampState;        //安控操控台回令灯状态，每个BIT代表含义如下：
                                    //BIT0-0 ：K1回令灯灭；1 ：K1回令灯亮
                                    //BIT1-0 ：K2回令灯灭；1 ：K2回令灯亮
                                    //BIT2-0 ：K3回令灯灭；1 ：K3回令灯亮
                                    //BIT3-0 ：K4回令灯灭；1 ：K4回令灯亮
                                    //BIT4-0 ：K5回令灯灭；1 ：K5回令灯亮
                                    //其余BIT保留
	UCHAR ucWarnLampState;          //安控操控台告警灯状态，每个BIT代表含义如下：
    UCHAR ucStatusFlag;             //安控操控台工作情况标记：1 正常，其余待定
	UCHAR ucLoopLampState;          //安控操控台小环灯状态，每个BIT代表含义如下：
                                    //BIT0-0 ：小环红灯灭；1 ：小环红灯亮
                                    //BIT1-0 ：小环绿灯灭；1 ：小环绿灯亮
                                    //其余BIT保留
    UCHAR ucReserve[2];             //保留
}TCOPER_STATUS_STRUCT, *PTCOPER_STATUS_STRUCT;

//安控操控台到安控终端的按键操作数据结构
typedef struct
{
	UCHAR ucKeyState;               //安控操控台按键操作状态：0 ：按下RESET键；1 ：按下K1键；2 ：按下K2键；3 ：按下K3键；4 ：按下K4键；5 ：按下K5键
    UCHAR ucReserve[7];             //保留
}TCOPER_KEYSTATE_STRUCT, *PTCOPER_KEYSTATE_STRUCT;

//安控终端到的安控操控台的参数设置数据结构
typedef struct
{
	UCHAR ucCmdLampState;           //设置安控操控台发令灯状态，每个BIT代表含义如下：
                                    //BIT0-0 ：灭K1发令灯；1 ：点K1发令灯
                                    //BIT1-0 ：灭K2发令灯；1 ：点K2发令灯
                                    //BIT2-0 ：灭K3发令灯；1 ：点K3发令灯
                                    //BIT3-0 ：灭K4发令灯；1 ：点K4发令灯
                                    //BIT4-0 ：灭K5发令灯；1 ：点K5发令灯
                                    //其余BIT保留
    UCHAR ucReturnLampState;        //设置安控操控台回令灯状态，每个BIT代表含义如下：
                                    //BIT0-0 ：灭K1回令灯；1 ：点K1回令灯
                                    //BIT1-0 ：灭K2回令灯；1 ：点K2回令灯
                                    //BIT2-0 ：灭K3回令灯；1 ：点K3回令灯
                                    //BIT3-0 ：灭K4回令灯；1 ：点K4回令灯
                                    //BIT4-0 ：灭K5回令灯；1 ：点K5回令灯
                                    //其余BIT保留
	UCHAR ucWarnLampState;          //设置安控操控台告警灯状态，0 ：灭告警灯 1 ：点告警灯
	UCHAR ucLoopLampState;          //设置安控操控台小环灯状态，每个BIT代表含义如下：
                                    //BIT0-0 ：灭小环红灯；1 ：点小环红灯
                                    //BIT1-0 ：灭小环绿灯；1 ：点小环绿灯
                                    //其余BIT保留
    UCHAR ucReserve[4];             //保留
}TCOPER_PARASET_STRUCT, *PTCOPER_PARASET_STRUCT;

//上行调制参数
const WORD gc_wMODU_TID = 0x1402;    //上行调制参数
const WORD gc_wZZMMODU_TID = 0xE564; //上行调制参数
typedef struct
{   //0x1402 //0xE564
    BYTE    byMode;                 //8位无符号整型数。Bit7～Bit0表示工作方式，00H表示任务方式；01H表示自检方式（任务模式下，调制器去调去载）。
    BYTE:8;                         //保留
    WORD:16;                        //保留
    DWORD   dwCarRF;                //上行载波频率，32位无符号整型数，量化单位：1 Hz，取值范围为2200M ~ 2400 MHz。
    DWORD   dwCarIF;                //输出载波中频，32位无符号整型数，量化单位：1 Hz，取值范围为68.5 M ~ 71.5 MHz，缺省为70 MHz。
    BYTE:8;                         //保留
    CHAR    cLevel;                 //输出功率，8位有符号整型数，量化单位：1 dBm，取值范围：－50 ~-10 dBm，缺省为－30 dBm。
    BYTE    byOnline;               //0-去载 1-加载 软件环境退出时设置为去载
    BYTE    byModu;                 //0-去调 1-加调
    SHORT   stOffset;               //多普勒频偏预置(kHz) [-250, 250] 缺省0 2311保留不用
    BYTE:8;                         //保留
    BYTE:8;                         //保留
    DWORD   dwRsv1;                 //保留
    BYTE    byNoiseSw;              //噪声开关，8位无符号整型数，仅在自检工作方式时有效。D7 ~ D0位：00H表示关；01H表示开。缺省值为关 2311保留不用。
    BYTE    byCN0;                  //输出载噪比，8位无符号整型数，量化单位：1 dBHz，取值范围：30 ~ 82 dBHz。缺省值为78dBHz 2311保留不用。
    BYTE:8;                         //保留
    BYTE:8;                         //保留
    WORD:16;                        //保留
    WORD:16;                        //保留
    BYTE    bySimSw;                //多普勒动态模拟开关，8位无符号整型数，D7 ~ D0：00H表示关；01H表示开。缺省值为关 2311保留不用。
    BYTE:8;                         //保留
    WORD    wSimRange;              //多普勒变化范围，16位无符号整型数，量化单位：1 kHz，取值范围：0 ~ 250 kHz，缺省为250 kHz 2311保留不用。
    WORD    wSimRate;               //多普勒变化率，16位无符号整型数，量化单位：1 Hz/s，取值范围：0 ~ 15 kHz/s，缺省为0 Hz/s 2311保留不用。
    BYTE:8;                         //保留
    BYTE:8;                         //保留
    BYTE:8;                         //保留
    BYTE:8;                         //保留
    BYTE:8;                         //保留
    BYTE:8;                         //保留
    BYTE:8;                         //保留
    BYTE:8;                         //保留
    BYTE:8;                         //保留
    BYTE:8;                         //保留
    DWORD   dwTmIndex;	            //调制频偏，32位无符号整型数，量化单位：1Hz，取值范围：4kHz ~ 200kHz。
    DWORD   adwRsv1[2];             //保留
}TC_MODU_PARA_STRUCT, *PTC_MODU_PARA_STRUCT;

//上行调制状态 FM为0x1401 主字母为0xE563
typedef struct TTCBBEModuStatusBlk
{   //0x1401 //0xE563
	TC_MODU_PARA_STRUCT Param;	    //
	/*UCHAR  ucLockStatuc;			// 锁定指示，00-失锁 01-锁定
	UCHAR ucReserved1;	            //
	UCHAR ucTcToneSend;	            //不使用 － 遥控/侧音发送状态，8位无符号整型数，须从调制器硬件板采集，仅在遥控/侧音分时发送时有效。D7~ D0位：00H表示发送侧音；01H表示发送遥控指令；02H表示停发侧音。
	UCHAR ucReserved2;	            //
	UINT  nDopNco;		            //上行多普勒频偏的NCO值，32位有符号整型数，须从调制器硬件板采集，量化单位：工作时钟/(2的32次方) Hz。
	UINT  nRsvd[4];*/	            //
	UINT  nRsvd[12];
}TTCBBEModuStatusStruct, *PTTCBBEModuStutasStruct;

//安控设置结构
//大小：81个32位字
const WORD gc_wTC_TID = 0x13BE;//安控设置结构
typedef struct  BBETCSetupBlk
{   //0x13BE
	USHORT		usCodeRate;		    //码速率，16位无符号整型数，量化单位：1 bps，范围：100~10kbps。
    UCHAR		ucCodeType;		    //码型，8位无符号整型数。D7 ~ D0位： 01H表示NRZ-L；02H表示NRZ-M；03H表示NRZ-S. 缺省值为NRZ-M。
	UCHAR		ucReserved1;	    //保留
	UCHAR		ucReserved2[2];	    //保留
	USHORT      usFrmLen;           //子帧长，16位无符号整型数，量化单位：1 字节，取值范围：8 ~ 1024 字节。它同时也是FPGA申请缓冲区数据长度 2311保留不用。（一个缓冲区一帧安控指令）
	UCHAR       ucFspLen;           //子帧同步码组长FspLen，8位无符号整型数，量化单位：1 位，取值范围：8 ~ 64 位。缺省值为16 2311保留不用。
	UCHAR		ucReserved3[3];     //保留3个字节
	DWORDLONG   dwlSynCode;         //子帧同步码组，64位无符号整型数。缺省值为EB90 2311保留不用。
	UCHAR       ucConvol;           //保留
	UCHAR       ucG2Inv;            //保留
	UCHAR	    ucReserved4[2];	    //保留
	//下面三个参数为遥控停发时保持的状态
	UCHAR       ucOnline;           //加载开关，8位无符号整型数，用于自动停发方式停发时，0表示去载，1表示加载。
	CHAR        cPNSw;              //副载波加调开关，8位无符号整型数，用于自动停发方式停发时， 0表示去调，1表示加调。
	UCHAR       ucDataSw;           //加调数据开关，8位无符号整型数，用于自动停发方式停发时， 0表示去调，1表示加调。
	UCHAR	    ucReserved5;        //保留1个字节
	UINT        nSubwave_rate;      //副载波频率，取值范围：4kHz~50kHz。
    UINT        nReserved6[9];      //保留9个32位字
	UCHAR	    ucReserved7[256];   //保留256字节
}BBETCSetupStruct,*PBBETCSetupStruct;

//安控命令
typedef struct
{   //0x13C0
	UCHAR       ucSwitch;           //发令控制，8位无符号整型数，0XFF表示开始发令；0X33表示停止发令。
	UCHAR       ucOnline;           //加载开关，8位无符号整型数，0表示去载，1表示加载 2固定设置为1。
	UCHAR       ucSubwave_Modu;     //副载波加调开关，8位无符号整型数， 0表示去调，1表示加调 2311保留不用。
	UCHAR       ucDataModu;         //加调数据开关，8位无符号整型数， 0表示去调，1表示加调 2311保留不用。
	UINT        nCmdSel;            //指令类型选择，32位无符号整型数，0：告警 1：解锁 2 炸毁 3 复位 4 自检
	                                //注意当选择炸毁时，炸毁指令初发后，自动复发解锁，炸毁参数CmdNumP次
	UINT        nCmdNumK;           //指令条数，32位无符号整型数，量化单位：1 条，0XFFFFFFFF表示命令停发方式，即上位机控制停发；0表示无意义；其它为自动停发方式，即FPGA根据指令条数自动停发。
	UINT        nCmdNumP;           //解锁炸毁交替发送方式下的P次
	UINT        nCmdTime;           //指令发送间的保护时间间隔，取值范围：100ms ~ 300ms
	//20150106 yan 修改 
	ULONG   ulB000_L32;             //0.1ms时码格式
	USHORT  usB000_H16;             //D3 ~ D0位：量化单位为0.1ms；
	//D7 ~ D4位：量化单位为1ms；
	//D11 ~ D8位：量化单位为10ms；
	//D15 ~ D12位：量化单位为100ms；
	//D19 ~ D16位：量化单位为1s；
	//D23 ~ D20位：量化单位为10s；
	//D27 ~ D24位：量化单位为1min；
	//D31 ~ D28位：量化单位为10min；
	//D35 ~ D32位：量化单位为1hour；
	//D37 ~ D36位：量化单位为10hour；
	//D41 ~ D38位：量化单位为1day；
	//D45 ~ D42位：量化单位为10day；
	//D47 ~ D46位：量化单位为100day。
	USHORT	usT25us;                //25us计数,16位无符号整型数，表示1秒内的时间，量化单位：25 us。
	//end
	UINT        nReserved1[3];      //保留3个32位字
}TCConctrolStruct; 

//安控指令设置
const WORD gc_wSingleCommandSet_TID = 0x1E06; //安控指令设置
struct  SinglePieceCommandSetStruct
{
	CHAR       szCmdData[64];       //指令内容
	UINT       nCmdLen;             //指令长度, 0~512
	UINT       ucFspLen;            //同步码组长FspLen，量化单位：1 位，取值范围：1 ~ 64 位。缺省值为16。
	UINT       ucFspPlace;          //同步码组位置，0：帧首 1：帧尾。
	DWORDLONG  dwlSynCode;          //子帧同步码组，64位无符号整型数。缺省值为EB90。
	UINT       nReserved1[11];      //保留11个32位字
};

typedef struct _CmdSetStruct
{   //0x1E06
	SinglePieceCommandSetStruct CmdSet[5]; //指令内容 0：告警 1：解锁 2 炸毁 3 复位 4 自检
}CMDSETSTRUCT, *PCMDSETSTRUCT;

//主字母安控指令设置
const WORD gc_wZZMCommandSet_TID = 0x1E0A; //主字母安控指令设置

union SingleZZMCmdSet
{
	WORD       wCmdData;       //指令内容
	struct
	{
		WORD wBIT1:1;  //对应第1个单音 0：不选择 1：选择
		WORD wBIT2:1;  //对应第2个单音 0：不选择 1：选择
		WORD wBIT3:1;  //对应第3个单音 0：不选择 1：选择
		WORD wBIT4:1;  //对应第4个单音 0：不选择 1：选择
		WORD wBIT5:1;  //对应第5个单音 0：不选择 1：选择
		WORD wBIT6:1;  //对应第6个单音 0：不选择 1：选择
		WORD wBIT7:1;  //对应第7个单音 0：不选择 1：选择
		WORD wBIT8:1;  //对应第8个单音 0：不选择 1：选择
		WORD wBIT9:1;  //对应第9个单音 0：不选择 1：选择
		WORD wBIT10:1; //对应第10个单音 0：不选择 1：选择
		WORD wBIT11:1; //对应第11个单音 0：不选择 1：选择
		WORD wBIT12:1; //对应第12个单音 0：不选择 1：选择
		WORD wBIT13:1; //对应第13个单音 0：不选择 1：选择
		WORD wBIT14:1; //对应第14个单音 0：不选择 1：选择
		WORD wBIT15:1; //对应第15个单音 0：不选择 1：选择
		WORD wBIT16:1; //固定填0
	};
};

struct  SingleZZMCmdSetStruct
{
	SingleZZMCmdSet singleZZM[8];
};

//导频和单音频率设置
//大小：16个32位字
typedef struct  DPHDYPLSETBlk
{   //0x1E08
	UCHAR ucTimef0;                  //导频时间， 单位1mx，缺省值5ms，此参数界面不显示，固定设5ms
	UCHAR ucTimefx;                  //单音时间， 单位1mx，缺省值5ms，此参数界面不显示，固定设5ms
	UCHAR ucLetterNum;               //单条指令的字母个数， 缺省值为7，此参数界面不显示，固定设7
	UCHAR ucReserved1;	             //保留

	UINT       nF0Freq;             //导频频率,单位1Hz,范围5kHz~12kHz,1Hz可设,缺省值5.12kHz
	UINT       nF1Freq;             //第一单音频率,单位1Hz,范围5kHz~12kHz,1Hz可设,缺省值6.4kHz
	UINT       nF2Freq;             //第二单音频率,单位1Hz,范围5kHz~12kHz,1Hz可设,缺省值6.8kHz
	UINT       nF3Freq;             //第三单音频率,单位1Hz,范围5kHz~12kHz,1Hz可设,缺省值7.2kHz
	UINT       nF4Freq;             //第四单音频率,单位1Hz,范围5kHz~12kHz,1Hz可设,缺省值7.6kHz
	UINT       nF5Freq;             //第五单音频率,单位1Hz,范围5kHz~12kHz,1Hz可设,缺省值8.0kHz
	UINT       nF6Freq;             //第六单音频率,单位1Hz,范围5kHz~12kHz,1Hz可设,缺省值8.4kHz
	UINT       nF7Freq;             //第七单音频率,单位1Hz,范围5kHz~12kHz,1Hz可设,缺省值8.8kHz
	UINT       nF8Freq;             //第八单音频率,单位1Hz,范围5kHz~12kHz,1Hz可设,缺省值9.2kHz
	UINT       nF9Freq;             //第九单音频率,单位1Hz,范围5kHz~12kHz,1Hz可设,缺省值9.6kHz
	UINT       nF10Freq;            //第十单音频率,单位1Hz,范围5kHz~12kHz,1Hz可设,缺省值10.0kHz
	UINT       nF11Freq;            //第十一单音频率,单位1Hz,范围5kHz~12kHz,1Hz可设,缺省值10.4kHz
	UINT       nF12Freq;            //第十二单音频率,单位1Hz,范围5kHz~12kHz,1Hz可设,缺省值10.8kHz
	UINT       nF13Freq;            //第十三单音频率,单位1Hz,范围5kHz~12kHz,1Hz可设,缺省值11.2kHz
	UINT       nF14Freq;            //第十四单音频率,单位1Hz,范围5kHz~12kHz,1Hz可设,缺省值11.6kHz
	UINT       nF15Freq;            //第十五单音频率,单位1Hz,范围5kHz~12kHz,1Hz可设,缺省值12.0kHz
	
	UINT  nReserved[30];            //保留30个32位字
	// 20150608 修改结束
}DPHDYPLSETBlkStruct,*PDPHDYPLSETBlkStruct;

struct  ZZMSetStruct
{   //0x1E0A
// 	UCHAR ucTimef0;                  //导频时间， 单位1mx，缺省值5ms，此参数界面不显示，固定设5ms
// 	UCHAR ucTimefx;                  //单音时间， 单位1mx，缺省值5ms，此参数界面不显示，固定设5ms
// 	UCHAR ucLetterNum;               //单条指令的字母个数， 缺省值为7，此参数界面不显示，固定设7
// 	UCHAR ucReserved1;//保留
// 	DPHDYPLSETBlkStruct DPBlkStruct;
// 	UINT  nReserved2[30];            //保留48个32位字
	SingleZZMCmdSetStruct CmsSet[5]; //指令内容 0：告警 1：解锁 2 炸毁 3 复位 4 自检

	UINT  nReserved3[14];            //保留14个32位字
};

//主字母安控发令状态
//大小：81+51个32位字
typedef struct  ZZMBBETCStatusBlk
{   //0x1E09
	ZZMSetStruct  TCSetPara;        //设置返回参数

	UCHAR      ucCmdSta;            //发令状态，8位无符号整型数，0XFF表示发令中；0X33表示未发令。
	UCHAR      ucCmdSel;            //正在发令的指令类型，8位无符号整型数，0：告警 1：解锁 2 炸毁 3 复位 4 自检
	WORD       wReserved1;          //保留2个字节
	UINT       nCmdCnt;             //发令计数，32位无符号整型数，表示本条安控指令的发令计数。
	UINT       nCmdNum;             //发令次数，32位无符号整型数，表示所发指令条数。
	UINT       nLoopCompareNum;     //小环比对指令数，32位无符号整型数，表示当前小环比对完的指令计数。
	UINT       nLoopErrorNum;       //小环比对误指令数，32位无符号整型数，表示小环比对错误的指令计数。
	UCHAR      ucLoopCompareResult; //小环比对结果，8位无符号整型数，表示比对结果，0xFF为正确，0xEE为错误, 0xCC表示输入信号功率<-30dBm。
	UCHAR      ucReserved[3];       //保留3个字节
	UINT       nSelfCompareNum;     //自环比对指令数，32位无符号整型数，表示当前自环比对完的指令计数。
	UINT       nSelfErrorNum;       //自环比对误指令数，32位无符号整型数，表示自环比对错误的指令计数。
	UCHAR      ucSelfCompareResult; //自环比对结果，8位无符号整型数，表示比对结果，0xFF为正确，0xEE为错误。
	UCHAR      ucReserved2[3];      //保留3个字节
	TIMEDATA_STRUCT CurTimeB000;    //当前时刻,48位无符号整形数,为0.1ms时码格式
	USHORT CurTime25us;             //当前时刻1s内的25us表示,16位无符号整形数,量化单位25us
	//20150106 yan 修改
	TIMEDATA_STRUCT SendTCCmdTimeB000;  //发送安控指令时间,48位无符号整形数,为0.1ms时码格式
	USHORT SendTime25us;//保留
	UINT		nReserved3[38];      //保留38个32位字
	// 20150608 修改结束
}ZZMBBETCStatusStruct,*PZZMBBETCStatusStruct;



//BBE时码单元状态结构
//      大小：4个32位字
typedef struct TimeUnitStatusBlk
{   //0xE625
	TC_TIMEUNIT_PARA_STRUCT TimeUnitPara; //设置返回参数
    
	UCHAR	ucLock;                 //锁定指示,8位无符号整型数，D7 ~ D0位：00H表示失锁；01H表示锁定
    UCHAR   ucReserved[3];          //保留
    ULONG   ulB000_L32;             //?????
	USHORT  usB000_H16;             //D3 ~ D0位：量化单位为0.1ms；
						            //D7 ~ D4位：量化单位为1ms；
						            //D11 ~ D8位：量化单位为10ms；
						            //D15 ~ D12位：量化单位为100ms；
						            //D19 ~ D16位：量化单位为1s；
						            //D23 ~ D20位：量化单位为10s；
						            //D27 ~ D24位：量化单位为1min；
						            //D31 ~ D28位：量化单位为10min；
						            //D35 ~ D32位：量化单位为1hour；
						            //D37 ~ D36位：量化单位为10hour；
						            //D41 ~ D38位：量化单位为1day；
						            //D45 ~ D42位：量化单位为10day；
						            //D47 ~ D46位：量化单位为100day。
	USHORT	usT25us;                //25us计数,16位无符号整型数，表示1秒内的时间，量化单位：25 us。
	ULONG	ulReserved;	            //保留4个字节
}TimeUnitStatusStruct, *PTimeUnitStatusStruct;

//安控发令状态
//大小：81+51个32位字
typedef struct  BBETCStatusBlk
{   //0x13BD
	BBETCSetupStruct  TCSetPara;    //设置返回参数

	UCHAR      ucCmdSta;            //发令状态，8位无符号整型数，0XFF表示发令中；0X33表示未发令。
	UCHAR      ucCmdSel;            //正在发令的指令类型，8位无符号整型数，0：告警 1：解锁 2 炸毁 3 复位 4 自检
	WORD       wReserved1;          //保留2个字节
	UINT       nCmdCnt;             //发令计数，32位无符号整型数，表示本条安控指令的发令计数。
	UINT       nCmdNum;             //发令次数，32位无符号整型数，表示所发指令条数。
	UINT       nLoopCompareNum;     //小环比对指令数，32位无符号整型数，表示当前小环比对完的指令计数。
	UINT       nLoopErrorNum;       //小环比对误指令数，32位无符号整型数，表示小环比对错误的指令计数。
	UCHAR      ucLoopCompareResult; //小环比对结果，8位无符号整型数，表示比对结果，0xFF为正确，0xEE为错误, 0xCC表示输入信号功率<-30dBm。
	UCHAR      ucReserved[3];       //保留3个字节
	UINT       nSelfCompareNum;     //自环比对指令数，32位无符号整型数，表示当前自环比对完的指令计数。
	UINT       nSelfErrorNum;       //自环比对误指令数，32位无符号整型数，表示自环比对错误的指令计数。
	UCHAR      ucSelfCompareResult; //自环比对结果，8位无符号整型数，表示比对结果，0xFF为正确，0xEE为错误。
	UCHAR      ucReserved2[3];      //保留3个字节
	TIMEDATA_STRUCT CurTimeB000;    //当前时刻,48位无符号整形数,为0.1ms时码格式
	USHORT CurTime25us;             //当前时刻1s内的25us表示,16位无符号整形数,量化单位25us
	//20150106 yan 修改
	TIMEDATA_STRUCT SendTCCmdTimeB000;  //发送安控指令时间,48位无符号整形数,为0.1ms时码格式
	USHORT SendTime25us;//保留
	UINT		nReserved3[38];      //保留38个32位字
	// 20150608 修改结束
}BBETCStatusStruct,*PBBETCStatusStruct;

//BBE硬件状态结构
typedef struct BBEModuDeviceStatusBlk
{   //调制 0xE469
	UCHAR	   ucVcoLock;		    //FPGA工作时钟锁定指示，8位无符号整型数，00H表示失锁；01H表示锁定。
	UCHAR	   ucRamTest;		    //内部RAM自检，8位无符号整型数。00H无效；0EH表示内部RAM读写异常；0FH表示内部RAM读写正常。
	UCHAR	   ucFpgaTest;	        //FPGA读写自检，8位无符号整型数，00H无效；0EH表示FPGA读写异常；0FH表示FPGA读写正常。
    UCHAR	   ucReserved;          //保留
	ULONG      ulMSCnt;		        //ms计数，32位无符号整型数，用于检测DSP工作时钟是否正常。
	UINT       nReserved1[6];       //保留
}BBEModuDeviceStatusStruct, *PBBEModuDeviceStatusStruct;

//存盘参数块结构
typedef struct
{
    ////  //F1H
	BYTE	ucStreamNum;
	BYTE	ucSaveEnabled;    ///0：停止  1：启动   2：暂停 3：恢复 4：保持
	BYTE	ucFileHeadFlag;   ///有无文件头标志		0：无文件头   1：有文件头
	BYTE	ucFileNameSpecified;   ///是否指定文件名		0：自动模式1，前缀有效（YCYS**） 
                                   ////  1：自动模式2，前缀有效（YYMMDD**） 2:指定文件名,前缀无效
	BYTE	ucSuccedFlag;    ///存盘成功标志		0：不成功   1：成功
	ULONG   ulFileSize;	      //文件大小字节数	4字节	双字
	ULONG   ulDiskFreeSize;	  //剩余空间字节数	4字节	双字
	BYTE    FileNamePre[80];     //存盘文件名前缀	80字节	字符串	以NULL表示结尾
	BYTE    FileName[248];     //存盘文件名	248字节	字符串	以NULL表示结尾
}DATA_SAVE_ENABLE,*PDATA_SAVE_ENABLE;


//信息报文首部
struct TCOMMANDHEAD
{
	//为应用标识号。
	//  发送控制命令时（T=01H），由发送方负责填写；
	//  发送控制命令的应答时（T=02H），由命令应答方从收到的控制命令帧中取出并进行回填；
	//  发送状态时（T=04H），由发送状态方负责填写；不用时保留填0。
	DWORD dwHandle;	
	
	//为信息报文帧序号，它是动态唯一标识某个信息报文帧的号，
	//  发送控制命令时（T=01H），由发送方负责填写；
	//  发送控制命令的应答时（T=02H），从收到的控制命令帧中取出并进行回填；
	//  发送状态时（T=04H），由发送状态方负责填写；
	//  在某个应用过程中，SEQ从0开始每发送一帧状态信息加1，溢出归0。
	DWORD dwSequence;
	
	//为任务代号，其具体值根据任务而定，不用时保留填0。
	BYTE M;
	
	//信息报文类型码
	//T    =01H 
	//  表示控制信息报文。接收到这类信息报文的应用程序将它当作命令对待，
	//  按命令的具体格式去解释其中的数据，按命令的具体含义去执行，并给对方回送对应的响应信息报文。
	
	//T    =02H
	//  表示响应信息报文。接收到这类信息报文的应用程序将它当作相应命令的响应对待，
	//  按命令响应的具体格式去解释其中的数据，并根据响应的具体结果决定作何处理。
	
	//T    =04H
	//  状态信息报文。接收到这类信息报文的应用程序将它当作状态对待，
	//  按状态的具体格式去解释其中的数据，并根据状态的具体结果决定作何处理。接收方对状态信息报文不进行应答。
	
	BYTE T;
	
	//为整个信息报文帧的长度，
	//信息报文首部（背景灰色）和报文数据区两部分的总长度，单位为字节，最大为32K字节。
	//由此用户所发送的每个信息报文长度不能超过32K字节。
	//接收方传输服务软件在完成一个信息报文的接收组装后，
	//可用信息报文长度与实际接收到的报文长度来检查信息报文接收的正确性。
	WORD L;
	
	//信息报文请求的传输服务有限级别
	//b7~b4 初始版本（0000）
	//b3~b0 优先级别
	//  0000：  普通
	//  0001：  实时
	//  0010：  紧急处理
	//  0011：  空闲
	BYTE P;
	
	//代码，提供信息报文类型的进一步信息，它与信息报文类型字段共同标识某一具体的信息报文
	WORD C;
	
	//附加字。通常保留不用，如果使用往往用来对信息报文作附加说明或用作代码扩充字。
	BYTE A;
	
	//信源，发送方设备号，表明本协议数据来源的设备。
	WORD S;
	
	// 信宿，接收方设备号，表明本协议数据送往的设备。
	//D 〉0：D为目的设备号。报文数据区DATA中的格式内容依据报文的类型自行约定使用，
	//       可以包括子包头，也可以直接定义数据内容。
	//D = 0：报文数据区DATA 中包含多个（最少一个）目的设备的数据，
	//       DATA中数据子包的包头再指出具体的设备（命令管理中心没有处理此种情况）。
	//D = 0xFFFF：为报文数据广播，对所有的设备都发送。
	WORD D;
	
	//保留字节。目前填0，供以后扩展使用。
	WORD R;
};

typedef struct SPackage {   //协议报文包的头结构
	BYTE M;  //任务号
	BYTE T;  //类型:1(命令)；2(响应);3(数据、暂不用);4(状态);
	WORD L;  //长度
	BYTE P;  //优先级
	WORD C;  //命令字
	BYTE A;  //附加字
	WORD S;  //来源分系统号
	WORD D;  //目的分系统号
	WORD R;  //保留字，一般为0
} SPACKAGE, *PSPACKAGE;

//协议头protocol
typedef struct TPackageItemHead
{
	WORD S;	//来源分系统号
	WORD D;	//目的分系统号
	WORD T;	//对于命令，代表命令类型
	WORD I;	///对于命令，同类命令的序号（从0开始）
	WORD O;	///对于命令，操作字，O =0表示对设备进行实际操作，O =1表示不操作
	WORD L;	///对于命令，数据区占用的字节数
}SPACKAGEITEMHEAD, *PSPACKAGEITEMHEAD;


//////////////////////////////////////////////////////////////////////////////////
//	改动时间: 2002-07-08
//--------------------------------------------------------------------------------
//	简要描述: 以下结构用于和当前目标号相关的信息元
//--------------------------------------------------------------------------------
//	   日志:
//////////////////////////////////////////////////////////////////////////////////
const WORD TC_CURRENTOBJECT_ITEMID = 0x0A18;//跟踪目标状态信息元

//跟踪目标设置
const WORD gc_wCurrentObject_TID = 0xDFF4;//跟踪目标设置
struct  CurrentObjectIDStruct
{
	//0xDFF4
	USHORT  ObjID;      //目标号
	ULONG   SetCount;   //设置计数
	
	USHORT  RadioModulSystem;  //射频调制体制   0：FM  1：PM
	USHORT  VideoModulSystem;  //视频调制体制   0：PSK 1：FSK  2: 4FSK
};


//////////////////////////////////////////////////////////////////////////////////
//	改动时间: 2002-08-27
//--------------------------------------------------------------------------------
//	简要描述: 实时巡检上报的结果用于点亮大环、小环灯；大小环结果用于显示表格中的
//            结果
//--------------------------------------------------------------------------------
//	   日志:
//////////////////////////////////////////////////////////////////////////////////
/////////////发令后小环，大环结果上报/////////////// (状态信息元)
const  WORD   SmallBigReslutReport_ItemID =  0x0A20;//状态上报

typedef struct
{
	UCHAR             Result;//0--无效；1--结果正确，2--结果错误
	UCHAR             ucCheckTime[15];//得出结果的时间
}Check_Result;


////////////////发令方式信息元////////////////////////////
const  WORD  CmsSendMode_ItemID = 0x0A26;

typedef struct
{
	BYTE     flag;   //有效标志0：无效 1：有效
	BYTE     mode;   //发令方式0：本控(站发) 1：远控(台发) 2：远控机发
	ULONG    ResetNum;  //按下RESET键的次数，影响界面显示
	BYTE     WarningStatus;  //告警标志，1：有告警；0：无告警
}CommandMode_Status;


typedef  struct
{
	//控制方式 0:时间控制; 1:条数控制;下列以T为前缀的表示时间控制,P为条数控制
	unsigned char  ControlManner;  //dws 20020729 控制方式可手动改变
	
	//时间控制方式下发令方式 0:连续发令;1:时间符合发令
	unsigned char  TSendOrderManner;			
	
	//条数控制方式下发令方式 0:连续发令;1:手动单发;2:自动单发;
	unsigned char  PSendOrderManner;
	
	//炸毁指令的逻辑,0：单一炸毁(炸毁指令前不需解锁指令) 1：解保炸毁(炸毁指令前需解锁指令)
	unsigned char  DestroyOrderLogic;
	
	//停发逻辑
	//时间控制方式下表示限时停发的限时时间(s);
	int  TStopTimeLimit;
	//条数控制方式下表示条数停发的条数
	int  PStopPieceLimit;
	
	//时间间隔 ms
	int            TimeInterval;
	
	//载波保持逻辑 0:载波关闭，1：载波保持
	unsigned char  CarrierWaveKeepLogic; 
	
}  FC_ParaStruct, *pFC_ParaStruct;


const  WORD gc_FCConfigInfoUnit_ItemID	= 0x0A19; //流程控制状态信息元
//流程控制参数
const  WORD gc_wFCConfigInfoUnit_TID = 0xA015; //流程控制参数
typedef  struct
{
	int			   TargetNum;		//目标数,恒定为10
	FC_ParaStruct  FCParaArray[10];	//对应目标数的流程控制参数设置
} FC_ParaArray, *pFC_ParaArray;


//自动功率控制参数
//参数1776   信息元1775
const WORD gc_wAutoPowerCtrlPara_TID = 0x1776;
typedef struct
{
	WORD wAutoPower; //0-自动 0xFF-手动
	WORD wMorePower; //功率计算富裕量 [5,10]
	UCHAR ucRev[12];
}AutoPowerControlPara, *LPAutoPowerControlPara;

//自动功率控制状态
//信息元 0x1775
const WORD gc_wAutoPowerCtrlStatus_TID = 0x1775;
typedef struct
{
	//控制参数
	AutoPowerControlPara tControlPara;

	//站址坐标
	DOUBLE     PosX;
	DOUBLE     PosY;
	DOUBLE     PosZ;

	//上行点频
 	UINT   nRFOutFreq;				// 输出射频频率，L上变频器该值范围：1750～1850MHz，量化单位：Hz

	//数据源
	WORD wDataSource;  //0-无数据源 1-自主外测 2-GPS外测/转发外测

	//自动功率控制状态
	WORD wAutoPowerControlStatus;   //0-正常
	                                //1-无数据源
									//2-无上行点频
									//3-无站址坐标
									//4-正在发令中，无法控制

	//保留
	UCHAR ucRev[28];
}AutoPowerControlStatus, *LPAutoPowerControlStatus;

//密码
const WORD gc_wPasswordCommand_TID = 0xDFDF;//安控指令装订密码 //0xDFDF SN=0xFFFF
const WORD gc_wDisPlayCommand_TID = 0x8000;//安控指令明文显示 //0x8000 SN=0xFFFF
struct  TeleCtrlPasswordStruct
{
	UINT    PasswordLen;
	CHAR    Password[15];
} ;


//安控设备机器工作状态
const WORD gc_wAKDeviceWorkStatus_TID = 0x177D;
typedef struct
{
	BYTE ucAKStatus;  //安控设备机器工作状态 0：正常  1：异常
	union
	{
		DWORD dwErrReason;//设备异常原因
		struct
		{
			DWORD dwUc       :1; //上变频器未连通
			DWORD dwDc       :1; //下变频器未连通
			DWORD dwFreq     :1; //上变频器输出射频和下变频器输入射频不一致
			DWORD dwTcOper   :1; //安控操控台未连通
			DWORD dwModul    :1; //安控硬件设备(调制器)未连通
			DWORD dwTc       :1; //安控硬件设备(模拟源)未连通
			DWORD dwFrmLen   :1; //设置的指令帧长和编辑的指令长度不一致
			DWORD dwAutoPower:3; //自动功率不正常
						         //1-无数据源
								 //2-无上行点频
			                     //3-无站址坐标
			                     //4-正在发令中，无法控制
			DWORD dwVcoLock  :1; //FPGA工作时钟锁定
			DWORD dwRamTest  :1; //内部RAM自检
			DWORD dwFpgaTest :1; //FPGA读写自检
		};
	};

}TAKDeviceWorkStatus, *LPAKDeviceWorkStatus;


//发令回令状态信息元
const WORD gc_wSendRcvCmdStatus_TID = 0x177F;
typedef struct
{
	UCHAR ucK0;                    //1-有告警 0-无告警 

	//发令
	UCHAR szSendCmdTime[15];        //发令的开始时刻
	UCHAR ucKnSend[5]; //1-发令成功 0-没有发令 

	//回令
	UCHAR szBigLoopTime[15];        //大环比对结果时间
	UCHAR ucKnReturn[5];//0－无  1-比对正确 2－比对错误 3－正在发令

	//小环
	UCHAR szSmallLoopTime[15];        //小环发令的开始时刻
	UCHAR ucKnValid[5]; //0－无  1-比对正确 2－比对错误 3－正在发令

	//关键参数(只显示四个)
	int iKeyCmdNum;
	char KeyCmdName[4][21]; //关键参数名称
	UCHAR ucKeyCmdResult[4]; //1-有告警 0-无告警 
}TSendRcvCmdStatus, *LPSendRcvCmdStatus;


///// 转发外测到天控遥控打包数据
const WORD gc_wZftoTkYkStatus_TID = 0x1791;
struct TZftoTkYk			// 共 33 Bytes
{
	UCHAR		nB;			// 0x77
	USHORT	nHead;		// '$''Y'
	
	UINT	nT;			// 时间，（单位：ms），北京时间，其值为：
	//	（小时*3600 + 分钟*60 + 秒）* 1000
	
	union				// 状态码
	{
		struct
		{
			UCHAR nDev	: 2;// 00：设备故障，暂时不能参加任务
			// 01：设备正常，但是未跟踪目标或丢失目标
			// 10：数据干扰
			// 11：位置数据有效
			UCHAR nGnss	: 1;// 0：GNSS转发器方案	1：GNSS接收机方案
			
			UCHAR nGdop	: 1;// 0：GDOP值不好		1：GDOP值好
			UCHAR nPdop	: 1;// 0：PDOP值不好		1：PDOP值好
			UCHAR nV		: 1;// 0：测速结果无效		1：测速结果有效
			UCHAR nRsv	: 2;// 保留
		};
		BYTE	nS;
	};
	
	double		dR;			// 距离
	float		fAzi;		// 方位角
	float		fEle;		// 仰角
	float		fRRate;		// 距离变化率
	float		fDoppler;	// 多卜勒
	
	USHORT	nTail;		// '\n''\r'
};

//发送到遥控的自主外测数据缓冲区：
//发送到遥控的自主外测数据保存于环形缓冲的数据信息元中。
//缓冲区深度为200（一秒20个点，缓冲深度为10秒）。
//缓冲区带有1024字节的数据头，其定义同DataBufferHeadStruct。
//发送到遥控的自主外测数据结构定义如下：
const WORD gc_wKPtoTkYkStatus_TID = 0x1792;
struct  TKPtoYKstruct
{
	////E01BH
	BYTE           ucHead1;   //字头 EB   （BCD码）
	BYTE           ucHead2;   //字头 90    （BCD码）
	
	DWORD          dwT;//绝对时
	union
	{
		BYTE	   ucS;  //TCK自主外测设备状态码
		struct
		{
			//00 设备故障，暂不能参加试验任务，待机  
			//01 设备正常，尚未跟踪目标或丢失目标
			//10 自跟踪  
			//11 自跟踪，且R有效  
			BYTE  b1b0  :2 ;     
			
			//0 非自跟踪
			//1 自跟踪
			BYTE b2		:1 ;  //来自天控状态码
			
			// 00 自动跟踪方式
			// 01 数字引导
			// 10 手动跟踪
			// 11 记忆跟踪方式
			BYTE b4b3    :2;   //来自天控状态码
			
			// 000 测速数据有效
			// 001 径向距离速度 R' 数据有效
			// 010 无效   
			// 011 无效
			BYTE b7b6b5   :3;
		};
	};
    FLOAT          fAzi;      //方位角  从天控来 
	FLOAT	       fElv;      //仰角   从天控来
	DOUBLE	       dRadial;   //测距结果（单位：米）  从自主外测来
	FLOAT	       fRspeed;   //测速结果（径向速度）（单位：米/秒） 从自主外测来
	BYTE           ucCheckSumLowByte;  //校验和低字节 （除了这个字节，从第一字节开始依次加起来）
};

struct TDataBufferHead
{
	union
	{
		struct 
		{
			WORD  wHeadLength;         // 数据头长度=1024   
			DWORD dwBufferLength;      // 缓冲区长度（字节）（不含数据头）   
			UCHAR ucInfo[50];          // 描述信息   	          
			DWORD dwMaxBlock;          // 缓冲区结构数  	        
			DWORD dwHeadPoint;         // 头指针   	         
			DWORD dwLengthOfPerBlock;  // 每一块的长度	     
			DWORD dwFormatChangeCount; // 缓冲区格式变化记录
		};
		UCHAR   ucReserve[1024];     // 保留    
	};	
};


//视频开关单元状态
typedef struct _sw_vf_mat_statu_struct_
{
	//0xE501
    UCHAR ucControlMode;    //控制模式，00-分控 01-本控
    UCHAR ucOutSel[16];     //开关组0~15状态
	
}VFSWMatStatu;

struct VFSWMatPara
{
	//0xE502
    WORD wValid;    // 有效标志 
					//Bit0:串口切换（2选1）
					//Bit1:串口切换备份（2选1）
	                //Bit2:视频切换（2选1）
					//Bit3~15：无效，若设置为1，则视为错误设置。

    UCHAR ucOutSel[16];     //开关组0~15输入选择 0-A机  1-B机 其他-超限
	
};


//功放状态采集: 27 BYTE
////0x1C11
const WORD gc_wPowerStatus_TID = 0x1C11;
typedef struct PowerStatusRead
{
	UINT	ucPowerType;		//功放类型，4字节无符号，范围[0-255],10-10W 40-40W 100-100W
	UINT	ucCount;			//计数，3字节，范围[0-255],每帧加1，自动归零重复
	DOUBLE	dbInputPower;		//输入功率，5字节双精度浮点数，小数点后1位
	DOUBLE	dbOutputPower;		//输出功率，5字节双精度浮点数，小数点后1位
	UCHAR   ucOutputStatus;		//输出状态，3字节，1-ON 0-OFF
	DOUBLE	dbPowerTemperature;	//功放温度，6字节双精度浮点数，小数点后1位
} PowerStatusStruct, *PPowerStatusStruct;

}
#pragma pack()

#endif
