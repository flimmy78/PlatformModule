#ifndef _SMCU_HEAD_FILE_H_
#define _SMCU_HEAD_FILE_H_
#pragma pack(1)

/*
//监控信息帧格式定义表
//标识	类型	字节长度	说明
//地址编码
typedef struct MCSAddressBlk
{
	UCHAR  ucUnit;   //分机，监控分系统00H~0FH，天伺馈分系统10H~1FH，多功能数字基带分系统60H~7FH
	UCHAR  ucDev;   //设备，同一站内的设备顺序排号，1~255，0保留
	UCHAR  ucStation; //站，按基地内各站顺序排号，1~255，0保留
	UCHAR  ucBase;   //基地，按照压缩BCD格式编码
	
} MCSAddressStruct, *PMCSAddressStruct;

//帧头
typedef struct MCSProtocolHeaderBlk
{
	MCSAddressStruct  SID;	//信源标识（SID）
	MCSAddressStruct  DID;	//信宿标识（DID）
	UINT   uiBID;			//信息类别。
							//过程控制命令0000F001H；
							//单元参数设置命令0000F002H；
							//组参数设置命令0000F003H；
	UCHAR  ucReserved[4];   //保留，固定填0
	UINT   uiDate;			//日期
	UINT   uiTime;		    //时间
	UINT   uiLength;		//数据字段(DATA)的字节长度，单位：字节
	
} MCSProtocolHeaderStruct, *PMCSProtocolHeaderStruct;
*/
//
typedef struct MCSProtocolReplyBlk
{
	UINT   uiBID;			//信息类别.填写接收到的命令的信息类别
	UINT   uiFlag;          //标识.过程控制命令：低2字节填写过程控制命令标识，高2字节填0；
							//单元参数设置命令：最低字节填写单元标识，高字节填0；
							//组参数设置命令：填写"全1"
	USHORT   usControlCount;//顺序号.填写被响应命令的顺序号
	UCHAR    ucResult;		//0：正常接收并执行
							//1：分控拒收
							//2：帧格式错误（未知命令）
							//3：被控对象不存在
							//4：参数错误
							//5：条件不具备
							//6：未知原因失败
} MCSProtocolReplyStruct, *PMCSProtocolReplyStruct;

typedef union
{
	struct  
	{
		UINT Day1:4;
		UINT Day10:4;
		
		UINT Month1:4;
		UINT Month10:4;
		
		UINT Year1:4;
		UINT Year10:4;
		
		UINT Year100:4;
		UINT Year1000:4;
	};
	UINT Date;
}MCSBCD_DATE,*PMCSBCD_DATE;
struct TSysCfg
{
	WORD wSID;   //源分系统号
	WORD wDID;   //目的分系统号
};

