//标准化接口数据结构
//MCSDataStruct.h

//*******************************************************
#ifndef _MCSBBE_20090615_STRUCT_H_
#define _MCSBBE_20090615_STRUCT_H_
#include "TypeDefine.h"
#include"SMCUHeadFile.h"
#pragma pack(1)


//帧头
//typedef struct MCSProtocolHeaderBlk
//{
//	MCSAddressStruct  SID;	//信源标识（SID）
//	MCSAddressStruct  DID;	//信宿标识（DID）
//	UINT   uiBID;			//信息类别
//	UCHAR  ucReserved[4];   //保留，固定填0
//	//UINT   uiDate;			//日期
//	USHORT   uiDate;			//日期
//	UINT   uiTime;		    //时间
//	UINT   uiLength;		//数据字段(DATA)的字节长度，单位：字节
	
//} MCSProtocolHeaderStruct, *PMCSProtocolHeaderStruct;

//下面定义数据元素
//公共单元参数数据元素（单元编号：01）
namespace GLOBAL_USE {
typedef struct MCSSysParamStructBlk
{
    UINT   uiM ;		    //任务标识（M字段）
    UCHAR  ucTaskDescribe[6];//任务代号
    UINT   dwlAnswerRevDownFreq;//下行工作频率，范围22000000~23000000，步进0.1MHz
    DWORDLONG   dwlComboID; //设备组合号
    UINT   uiSystemWorkMode;//工作模式
} MCSSysParamStruct, *PMCSSysParamStruct;
}

typedef struct MCSSysParamStructBlk
{
    UCHAR  ucTaskDescribe[6];//任务代号
    UINT   uiM ;		    //任务标识（M字段）
    UCHAR ucSendMode;//基带送数选择 0: 无此参数, 1: 常送, 2: 人工.
    UINT   dwlAnswerRevDownFreq;//下行工作频率，范围22000000~23000000，步进0.1MHz
    UCHAR ucDataTransferProcessFlag;//数据传输处理标志码, 用于PDXP协议中
    UINT uiInfoTransferFormat;//信息传输格式.
    UCHAR ucDDTOutFormat;//数传类别, 0: 无此参数, 1: 921数传, 2:  综合数传.
    UCHAR ucDDTSendSwitchI;//数传I路送数 0：无效，1：送数，2：停送
    UCHAR ucDDTSendSwitchQ;//数传Q路送数 0：无效，1：送数，2：停送
} MCSSysParamStruct, *PMCSSysParamStruct;
//分集接收机数据元素（单元编号：02）
typedef struct MCSDivRevParamBlk
{
	USHORT    usWorkMode;       //工作模式,16位无符号整型数，0表示Fm分集接收;1表示Fm单通道左旋工作;2表示Fm单通道右旋工作

    //UINT      uiLCarrFreq;      //Fm接收机左旋中心频率,32位无符号整型数,单位Hz,
	                            //取值范围69000000 ~ 71000000,缺省为70000000
    //UINT      uiRCarrFreq;      //Fm接收机右旋中心频率,32位无符号整型数，单位Hz,
								//取值范围69000000 ~ 71000000,缺省为70000000
    UINT      uiCarrCapRange;   //载波频率捕获范围,32位无符号整型数,单位kHz,取值范围0 ~ 1000,缺省值为1000
	
    USHORT	  ucAfcCtrl;        //Afc闭环使能,8位无符号整型数,0表示允许载波闭环，1表示禁止载波闭环,此设置项缺省值为0	
    USHORT    usAfc;            //Afc时间常数,单位ms,设置值可为3、15*、60和300,缺省值为60,
	
    //USHORT	  ucAgcMode;        //AGC控制方式，00为手动，01为自动。缺省值为01
    //USHORT    usAgcA;           //左旋（A路）信号手动AGC控制量,设置范围从0xC0～0x600,缺省值为0x120
    //USHORT    usAgcB;           //右旋（B路）信号手动AGC控制量,设置范围从0xC0～0x600,缺省值为0x120
    UCHAR	  ucAgcTime;        //时间常数（AgcTime），共2位，单位ms,
								//设置为0、1、2和3、4、5分别代表时间常数为10、50、100和1000、1、3,缺省值为2
    UINT      uiIfBn;           //中频带宽,32位无符号整型数，单位为Hz,设置值为300000(300kHz）、500000(500k)、750000(750k)、
								//1000000(1000k)、1500000(1.5M)、2400000(2.4M)、3300000(3.3M)、4000000(4M)、6000000(6M)、
								//10000000(10M)、15000000(15M)、20000000(20M)、24000000(24M)，1自适应带宽，不必出现在用户界面

    USHORT    usDivPllBw;       //差模环带宽，单位Hz，设置范围150至4800,设置值可为150、600、2400
	USHORT    usMolRate;    //调制指数，范围0.1~20，单位0.01，默认值0.7
	//UCHAR   Track_Sel;     //角跟踪类型选择，0：单通道单脉冲，1：多模馈源，2：双通道单脉冲
	//USHORT   usAEBw;      // 角误差输出带宽。量化单位：0.1Hz，取值范围：5.0Hz～1000Hz，精度0.1Hz，未用
        UCHAR ucEquSwitch; //均衡开关 0：关闭 1： 打开
} MCSDivRevParamStruct, *PMCSDivRevParamStruct;

