//TK4401扩频接口数据结构数据结构定义
//Created by Lix-2013-10-05
//SMCU_BBE_CDMAStruct.h
//20141102  遥控参数中增加BCH编码的开关  weij
//***************************** 更改说明 ************************************
//
//20131005 lix   基于项目4223/4224/4225数据结构定义生成初始版本
//注意：为保留结构完整性，头文件中关于引导、跟踪接收机及校相、
//数传的相关数据结构并未删去，但本项目没有上述功能，请忽略。
//***************************************************************************


//*******************************************************

#ifndef SMCUSSB2_BBE_CDMA_20131005_STRUCT_H_
#define SMCUSSB2_BBE_CDMA_20131005_STRUCT_H_
#include "TypeDefine.h"
#include "MCSDataStruct.h"
#pragma pack(1)

namespace SMCU2
{
//监控信息帧格式定义表
//标识	类型	字节长度	说明
//地址编码
//typedef struct MCSAddressBlk
//{
//	UCHAR  ucUnit;   //分机，监控分系统00H~0FH，天伺馈分系统10H~1FH，多功能数字基带分系统60H~7FH
//	UCHAR  ucDev;   //设备，同一站内的设备顺序排号，1~255，0保留
//	UCHAR  ucStation; //站，按基地内各站顺序排号，1~255，0保留
//	UCHAR  ucBase;   //基地，按照压缩BCD格式编码

//} MCSAddressStruct, *PMCSAddressStruct;

////帧头
//typedef struct MCSProtocolHeaderBlk
//{
//	MCSAddressStruct  SID;	//信源标识（SID）
//	MCSAddressStruct  DID;	//信宿标识（DID）
//	UINT   uiBID;			//信息类别
//	USHORT  usVer;          //主版本号  目前0x212
//	UCHAR  ucReserved[2];   //保留，固定填0
//	UINT   uiDate;			//日期
//	UINT   uiTime;		    //时间
//	UINT   uiLength;		//数据字段(DATA)的字节长度，单位：字节

//} MCSProtocolHeaderStruct, *PMCSProtocolHeaderStruct;


// 系统工作模式
typedef struct TMCSSystemWorkModeBlk
{
    //////////////////////////////////////
    // DownLink
    //BYTE0
    DWORDLONG	ulS_STD_TTC_D		: 1;
    DWORDLONG	ulS_SS1_TTC_D		: 1;
    DWORDLONG	ulS_SS2_TTC_D		: 1;
    DWORDLONG						: 1;
    DWORDLONG	ulS_FM_TM_D			: 1;
    DWORDLONG	ulS_LDDT_D			: 1;
    DWORDLONG	ulC_STD_TTC_D		: 1;
    DWORDLONG	ulC_SS1_TTC_D		: 1;

    // BYTE1
    DWORDLONG	ulC_SS2_TTC_D		: 1;
    DWORDLONG	ulXKa_STD_TTC_D		: 1;
    DWORDLONG	ulXKa_SS1_TTC_D		: 1;
    DWORDLONG	ulXKa_SS2_TTC_D		: 1;
    DWORDLONG						: 1;
    DWORDLONG	ulXKa_LDDT_D		: 1;
    DWORDLONG	ulXKa_HDDT_D		: 1;
    DWORDLONG						: 1;

    // BYTE2 BYTE3
    DWORDLONG						: 8;
    DWORDLONG						: 8;

    ///////////////////////////////////////
    // UpLink
    // BYTE4
    DWORDLONG	ulS_STD_TTC_U		: 1;
    DWORDLONG	ulS_SS1_TTC_U		: 1;
    DWORDLONG	ulS_SS2_TTC_U		: 1;
    DWORDLONG						: 1;
    DWORDLONG						: 1;
    DWORDLONG	ulS_LDDT_U			: 1;
    DWORDLONG	ulC_STD_TTC_U		: 1;
    DWORDLONG	ulC_SS1_TTC_U		: 1;

    // BYTE5
    DWORDLONG	ulC_SS2_TTC_U		: 1;
    DWORDLONG	ulXKa_STD_TTC_U		: 1;
    DWORDLONG	ulXKa_SS1_TTC_U		: 1;
    DWORDLONG	ulXKa_SS2_TTC_U		: 1;
    DWORDLONG						: 1;
    DWORDLONG	ulXKa_LDDT_U		: 1;
    DWORDLONG						: 1;
    DWORDLONG						: 1;

    // BYTE6 BYTE7
    DWORDLONG						: 8;
    DWORDLONG						: 8;
}TMCSSystemWorkModeStruct, *PTMCSSystemWorkModeStruct;

//////////////////////////////////////////////////////////////////
// 单元和参数编码
// 一套设备各个分机内部划分为若干监控单元，单元划分可对应到实际的部件，或功能上具有一定关系的逻辑单元；
// 一个单元内部带有若干参数。

// 单元编码：分机内部的单元顺序编号，1~255，0保留
// 参数编码：单元内部的参数顺序编号，1~255，0保留

// 数据元素：信息交互最小单位，分为状态、设置、控制元素。
// 设置和控制元素同时也是状态元素。


//下面定义数据元素
//公共单元参数数据元素（单元编号：01）
typedef struct MCSSysParamStructBlk
{
    UCHAR  ucTargetNum;		//目标数。
    UCHAR  ucSameWave;		//目标波束：0：无效，1：同波束 2：不同波束
    DWORDLONG   dwlComboID; //设备组合号
    UCHAR  ucSendMode;      //送数方式 0：无效，1：常送，2：人工
    UCHAR  ucTrackChnlSel;   //跟踪通道选择    --
    UCHAR  ucTrackOutType;       //跟踪目标选择方式 0：无效，1--指定 2--择优
    UCHAR  ucTrackDefCH;         //跟踪目标选择 输出选择模式为指定时有效0：无效，1--通道1 2--通道2 3--通道3
    UCHAR  ucTrackType;          //主跟踪支路类型 0：无效 1--测量通道 2--遥测通道、
    // UCHAR	ucTrkOutTh;			//择优输出门限，8位无符号整型数，量化单位：1dB。取值范围：3 ~ 30dB。
    // UCHAR	ucTrkHoldTh;		//跟踪保持门限，8位无符号整型数，量化单位：1dBHz。取值范围：30 ~ 127dBHz。
    // UCHAR	ucTrkHoldTime;		//跟踪判决时间，8位无符号整型数，量化单位：s。取值范围：1 ~ 127s。
    UCHAR ucDataMode;//数据网数据格式 0：无效，1：新格式，2：老格式
    // 	UCHAR  ucCjCode;  //测距数据处理标志码 保留
    // 	UCHAR  ucCsCode;  //测速数据处理标志码 保留
    // 	UCHAR  ucTmCode;  //遥测数据处理标志码 保留

} MCSSysParamStruct, *PMCSSysParamStruct;

typedef struct MCSSSB2TargetSysParamStructBlk
{
    UINT   uiM ;		    //任务标识（M字段）
    UCHAR  ucTaskDescribe[6];//任务代号
    DWORDLONG   dwlAnswerRevUpFreq;//上行工作频率，范围20250000~21200000，步进1Hz
    DWORDLONG   dwlAnswerRevDownFreq;//下行工作频率，范围22000000~23000000，步进1Hz
    UCHAR  ucCSSendRate;    //测速采样周期 0：无效，1：0.05s，2：0.1s，3：3s，4：3s，5：10s
    UCHAR   ucCSAccTime;      //测速积分时间  0：无效，01H-0.04s；02H-0.05s；03H-0.08s；04H-0.25s；05H-0.4s；06H-0.8s;07H-2s;8-3s;9-1s;10-10s.
    UCHAR  ucCJSendRate;    //测距采样率 0：无效，1：1z，2：2Hz，3：4Hz，4：10Hz，5：20Hz，6：0.2Hz
    // 	UINT   uiRSPZero;       //应答机距离零值，单位：0.001m
    // 	UINT   uiRSPTDelay;	//应答机相对时延差；单位：0.01ns；
    UCHAR  ucCjCode;  //测距数据处理标志码 保留
    UCHAR  ucCsCode;  //测速数据处理标志码 保留
    UCHAR  ucTmCode;  //遥测数据处理标志码 保留

} MCSSSB2TargetSysParamStruct, *PMCSSSB2TargetSysParamStruct;

//扩频接收机及测速单元参数数据元素（单元编号：02、03）
typedef struct MCSSSRevParamBlk
{
    UCHAR  ucRevChannel;    //接收通道 0：无效，1：左旋，2：右旋，3：左右旋同时（保留）
    //	UINT   uiCarrIF;		//输入中频，量化单位：1 Hz，取值范围：68 MHz ~ 72 MHz，缺省值为70 MHz。
    // 	UCHAR  ucAgcTime;		//AGC时间常数  0：无效，1：1ms，2：10ms，2：100ms，3：1000ms。
    // 	UCHAR  ucPllBw;	    	//载波环路带宽 0：无效，1：5Hz（保留），2：10Hz（保留），3：100Hz，4：500Hz，5：1kHz，6：2kHz。
    //	short  sDoppOff;		//下行多普勒预置，范围：-180kHz~180kHz，步进：5 kHz
    // 	USHORT  usAcquRange;     //下行多普勒频率捕获范围，量化单位：1kHz，取值范围：0 ~ 180 kHz，缺省为180kHz（即±180kHz）。
    // 	USHORT  usDopChangeRate;  //最大多普勒变化率   S频段 0~15kHz/s  Ka频段 0~25kHz/s  //20140827 新增加

} MCSSSRevParamStruct, *PMCSSSRevParamStruct;

typedef struct MCSTargetTMSSRevParamBlk
{
    UCHAR	ucPNRate;		//伪码速率,量化单位：1.023 Mc/s，取值范围为1 ~ 10
    UCHAR  ucAgcTime;		//AGC时间常数  0：无效，1：1ms，2：10ms，2：100ms，3：1000ms。
    UCHAR  ucPllBw;	    	//载波环路带宽 0：无效，1：5Hz（保留），2：10Hz（保留），3：100Hz，4：500Hz，5：1kHz，6：2kHz。

    USHORT	usPNPIIBw;			//伪码环路带宽，量化单位：0.1 Hz，取值范围：0.1 ~ 32.0 Hz。
    // 	SHORT	siDoppOffset;		//下行多普勒预置,量化单位：1 kHz，取值范围：－180 ~ 180 kHz，缺省值为0 kHz。
    UCHAR	ucCN0;				//捕获门限，取值范围：30～100dBHz，量化单位：1 dBHz。

} MCSTargetTMSSRevParamStruct, *PMCSTargetTMSSRevParamStruct;

typedef struct MCSTargetMeasSSRevParamBlk
{
    UCHAR	ucPNRate;		//伪码速率,量化单位：1.023 Mc/s，取值范围为1 ~ 10
    UCHAR  ucAgcTime;		//AGC时间常数  0：无效，1：1ms，2：10ms，2：100ms，3：1000ms。
    UCHAR  ucPllBw;	    	//载波环路带宽 0：无效，1：5Hz（保留），2：10Hz（保留），3：100Hz，4：500Hz，5：1kHz，6：2kHz。
    USHORT	usPNPIIBw;			//伪码环路带宽，量化单位：0.1 Hz，取值范围：0.1 ~ 32.0 Hz。
    // 	SHORT	siDoppOffset;		//下行多普勒预置,量化单位：1 kHz，取值范围：－180 ~ 180 kHz，缺省值为0 kHz。
    UCHAR	ucCN0;				//捕获门限，取值范围：30～100dBHz，量化单位：1 dBHz。

} MCSTargetMeasSSRevParamStruct, *PMCSTargetMeasSSRevParamStruct;

//扩频上行调制单元参数数据元素 （单元编号：04）
typedef struct MCSSSUpModulParamBlk
{
    UCHAR  ucWaveOutCtrl;	//载波输出控制 0：无效，1：允许，2：禁止
    char   cWaveOutLevel;	//载波输出电平：范围-30～0，单位dBm
    UCHAR	ucModulate;		//载波调制方式：0：无效，1：单载波，2：已调载波//加调/去调,0：无效，1：加调，2：去调40
    UCHAR  ucNoiseSw;	    //噪声开关 0：无效，1：开，2：关
    UCHAR  ucCN0;           //载波输出载噪比，范围30～100，单位：dBHz
    // 	UCHAR	usDoppCompType;		//多卜勒补偿方式,1H表示手动；2H表示轨道根数；3H表示下行多卜勒反推；4H表示理论弹道；5H表示不补偿。
    // 	USHORT	usDoppCompPrd;		//多卜勒补偿周期,表示多卜勒补偿的周期，量化单位：1 ms，取值范围：10 ~ 10 000 ms。
    UCHAR  ucChannelWaveOutCtrl;	//通道载波输出控制 0：无效，1：允许，2：禁止
    char   cChannelWaveOutLevel;	//通道载波输出电平：范围-30～0，单位dBm


} MCSSSB2SSUpModulParamStruct, *PMCSSSUpModulParamStruct;

typedef struct MCSTargetTCSSUpModulParamBlk
{
    // UCHAR  ucOffestMode;    //上行多普勒预置方式 0：无效，1：手动，2：自动

    //UCHAR  ucPreDplValue;	//上行多普勒频率预置值：范围-180kHz～180kHz，步进5kHz。// 20120719 20140827
    short  ucPreDplValue;	//上行多普勒频率预置值：范围-180kHz～180kHz，步进5kHz。// 20120719 协议改为2字节

    USHORT	usRetZVel;		//回零速度
    UCHAR	ucPNRate;		//伪码速率,量化单位：1.023 Mc/s，取值范围为1 ~ 10
    // 	USHORT	usSPNInitPhaseA;	//短码初相A,16位无符号整型数，D9 ~ D0位有效。
    // 	USHORT	usSPNInitPhaseB;	//短码初相B,16位无符号整型数，D9 ~ D0位有效
    // 	USHORT	usSPNPolyA;			//短码本原多项式A,16位无符号整型数，D10~ D0位有效
    // 	USHORT	usSPNPolyB;			//短码本原多项式B,16位无符号整型数，D10 ~ D0位有效
    UCHAR	ucOutputSwitch;		//输出开关,8位无符号整型数,0：无效，1：允许，2：禁止
    UCHAR	cPowAdjust;			//功率微调,量化单位：1 dBm，取值范围为－20 ~ 0 dBm，缺省为0 dBm
    UCHAR	ucPNIModuSw;		//I路伪码加调/去调，0：无效，1：加调，2：去调。
    UCHAR	ucDataModuSw;		//数据调制,表示数据加调/去调，0：无效，1：加调，2：去调
    //	UCHAR  ucTcCntr;        //盲发开关，0：无效，1：允许，2：禁止


} MCSSSB2TargetTCSSUpModulParamStruct, *PMCSTargetTCSSUpModulParamStruct;

typedef struct MCSTargetMeasSSUpModulParamBlk
{
    UCHAR	ucTargetCode;	// 目标代号
    // UCHAR	ucOffestMode;    //上行多普勒预置方式 0：无效，1：手动，2：自动

    UCHAR	ucPreDplValue;	//上行多普勒频率预置值：范围0～180kHz，
    //short	ucPreDplValue;	//上行多普勒频率预置值：范围0～180kHz，。

    USHORT	usRetZVel;		//回零速度
    UCHAR	ucPNRate;		//伪码速率,量化单位：1.023 Mc/s，取值范围为1 ~ 10

    UCHAR	ucOutputSwitch;		//输出开关,8位无符号整型数,0：无效，1：允许，2：禁止
    UCHAR	cPowAdjust;			//功率微调,量化单位：1 dBm，取值范围为－20 ~ 0 dBm，缺省为0 dBm
    UCHAR	ucPNIModuSw;		//I路伪码加调/去调，0：无效，1：加调，2：去调。
    UCHAR	ucDataModuSw;		//数据调制,表示数据加调/去调，0：无效，1：加调，2：去调
    SHORT   sCarrFreqDif;          //星上载波频率偏差，16位有符号整型数。量化单位：1 kHz，取值范围：－180 ~ 180 kHz。

} MCSSSB2TargetMeasSSUpModulParamStruct, *PMCSTargetMeasSSUpModulParamStruct;

//扩频遥控单元参数数据元素（单元编号：07）
typedef struct MCSSSB2TCParamBlk
{
    //	UCHAR  ucModStyle;		//副载波调制体制：0：无效，1:PSK, 2:FSK。ASK、PSK时只有1频率副载波(副载波0频率)存在，FSK存在4个
    UCHAR  ucCodeStyle;		//码型：0：无效，1： RZ（FSK时有效）  2： NRZ-L，3： NRZ- M，
    //4： NRZ-S，5：= Biφ-L，6： Biφ-M，7： Biφ-S
    UINT   uiCodeRate;		//码速率：范围100～8000，单位bps
    //	UINT   uiSwaveFreq_1;	//副载频0频率(f0)：范围2000～100000，单位Hz,0：无效，FSK PSK体制有效
    //	UINT   uiSwaveFreq_2;	//副载频1频率(f1)：范围2000～100000，单位Hz,0：无效，FSK体制有效
    //	UINT   uiSwaveFreq_3;	//副载频填充频率(f填充)：范围2000～100000，单位Hz,0：无效，FSK体制有效
    //	UINT   uiSwaveFreq_4;	//副载频执行频率(f执行)：范围2000～100000，单位Hz,0：无效，FSK体制有效
    UCHAR  ucSWaveMd;		//遥控工作模式：0：无效，1：常发，2：突发，3：半突发
    UCHAR  ucFCodeMinU;		//前补序列码样：1byte16进制数,PSK体制有效
    USHORT usFQueueLen;		//前补序列长度：范围0～8000，单位byte,PSK体制有效
    UCHAR  ucBCodeMinU;		//后补序列码样：1byte16进制数,PSK体制有效
    USHORT usBQueueLen;		//后补序列长度：范围0～8000，单位byte,PSK体制有效
    UCHAR  ucA1FQueueCheck;	//A1前补序列选择：0：无效，1：有，2：无,PSK体制有效
    UCHAR  ucA1BQueueCheck;	//A1后补序列选择：0：无效，1：有，2：无,PSK体制有效
    UCHAR  ucC7FQueueCheck;	//C7前补序列选择：0：无效，1：有，2：无,PSK体制有效
    UCHAR  ucC7BQueueCheck;	//C7后补序列选择：0：无效，1：有，2：无,PSK体制有效
    UCHAR  ucC8FQueueCheck;	//C8前补序列选择：0：无效，1：有，2：无（与C7前补一致）,PSK体制有效
    UCHAR  ucC8BQueueCheck;	//C8后补序列选择：0：无效，1：有，2：无（与C9后补一致）,PSK体制有效
    UCHAR  ucC9FQueueCheck;	//C9前补序列选择：0：无效，1：有，2：无,PSK体制有效
    UCHAR  ucC9BQueueCheck;	//C9后补序列选择：0：无效，1：有，2：无,PSK体制有效
    DWORDLONG  ucECodeMinU;	    //执行编码码样：十六进制码（当选择FSK调制时，该项设置无效，即不允许设置）
    UCHAR  ucECodeLen;		//执行编码长度：8～64bit（当选择FSK调制时，该项设置无效，即不允许设置）
    UCHAR  ucIndleCode;		//空闲序列码样：十六进制码（当选择突发状态时，该项设置无效，即不允许设置）
    //	UCHAR  ucLCFCodeChange; //空闲序列是否码型变换，0：无效，1：是，2：否。
    UCHAR ucC8Data;			//C8内部间隔：范围：0～127，单位：Byte（PSK时为填充空闲序列长度，FSK时为填充频率的填充长度）
    USHORT usTCProctC;		//指令保护间隙：范围：100～10000，单位ms，步进100,默认1000ms
    UCHAR ucTCWorkCyc;		//遥控工作期启动方式：0：无效，1：人工, 2：自动，

    UCHAR ucBCHCode;    //BCH编码   1不编码  2编码   20141102  新增
    UCHAR		ucBCHBeginSeriesCtrl;	//BCH起始序列控制  0:无此参数 1:不加 2:加
    USHORT		usBCHBeginSeries;        //BCH起始序列图样   CCSDS 推荐EB90
    UCHAR		ucBCHEndCtrl;            //BCH结尾序列控制  0:无此参数 1:不加 2:加
    DWORDLONG  dwlBCHEndSeries;      //BCH结尾序列图样

    UCHAR   ucDisturboCtrl;          //加扰控制 0:无此参数; 1:不加扰; 2:加扰
    UCHAR   ucDisturboLen;          //加扰多项式长度 1:1字节;2:2字节; 3:3字节 4:4字节
    UINT    uiDisPoly;              //加扰多项式
    UINT    uiDisPhase;              //加扰初相

} MCSSSB2TCParamStruct, *PMCSSSB2TCParamStruct;

//扩频遥测单元参数数据元素（单元编号：08）
typedef struct MCSSSB2TMParamBlk
{
    UINT   uiCodeStyle;	    //码型：0：无效，1：NRZ-L，2：NRZ-M，3：NRZ-S
    UINT   uiTM_CodeRate;	//码速率：范围100～64000，单位bps
    UCHAR  ucBitOut;        //遥测数据类型:0：无效，1：帧格式；2：位流
    UCHAR  ucCodingMd;		//译码方式：0：无效，1：不译码，2：Viterbi译码，3：R-S译码，4：R-S和Viterbi级联译码
    // UCHAR  ucCodingMdNew;	//译码方式：0：无效，1：关 2：Viterbi译码，3：R-S译码，4：R-S和Viterbi级联译码

    UCHAR  ucViterbiMd;		//卷积码类型:0：无效，1：(7,1/2)；2：(7,3/4)
    UCHAR  ucG2Inv;		    //Viterbi译码相位:0：无效，1：正相；2：反相。
    UCHAR  ucRSMd;	    	//RS码类型:0：无效，1：(223,255)；2：(239,255)
    UCHAR  ucInterleav;	    //RS译码交织深度，取值范围：1 ~ 5。
    UCHAR  ucBasiSel;	    //R-S编码双基选择:0：无效，1：对偶基；2：常规基

    UCHAR  ucTurboRatio;     //Turbo码率 0:无此参数; 1:1/2; 2:1/3;  3:1/4; 4:1/6
    UCHAR  ucLDPCRatio;      //LDPC 码率  0:无此参数 1:1/2; 2:2/3;  3:3/4; 4:4/5; 5:7/8(8160,7136); 6:7/8(8176,7156)

    UCHAR  ucUnturbCtrl;	//解扰控制：0：无效，1：不解扰，2：自动解扰，3：人工解扰
    UCHAR  ucRandLen;		//解扰多项式长度：0：无效，1：1字节，2：2字节，3：3字节，4：4字节
    UINT   uiRandPoly;		//解扰多项式
    UINT   uiPhase;		    //解扰初相

    UCHAR  ucCRCSw;         //传输帧CRC校验  0:无此参数; 1:开; 2:关

    UCHAR  ucWordLen;		//字长，默认8位
    USHORT usFrameLen;		//帧长：范围4～1024，单位字
    USHORT  ucSframeLen;	    //格式(副帧)长：范围4～256，单位 帧

    UCHAR  ucFrameSynGLen;	//帧同步码组长度：0：无效，1：16bit，2：24bit，3：32bit
    UINT   uiFrameSynG;	    //帧同步码组：16进制数
    UCHAR  ucFspPlace;		//子帧同步码组位置，0：无效，01表示帧首；02表示帧尾
    UCHAR  ucSFrameSynMd;	//副帧同步方式：0：无效，1：无副帧，2：ID副帧，3：循环副帧，4：反码副帧
    USHORT usIDSubFrmPos;	//ID副帧ID字位置，单位：字；0：无效，取值范围：1 ~ 1024
    UCHAR  ucIDBase;		//ID字基值，0：无效，1：1，2：0
    UCHAR  ucIDDirect;		//ID计数方向，0：无效，1：加计数，2：减计数。
    USHORT usCycFrmSynCodePos;//循环副帧码组位置,量化单位：1 字，0：无效，取值范围：1 ~ 1024 字
    UCHAR  ucCycFrmSynCodeLen;//循环副帧码组长度  0：无效，1：8bit，2：16bit，3：24bit，4：32bit
    UINT   uiCycFrmSynCode; //循环副帧码组,64位无符号整型数。
    UCHAR  ucFrSynSerLiErr; //帧同步容错搜索容错位数：范围0~7，单位bit
    UCHAR  ucFrSynLoLiErr;	//帧同步容错锁定容错位数：范围0~7，单位bit
    UCHAR  ucFrSynVerLiErr; //帧同步容错校核容错位数：范围0~7，单位bit
    UCHAR  ucFrSynVerCheck; //帧同步容错校核检测帧数：范围1~8，单位帧
    UCHAR  ucFrSynLoCheck;	//帧同步容错锁定检测帧数：范围1~8，单位帧
    UCHAR  ucCFSearchErrBits;//循环副帧搜索容错位数，取值范围：0 ~ 7 位；
    UCHAR  ucCFVerifyErrBits;//循环副帧校核容错位数，取值范围：0 ~ 7 位；
    UCHAR  ucCFLockErrBits;	//循环副帧锁定容错位数，取值范围：0 ~ 7 位。
    UCHAR  ucSubFVerifyFrame;//副帧三态逻辑:校核检测帧数,取值范围：1 ~ 8 帧；
    UCHAR  ucSubFLockFrame;	//副帧三态逻辑:锁定检测帧数,取值范围：1 ~ 8 帧。
    UCHAR  ucDataCode;  //数据帧标志码

} MCSSSB2TMParamStruct, *PMCSSSB2TMParamStruct;

//扩频测距单元参数数据元素（单元编号：05、06）
typedef struct MCSSSCjParamBlk
{
    //	USHORT	usStationID;	//地面测站代号，16位无符号整型数。取值范围0～1023。
    UCHAR	ucTargetType;		//目标类型；0：无效，1：非JB-8目标，2：JB-8主星；3：JB-8副星
    UCHAR	ucTargetCode;		//目标代号；0x1A--JB-8主星；0x2B--JB-8副星1；0x2C--JB-8副星2；
    USHORT  ucStationID;		//测站代号
    UCHAR	ucStationIDIsUsed;   //测站代号是否使用
    UCHAR	ucCRC;				//上行CRC校验开关，0：无效，1：有，2：无
    UCHAR	ucRP;				//下行测量帧RP；0：无效，1：有，2：无
    UCHAR	ucDownCRC;			//下行测量帧CRC校验开关；0：无效，1：有，2：无
    UCHAR  ucRMHCheck;		//距离解模糊：0：无效，1：基带解，2：中心解
    UCHAR  ucPreRangDataMd;	//距离预报方式：0：无效，1：手动，2：轨道根数，3：理论弹道
    UINT  ulPridictR;		//距离预报值，32位无符号整型数，量化单位：1m。
    UCHAR  ucStationCount;	//工作站数目；范围：1~4；
    UCHAR  ucSatChnelNo;		//本站对应星上通道号；范围：1~ucStationCount;
    UINT   uiRSPDisZero;   //应答机距离零值
    UINT   uiRSPDifTime;   //应答机相对时延差

} MCSSSB2CjParamStruct, *PMCSSSCjParamStruct;

//扩频主跟踪单元参数数据元素（单元编号：09、10）
typedef struct MCSSSTraceParamBlk
{
    /*int    iAzSlope;		//方位斜率:整数
    int	   iElSlope;		//俯仰斜率:整数
    USHORT usAzPhaseValue;	//方位通道相位：范围0～3600，单位 0.1度
    USHORT usElPhaseValue;	//俯仰通道相位：范围0～3600，单位 0.1度*/

} MCSSSTraceParamStruct, *PMCSSSTraceParamStruct;

//扩频引导中频接收机单元参数数据元素（单元编号：13）
typedef struct MCSSSGuideRevParamBlk
{
    UCHAR  ucRevChannel;    //接收通道 0：无效，1：左旋，2：右旋，3：左右旋同时（保留）
    //	UINT   uiCarrIF;		//输入中频，量化单位：1 Hz，取值范围：68 MHz ~ 72 MHz，缺省值为70 MHz。
    UCHAR  ucAgcTime;		//AGC时间常数  0：无效，1：250ms，2：1000ms。
    UCHAR  ucPllBw;	    	//载波环路带宽 0：无效，1：5Hz（保留），2：10Hz（保留），3：100Hz，4：500Hz，5：1kHz，6：2kHz。
    //	short  sDoppOff;		//下行多普勒预置，范围：-180kHz~180kHz，步进：5 kHz
    UCHAR  ucAcquRange;     //下行多普勒频率捕获范围，量化单位：1kHz，取值范围：0 ~ 180 kHz，缺省为180kHz（即±180kHz）。
    UCHAR	ucPNRate;		//伪码速率,量化单位：1.023 Mc/s，取值范围为1 ~ 10
    USHORT	usSPNInitPhaseA;	//短码初相A,16位无符号整型数，D9 ~ D0位有效。
    USHORT	usSPNInitPhaseB;	//短码初相B,16位无符号整型数，D9 ~ D0位有效
    USHORT	usSPNPolyA;			//短码本原多项式A,16位无符号整型数，D10~ D0位有效
    USHORT	usSPNPolyB;			//短码本原多项式B,16位无符号整型数，D10 ~ D0位有效
    USHORT	usPNPIIBw;			//伪码环路带宽，量化单位：0.1 Hz，取值范围：0.1 ~ 32.0 Hz。
    SHORT	siDoppOffset;		//下行多普勒预置,量化单位：1 kHz，取值范围：－180 ~ 180 kHz，缺省值为0 kHz。
    UCHAR	ucCN0;				//捕获门限，取值范围：30～100dBHz，量化单位：1 dBHz。

} MCSSSB2GuideRevParamStruct, *PMCSSSGuideRevParamStruct;

//扩频引导跟踪单元参数数据元素（单元编号：14）
typedef struct MCSSSGuideTraceParamBlk
{
    /*	int    iAzSlope;		//方位斜率:整数
    int	   iElSlope;		//俯仰斜率:整数
    USHORT usRefDelay;  	//时延修正：单位 0.01度*/

} MCSSSGuideTraceParamStruct, *PMCSSSGuideTraceParamStruct;

//扩频遥测模拟源单元参数数据元素（单元编号：11）
typedef struct MCSSSB2TMSimulParamBlk
{
    UCHAR  ucDataCheck;	    //数据源选择 0：无效，1：内，2：外。
    UCHAR  ucdatasel;	    //模拟数据，0：无效，1：固定码，2：随机码，3：顺序数，4：自定义
    UCHAR   uidata;		    //固定码数据
    // UINT   uiSWFreq;		//副载波频率：范围5000～512000单位Hz
    UINT   uiCodeStyle;	    //码型：0：无效，1：NRZ-L，2：NRZ-M，3：NRZ-S，4：Biφ-L，5：Biφ-M，6：Biφ-S
    UINT   uiTM_CodeRate;	//码速率：范围100～64000，单位bps
    UCHAR  ucCodingMd;		//编码方式：0：无效，1：不编码，2：卷积编码，3：R-S编码，4：R-S和卷积级联编码
    UCHAR  ucViterbiMd;		//卷积码类型:0：无效，1：(7,1/2)；2：(7,3/4)
    UCHAR  ucG2Inv;		    //卷积编码相位:0：无效，1：正相；2：反相。
    UCHAR  ucRSMd;	    	//RS码类型:0：无效，1：(223,255)；2：(239,255)
    UCHAR  ucInterleav;	    //RS编码交织深度，取值范围：1 ~ 5。
    UCHAR  ucBasiSel;	    //R-S编码双基选择:0：无效，1：对偶基；2：常规基
    UCHAR  ucTurboRatio;    //Turbo码率 0:无此参数; 1:1/2; 2:1/3;  3:1/4; 4:1/6
    UCHAR  ucLDPCRatio;      //LDPC 码率  0:无此参数 1:1/2; 2:2/3;  3:3/4; 4:4/5; 5:7/8(8160,7136); 6:7/8(8176,7156)

    UCHAR  ucturbCtrl;  	//加扰控制：0：无效，1：不加扰，2：自动加扰，3：人工加扰
    UCHAR  ucRandLen;		//加扰多项式长度：0：无效，1：1字节，2：2字节，3：3字节，4：4字节
    UINT   uiRandPoly;		//加扰多项式
    UINT   uiPhase;		    //加扰初相

    UCHAR  ucCRC;           //传输帧CRC校验

    UCHAR  ucWordLen;		//字长，默认8位
    USHORT usFrameLen;		//帧长：范围4～1024，单位字
    USHORT  ucSframeLen;	//格式(副帧)长：范围4～256，单位 帧
    UCHAR  ucFrameSynGLen;	//帧同步码组长度：0：无效，1：16bit，2：24bit，3：32bit
    UINT   uiFrameSynG;	    //帧同步码组：16进制数
    UCHAR  ucFspPlace;		//子帧同步码组位置，0：无效，01表示帧首；02表示帧尾
    UCHAR  ucSFrameSynMd;	//副帧同步方式：0：无效，1：无副帧，2：ID副帧，3：循环副帧，4：反码副帧
    USHORT usIDSubFrmPos;	//ID副帧ID字位置，单位：字；0：无效，取值范围：1 ~ 1024
    UCHAR  ucIDBase;		//ID字基值，0：无效，1：1，2：0
    UCHAR  ucIDDirect;		//ID计数方向，0：无效，1：加计数，2：减计数。
    USHORT usCycFrmSynCodePos;//循环副帧码组位置,量化单位：1 字，0：无效，取值范围：1 ~ 1024 字
    UCHAR  ucCycFrmSynCodeLen;//循环副帧码组长度  0：无效，1：8bit，2：16bit，3：24bit，4：32bit
    UINT   uiCycFrmSynCode; //循环副帧码组,32位无符号整型数。


} MCSSSB2TMSimulParamStruct, *PMCSSSB2TMSimulParamStruct;


//扩频码 单元
typedef struct MCSSSB2PNCodeBlk
{
    UCHAR	ucPNSource;     //伪码来源 1：本机  2：伪码服务器
    DWORDLONG  dwlUpMessPNCodeNo;     //上行测量伪码码号
    DWORDLONG  dwlUpTCPNCodeNo;       //上行遥控伪码码号
    DWORDLONG  dwlDnMessPNCodeNo;     //下行测量伪码码号
    DWORDLONG  dwlDnTCPNCodeNo;       //下行遥控伪码码号

    USHORT	usUpTCSPNInitPhaseA;		//上行遥控伪码初相A,16位无符号整型数，D9 ~ D0位有效。
    USHORT	usUpTCSPNInitPhaseB;		//上行遥控伪码初相B,16位无符号整型数，D9 ~ D0位有效
    USHORT	usUpTCSPNPolyA;			//上行遥控伪码本原多项式A,16位无符号整型数，D10~ D0位有效
    USHORT	usUpTCSPNPolyB;			//上行遥控伪码本原多项式B,16位无符号整型数，D10 ~ D0位有效
    USHORT	usUpMessSPNInitPhaseA;	//上行测量伪码初相A,16位无符号整型数，D9 ~ D0位有效。
    USHORT	usUpMessSPNInitPhaseB;	//上行测量伪码初相B,16位无符号整型数，D9 ~ D0位有效
    USHORT	usUpMessSPNPolyA;			//上行测量伪码本原多项式A,16位无符号整型数，D10~ D0位有效
    USHORT	usUpMessSPNPolyB;			//上行测量伪码本原多项式B,16位无符号整型数，D10 ~ D0位有效

    USHORT	usDnTCSPNInitPhaseA;		//下行遥控伪码初相A,16位无符号整型数，D9 ~ D0位有效。
    USHORT	usDnTCSPNInitPhaseB;		//下行遥控伪码初相B,16位无符号整型数，D9 ~ D0位有效
    USHORT	usDnTCSPNPolyA;			//下行遥控伪码本原多项式A,16位无符号整型数，D10~ D0位有效
    USHORT	usDnTCSPNPolyB;			//下行遥控伪码本原多项式B,16位无符号整型数，D10 ~ D0位有效
    USHORT	usDnMessSPNInitPhaseA;	//下行测量伪码初相A,16位无符号整型数，D9 ~ D0位有效。
    USHORT	usDnMessSPNInitPhaseB;	//下行测量伪码初相B,16位无符号整型数，D9 ~ D0位有效
    USHORT	usDnMessSPNPolyA;			//下行测量伪码本原多项式A,16位无符号整型数，D10~ D0位有效
    USHORT	usDnMessSPNPolyB;			//下行测量伪码本原多项式B,16位无符号整型数，D10 ~ D0位有效
} MCSSSB2PNCodeStruct, *PMCSSSB2PNCodeStruct;

//视频切换单元参数数据元素（单元编号：20）
typedef struct MCSVFSWUnitParamBlk
{
    UCHAR	ucOutSel;	    //跟踪通道选择 0：无效，1：基带1，2：基带2，3：基带3，4：基带4

} MCSVFSWUnitParamStruct, *PMCSVFSWUnitParamStruct;


//公共单元状态数据元素（单元编号：01）
typedef struct MCSSysStatusStructBlk
{
    UCHAR  ucMonitorMode;	//监控方式	0：无效， 1：分控， 2：本控
    UCHAR  ucBBEWorkStatus;	//基带设备综合状态	0：无效，1：故障，2：正常
    UCHAR  ucBBEOnline;		//本机在线状态	0：无效，1：在线，2：备份
    UCHAR  ucTimeCodeStatus;//外时码状态	0：无效，1：故障，2：正常
    UCHAR  ucPingBiaoStatus;//外频标状态	0：无效，1：故障，2：正常
    DWORDLONG   ucSystemMode;	//工作模式
    UCHAR  ucSystemWorkMode;//系统工作环路，  1:任务方式  2：联试应答机无线闭环 3：联试应答机有线闭环
    //4：校零变频器无线闭环 5：校零变频器有线闭环 6：偏馈校零变频器闭环
    //7：射频模拟源有线闭环 8:信标方式 9:中频闭环 10:记录回放 11:其他方式

//    UCHAR  ucTrackTarget;   //当前跟踪目标 0：无效，1--目标1 2--目标2 3--目标3
//    UCHAR  ucTrackPolar;   //当前跟踪通道	0：无效， 1：左旋， 2：右旋
    UCHAR  ucUpFreqLock;     //上行频综锁定 00-失锁 01-锁定
    UCHAR  ucDownFreqLock;   //下行频综锁定 00-失锁 01-锁定

    // 	UCHAR	ucBusRWStatus;  //总线读写状态
    // 	UCHAR	ucDSPWorkStatus;  //DSP 工作时钟状态
    // 	UCHAR   ucRecFPGALockStatus;  //FPGA锁定状态
    // 	UCHAR	ucModuBusRWStatus; //调制板总线读写状态
    // 	UCHAR	ucModuDspTimeStatus; //调制板DSP工作时钟状态
    // 	UCHAR	ucModuFPGALockStatus;//调制板FPGA频综锁定状态
    // 	UINT    usContenaSN;  //当前使用的天线编号
    MCSSysParamStruct	tParams;	//参数数据

} MCSSysStatusStruct, *PMCSSysStatusStruct;

typedef struct MCSTargetSysStatusStructBlk
{
    UCHAR	ucTMCarrLock;			//遥测载波锁定指示,0：无效，1：锁定，2：失锁。
    UCHAR	ucTMSPNLock;			//遥测短码锁定指示,0：无效，1：锁定，2：失锁。
    UCHAR	ucMeasCarrLock;			//测量载波锁定指示,0：无效，1：锁定，2：失锁。
    UCHAR	ucMeasSPNLock;			//测量短码锁定指示,0：无效，1：锁定，2：失锁。
    UCHAR	ucSatLock;				//卫星锁定指示,0：无效，1：锁定，2：失锁。
    UCHAR	ucReserved;				//卫星测量锁定指示  20141201 new add
    UCHAR   ucSignalPolar;			//当前跟踪通道	0：无效， 1：左旋， 2：右旋，
    INT     iPreDopp;				//预报多普勒，单位0.01Hz
    USHORT  usRecvCenterCmdCount;   //接收中心指令包数
    USHORT usTCSendNum;				//遥控发令条数
    USHORT usTCSmallLoopErrNum;		//遥控小环误指令条数
    USHORT usTCSmallLoopReturnNum;//遥控小环环回中心条数
    UCHAR  ucCSSendSwitch;	//测速送数控制	0：无效，1：送数，2：停送
    UCHAR  ucCJSendSwitch;	//测距送数控制	0：无效，1：送数，2：停送
    UCHAR  ucTMSendSwitch;	//遥测送数控制	0：无效，1：送数，2：停送

    MCSSSB2TargetSysParamStruct	tParams;	//参数数据

} MCSSSB2TargetSysStatusStruct, *PMCSSSB2TargetSysStatusStruct;

//扩频接收机及测速单元状态数据元素（单元编号：02）
typedef struct MCSSSRevStatusBlk
{
    MCSSSRevParamStruct	tParams;	//参数数据

} MCSSSRevStatusStruct, *PMCSSSRevStatusStruct;

typedef struct MCSTargetTMSSRevStatusBlk
{
    UCHAR	ucCarrLock;			//载波锁定指示,0：无效，1：锁定，2：失锁。
    UCHAR	ucSPNLock;			//短码锁定指示,0：无效，1：锁定，2：失锁。
    USHORT usAgcVolt;		//AGC电压，量化单位：0.001 V
    USHORT usSqDir;		    //S/Φ：单位0.1dBHz
    __int64    iDopRate;        //实测多普勒频率，单位：0.01 Hz
    MCSTargetTMSSRevParamStruct	tParams;	//参数数据

} MCSTargetTMSSRevStatusStruct, *PMCSTargetTMSSRevStatusStruct;

typedef struct MCSTargetMeasSSRevStatusBlk
{
    UCHAR	ucCarrLock;			//载波锁定指示,0：无效，1：锁定，2：失锁。
    UCHAR	ucSPNLock;			//短码锁定指示,0：无效，1：锁定，2：失锁。
    USHORT usAgcVolt;		//AGC电压，量化单位：0.001 V
    USHORT usSqDir;		    //S/Φ：单位0.1dBHz
    __int64    iDopRate;        //实测多普勒频率，单位：0.01 Hz
    int    iSpeed;          //实测速度值，单位：0.0001 m/s
    int		iDeltFrq;		//星地频差；4个字节，补码表示的有符号二进制数，单位：0.01Hz
    MCSTargetMeasSSRevParamStruct	tParams;	//参数数据

} MCSTargetMeasSSRevStatusStruct, *PMCSTargetMeasSSRevStatusStruct;

//扩频上行调制单元状态数据元素 （单元编号：03）
typedef struct MCSSSUpModulStatusBlk
{
    MCSSSB2SSUpModulParamStruct	tParams;	//参数数据

} MCSSSB2SSUpModulStatusStruct, *PMCSSSUpModulStatusStruct;

typedef struct MCSTargetTCSSUpModulStatusBlk
{
    UCHAR        ucZeroStatus;
    MCSSSB2TargetTCSSUpModulParamStruct	tParams;	//参数数据

} MCSSSB2TargetTCSSUpModulStatusStruct, *PMCSTargetTCSSUpModulStatusStruct;

typedef struct MCSTargetMeasSSUpModulStatusBlk
{
    UCHAR        ucZeroStatus;
    MCSSSB2TargetMeasSSUpModulParamStruct	tParams;	//参数数据


} MCSSSB2TargetMeasSSUpModulStatusStruct, *PMCSTargetMeasSSUpModulStatusStruct;

//扩频遥控单元状态数据元素（单元编号：04）
typedef struct MCSSSB2TCStatusBlk
{
    UCHAR  ucTCStatus;      //遥控发令指示 0：无效，1：正在发令，2：未发令
    UINT   uiSendType;      //发令类别(填写对应的B字段)
    USHORT   uiA2TaskID;      //显示指令代号(遥控代号发令方式下有效)
    UINT   uiTCStartTime;      //发令开始时间
    UINT   uiTCStopTime;      //发令结束时间
    UCHAR ucTCWorkCyc;		//遥控工作期：0：无效，1：开始，2：结束
    USHORT	uiAllCmdSendTotal;		//发出指令总数
    USHORT	uiAllSmallLoopRight;	//小环正确总数
    UCHAR  ucFTBCStatus;      //非同控遥控发令指示 0：无效，1：初始，2：等待，3：运行
    UINT  ulFTBDdeltaT;       //非同控脉冲宽度;
    UINT  ulFTBDdeltaTExe;    //非同控脉冲已发宽度;
    USHORT   usPulseNum;	//非同控脉冲脉串执行次数，16位无符号整型数，量化单位：1 次。
    USHORT    ulPeriod;		//非同控脉冲串周期，32位无符号整型数，量化单位：1 ms。
    USHORT    uldeltaT;		//非同控脉冲串宽度，32位无符号整型数，量化单位：1 ms。
    USHORT   usSendPulseNum;	//非同步脉冲串已执行次数，16位无符号整型数，量化单位：1 次。
    USHORT    ulSendPeriod;		//非同步脉冲串实际执行周期，32位无符号整型数，量化单位：1 ms。
    USHORT    ulSenddeltaT;		//非同步脉冲串实际执行脉冲宽度，32位无符号整型数，量化单位：1 ms。

    USHORT    ulSendPrd;		//当前周期已执行时间，32位无符号整型数，量化单位：1 ms。
    USHORT    ulSendT;		//当前周期脉冲宽度，32位无符号整型数，量化单位：1 ms。
    UCHAR  ucSELFLock;		//自环比对结果，0：无效；1：正常；2：异常。
    UCHAR  ucSMLLock;		//小环锁定指示，0：无效，1：锁定，2：失锁。
    UCHAR  ucSMLLoop;		//小环比对，0：无效；1：正常；2：异常。
    MCSSSB2TCParamStruct	tParams;	//参数数据

} MCSSSB2TCStatusStruct, *PMCSSSB2TCStatusStruct;

//扩频遥测单元状态数据元素（单元编号：05）
typedef struct MCSSSB2TMStatusBlk
{
    UCHAR  ucCodeLock;		//码同步器锁定指示，0：无效，1：锁定，2：失锁
    UCHAR  ucFrmLock;		//帧同步器锁定指示，0：无效，1：锁定，2：失锁
    UCHAR  ucSubFrmLock;	//副帧同步器锁定指示，0：无效，1：锁定，2：失锁
    //	UINT   uiErrBitRate;    //遥测误码率
    //	UINT   uiTotalFrames;    //遥测总帧数
    //	UINT   uiErrFramesNum;   //误帧数
    USHORT usEbN0;          //Eb/N0，单位：0.1dB
    UCHAR  ucVFill;		    //虚拟填充开关，0：无效，1：不填充，2：填充。
    MCSSSB2TMParamStruct	tParams;	//参数数据

} MCSSSB2TMStatusStruct, *PMCSSSB2TMStatusStruct;

//扩频测距单元状态数据元素（单元编号：06）
typedef struct MCSSSCjStatusBlk
{
    UCHAR  ucCodeLock;		//码同步器锁定指示，0：无效，1：锁定，2：失锁
    UCHAR  ucFrmLock;		//帧同步器锁定指示，0：无效，1：锁定，2：失锁
    int    iDistance;       //实测距离值，单位：0.1m
    int    iDeltT;		//星地时差，单位：0.1ns；
    MCSSSB2CjParamStruct	tParams;	//参数数据

} MCSSSB2CjStatusStruct, *PMCSSSCjStatusStruct;

//扩频遥测模拟源单元状态数据元素（单元编号：08）
typedef struct MCSSSB2TMSimulStatusBlk
{
    MCSSSB2TMSimulParamStruct	 tParams;	//参数数据

} MCSSSB2TMSimulStatusStruct, *PMCSSSB2TMSimulStatusStruct;

//扩频码单元 单元编号 12
typedef struct MCSSSB2PNCODEStatusBlk
{
    MCSSSB2PNCodeStruct tParams;  //参数数据
}MCSSSB2PNCODEStatusStruct, *PMCSSSB2PNCODEStatusStruct;

//标准TT&C主跟踪单元状态数据元素（单元编号：09、10）
typedef struct MCSSSTraceStatusBlk
{
    SHORT  siAzimuV;		//方位误差电压，量化单位：mV
    SHORT  siElevaV;		//俯仰误差电压，量化单位：mV
    //	MCSUSBTraceParamStruct	tParams;	//参数数据

} MCSSSTraceStatusStruct, *PMCSSSTraceStatusStruct;
typedef struct{
    MCSSSTraceStatusStruct trace[8];
}MCSSSTraceStatusStruct8;
//扩频引导中频接收机单元状态数据元素（单元编号：13）
typedef struct MCSSSGuideRevStatusBlk
{
    UCHAR	ucCarrLock;			//载波锁定指示,0：无效，1：锁定，2：失锁。
    UCHAR	ucSPNLock;			//短码锁定指示,0：无效，1：锁定，2：失锁。
    USHORT usAgcVolt;		//AGC电压，量化单位：0.001 V
    USHORT usSqDir;		    //S/Φ：单位0.1dBHz
    //__int64    iDopRate;        //实测多普勒频率，单位：0.01 Hz
    MCSSSB2GuideRevParamStruct	tParams;	//参数数据

} MCSSSB2GuideRevStatusStruct, *PMCSSSGuideRevStatusStruct;

//扩频引导跟踪单元状态数据元素（单元编号：14）
//typedef struct MCSUSBGuideTraceStatusBlk
//{
//	SHORT  siAzimuV;		//方位误差电压，量化单位：mV
//	SHORT  siElevaV;		//俯仰误差电压，量化单位：mV
////	MCSUSBGuideTraceParamStruct	tParams;	//参数数据

//} MCSSSGuideTraceStatusStruct, *PMCSSSGuideTraceStatusStruct;


#define SMCU_BBE_CDMA_20131005_STRUCT_H_

//视频切换单元状态数据元素（单元编号：20）
typedef struct MCSVFSWUnitStatusBlk
{
    MCSVFSWUnitParamStruct	tParams;	//参数数据

} MCSVFSWUnitStatusStruct, *PMCSVFSWUnitStatusStruct;

//扩频码开关控制命令 2012-11-03
typedef struct MCSPNSwitchCmdStruct
{
    UCHAR ucPNSwitch;		// 伪码开关，1：解扩；2：不解扩。
    UCHAR ucChannelType;	// 通道类型 1：接收通道 2：调制通道

    UCHAR ucReserved[2];

}MCSPNSWITCHCMDSTRUCT, *PMCSPNSWITCHCMDSTRUCT;

//中频接收机及测试单元 参数以及状态
typedef struct _tag_MCSRevParamStruct
{
    MCSSSRevParamStruct sysPara;
    struct{
        MCSTargetTMSSRevParamStruct recvTMPara;
        MCSTargetMeasSSRevParamStruct recvMeasPara;
    }revParam[4];
}MCSRevParamStruct4, *PMCSRevParamStruct4;

typedef struct _tag_MCSRevStatusStruct
{
    MCSSSRevStatusStruct sysStatus;

    struct{
        MCSTargetTMSSRevStatusStruct recvTMStatus;
        MCSTargetMeasSSRevStatusStruct recvMeasStatus;
    }revStatu[4];
}MCSRevStatusStruct4, *PMCSRevStatusStruct4;

//模式2扩频码设置 9
typedef struct _tag_MCSSSB2PNCodeStruct
{
    MCSSSB2PNCodeStruct PNCode[4];
}MCSSSB2PNCodeStruct4;
typedef struct _tag_MCSSSB2PNCODEStatusStruct
{
    MCSSSB2PNCODEStatusStruct PNCodeStatu[4];
}MCSSSB2PNCODEStatusStruct4;

//公共单元状态数据元素（单元编号：01）
typedef struct _tag_MCSSysTarget4StatusStruct
{
    MCSSysStatusStruct MCSSysStatu;
    MCSSSB2TargetSysStatusStruct MCSSSB2TargetSysStatu[4];
}MCSSysTarget4StatusStruct;

typedef struct _tag_MCSSysParamStruct
{
    MCSSysParamStruct MCSSysParam;
    MCSSSB2TargetSysParamStruct MCSSSB2TargetSysParam[4];
}MCSSysParamStruct4;

//扩频2测速测距数据参数及状态
typedef struct _tag_MCSSSB2CjParamStruct
{
    MCSSSB2CjParamStruct cjParam[4];
}MCSSSB2CjParamStruct4, *PMCSSSB2CjParamStruct4;

typedef struct _tag_MCSSSB2CjStatusStruct
{
    MCSSSB2CjStatusStruct cjStatus[4];
}MCSSSB2CjStatusStruct4, *PMCSSSB2CjStatusStruct4;

//扩频上行调制参数设置
typedef struct _tag_MCSUpModulParamStruct
{
    MCSSSB2SSUpModulParamStruct m_MCSSSUpModulParam;
    struct{
        MCSSSB2TargetTCSSUpModulParamStruct m_MCSTargetTCSSUpModulParam;
        MCSSSB2TargetMeasSSUpModulParamStruct m_MCSTargetMeasSSUpModulParam;
    }upModPara[4];
}MCSUpModulParamStruct4, *PMCSUpModulParamStruct4;

typedef struct _tag_MCSUpModuleStatusStruct
{
    MCSSSB2SSUpModulStatusStruct m_MCSSSUpModulStatus;
    struct{
        MCSSSB2TargetTCSSUpModulStatusStruct m_MCSTargetTCSSUpModulStatus;
        MCSSSB2TargetMeasSSUpModulStatusStruct m_MCSTargetMeasSSUpModulStatus;
    }TcMeas[4];
}MCSUpModuleStatusStruct4, *PMCSUpModuleStatusStruct4;
//

//扩频遥控单元状态数据元素（单元编号：05）
typedef struct
{
    MCSSSB2TCStatusStruct MCSSSB2TCStatu[4];
}MCSSSTCStatusStruct4;
//单遥控遥控发射状态
typedef struct
{
    MCSSSB2TCStatusStruct MCSSSB2TCStatu[4];
    int valid;//0标志无效。
}MCSSSTCStatus4;
typedef struct
{
    MCSSSB2TCParamStruct MCSSSB2TCPara[4];
}MCSSSTCParamStruct4;
//扩频遥测单元状态数据元素（单元编号：06）
typedef struct
{
    MCSSSB2TMStatusStruct MCSSSB2TMStatu[4];
}MCSSSB2TMStatusStruct4;


typedef struct{
    MCSSSB2TMParamStruct SSTMParam[4];
}MCSSSB2TMParamStruct4;

//扩频遥测模拟源单元状态数据元素（单元编号：08）
typedef struct
{
    MCSSSB2TMSimulStatusStruct MCSSSB2TMSimulStatu[4];
}MCSSSB2TMSimulStatusStruct4;

typedef struct
{
    MCSSSB2TMSimulParamStruct SSTMSimulParam[4];
}MCSSSB2TMSimulParamStruct4;



}
#pragma pack()

#else
#endif//SMCUSSB2_BBE_CDMA_20131005_STRUCT_H_