// 分系统公共单元
typedef struct
{
	UINT   uiM ;		    //任务标识（M字段）
	UCHAR  ucTaskDescribe[6];//任务代号
	DWORDLONG   dwlComboID; //设备组合号
	DWORDLONG   dwlAnswerRevUpFreq;//上行工作频率，范围20250000~21200000，步进1Hz
	DWORDLONG   dwlAnswerRevDownFreq;//下行工作频率，范围22000000~23000000，步进1Hz
	UCHAR  ucCSSendRate;    //测速采样率 0：无效，1：1/3z，2：1Hz，3：10Hz，4：20Hz
	UCHAR  ucAccTime;      //测速积分时间  0：无效，01H-0.04s；02H-0.05s；03H-0.08s；04H-0.25s；05H-0.4s；06H-0.8s;07H-2s;8-3s;9-1s;10-10s.
	UCHAR  ucCJSendRate;    //测距采样率 0：无效，1：1z，2：10Hz，3：20Hz
	UCHAR  ucSendMode;      //送数方式 0：无效，1：常送，2：人工  
	UCHAR  ucSystemOut;     //系统输出  0：无效，1：左旋通道，2：右旋通道，3：择优
    UCHAR  ucSignalOut;     //跟踪信号选择  0：无效，1：TT&C，2：S数传，3：Ka/X数传
	UCHAR  ucDataMode;//数据网数据格式 0：无效，1：新格式，2：老格式 
	UCHAR  ucCjCode;  //测距数据处理标志码
	UCHAR  ucCsCode;  //测速数据处理标志码
	UCHAR  ucTmCode;  //遥测数据处理标志码
	UCHAR  ucDDTCode;  //综合数传数据处理标志码
	UCHAR  ucRTPPT;   //RTP负载类型 0：无效，1：飞船，2：目标飞行器 
	UCHAR  ucDDTOutFormat; //数传输出格式。0：无效，1-921格式，2-综合数传
}MCSSysDataStruct, *PMCSSysDataStruct;
//公共单元数据元素(单元编号: 1)
typedef struct MCSSysDataStatusStructBlk
{
	UCHAR  ucMonitorMode;	//监控方式	0：无效， 1：分控， 2：本控 
	UCHAR  ucBBEWorkStatus;	//基带设备综合状态	0：无效，1：故障，2：正常 
	UCHAR  ucBBEOnline;		//本机在线状态	0：无效，1：在线，2：备份 
	UCHAR  ucTimeCodeStatus;//外时码状态	0：无效，1：故障，2：正常 
	UCHAR  ucPingBiaoStatus;//外频标状态	0：无效，1：故障，2：正常 
	DWORDLONG   ucSystemMode;	//工作模式
	UCHAR  ucSystemWorkMode;//系统工作环路，  
							//1:任务方式  2：联试应答机无线闭环 3：联试应答机有线闭环 
							//4：校零变频器无线闭环 5：校零变频器有线闭环 6：偏馈校零变频器闭环
							//7：射频模拟源有线闭环 8:信标方式 9:中频闭环 10:记录回放 11:其他方式
	UCHAR  ucSystemCaptMode;//系统捕获 方式
	UCHAR  ucDoubleAqu;     //双捕	0：无效， 1：未启动， 2：开始， 3：完成
	UCHAR  ucDistanceAqu;   //距捕	0：无效， 1：未启动， 2：开始， 3：完成
	UCHAR  ucSignalPolar;   //当前跟踪通道	0：无效， 1：左旋， 2：右旋， 3：不跟踪
	UCHAR  ucCSSendSwitch;	//测速送数控制	0：无效，1：送数，2：停送 
	UCHAR  ucCJSendSwitch;	//测距送数控制	0：无效，1：送数，2：停送
	UCHAR  ucTMSendSwitch;	//遥测送数控制	0：无效，1：送数，2：停送
	UCHAR  ucDDTSendSwitchI;//数传I路送数控制	0：无效，1：送数，2：停送
	UCHAR  ucDDTSendSwitchQ;//数传Q路送数控制	0：无效，1：送数，2：停送
	USHORT usTCRevNum;		//接收中心指令包数
	USHORT usTCSendNum;		//遥控发令条数
	USHORT usTCSmallLoopErrNum;		//遥控小环误指令条数
	USHORT usTCSmallLoopReturnNum;	//遥控小环环回中心条数
	UINT uiAudioSendNum;			//话音码元总数
	UINT uiAudioSmallLoopErrNum;	//话音小环误码数
	UINT uiAudioRTPErrNum;			//RTP话音乱序包数
	
    MCSSysDataStruct	tParams;	//参数数据
} MCSSysDataStatusStruct, *PMCSSysDataStatusStruct;

