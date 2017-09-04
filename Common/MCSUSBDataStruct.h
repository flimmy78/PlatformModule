//标准化接口数据结构
//MCSUSBDataStruct.h
//*******************************************************
#ifndef _MCSUSBBBE_20090615_STRUCT_H_
#define _MCSUSBBBE_20090615_STRUCT_H_

#include "TypeDefine.h"
#pragma pack(1)

//标准TT&C接收机及测速单元参数数据元素（单元编号：02、03）
typedef struct MCSUSBRevParamBlk
{
	UCHAR  ucRevChannel;    //接收通道 0：无效，1：左旋，2：右旋，3：左右旋同时（保留）
	UCHAR  ucAgcTime;		//AGC时间常数  0：无效，1：1ms，2：10ms，2：100ms，3：1000ms。
	UCHAR  ucPllBw;	    	//载波环路带宽 0：无效，1：5Hz（保留），2：10Hz（保留），3：100Hz，4：500Hz，5：1kHz，6：2kHz。
    UINT   uiCarrIF;		//输入中频，量化单位：1 Hz，取值范围：68 MHz ~ 72 MHz，缺省值为70 MHz。
//	short  sDoppOff;		//下行多普勒预置，范围：-180kHz~180kHz，步进：5 kHz
//	short  sAcquRange;     //载波频率捕获范围，量化单位：1kHz，取值范围：0 ~ 180 kHz，缺省为180kHz（即±180kHz）。
	
} MCSUSBRevParamStruct, *PMCSUSBRevParamStruct;

//标准TT&C上行调制单元参数数据元素 （单元编号：04）
typedef struct MCSUSBUpModulParamBlk
{
	UCHAR  ucWaveOutCtrl;	//载波输出控制 0：无效，1：允许，2：禁止
	char   cWaveOutLevel;	//载波输出电平：范围-30～0，单位dBm
//	UINT   uiCarIF;		    //输出载波中频，量化单位：1 Hz，取值范围为68 M ~ 72 MHz，缺省为70 MHz。
//	UCHAR  ucOffestMode;    //上行多普勒预置方式 0：无效，1：手动，2：自动
	short  ucPreDplValue;	//上行多普勒频率预置值：范围-180kHz～180kHz，步进5kHz。
	UCHAR  ucTcSw;		    //遥控加调，0：无效，1：加调，2：去调
	UCHAR  ucToneSw;		//侧音加调，0：无效，1：加调，2：去调
	UCHAR  ucAudioSw;	    //话音加调，0：无效，1：加调，2：去调
	UCHAR  ucTmsimSw;		//遥测模拟源加调，0：无效，1：加调，2：去调
	UCHAR  ucT0Index;       //主音调制度，量化单位：0.01 rad，取值范围：0.20 ~ 1.50 rad。
	UCHAR  ucTnIndex;       //次音调制度，量化单位：0.01 rad，取值范围：0.20 ~ 1.50 rad。
	UCHAR  ucTcIndex;       //遥控调制度，量化单位：0.01 rad，取值范围：0.20 ~ 1.50 rad。
	UCHAR  ucAudioIndex;    //话音调制度，量化单位：0.01 rad，取值范围：0.20 ~ 1.50 rad。
	UCHAR  ucTmsimIndex;    //遥测模拟源调制度，量化单位：0.01 rad，取值范围：0.20 ~ 1.50 rad。
	UCHAR  ucTcCntr;        //盲发开关，0：无效，1：允许，2：禁止
//	UCHAR  ucSweepMode;		//扫描方式  0：无效，1：自动，2：半自动，3：手动，4：禁止。
	int   uiScanRate;		//扫描速率：范围0～180kHz/s，步进500Hz/s
    UINT   uiScanRange;		//扫描范围：传输范围0～150kHz，单位kHz，步进5。
	UCHAR  ucNoiseSw;	    //噪声开关 0：无效，1：开，2：关
	UCHAR  ucCN0;           //载波输出载噪比，范围30～100，单位：dBHz

} MCSUSBUpModulParamStruct, *PMCSUSBUpModulParamStruct;