//上行调制单元参数数据元素 （单元编号：03）
typedef struct MCSUpModulParamBlk
{
	UCHAR	ucOnline;			// 界面设置，加载/去载：0x00：去载，0x01：加载
	UCHAR	ucModu;				// 界面设置，信标模式固定为0x00，加调/去调：0x00：去调，0x01：加调
	UCHAR	ucOutBand;			// 界面设置，载波频段：0x00：S频段，0x01：X频段（不显示），0x02：Ka频段

	UINT	unCarrIf;			// 界面设置，载波中频，量化单位：1Hz，68~72MHz

	INT		nCarrOffSet;		// 界面设置，载波多普勒预置，量化单位：1Hz，取值范围：-2MHz~2MHz，缺省值0

	UCHAR	ucLevelAdSw;		// 界面设置，电平修正开关：0x00：关，0x01：开

	SHORT	sLevel;				// 界面设置，输出功率，量化单位0.1dBm，取值范围：-70.0dBm~0.0dBm
		  
	USHORT	usFM_ModuOffSet;	// 界面设置（仅FM设置），调制度。量化单位：0.01，取值范围：0.1~20，缺省值0.7
	UCHAR	ucNoiseSw1;			// 界面设置，输出通道1噪声开关：0x00：关，0x01：开
	UCHAR	ucNoiseSw2;			// 界面设置，输出通道2噪声开关：0x00：关，0x01：开
		  
	USHORT	usC_N01;			// 界面设置，通道1输出信噪比，仅在(NoiseSw1 == 0x01)时有效。量化单位：0.1dBHz，取值范围：0.0~100.0dBHz。
	USHORT	usC_N02;			// 界面设置，通道2输出信噪比，仅在(NoiseSw2 == 0x01)时有效。量化单位：0.1dBHz，取值范围：0.0~100.0dBHz。
		  
	UCHAR	ucDoplSimSw;		// 界面设置，多普勒文件模拟开关，仅在(SimSw == 0x01)时有效，0x00：关，0x01：开 
	INT	usSimRnge;			// 界面设置（仅单信标体制设置），仅(ucDoplSimSw == 0x01)时有效，
							// 多普勒模拟范围，量化单位：1kHz，取值范围：-1000~1000kHz。
		  
	INT		nSimRate;			// 界面设置（仅单信标体制设置），仅(SimDoplSw == 0x01)时有效，
	                            // 多普勒模拟多谱勒变化率，量化单位：1Hz，取值范围：-200000Hz/s~200000Hz/s（即±2400 kHz）。
        UCHAR   ucAPhaseSW;          //a支路相位变化开关 0：关（默认） 1：开
        UCHAR   ucASignalSW;         //a支路信号延迟开关 0：关（默认） 1：开
        UCHAR   ucBPhaseSW;          //b支路相位变化开关 0：关（默认） 1：开
        UCHAR   ucBSignalSW;         //b支路信号延迟开关 0：关（默认） 1：开

        USHORT    wAInitPhase;         //a支路初始相位 0~359.99度 量化单位0.01度，默认值为0
        USHORT    wAChangePhase;       //a支路相位变化率 0~180.0度 量化单位0.1度/秒，默认值为0
        USHORT    wBInitPhase;         //b支路初始相位 0~359.99度 量化单位0.01度，默认值为0
        USHORT    wBChangePhase;       //b支路相位变化率 0~180.0度 量化单位0.1度/秒，默认值为0
        USHORT    wADelay;             //a支路时延大小 10~10000 ns 量化单位5ns，默认值为0
        USHORT    wBDelay;             //b支路时延大小 10~10000 ns 量化单位5ns，默认值为0

} MCSUpModulParamStruct, *PMCSUpModulParamStruct;