// 数传中频接收机
typedef struct
{
	UCHAR  ucModesel;		//调制体制：0：无效，1：BPSK，2：QPSK，3：UQPSK，4：SQPSK，05：SRCC-OQPSK，06：GMSK
	UCHAR  ucAgcTime;		//AGC时间常数，0：无效，1：1ms；2：10ms；3：100ms；4：1000ms。
	UCHAR  ucPllBw;		    //载波捕获环路带宽，0：无效，1：100Hz；2：300Hz；3：1kHz；4：3kHz；5：保留。
	UCHAR  ucIPowRate;      // I路功率因子 1~10， 0：无效
	UCHAR  ucQPowRate;      // Q路功率因子 1~10， 0：无效
	UCHAR  ucPowerType;     //I/Q功率比设置方式，0：无效，1：分贝方式；2：功率比值方式。
	char   cRatiodB;        //I/Q功率分贝比，量化单位：1 dB，取值范围：-10~10dB(UQPSK有效)
	UCHAR  ucSignalSel;     //数据输出方式，0：无效，1：I/Q合路，2：I/Q独立，3：I路单独
	UCHAR  ucGrayType;		//Gray编码选择：0：无效，1：Gray0，2：Gray1，3：Gray2，4：Gray3，5：Gray4，6：Gray5，7：Gray6，8：Gray7，9：无Gray(QPSK有效)
} MCSDDTRevParamStruct, *PMCSDDTRevParamStruct;
//数传接收机单元状态数据元素（单元编号：15）
typedef struct MCSDDTRevStatusBlk
{
	UCHAR  ucLockStatus;    //载波锁定指示 0：无效，1：锁定，2：失锁。
	USHORT usAgcVolt;		//AGC电压，0~5000，量化单位：0.001 V
	UINT   uiCN0;           //载噪比，单位：dBHz
	UCHAR  ucChnlCirc;		//通道选择：0：无效，1：左旋，2：右旋，3：左右旋同时
	MCSDDTRevParamStruct	tParams;	//参数数据
	
} MCSDDTRevStatusStruct, *PMCSDDTRevStatusStruct;

// 数传解调器单元
typedef struct
{
	UCHAR  ucCodeStyle;	    //码型：0：无效，1：NRZ-L，2：NRZ-M（QPSK无效），3：NRZ-S（QPSK无效）
	UINT usCodeRate;	    //码速率：范围64kb/s～20Mb/s，单位bps，0：无效
	UCHAR  ucBitOutEn;		//是否位流输出，0：无效，1：是，2：否
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
	USHORT usFrameLen;		//帧长：范围4～1024，单位字
	UCHAR  ucFrameSynGLen;	//帧同步码组长度：0：无效，1：16bit，2：24bit，3：32bit
	UINT   uiFrameSynG;	    //帧同步码组：16进制数
	UCHAR  ucFspPlace;		//子帧同步码组位置，0：无效，01表示帧首；02表示帧尾
	UCHAR  ucFrSynSerLiErr; //帧同步容错搜索容错位数：范围0~7，单位bit
	UCHAR  ucFrSynLoLiErr;	//帧同步容错锁定容错位数：范围0~7，单位bit
	UCHAR  ucFrSynVerLiErr; //帧同步容错校核容错位数：范围0~7，单位bit
	UCHAR  ucFrSynLoCheck;	//帧同步容错锁定检测帧数：范围1~8，单位帧
	UCHAR  ucFrSynVerCheck; //帧同步容错校核检测帧数：范围1~8，单位帧
	USHORT  ucSframeLen;	    //格式(副帧)长：范围4～256，单位 帧
	UCHAR  ucSFrameSynMd;	//副帧同步方式：0：无效，1：反码副帧，2：ID副帧，3：循环副帧，4：无副帧 
	USHORT usCycFrmSynCodePos;//循环副帧码组位置,量化单位：1 字，0：无效，取值范围：1 ~ 1024 字
	UCHAR  ucCycFrmSynCodeLen;//循环副帧码组长度  0：无效，1：8bit，2：16bit，3：24bit，4：32bit
	UINT   uiCycFrmSynCode; //循环副帧码组,64位无符号整型数。
	USHORT usIDSubFrmPos;	//ID副帧ID字位置，单位：字；0：无效，取值范围：1 ~ 1024
	UCHAR  ucIDBase;		//ID字基值，0：无效，1：1，2：0
	UCHAR  ucIDDirect;		//ID计数方向，0：无效，1：加计数，2：减计数。			
	UCHAR  ucCFSearchErrBits;//循环副帧搜索容错位数，取值范围：0 ~ 7 位；
	UCHAR  ucCFLockErrBits;	//循环副帧锁定容错位数，取值范围：0 ~ 7 位。
	UCHAR  ucCFVerifyErrBits;//循环副帧校核容错位数，取值范围：0 ~ 7 位；
	UCHAR  ucSubFLockFrame;	//副帧三态逻辑:锁定检测帧数,取值范围：1 ~ 8 帧。
	UCHAR  ucSubFVerifyFrame;//副帧三态逻辑:校核检测帧数,取值范围：1 ~ 8 帧；

} MCSDDTTMIParamStruct, *PMCSDDTTMIParamStruct;