//标准TT&C遥控单元参数数据元素（单元编号：07）
typedef struct MCSUSBTCParamBlk
{
	UCHAR  ucModStyle;		//副载波调制体制：0：无效，1:PSK, 2:ASK,3:FSK。ASK、PSK时只有1频率副载波(副载波0频率)存在，FSK存在4个
	UCHAR  ucCodeStyle;		//码型：0：无效，1： RZ（FSK时有效）  2： NRZ-L，3： NRZ- M，
	                        //4： NRZ-S，5：= Biφ-L，6： Biφ-M，7： Biφ-S
	UINT   uiCodeRate;		//码速率：范围100～8000，单位bps
	UINT   uiSwaveFreq_1;	//副载频0频率(f0)：范围2000～100000，单位Hz,0：无效，FSK PSK体制有效
	UINT   uiSwaveFreq_2;	//副载频1频率(f1)：范围2000～100000，单位Hz,0：无效，FSK体制有效
	UINT   uiSwaveFreq_3;	//副载频填充频率(f填充)：范围2000～100000，单位Hz,0：无效，FSK体制有效
	UINT   uiSwaveFreq_4;	//副载频执行频率(f执行)：范围2000～100000，单位Hz,0：无效，FSK体制有效
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
	UCHAR  ucECodeLen;		//执行编码长度：8～64 bit（当选择FSK调制时，该项设置无效，即不允许设置）
	UCHAR  ucIndleCode;		//空闲序列码样：十六进制码（当选择突发状态时，该项设置无效，即不允许设置）
//	UCHAR  ucLCFCodeChange; //空闲序列是否码型变换，0：无效，1：是，2：否。
	UCHAR ucC8Data;			//C8内部间隔：范围：0～127，单位：Byte（PSK时为填充空闲序列长度，FSK时为填充频率的填充长度）
	USHORT usTCProctC;		//指令保护间隙：范围：100～10000，单位ms，步进100,默认1000ms
	UCHAR ucTCWorkCyc;		//遥控工作期启动方式：0：无效，1：自动，2：人工
	UCHAR ucBCHCode;        //BCH编码 01--不编码 02--编码
	UCHAR ucBCHFQueueCheck; //BCH起始序列控制 01--不加 02--加
    USHORT usBCHFQueue;     //BCH起始序列
	UCHAR ucBCHBQueueCheck; //BCH结尾序列控制 01--不加 02--加
	DWORDLONG usBCHBQueue; //BCH结尾序列
	UCHAR  ucturbCtrl;  	//加扰控制：0：无效，1：不加扰，2：加扰
	UCHAR  ucRandLen;		//加扰多项式长度：0：无效，1：1字节，2：2字节，3：3字节，4：4字节
	UINT   uiRandPoly;		//加扰多项式
    UINT   uiPhase;		    //加扰初相

	
} MCSUSBTCParamStruct, *PMCSUSBTCParamStruct;