//遥测单元参数数据元素（单元编号：04）
typedef struct MCSTMParamBlk
{
	UCHAR   ucCodeStyle;	    //码型：00H表示NRZ-L；01H表示NRZ-M；02H表示NRZ-S；03H表示Biφ-L；
							//04H表示Biφ-M；05H表示Biφ-S；06H表示RNRZ-L。
	UINT	uiCodeRate;			//码速率，32位无符号整型数，
						//调频模式定义：量化单位：1 bps，取值范围：1000 ~ 10000000，每bps步进。缺省值为2000000。
						//《任务书》要求单路1kbps ~ 2Mbps，每kbps步进。
	UCHAR	usCodeBn;			//码环带宽,16位无符号整型，0表示1%，1表示0.5%，2表示0.1%，其他位保留。

	UCHAR  ucWordLen;		//字长，8,16位
	UCHAR	ucFspLen;			//帧同步码组长,8位无符号整型数，量化单位：1 位，取值范围：8 ~ 32 位。
	USHORT	usFrmLen;			//帧长,16位无符号整型数，量化单位：1 字，取值范围：4 ~ 4096 字。
	UINT	dwlFsp;			//帧同步码组。
	UCHAR	ucSearchErrBits;	//搜索容错位数，取值范围：0 ~ 7 位；
	UCHAR	ucVerifyErrBits;	//校核容错位数，取值范围：0 ~ 7 位；
	UCHAR	ucLockErrBits;		//锁定容错位数，取值范围：0 ~ 7 位。缺省值均为0。
	UCHAR	ucVerifyCheckFrame;	//三态逻辑：校核检测帧数，取值范围：1 ~ 8 副帧,缺省值为2 帧
	UCHAR	ucLockCheckFrame;	//三态逻辑：锁定检测帧数，取值范围：1 ~ 8 副帧缺省值为3 帧
	UCHAR	ucSubFrmType;		//副帧方式，8位无符号整型数。
								//00H表示无副帧；01H表示ID副帧；02H表示循环副帧；03H表示反码副帧 缺省值为无副帧
	USHORT	usSubFrmLen;		//副帧长度，16位无符号整型数，单位：帧，取值范围：2 ~ 256 帧，缺省值为256帧。
	USHORT	usIDSubFrmPos;		//ID副帧ID字位置，单位：字；取值范围：1 ~ 1024
	UCHAR	ucIdBase;			//ID字基值，取值范围：0或1；
	UCHAR	ucIdDirect;			//ID计数方向，0：加计数，1：减计数。
	DWORDLONG dwlCycFrmSynCode;	//循环副帧码组,64位无符号整型数。
	USHORT	usCycFrmSynCodePos;	//循环副帧码组位置,16位无符号整型数，量化单位：1 字，取值范围：1 ~ 4096 字
	USHORT	ucCycFrmSynCodeLen;	//循环副帧码组长度,16位无符号整型数，量化单位：1 位，取值范围：16 ~ 32 位
	UCHAR	ucCFSearchErrBits;	//循环副帧搜索容错位数，取值范围：0 ~ 7 位；
	UCHAR	ucCFVerifyErrBits;	//循环副帧校核容错位数，取值范围：0 ~ 7 位；
	UCHAR	ucCFLockErrBits;	//循环副帧锁定容错位数，取值范围：0 ~ 7 位。缺省值均为0
	UCHAR	ucSubFVerifyFrame;	//副帧三态逻辑：校核检测帧数，取值范围：1 ~ 8 副帧；
	UCHAR	ucSubFLockFrame;	//副帧三态逻辑：锁定检测帧数，取值范围：1 ~ 8 副帧。
	UCHAR	ucMSD;				//多符号检测，D7 ~ D0位：00H表示否；01H表示是。当选择MSD时，需要以“弹出对话框”的形式作出以下提示“要求调制指数在h=0.7+-0.025范围”
	UCHAR	ucTPCCode;			//TPC码，D7 ~ D0位：00H表示否；01H表示是。
	//USHORT  usEmptyFsp;			//Tpc空包标志字节，0x0000为空包；0xffff为有效数据包，默认0x0000
	UINT    uiMSDFsp;			//块同步帧同步码组，默认1ACFFC1D
	UCHAR	ucMSDSearchErrBits;	//MSD块同步搜索容错位数，取值范围：0 ~ 7 位；
	UCHAR	ucMSDVerifyErrBits;	//MSD块同步循环副帧校核容错位数，取值范围：0 ~ 7 位；
	UCHAR	ucMSDLockErrBits;	//MSD块同步循环副帧锁定容错位数，取值范围：0 ~ 7 位。缺省值均为5位

} MCSTMParamStruct, *PMCSTMParamStruct;