typedef struct MCSDDTTMIStatusBlk
{
	UCHAR  ucCodeLock;		//码同步器锁定指示，0：无效，1：锁定，2：失锁
	UCHAR  ucFrmLock;		//帧同步器锁定指示，0：无效，1：锁定，2：失锁
	UCHAR  ucSubFrmLock;	//副帧同步器锁定指示，0：无效，1：锁定，2：失锁
	UCHAR  ucSendSwitch;    //送数控制	0：无效，1：送数，2：停送
	USHORT usEbN0;          //Eb/N0，单位：0.1dB
	UCHAR  ucVFill;		    //虚拟填充开关，0：无效，1：不填充，2：填充。
	MCSDDTTMIParamStruct	tParams;	//参数数据
	
} MCSDDTTMIStatusStruct, *PMCSDDTTMIStatusStruct;

//数传Q路解调单元参数数据元素（单元编号：17）
typedef struct MCSDDTTMQParamBlk
{
	UCHAR  ucCodeStyle;	    //码型：0：无效，1：NRZ-L，2：NRZ-M（QPSK无效），3：NRZ-S（QPSK无效）
	UINT usCodeRate;	    //码速率：范围64kb/s～20Mb/s，单位bps，0：无效
	UCHAR  ucBitOutEn;		//是否位流输出，0：无效，1：否，2：是
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
	USHORT usFrameLen;		//帧长：范围4～1024，单位字
	UCHAR  ucFrameSynGLen;	//帧同步码组长度：0：无效，1：16bit，2：24bit，3：32bit
	UINT   uiFrameSynG;	    //帧同步码组：16进制数
	UCHAR  ucFspPlace;		//子帧同步码组位置，0：无效，01表示帧首；02表示帧尾
	UCHAR  ucFrSynSerLiErr; //帧同步容错搜索容错位数：范围0~7，单位bit
	UCHAR  ucFrSynLoLiErr;	//帧同步容错锁定容错位数：范围0~7，单位bit
	UCHAR  ucFrSynVerLiErr; //帧同步容错校核容错位数：范围0~7，单位bit
	UCHAR  ucFrSynLoCheck;	//帧同步容错锁定检测帧数：范围1~8，单位帧
	UCHAR  ucFrSynVerCheck; //帧同步容错校核检测帧数：范围1~8，单位帧
	USHORT  ucSframeLen;	    //格式(副帧)长：范围4～256，单位 帧
	UCHAR  ucSFrameSynMd;	//副帧同步方式：0：无效，1：反码副帧，2：ID副帧，3：循环副帧，4：无副帧 
	USHORT usCycFrmSynCodePos;//循环副帧码组位置,量化单位：1 字，0：无效，取值范围：1 ~ 1024 字
	UCHAR  ucCycFrmSynCodeLen;//循环副帧码组长度  0：无效，1：8bit，2：16bit，3：24bit，4：32bit
	UINT   uiCycFrmSynCode; //循环副帧码组,64位无符号整型数。
	USHORT usIDSubFrmPos;	//ID副帧ID字位置，单位：字；0：无效，取值范围：1 ~ 1024
	UCHAR  ucIDBase;		//ID字基值，0：无效，1：1，2：0
	UCHAR  ucIDDirect;		//ID计数方向，0：无效，1：加计数，2：减计数。			
	UCHAR  ucCFSearchErrBits;//循环副帧搜索容错位数，取值范围：0 ~ 7 位；
	UCHAR  ucCFLockErrBits;	//循环副帧锁定容错位数，取值范围：0 ~ 7 位。
	UCHAR  ucCFVerifyErrBits;//循环副帧校核容错位数，取值范围：0 ~ 7 位；
	UCHAR  ucSubFLockFrame;	//副帧三态逻辑:锁定检测帧数,取值范围：1 ~ 8 帧。
	UCHAR  ucSubFVerifyFrame;//副帧三态逻辑:校核检测帧数,取值范围：1 ~ 8 帧；
	
	
} MCSDDTTMQParamStruct, *PMCSDDTTMQParamStruct;