//标准TT&C遥测单元参数数据元素（单元编号：08）
typedef struct MCSUSBTMParamBlk
{
	UINT   uiSWFreq;		//副载波频率：范围5000～512000单位Hz
	UINT   uiCodeStyle;	    //码型：0：无效，1：NRZ-L，2：NRZ-M，3：NRZ-S，4：Biφ-L，5：Biφ-M，6：Biφ-S
	UINT   uiTM_CodeRate;	//码速率：范围100～64000，单位bps
	UCHAR  ucBitOut;        //遥测数据类型:0：无效，1：帧格式；2：位流
	UCHAR  ucCodingMd;		//译码方式：0：无效，1：不译码，2：Viterbi译码，3：R-S译码，4：R-S和Viterbi级联译码，5：Turbo，6：LDPC
	UCHAR  ucViterbiMd;		//卷积码类型:0：无效，1：(7,1/2)；2：(7,3/4)；3：(7,2/3)；4：(7,5/6)；5：(7,7/8)
	UCHAR  ucG2Inv;		    //Viterbi译码相位:0：无效，1：正相；2：反相。
	UCHAR  ucRSMd;	    	//RS码类型:0：无效，1：(223,255)；2：(239,255)
	UCHAR  ucInterleav;	    //RS译码交织深度，取值范围：1 ~ 5。
	UCHAR  ucBasiSel;	    //R-S编码双基选择:0：无效，1：对偶基；2：常规基
	UCHAR  ucTurboRatio;         //TURBO码率 01--1/2 02--1/3 03--1/4 04--1/6
	UCHAR  ucLDPCRatio;          //LDPC码率，D7 ~ D0位：01H表示1/2；02H表示2/3；03H表示4/5；04H表示7/8。
	UCHAR  ucUnturbCtrl;	//解扰控制：0：无效，1：不解扰，2：自动解扰，3：人工解扰
	UCHAR  ucRandLen;		//解扰多项式长度：0：无效，1：1字节，2：2字节，3：3字节，4：4字节
	UINT   uiRandPoly;		//解扰多项式
	UINT   uiPhase;		    //解扰初相
	UCHAR  ucCRC;           //传输帧CRC校验：1：开，2：关
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

} MCSUSBTMParamStruct, *PMCSUSBTMParamStruct;

//标准TT&C测距单元参数数据元素（单元编号：05、06）
typedef struct MCSUSBCjParamBlk
{
	UCHAR  ucLSToneCheck;	//最低侧音选择：0：无效，1：0.5Hz，2：2Hz，3：8Hz，4：10Hz，5：32Hz，6：40Hz
	UINT   uiSToneWait;		//侧音等待时延：范围：0～5000，单位：ms
	UCHAR  ucRMHCheck;		//距离解模糊：0：无效，1：基带解，2：中心解
	UCHAR  ucPreRangDataMd;	//距离预报方式：0：无效，1：手动，2：轨道根数，3：理论弹道
    UINT   ulPridictR;		//距离预报值，32位无符号整型数，量化单位：1m。
	UINT   uiRSPZero;       //应答机距离零值，单位：0.01m

} MCSUSBCjParamStruct, *PMCSUSBCjParamStruct;

//标准TT&C主跟踪单元参数数据元素（单元编号：09、10）
typedef struct MCSUSBTraceParamBlk
{
/*	int    iAzSlope;		//方位斜率:整数
	int	   iElSlope;		//俯仰斜率:整数
	USHORT usAzPhaseValue;	//方位通道相位：范围0～3600，单位 0.1度
	USHORT usElPhaseValue;	//俯仰通道相位：范围0～3600，单位 0.1度*/

} MCSUSBTraceParamStruct, *PMCSUSBTraceParamStruct;

//标准TT&C引导中频接收机单元参数数据元素（单元编号：13）
typedef struct MCSUSBGuideRevParamBlk
{
//	UINT   uiCarrIF;		//输入中频，量化单位：1 Hz，取值范围：68 MHz ~ 72 MHz，缺省值为70 MHz。
	UCHAR  ucAgcTime;		//AGC时间常数  0：无效，1：250ms，2：1000ms。
	UCHAR  ucPllBw;	    	//载波环路带宽 0：无效，1：5Hz（保留），2：10Hz（保留），3：100Hz，4：500Hz，5：1kHz，6：2kHz。
	UCHAR  ucInput;		//输入选择，8位无符号整型数。D7 ~ D0位：01H表示左旋；02H表示右旋。
	
} MCSUSBGuideRevParamStruct, *PMCSUSBGuideRevParamStruct;