//跟踪接收机单元参数数据元素（单元编号：06）
typedef struct MCSTraceParamBlk
{
        //BYTE byRcvType;		// 跟踪类型；命令界面设置
						// 0x00：保留；
						// 0x01：多模馈源。
						// 0x02：双通道单脉冲
	                    //对于YW-2601，多模馈源和双通道单脉冲可选，对于YW-2602，仅双通道单脉冲可选
        //USHORT  rsRefFreq;  //基准信号频率，16位无符号整型数。多模馈源：00H表示1Hz，01H表示10kHz；默认为00H
	                    //                                双通道单脉冲固定为00H

// 	USHORT AEBw;		//角误差电压输出带宽，取值范围5.0~100.0Hz，量化单位0.1Hz
// 	USHORT ucWordMode;   // 工作模式；界面设置。0x00：分集接收；0x01：单左旋工作；0x02：单右旋工作；
// 	USHORT Pinhua_BW;//角误差平滑带宽，取值范围1Hz，10Hz，20Hz，100Hz，1000Hz
// 	UCHAR  CHWtMode;//加权模式，0表示自动加权，1表示手动加权
// 	UCHAR  CHWtLeftcoEfficient;//左旋手动加权系数，手动加权有效，范围0~1，缺省值为0.5，左右旋加权系数和为1；
// 	UCHAR  CHWtRightcoEfficient;//右旋手动加权系数，手动加权有效，范围0~1，缺省值为0.5，左右旋加权系数和为1；
    UCHAR ucSwitch; // 工作开关；固定设置为1。
            // 0x00：关；
            // 0x01：开。

    UCHAR ucWordMode;   // 工作模式；界面设置。
                // 0x00：信号跟踪模式；
                //0x01：信号校相模式；

    SHORT usSensitivity;  // 检测灵敏度；校相界面设置。
                    // 量化单位：1 mV/mil。

    SHORT sAzimuPoint;  // 天线方位指向；校相界面设置。
                    // 量化单位：0.01 mil。

    SHORT sElevaPoint;  // 天线俯仰指向；校相界面设置。
                    // 量化单位：0.01 mil。

    SHORT sAzimuZero;   // 方位零值修正；固定设置为0。
                    // 量化单位：10/(2^15) V。

    SHORT sElevaZero;   // 俯仰零值修正；固定设置为0。
                    // 量化单位：10/(2^15) V。

    SHORT sMeasNum;     // 测量次数；校相界面设置。
                    // 表示自动校准时方位、俯仰电压测量2^MeasNum次，取平均值上报。

//	SHORT sRsvd_a;      // 此项目保留不用
//                        // 基准信号频率；界面设置。
//                        // 量化单位：1 Hz，缺省值为1000Hz。

    BYTE byRcvType;		// 接收类型；命令界面设置
                                            // 0x00：单通道单脉冲；
                                            // 0x01：多模馈源。
                        // 0x02：双通道单脉冲
                        //对于YW-2601，多模馈源和双通道单脉冲可选，对于YW-2602，仅双通道单脉冲可选
    BYTE : 8;

    INT nAzimuSlope;    // 方位斜率修正，界面设置。

    INT nElevaSlope;    // 俯仰斜率修正，界面设置。

    SHORT sAzimuPhase;  // 方位相位修正：量化单位：360/1024，取值范围：0~1023。
    SHORT sElevaPhase;  // 俯仰相位修正：量化单位：360/1024，取值范围：0~1023。
    UINT unDelayT;      // 此项目保留不用
    SHORT sCycleNum;    // 此项目保留不用
    SHORT sAEBw;        // 角误差电压输出带宽；界面设置。16位无符号整数，取值范围5.0~100.0Hz，量化单位0.1Hz。保留不用。

    USHORT usRefFreq;   // 基准信号频率；界面设置。
                    // 量化单位：1 Hz，缺省值为1000Hz。

    USHORT usRefDelay;  // 基准时延修正；界面设置。
                    // 表示输入基准与本地基准的时延修正量，量化单位：65536/360。
                                            // 取值范围：00～359.990。


    UCHAR ucOutType;    // 此项目保留不用
    UCHAR ucOutPut;     // 此项目保留不用
                    // 接收载波为多模馈源条件下有效；界面设置。
                    // 0x00：左旋；
                    // 0x01：右旋；
                    // 0x02：合成。

    UCHAR ucHoldTh;     // 此项目保留不用
    UCHAR ucHoldeTime;  // 此项目保留不用

    USHORT usTrackOutTh;// 此项目保留不用
    USHORT usRsvd_b;    // 此项目保留不用

    UCHAR ucParaSet;    // 0x00：部分参数更新
                    // 0x01；全部参数更新
    UCHAR ucRsvd0;
    USHORT usErrorChaPhase;//16位无符号整数，通道相位差，量化单位：360/1024，取值范围0~1023（双通道单脉冲使用）
    USHORT AEBw;//基准信号频率，取值范围5.0~100.0Hz，量化单位0.1Hz
    USHORT Pinhua_BW;//角误差平滑带宽，取值范围1Hz，10Hz，20Hz，100Hz，1000Hz
    UINT unRsvd[14];
} MCSTraceParamStruct, *PMCSTraceParamStruct;