//数传Q路解调单元状态数据元素（单元编号：17）
typedef struct MCSDDTTMQStatusBlk
{
	UCHAR  ucCodeLock;		//码同步器锁定指示，0：无效，1：锁定，2：失锁
	UCHAR  ucFrmLock;		//帧同步器锁定指示，0：无效，1：锁定，2：失锁
	UCHAR  ucSubFrmLock;	//副帧同步器锁定指示，0：无效，1：锁定，2：失锁
	UCHAR  ucSendSwitch;    //送数控制	0：无效，1：送数，2：停送
	USHORT usEbN0;          //Eb/N0，单位：0.1dB
	UCHAR  ucVFill;		    //虚拟填充开关，0：无效，1：不填充，2：填充。
	MCSDDTTMQParamStruct	tParams;	//参数数据
	
} MCSDDTTMQStatusStruct, *PMCSDDTTMQStatusStruct;

typedef struct
{
	UCHAR  ucModesel;		//调制体制：0：无效，1：BPSK，2：QPSK，3：UQPSK，4：SQPSK，05：SRCC-OQPSK，06：GMSK
	UCHAR  ucSimSw;		    //多普勒动态模拟开关，0：无效，1：开，2：关
	USHORT    iSimRange;	//多普勒变化范围，量化单位：1 Hz，取值范围：0 ~ 180 kHz。
	short    iSimRate;		//多普勒变化率，量化单位：1 Hz/s，取值范围：-15 ~ 15 kHz/s。
	UCHAR  ucOnline;		// 加载设置：0：无效，1：加载，2：去载
	UCHAR  ucModu;			// 加调设置：0：无效，1：加调，2：去调
	UCHAR  ucNoiseSw;	    //噪声开关 0：无效，1：开，2：关
	UCHAR  ucCN0;           //载波输出载噪比，范围30～100，单位：dBHz
	UCHAR  ucSignalSel;     //数据输出方式，0：无效，1：I/Q合路，2：I/Q独立，3：I路单独
	UCHAR  ucdataselI;	     //固定码数据与随机数据选择，（0：无效，1：选择固定码数据，2：选择随机数据, 3：顺序数, 4：自定义 ）
	UCHAR   uldataI;		    //固定码数据，32位无符号整型数
	UCHAR  ucdataselQ;	     //固定码数据与随机数据选择，（0：无效，1：选择固定码数据，2：选择随机数据, 3：顺序数, 4：自定义 ）
	UCHAR   uldataQ;		    //固定码数据，32位无符号整型数
	UCHAR  ucIPowRate;      // I路功率因子 1~10， 0：无效
	UCHAR  ucQPowRate;      // Q路功率因子 1~10， 0：无效
	UCHAR  ucPowerType;     //I/Q功率比设置方式，0：无效，1：分贝方式；2：功率比值方式。
	char   cRatiodB;        //I/Q功率分贝比，量化单位：1 dB，取值范围：-10~10dB(UQPSK有效)
	UCHAR  ucGrayType;		//Gray编码选择：0：无效，1：Gray0，2：Gray1，3：Gray2，4：Gray3，5：Gray4，6：Gray5，7：Gray6，8：Gray7，9：无Gray(QPSK有效)
	UCHAR  ucCodeStyleI;	//I路码型：0：无效，1：NRZ-L，2：NRZ-M（QPSK无效），3：NRZ-S（QPSK无效）
	UINT usCodeRateI;	    //I路码速率：范围64kb/s～20Mb/s，单位bps，0：无效
	UCHAR  ucCodingMdI;		//I路译码方式：0：无效，1：关，2：卷积，3：R-S，4：R-S和卷积级联，5：Turbo，6：LDPC
	UCHAR  ucViterbiMdI;		//I路卷积码类型:0：无效，1：(7,1/2)；2：(7,3/4)；3：(7,2/3)；4：(7,5/6)；5：(7,7/8)
	UCHAR  ucG2InvI;		    //I路卷积编码相位:0：无效，1：正相；2：反相。
	UCHAR  ucRSMdI;	    	//I路RS码类型:0：无效，1：(223,255)；2：(239,255)
	UCHAR  ucInterleavI;	    //RS编码交织深度，取值范围：1 ~ 5。
	UCHAR  ucBasiSelI;	    //I路R-S编码双基选择:0：无效，1：对偶基；2：常规基
	UCHAR  ucTurboRatioI;         //I路TURBO码率 01--1/2 02--1/3 03--1/4 04--1/6
	UCHAR  ucLDPCRatioI;          //I路LDPC码率，D7 ~ D0位：01H表示1/2；02H表示2/3；03H表示4/5；04H表示7/8。
	UCHAR  ucturbCtrlI;  	//I路加扰控制：0：无效，1：不加扰，2：自动加扰，3：人工加扰
	UCHAR  ucRandLenI;		//I路加扰多项式长度：0：无效，1：1字节，2：2字节，3：3字节，4：4字节
	UINT   uiRandPolyI;		//I路加扰多项式
	UINT   uiPhaseI;		    //I路加扰初相
	UCHAR  ucCRCI;           //I路传输帧CRC校验：1：开，2：关
	USHORT usFrameLenI;		//I路帧长：范围4～1024，单位字
	UCHAR  ucFrameSynGLenI;	//I路帧同步码组长度：0：无效，1：16bit，2：24bit，3：32bit
	UINT   uiFrameSynGI;	//I路帧同步码组：16进制数
	UCHAR  ucFspPlaceI;		//I路帧同步码组位置，0：无效，01表示帧首；02表示帧尾
	USHORT  ucSframeLenI;	//I路格式(副帧)长：范围4～256，单位 帧
	UCHAR  ucSFrameSynMdI;	//I路副帧同步方式：0：无效，1：反码副帧，2：ID副帧，3：循环副帧，4：无副帧 
	USHORT usCycFrmSynCodePosI;//I路循环副帧码组位置,量化单位：1 字，0：无效，取值范围：1 ~ 1024 字
	UCHAR  ucCycFrmSynCodeLenI;//I路循环副帧码组长度  0：无效，1：8bit，2：16bit，3：24bit，4：32bit
	UINT   uiCycFrmSynCodeI; //I路循环副帧码组,64位无符号整型数。
	USHORT usIDSubFrmPosI;	//I路ID副帧ID字位置，单位：字；0：无效，取值范围：1 ~ 1024
	UCHAR  ucIDBaseI;		//I路ID字基值，0：无效，1：1，2：0
	UCHAR  ucIDDirectI;		//I路ID计数方向，0：无效，1：加计数，2：减计数。
	UCHAR  ucCodeStyleQ;	//Q路码型：0：无效，1：NRZ-L，2：NRZ-M（QPSK无效），3：NRZ-S（QPSK无效）
    UINT usCodeRateQ;	    //Q路码速率：范围64kb/s～20Mb/s，单位bps，0：无效
	UCHAR  ucCodingMdQ;		//Q路译码方式：0：无效，1：关，2：卷积，3：R-S，4：R-S和卷积级联，5：Turbo，6：LDPC
	UCHAR  ucViterbiMdQ;		//Q路卷积码类型:0：无效，1：(7,1/2)；2：(7,3/4)；3：(7,2/3)；4：(7,5/6)；5：(7,7/8)
	UCHAR  ucG2InvQ;		    //Q路卷积编码相位:0：无效，1：正相；2：反相。
	UCHAR  ucRSMdQ;	    	//Q路RS码类型:0：无效，1：(223,255)；2：(239,255)
	UCHAR  ucInterleavQ;	    //Q路RS编码交织深度，取值范围：1 ~ 5。
	UCHAR  ucBasiSelQ;	    //Q路R-S编码双基选择:0：无效，1：对偶基；2：常规基
	UCHAR  ucTurboRatioQ;         //Q路TURBO码率 01--1/2 02--1/3 03--1/4 04--1/6
	UCHAR  ucLDPCRatioQ;          //Q路LDPC码率，D7 ~ D0位：01H表示1/2；02H表示2/3；03H表示4/5；04H表示7/8。
	UCHAR  ucturbCtrlQ;  	//Q路加扰控制：0：无效，1：不加扰，2：自动加扰，3：人工加扰
	UCHAR  ucRandLenQ;		//Q路加扰多项式长度：0：无效，1：1字节，2：2字节，3：3字节，4：4字节
	UINT   uiRandPolyQ;		//Q路加扰多项式
	UINT   uiPhaseQ;		    //Q路加扰初相
	UCHAR  ucCRCQ;           //Q路传输帧CRC校验：1：开，2：关
	USHORT usFrameLenQ;		//Q路帧长：范围4～1024，单位字
	UCHAR  ucFrameSynGLenQ;	//Q路帧同步码组长度：0：无效，1：16bit，2：24bit，3：32bit
	UINT   uiFrameSynGQ;	//Q路帧同步码组：16进制数
	UCHAR  ucFspPlaceQ;		//Q路帧同步码组位置，0：无效，01表示帧首；02表示帧尾
	USHORT  ucSframeLenQ;	//Q路格式(副帧)长：范围4～256，单位 帧
	UCHAR  ucSFrameSynMdQ;	//Q路副帧同步方式：0：无效，1：反码副帧，2：ID副帧，3：循环副帧，4：无副帧 
	USHORT usCycFrmSynCodePosQ;//Q路循环副帧码组位置,量化单位：1 字，0：无效，取值范围：1 ~ 1024 字
	UCHAR  ucCycFrmSynCodeLenQ;//Q路循环副帧码组长度  0：无效，1：8bit，2：16bit，3：24bit，4：32bit
	UINT   uiCycFrmSynCodeQ; //Q路循环副帧码组,64位无符号整型数。
	USHORT usIDSubFrmPosQ;	//Q路ID副帧ID字位置，单位：字；0：无效，取值范围：1 ~ 1024
	UCHAR  ucIDBaseQ;		//Q路ID字基值，0：无效，1：1，2：0
	UCHAR  ucIDDirectQ;		//Q路ID计数方向，0：无效，1：加计数，2：减计数。
//	//////////////////////////////////////////////////////////////////////////
//	UCHAR ucTimeDlySwL;//左旋支路信号延迟开关。1表示开；0表示关（默认）
//	UCHAR ucTimeDlySwR;//右旋支路信号延迟开关。1表示开；0表示关（默认）
//	WORD wTimeDlySizeL;//左旋支路时延大小
//	WORD wTimeDlySizeR;//右旋支路时延大小
} MCSDDTTMSimulParamStruct, *PMCSDDTTMSimulParamStruct;