//标准TT&C引导跟踪单元参数数据元素（单元编号：14）
typedef struct MCSUSBGuideTraceParamBlk
{
/*	int    iAzSlope;		//方位斜率:整数
	int	   iElSlope;		//俯仰斜率:整数
	USHORT usRefDelay;  	//时延修正：单位 0.01度*/

} MCSUSBGuideTraceParamStruct, *PMCSUSBGuideTraceParamStruct;

//标准TT&C话音单元参数数据元素（单元编号：11）
typedef struct MCSUSBAudioParamBlk
{
	UINT   uiSWFreq;		//副载波频率：范围100～512单位kHz
	UCHAR  ucCodeStyle;	    //码型：0：无效，1：NRZ-L，2：NRZ-M，3：NRZ-S
	UCHAR  ucCodeRate; 	    //码速率：范围16~96，步长8kbit/s，(副载波频率/码速率=整数，并≥3)
	UCHAR  ucSource;		//调制数据源：0：无效，1：外源，2：内源
    UINT   uiSourceData;    //内调制数据，0：无效
	UCHAR ucAudioNum; //话音缓存包数

} MCSUSBAudioParamStruct, *PMCSUSBAudioParamStruct;

//标准TT&C遥测模拟源单元参数数据元素（单元编号：12）
typedef struct MCSUSBTMSimulParamBlk
{
	UCHAR  ucDataCheck;	    //数据源选择 0：无效，1：内，2：外。
	UCHAR  ucdatasel;	    //模拟数据，0：无效，1：固定码，2：随机码，3：顺序数，4：自定义
	UCHAR   uidata;		    //固定码数据
	UINT   uiSWFreq;		//副载波频率：范围5000～512000单位Hz
	UINT   uiCodeStyle;	    //码型：0：无效，1：NRZ-L，2：NRZ-M，3：NRZ-S，4：Biφ-L，5：Biφ-M，6：Biφ-S
	UINT   uiTM_CodeRate;	//码速率：范围100～64000，单位bps
	UCHAR  ucCodingMd;		//编码方式：0：无效，1：不编码，2：卷积编码，3：R-S编码，4：R-S和卷积级联编码，5：Turbo，6：LDPC
	UCHAR  ucViterbiMd;		//卷积码类型:0：无效，1：(7,1/2)；2：(7,3/4)；3：(7,2/3)；4：(7,5/6)；5：(7,7/8)
    UCHAR  ucG2Inv;		    //卷积编码相位:0：无效，1：正相；2：反相。
	UCHAR  ucRSMd;	    	//RS码类型:0：无效，1：(223,255)；2：(239,255)
	UCHAR  ucInterleav;	    //RS编码交织深度，取值范围：1 ~ 5。
	UCHAR  ucBasiSel;	    //R-S编码双基选择:0：无效，1：对偶基；2：常规基
	UCHAR  ucTurboRatio;         //TURBO码率 01--1/2 02--1/3 03--1/4 04--1/6
	UCHAR  ucLDPCRatio;          //LDPC码率，D7 ~ D0位：01H表示1/2；02H表示2/3；03H表示4/5；04H表示7/8。
	UCHAR  ucturbCtrl;  	//加扰控制：0：无效，1：不加扰，2：自动加扰，3：人工加扰
	UCHAR  ucRandLen;		//加扰多项式长度：0：无效，1：1字节，2：2字节，3：3字节，4：4字节
	UINT   uiRandPoly;		//加扰多项式
	UINT   uiPhase;		    //加扰初相
	UCHAR  ucCRC;           //传输帧CRC校验：1：开，2：关
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
	
} MCSUSBTMSimulParamStruct, *PMCSUSBTMSimulParamStruct;