//角误差输出单元参数数据元素（单元编号：07）
typedef struct MCSLeadRevOutSigPara
{
	USHORT ucWordMode;   // 工作模式；界面设置。0x00：分集接收；0x01：单左旋工作；0x02：单右旋工作；
	UCHAR  CHWtMode;//加权模式，0表示自动加权，1表示手动加权
	UCHAR  CHWtLeftcoEfficient;//左旋手动加权系数，手动加权有效，范围0~1，缺省值为0.5，左右旋加权系数和为1；
	UCHAR  CHWtRightcoEfficient;//右旋手动加权系数，手动加权有效，范围0~1，缺省值为0.5，左右旋加权系数和为1；
	USHORT AEBw;		//角误差电压输出带宽，取值范围5.0~100.0Hz，量化单位0.1Hz
	USHORT Pinhua_BW;//角误差平滑带宽，取值范围1Hz，10Hz，20Hz，100Hz，1000Hz
} MCSLeadRevOut, *pMCSLeadRevOutPara;

//遥测模拟源单元参数数据元素（单元编号：05）
typedef struct MCSTMSimulParamBlk
{
	UINT     uiCodeRate;        //码率 单位bps TT&C遥测范围：100kbps~10Mbps		     
	UCHAR    ucCodeType;        //码型 00--NRZ-L 01--NRZ-M 02--NRZ-S 03--Biφ-L 04--Biφ-M 05--Biφ-S 06--RNRZ-L

	UCHAR    ucWordLen;           //字长 8,16位
	UCHAR    ucFspLen;            //帧同步码组长 8~32位 
	USHORT   usFrmLen;            //帧长 2~4096字
	//		
	UINT     szFsp;            //帧同步码组

	UCHAR    ucSubFrmType;		  //副帧类型 00--无副帧 01--ID副帧 02--循环副帧 03--反码副帧
	USHORT   usSubFrmLen;	      //副帧长 2~256 帧
	
	USHORT   usIDSubFrmPos;	      //ID字位置[1,4096] 单位：字 
	UCHAR    ucIdBase;            //ID基值 0或1
	UCHAR    ucIdDirect;           //ID计数方向 00--加计数 01--减计数
	
	UINT64     szCycFsp;         //循环副帧同步码组
		 //
	USHORT   usCycFrmSynCodePos;  //循环副帧同步码组位置[1,4096] 单位：字 范围与帧长、同步码组位置、同步码组长有关
	USHORT    ucCycFrmSynCodeLen;         //循环副帧同步码组长度[16,32] 单位：位

	UCHAR   DataType;            //模拟源数据形式，00H表示固定码，01H表示随机码
	UCHAR	Data;				//仅在数据形式为固定码时有效。遥测模拟源数据内容。
	UCHAR   TPCCodeSel;             //0x0表示不选择TPC编码，0x1表示选择TPC编码
	UINT	uiMSDFsp;			//块同步帧同步码组，32位无符号数，默认1ACFFC1D
} MCSTMSimulParamStruct, *PMCSTMSimulParamStruct;

//检测输入输出参数数据元素（单元编号：08）

typedef struct SCBBETestOutSetBlk
{
	UCHAR Digital_Check;//J3数字检测信号选择输出选择，界面设置，0：输出时码信息；1：输出1kc/2kc角误差基准
	UCHAR Analog_Check;//低速DA模拟检测信号选择输出选择，界面设置，0：检测载波捕获时间；1：输出角误差包络；2：输出1kc/2kc角误差基准

} SCBBETestOutParamStruct, *PSCBBETestOutParamStruct;


//公共单元状态数据元素（单元编号：01）
typedef struct MCSSysStatusStructBlk
{
    UCHAR  ucMonitorMode;	//监控方式	0：无效， 1：分控， 2：本控
    UINT  dwSystemMode;	//工作模式,01：Ka模式，02：Ka+S模式1（A机Ka，B机S），04：Ka+S模式2（B机Ka，A机S）
    //08：S模式1（1个点频），10：S模式2（2个点频）
    UCHAR  ucSystemWorkMode;//系统工作方式，  01:任务方式 0A:调频遥测中频闭环
    //0C:记录回放（A机调频，B机扩频）  0D:其他方式
    //UCHAR  ucBBEOnline;		//本机在线状态	0：无效，1：在线，2：备份
    UCHAR  ucTimeCodeStatus;//时码状态	0：无效，1：故障，2：正常
    UCHAR  ucBBETimeMode;   //基带时统模式  0：无效，1：内时统，2：外时统
    //UCHAR  ucPingBiaoStatus;//外频标状态	0：无效，1：故障，2：正常


    UCHAR  ucDTETranStatus;//1:故障，2正常
    //UCHAR  ucBBEWorkStatus;	//基带设备综合状态	0：无效，1：故障，2：正常

    UCHAR  FMTMBEEBoardWorkState;//调频遥测基带板工作状态：正常，故障
    UCHAR  SupplyElectricity;//供电状态：正常，异常
    UCHAR  HardWareTemperature;//硬件温度，量化单位：1度
    SHORT Power_L;//左旋功率，量化单位0.1dBm，显示范围-60.0dBm~0.0dBm
    SHORT Power_R;//右旋功率，量化单位0.1dBm，显示范围-60.0dBm~0.0dBm
    UCHAR TMDataTranStatus; //遥测数据送数状态 0：无此参数，1：发送，2：未发送
    UCHAR TMDataSaveStatus; //遥测数据存盘状态 0：无此参数，1：正在存盘，2：未存盘
    UCHAR ucMemStoreStatus; //存盘空间状态 1：正常 2：预警（500M） 3：告警（10M）

    UCHAR  ucOuterTimeWorkStatus;//1:异常 0 正常
    UCHAR  ucChannelPowerFull;//1:饱和 0 不饱和
    UCHAR  ucBaseSignal;//1:异常 0 正常
    UCHAR  ucCarrierDoppl;//1:异常 0 正常
    UCHAR  ucCodeDoppl;//1:异常 0 正常

    UCHAR  ucFMTMOnline;//1:在线，2：不在线
    GLOBAL_USE::MCSSysParamStruct	tParams;	//参数数据

} MCSSysStatusStruct, *PMCSSysStatusStruct;