//数传模拟源单元状态数据元素（单元编号：18）
typedef struct MCSDDTTMSimulStatusBlk
{
	UCHAR   ucInputSource;  // 调制信号来源选择，0：无效，1：内部产生，2：文件(含外部文件)
	MCSDDTTMSimulParamStruct	tParams;	//参数数据
	
} MCSDDTTMSimulStatusStruct, *PMCSDDTTMSimulStatusStruct;

// 数传存储转发
typedef struct
{	
	UINT  uiReplayRate;      //转发速率 kbps
	UCHAR  ucFrameNum;     //传输帧数量
	UCHAR  ucSelFrame;    //是否挑帧 0：无效，1：不挑帧，2：挑帧
	UCHAR ucCsFreq;//对应测速点频 0：无效，1：频点1，2：频点2 
	UCHAR  ucDDTCode;  //综合数传数据处理标志码
} MCSDDTReplayParamStruct, *PMCSDDTReplayParamStruct;

//数传存储转发单元状态数据元素（单元编号：19）
typedef struct MCSDDTReplayStatusStructBlk
{
	UCHAR  ucSaveStatus;    //存储状态 0：无效，1：正在存盘，2：未存盘
	UCHAR   ucSaveRate;      //存储空间 0-100
	UCHAR  ucReplayStatus;      //转发状态 0：无效，1：正在转发，2：未转发  
	MCSDDTReplayParamStruct	tParams;	//参数数据
} MCSDDTReplayStatusStruct, *PMCSDDTReplayStatusStruct;