//标准TT&C接收机及测速单元状态数据元素（单元编号：02、03）
typedef struct MCSUSBRevStatusBlk
{
	UCHAR  ucLockStatus;    //载波锁定指示 0：无效，1：锁定，2：失锁。
	UCHAR  ucFollow;        //随扫指示，0：无效，1：未随扫，2：随扫
	USHORT usAgcVolt;		//AGC电压，量化单位：0.001 V
	USHORT usSqDir;		    //S/Φ：单位0.1dBHz
	__int64    iDopRate;        //实测多普勒频率，单位：0.01 Hz
	int    iSpeed;          //实测速度值，单位：0.0001 m/s
	MCSUSBRevParamStruct	tParams;	//参数数据
	
} MCSUSBRevStatusStruct, *PMCSUSBRevStatusStruct;

//标准TT&C上行调制单元状态数据元素 （单元编号：04）
typedef struct MCSUSBUpModulStatusBlk
{
	UCHAR ucCarriRetZero;                   // added by yangf(todo: =value)
	MCSUSBUpModulParamStruct	tParams;	//参数数据
	

} MCSUSBUpModulStatusStruct, *PMCSUSBUpModulStatusStruct;

//标准TT&C遥控单元状态数据元素（单元编号：07）
typedef struct MCSUSBTCStatusBlk
{
	UCHAR  ucTCStatus;      //遥控发令指示 0：无效，1：正在发令，2：未发令
	UINT   uiSendType;      //发令类别(填写对应的B字段)
	USHORT   uiA2TaskID;      //显示指令代号(遥控代号发令方式下有效)
	UINT   uiTCStartTime;      //发令开始时间
	UINT   uiTCStopTime;      //发令结束时间
	UCHAR ucTCWorkCyc;		//遥控工作期：0：无效，1：开始，2：结束
//	USHORT	uiAllCmdSendTotal;		//发出指令总数
//	USHORT	uiAllSmallLoopRight;	//小环正确总数
	UCHAR  ucFTBCStatus;      //非同控遥控发令指示 0：无效，1：初始，2：等待，3：运行
    UINT  ulFTBDdeltaT;       //非同控脉冲宽度;
    UINT  ulFTBDdeltaTExe;    //非同控脉冲已发宽度;
	USHORT   usPulseNum;	//非同控脉冲脉冲个数，16位无符号整型数，量化单位：1 次。
	USHORT    ulPeriod;		//非同控脉冲周期，32位无符号整型数，量化单位：1 ms。
	USHORT    uldeltaT;		//非同控脉冲宽度，32位无符号整型数，量化单位：1 ms。
	USHORT   usSendPulseNum;	//已发送脉冲个数，16位无符号整型数，量化单位：1 次。
    USHORT    ulSendPeriod;		//已发送脉冲周期，32位无符号整型数，量化单位：1 ms。
	USHORT    ulSenddeltaT;		//已发送脉冲宽度，32位无符号整型数，量化单位：1 ms。	 
	USHORT    ulSendPrd;		//当前周期已执行时间，32位无符号整型数，量化单位：1 ms。
	USHORT    ulSendT;		//当前脉冲已发送宽度，32位无符号整型数，量化单位：1 ms。	
	UCHAR  ucSELFLock;		//自环比对结果，0：无效；1：正常；2：异常。
	UCHAR  ucSMLLock;		//小环锁定指示，0：无效，1：锁定，2：失锁。
	UCHAR  ucSMLLoop;		//小环比对，0：无效；1：正常；2：异常。
	MCSUSBTCParamStruct	tParams;	//参数数据
	
} MCSUSBTCStatusStruct, *PMCSUSBTCStatusStruct;