//分集接收机状态数据元素（单元编号：02）
typedef struct MCSDivRevStatusBlk
{
    INT      iCarrDoppFreq;       //载波多普勒频率，32位有符号整型数,单位0.01Hz
    UCHAR    ucCarrLk;            //为1表示共模环锁定,为0表示共模环失锁
    UCHAR    ucCarrDivLk;         //为1表示差模环锁定,为0表示差模环失锁
    USHORT	 usAgcVoltL;          //左旋通道的AGC电压,单位毫伏(mV),范围从0到5000,代表电压值为0～5V，对上报值进行除以1000的处理
    //各通道AGC电压为dB线性，为0时代表输入中频信号功率为-70dBm,
    //为5000mV时代表输入中频信号功率为+10dBm
    USHORT	 usAgcVoltR;          //右旋通道的AGC电压,单位毫伏(mV),范围从0到5000,代表电压值为0～5V，对上报值进行除以1000的处理
    //各通道AGC电压为dB线性，为0时代表输入中频信号功率为-70dBm,
    //为5000mV时代表输入中频信号功率为+10dBm
    UINT	 uiSnrL;              //左旋信号的信噪比 S/N = 10*lg(uiSnrL)
    UINT	 uiSnrR;              //右旋信号的信噪比S/N = 10*lg(uiSnrR)
    UINT	 uiSnrC;              //合成信噪比S/N = 10*lg(uiSnrC)
    MCSDivRevParamStruct	tParams;	//参数数据

} MCSDivRevStatusStruct, *PMCSDivRevStatusStruct;

//上行调制单元状态数据元素 （单元编号：03）
typedef struct MCSUpModulStatusBlk
{
	////wayne 20130425成都 以下4个参数改为有符号数
	INT		nDoplNco1;      // 界面显示多普勒频偏，多普勒频偏 = DoplNco1 * 100e6 / 2^32
	INT		nDoplNco2;      // 界面显示多普勒频偏，多普勒频偏 = DoplNco2 * 100e6 / 2^32
		
	SHORT	sAm1;           // 界面显示输出信号功率，量化单位0.1dBm
	SHORT	sAm2;           // 界面显示输出信号功率，量化单位0.1dBm
	MCSUpModulParamStruct	tParams;	//参数数据
	

} MCSUpModulStatusStruct, *PMCSUpModulStatusStruct;

//遥测单元状态数据元素（单元编号：04）
typedef struct MCSTMStatusBlk
{
	UCHAR	ucCodeLock;			//码同步器锁定指示，8位无符号整型数。
								//D7 ~ D0位，00H：失锁，01H：锁定。
	UCHAR	ucFrmLock;			//帧同步器锁定指示，8位无符号整型数。
								//D7 ~ D0位，00H：失锁，01H：锁定。
	UCHAR	ucSubFrmLock;		//副帧同步器锁定指示，8位无符号整型数。
								//D7 ~ D0位，00H：失锁，01H：锁定。
	MCSTMParamStruct	tParams;	//参数数据
	
} MCSTMStatusStruct, *PMCSTMStatusStruct;