// 分集合成单元
typedef struct MCSDivComParamStructBlk
{	
	UCHAR     ucCombType;       //合成方式。0：无效,1表示最大合成比，2表示输入左旋，3表示输入右旋。
	USHORT    usDiffPllBW;      //差模环路带宽，16位无符号整型数，量化单位0.1Hz，取值范围0.1-2000Hz，仅在最大比合成时有效
}MCSDivComParamStruct, *PMCSDivComParamStruct;

//分集合成单元状态数据元素（单元编号：20）
typedef struct MCSDivComStatusStructBlk
{
	USHORT     usDiffPllLock;    //差模环锁定指示，16位无符号整型数，0表示失锁，1表示锁定
	USHORT     usDivcombCoefL;   //左旋合成系数，16位无符号整型数，量化单位：0.01，取值范围0.00-1.00
	USHORT     usDivcombCoefR;   //右旋合成系数，16位无符号整型数，量化单位：0.01，取值范围0.00-1.00
	USHORT     usAGCVoltL;       //左旋AGC电压，16位无符号整型数，量化单位：0.001V，取值范围：0-5.000V
	USHORT     usAGCVoltR;       //右旋AGC电压，16位无符号整型数，量化单位：0.001V，取值范围：0-5.000V
	MCSDivComParamStruct tParams;
} MCSDivComStatusStruct, *PMCSDivComStatusStruct;