//标准TT&C遥测单元状态数据元素（单元编号：08）
typedef struct MCSUSBTMStatusBlk
{
	UCHAR  ucRevCarrLock;   //遥测副载波锁定 0：无效，1：锁定，2：失锁
	UCHAR  ucCodeLock;		//码同步器锁定指示，0：无效，1：锁定，2：失锁
	UCHAR  ucFrmLock;		//帧同步器锁定指示，0：无效，1：锁定，2：失锁
	UCHAR  ucSubFrmLock;	//副帧同步器锁定指示，0：无效，1：锁定，2：失锁
//	UINT   uiErrBitRate;    //遥测误码率
//	UINT   uiTotalFrames;    //遥测总帧数
//	UINT   uiErrFramesNum;   //误帧数
	USHORT usEbN0;          //Eb/N0，单位：0.1dB
	UCHAR  ucVFill;		    //虚拟填充开关，0：无效，1：不填充，2：填充。
	MCSUSBTMParamStruct	tParams;	//参数数据

} MCSUSBTMStatusStruct, *PMCSUSBTMStatusStruct;

//标准TT&C测距单元状态数据元素（单元编号：05、06）
typedef struct MCSUSBCjStatusBlk
{
	UCHAR  ucStatus;        //距捕状态，0：无效，1：锁定，2：失锁
	UCHAR  ucLock;			//主音锁定指示，0：无效，1：锁定，2：失锁
	UCHAR  ucCurTone;   	//当前发送次音：0：无效，1：未发送次音，2：20，3：16，4：16.8，5：16.16，6：16.04
	                        //7：16.032，8：16.010，9：16.008，10：16.002，11：16.0005
	int    iDistance;       //实测距离值，单位：0.1m
	MCSUSBCjParamStruct	tParams;	//参数数据

} MCSUSBCjStatusStruct, *PMCSUSBCjStatusStruct;

//标准TT&C主跟踪单元状态数据元素（单元编号：09、10）
typedef struct MCSUSBTraceStatusBlk
{
	SHORT  siAzimuV;		//方位误差电压，量化单位：mV
	SHORT  siElevaV;		//俯仰误差电压，量化单位：mV
//	MCSUSBTraceParamStruct	tParams;	//参数数据

} MCSUSBTraceStatusStruct, *PMCSUSBTraceStatusStruct;

//标准TT&C引导中频接收机单元状态数据元素（单元编号：13）
typedef struct MCSUSBGuideRevStatusBlk
{
	UCHAR  ucLockStatus;    //载波锁定指示 0：无效，1：锁定，2：失锁。
	USHORT usAgcVolt;		//AGC电压，量化单位：0.001 V
	USHORT usSqDir;		    //S/Φ：单位0.1dBHz
	MCSUSBGuideRevParamStruct	tParams;	//参数数据
	
} MCSUSBGuideRevStatusStruct, *PMCSUSBGuideRevStatusStruct;

//标准TT&C引导跟踪单元状态数据元素（单元编号：14）
typedef struct MCSUSBGuideTraceStatusBlk
{
	SHORT  siAzimuV;		//方位误差电压，量化单位：mV
	SHORT  siElevaV;		//俯仰误差电压，量化单位：mV
//	MCSUSBGuideTraceParamStruct	tParams;	//参数数据

} MCSUSBGuideTraceStatusStruct, *PMCSUSBGuideTraceStatusStruct;

//标准TT&C话音单元状态数据元素（单元编号：11）
typedef struct MCSUSBAudioStatusBlk
{
	UCHAR  ucSELLock;		//自环比对，0：无效，1：正确，2：错误。
	UCHAR  ucSMLLoop;		//小环比对，0：无效，1：正确，2：错误。
	UCHAR  ucAudioSendSwitch;	//测速送数控制	0：无效，1：送数，2：停送 
	MCSUSBAudioParamStruct	tParams;	//参数数据

} MCSUSBAudioStatusStruct, *PMCSUSBAudioStatusStruct;

//标准TT&C遥测模拟源单元状态数据元素（单元编号：12）
typedef struct MCSUSBTMSimulStatusBlk
{
	MCSUSBTMSimulParamStruct	tParams;	//参数数据

} MCSUSBTMSimulStatusStruct, *PMCSUSBTMSimulStatusStruct;

#pragma pack()

#else
#endif // _MCSUSBBBE_20090615_STRUCT_H_