//踪接收机单元状态数据元素（单元编号：06）
typedef struct MCSTraceStatusBlk
{
    MCSTraceParamStruct	tParams;	//参数数据
    SHORT sAzimuVolt; // 方位误差电压；仅在正常工作时有效，界面显示状态。
    //量化单位：10/2^15 V。

    SHORT sElevaVolt; // 俯仰误差电压；界面显示状态。
    //仅在正常工作时有效。量化单位：10/2^15 V。

    UCHAR ucOutSel;	// 输出通道指示；界面显示状态。
    // 0x00：左旋；
    // 0x01：右旋；
    // 0x02：合成。

    UCHAR ucStatus;	// 校相状态；仅在自动校相时有效，界面显示状态。
    // 0x 00：无效；
    // 0x 01：正在进行自动校相；
    // 0x 02：自动校相失败；
    // 0x FF：自动校相结束。

    USHORT PhaseAdjust;	// 相位调整量；仅在自动校相时有效，界面显示状态。
    // 量化单位：360°/1024。

    INT nDelayAdjust;		// 基准时延调整量；界面显示状态。
    // 量化单位：2^16/360°。取值范围0~359.99，仅单通道单脉冲、多模馈源使用


    SHORT sAzimuZero;	// 方位零值修正；校相界面显示状态
    // 量化单位：10/2^15 V。

    SHORT sElevaZero;	// 俯仰零值修正；校相界面显示状态
    // 量化单位：10/2^15 V。


    INT nAzimuSlope;	// 方位斜率，仅在方位拉偏自动校相结束时有效。界面显示状态。
    INT nElevaSlope;	// 俯仰斜率，仅在俯仰拉偏自动校准结束时有效。界面显示状态。

    USHORT usErrorCharPhase;//差通道之间相位误差，量化单位：360/1024,取值范围：0~1023，未用。
    UCHAR  Mode;//工作模式，命令界面显示状态。0x00：信号跟踪模式，0x01信号校相模式
    UCHAR  revd1;

    USHORT  AzimuPhase;//方位相位修改，量化单位360/1024，取值范围0~360，仅在方位拉偏自动校相结束时有效。校相界面显示状态。
    USHORT  ElevaPhase;//俯仰相位修改，量化单位360/1024，取值范围0~360，仅在方位拉偏自动校相结束时有效。校相界面显示状态。

    USHORT  AgcVolt_C;//分集合成后的AGC电压，命令界面显示状态。单位mV，范围0~5000，代表电压值为0~5V，对上报值进行处以1000的处理
    //各通道AGC电压为dB线性，为0时代表输入中频信号功率为-60dBm，为5000mV时，代表输入中频信号功率为0dBm
    USHORT  AE_Lock;//角误差锁定指示，命令界面显示状态，1：锁定，0：失锁

    UINT unrsvd[7];

} MCSTraceStatusStruct, *PMCSTraceStatusStruct;

//角误差输出单元状态数据元素（单元编号：07）
typedef struct MCSLeadRevOutSigSta
{
	MCSLeadRevOut  sameStatus;
		
} MCSLeadRevOutSta, *pMCSLeadRevOutSta;

//遥测模拟源单元状态数据元素（单元编号：05）
typedef struct MCSTMSimulStatusBlk
{
	MCSTMSimulParamStruct	tParams;	//参数数据

} MCSTMSimulStatusStruct, *PMCSTMSimulStatusStruct;

//检测输入输出状态数据元素（单元编号：08）

typedef struct SCBBETestOutStatusBlk
{
	SCBBETestOutParamStruct	tParams;	//参数数据

} SCBBETestOutStatusStruct, *PSCBBETestOutStatusStruct;


//变状态检测控制
typedef struct MCSStatusChangeCmdByCWordCheckBlk
{
	UCHAR    usControlCmd;           //控制命令   2表示停止 1表示开始
	UCHAR   usStatusChangeState;		  //变状态模飞使能标致 1：启动变状态模飞，2：不使用变状态模飞	
}MCSStatusChangeCmdByCWordCheckStruct;

//------------------------------------------------
// 命令响应部分
//------------------------------------------------

typedef struct _ERRORTESTREPORT
{
    UCHAR ucTestStatu;//0：无效，1：开始测试，2：正在测试，3：测试完成
    UINT  nTotalBitNum; ////已测总码位数
    UINT  nErrBitNum;   ////误码位数
}MCSERRORTESTREPORT, *PMCSERRORTESTREPORT;