const WORD gc_wTypeCarrStatus = 0x1339;//载波恢复状态信息元
const WORD gc_wTypeCarrPara = 0x1340;//载波恢复工作参数

const WORD gc_wTypeCodeStatus = 0x1341;//码同步状态信息元
const WORD gc_wTypeCodePara = 0x1342;//码同步工作参数

const WORD gc_wTypeFrmStatus = 0x1343;//帧同步状态
const WORD gc_wTypeFrmPara = 0x1344;//帧同步工作参数

const WORD gc_wTypeModuStatus = 0x1345;//调制器工作状态
const WORD gc_wTypeModuPara = 0x1346;//调制器工作参数

const WORD gc_wTypeTimeStatus = 0xE463;//时码同步状态
const WORD gc_wTypeTimePara = 0xE464;//时码同步工作参数

const WORD gc_wTypeRcvSelfCheck = 0x1375; //系统自检状态（解调）
const WORD gc_wTypeModuSelfCheck = 0x1377;//系统自检状态（调制）

const WORD gc_wTypeGlobalStatus = 0xE467;//X数传基带系统的全局状态
const WORD gc_wTypeSystemPara = 0xE468;//X数传基带系统工作参数

const WORD gc_wCmdCtrlSwitch = 0x436; //控制切换命令
const DWORD gc_dwFifoCount = 4;
#pragma pack()
#endif