typedef struct _AGCRANGE
{
    UCHAR ucTestStatu;                  //0：无效，1：开始测试，2：测试完成，3：终止完成
    UCHAR ucSignalPolar;                //校相信号旋向 1--左旋 2--右旋
    WORD  wAGCRange;                    //AGC控制范围 单位:1dB
    WORD  wAGCJINGDU;                   //AGC控制精度 单位:0.1dB
}AGCRANGE, *PAGCRANGE;
typedef struct _SSAGCFRMRANGE
{
    UCHAR ucTestStatu;                  //0：无效，1：开始测试，2：测试完成，3：终止完成
    UCHAR ucSignalPolar;                //校相信号旋向 1--左旋 2--右旋
    UCHAR ucFrmRange1;                  //遥测通道测试结果 1：满足 2：不满足
    UCHAR ucFrmRange2;                  //测量通道测试结果 1：满足 2：不满足
}SSAGCFRMRANGE, *PSSAGCFRMRANGE;
typedef struct _SSAGCJD
{
    UCHAR ucTestStatu;                  //0：无效，1：开始测试，2：测试完成，3：终止完成
    UCHAR ucSignalPolar;                //校相信号旋向 1--左旋 2--右旋
    WORD  wAGCJD1;                      //遥测通道AGC控制精度 单位:0.1dB
    WORD  wAGCJD2;                      //测量通道AGC控制精度 单位:0.1dB
}SSAGCJD, *PSSAGCJD;
typedef struct _AGCTIME
{
    UCHAR ucTestStatu;                  //0：无效，1：开始测试，2：测试完成，3：终止完成
    UCHAR ucSignalPolar;                //校相信号旋向 1--左旋 2--右旋
    WORD  wAGCTime1;                    //AGC时间常数0.1ms 单位:0.1ms
    WORD  wAGCTime2;                    //AGC时间常数1ms 单位:0.1ms
    WORD  wAGCTime3;                    //AGC时间常数3ms 单位:0.1ms
    WORD  wAGCTime4;                    //AGC时间常数10ms 单位:0.1ms
    WORD  wAGCTime5;                    //AGC时间常数100ms 单位:0.1ms
    WORD  wAGCTime6;                    //AGC时间常数1000ms 单位:0.1ms
}AGCTIME, *PAGCTIME;
typedef struct _FRERANGE
{
    UCHAR ucTestStatu;                  //0：无效，1：开始测试，2：测试完成，3：终止完成
    UCHAR ucSignalPolar;                //校相信号旋向 1--左旋 2--右旋
    SHORT sFRERange;                    //频率动态范围 单位:dBm
}FRERANGE, *PFRERANGE;
typedef struct _CJWXPARA
{
    UCHAR ucSignalPolar;                //信号旋向 1--左旋 2--右旋
    UINT  nPointCnt;                    //统计点数
    INT   iJZ;                          //均值 单位：0.1m
    UINT  nFC;                          //方差 单位：0.1m
}CJWXPARA, *PCJWXPARA;
typedef struct _CJWX
{
    UCHAR ucTestStatu;                  //0：无效，1：开始测试，2：测试完成，3：终止完成
    CJWXPARA para;
}CJWX, *PCJWX;
typedef struct _SPANRANGE
{
    UCHAR ucTestStatu;                  //0：无效，1：开始测试，2：测试完成，3：终止完成
    UCHAR ucSignalPolar;                //校相信号旋向 1--左旋 2--右旋
    UINT  nSPANRange;                   //幅度动态范围 单位:Hz
}SPANRANGE, *PSPANRANGE;

typedef struct _CAPTURETIME
{
    UCHAR ucTestStatu;                  //0：无效，1：开始测试，2：测试完成，3：终止完成
    UINT  nCaptureTime;                 //捕获时间 单位:ms
}CAPTURETIME, *PCAPTURETIME;
typedef struct _SSAGCTIME
{
    UCHAR ucTestStatu;                  //0：无效，1：开始测试，2：测试完成，3：终止完成
    UCHAR ucSignalPolar;                //校相信号旋向 1--左旋 2--右旋
    WORD  wAGCTime1;                    //遥测通道AGC时间常数1ms 单位:0.1ms
    WORD  wAGCTime2;                    //遥测通道AGC时间常数10ms 单位:0.1ms
    WORD  wAGCTime3;                    //遥测通道AGC时间常数100ms 单位:0.1ms
    WORD  wAGCTime4;                    //遥测通道AGC时间常数1000ms 单位:0.1ms
    WORD  wAGCTime5;                    //测量通道AGC时间常数1ms 单位:0.1ms
    WORD  wAGCTime6;                    //测量通道AGC时间常数10ms 单位:0.1ms
    WORD  wAGCTime7;                    //测量通道AGC时间常数100ms 单位:0.1ms
    WORD  wAGCTime8;                    //测量通道AGC时间常数1000ms 单位:0.1ms
}SSAGCTIME, *PSSAGCTIME;

typedef union _BBETEST_STATUS
{   //0x12B
    AGCRANGE  agcRange;
    SSAGCFRMRANGE SSAgcFrmRange;
    SSAGCJD   SSAgcJD;
    AGCTIME   agcTime;
    FRERANGE  freRange;
    CJWX      CJ;
    CJWX      CS;
    SPANRANGE spanRange;
    CAPTURETIME CaptureTime;
    SSAGCTIME SSAgcTime;
}BBETEST_STATUS, *PBBETEST_STATUS;

#pragma pack()


#endif // _MCSBBE_20090615_STRUCT_H_
