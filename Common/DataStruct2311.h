#ifndef  __DSZ_DATA_STRUCT_H__
#define  __DSZ_DATA_STRUCT_H__
#include "TypeDefine.h"
#include "MCSDataStruct.h"
#include "MCSUSBDataStruct.h"
#include "SMCU_BBE_CDMAStruct.h"
#include "SMCUSSB2_BBE_CDMAStruct.h"

#define MaxDataLen 4096
#define L_BUFFER_LENGTH		4096L

//定义组大目标数
#define MAX_TARGET_COUNT      4

/**********************wl*****************************/
//联试应答机控制相关宏定义
#define RSP_MACRO_SEARCH    ("宏查询")
#define RSP_MACRO_CALL      ("宏调用")
#define RSPDCU_CMD          ("联试应答机控制命令")
/**********************wl*****************************/

//模式1公共名 用户获取结构体中的目标个数
#define COMM_SS1BBE_NAME ("扩频1系统工作参数设置")
#define COMM_SS2BBE_NAME ("扩频2系统工作参数设置")
#define COMM_TCBBE_NAME ("单遥控系统工作参数设置")
#define CMDNAME ("过程控制命令")
#define REMOTESTATUSSEL "Remote状态查询命令"
#define STATUSSEL "状态查询命令"
#define MACROSTATUSSEL "宏查询命令"
#pragma pack (1)

//参数描述信息
struct ParaStruct
{
    char    cParaName[80];
    char    cParaID[80];
    long    lParaNum;        //个数
    char    cParaType[10];   //类型
    double  lfCoefVal;       //系数值
    double  lfprecision;	//精度
    char    cParaUnit[10];   //单位
    //long    lMaxVal;         //最大值
    union
    {
        __int64  imax;
        long   lmax;
        float  fmax;
        double dmax;
    }MaxVal;
    //long    lMinVal;         //最小值
    union
    {
        __int64 imin;
        long   lmin;
        float  fmin;
        double dmin;
    }MinVal;
    //long    lInitVal;        //初值
    union
    {
        __int64 ival;
        long   lval;
        float  fval;
        double dval;
    }InitVal;
    union
    {
        char cValue[20];
    }CVal;  //外部配置字符串使用
} ;

//////////////////////////////////////////////////////////////////////////
//监控服务器主备状态信息元
typedef struct   _XTJK_HOSTBAK_INFO
{   //0x1013
	UCHAR		  ucServSwitchMode;		//监控服务器主备切换方式	  1：手动； 2：自动
	WORD		  wServWorkID;			//监控服务器工作机号  1：一号机；  2：二号机
	WORD		  wServBakID;			//监控服务器备份机号  1：一号机；  2：二号机
}XTJK_HOSTBAK_INFO, *PXTJK_HOSTBAK_INFO;

//系统监控台全局信息元
typedef struct   DSZ_XTJK_GlobalStatusBlk
{   //0x0135
        UCHAR		  ucMonitorMode;		//控制模式	      0: 本控，  1：远控
        WORD		  wGPSBDTimerWorkID;	//GPS/BD授时仪工作机号（分系统号）
        WORD		  wGPSBDTimerBakID;		//GPS/BD授时仪备份机号（分系统号）
        WORD		  wTimeCoderWorkID;		//时码器工作机号（分系统号）
        WORD		  wTimeCoderBakID;		//时码器备份机号（分系统号）
        UCHAR		  ucCurrentOptAnt;		//当前操作天线号
        CHAR		  chServ1WorkStatus;    //服务器1号机是否正常: 0 不确定; >0  正常; <0 异常
        CHAR		  chServ2WorkStatus;    //服务器2号机是否正常: 0 不确定; >0  正常; <0 异常
        CHAR		  chServ1LinkServ2;     //服务器1号机与2号机连通状态: 0 不确定; >0  正常; <0 异常
        union
        {
                DWORD dwDeviceWorkStatus;          //各天线不相关设备工作状态
                struct
                {
                        unsigned GPSBDTimerHostStatus : 2;  //GPS/BD授时仪1号机是否正常: 0 不确定; 1  正常; 2 异常
                        unsigned XDKZZHStatus         : 2;  //信道控制组合是否正常: 0 不确定; 1  正常; 2 异常
                        unsigned TimeCoderHostStatus  : 2;  //时码器1号机是否正常: 0 不确定; 1  正常; 2 异常
                        unsigned KaUCStatus		      : 2;  //Ka上变频器是否正常: 0 不确定; 1  正常; 2 异常
                        unsigned EMUStatus            : 2;  //系统校验设备是否正常: 0 不确定; 1  正常; 2 异常
                        unsigned UCStatus             : 2;  //上变频器是否正常: 0 不确定; 1  正常; 2 异常
                        unsigned SPAStatus            : 2;  //频谱仪是否正常: 0 不确定; 1  正常; 2 异常
                        unsigned IF_MATRIXStatus      : 2;  //中频开关矩阵是否正常: 0 不确定; 1  正常; 2 异常
                        unsigned KaDCStatus			  : 2;  //Ka下变频器是否正常: 0 不确定; 1  正常; 2 异常
                        unsigned NUMEROSCOPEStatus1   : 2;  //数字记录器1号机是否正常: 0 不确定; 1  正常; 2 异常
                        unsigned NUMEROSCOPEStatus2   : 2;  //数字记录器2号机是否正常: 0 不确定; 1  正常; 2 异常
                        unsigned NUMEROSCOPEStatus3   : 2;  //数字记录器3号机是否正常: 0 不确定; 1  正常; 2 异常
                        unsigned NUMEROSCOPEStatus4   : 2;  //数字记录器4号机是否正常: 0 不确定; 1  正常; 2 异常
                        unsigned HSSInitialOK		  : 1;  //软件启动时主备切换过程结束标志。0 不确定; 1 已完成;
                        //tdm10100301
                        unsigned GNSS_Status	      : 2;  //GNSS外测是否正常: 0 不确定; 1  正常; 2 异常
                        unsigned DTE_Status		      : 2;  //DTE是否正常: 0 不确定; 1  正常; 2 异常
                        unsigned reserved			  : 1;  //保留
                };
        }DeviceWorkStatusUnion;
        union
        {
                ULONGLONG ddwDeviceLinkStatus;            //各天线不相关设备与服务器连通状态
                struct
                {
                        unsigned GPSBDTimerHostLinkSrv1 : 2;  //GPS/BD授时仪1号机与服务器1号机连通状态:  0 不确定; 1  正常; 2 异常
                        unsigned GPSBDTimerHostLinkSrv2 : 2;  //GPS/BD授时仪1号机与服务器2号机连通状态:  0 不确定; 1  正常; 2 异常
                        unsigned XDKZZH1LinkSrv1        : 2;  //信道控制组合与服务器1号机连通状态:  0 不确定; 1  正常; 2 异常
                        unsigned XDKZZH1LinkSrv2        : 2;  //信道控制组合与服务器2号机连通状态:  0 不确定; 1  正常; 2 异常
                        unsigned TimeCoderHostLinkSrv1  : 2;  //时码器1号机与服务器1号机连通状态:  0 不确定; 1  正常; 2 异常
                        unsigned TimeCoderHostLinkSrv2  : 2;  //时码器1号机与服务器2号机连通状态:  0 不确定; 1  正常; 2 异常
                        unsigned KaUCLinkSrv1           : 2;  //Ka上变频器与服务器1号机连通状态:  0 不确定; 1  正常; 2 异常
                        unsigned KaUCLinkSrv2           : 2;  //Ka上变频器与服务器2号机连通状态:  0 不确定; 1  正常; 2 异常
                        unsigned DTEHostLinkSrv1        : 2;  //DTE1号机与服务器1号机连通状态:  0 不确定; 1  正常; 2 异常
                        unsigned DTEHostLinkSrv2        : 2;  //DTE1号机与服务器2号机连通状态:  0 不确定; 1  正常; 2 异常
                        unsigned DTEBakLinkSrv1         : 2;  //DTE2号机与服务器1号机连通状态:  0 不确定; 1  正常; 2 异常
                        unsigned DTEBakLinkSrv2         : 2;  //DTE2号机与服务器2号机连通状态:  0 不确定; 1  正常; 2 异常
                        unsigned EMULinkSrv1            : 2;  //系统校验设备与服务器1号机连通状态:  0 不确定; 1  正常; 2 异常
                        unsigned EMULinkSrv2            : 2;  //系统校验设备与服务器2号机连通状态:  0 不确定; 1  正常; 2 异常
                        unsigned UCLinkSrv1             : 2;  //上变频器与服务器1号机连通状态:  0 不确定; 1  正常; 2 异常
                        unsigned UCLinkSrv2             : 2;  //上变频器与服务器2号机连通状态:  0 不确定; 1  正常; 2 异常
                        unsigned SPALinkSrv1            : 2;  //频谱仪与服务器1号机连通状态:  0 不确定; 1  正常; 2 异常
                        unsigned SPALinkSrv2            : 2;  //频谱仪与服务器2号机连通状态:  0 不确定; 1  正常; 2 异常
                        unsigned IF_MATRIXLinkSrv1      : 2;  //中频开关网络与服务器1号机连通状态:  0 不确定; 1  正常; 2 异常
                        unsigned IF_MATRIXLinkSrv2      : 2;  //中频开关网络与服务器2号机连通状态:  0 不确定; 1  正常; 2 异常
                        unsigned NUMEROSCOPE4LinkSrv1   : 2;  //数字记录器4号机与服务器1号机连通状态:  0 不确定; 1  正常; 2 异常
                        unsigned NUMEROSCOPE4LinkSrv2   : 2;  //数字记录器4号机与服务器2号机连通状态:  0 不确定; 1  正常; 2 异常
                        unsigned NUMEROSCOPE1LinkSrv1   : 2;  //数字记录器1与服务器1号机连通状态:  0 不确定; 1  正常; 2 异常
                        unsigned NUMEROSCOPE1LinkSrv2   : 2;  //数字记录器1与服务器2号机连通状态:  0 不确定; 1  正常; 2 异常
                        unsigned NUMEROSCOPE2LinkSrv1   : 2;  //数字记录器2与服务器1号机连通状态:  0 不确定; 1  正常; 2 异常
                        unsigned NUMEROSCOPE2LinkSrv2   : 2;  //数字记录器2与服务器2号机连通状态:  0 不确定; 1  正常; 2 异常
                        unsigned NUMEROSCOPE3LinkSrv1   : 2;  //数字记录器3与服务器1号机连通状态:  0 不确定; 1  正常; 2 异常
                        unsigned NUMEROSCOPE3LinkSrv2   : 2;  //数字记录器3与服务器2号机连通状态:  0 不确定; 1  正常; 2 异常
                        //tdm20100301
                        unsigned GNSS_LinkSrv1	        : 2;  //GNSS外测与服务器1号机连通状态:  0 不确定; 1  正常; 2 异常
                        unsigned GNSS_LinkSrv2	        : 2;  //GNSS外测与服务器2号机连通状态:  0 不确定; 1  正常; 2 异常
                        unsigned DTE_LinkSrv1	        : 2;  //DTE与服务器1号机连通状态:  0 不确定; 1  正常; 2 异常
                        unsigned KaDC_LinkSrv1	        : 2;  //Ka下变频器与服务器1号机连通状态:  0 不确定; 1  正常; 2 异常
                };
        }DeviceLinkStatusUnion;
}DSZ_XTJK_GlobalStatusStruct,*PDSZ_XTJK_GlobalStatusStruct;

//系统监控台天线流信息元
typedef struct   DSZ_XTJK_ANTGlobalStatusBlk
{   //0x1003
	USHORT		  usMID;				   //任务标识MID，即宏号。与任务一一对应
    UCHAR         ucSystemWorkMode;        //系统工作方式 0x01: 任务模式；0x02: 中频闭环自检；0x03: 射频闭环自检；
										   // 2CH－S有线闭环; 2DH－S无线闭环 ; 2EH－Ka无线闭环
    UCHAR         ucSystemSSTTCOrTTC;      //系统工作模式 0x01: 标准TTC模式；0x02: 扩频TTC模式； 0x03: FM模式；0x04: CDMA模式；0x05: 信标模式
	WORD		  wLNAWorkID;			   //LNA工作机号（分系统号）
	WORD		  wLNABakID;			   //LNA备机号（分系统号）
	WORD		  wDCWorkID;			   //下变频器工作机号（分系统号）
	WORD		  wDCBakID;			       //下变频器备机号（分系统号）
	WORD		  wBBEWorkID;			   //基带工作机号（分系统号）
    WORD		  wBBEBakID;			   //基带备机号（分系统号）
	UCHAR         ucDataTransStatus;	   //基带数据发送启停状态	  00H：发送； FFH：不发送
	UCHAR		  ucTaskID;				   //卫星分类码（卫星代号）
	union
	{
		DWORD dwDeviceWorkStatus;          //各天线相关设备工作状态
		struct  
		{
			unsigned LNAHostStatus   : 2;  //LNA工作机是否正常: 0 不确定; 1  正常; 2 异常
			unsigned LNABakStatus    : 2;  //LNA备机是否正常: 0 不确定; 1  正常; 2 异常
			unsigned DCHostStatus    : 2;  //下变频器工作机是否正常: 0 不确定; 1  正常; 2 异常
			unsigned DCBakStatus     : 2;  //下变频器备机是否正常: 0 不确定; 1  正常; 2 异常
			unsigned BBEHostStatus   : 2;  //基带工作机是否正常: 0 不确定; 1  正常; 2 异常
			unsigned BBEBakStatus    : 2;  //基带备机是否正常: 0 不确定; 1  正常; 2 异常
			unsigned AMCUStatus      : 2;  //天控器否正常: 0 不确定; 1  正常; 2 异常
//			unsigned reserved        : 18; //保留
			unsigned SUCHostStatus   : 2;  //S上变频器工作机是否正常: 0 不确定; 1  正常; 2 异常
			unsigned reserved        : 16; //保留
		};
	}DeviceWorkStatusUnion;
	union
	{
		DWORD dwDeviceLinkStatus;          //各天线相关设备与服务器连通状态
		struct  
		{
			unsigned LNAHostLinkSrv1 : 2;  //LNA1号机与服务器1号机连通状态:  0 不确定; 1  正常; 2 异常
			unsigned LNAHostLinkSrv2 : 2;  //LNA1号机与服务器2号机连通状态:  0 不确定; 1  正常; 2 异常
			unsigned LNABakLinkSrv1  : 2;  //LNA2号机与服务器1号机连通状态:  0 不确定; 1  正常; 2 异常
			unsigned LNABakLinkSrv2  : 2;  //LNA2号机与服务器2号机连通状态:  0 不确定; 1  正常; 2 异常
			unsigned DCHostLinkSrv1  : 2;  //下变频器1号机与服务器1号机连通状态:  0 不确定; 1  正常; 2 异常
			unsigned DCHostLinkSrv2  : 2;  //下变频器1号机与服务器2号机连通状态:  0 不确定; 1  正常; 2 异常
			unsigned DCBakLinkSrv1   : 2;  //下变频器2号机与服务器1号机连通状态:  0 不确定; 1  正常; 2 异常
			unsigned DCBakLinkSrv2   : 2;  //下变频器2号机与服务器2号机连通状态:  0 不确定; 1  正常; 2 异常
			unsigned BBEHostLinkSrv1 : 2;  //基带1号机与服务器1号机连通状态:  0 不确定; 1  正常; 2 异常
			unsigned BBEHostLinkSrv2 : 2;  //基带1号机与服务器2号机连通状态:  0 不确定; 1  正常; 2 异常
			unsigned BBEBakLinkSrv1  : 2;  //基带2号机与服务器1号机连通状态:  0 不确定; 1  正常; 2 异常
			unsigned BBEBakLinkSrv2  : 2;  //基带2号机与服务器2号机连通状态:  0 不确定; 1  正常; 2 异常
			unsigned AMCULinkSrv1    : 2;  //天控器与服务器1号机连通状态:  0 不确定; 1  正常; 2 异常
			unsigned AMCULinkSrv2    : 2;  //天控器与服务器2号机连通状态:  0 不确定; 1  正常; 2 异常
//			unsigned reserved        : 4;  //保留
			unsigned SUCHostLinkSrv1 : 2;  //S上变频器1号机与服务器1号机连通状态:  0 不确定; 1  正常; 2 异常
			unsigned                 : 2;  //保留
		};
	}DeviceLinkStatusUnion;
	CHAR	     szTaskName[20];		   //任务名称
}DSZ_XTJK_ANTGlobalStatusStruct, *PDSZ_XTJK_ANTGlobalStatusStruct;

//基带全局信息元
typedef union   DSZ_BBE_GlobalStatusBlk
{   //0x0135
    struct
    {
        BYTE byRsvdAll[2048];
    };

    struct
    {
        UCHAR		  ucMonitorMode;		//控制模式	      0: 分控， 1：本控
        UCHAR		  ucBBEWorkID;			//基带工作机号	  1：A套； 2：B套
        UCHAR		  ucBBEID;				//本机代号		  1：A套； 2：B套
        UCHAR		  ucBBEWorkStatus;		//基带工作状态	  00H：正常；FFH：故障
        UCHAR         ucSystemWorkMode;     //系统工作模式    0x01: 标准TTC模式；0x02: 扩频TTC模式 0x03: FM模式；0x04: CDMA模式 0x05: 信标模式
        UCHAR         ucDataTransStatus;	//基带数据发送启停状态	  00H：不发送； FFH：发送
        USHORT		  usMID;				//不使用 － 任务标识MID，数据传输帧头信息。与任务一一对应
        UINT		  nBID;				    //数据类型标识BID，数据传输帧头信息。与任务一一对应
        UCHAR		  ucSatEncryptionFlag;	//不使用 － 卫星是否加密00H：加密； FFH：未加密
        UCHAR		  ucSystemMode;			//系统工作方式，  01:任务方式   03：联试应答机有线闭环  04：联试应答机无线闭环 06：近场校零变频器无线闭环
        //07：调频射频模拟源有线闭环  09:信标环方式  0A:调频遥测中频闭环
        //0C:记录回放（A机调频，B机扩频）  0D:其他方式 2A:文件模飞 2B：带时码文件模飞 2C：时间符合文件模飞
        UCHAR		  ucDeviceAssembleID;	//2514保留不用。设备组合号0x00：LNA1 A通道工作、LNA2 A通道工作；0x01：LNA1 A通道工作、LNA2 B通道工作；0x02：LNA1 B通道工作、LNA2 A通道工作；0x03：LNA1 B通道工作、LNA2 B通道工作；
        UCHAR		  ucDecrypterOnline;	//不使用 － 解密机在线状态；00H：不在线； FFH：在线
        UCHAR		  ucTaskID;				//宏号
        UINT          nSubCarFreq;			//副载波频率，32位无符号整型数，量化单位：1 Hz，取值范围：5 kHz ~ 512 kHz。副载波频率必须是码速率的整数倍，并满足如下关系：2≤副载波频率/码速率≤512。
        UINT          nCodeRate;			//码速率，32位无符号整型数，量化单位：1 bps，取值范围：32bit/s～64kbit/s。
        UCHAR         ucWordLen;			//字长，8位无符号整型数，量化单位：1 位，取值范围：4 ~ 16 位。
        USHORT        usFrmLen;			    //子帧长，16位无符号整型数，量化单位：1 字，取值范围：4 ~ 2048 字。当字长小于等于8时，帧长最大可以设置为2048；当字长大于8时，帧长最大可以设置为1024。
        DWORDLONG     dwlSynCode;			//子帧同步码组，64位无符号整型数。
        UCHAR         ucFspLen;				//子帧同步码组长，8位无符号整型数，量化单位：1 位，取值范围：8～32位（为字长的整数倍）。
        UCHAR         ucSubFrmType;			//副帧方式，8位无符号整型数。D7 ~ D0位：00H表示无副帧；01H表示ID副帧；02H表示循环副帧；03H表示反码副帧。缺省值为无副帧。
        USHORT        usSubFrmLen;			//副帧长度，16位无符号整型数，单位：帧，取值范围：2 ~ 256 帧。
        UCHAR         ucCodeLock;			//码同步器锁定指示，8位无符号整型数。D7 ~ D0位，00H：失锁，01H：锁定。
        UCHAR         ucFrmLock;			//帧同步器锁定指示，8位无符号整型数。D7 ~ D0位，00H：失锁，01H：锁定。
        UCHAR         ucSubFrmLock;			//副帧同步器锁定指示，8位无符号整型数。D7 ~ D0位，00H：失锁，01H：锁定。
        CHAR	      szTaskName[20];		//任务名称
        BYTE		  byStaID;				//站号
        __int64		  i64DeviceAssembleID;	// 设备组合号
        BYTE		  byRcvType;			// 接收类型；
        // 0x00：单通道单脉冲；
        // 0x01：多模馈源。
        UCHAR ucCombType;					// 合成方式；缺省为0，界面设置
        // 0x00：手动加权；（缺省）
        // 0x01：最大比合成；
        // 0x02：单左旋；
        // 0x03：单右旋。
        // 0x04：手动合成。
        UCHAR   ucRcvBand;					//载波波段。
        //0x00：S频段；
        //0x01：X频段；屏蔽不用；
        //0x02：Ka频段；
        USHORT   usAveBw;					// 平滑带宽。量化单位：0.1Hz，取值范围：0.1Hz～1000Hz。
        USHORT   usAEBw;					// 角误差电压输出带宽。16位无符号整数。量化单位：0.1Hz，取值范围：5.0Hz～100.0Hz。
        USHORT   usErrorChaPhase;           //16位无符号整数，通道相位差，量化单位360/1024，取值范围0~1023
        __int64  i64DownRF;                 //下行载波频率  校相模块作为索引使用，AGC曲线装订作为索引使用

        UINT     uiMID;                     //任务标识，由于系统标准接口是UINT，因此扩充。
        DWORDLONG   dwSystemWorkMode;		//工作模式,01：Ka模式，02：Ka+S模式1（A机Ka，B机S），04：Ka+S模式2（B机Ka，A机S）
        //08：S模式1（1个点频），10：S模式2（2个点频）
        MCSSysStatusStruct DestMCSSysStatus; //

        UCHAR codeSyn;
        UCHAR frmSyn;
        UCHAR subfrmSyn;
        float fSnrC; // 合成信噪比
        UCHAR ucCarrLk; // 共模
        UCHAR ucDivCarrLk; //差模环

        UINT          nSubCarFreqQ;			//副载波频率，32位无符号整型数，量化单位：1 Hz，取值范围：5 kHz ~ 512 kHz。副载波频率必须是码速率的整数倍，并满足如下关系：2≤副载波频率/码速率≤512。
        UINT          nCodeRateQ;			//码速率，32位无符号整型数，量化单位：1 bps，取值范围：32bit/s～64kbit/s。
        UCHAR         ucWordLenQ;			//字长，8位无符号整型数，量化单位：1 位，取值范围：4 ~ 16 位。
        USHORT        usFrmLenQ;			    //子帧长，16位无符号整型数，量化单位：1 字，取值范围：4 ~ 2048 字。当字长小于等于8时，帧长最大可以设置为2048；当字长大于8时，帧长最大可以设置为1024。
        DWORDLONG     dwlSynCodeQ;			//子帧同步码组，64位无符号整型数。
        UCHAR         ucFspLenQ;				//子帧同步码组长，8位无符号整型数，量化单位：1 位，取值范围：8～32位（为字长的整数倍）。
        UCHAR         ucSubFrmTypeQ;			//副帧方式，8位无符号整型数。D7 ~ D0位：00H表示无副帧；01H表示ID副帧；02H表示循环副帧；03H表示反码副帧。缺省值为无副帧。
        USHORT        usSubFrmLenQ;			//副帧长度，16位无符号整型数，单位：帧，取值范围：2 ~ 256 帧。
        UCHAR         ucCodeLockQ;			//码同步器锁定指示，8位无符号整型数。D7 ~ D0位，00H：失锁，01H：锁定。
        UCHAR         ucFrmLockQ;			//帧同步器锁定指示，8位无符号整型数。D7 ~ D0位，00H：失锁，01H：锁定。
        UCHAR         ucSubFrmLockQ;			//副帧同步器锁定指示，8位无符号整型数。D7 ~ D0位，00H：失锁，01H：锁定。
        UCHAR codeSynQ;
        UCHAR frmSynQ;
        UCHAR subfrmSynQ;
        float fSnrCQ; // 合成信噪比

    };
}BBE_GlobalStatusStruct,*PDSZ_BBE_GlobalStatusStruct;

//左右旋状态
typedef struct   DSZ_USBBBE_IndexStatusBlk
{
    UCHAR ucCarrLk; // 载波锁定 00H：失锁，01H：锁定。
    UCHAR ucSweep; //随扫 0：无效，1：未随扫，2：随扫
    UCHAR ucDobbleCap; //双捕 0：无效， 1：未启动， 2：开始， 3：完成

    USHORT usAgcVolt;		//AGC电压，量化单位：0.001 V
    USHORT usSqDir;		    //S/Φ：单位0.1dBHz


    UINT          nSubCarFreq;			//副载波频率，32位无符号整型数，量化单位：1 Hz，取值范围：5 kHz ~ 512 kHz。副载波频率必须是码速率的整数倍，并满足如下关系：2≤副载波频率/码速率≤512。
    UINT          nCodeRate;			//码速率，32位无符号整型数，量化单位：1 bps，取值范围：32bit/s～64kbit/s。
    UCHAR         ucWordLen;			//字长，8位无符号整型数，量化单位：1 位，取值范围：4 ~ 16 位。
    USHORT        usFrmLen;			    //子帧长，16位无符号整型数，量化单位：1 字，取值范围：4 ~ 2048 字。当字长小于等于8时，帧长最大可以设置为2048；当字长大于8时，帧长最大可以设置为1024。
    DWORDLONG     dwlSynCode;			//子帧同步码组，64位无符号整型数。
    UCHAR         ucFspLen;				//子帧同步码组长，8位无符号整型数，量化单位：1 位，取值范围：8～32位（为字长的整数倍）。
    UCHAR         ucSubFrmType;			//副帧方式，8位无符号整型数。D7 ~ D0位：00H表示无副帧；01H表示ID副帧；02H表示循环副帧；03H表示反码副帧。缺省值为无副帧。
    USHORT        usSubFrmLen;			//副帧长度，16位无符号整型数，单位：帧，取值范围：2 ~ 256 帧。
    UCHAR         ucSubCarLock;			//副载波锁定指示，8位无符号整型数。D7 ~ D0位，00H：失锁，01H：锁定。
    UCHAR         ucCodeLock;			//码同步器锁定指示，8位无符号整型数。D7 ~ D0位，00H：失锁，01H：锁定。
    UCHAR         ucFrmLock;			//帧同步器锁定指示，8位无符号整型数。D7 ~ D0位，00H：失锁，01H：锁定。
    UCHAR         ucSubFrmLock;			//副帧同步器锁定指示，8位无符号整型数。D7 ~ D0位，00H：失锁，01H：锁定。

    UCHAR usRangeCap; //距捕 0：无效， 1：未启动， 2：开始， 3：完成
    UCHAR ucMainTone; //主音锁定 00H：失锁，01H：锁定。
    UCHAR ucSideTone; //侧音锁定 00H：失锁，01H：锁定。
    int    iDistance;       //实测距离值，单位：0.1m
    __int64    iDopRate;        //实测多普勒频率，单位：0.01 Hz
    int    iSpeed;          //实测速度值，单位：0.0001 m/s
}USBBBE_IndexStatusStruct,*PDSZ_USBBBE_IndexStatusStruct;

//基带全局信息元(USB)
typedef union   DSZ_USBBBE_GlobalStatusBlk
{   //0x0135
    struct
    {
        BYTE byRsvdAll[2048];
    };

    struct
    {
        UCHAR		  ucMonitorMode;		//控制模式	      0: 分控， 1：本控
        UCHAR		  ucBBEWorkID;			//基带工作机号	  1：A套； 2：B套
        UCHAR		  ucBBEID;				//本机代号		  1：A套； 2：B套
        UCHAR		  ucBBEWorkStatus;		//基带工作状态	  00H：正常；FFH：故障
        UCHAR         ucSystemWorkMode;     //系统工作模式    0x01: 标准TTC模式；
        UCHAR         ucDataTransStatus;	//基带数据发送启停状态	  00H：不发送； FFH：发送
        USHORT		  usMID;				//不使用 － 任务标识MID，数据传输帧头信息。与任务一一对应
        UINT		  nBID;				    //数据类型标识BID，数据传输帧头信息。与任务一一对应
        UCHAR		  ucSatEncryptionFlag;	//不使用 － 卫星是否加密00H：加密； FFH：未加密
        UCHAR		  ucSystemMode;			//系统工作方式，  01:任务方式   03：联试应答机有线闭环  04：联试应答机无线闭环 06：近场校零变频器无线闭环
        //07：调频射频模拟源有线闭环  09:信标环方式  0A:调频遥测中频闭环
        //0C:记录回放（A机调频，B机扩频）  0D:其他方式 2A:文件模飞 2B：带时码文件模飞 2C：时间符合文件模飞
        UCHAR		  ucDeviceAssembleID;	//设备组合号
        UCHAR		  ucDecrypterOnline;	//不使用 － 解密机在线状态；00H：不在线； FFH：在线
        UCHAR		  ucTaskID;				//宏号
        CHAR	      szTaskName[20];		//任务名称
        BYTE		  byStaID;				//站号
        __int64		  i64DeviceAssembleID;	// 设备组合号
        BYTE		  byRcvType;			// 接收类型

        USBBBE_IndexStatusStruct IndexStatus[2];

        UCHAR  ucTimeCodeStatus;//外时码状态	0：无效，1：故障，2：正常
        UCHAR  ucSaveStatus;//存盘状态	0：停止，1：启动
        UCHAR  ucCSSendSwitch;	//测速送数控制	0：无效，1：送数，2：停送
        UCHAR  ucCJSendSwitch;	//测距送数控制	0：无效，1：送数，2：停送
        UCHAR  ucTMSendSwitch;	//遥测送数控制	0：无效，1：送数，2：停送

    };
}USBBBE_GlobalStatusStruct,*PDSZ_USBBBE_GlobalStatusStruct;


//各目标状态
typedef struct   DSZ_SS1BBE_IndexStatusBlk
{
    UCHAR ucCarrLk; // 载波锁定 00H：失锁，01H：锁定。
    UCHAR	ucSPNLock;			//I路伪码锁定指示,0：无效，1：锁定，2：失锁。
    UCHAR   ucQPNLock;          //Q路伪码锁定指示 0：无效，1：锁定，2：失锁。
    UCHAR	ucSatLock;				//卫星锁定指示,0：无效，1：锁定，2：失锁。

    USHORT usEbN0;          //Eb/N0，单位：0.1dB
    UINT          nCodeRate;			//码速率，32位无符号整型数，量化单位：1 bps，取值范围：32bit/s～64kbit/s。
    UCHAR         ucWordLen;			//字长，8位无符号整型数，量化单位：1 位，取值范围：4 ~ 16 位。
    USHORT        usFrmLen;			    //子帧长，16位无符号整型数，量化单位：1 字，取值范围：4 ~ 2048 字。当字长小于等于8时，帧长最大可以设置为2048；当字长大于8时，帧长最大可以设置为1024。
    DWORDLONG     dwlSynCode;			//子帧同步码组，64位无符号整型数。
    UCHAR         ucFspLen;				//子帧同步码组长，8位无符号整型数，量化单位：1 位，取值范围：8～32位（为字长的整数倍）。
    UCHAR         ucSubFrmType;			//副帧方式，8位无符号整型数。D7 ~ D0位：00H表示无副帧；01H表示ID副帧；02H表示循环副帧；03H表示反码副帧。缺省值为无副帧。
    USHORT        usSubFrmLen;			//副帧长度，16位无符号整型数，单位：帧，取值范围：2 ~ 256 帧。
    UCHAR         ucCodeLock;			//码同步器锁定指示，8位无符号整型数。D7 ~ D0位，00H：失锁，01H：锁定。
    UCHAR         ucFrmLock;			//帧同步器锁定指示，8位无符号整型数。D7 ~ D0位，00H：失锁，01H：锁定。
    UCHAR         ucSubFrmLock;			//副帧同步器锁定指示，8位无符号整型数。D7 ~ D0位，00H：失锁，01H：锁定。

    UCHAR    ucDisAble;       //距离有效
    UCHAR    ucSpeedAble;       //速度有效
    int    iDistance;       //实测距离值，单位：0.1m
    __int64    iDopRate;        //实测多普勒频率，单位：0.01 Hz
    int    iSpeed;          //实测速度值，单位：0.0001 m/s
    USHORT usAgcVolt;		//AGC电压，量化单位：0.001 V
    USHORT usSqDir;		    //S/Φ：单位0.1dBHz
    UCHAR  ucPreRangDataMd;	//距离预报方式：0：无效，1：手动，2：引导数据
    UINT  ulPridictR;		//距离预报值，32位无符号整型数，量化单位：1m。
    UINT   uiDeviceDisZero;   //应答机距离零值

    UCHAR  ucTMSaveStatus;//遥测存盘状态	0：停止，1：启动
    UCHAR  ucCJSaveStatus;//测据存盘状态	0：停止，1：启动
    UCHAR  ucCSSaveStatus;//测速存盘状态	0：停止，1：启动

    UCHAR  ucTMSendSwitch;	//遥测送数控制	0：无效，1：送数，2：停送
    UCHAR  ucCJSendSwitch;	//测距送数控制	0：无效，1：送数，2：停送
    UCHAR  ucCSSendSwitch;	//测速送数控制	0：无效，1：送数，2：停送

    UCHAR		  ucTaskID;				//任务代号
    CHAR	      szTaskName[20];		//任务名称
}SS1BBE_IndexStatusStruct,*PDSZ_SS1BBE_IndexStatusStruct;

//基带全局信息元(SS1)
typedef union   DSZ_SS1BBE_GlobalStatusBlk
{   //0x0135
    struct
    {
        BYTE byRsvdAll[2048];
    };

    struct
    {
        UCHAR		  ucMonitorMode;		//控制模式	      0: 分控， 1：本控
        UCHAR		  ucBBEWorkID;			//基带工作机号	  1：A套； 2：B套
        UCHAR		  ucBBEID;				//本机代号		  1：A套； 2：B套
        UCHAR		  ucBBEWorkStatus;		//基带工作状态	  00H：正常；FFH：故障
        UCHAR         ucSystemWorkMode;     //系统工作模式    0x01: 标准TTC模式；
        UCHAR         ucDataTransStatus;	//基带数据发送启停状态	  00H：不发送； FFH：发送
        USHORT		  usMID;				//不使用 － 任务标识MID，数据传输帧头信息。与任务一一对应
        UINT		  nBID;				    //数据类型标识BID，数据传输帧头信息。与任务一一对应
        UCHAR		  ucSatEncryptionFlag;	//不使用 － 卫星是否加密00H：加密； FFH：未加密
        UCHAR		  ucSystemMode;			//系统工作方式，  01:任务方式   03：联试应答机有线闭环  04：联试应答机无线闭环 06：近场校零变频器无线闭环
        //07：调频射频模拟源有线闭环  09:信标环方式  0A:调频遥测中频闭环
        //0C:记录回放（A机调频，B机扩频）  0D:其他方式 2A:文件模飞 2B：带时码文件模飞 2C：时间符合文件模飞
        UCHAR		  ucDeviceAssembleID;	//设备组合号
        UCHAR		  ucDecrypterOnline;	//不使用 － 解密机在线状态；00H：不在线； FFH：在线
        UCHAR		  ucTaskID;				//宏号
        CHAR	      szTaskName[20];		//任务名称
        BYTE		  byStaID;				//站号
        __int64		  i64DeviceAssembleID;	// 设备组合号
        BYTE		  byRcvType;			// 接收类型

        SS1BBE_IndexStatusStruct IndexStatus[4];

        UCHAR  ucTimeCodeStatus;//外时码状态	0：无效，1：故障，2：正常
        UCHAR  ucTrackChnlSel;   //跟踪通道选择    --0:无此参数, 1:左旋通道; 2:右旋通道; 3:择优
        UCHAR  ucTrackDefCH;         //跟踪目标选择   输出选择模式为指定时有效0：无效，1--目标1 2--目标2 3--目标3 4--目标4


    };
}SS1BBE_GlobalStatusStruct,*PDSZ_SS1BBE_GlobalStatusStruct;

//各目标状态
typedef struct   DSZ_SS2BBE_IndexStatusBlk
{
    UCHAR	ucTMCarrLock;			//遥测载波锁定指示,0：无效，1：锁定，2：失锁。
    UCHAR	ucTMSPNLock;			//遥测短码锁定指示,0：无效，1：锁定，2：失锁。
    UCHAR	ucMeasCarrLock;			//测量载波锁定指示,0：无效，1：锁定，2：失锁。
    UCHAR	ucMeasSPNLock;			//测量短码锁定指示,0：无效，1：锁定，2：失锁。
    UCHAR	ucSatLock;				//卫星锁定指示,0：无效，1：锁定，2：失锁。

    __int64    iTMDopRate;        //遥测多普勒频率，单位：0.01 Hz
    USHORT usTMSqDir;		    //遥测S/Φ：单位0.1dBHz
    USHORT usTMAgcVolt;		//遥测AGC电压，量化单位：0.001 V
    USHORT usEbN0;          //Eb/N0，单位：0.1dB
    UINT          nCodeRate;			//码速率，32位无符号整型数，量化单位：1 bps，取值范围：32bit/s～64kbit/s。
    UCHAR         ucWordLen;			//字长，8位无符号整型数，量化单位：1 位，取值范围：4 ~ 16 位。
    USHORT        usFrmLen;			    //子帧长，16位无符号整型数，量化单位：1 字，取值范围：4 ~ 2048 字。当字长小于等于8时，帧长最大可以设置为2048；当字长大于8时，帧长最大可以设置为1024。
    DWORDLONG     dwlSynCode;			//子帧同步码组，64位无符号整型数。
    UCHAR         ucFspLen;				//子帧同步码组长，8位无符号整型数，量化单位：1 位，取值范围：8～32位（为字长的整数倍）。
    UCHAR         ucSubFrmType;			//副帧方式，8位无符号整型数。D7 ~ D0位：00H表示无副帧；01H表示ID副帧；02H表示循环副帧；03H表示反码副帧。缺省值为无副帧。
    USHORT        usSubFrmLen;			//副帧长度，16位无符号整型数，单位：帧，取值范围：2 ~ 256 帧。
    UCHAR         ucTMCodeLock;			//遥测码同步器锁定指示，8位无符号整型数。D7 ~ D0位，00H：失锁，01H：锁定。
    UCHAR         ucTMFrmLock;			//遥测帧同步器锁定指示，8位无符号整型数。D7 ~ D0位，00H：失锁，01H：锁定。
    UCHAR         ucSubFrmLock;			//副帧同步器锁定指示，8位无符号整型数。D7 ~ D0位，00H：失锁，01H：锁定。

    UCHAR         ucMeasCodeLock;			//测量码同步器锁定指示，8位无符号整型数。D7 ~ D0位，00H：失锁，01H：锁定。
    UCHAR         ucMeasFrmLock;			//测量帧同步器锁定指示，8位无符号整型数。D7 ~ D0位，00H：失锁，01H：锁定。
    UCHAR    ucDisAble;       //距离有效
    UCHAR    ucSpeedAble;       //速度有效
    int    iDistance;       //实测距离值，单位：0.1m
    __int64    iMeasDopRate;        //测量多普勒频率，单位：0.01 Hz
    int    iSpeed;          //实测速度值，单位：0.0001 m/s
    USHORT usMeasAgcVolt;		//测量AGC电压，量化单位：0.001 V
    USHORT usMeasSqDir;		    //测量S/Φ：单位0.1dBHz
    UCHAR  ucPreRangDataMd;
    UINT  ulPridictR;		//距离预报值，32位无符号整型数，量化单位：1m。
    UINT   uiDeviceDisZero;   //应答机距离零值

    int    iDeltT;		//星地时差，单位：0.1ns；
    int		iDeltFrq;		//星地频差；4个字节，补码表示的有符号二进制数，单位：0.01Hz

    UCHAR  ucTMSaveStatus;//遥测存盘状态	0：停止，1：启动
    UCHAR  ucCJSaveStatus;//测据存盘状态	0：停止，1：启动
    UCHAR  ucCSSaveStatus;//测速存盘状态	0：停止，1：启动

    UCHAR  ucTMSendSwitch;	//遥测送数控制	0：无效，1：送数，2：停送
    UCHAR  ucCJSendSwitch;	//测距送数控制	0：无效，1：送数，2：停送
    UCHAR  ucCSSendSwitch;	//测速送数控制	0：无效，1：送数，2：停送
    UCHAR		  ucTaskID;				//任务代号
    CHAR	      szTaskName[20];		//任务名称
}SS2BBE_IndexStatusStruct,*PDSZ_SS2BBE_IndexStatusStruct;

//基带全局信息元(SS2)
typedef union   DSZ_SS2BBE_GlobalStatusBlk
{   //0x0135
    struct
    {
        BYTE byRsvdAll[2048];
    };

    struct
    {
        UCHAR		  ucMonitorMode;		//控制模式	      0: 分控， 1：本控
        UCHAR		  ucBBEWorkID;			//基带工作机号	  1：A套； 2：B套
        UCHAR		  ucBBEID;				//本机代号		  1：A套； 2：B套
        UCHAR		  ucBBEWorkStatus;		//基带工作状态	  00H：正常；FFH：故障
        UCHAR         ucSystemWorkMode;     //系统工作模式    0x01: 标准TTC模式；
        UCHAR         ucDataTransStatus;	//基带数据发送启停状态	  00H：不发送； FFH：发送
        USHORT		  usMID;				//不使用 － 任务标识MID，数据传输帧头信息。与任务一一对应
        UINT		  nBID;				    //数据类型标识BID，数据传输帧头信息。与任务一一对应
        UCHAR		  ucSatEncryptionFlag;	//不使用 － 卫星是否加密00H：加密； FFH：未加密
        UCHAR		  ucSystemMode;			//系统工作方式，  01:任务方式   03：联试应答机有线闭环  04：联试应答机无线闭环 06：近场校零变频器无线闭环
        //07：调频射频模拟源有线闭环  09:信标环方式  0A:调频遥测中频闭环
        //0C:记录回放（A机调频，B机扩频）  0D:其他方式 2A:文件模飞 2B：带时码文件模飞 2C：时间符合文件模飞
        UCHAR		  ucDeviceAssembleID;	//设备组合号
        UCHAR		  ucDecrypterOnline;	//不使用 － 解密机在线状态；00H：不在线； FFH：在线
        UCHAR		  ucTaskID;				//宏号
        CHAR	      szTaskName[20];		//任务名称
        BYTE		  byStaID;				//站号
        __int64		  i64DeviceAssembleID;	// 设备组合号
        BYTE		  byRcvType;			// 接收类型

        SS2BBE_IndexStatusStruct IndexStatus[4];

        UCHAR  ucTimeCodeStatus;//外时码状态	0：无效，1：故障，2：正常
        UCHAR  ucTrackChnlSel;   //跟踪通道选择    --0:无此参数, 1:左旋通道; 2:右旋通道; 3:择优
        UCHAR  ucTrackDefCH;         //跟踪目标选择   输出选择模式为指定时有效0：无效，1--目标1 2--目标2 3--目标3 4--目标4


    };
}SS2BBE_GlobalStatusStruct,*PDSZ_SS2BBE_GlobalStatusStruct;


//系统校验设备全局信息元
typedef struct   DSZ_Emu_GlobalStatusBlk 
{   //0x0135
	UCHAR		  ucMonitorMode;        //控制模式	      0: 分控， 1：本控
    UCHAR         ucSystemWorkMode;     //系统工作方式 0x01: 标准TTC任务模式；0x02: 扩频TTC任务模式；0x03 : 标准TTC射频闭环；0x04 : 扩频TTC射频闭环
	UCHAR		  ucSystemMode;         //系统工作模式    0x01: 标准TTC模式；0x02: 扩频TTC模式；0x03: FM模式；0x04: CDMA模式 
	UCHAR		  ucTaskID;             //宏号
	UCHAR         ucSwitch;             //工作开关，8位无符号整型数。D7 ~ D0：00H表示关；01H表示开。
	UINT          nSubCarFreq;          //副载波频率，32位无符号整型数，量化单位：1 Hz，取值范围：5 kHz ~ 512 kHz。副载波频率必须是码速率的整数倍，并满足如下关系：2≤副载波频率/码速率≤512。
	UINT          nCodeRate;            //码速率，32位无符号整型数，量化单位：1 bps，取值范围：32bit/s～64kbit/s。
	UCHAR         ucWordLen;            //字长，8位无符号整型数，量化单位：1 位，取值范围：4 ~ 16 位。
	USHORT        usFrmLen;             //子帧长，16位无符号整型数，量化单位：1 字，取值范围：4 ~ 2048 字。当字长小于等于8时，帧长最大可以设置为2048；当字长大于8时，帧长最大可以设置为1024。
	DWORDLONG     dwlSynCode;           //子帧同步码组，64位无符号整型数。
	UCHAR         ucFspLen;             //子帧同步码组长，8位无符号整型数，量化单位：1 位，取值范围：8～32位（为字长的整数倍）。
	UCHAR         ucSubFrmType;         //副帧方式，8位无符号整型数。D7 ~ D0位：00H表示无副帧；01H表示ID副帧；02H表示循环副帧；03H表示反码副帧。缺省值为无副帧。
	USHORT        usSubFrmLen;			//副帧长度，16位无符号整型数，单位：帧，取值范围：2 ~ 256 帧。
	CHAR	      szTaskName[20];		//任务名称
	UCHAR         ucReserved[2048-113]; //
}DSZ_Emu_GlobalStatusStruct,*PDSZ_Emu_GlobalStatusStruct;

//综合时间信息元  F0H
typedef struct
{
    short       SelfSize;            //本结构的大小--20
    UCHAR       AbsoluteTime[6];    //绝对时间
    long        RelativeTime;       //相对时间
    UCHAR       FlyStartTime[6];     //起飞零时间 
    UCHAR       FlayStartTimeFlag;   //起飞零有效标记  0-无效，1-有效
    UCHAR       TimeCodeSelect;     //时间选择标记  0-时码器，1-时码板，2-外测时间，3-系统时间
}GlobalTimeCodeCellStruct,*PGlobalTimeCodeCell;
//////////////////////////////////////////////////////////////////////////

// 系统公共参数
// 0x101B
union SYS_PARA
{
    // 定义结构体大小
    UINT nRsvAll[128];
    struct
    {
        UINT   uiValidFlag;//参数有效标志  D0～D8有效  1参数有效 0参数无效
        //D0安控发射频率  D1扩频发射频率  D2备份发射频率
        //D3调频接收频率 D4扩频接收频率  D5备份接收(校验)频率
        //D6引导接收主用频率  D7引导接收备用频率  D8应答机零值

        // 任务标识
        UINT unM;

        // 任务代号
        UCHAR ucTaskDescribe[6];      //任务代号
        UCHAR ucRsvd1[2];

        // 设备组合号
        DWORDLONG dwlComboID;         //设备组合号。宏中此参数无效，由监控服务加载时实时修改

        // 调频基带工作模式
        UINT unWorkMode;

        // 调频下行工作频点。范围26000～28350 MHz，量化单位Hz
        DWORDLONG dwlFMDnFreq;

        // 扩频下行工作频点。范围26000～28350 MHz，量化单位Hz
        DWORDLONG dwlSSDnFreq;

        // 备份下行工作频点。范围26000～28350 MHz，量化单位Hz
        DWORDLONG dwlBkDnFreq;

        // 安控上行工作频点。范围30500～31300 MHz，量化单位Hz
        DWORDLONG dwlSCUpFreq;

        // 扩频上行工作频点。范围30500～31300 MHz，量化单位Hz
        DWORDLONG dwlSSUpFreq;

        // 备份上行工作频点。范围30500～31300 MHz，量化单位Hz
        DWORDLONG dwlBkUpFreq;

        // 引导接收主用频率。范围26000～28350 MHz，量化单位Hz
        DWORDLONG dwlLdMnFreq;

        // 引导接收备用频率。范围26000～28350 MHz，量化单位Hz
        DWORDLONG dwlLdBkFreq;

        // 应答机距离零值 量化单位0.001米 范围0～10000米
        UINT unRspRangeZero;

        // 测角送数率
        BYTE ucAngleSendRate;
        BYTE ucRsvd2[3];
    };
};
/*----------------------------------------------------------------------*/
/*-----------------   FM、CDMA和信标体制 公有结构体  -------------------*/
/*----------------------------------------------------------------------*/

//模拟源数据
//PCM模拟源数据中插入的波形参数块结构
//下面定义在DevParaBlk30.h有定义
typedef struct
{
	//插入的波道类型	//波道类型	1字节	字节	0：子帧波道    1：副帧波道
	BYTE	ucTheFrameToInsert;  
	//路号 波道号	2字节	无浮号短整型	1~子帧长度
	USHORT	nRoadNum;  
	//帧号	//帧号	2字节	无浮号短整型	1~副帧长度
	USHORT	nSubFrameNum;          
	//波形类型	1字节	字节	
	//0：正弦波  //1：三角波 //2：锯齿波1 //3：锯齿波2  //4：方波  //5：加计数	//6：减计数
	BYTE	ucWaveType;             //波形类型
	//波形频率  波形频率	4字节	无浮号长整型	单位：Hz
	float	lWaveFreq;
	//波形初始相位  波形初始相位	2字节	无浮号短整型	0~360；单位：度
	USHORT	nWaveInitPhase; 
	
}PCM_EMUDATA_WAVEINSERT_PARA,*pPCM_EMUDATA_WAVEINSERT_PARA;

//该结构用于向框架中保存模拟源数据
typedef struct EmulatorDataToBritGWStruct
{
	//( 分系统号 << 48 ) | ( 0xDFED << 32 ) | ( 流号 | 0x1000 << 16 ) | 0
	BYTE	ucWaveValid;	//插入的波形个数	1字节	字节	0~10;  0表示不插入波形
	PCM_EMUDATA_WAVEINSERT_PARA    WaveInsertData[10];	//波形
	BYTE	ucDataType;		//生成的数据类型	1字节	字节	0：随机码   1：0101码
	USHORT	ucDataSize;		//模拟源数据的长度	2字节	无浮号短整型	单位：字节
	//0：表示生成数据，常数
	//>0：表示使用子帧编辑的数据
	BYTE	EmuData[4096];	//子帧编辑的模拟源数据,字节数组,长度可变。不含时码
	USHORT	usConstVal;		//常数数据
	
} CTMEmulatorDataStruct, *PCTMEmulatorDataStruct;

// 3.5 遥测模拟源数据
typedef struct
{
	UINT nWord[MaxDataLen];     //调制数据字，格式基本同"解调数据字"，但去掉了填"0"字节。
}ctms_data_struct;

//控制动态模飞命令参数块结构  
typedef struct _Dynamic_FlySimu_PARA
{
	//参数块属性标志	2字节	字 	     0112H
	BYTE    ucSimuCommandFlag;//模飞命令标志	1字节	字节	0：停止  1：启动   2：暂停3：恢复
	BYTE    ucSimulationMode;//模飞类型	1字节	字节	0：模拟源动态数据 1：存盘文件模飞   
	char    strSimuFileName[248];//模飞文件名	248字节	字符串	以NULL表示结尾
	
	ULONG	ulCodeRate; //码率	4字节	无浮号长整型	1000bps~5000000 bps 
	DWORDLONG dwlSynCode;//子帧同步码组	8字节	字节数组	低字节再前，高字节在后，总长64位，码组取值向低位靠齐。
	BYTE	ucSynCodeLen;//子帧同步码组长	1字节	字节	8~64比特，长度必为字长的整数倍
	USHORT	nFrameLen;//子帧长度	2字节	无浮号短整型	4~4096   单位：遥测字
	BYTE	ucWordLen;//字长	1字节	字节	4~16比特
	BYTE	ucByframeType;//副帧方式	1字节	字节	0：无副帧   1：ID副帧  2：循环副帧  3：反码副帧
	USHORT	nByframeLen;//副帧长度	2字节	无浮号短整型	2~4096     单位：帧
	USHORT	nByframePos;//ID字或循环副帧码组位置	2字节	无浮号短整型	1~4095   单位：字
	BYTE	ucIdBase;//ID基值	1字节	字节	0或1
	BYTE	ucIdDirect;	//ID计数方向	1字节	字节	0：加计数   1：减计数
	DWORDLONG	dwlByframeSynCode;	//循环副帧同 步码组	8字节	字节 数组	低字节再前，高字节在后，总长64位，码组取值向低位靠齐。
	BYTE	ucLoopCodeLen;  //循环副帧码组长	1字节	字节	8~64比特，长度必为字长的整数倍
	
	BYTE    ucWaveTotal;//插入的波形个数	1字节	字节	0~10;  0表示不插入波形
	PCM_EMUDATA_WAVEINSERT_PARA  InsetWavePara[10];//波形设置参数结构	12字节 * 10	结构数组	数组长度：10
	BYTE    ucDataType;//生成的数据类型	1字节	字节	0：随机码   1：0101码
	
	//模拟源数据的长度	2字节	无浮号短整型	单位：字节
	//0：表示生成数据
	//>0：表示使用子帧编辑的数据
	USHORT    lDataSize;
	UCHAR   DataBuff[L_BUFFER_LENGTH];//子帧编辑的模拟源数据	2048字节	字节数组	最大长度为2048字节，长度可变。不含时码
	
} Dynamic_FlySimu_PARA, *PDynamic_FlySimu_PARA;

//BBE硬件状态结构
//大小：12个32位字
typedef struct HardWareStatusBlk
{
	//0x15DB
	UCHAR	ucVcoLock;		// FPGA工作时钟锁定指示，8位无符号整型数。
	                        // 00H表示失锁；01H表示锁定。
	UCHAR	ucRamTest ;		// 内部RAM自检，8位无符号整型数。
	                        // 00H无效；0EH表示内部RAM读写异常；
	                        // 0FH表示内部RAM读写正常。界面显示状态。
	UCHAR	ucFpgaTest ;	// FPGA读写自检，8位无符号整型数。
	                        // 00H无效；0EH表示FPGA读写异常；0FH表示FPGA读写正常。界面显示状态。
    UCHAR	ucTimeOutNorm;  // 外时码是否正常，0：正常 1：异常
	                        //
	ULONG   ulMSCnt;		// ms计数，32位无符号整型数，
	                        // 用于检测DSP工作时钟是否正常。界面显示状态。

    ULONG   ulCmdErr;		// 命令交互错误计数，32位无符号整型数，
	                        // 用于记录命令交互过程中校验错误的出现次数。界面显示状态。
  
    UCHAR   ucPowerSta;		// 硬件供电状态，；界面显示状态。
                            // 0x 00：异常；
                            // 0x 01：正常。
    UCHAR	ucRsvd2[2];
    UCHAR   ucTemprature;	// 硬件温度；界面显示状态。
                            // 量化：1oC。下限0度，上限80度，超过限度要求报警
        SHORT  Power_L;//左旋输入通道功率，量化单位0.1dBm，显示范围-60.0dBm~0.0dBM。界面显示状态。
        SHORT  Power_R;//右旋输入通道功率，量化单位0.1dBm，显示范围-60.0dBm~0.0dBM。界面显示状态。

	UCHAR	ucPowerNorm;    // 通道功率是否饱和，0：正常 1：异常
	UCHAR	ucBenchNorm;    // 角误差基准信号是否正常，0：正常 1：异常
	UCHAR	ucCarrDoppNorm; // 载波多普勒是否正常，0：正常 1：异常
	UCHAR	ucCodeDoppNorm; // 码多普勒是否正常，0：正常 1：异常

	UINT    unRsvd[6];
	
} HardWareStatusStruct, *PHardWareStatusStruct;

//跟踪接收机参数设置，通用
//     大小：28个32位字
typedef struct BBEMainTrackSetBlk
{
	//0x15D2	
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

} BBEMainTrackSetStruct, *PBBEMainTrackSetStruct;

//跟踪接收机状态
//     大小：28+16个32位字
typedef struct BBEMainTrackStatusBlk
{
	//0x15D1
	BBEMainTrackSetStruct tParams; //跟踪接收机参数
	
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
} BBEMainTrackStatusStruct, *PBBEMainTrackStatusStruct;


//BBE时码单元设置结构
//      大小：12个32位字
typedef struct BBETimeUnitBlk
{
	//0xE464
	UCHAR	ucWorkMode;			//工作模式,8位无符号整型数，D7 ~ D0位：00H表示
	//内时统；01H表示外时统
    UCHAR	ucReserved;	
	UCHAR	ucLeap;				//闰年,00H表示否；01H表示是,仅在"内时统"时有效
	UCHAR	ucReserved1;
	//
	UCHAR	ucSecond;			//秒，取值范围：0 ~ 59
	UCHAR	ucMinute;			//分，取值范围：0 ~ 59
	UCHAR	ucHour;				//时，取值范围：0 ~ 23
    UCHAR	ucReserved2;
	//
	USHORT	usDay;				//天，取值范围：1 ~ 365/366
	UCHAR   ucReserved3[2];
	//
	ULONG	ulReserved[4];		//保留
}BBETimeUnitSetStruct, *PBBETimeUnitSetStruct;

//BBE时码单元状态结构
//      大小：12+16个32位字
typedef struct BBETimeUnitStatusBlk
{	
	//0xE463
	BBETimeUnitSetStruct    tParams;//返回设置参数
	
	UCHAR	ucLock;			//锁定指示,8位无符号整型数。
									//D7 ~ D0位：00H表示失锁；01H表示锁定
    UCHAR   ucReserved[3];
	//
    ULONG   ulB000_L32;   //当前时刻，48位无符号整型数，为0.1ms时码格式。D3 ~ D0：量化单位为0.1ms；
	//D7 ~ D4：量化单位为1ms；D11 ~ D8：量化单位为10ms；D15 ~ D12：量化单位为100ms；
	//D19 ~ D16：量化单位为1s；D23 ~ D20：量化单位为10s；D27 ~ D24：量化单位为1min；
	//D31 ~ D28：量化单位为10min；D35 ~ D32：：量化单位为1hour；
	//
	USHORT  usB000_H16;    //D37 ~ D36：量化单位为10hour；
	//D41 ~ D38：量化单位为1day；D45 ~ D42：量化单位为10day；D47 ~ D46：量化单位为100day。
	USHORT	usT25us;		//25us计数,16位无符号整型数，表示1秒内的时间，
	//量化单位：25 us。
    //
	ULONG	ulReserved;	//保留4个字节
}BBETimeUnitStatusStruct, *PBBETimeUnitStatusStruct;

//信号分析设置结构
//     大小：12个32位字
typedef struct BBESignalAlalyzerSetBlk
{
	//0x15D8
	UCHAR ucAnalyType;	// 此项目不用。非相关信号分析方法；界面设置。
						// 0x00：方法1；
						// 0x01：方法2；
						// 0x02：方法3。

	UCHAR ucRsvd1[3];

	UINT unSampTime;	// 平均时间；界面设置。
						// 量化单位：0.1ms
						// 取值范围：0.1ms~10s

	UINT unAnalyBw;		// 此项目不用。分析带宽；界面设置。
						// 量化单位：0.01Hz；
						// 取值范围：0.01Hz~10000Hz。


	UCHAR ucParaSet;	// 0x00：部分参数更新
						// 0x01；全部参数更新

	UCHAR ucRsvd2[3];

	UINT unRsvd3[8];
} BBESignalAlalyzerSetStruct, *PBBESignalAlalyzerSetStruct;

//信号分析状态结构
//      大小：12+16个32位字
typedef struct BBESignalAlalyzerStatusBlk
{	
	//0x15D7
	BBESignalAlalyzerSetStruct    tParams;//返回设置参数
	
	INT nFreq;	    // 信号频率估计。界面显示状态。
					// 量化单位：0.01Hz

	UINT unPower;	// 信号功率估计。界面显示状态。
					// 量化单位：0.01dBm
	
	UINT unNoise;	// 噪声功率估计，界面显示状态。
					// 量化单位：0.01dBm

	UINT unrsvd[13];
}BBESignalAlalyzerStatusStruct, *PBBESignalAlalyzerStatusStruct;

//频谱监测设置结构
//     大小：8个32位字
typedef struct BBEPowerSpectrumSetBlk
{
	//0x146E
	UCHAR ucSw;
	UINT unCentFreq; 
	UINT unSpan;
	UCHAR ucRbw;
	USHORT usPoints;
} BBEPowerSpectrumSetStruct, *PBBEPowerSpectrumSetStruct;

//频谱监测状态结构
//      大小：8+12个32位字
typedef struct BBEPowerSpectrumStatusBlk
{	
	//0x146D
	BBEPowerSpectrumSetStruct tParams;//返回设置参数
	
	UINT unSendCnt; // 加载计数。32位无符号整型数。取值0~2^32-1

	UINT unRsvd[11];
}BBEPowerSpectrumStatusStruct, *PBBEPowerSpectrumStatusStruct;

// 调制电平纠正设置结构
//     大小：72个32位字
typedef struct  
{   //0x15EE
	unsigned int ValidFlag;//调制器设置有效标志  0 调制器修改设置数据无效  1调制器修改设置数据有效
	short SetValue[142];   //调制器设置修正数据 -70dB ,0.5dB步进
}modu_set_struct;


//调制电平纠正状态结构
//      大小：72+2个32位字
typedef struct  
{   //0x15ED
	modu_set_struct  moduPara;
	unsigned int     Ready;
	unsigned int     rsvd;
}modu_status_struct;


// 接收电平纠正设置结构
//     大小：72个32位字
typedef struct  
{   //0x15F0
	unsigned int ValidFlag;//接收机有效标志  0 采集校准数据条件下，上报软件数据无效  1 采集校准数据条件下，上报软件数据有效
	                       //                2 注入校准数据条件下，软件注入数据无效  3 注入校准数据条件下，软件注入数据有效
	short SetValue[142];//接收机(采集/注入)修正数据  -70dB,0.5dB步进
}recv_set_struct;


//接收电平纠正状态结构
//      大小：72+72+5个32位字
typedef struct  
{   //0x15EF
	recv_set_struct recvPara;
	recv_set_struct recvStatus;
	UINT uCnt;//校准状态更新计数
	UINT :32;
	UINT :32;
	UINT :32;
	UINT :32;
}recv_status_struct;


//三种模式统一的调制设置参数结构
//     大小：28个32位字
typedef struct BBEModuCarrSetBlk
{
	UCHAR	ucMode;				// 界面设置，工作方式：0x00：任务方式，0x01：自检方式
	UCHAR	ucOnline;			// 界面设置，加载/去载：0x00：去载，0x01：加载
	UCHAR	ucModu;				// 界面设置，信标模式固定为0x00，加调/去调：0x00：去调，0x01：加调
	UCHAR	ucOutBand;			// 界面设置，载波频段：0x00：S频段，0x01：X频段（不显示），0x02：Ka频段

	//
	UINT	uiCarrRF;//unRfL;				// 界面设置（仅扩频设置），上行载波频率，量化单位：1Hz，共40位，低32位
	//

	//
	UCHAR	:8;	     			// 界面设置（仅扩频设置），上行载波频率，量化单位：1Hz，共40位，高8位
	UCHAR	ucUpConv;			// 固定设置为0x00，上变频方式：0x00：低差，0x01：高差
	UCHAR	ucSys;				// 固定设置为0x00，扩频模式：0x00：模式1，0x01：模式2
	UCHAR	: 8;        

	//
	UINT	unCarrIf;			// 界面设置，载波中频，量化单位：1Hz，取值范围：68~72MHz，缺省值70MHz

	INT		nCarrOffSet;		// 界面设置，载波多普勒预置，量化单位：1Hz，取值范围：-2MHz~2MHz，缺省值0

	//
	UCHAR	ucLevelAdSw;		// 界面设置，电平修正开关：0x00：关，0x01：开
	UCHAR	: 8;
	SHORT	sLevel;				// 界面设置，输出功率，量化单位0.1dBm，取值范围：-70.0dBm~0.0dBm

	USHORT	usFM_ModuOffSet;	// 界面设置（仅FM设置），调制度。量化单位：0.01，取值范围：0.1~20，缺省值0.7
	UCHAR	ucNoiseSw1;			// 界面设置，输出通道1噪声开关：0x00：关，0x01：开
	UCHAR	ucNoiseSw2;			// 界面设置，输出通道2噪声开关：0x00：关，0x01：开

	USHORT	usC_N01;			// 界面设置，通道1输出信噪比，仅在(NoiseSw1 == 0x01)时有效。量化单位：0.1dBHz，取值范围：0.0~100.0dBHz。
	USHORT	usC_N02;			// 界面设置，通道2输出信噪比，仅在(NoiseSw2 == 0x01)时有效。量化单位：0.1dBHz，取值范围：0.0~100.0dBHz。

	UCHAR	ucSimSw;			// 界面设置，文件模拟总开关：0x00：关，0x01：开
	UCHAR	ucrsvd_d;        
	USHORT	usSimPrd;			// 界面设置，文件模拟更新周期，仅在(SimSw == 0x01)时有效，量化单位：1ms，取值范围：1~1000ms

	UCHAR	ucDoplSimSw;		// 界面设置，载波多普勒文件模拟开关，仅在(SimSw == 0x01)时有效，0x00：关，0x01：开
	UCHAR	ucAmSimSw;			// 界面设置，输出功率文件模拟开关，仅在(SimSw == 0x01)时有效，0x00：关，0x01：开
	UCHAR	ucAESimSw;			// 界面设置，角误差文件模拟开关，仅在(SimSw == 0x01)时有效，0x00：关，0x01：开
	UCHAR	ucAEType;			// 界面设置，仅在(SimSw == 0x01)时有效，角误差模拟源类型：0x00：单通道单脉冲，0x03：多模馈源。
	                            // 以下内容屏蔽（0x04：圆锥扫描，0x01：双通道单脉冲，0x02：三通道单脉冲）

	UCHAR	ucSimDoplSw;		// 界面设置（仅单信标体制设置），单信标多普勒模拟开关：0x00：关，0x01：开（仅在SimSw == 0x00时可设置为开）
	UCHAR	ucAESignalSel;      // AE信号选择 0：方位 1：俯仰 2：方位，俯仰，3 不输出
	USHORT	usSimRnge;			// 界面设置（仅单信标体制设置），仅(SimDoplSw == 0x01)时有效，
	                            // 单信标多普勒模拟范围，量化单位：1kHz，取值范围：0~2400kHz（即±2400 kHz）。

	INT		nSimRate;			// 界面设置（仅单信标体制设置），仅(SimDoplSw == 0x01)时有效，
	                            // 单信标多普勒模拟多谱勒变化率，量化单位：1Hz，取值范围：-200000Hz/s~200000Hz/s（即±2400 kHz）。

	USHORT	usM;				// 界面设置，多模馈源耦合系数，仅(AEType == 0x03)时有效，量化单位：0.001，取值范围：0~1.000
	USHORT	usMiu;				// 界面设置，相对斜率，仅(AEType == 0x03)时有效，量化单位：0.001，取值范围：0~1.000

	UCHAR	ucAIndex;			// 界面设置，和路信号调制度，量化单位：1%，取值范围0~100%
	UCHAR	ucEIndex;			// 界面设置，差路信号调制度，量化单位：1%，取值范围0~100%
	USHORT	usModuRefFrq;		// 界面设置，基准信号频率，量化单位：1Hz，取值范围：1000Hz

	UCHAR	ucParaSet;			// 参数更新标志，0x00：部分参数更新，0x01：全部参数更新
	UCHAR   Interrupter_SW;//闪断开关，0x00:关，0x01:开。
	USHORT  Interrupter_Time;//闪断时间，单位10ns

	UINT    Interrupter_Interval;//闪断间隔，单位10ns。
    /*以下10个参数2311新增加*/
	UCHAR   ucAPhaseSW;          //a支路相位变化开关 0：关（默认） 1：开
	UCHAR   ucASignalSW;         //a支路信号延迟开关 0：关（默认） 1：开
	UCHAR   ucBPhaseSW;          //b支路相位变化开关 0：关（默认） 1：开
	UCHAR   ucBSignalSW;         //b支路信号延迟开关 0：关（默认） 1：开

	WORD    wAInitPhase;         //a支路初始相位 0~359.99度 量化单位0.01度，默认值为0
	WORD    wAChangePhase;       //a支路相位变化率 0~180.0度 量化单位0.1度/秒，默认值为0
	WORD    wBInitPhase;         //b支路初始相位 0~359.99度 量化单位0.01度，默认值为0
	WORD    wBChangePhase;       //b支路相位变化率 0~180.0度 量化单位0.1度/秒，默认值为0
	WORD    wADelay;             //a支路时延大小 10~10000 ns 量化单位5ns，默认值为0
	WORD    wBDelay;             //b支路时延大小 10~10000 ns 量化单位5ns，默认值为0

	//保留
	UINT unRsvd[8];

}BBEModuCarrSetStruct, *PBBEModuCarrSetStruct;

//三种模式统一的调制状态结构
//     大小：28+16个32位字
typedef struct BBEModuCarrStatusBlk
{
	BBEModuCarrSetStruct	tParams;	//参数数据

	INT		nDoplNco1;      // 界面显示多普勒频偏，多普勒频偏 = DoplNco1 * 100e6 / 2^32
	INT		nDoplNco2;      // 界面显示多普勒频偏，多普勒频偏 = DoplNco2 * 100e6 / 2^32

	INT		nDoplRateNco1;  // 界面不显示
	INT		nDoplRateNco2;  // 界面不显示

	SHORT	sAm1;           // 界面显示输出信号功率，量化单位0.1dBm
	SHORT	sAm2;           // 界面显示输出信号功率，量化单位0.1dBm

	SHORT	sDeltA1;        // 方位值
	SHORT	sDeltE1;        // 俯仰值

	SHORT	sDeltA2;        // 方位值
	SHORT	sDeltE2;        // 俯仰值
			 
	UINT	unRsvd[9];
}BBEModuCarrStatusStruct, *PBBEModuCarrStatusStruct;


//////////////////////////////////////////////////////////////////////////

//调频基带分集接收机参数设置
typedef struct FMBBEDiversityRecvSetBlk
{
    //0xE474
    USHORT    usWorkCtrl;       //工作控制,16位无符号整型数,1表示复位,2表示开始工作
    USHORT    usWorkMode;       //工作模式,16位无符号整型数，0表示Fm分集接收;1表示Fm单通道左旋工作;2表示Fm单通道右旋工作

    UINT      uiLCarrFreq;      //Fm接收机左旋中心频率,32位无符号整型数,单位Hz,
    //取值范围69000000 ~ 71000000,缺省为70000000
    UINT      uiRCarrFreq;      //Fm接收机右旋中心频率,32位无符号整型数，单位Hz,
    //取值范围69000000 ~ 71000000,缺省为70000000
    UINT      uiCarrCapRange;   //载波频率捕获范围,32位无符号整型数,单位kHz,取值范围0 ~ 1000,缺省值为1000

    UCHAR	  ucAfcCtrl;        //Afc闭环使能,8位无符号整型数,0表示允许载波闭环，1表示禁止载波闭环,此设置项缺省值为0	
    UCHAR     Reserved1[1];	    //保留1个字节
    USHORT    usAfc;            //Afc时间常数,单位ms,设置值可为3、15*、60和300,缺省值为60,

    UCHAR	  ucAgcMode;        //AGC控制方式，00为手动，01为自动。缺省值为01
    UCHAR     Reserved2[1];	    //保留1个字节
    USHORT    usAgcA;           //左旋（A路）信号手动AGC控制量,设置范围从0xC0～0x600,缺省值为0x120
    USHORT    usAgcB;           //右旋（B路）信号手动AGC控制量,设置范围从0xC0～0x600,缺省值为0x120
    UCHAR	  ucAgcTime;        //时间常数（AgcTime），共2位，单位ms,
    //设置为0、1、2和3、4、5分别代表时间常数为10、50、100和1000、1、3,缺省值为2
    // 2311中使用0、2、3、5，对应10、100、1000、3ms
    UCHAR     Reserved3[1];	    //保留1个字节
    UINT      uiAgcReserved;    //AGC控制,保留置零

    UINT      uiMaxFreqShft;    //最大频偏,32位无符号整型数,单位kHz,范围0~3500kHz 缺省值700kHz
    UINT      uiIfBn;           //中频带宽,32位无符号整型数，单位为Hz,设置值为300000(300kHz）、500000(500k)、750000(750k)、
    //1000000(1000k)、1500000(1.5M)、2400000(2.4M)、3300000(3.3M)、4000000(4M)、6000000(6M)、
    //10000000(10M)、15000000(15M)、20000000(20M)、24000000(24M)，1自适应带宽，不必出现在用户界面

    UCHAR	  ucCHWtMode;       //表示加权模式，0表示自动加权,1表示手动加权,缺省设为0	
    UCHAR	  ucCHWtL;          //左旋手动加权系数,范围是0～1，缺省值为0.5
    UCHAR	  ucCHWtR;          //右旋手动加权系数,范围是0～1，缺省值为0.5
    //左旋右旋加权系数和为1
    UCHAR     Reserved4[1];	    //保留1个字节

    UCHAR	  ucDivCloseLpEn;   //差模环闭环使能,为0表示允许载波闭环，为1表示禁止载波闭环,此设置项缺省值为0
    UCHAR     Reserved5[3];	    //保留3个字节
    USHORT    usDivPll;         //差模环锁定门限，范围0~99，缺省值为10
    USHORT    usDivPllBw;       //差模环带宽，单位Hz，设置范围150至4800,设置值可为150、300*、600、1200*、
    //2400、3600*和4800*，缺省值为150

    UCHAR     ucParaSet;        // 0x00：部分参数更新
    // 0x01；全部参数更新
    UCHAR Track_Sel;//角跟踪类型选择，0：单通道单脉冲，1：多模馈源，2：双通道单脉冲（未用），固定填0，不显示
    USHORT   usAveBw;      // 平滑带宽。量化单位：0.1Hz，取值范围：1Hz，10Hz，20Hz，100Hz，1000Hz，未用。

    //
    USHORT   usAEBw;      // 角误差输出带宽。量化单位：0.1Hz，取值范围：5.0Hz～1000Hz，精度0.1Hz，未用
    USHORT   usFmModuOffset;//调制指数，量化单位：0.01，取值范围：0.1-20，缺省值0.7

    UCHAR	  ucEquSwitch;      //均衡开关，0表示关闭均衡,1表示打开均衡,缺省设为0

    UCHAR     ucReserved7[7];	//保留7个字节

    //注:带*的数不必出现在交付用户的界面中
} FMBBEDiversityRecvSetBlkStruct, *PFMBBEDiversityRecvSetBlkStruct;

//调频基带上行调制参数设置
	//0x15DE
typedef BBEModuCarrSetStruct FMBBEUpModuSetBlk;
typedef BBEModuCarrSetStruct FMBBEUpModuSetStruct;
typedef BBEModuCarrSetStruct *PFMBBEUpModuSetStruct;



//调频基带遥测模拟源参数设置
typedef struct FMBBECTMEmulatorSetBlk
{
	//0x15E0
	UCHAR    ucSwitch;          //工作开关 00--关 01--开
	UCHAR    ucSource;		    //数据来源 00--内调制 01--外调制 06H表示带时码模飞。
	UCHAR	ucSendType;			//发送方式，00H：循环发送，01H：连续发送。
    UCHAR   ucModulMode;        //8位无符号数，00H表示数据模飞方式，01H表示固定码自检、02H表示外调制 
	//
	UINT     uiCodeRate;        //码率 单位0.1bps TT&C遥测范围：1.0bps~2Mbps
	//数传PCM/PM体制 1000.0bps~10Mbps BPSK 体制 1000.0bps~10Mbpa
	//QPSK OQPSK 取值范围：20kbps~20Mbps
	//		     
	UCHAR    ucCodeType;        //码型 00--NRZ-L 01--NRZ-M 02--NRZ-S 03--Biφ-L 04--Biφ-M 05--Biφ-S
	UCHAR    ucSubCarrType;     //副载波类型 TT&C遥测有效 00--方波 01--正弦波
	UCHAR    ucTestSw;          //单音发送开关 00--关 01--开
	UCHAR    ucResv2;
    //
	UINT     uiSubCarFreq;      //副载波频率  单位:0.1Hz 范围：1kHz~4MHz TT&C遥测有效
	//		
	UCHAR    Convol;            //卷积编码 00--否 01--是
	UCHAR    G2Inv;             //卷积编码G2相位 00--正相 01--反相
	USHORT   usResv3;
	//
	UCHAR    ucRandom;           //加扰 00--不加扰 01--加扰
	UCHAR    ucResv4;
	USHORT   usRandPoly;	     //加扰多项式
	//	
	USHORT   usRandPhase;		 //加扰多项式初相
	USHORT   usRandLen;		     //加扰多项式长度 8~16位
	//
	UCHAR    RS;                 //RS编码 00--关 01--开
	UCHAR    VFill;              //RS编码虚拟填充开关 00--关 01--开
	UCHAR    ucDRadix;           //RS译码参考系  00--常规基 01--对偶基
	UCHAR    ucInterleav;	     //RS编码交织深度 范围：1~5
    //
	UCHAR	ucTurboCode;		//Turbo码，D7 ~ D0位：00H表示否；01H表示内编码；02H表示外编码。
	UCHAR    ucTurboLen;         //TURBO码码率 00--1/2 01--1/3 02--1/4 03--1/6
	UCHAR    ucTurboRatio;       //TURBO码信息位长度 00--1784位 01--3568位 02--7136位 03--8920位 04--16384位
	UCHAR    ucResv6;
/*	// 2010年比测状态
	USHORT	Data;				//16位无符号整型数，常数数据，D7 ~ D0表示在选择常数生成帧数据模式下。
    UCHAR   DataSel;            //8位无符号整型数。D7 ~ D0：00H表示选择常数生成帧数据模式；01H表示选择随机码生成帧数据模式。
	UCHAR   ucResv7;
	//
	UINT     szResv8[3];		
	//
    UINT     uiResv9;
*/
	UCHAR	ucLDPCEn;				//
    UCHAR   ucLDPCRatio;            //
	USHORT	:16;
	//
	UINT     szResv8[3];		
	//
	UCHAR	ucDecrypt;				//
    UCHAR   ucKey;					//
	USHORT	:16;
    //
	UCHAR    ucWordLen;           //字长 4~16位
	UCHAR    ucFspLen;            //帧同步码组长 16~192位 
	USHORT   usFrmLen;            //帧长 4~16384字
	//		
	UINT     szFsp[6];            //帧同步码组
	//
	UCHAR    ucFspPlace;          //子帧同步码组位置 00--帧首 01--帧尾
	UCHAR    ucSubFrmType;		  //副帧类型 00--无副帧 01--ID副帧 02--循环副帧 03--反码副帧
	USHORT   usSubFrmLen;	      //副帧长 2~256 帧
	//	
	USHORT   usIDSubFrmPos;	      //ID字位置 单位：字 范围与帧长、同步码组位置、同步码组长有关
	UCHAR    ucIdBase;            //ID基值 0或1
	UCHAR    ucIdDirect;           //ID计数方向 00--加计数 01--减计数
	//	
	UINT     szCycFsp[6];         //循环副帧同步码组
	//
	USHORT   usCycFrmSynCodePos;  //循环副帧同步码组位置 单位：字 范围与帧长、同步码组位置、同步码组长有关
	UCHAR    ucCycFrmSynCodeLen;         //循环副帧同步码组长度 单位：位 范围：16~192
	UCHAR    ucResv10;
	//
	// 2010年比测时保留
	// UINT	 uiResv11;
    UCHAR   DataType;            //
	UCHAR	Data;				//
	USHORT	: 16;
	//
	USHORT	: 16;
	USHORT   usSetCnt;            //设置计数 固定填充1
	//
	UCHAR    ucParaSet;           //参数更新标志 00--只更新修改参数 01--更新所有参数
	UCHAR   : 8;
	USHORT	: 16;
    //
	UINT	uiTpcSw;
	//
	UINT	uiMSDFsp;			//块同步帧同步码组，32位无符号数，默认1ACFFC1D
	//
	UCHAR	ucCarrCtrl; //time sw
	UCHAR	ucRev14[3];

	ULONG	ulTimeCode_L;
	USHORT  usTimeCode_H;
	USHORT	usRev15;

	unsigned int szResv16[10];

} FMBBECTMEmulatorSetStruct, *PFMBBECTMEmulatorSetStruct;


//调频基带遥测参数设置
typedef struct FMBBECTMSetBlk
{
	//0xE478
	UCHAR	ucSwitch;			//工作开关，8位无符号整型数，D7 ~ D0：00H表示关；01H表示开。
	UCHAR	ucInputSel;			//输入选择，D7 ~ D0位：00H表示视频自检；01H表示中频输入;02H表示卫星模拟器输入
	UCHAR	ucCodeType;			//码型，8位无符号整型数。D7 ~ D0位：
								//调频模式定义：00H表示NRZ-L；01H表示NRZ-M；02H表示NRZ-S；03H表示Bi(fi)-L；
								//				04H表示Bi(fi)-M；05H表示Bi(fi)-S；06H表示RNRZ-L。
	UCHAR	Reserved1;			//保留1个字节

    //
	UINT	uiCodeRate;			//码速率，32位无符号整型数，
								//调频模式定义：量化单位：1 bps，取值范围：1000 ~ 10000000，每bps步进。缺省值为2000000。
								//				《任务书》要求单路1kbps ~ 2Mbps，每kbps步进。

	USHORT	usCodeBn;			//码环带宽,16位无符号整型，0表示1%，1表示0.5%，2表示0.1%，其他位保留。
	UCHAR	ucMSD;				//多符号检测，D7 ~ D0位：00H表示否；01H表示是。当选择MSD时，需要以“弹出对话框”的形式作出以下提示“要求调制指数在h=0.7+-0.025范围”
	UCHAR	ucTPCCode;		//TPC码，D7 ~ D0位：00H表示否；01H表示是。
	//UCHAR	ucTurboCode;		//Turbo码，D7 ~ D0位：00H表示否；01H表示是。

    //下面1个变量仅标准TT&C模式有
	UINT	uiSubCarFreq;		//副载波频率，量化单位：1 Hz，取值范围：5 kHz ~ 512 kHz。
								//副载波频率必须是码速率的整数倍，并满足如下关系：2≤副载波频率/码速率≤512。
	//
	UCHAR	ucWordLen;			//字长WordLen,8位无符号整型数,量化单位:1 位,取值范围：4~16 位,缺省值为8位。
	UCHAR	ucFspLen;			//帧同步码组长,8位无符号整型数，量化单位：1 位，取值范围：7 ~ 64 位，缺省值为32位。
	USHORT	usFrmLen;			//帧长,16位无符号整型数，量化单位：1 字，取值范围：4 ~ 1024 字，缺省值为50字。

    //
//	DWORDLONG	dwlFsp;			//帧同步码组,64位无符号整型数，低位有效,不足64位，高位应补零。
    UINT    dwlFspL;
    UINT    dwlFspH;	

    //
	UCHAR	ucSearchErrBits;	//搜索容错位数，取值范围：0 ~ 7 位；
	UCHAR	ucVerifyErrBits;	//校核容错位数，取值范围：0 ~ 7 位；
	UCHAR	ucLockErrBits;		//锁定容错位数，取值范围：0 ~ 7 位。缺省值均为0。
	UCHAR	Reserved3;			//保留1个字节

    //
	UCHAR	ucVerifyCheckFrame;	//三态逻辑：校核检测帧数，取值范围：1 ~ 8 副帧,缺省值为2 帧
	UCHAR	ucLockCheckFrame;	//三态逻辑：锁定检测帧数，取值范围：1 ~ 8 副帧缺省值为3 帧
	UCHAR	Reserved4;			//保留1个字节
	UCHAR	ucSubFrmType;		//副帧方式，8位无符号整型数。
								//00H表示无副帧；01H表示ID副帧；02H表示循环副帧；03H表示反码副帧 缺省值为无副帧

    //
	USHORT	usSubFrmLen;		//副帧长度，16位无符号整型数，单位：帧，取值范围：2 ~ 256 帧，缺省值为256帧。
	USHORT	usIDSubFrmPos;		//ID副帧ID字位置，单位：字；取值范围：1 ~ 1024
	UCHAR	ucIdBase;			//ID字基值，取值范围：0或1；
	UCHAR	ucIdDirect;			//ID计数方向，0：加计数，1：减计数。
	UCHAR	Reserved5[2];		//保留2个字节

    //
//	DWORDLONG dwlCycFrmSynCode;	//循环副帧码组,64位无符号整型数。
    UINT    dwlCycFrmSynCodeL;
    UINT    dwlCycFrmSynCodeH;

    //
	USHORT	usCycFrmSynCodePos;	//循环副帧码组位置,16位无符号整型数，量化单位：1 字，取值范围：1 ~ 1024 字
	UCHAR	Reserved6;			//保留1个字节
	UCHAR	ucCycFrmSynCodeLen;	//循环副帧码组长度,8位无符号整型数，量化单位：1 位，取值范围：8 ~ 64 位

    //
	UCHAR	ucCFSearchErrBits;	//循环副帧搜索容错位数，取值范围：0 ~ 7 位；
	UCHAR	ucCFVerifyErrBits;	//循环副帧校核容错位数，取值范围：0 ~ 7 位；
	UCHAR	ucCFLockErrBits;	//循环副帧锁定容错位数，取值范围：0 ~ 7 位。缺省值均为0
	UCHAR	Reserved7;			//保留1个字节

    //
	UCHAR	ucSubFVerifyFrame;	//副帧三态逻辑：校核检测帧数，取值范围：1 ~ 8 副帧；
	UCHAR	ucSubFLockFrame;	//副帧三态逻辑：锁定检测帧数，取值范围：1 ~ 8 副帧。
	USHORT  usRandEn;			//解扰使能：1表示解扰使能  0表示解扰禁止
	USHORT  usRandPoly;			//解扰多项式：每一位为1表示此阶有效，为0表示此阶无效。
	USHORT	LoadCnt;            //加载计数 
	//
	USHORT  usEmptyFsp;			//Tpc空包标志字节，0x0000为空包；0xffff为有效数据包，默认0x0000
	UCHAR	Reserved8[2];		//保留2个字节
	UINT    uiMSDFsp;			//块同步帧同步码组，默认1ACFFC1D

	UCHAR	ucMSDSearchErrBits;	//MSD块同步搜索容错位数，取值范围：0 ~ 7 位；
	UCHAR	ucMSDVerifyErrBits;	//MSD块同步循环副帧校核容错位数，取值范围：0 ~ 7 位；
	UCHAR	ucMSDLockErrBits;	//MSD块同步循环副帧锁定容错位数，取值范围：0 ~ 7 位。缺省值均为5位
	UCHAR	Reserved9;			//保留1个字节
	
	UCHAR ucParaSet;            // 0x00：部分参数更新
                                // 0x01；全部参数更新
	UCHAR	ucReserved10[3];	//保留3个字节

    UINT	unReserved11[15];	//保留15个32位字

} FMBBECTMSetStruct, *PFMBBECTMSetStruct;

// 时码单元参数设置
//     大小：12个32位字
// 定义参见公共部分BBETimeUnitSetStruct

//信号分析参数设置
//     大小：12个32位字
// 定义参见公共部分BBESignalAlalyzerSetStruct

//检测输入输出参数设置
//     大小：4个32位字
typedef struct FMBBETestOutSetBlk
{
	//0x15E2
	UCHAR ucDigitalCheck; //J3数字检测信号输出选择 0：输出调制/解调PCM数据流 1：输出时码信息/秒脉冲 2：输出修正1kc/2kc基准
	UCHAR ucAnalogCheck;  //低速DA模拟检测信号输出选择
	                      //0：输出角误差包络/修正1kc基准
	                      //1：输出角误差包络/原始1kc基准
	                      //2：输出角误差包络/解调PCM眼图
	                      //3：输出角误差包络/调制PCM信号
	                      //4：输出调制信号/解调PCM眼图
	                      //5：输出原始1kc/2kc基准
	                      //6：输出修正1kc/2kc基准
	                      //7：测试载波捕获时间
	UCHAR ucShiftNumI;    //2311不使用
	UCHAR ucShiftNUmQ;    //2311不使用

	UCHAR ucAOut;         //2311不使用
	UCHAR ucrsvd_b[3];

	UINT unAveBw;         //2311不使用
	
	UINT unrsvd;
} FMBBETestOutSetStruct, *PFMBBETestOutSetStruct;

//***************** 设置结构结束 *************************

//***************** 状态结构 *****************************

//调频基带分集接收机状态
typedef struct FMBBERecvStatusBlk
{
	//0xE473
	FMBBEDiversityRecvSetBlkStruct   tParams;	         //返回设置参数

	INT      iCarrDoppFreq;       //32位有符号整型数,单位0.01Hz
	UCHAR    ucCarrLk;            //为1表示共模环锁定,为0表示共模环失锁
	UCHAR    ucCarrDivLk;         //为1表示差模环锁定,为0表示差模环失锁
	USHORT   usWorkMode;			   //工作模式 0：分集接收 1：单左旋 2：单右旋
    USHORT	 usAgcVoltL;          //左旋通道的AGC电压,单位毫伏(mV),范围从0到5000,代表电压值为0～5V，对上报值进行除以1000的处理
	                              //各通道AGC电压为dB线性，为0时代表输入中频信号功率为-70dBm,
	                              //为5000mV时代表输入中频信号功率为+10dBm
    USHORT	 usAgcVoltR;          //右旋通道的AGC电压,单位毫伏(mV),范围从0到5000,代表电压值为0～5V，对上报值进行除以1000的处理
	                              //各通道AGC电压为dB线性，为0时代表输入中频信号功率为-70dBm,
    	                          //为5000mV时代表输入中频信号功率为+10dBm
    UINT	 uiSnrL;              //左旋信号的信噪比 S/N = 10*lg(uiSnrL)
    UINT	 uiSnrR;              //右旋信号的信噪比S/N = 10*lg(uiSnrR)
    UINT	 uiSnrC;              //合成信噪比S/N = 10*lg(uiSnrC)
    USHORT	 usFactorL;           //左旋信号的加权值 D0~D7表示0~1
    USHORT	 usFactorR;           //右旋信号的加权值 D0~D7表示0~1
    UINT     uiIfBn;              //中频带宽,32位无符号整型数，单位为Hz,设置值为300000(300kHz）、500000(500k)、750000(750k)、
								//1000000(1000k)、1500000(1.5M)、2400000(2.4M)、3300000(3.3M)、4000000(4M)、6000000(6M)、
	                            //10000000(10M)、15000000(15M)、20000000(20M)、24000000(24M)，1自适应带宽，不必出现在用户界面
	UCHAR	 ucA_B_Sel;			  // AB sel 0---A,1---B
	UCHAR    Reserved3[3];	      //保留3个字节

	INT      CarrDPDopp;//单位0.01Hz，当CarrDPDopp等于-10000时，对应载波差模环多普勒频率为0.01*(-10000)Hz,即-100Hz
	UCHAR    Reserved2[8];	      //保留8个字节
	
}FMBBEDiversityRecvStatusStruct, *PFMBBEDiversityRecvStatusStruct;

//调频基带上行调制状态
	//0x15DD
typedef BBEModuCarrStatusStruct FMBBEUpModuStatusBlk;
typedef BBEModuCarrStatusStruct FMBBEUpModuStatusStruct;
typedef BBEModuCarrStatusStruct *PFMBBEUpModuStatusStruct;


//调频遥测模拟源状态
typedef struct FMBBECTMEmulatorStatusBlk
{
	//0x15DF
    FMBBECTMEmulatorSetStruct   tParams;	  //返回设置参数

	UCHAR	B000T[8];			//每帧第1位数据的调制时刻，48位无符号整型数，为0.1ms时码格式。
								//D3 ~ D0：量化单位为0.1ms；D7 ~ D4：量化单位为1ms；
								//D11 ~ D8：量化单位为10ms；D15 ~ D12：量化单位为100ms；
								//D19 ~ D16：量化单位为1s；D23 ~ D20：量化单位为10s；
								//D27 ~ D24：量化单位为1min；D31 ~ D28：量化单位为10min；
								//D35 ~ D32：量化单位为1hour；D37 ~ D36：量化单位为10hour；
								//D41 ~ D38：量化单位为1day；D45 ~ D42：量化单位为10day；
								//D47 ~ D46：量化单位为100day。
	UINT	Reserved[6];		//保留6个32位字
 
} FMBBECTMEmulatorStatusStruct, *PFMBBECTMEmulatorStatusStruct;


//调频基带遥测状态
typedef struct FMBBECTMStatusBlk
{
	//0xE477
    FMBBECTMSetStruct        tParams;	         //返回设置参数

	UCHAR	ucCodeLock;			//码同步器锁定指示，8位无符号整型数。
								//D7 ~ D0位，00H：失锁，01H：锁定。
	UCHAR	ucFrmLock;			//帧同步器锁定指示，8位无符号整型数。
								//D7 ~ D0位，00H：失锁，01H：锁定。
	UCHAR	ucSubFrmLock;		//副帧同步器锁定指示，8位无符号整型数。
								//D7 ~ D0位，00H：失锁，01H：锁定。
	UCHAR	Reserved1;			//保留1个字节

	UCHAR	Reserved2[16];		//保留16个字节


} FMBBECTMStatusStruct, *PFMBBECTMStatusStruct;

// 时码单元状态
//     大小：12+16个32位字
// 定义参见公共部分BBETimeUnitStatusStruct

//信号分析状态
//     大小：12+16个32位字
// 定义参见公共部分BBESignalAlalyzerStatusStruct

//检测输入输出状态
//     大小：4+4个32位字
typedef struct FMBBETestOutStatusBlk
{
	//0x15E2
	UCHAR ucDOut;
	UCHAR ucDIn;
	UCHAR ucShiftNumI;
	UCHAR ucShiftNUmQ;

	UCHAR ucAOut;
	UCHAR ucrsvd_b[3];

	UINT unAveBw;
	
	UINT unrsvd;
} FMBBETestOutStatusStruct, *PFMBBETestOutStatusStruct;

// 频谱监测状态
//     大小：8+12个32位字
// 定义参见公共部分BBEPowerSpectrumStatusStruct


//调制电平纠正状态结构
//      大小：140+12个32位字
// 定义参见公共部分UModuLevelAdStatus

//接收电平纠正状态结构
//      大小：140+12个32位字
// 定义参见公共部分URcvrLevelAdStatus


//***************** 状态结构结束 *************************

////////////////////////////////////////单命令//////////////////////////////////

//
//遥测数据强制读
//大小：7个32位字
typedef struct USBBBETMForceReadBlk
{
	UINT	uiCmdItem;		//单命令项目，0031 0800H表示"遥测数据强制读"。

	//
	USHORT	usTM1;			//是否读取第1路遥测数据，00H表示否；01H表示是。
	UCHAR	Reserved1[6];			
	
} BBETMForceReadStruct, *PUSBBBETMForceReadStruct;

//时间符合调制输出
//大小：7个32位字
typedef struct BBETimeFitSCmdBlk
{
	UINT	uiCmdItem;		//单命令项目，0031 0B00H表示"时间符合调制输出"。

    //
	UCHAR	ucSw;		    //开关，8位无符号整型数，00H表示否；01H表示是。
	UCHAR	Reserved1;		
	UCHAR	Reserved2;		
	UCHAR	Reserved3;	
    
	UCHAR	B000T25us[6];		//时码，48位无符号整型数，为25us时码格式。
								//D15 ~ D0位：16位无符号整型数，表示1秒内的时间，量化单位：25 us。
								//D19 ~ D16：量化单位为1s；D23 ~ D20：量化单位为10s；
								//D27 ~ D24：量化单位为1min；D31 ~ D28：量化单位为10min；
								//D35 ~ D32：量化单位为1hour；D37 ~ D36：量化单位为10hour；
								//D41 ~ D38：量化单位为1day；D45 ~ D42：量化单位为10day；
								//D47 ~ D46：量化单位为100day。

	UCHAR	ucRsvd[2];
	
    //
	UINT	Reserved4[4];
	
} BBETimeFitSCmdStruct, *PBBEpTimeFitSCmdStruct;

//加载/去载、加调/去调
//大小：7个32位字
typedef struct BBESwsSCmdBlk
{
	UINT	uiCmdItem;		//单命令项目，0031 0B00H表示"加载/去载、加调/去调"。

    //
	UCHAR	ucSw1;		    //加载/去载开关，8位无符号整型数，00H表示否；01H表示是。
	UCHAR	ucSw2;		    //加调/去调开关，8位无符号整型数，00H表示否；01H表示是。
	UCHAR	ucRsvd1[2];		
	
    //
	UINT	unRsvd2[5];
	
} BBESwsSCmdStruct, *PBBESwsSCmdStruct;

//接收电平纠正
//大小：7个32位字
typedef struct BBEAdLevelSCmdBlk
{
	UINT	uiCmdItem;		//单命令项目，0031 0B00H表示"接收电平纠正"。

    //
	UCHAR	ucSw;		    //开关，8位无符号整型数，00H表示否；01H表示是。
	UCHAR	ucRsvd1[3];		

	UCHAR ucStartLevelL;	//8位无符号整型数，左旋起始电平，量化单位：1dBm，取值范围0~-70dBm
	UCHAR ucEndLevelL;		//8位无符号整型数，左旋结束电平，量化单位：1dBm，取值范围0~-70dBm，小于左旋起始电平
	UCHAR ucStartLevelR;	//8位无符号整型数，右旋起始电平，量化单位：1dBm，取值范围0~-70dBm
	UCHAR ucEndLevelR;		//8位无符号整型数，右旋结束电平，量化单位：1dBm，取值范围0~-70dBm，小于右旋起始电平

    //
	UINT	unRsvd2[4];
	
} BBEAdLevelSCmdStruct, *PBBEAdLevelSCmdStruct;

//模拟源曲线模拟
//大小：7个32位字
typedef struct BBESigSimSCmdBlk
{
	UINT	uiCmdItem;		//单命令项目，0031 0400H表示"模拟源曲线模拟"。

    //
	USHORT	usFileSimPrd;	//文件模拟更新周期，界面设置，在SimModeSw开情况下有效。
							//量化单位：1ms;(缺省)
							//取值范围：1~1000ms。
	UCHAR	ucRsvd1[2];		

    //
	UINT	unRsvd2[5];
	
} BBESigSimSCmdStruct, *PBBESigSimSCmdStruct;

//单命令
//大小：7个32位字
typedef union FMBBESingleCmdBlk
{
	//0xE668
	BBETMForceReadStruct		TMForceRead;	//遥测数据强制读
	BBETimeFitSCmdStruct		TimeFitModu;	//时间符合调制输出
	BBESwsSCmdStruct			Sws;			//加载/去载、加调/去调
	BBEAdLevelSCmdStruct		AdLevel;		//接收电平纠正
	BBESigSimSCmdStruct			SigSim;			//模拟源曲线模拟 
	
}FMBBESingleCmdStruct, *PFMBBESingleCmdStruct;

////////////////////////////////////////单命令结束//////////////////////////////////


//////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------*/
/*---------------------CDMA体制 结构体  ----------------------------------*/
/*----------------------------------------------------------------------*/
//***************** 设置结构 *************************

/***************** 命令结构定义*****************************/

// 1.1 跟踪接收机参数设置
//     大小：11个32位字
typedef struct BBEGuidTrackSetBlk
{
	//0xE554
	UCHAR	ucSwitch;			//工作开关，8位无符号整型数。D7 ~ D0位：00H表示关；01H`表示开
	UCHAR	ucWordMode;			//工作模式,8位无符号整型数。D7 ~ D0位：00H表示正常工作；01H表示自动校相
	USHORT	usSensitivity;		//检测灵敏度，16位无符号整型数，量化单位：1 mV/mil，缺省值为80 mV/mil。
	//
	SHORT	siAzimuPoint;		//天线方位指向,16位有符号整型数，量化单位：0.01 mil。
	SHORT	siElevaPoint;		//天线俯仰指向,16位有符号整型数，量化单位：0.01 mil。
	//
	SHORT	siAzimuZeroAmend;	//方位零值修正,16位有符号整型数，量化单位：10/2^15 V。
	SHORT	siElevaZeroAmend;	//俯仰零值修正,16位有符号整型数，量化单位：10/2^15 V。
	//
	int		iAzimuSlope;		//方位斜率,32位有符号整型数
	//
	int		iElevaSlope;		//俯仰斜率,32位有符号整型数
	//
	USHORT	usMeasNum;			//测量点数,16位无符号整型数，表示校相时每个相位的方位、俯仰电压测量2^MeasNum次。
								//上报的测量数据是2^MeasNum个测量数据的平均值。
	//
	USHORT  rsReserved;         //保留两个字节
	//
	USHORT  rsRefFreq;          //基准信号频率，16位无符号整型数，量化单位：1 Hz，缺省值为25Hz。
	SHORT   iRefDelay;			//时延修正，16位有符号整型数，表示输入基准与本地基准的时延修正量，量化单位：1/工作时钟。
	//
	UINT	Reserved3[4];		//保留8个字节，共44字节

} BBEGuidTrackSetStruct, *PBBEGuidTrackSetStruct;

//输出选择[yangf角误差输出]
//     大小：8个32位字
typedef struct SSBBEOutputSelSetBlk
{
	//0x1108
	UCHAR	ucOutType;			//输出方式，8位无符号整型数。D7 ~ D0位：
								//00H表示指定输出；01H表示择优输出,缺省为00H。
	UCHAR	ucObjOut;			//指定输出，8位无符号整型数。D7 ~ D0位：00H表示指定左旋输出；01H表示指定右旋输出。缺省为0。
	UCHAR	ucChanOut;			//指定通道输出，8位无符号整型数， D7 ~ D0位：00H表示指定通道1角误差输出；
								//01H表示指定通道2角误差输出；02H表示指定通道3角误差输出；03H表示指定通道4角误差输出；
								//04H表示指定通道5角误差输出。缺省为0。
	UCHAR	Reserved1;			//
	//
	UCHAR	ucTrkHoldTh;		//跟踪保持门限，8位无符号整型数，量化单位：1dBHz。取值范围：30 ~ 100dBHz。
								//仅在输出方式为择优输出时有效,缺省为50。
	UCHAR	ucTrkHoldTime;		//跟踪判决时间，8位无符号整型数，量化单位：s。取值范围：1 ~ 100s。
								//仅在输出方式为择优输出时有效,缺省为5。
	USHORT	usTrkOutTh;			//择优输出门限，16位无符号整型数，量化单位：1dB。取值范围：3 ~ 30dB。
								//设置到硬件时进行如下转换：[10×10n/10]，n表示界面设置值，[ ]表示取整。
								//仅在输出方式为择优输出时有效,缺省为6dB。
	//
	UINT	Reserved2[6];		//保留6*4=24个字节

} SSBBEOutputSelSetStruct, *PSSBBEOutputSelSetStruct;

//扩频接收载波参数设置
//     大小：11个32位字
typedef struct SSBBERecvCarrSetBlk
{
	//0xE560
	UCHAR	ucAgcMode;			//AGC手动/自动,8位无符号整型数，表示AGC控制方式。缺省为自动
								//00H表示手动，手动时设手动衰减量；01H表示自动，自动时设时间常数

	UCHAR	ucAgcTime;			//AGC时间常数，8位无符号整型数。D7 ~ D0位：00H表示3ms；01H表示10ms；02H表示100ms；03H表示1000ms。

	USHORT	usAgcManAtten;		//AGC手动衰减量,16位无符号整型数，取值范围：0 ~ 4095，缺省值为4095.
	//
	UCHAR	ucSys;				//扩频模式，8位无符号整型数，D7 ~ D0位：00H表示扩频模式1；01H表示扩频模式2。

	UCHAR	ucDownConvType;		//下变频方式,8位无符号整型数，00H表示低差；01H表示高差

	UCHAR	ucMode;				//工作方式，8位无符号整型数，D7 ~ D0位：00H表示左右旋同时；01H表示左右旋合成。缺省值0。
	                            
	UCHAR	ucDownAtten;		//下行信道衰减量，8位无符号整型数，量化单位：1dB。
	//
	UINT	uiCarrRF;			//下行点频,32位无符号整型数，量化单位：1 Hz，取值范围为2 200 M ~ 2 300 MHz
	//
	UINT	uiCarrIF;			//输入中频,32位无符号整型数，量化单位：1 Hz，取值范围为68 M ~ 72 MHz，缺省为70 MHz
	//
	USHORT	usMaxDopp1st;		//最大多卜勒变化率，16位无符号整型数，量化单位：1 Hz/s，
								//取值范围：0 ~ 1.8 kHz/s，缺省值为1.8kHz/s（即±1.8 kHz/s）.

	USHORT	usAcquRange;		//载波频率捕获范围，16位无符号整型数，仅在无多卜勒预报时有效，量化单位：1kHz，
								//取值范围：0 ~ 250 kHz，缺省为250kHz（即±250kHz）。
	//
	UCHAR	ucPIIBw;			//载波环路带宽，8位无符号整型数。D7 ~ D0位：00H表示100Hz；01H表示200Hz；02H表示500Hz；03H表示1kHz。
								
	UCHAR	ucPredictSw;		//多卜勒预报开关，00H表示无预报；01H表示有预报。
	UCHAR	ucDoppErr;			//多卜勒预报精度,8位无符号整型数，量化单位：1 kHz，
								//取值范围：0 ~ ±10 kHz，缺省值为5 kHz（即±5 kHz）
	UCHAR	Reserved1;			//保留
	//
	//I/Q功率比,16位无符号整型数，仅在扩频模式1时有效。I/Q功率比的缺省值为4/1,
	//注：（I路功率因子/Q路功率因子）为最简分数的形式
	USHORT	ucIPowFactor;		//表示I路功率因子,取值范围：1 ~ 100,缺省值为4,仅在扩频模式1下I/Q功率比设置方式为功率比值时有效。
	USHORT	ucQPowFactor;		//表示Q路功率因子,取值范围：1 ~ 100,缺省值为1,仅在扩频模式1下I/Q功率比设置方式为功率比值时有效。
	//
	UCHAR   ucPowerType;        //I/Q功率比设置方式，00H表示分贝方式；01H表示功率比值方式。仅在扩频模式1下有效。
	UCHAR   Reserved2;          //保留
	USHORT  ucPoserDB;          //I/Q功率分贝比，16位有符号整型数，量化单位：0.1 dB，取值范围：-20~20dB，
								//缺省值为6dB。仅在扩频模式1下I/Q功率比设置方式为分贝时有效。
	UINT	Reserved3[3];		//保留16个字节，共44个字节

} SSBBERecvCarrSetStruct, *PSSBBERecvCarrSetStruct;

//扩频接收通道参数设置
//     大小：11个32位字
typedef struct SSBBERecvChannelSetBlk
{
	//0xE562
	UCHAR	ucSwitch;			//工作开关,8位无符号整型数,00H表示关；01H表示开
	UCHAR	ucInputSel;			//输入选择,00H表示左旋入；01H表示右旋入
	SHORT	siDoppOffset;		//下行多普勒预置,量化单位：1 kHz，取值范围：－250 ~ 250 kHz，缺省值为0 kHz。
	//
	UCHAR	ucPNSwitch;			//伪码开关,00H表示关；01H表示开
								//注：关闭伪码，可接收单载波或无扩频的BPSK信号
	UCHAR   ucCNO;			    //捕获门限，取值范围：30～100dBHz，量化单位：1 dBHz，缺省值为：40dBHz。
	UCHAR	ucSpnLen;			//短码长度，8位无符号整型数。D7 ~ D0位：00H表示127；01H表示255；02H表示511；03H表示1023。缺省为1023。
	UCHAR   ucPN_ID;			//伪码号，8位无符号整型数。D7 ~ D0位：1～32
	//
	UINT	uiPNRate;			//伪码速率,量化单位：1 c/s，取值范围为1 M ~ 10 M c/s，缺省为5.115 Mc/s
	//
	USHORT	usSPNInitPhaseA;	//短码m1初相，16位无符号整型数。
	USHORT	usSPNPolyA;			//短码m1本原多项式，16位无符号整型数。
	//
	USHORT	usSPNInitPhaseB;	//短码m2初相，16位无符号整型数。
	USHORT	usSPNPolyB;			//短码m2本原多项式，16位无符号整型数
	//
	UINT	uiLPNInitPhase;		//长码初相，32位无符号整型数，仅在扩频模式1时有效。
	//
	UINT	uiLPNPoly;			//长码本原多项式，32位无符号整型数，仅在扩频模式1时有效。
	//
	UCHAR	ucPNPIICarrAid;		//载波辅助伪码捕获，8位无符号整型数。D7 ~ D0位：00H表示无；01H表示有。
	UCHAR   ucPIIType;			//锁相环类型，00H表示二阶环；01H表示载波辅助一阶环。
	USHORT	usPNPIIBw;			//伪码环路带宽，量化单位：0.1 Hz，取值范围：0.1 ~ 32.0 Hz。
	//
	UCHAR	ucPNType;			//伪码类型，8位无符号整型数，D7 ~ D0位：00H表示GOLD码；01H表示m序列。缺省为GOLD码。

	UCHAR   ucPNInterf;			//数据钟与伪码钟是否相干，8位无符号整型数，D7 ~ D0位：00H表示不相干；01H表示相干

	UCHAR   Reserved2[2];		//
	//
	UINT uiReserved3[2];		//
	
} SSBBERecvChannelSetStruct, *PSSBBERecvChannelSetStruct;


//扩频调制载波参数设置,同时设调制器和接收机
//     大小：15个32位字
typedef struct SSBBEModuCarrSetBlk
{
	//调制器：0xE564；接收机：0xE630
	UCHAR	ucWorkMode;			//工作方式，00H表示任务方式；01H表示自检方式,02H表示测试基带上行调制时延（仅在扩频模式2时有效）；
	                            //03H表示测试秒脉冲传输时延（仅在扩频模式2时有效）；04H表示测试星地时差零值（仅在扩频模式2时有效）。
	UCHAR	ucUpConvType;		//上变频方式,8位无符号整型数，00H表示低差；01H表示高差
	UCHAR   ucSys;				//扩频模式，00H表示扩频模式1；01H表示扩频模式2。根据配置内容固定写
	UCHAR   Reserved1;			//保留1个字节
	//
	UINT	uiCarrRF;			//上行点频,32位无符号整型数，量化单位：1 Hz，取值范围为2200 M ~ 2300 MHz
	//
	UINT	uiCarrIF;			//输出中频,32位无符号整型数，量化单位：1 Hz，取值范围为68 M ~ 72 MHz，缺省为70 MHz
	//
	UCHAR   Reserved2;			//保留1个字节
	char	ucOutputPow;		//输出功率,量化单位：1 dBm，取值范围：－60 ~ 0 dBm，缺省为－30 dBm
	UCHAR	ucOnline;			//载波控制，00H表示去载；01H表示加载。
	UCHAR	ucModulate;			//加调/去调,00H表示去调；01H表示加调
	//
	UCHAR   Reserved3[2];		//保留2个字节
	UCHAR	ucAMModu;			//是否调幅调制，00H表示否；01H表示是。
	UCHAR	ucAMDepth;			//调幅调制深度，仅在调幅调制时有效，量化单位：1 %，取值范围：0 ~ 100 %。
	//
	UINT	uiAMFreq;			//调幅调制信号频率，仅在调幅调制时有效，量化单位：1 Hz。
	//
	UCHAR	ucNoiseSw;			//噪声开关，8位无符号整型数，仅在自检工作方式时有效。D7 ~ D0位：
								//00H表示关；01H表示开。缺省值为关。
	UCHAR	ucCN0;				//输出载噪比，8位无符号整型数，量化单位：1 dBHz，取值范围：30 ~ 100 dBHz。
	USHORT  ReZRate;		//回零速率，16位无符号整型数，量化单位1Hz，取值范围：0～15000Hz
	//
	UCHAR	ucDoppCompSwtich;	//多卜勒补偿,补偿开关,00H表示关；01H表示开
	UCHAR	ucDoppCompType;		//多卜勒补偿,补偿方式,00H表示理论预报；01H表示下行多卜勒反推
	USHORT	usDoppCompPrd;		//多卜勒补偿,补偿周期,表示多卜勒补偿的周期，仅在补偿方式为多卜勒补偿时有效，
								//量化单位：1 ms，取值范围：10 ~ 10 000 ms，缺省值为1 000 ms。
	//
	UCHAR	ucDoppDynSwitch;	//多卜勒动态模拟开关，8位无符号整型数，仅在自检工作方式时有效。D7 ~ D0：00H表示关；01H表示开。缺省值为关。
	UCHAR   Reserved5;			//保留1个字节
	USHORT	usDoppDynRange;		//多卜勒变化范围，16位无符号整型数，量化单位：1 kHz，取值范围：0 ~ 250 kHz，缺省为250 kHz。
	//
	USHORT	usDoppDynRate;		//多卜勒动态变化率,量化单位：1 Hz，取值范围：0 ~ 1800 Hz，缺省为0 Hz
	UCHAR	ucPowCompSwitch;	//功率补偿,补偿开关,00H表示关；01H表示开
	UCHAR	ucPowCompType;		//功率补偿,补偿方式,00H表示理论预报；01H表示下行功率反推
	//
	SHORT	siPowCompCyc;		//功率补偿周期，16位无符号整型数，量化单位：1 ms，
								//取值范围：10 ~ 10 000 ms，缺省值为1 000 ms。
	char	cPowCompRef;		//功率补偿基准值,表示下行功率基准值，
								//仅在功率补偿为下行功率反推方式时有效，量化单位：1 dBm
	UCHAR   Reserved6;			//保留1个字节
	//
	//I/Q功率比,16位无符号整型数，仅在扩频模式1时有效。I/Q功率比的缺省值为4/1,
	//注：（I路功率因子/Q路功率因子）为最简分数的形式
	USHORT	usIPowFactor;		//表示I路功率因子,取值范围：1 ~ 100,缺省值为10
	USHORT	usQPowFactor;		//表示Q路功率因子,取值范围：1 ~ 100,缺省值为1；
	
	UCHAR   ucPowerTyep;        //I/Q功率比设置方式，00H表示分贝方式；01H表示功率比值方式。仅在扩频模式1下有效。
	UCHAR   Reserved7;			//保留1个字节
	USHORT  usPowerDB;          //I/Q功率分贝比，16位有符号整型数，量化单位：0.1 dB，取值范围：-20~20dB，
		                        //缺省值为6dB。仅在扩频模式1下I/Q功率比设置方式为分贝时有效。
	//
	UCHAR	Reserved8[8];		//保留12个字节，共60个字节

} SSBBEModuCarrSetStruct, *PSSBBEModuCarrSetStruct;

//扩频调制通道参数设置,同时设调制器和接收机
//     大小：11个32位字
typedef struct SSBBEModuChannelSetBlk
{
	//接收机：0xE632
	UCHAR	ucWorkMode;			//工作方式, 8位无符号整型数，00H:任务方式；01H:自检方式。
	UCHAR	ucOutputSwitch;		//输出开关,8位无符号整型数,00H表示关；01H表示开
	SHORT	siFrqOffset;		//上行多卜勒预置，量化单位：1 kHz，取值范围：－250 ~ 250 kHz，缺省值为0 kHz。
	//
	UCHAR	ucPNIModuSw;		//I路伪码加调/去调，00H表示去调；01H表示加调。
	UCHAR	ucDataModuSw;		//数据调制,表示数据加调/去调，00H表示去调；01H表示加调
	UCHAR	ucSpnLen;			//短码长度，8位无符号整型数。D7 ~ D0位：00H表示127；01H表示255；02H表示511；03H表示1023。缺省1023。
	UCHAR	cPowAdjust;			//功率微调,量化单位：1 dBm，取值范围为－20 ~ 0 dBm，缺省为0 dBm
	//
	UINT	uiPNRate;			//伪码速率,量化单位：1 c/s，取值范围为1.023 M ~ 10.23 M c/s，缺省为5.115 Mc/s
	//
	USHORT	usSPNInitPhaseA;	//短码初相A,16位无符号整型数，D9 ~ D0位有效。
	USHORT	usSPNPolyA;			//短码本原多项式A,16位无符号整型数，D10~ D0位有效
	//
	USHORT	usSPNInitPhaseB;	//短码初相B,16位无符号整型数，D9 ~ D0位有效
	USHORT	usSPNPolyB;			//短码本原多项式B,16位无符号整型数，D10 ~ D0位有效
	//
	//注：以下两个参数仅在扩频模式1的通道1 ~ 3有效
	UINT	uiLPNInitPhase;		//长码初相,32位无符号整型数，D17 ~ D0位有效
	//
	UINT	uiLPNPoly;			//长码本原多项式,32位无符号整型数，D18 ~ D0位有效
	//
	UCHAR   ucPN_ID;			//伪码号，8位无符号整型数。D7 ~ D0位：1～32
	//UCHAR	Reserved;			//保留       20080918  wj
	UCHAR	ucPNInterf;			//数据钟与伪码钟是否相干，8位无符号整型数，D7 ~ D0位：00H表示不相干；01H表示相干。
	UCHAR	ucPNType;			//伪码类型，8位无符号整型数，D7 ~ D0位：00H表示GOLD码；01H表示m序列。
	UCHAR	ucPNQModuSw;		//Q路伪码加调/去调，00H表示去调；01H表示加调。仅在模式1有效。
	//
	int		Reserved1[3];		//共44个字节	

} SSBBEModuChannelSetStruct, *PSSBBEModuChannelSetStruct;

//扩频遥测解调参数设置
//     大小：23个32位字
typedef struct SSBBECTMSetBlk
{
	//0xE570
	UCHAR	ucSwitch;			//工作开关，8位无符号整型数，D7 ~ D0：00H表示关；01H表示开。
	UCHAR	Reserved1;			//保留1个字节
	UCHAR	ucInputSel;			//输入选择，D7 ~ D0位：00H表示视频自检；01H表示中频输入;02H表示卫星模拟器输入
	UCHAR	ucCodeType;			//码型，8位无符号整型数。D7 ~ D0位：
								//00H表示NRZ-L；01H表示NRZ-M；02H表示NRZ-S；03H表示BIφ-L；
								//04H表示BIφ-M；05H表示BIφ-S; 06H表示RNRZ-L。							
	//
	UINT	uiCodeRate;			//码速率，32位无符号整型数，
								//量化单位：1 bps，取值范围：1 kbps ~ 64 kbps。
	//
	UCHAR	Reserved2[4];		//保留4个字节
	//
	UCHAR	ucViterbiCode;		//Viterbi译码，00H表示否；01H表示是,缺省值0.
	UCHAR	ucG2Inv;			//Viterbi译码G2相位反转，00H表示否；01H表示是,缺省值1。
	UCHAR	ucDecision;			//判决方式，00H表示硬判决；01H表示软判决,默认硬判决。
	UCHAR	Reserved3;			//保留1个字节
	//
	UCHAR	Reserved4[4];		//保留4个字节
	//
	UCHAR	ucRandom;			//解扰，00H表示否；01H表示是。
	UCHAR	ucRandLen;			//解扰多项式长度
	USHORT	usRandPoly;			//解扰多项式
	//UCHAR	Reserved5[3];		//保留3个字节
	//
	UCHAR	ucRScode;			//R-S译码，00H表示否；01H表示是。
	UCHAR	ucInterleav;		//交错深度，8位无符号整型数，取值范围：1 ~ 5。
	UCHAR	ucVFill;			//虚拟填充，00H表示无；01H表示有。
	UCHAR	Reserved6[1];		//保留1个字节
	//
	UCHAR	ucDecrypt;			//解密，8位无符号整型数。D7 ~ D0位：00H表示否；01H表示是。
	UCHAR	ucKeySel;			//密钥选择，8位无符号整型数。
	UCHAR	Reserved7[2];		//保留2个字节
	//
	UCHAR	Reserved8[3];		//保留3个字节
	UCHAR	ucFspPlace;			//子帧同步码组位置，8位无符号整型数。D7 ~ D0位：00H表示帧首；01H表示帧尾,缺省值帧尾。
	//
	UCHAR	ucWordLen;			//字长WordLen,8位无符号整型数,量化单位:1 位,取值范围：8~16 位,缺省值为10位。
	UCHAR	ucFspLen;			//帧同步码组长,8位无符号整型数，量化单位：1 位，取值范围：8 ~ 32 位，缺省值为20位。
	USHORT	usFrmLen;			//帧长,16位无符号整型数，量化单位：1 字，取值范围：4 ~ 4096 字，缺省值为50字。
	//
	DWORDLONG	dwlFsp;			//帧同步码组,64位无符号整型数，低位有效,不足64位，高位应补零。
	//
	UCHAR	ucSearchErrBits;	//搜索容错位数，取值范围：0 ~ 7 位；
	UCHAR	ucVerifyErrBits;	//校核容错位数，取值范围：0 ~ 7 位；
	UCHAR	ucLockErrBits;		//锁定容错位数，取值范围：0 ~ 7 位。
	UCHAR	ucVerifyCheckFrame;	//三态逻辑：校核检测帧数，取值范围：1 ~ 8 副帧；
	//
	UCHAR	ucLockCheckFrame;	//三态逻辑：锁定检测帧数，取值范围：1 ~ 8 副帧。
	UCHAR	ucSubFrmType;		//副帧方式，8位无符号整型数。
								//00H表示无副帧；01H表示ID副帧；02H表示循环副帧；03H表示反码副帧,缺省值为无副帧。
	USHORT	usSubFrmLen;		//副帧长度，16位无符号整型数，单位：帧，取值范围：2 ~ 256 帧
	//
	USHORT	usIDSubFrmPos;		//ID副帧ID字位置，单位：字；取值范围：1 ~ 4096字
	UCHAR	ucIdBase;			//ID字基值，取值范围：0或1；
	UCHAR	ucIdDirect;			//ID计数方向，0：加计数，1：减计数。
	//
	DWORDLONG dwlCycFrmSynCode;	//循环副帧码组,64位无符号整型数。
	//
	USHORT	usCycFrmSynCodePos;	//循环副帧码组位置,16位无符号整型数，量化单位：1 字，取值范围：1 ~ 4096 字
	UCHAR	ucCycFrmSynCodeLen;	//循环副帧码组长度,8位无符号整型数，量化单位：1 位，取值范围：8 ~ 32 位
	UCHAR	ucCFSearchErrBits;	//循环副帧搜索容错位数，取值范围：0 ~ 7 位；
	//
	UCHAR	ucCFVerifyErrBits;	//循环副帧校核容错位数，取值范围：0 ~ 7 位；
	UCHAR	ucCFLockErrBits;	//循环副帧锁定容错位数，取值范围：0 ~ 7 位。
	UCHAR	ucSubFVerifyFrame;	//副帧三态逻辑：校核检测帧数，取值范围：1 ~ 8 副帧；
	UCHAR	ucSubFLockFrame;	//副帧三态逻辑：锁定检测帧数，取值范围：1 ~ 8 副帧。
	//
	UCHAR	Reserved9[12];		//保留14个字节,共92个字节

	USHORT  usRandPhase;//加扰初相，16位无符号整型数。
	USHORT	usLoadCnt;           //加载计数 


} SSBBECTMSetStruct, *PSSBBECTMSetStruct;

//扩频编遥模拟源参数设置
//      大小：23个32位字
typedef struct SSBBECTMEmulatorSetBlk
{
	//0xE572
	UCHAR	ucSwitch;			//工作开关，8位无符号整型数，D7 ~ D0：00H表示关；01H表示开。
	UCHAR	ucSource;			//内/外调制，D7 ~ D0：00H表示内调制；01H表示外调制
	UCHAR	ucSendType;			//发送方式，00H：循环发送，01H：连续发送。
	UCHAR	ucCodeType;			//码型，8位无符号整型数。D7 ~ D0位：
								//00H表示NRZ-L；01H表示NRZ-M；02H表示NRZ-S；03H表示BIφ-L；04H表示BIφ-M；05H表示BIφ-S; 06H表示RNRZ-L
	//
	UINT	uiCodeRate;			//码速率，32位无符号整型数，
								//扩频模式定义：量化单位：1 bps，取值范围：1 kbps ~ 64 kbps。
	//
	UCHAR	Reserved[4];		//保留4个字节
	//
	UCHAR	ucConvolCode;		//卷积编码，8位无符号整型数。D7 ~ D0位：00H表示否；01H表示是.
	UCHAR	ucG2Inv;			//卷积编码G2相位反转，8位无符号整型数。D7 ~ D0位：00H表示否；01H表示是, 缺省值1。
	UCHAR	ucG1G2Switch;		//卷积编码G1、G2互换，8位无符号整型数。D7 ~ D0位：00H表示否；01H表示是。
	UCHAR	Reserved1;			//保留1个字节
	//
	UCHAR	Reserved2[4];		//保留4个字节
	//
	UCHAR	ucRandom;			//加扰，8位无符号整型数。D7 ~ D0位：00H表示否；01H表示是,缺省值0。
	UCHAR	ucRandLen;			//加扰多项式长度，8位无符号整型数。
	USHORT  usRandPoly;			//加扰多项式，16位无符号整型数。
	//
	UCHAR	ucRScode;			//R-S编码，8位无符号整型数。D7 ~ D0位：00H表示否；01H表示是。
	UCHAR	ucInterleav;		//交错深度，8位无符号整型数，取值范围：1 ~ 5。
	UCHAR	ucVFill;			//虚拟填充，00H表示无；01H表示有。
	UCHAR	Reserved4[1];		//保留1个字节
	//
	UCHAR	ucEncrypt;			//加密，8位无符号整型数。D7 ~ D0位：00H表示否；01H表示是。
	UCHAR	ucKeySel;			//密钥选择，8位无符号整型数。
	UCHAR	Reserved5[2];		//保留2个字节
	//
	UCHAR	Reserved6[3];		//保留3个字节
	UCHAR	ucFspPlace;			//子帧同步码组位置，8位无符号整型数。D7 ~ D0位：00H表示帧首；01H表示帧尾,缺省值帧尾。
	//
	UCHAR	ucWordLen;			//字长WordLen,8位无符号整型数,量化单位:1 位,取值范围：8~16 位,缺省值为10位。
	UCHAR	ucFspLen;			//帧同步码组长,8位无符号整型数，量化单位：1 位，取值范围：8 ~ 32 位，缺省值为20位。
	USHORT	usFrmLen;			//帧长,16位无符号整型数，量化单位：1 字，取值范围：4 ~ 4096 字，缺省值为50字。
	//
	DWORDLONG	dwlFsp;			//帧同步码组,64位无符号整型数，低位有效,不足64位，高位应补零。
	//
	UCHAR	Reserved7[4];		//保留4个字节
	//
	UCHAR	Reserved8;			//保留1个字节
	UCHAR	ucSubFrmType;		//副帧方式，8位无符号整型数。
								//00H表示无副帧；01H表示ID副帧；02H表示循环副帧；03H表示反码副帧,缺省值为无副帧
	USHORT	usSubFrmLen;		//副帧长度，16位无符号整型数，单位：帧，取值范围：2 ~ 256 帧。
	//
	USHORT	usIDSubFrmPos;		//ID副帧ID字位置，单位：字；取值范围：1 ~ 4096字
	UCHAR	ucIdBase;			//ID字基值，取值范围：0或1；
	UCHAR	ucIdDirect;			//ID计数方向，0：加计数，1：减计数。
	//
	DWORDLONG dwlCycFrmSynCode;	//循环副帧码组,64位无符号整型数。
	//
	USHORT	usCycFrmSynCodePos;	//循环副帧码组位置,16位无符号整型数，量化单位：1 字，取值范围：1 ~ 4096 字
	UCHAR	ucCycFrmSynCodeLen;	//循环副帧码组长度,8位无符号整型数，量化单位：1 位，取值范围：8 ~ 32 位
	UCHAR	Reserved9;			//保留1个字节
	//
        UINT	Rsvd4[3];
        BYTE        ucDataType;		//生成的数据类型	1字节	字节	0：随机码   1：固定数
        USHORT	usConstVal;		//常数数据
        UCHAR	Reserved10;			//保留1个字节
	//
	USHORT usRandPhase;			//加扰初相，16位无符号整型数。
        USHORT Reserved11;

} SSBBECTMEmulatorSetStruct, *PSSBBECTMEmulatorSetStruct;


//检测输出，同时设调制器和接收机
typedef struct SSBBETestOutSetBlk
{
	//接收机：0xE634
	UCHAR	D1Out;				//数字检测信号输出选择，
								//8位无符号整型数。定义见"检测输出定义表"。
	UCHAR	Reserved1[3];		//保留3个字节
	//
	UCHAR	Reserved[24];		//保留24个字节

} SSBBETestOutSetStruct, *PSSBBETestOutSetStruct;
//	表1  数字检测信号输出定义表
//	-----------------------------------------------------------
//	D1Out设置	数字信号检测输出1		数字信号检测输出2
//	-----------------------------------------------------------
//	接收机	00H	遥测模拟源帧脉冲（流1）	遥测解调帧脉冲（流1）
//			01H	遥测模拟源帧脉冲（流2）	遥测解调帧脉冲（流2）
//			02H	遥测模拟源帧脉冲（流3）	遥测解调帧脉冲（流3）
//			03H	遥测解调码钟（流1）		遥测解调数据  （流1）
//			04H	输入解密数据（流1）		输入解密数据码钟（流1）
//	-----------------------------------------------------------
//	调制器	00H	遥控数据（流1）			小环解调遥控数据（流1）
//			01H	遥控数据（流2）			小环解调遥控数据（流2）
//			02H	遥控数据（流3）			小环解调遥控数据（流3）
//	-----------------------------------------------------------
//	注：
//	（1）	接收机和调制器均具有2路数字信号检测输出的能力。
//	-----------------------------------------------------------

////////////////////////////////////////单命令//////////////////////////////////
//通道重捕
//大小：7个32位字
typedef struct SSBBEChnlReCaptureBlk
{
	UINT	uiCmdItem;		//单命令项目，0031 0500H表示"通道重捕"。
	//
	UCHAR	ucbChnl1;		//是否重捕第1路接收通道，00H表示否；01H表示是。	
	UCHAR	ucbChnl2;		//是否重捕第2路接收通道，00H表示否；01H表示是。	
	UCHAR	ucbChnl3;		//是否重捕第3路接收通道，00H表示否；01H表示是。		
	UCHAR	ucbChnl4;		//是否重捕第4路接收通道，00H表示否；01H表示是。	
							//仅在扩频模式2时接收机硬件板有效。
	//
	UCHAR	ucbChnl5;		//是否重捕第5路接收通道，00H表示否；01H表示是。		
							//仅在扩频模式2时接收机硬件板有效。
	UCHAR	ucbChnl6;		//是否重捕第6路接收通道，00H表示否；01H表示是。		
							//仅在扩频模式2时接收机硬件板有效。
	UCHAR	Reserved1[2];	//
	//
	UINT	Reserved2[4];
	
} SSBBEChnlReCaptureStruct, *PSSBBEChnlReCaptureStruct;

// 上行输出
// 大小：7个32位字
typedef struct SSBBEUpOutBlk
{
	UINT	uiCmdItem;		//单命令项目，0031 0600H表示"上行输出"。
	//
	UCHAR	ucOutput;		//8位无符号整型数，00H表示去载；01H表示加载。
	UCHAR	Reserved1[3];			
	//
	UINT	Reserved2[5];

} SSBBEUpOutStruct, *PSSBBEUpOutStruct;


// 长码重捕
// 大小：7个32位字
typedef struct SSBBELPNReCaptureBlk
{
	UINT	uiCmdItem;		//单命令项目，0031 0501H表示"长码重捕"，仅在扩频模式1时有效。
	//
	UCHAR	ucbChnl1;		//是否重捕第1路接收通道的长码，00H表示否；01H表示是。	
	UCHAR	ucbChnl2;		//是否重捕第2路接收通道的长码，00H表示否；01H表示是。	
	UCHAR	ucbChnl3;		//是否重捕第3路接收通道的长码，00H表示否；01H表示是。		
	UCHAR	Reserved1;			
	//
	UINT	Reserved2[5];
	
} SSBBELPNReCaptureStruct, *PSSBBELPNReCaptureStruct;

//
//遥测数据强制读
//大小：7个32位字
typedef struct SSBBETMForceReadBlk
{
	UINT	uiCmdItem;		//单命令项目，0031 0800H表示"遥测数据强制读"。
	//
	UCHAR	ucTM1;			//是否读取第1路遥测数据，00H表示否；01H表示是。
	UCHAR	ucTM2;			//是否读取第2路遥测数据，00H表示否；01H表示是。
	UCHAR	ucTM3;			//是否读取第3路遥测数据，00H表示否；01H表示是。
	UCHAR	Reserved1;			
	//
	UINT	Reserved2[5];

} SSBBETMForceReadStruct, *PSSBBETMForceReadStruct;
/*
//调制闪断
//大小：7个32位字
typedef struct	SSBBEModulFlashIntBlk
{
	UINT	uiCmdItem;		//单命令项目，0031 0900H表示"调制闪断"。

	USHORT	usTimeLen;		//闪断时间长度，16位无符号整型数，单位：ms。
	UCHAR	Reserved1[2];			
	//
	UINT	Reserved2[5];

} SSBBEModulFlashIntStruct, *PSSBBEModulFlashIntStruct;
*/
//单命令
 typedef union SSBBESingleCmdBlk
 {
 	//接收机：0xE642；调制器：0xE644
 	SSBBEChnlReCaptureStruct	ChnlReCapture;	//通道重捕，接收机
 	SSBBEUpOutStruct			UpOute;			//上行输出，调制器
 	SSBBELPNReCaptureStruct		LPNReCapture;	//长码重捕，接收机
 	SSBBETMForceReadStruct		TMForceRead;	//遥测数据强制读，接收机
 
 } SSBBESingleCmdStruct, *PSSBBESingleCmdStruct;


/****************************** 状态结构 ***************************************/

//扩频引导接收机状态
//     大小：11+8个32位字
typedef struct BBEGuidTrackStatusBlk
{
	//0xE553
	BBEGuidTrackSetStruct	tParams;	//参数数据

	SHORT	siAzimuV;			//方位误差电压,16位有符号整型数，量化单位：10/2^15 V
	SHORT	siElevaV;			//俯仰误差电压,16位有符号整型数，量化单位：10/2^15 V
	//
	UCHAR	ucAutoPhStatus;		//自动校相状态,00H无效；01H示正在进行自动校相；02H表示自动校准失败；FFH值表示自动校相结束
	UCHAR	Reserved1[3];  	    //保留3个字节
	//USHORT	usPhaseAdjust;		//相位调整量，16位无符号整型数，仅在自动校相时有效，量化单位：360°/1024。
	//
	SHORT	siAzimuZeroAmend;	//方位零值修正,16位有符号整型数，量化单位：10/2^15 V。
	SHORT	siElevaZeroAmend;	//俯仰零值修正,16位有符号整型数，量化单位：10/2^15 V。
	//
	int		iAzimuSlope;		//方位斜率,32位有符号整型数,仅在方位拉偏自动校相结束时有效。
	//
	int		iElevaSlope;		//俯仰斜率,32位有符号整型数,仅在俯仰拉偏自动校准结束时有效。
	//
	UCHAR	Reserved2[12];		//保留3个字节，共32个字节+参数44个字节

} BBEGuidTrackStatusStruct, *PBBEGuidTrackStatusStruct;

//输出选择状态
//     大小：8+4个32位字
typedef struct SSBBEOutputSelStatusBlk
{
	//0x1107
	SSBBEOutputSelSetStruct	tParams;	//参数数据
	//
	UCHAR	ucObjOutSel;		//输出指示，8位无符号整型数，00H表示左旋；01H表示右旋。
	UCHAR	ucChanOutSel;		//通道输出指示，8位无符号整型数，仅在扩频模式2时有效。00H表示遥测通道角误差输出；01H表示测量通道角误差输出。
	USHORT	Reserved1;			//
	//
	int		Reserved2[3];		//

} SSBBEOutputSelStatusStruct, *PSSBBEOutputSelStruct;

//扩频接收载波状态
typedef struct SSBBERecvCarrStatusBlk
{
	//0xE559
	SSBBERecvCarrSetStruct	tParams;	//参数数据

	USHORT	usLeftAgcV;			//左旋非相干AGC电压,16位无符号整型数，
								//取值范围：0 ~ 5000，对应电压0.00 ~ 5.00 V
	USHORT	usRightAgcV;		//右旋非相干AGC电压,16位无符号整型数，
								//取值范围：0 ~ 5000，对应电压0.00 ~ 5.00 V
	//
	UCHAR	Reserved[28];		//保留28个字节，共32个字节+44个字节


} SSBBERecvCarrStatusStruct, *PSSBBERecvCarrStatusStruct;

//扩频接收通道状态
//     大小：11＋20个32位字
typedef struct SSBBERecvChannelStatusBlk
{
	//0xE561
	SSBBERecvChannelSetStruct	tParams;	//参数数据
	//
	USHORT	usAgcV;				//AGC电压，16位无符号整型数，
								//取值范围：0 ~ 5000，对应电压0.00 ~ 5.00 V
	UCHAR	ucAgcStab;			//AGC稳定指示，00H表示否；01H表示是。
	UCHAR	Reserved1;			//
	//
	UINT	uiStabTime;			//AGC稳定时间，32位无符号整型数，量化单位：25us。
	//
	UINT	uiPowAv;			//信号能量的均值，32位无符号整型数。
	//
	USHORT	usIPow;				//I路能量均值，16位无符号整型数，仅在扩频模式1时有效。
	USHORT	usQPow;				//Q路能量均值，16位无符号整型数，仅在扩频模式1时有效。
	//
	UINT	uiAmplAv;			//信号幅度的均值，32位无符号整型数。
	//
	UINT	uiPdErrAv;			//鉴相误差幅度的均值，32位无符号整型数。
	//
	UCHAR	ucCarrLock;			//载波锁定指示,00H表示失锁；01H表示锁定
	UCHAR	ucSPNLock;			//短码锁定指示,00H表示短码失锁；01H表示短码锁定
	UCHAR	ucLPNLock;			//长码锁定指示,00H表示长码失锁；01H表示长码锁定
	UCHAR	Reserved2;			//保留1个字节
	//
	int		iCarrDoppNco;		//载波多卜勒的NCO值，32位有符号整型数，量化单位：工作时钟/2^32 Hz。
	//
	UINT	uiSignalPow;		//信号能量,表示解扩恢复的载波能量值
	//
	UINT	uiNoisePow;			//噪声能量,表示测得的噪声能量值,载噪比C/N0的计算方法：待定。
	//
	int		iDopRate;			//多普勒变化率的NCO值，量化单位：工作时钟/2^32 Hz/s。
	//
	UINT	uiAcquTime;			//捕获时间32位无符号整型数，
								//量化单位：1 ms，0表示无效。
	//
	int		iPNDopNco;			//伪码多卜勒的NCO值，32位有符号整型数，量化单位：工作时钟/232 Hz。
								//工作时钟暂定为838.9*10^6/9Hz
	//
	UCHAR	B000T25us1[8];		//下行载波锁定时刻，48位无符号整型数，为25us时码格式。
	                            //下行载波锁定时刻，48位无符号整型数，为25us时码格式。D15 ~ D0位：
	                            //16位无符号整型数，表示1秒内的时间，量化单位：25 us。D19 ~ D16位：
	                            //量化单位为1s；D23 ~ D20位：量化单位为10s；D27 ~ D24位：量化单位为1min；
	                            //D31 ~ D28位：量化单位为10min；D35 ~ D32位：量化单位为1hour；D37 ~ D36位：
	                            //量化单位为10hour；D41 ~ D38位：量化单位为1day；D45 ~ D42位：量化单位为10day；D47 ~ D46位：量化单位为100day。
	UCHAR	B000T25us2[8];		//下行载波失锁时刻，48位无符号整型数，为25us时码格式。
	                            //下行载波失锁时刻，48位无符号整型数，为25us时码格式。D15 ~ D0位：
	                            //16位无符号整型数，表示1秒内的时间，量化单位：25 us。D19 ~ D16位：
	                            //量化单位为1s；D23 ~ D20位：量化单位为10s；D27 ~ D24位：量化单位为1min；
	                            //D31 ~ D28位：量化单位为10min；D35 ~ D32位：量化单位为1hour；D37 ~ D36位：
	                            //量化单位为10hour；D41 ~ D38位：量化单位为1day；D45 ~ D42位：量化单位为10day；D47 ~ D46位：量化单位为100day。
	//
	UCHAR	Reserved3[12];		//保留12个字节，共80个字节+44个字节


} SSBBERecvChannelStatusStruct, *PSSBBERecvChannelStatusStruct;

//扩频调制载波状态,调制器和接收机同时返回状态
//     大小：15+0个32位字
typedef struct SSBBEModuCarrStatusBlk
{
        ////调制器：0xE563 接收机：0x629
	SSBBEModuCarrSetStruct	tParams;	//参数数据

} SSBBEModuCarrStatusStruct, *PSSBBEModuCarrStatusStruct;

//扩频调制通道状态,调制器和接收机同时返回状态
//     大小：11+4个32位字
typedef struct SSBBEModuChannelStatusBlk
{
	//接收机：0xE631
	SSBBEModuChannelSetStruct	tParams;	//参数数据
	//
	int		iDoppNco;			//上行多普勒频偏的NCO值，32位有符号整型数，量化单位：工作时钟/2^32 Hz。
	//
	char	cLevel;				//上行功率值，8位有符号整型数，量化单位：1 dBm。
	//20070613 增加
	UCHAR	ucReZero;			//多普勒回零标志，00H表示多普勒未回零；01H表示多普勒回零。
	UCHAR	Reserved1[2];		//保留2个字节
	//
	UCHAR	Reserved2[8];		//保留8个字节，共16个字节+44个字节

} SSBBEModuChannelStatusStruct, *PSSBBEModuChannelStatusStruct;

//编遥解调状态
//     大小：23＋8个32位字
typedef struct SSBBECTMStatusBlk
{
	//0xE569
	SSBBECTMSetStruct	tParams;	//参数数据
	//
	UCHAR	ucCodeLock;			//码同步器锁定指示，8位无符号整型数，
								//D7 ~ D0位，00H：失锁，01H：锁定。
	UCHAR	ucFrmLock;			//帧同步器锁定指示，8位无符号整型数，
								//D7 ~ D0位，00H：失锁，01H：锁定。
	UCHAR	ucSubFrmLock;		//副帧同步器锁定指示，8位无符号整型数，
								//D7 ~ D0位，00H：失锁，01H：锁定。
	UCHAR	Reserved1;			//保留1个字节
	//
	UCHAR	Reserved2[4];		//保留4个字节
	//
	int		iCodeDopNco;		//码多普勒的NCO值，量化单位：工作时钟/2^32 Hz。
	//
	UINT	uiEb;				//码元能量，32位无符号整型数，
	UINT	uiN0;				//噪声能量，32位无符号整型数，
								//码元信噪比Eb/N0的计算方法：Eb/N0=10lg(Eb/N0)。
	//
	UINT	uiDemDelay;			//解调时延，32位无符号整型数，量化单位：25us。
	//
	UCHAR	Reserved3[8];		//保留8个字节，共32个字节+92个字节

} SSBBECTMStatusStruct, *PSSBBECTMStatusStruct;

//编遥模拟源状态
typedef struct SSBBECTMEmulatorStatusBlk
{
	//0xE571
	SSBBECTMEmulatorSetStruct	tParams;	//参数数据
	//
	UCHAR	B000T[8];			//每帧第1位数据的调制时刻，48位无符号整型数，为0.1ms时码格式。
								//D3 ~ D0：量化单位为0.1ms；D7 ~ D4：量化单位为1ms；
								//D11 ~ D8：量化单位为10ms；D15 ~ D12：量化单位为100ms；
								//D19 ~ D16：量化单位为1s；D23 ~ D20：量化单位为10s；
								//D27 ~ D24：量化单位为1min；D31 ~ D28：量化单位为10min；
								//D35 ~ D32：量化单位为1hour；D37 ~ D36：量化单位为10hour；
								//D41 ~ D38：量化单位为1day；D45 ~ D42：量化单位为10day；
								//D47 ~ D46：量化单位为100day。
	UCHAR	Reserved[8];		//保留10个字节，共16个字节+92个字节

} SSBBECTMEmulatorStatusStruct, *PSSBBECTMEmulatorStatusStruct;

// 检测输出状态
// 大小：7个32位字
typedef struct SSBBETestOutStatusBlk
{
	//接收机：0x633
	SSBBETestOutSetStruct Param;

} SSBBETestOutStatusStruct, *PSSBBETestOutStatusStruct;




/*********************************** 数据结构 **************************************/

//角误差解调数据
//     大小：3个32位字
typedef struct BBEAngleDiffDataBlk
{
	//0x0F03
	UCHAR	B000T25us[6];		//时码，48位无符号整型数，为25us时码格式。
								//D15 ~ D0位：16位无符号整型数，表示1秒内的时间，量化单位：25 us。
								//D19 ~ D16：量化单位为1s；D23 ~ D20：量化单位为10s；
								//D27 ~ D24：量化单位为1min；D31 ~ D28：量化单位为10min；
								//D35 ~ D32：量化单位为1hour；D37 ~ D36：量化单位为10hour；
								//D41 ~ D38：量化单位为1day；D45 ~ D42：量化单位为10day；
								//D47 ~ D46：量化单位为100day。

	UCHAR	ucSN;				//数据流号，8位无符号整型数。D7 ~ D0位：00H表示左旋；01H表示右旋。
	UCHAR	Reserved;			//保留1个字节
	//
	short	siAzimuV;			//方位误差电压,16位有符号整型数，量化单位：10/2^15 V
	short	siElevaV;			//俯仰误差电压,16位有符号整型数，量化单位：10/2^15 V

} BBEAngleDiffDataStruct, *PBBEAngleDiffDataStruct;

// 3.5 遥测解调数据（位流格式）
typedef struct
{
	//0x0401
	ULONG   ulB000_L32;
	
	USHORT  usB000_H16;
	USHORT  usReserved;
	
	USHORT Word[MaxDataLen];

} ctm_bit_struct;


//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------*/
/*---------------------自动化测试 结构体  ------------------------------*/
/*----------------------------------------------------------------------*/
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

///////////////////////CDMA体制/////////////////////////////////////////////
/*********************自动化测试******************/

// ----------- hfn -------------- 20081024 add
//1.AGC控制精度调制器部分
typedef struct SSBBEModuAgcCtlCmdBlk
{
	UINT	uiTestItem;	// 测试项目，32位无符号整型数，00C5 0400H表示"AGC控制范围、精度"。

	USHORT	usTestCh;	// 测试启停控制命令，16位无符号整型数，D31 ~ D0位：00H表示停止测试；01H表示启动测试。
	SHORT	Level;		// 输出功率，8位有符号整型数，量化单位：1 dBm，取值范围：－80 ~ -10 dBm
		
	UINT	Rsvd[5];

} SSBBEModuAgcCtlCmdStruct, *PSSBBEModuAgcCtlCmdStruct;

//2.AGC控制精度接收机部分
typedef struct SSBBERecAgcCtlCmdBlk
{
	UINT	uiTestItem;		// 测试项目，32位无符号整型数，00C5 0400H表示"AGC控制范围、精度"。

	USHORT	usTestCh;		// 测试启停控制命令，16位无符号整型数，D31 ~ D0位：00H表示停止测试；01H表示启动测试。
	USHORT	usReserved;		//
		
	UINT	Rsvd[5];

} SSBBERecAgcCtlCmdStruct, *PSSBBERecAgcCtlCmdStruct;

//3.AGC时常数调制器部分
typedef struct SSBBEModuAgcTCmdBlk
{
	UINT	uiTestItem;		// 测试项目，32位无符号整型数，00C5 0401H表示"AGC时间常数"。

	USHORT	usTestCh;		// 测试启停控制命令，16位无符号整型数，D15 ~ D0位：00H表示停止测试；01H表示开始测试。
	USHORT	usCarrOnOff;	// 载波开关信号，16位无符号整型数，D15~D0位：00H表示去载，01H表示加载
		
	UINT	Rsvd[5];

} SSBBEModuAgcTCmdStruct, *PSSBBEModuAgcTCmdStruct;

//4.AGC时常数接收机部分
typedef struct SSBBERecAgcTCmdBlk
{
	UINT	uiTestItem;		// 测试项目，32位无符号整型数，00C5 0401H表示"AGC时间常数"。

	USHORT	usTestCh;		// 测试启停控制命令，16位无符号整型数，D15 ~ D0位：00H表示停止测试；01H表示开始测试。
	USHORT	usAgcTime;		// AGC时间常数，16位无符号整型数，D15 ~ D0位：00H表示1ms；01H表示10ms；
							// 02H表示50ms；03H表示250ms；04H表示1000ms
		
	UINT	Rsvd[5];

} SSBBERecAgcTCmdStruct, *PSSBBERecAgcTCmdStruct;

//5. 等效环路带宽调制器部分
typedef struct SSBBEModuPIIBwCmdBlk
{
	UINT	uiTestItem;		// 测试项目，32位无符号整型数，00C5 0402H表示"等效环路带宽"。

	USHORT	usTestCh;		// 测试启停控制命令，16位无符号整型数，D15 ~ D0位：00H表示停止测试；01H表示开始测试。
	USHORT	usReserved;
	
	UINT    uiDropCha;		// 多普勒变化率，32位无符号整型数，量化单位：1 Hz/s
	UINT    Rsvd[4];

} SSBBEModuPIIBwCmdStruct, *PSSBBEModuPIIBwCmdStruct;

//6. 等效环路带宽接收机部分
typedef struct SSBBERecPIIBwCmdBlk
{
	UINT	uiTestItem;		// 测试项目，32位无符号整型数，00C5 0402H表示"等效环路带宽"。

	USHORT	usTestCh;		// 测试启停控制命令，16位无符号整型数，D15 ~ D0位：00H表示停止测试；01H表示开始测试。
	USHORT	usPllBw;		// 载波环路带宽, 16位无符号整型数。D15 ~ D0位：00H表示5Hz；01H表示10Hz；
							// 02H表示100Hz；03H表示500Hz；04H表示1kHz；05H表示2kHz。

	UINT	Rsvd[5];

} SSBBERecPIIBwCmdStruct, *PSSBBERecPIIBwCmdStruct;

//20061122 增加下述测试项目

//7.多普勒动态范围调制器部分
typedef struct SSBBEModuDoppRangeBlk
{
	//0x
	UINT	uiTestItem;		//测试项目，32位无符号整型数，00C5 0403H表示"多普勒动态范围测试"。
	//
	USHORT	usTestCh;		//测试启停控制命令，16位无符号整型数，D15 ~ D0位：00H表示停止测试；01H表示开始测试。
	USHORT	usReserved1;	//
	//
	short	siOffSet;		//调制频偏，16位有符号整型数，单位：KHz
	USHORT	usReserved2;	//	
	//
	UINT	uiRsvd[4];		//

} SSBBEModuDoppRangeStruct, *PSSBBEModuDoppRangeStruct;

//8.多普勒动态范围接收机部分
typedef struct SSBBERecDoppRangeBlk
{
	//0x
	UINT	uiTestItem;		//测试项目，32位无符号整型数，00C5 0403H表示"多普勒动态范围测试"。
	//
	USHORT	usTestCh;		//测试启停控制命令，16位无符号整型数，D15 ~ D0位：00H表示停止测试；01H表示开始测试。
	short	siOffSet;		//调制频偏，16位有符号整型数，单位：KHz
	//		
	UINT	uiRsvd[5];		//

} SSBBERecDoppRangeStruct, *PSSBBERecDoppRangeStruct;

//9. 多普勒一次变化率调制器部分
typedef struct	SSBBEModuDoppRateBlk
{
	//
	UINT	uiTestItem;		//测试项目，32位无符号整型数，00C5 0404H表示"多普勒一次变化率"。
	//
	USHORT	usTestCh;		//测试启停控制命令，16位无符号整型数，D15 ~ D0位：00H表示停止测试；01H表示开始测试。
	USHORT	usReserved;		//
	//
	UINT	uiSimRate;		//多普勒变化率，32位无符号整型数，量化单位：1 Hz/s，取值范围0～15000。
	//
	UINT	uiRsvd[4];		//

} SSBBEModuDoppRateStruct, *PSSBBEModuDoppRateStruct;

//10. 多普勒一次变化率接收机部分
typedef struct SSBBERecDoppRateBlk
{
	//
	UINT	uiTestItem;		//测试项目，32位无符号整型数，00C5 0404H表示"多普勒一次变化率"。
	//
	USHORT	usTestCh;		//测试启停控制命令，16位无符号整型数，D15 ~ D0位：00H表示停止测试；01H表示开始测试
	USHORT	usPllBw;		//载波环路带宽, 16位无符号整型数。D15 ~ D0位：
							//00H表示5Hz；01H表示10Hz；02H表示100Hz；03H表示500Hz；04H表示1kHz；05H表示2kHz。
							//2304:::00H表示100Hz；01H表示200Hz；02H表示500Hz；03H表示1000Hz
	//	
	UINT	uiRsvd[5];		//

} SSBBERecDoppRateStruct, *PSSBBERecDoppRateStruct;

//11. 遥测解调时延调制器部分
typedef struct	SSBBEModuTMDelayBlk
{
	//
	UINT	uiTestItem;		//测试项目，32位无符号整型数，00C5 0405H表示"遥测解调时延"。
	//
	USHORT	usTestCh;		//测试启停控制命令，16位无符号整型数，D15 ~ D0位：00H表示停止测试；01H表示开始测试。
	USHORT	usReserved;		//
	//
	USHORT	usConvol;		//卷积编码，0表示不编码，1表示编码。
    USHORT	usG2Inv;		//G2支路反转，0表示不反转，1表示反转。
	//
	UINT	uiRsvd[4];		//

} SSBBEModuTMDelayStruct, *PSSBBEModuTMDelayStruct;

//12. 遥测解调时延接收机部分
typedef struct	SSBBERecTMDelayBlk
{
	//
	UINT	uiTestItem;		//测试项目，32位无符号整型数，00C5 0405H表示"遥测解调时延"。
	//
	USHORT	usTestCh;		//测试启停控制命令，16位无符号整型数，D15 ~ D0位：00H表示停止测试；01H表示开始测试。
	USHORT	usReserved;		//
	//	
	USHORT	usViterbi;		//Viterbi译码，0表示不译码，1表示译码。
    USHORT	usG2Inv;		//G2支路反转，0表示不反转，1表示反转。
	//	
	UINT	uiRsvd[4];		//

} SSBBERecTMDelayStruct, *PSSBBERecTMDelayStruct;

//13.捕获时间测试
typedef struct SSBBEModulCaptureTTestBlk
{
	//
	UINT	uiTestItem;		//测试项目，32位无符号整型数，00C5 0408H表示"捕获时间测试"。
	//
	USHORT	usTestCh;		//测试启停控制命令，16位无符号整型数，D15 ~ D0位：00H表示停止测试；01H表示开始测试。
	USHORT	usReserved;		//
	//	
	UINT	uiRsvd[5];

} SSBBEModulCaptureTTestStruct, *PSSBBEModulCaptureTTestStruct;

//14.捕获时间测试
#define SSBBERecCaptureTTestStruct SSBBEModulCaptureTTestStruct

//15. 中频信号幅度测试调制器部分
typedef struct SSBBEModuISDRCmdBlk
{
	UINT	uiTestItem;

	USHORT	usTestCh;
	USHORT	usReserved;	

	UINT	uiRsvd[5];

}SSBBEModuISDRCmdStruct, *PSSBBEModuISDRCmdStruct;

//16.中频信号幅度接收机部分
typedef struct SSBBERecISDRCmdBlk
{
	UINT	uiTestItem;

	USHORT	usTestCh;
	USHORT	usReserved;
}SSBBERecISDRCmdStruct, *PSSBBERecISDRCmdStruct;
//20021122 end

//状态结构体:
//1.AGC控制精度调制器部分
// 注：调制器只报响应参数，接收机报参数和响应内容
typedef struct SSBBEModuAgcCtlAnsBlk
{
	UINT	uiTestItem;	// 测试项目，32位无符号整型数，00C5 0400H表示"AGC控制范围、精度"。

	USHORT	usTestCh;	// 测试启停控制命令，16位无符号整型数，D31 ~ D0位：00H表示停止测试；01H表示启动测试。
	short	Level;		// 输出功率，8位有符号整型数，量化单位：1 dBm，取值范围：－80 ~ -10 dBm
		
	UINT	Rsvd[6];

} SSBBEModuAgcCtlAnsStruct, *PSSBBEModuAgcCtlAnsStruct;

//2.AGC控制精度接收机部分
typedef struct SSBBERecAgcCtlAnsBlk
{
	UINT	uiTestItem;		// 测试项目，32位无符号整型数，00C5 0400H表示"AGC控制范围、精度"。

	USHORT	usTestCh;		// 测试启停控制命令，16位无符号整型数，D31 ~ D0位：00H表示停止测试；01H表示启动测试。
	USHORT	usReserved;		//
	
	UCHAR	ucLock0;		// 8位无符号整型，目标1锁定，1表锁定
	UCHAR	ucLock1;		// 8位无符号整型，目标2锁定，1表锁定
	UCHAR	ucLock2;		// 8位无符号整型，目标3锁定，1表锁定
	UCHAR	ucReserved;
	
	UINT	uiSignalAmg0;	// 32位无符号整型数，D31 ~ D0位：表示目标1信号当前调制电平下的接收信号的功率。
	UINT	uiSignalAmg1;	// 32位无符号整型数，D31 ~ D0位：表示目标2信号当前调制电平下的接收信号的功率。
	UINT	uiSignalAmg2;	// 32位无符号整型数，D31 ~ D0位：表示目标3信号当前调制电平下的接收信号的功率。
	
	UINT	uiTestSta;		// 32位无符号整型数，D31~D0位，01H表示正在测试中，02H表示测试完毕。
	UINT	uiSignalAmg3;	// 32位无符号整型数，D31 ~ D0位：表示目标1信号当前调制电平下的接收信号的功率。
	UINT	uiSignalAmg4;	// 32位无符号整型数，D31 ~ D0位：表示目标2信号当前调制电平下的接收信号的功率。
	UINT	uiSignalAmg5;	// 32位无符号整型数，D31 ~ D0位：表示目标3信号当前调制电平下的接收信号的功率。
	UINT	uiRsv[1];			//

} SSBBERecAgcCtlAnsStruct, *PSSBBERecAgcCtlAnsStruct;

//3.AGC时常数调制器部分
//注：调制器只报响应参数，接收机报参数和响应内容
typedef struct SSBBEModuAgcTAnsBlk
{
	UINT	uiTestItem;		// 测试项目，32位无符号整型数，00C5 0401H表示"AGC时间常数"。

	USHORT	usTestCh;		// 测试启停控制命令，16位无符号整型数，D15 ~ D0位：00H表示停止测试；01H表示开始测试。
	USHORT	usCarrOnOff;	// 载波开关信号，16位无符号整型数，D15~D0位：00H表示去载，01H表示加载
		
	UINT	Rsvd[6];

} SSBBEModuAgcTAnsStruct, *PSSBBEModuAgcTAnsStruct;

//4.AGC时常数接收机部分
typedef struct	SSBBERecAgcTAnsBlk
{
	UINT	uiTestItem;		// 测试项目，32位无符号整型数，00C5 0401H表示"AGC时间常数"。

	USHORT	usTestCh;		// 测试启停控制命令，16位无符号整型数，D15 ~ D0位：00H表示停止测试；01H表示开始测试。
	USHORT	usAgcTime;		// AGC时间常数，16位无符号整型数，D15 ~ D0位：00H表示1ms；01H表示10ms；
							// 02H表示50ms；03H表示250ms；04H表示1000ms
	
	UINT	uiAgcTime0;		// 表示目标1 AGC时间常数，32位无符号整型数，量化单位：25us。
	UINT	uiAgcTime1;		// 表示目标2 AGC时间常数，32位无符号整型数，量化单位：25us。
	UINT	uiAgcTime2;		// 表示目标3 AGC时间常数，32位无符号整型数，量化单位：25us。
	
	UINT	uiSlipOver;		//AGC电压平滑结束标志,1表示AGC电压平滑结束可以进行下一步
	UINT	uiTestSta;		// 32位无符号整型数，D31~D0位，01H表示正在测试中，02H表示测试完毕。
	UINT	uiAgcTime3;		// 表示目标1 AGC时间常数，32位无符号整型数，量化单位：25us。
	UINT	uiAgcTime4;		// 表示目标2 AGC时间常数，32位无符号整型数，量化单位：25us。
	UINT	uiAgcTime5;		// 表示目标3 AGC时间常数，32位无符号整型数，量化单位：25us。
	UINT	uiRsvd[1];

} SSBBERecAgcTAnsStruct, *PSSBBERecAgcTAnsStruct;

//5. 等效环路带宽调制器部分
//注：调制器只报响应参数，接收机报参数和响应内容
typedef struct SSBBEModuPIIBwAnsBlk
{
	UINT	uiTestItem;		// 测试项目，32位无符号整型数，00C5 0402H表示"等效环路带宽"。

	USHORT	usTestCh;		// 测试启停控制命令，16位无符号整型数，D15 ~ D0位：00H表示停止测试；01H表示开始测试。
	USHORT	usReserved;
	
	UINT    uiDropCha;		// 多普勒变化率，32位无符号整型数，量化单位：1 Hz/s
	UINT    Rsvd[5];

} SSBBEModuPIIBwAnsStruct, *PSSBBEModuPIIBwAnsStruct;

//6. 等效环路带宽接收机部分
typedef struct SSBBERecPIIBwAnsBlk
{
	UINT	uiTestItem;		// 测试项目，32位无符号整型数，00C5 0402H表示"等效环路带宽"。

	USHORT	usTestCh;		// 测试启停控制命令，16位无符号整型数，D15 ~ D0位：00H表示停止测试；01H表示开始测试。
	USHORT	usPllBw;		// 载波环路带宽, 16位无符号整型数。D15 ~ D0位：00H表示5Hz；01H表示10Hz；
							// 02H表示100Hz；03H表示500Hz；04H表示1kHz；05H表示2kHz。

    UCHAR	ucLock0;		//目标1锁定，1有效
    UCHAR	ucLock1;		//目标2锁定，1有效
    UCHAR	ucLock2;		//目标3锁定，1有效
	UCHAR	ucReserved;		
	
	USHORT	usPdErrAv0;		// 16位无符号整型数，表示目标1鉴相误差幅值均值；对扩频模式2，表示遥测通道1鉴相误差幅值均值；
	USHORT	usAmplAv0;		// 16位无符号整型数，表示目标1信号幅度的均值；对扩频模式2，表示遥测通道1信号幅度的均值；

	USHORT	usPdErrAv1;		// 16位无符号整型数，表示目标2鉴相误差幅值均值；对扩频模式2，表示遥测通道2鉴相误差幅值均值；
	USHORT	usAmplAv1;		// 16位无符号整型数，表示目标2信号幅度的均值；对扩频模式2，表示遥测通道2信号幅度的均值；

	USHORT	usPdErrAv2;		// 16位无符号整型数，表示目标3鉴相误差幅值均值；对扩频模式2，表示遥测通道3鉴相误差幅值均值；
	USHORT	usAmplAv2;		// 16位无符号整型数，表示目标3信号幅度的均值;对扩频模式2，表示遥测通道3信号幅度的均值；
		
	UINT	uiTestSta;		// 32位无符号整型数，D31~D0位，01H表示正在测试中，02H表示测试完毕。
	
	UINT	uiRsvd;

	//20070814 add
	USHORT	usPdErrAv3;		//16位无符号整型数，对扩频模式2，表示测量通道1鉴相误差幅值均值；
	USHORT	usAmplAv3;		//16位无符号整型数，对扩频模式2，表示测量通道1信号幅度的均值；

	USHORT	usPdErrAv4;		//16位无符号整型数，对扩频模式2，表示测量通道2鉴相误差幅值均值；
	USHORT	usAmplAv4;		//16位无符号整型数，对扩频模式2，表示测量通道2信号幅度的均值；

	USHORT	usPdErrAv5;		//16位无符号整型数，对扩频模式2，表示测量通道3鉴相误差幅值均值；
	USHORT	usAmplAv5;		//16位无符号整型数，对扩频模式2，表示测量通道3信号幅度的均值。


} SSBBERecPIIBwAnsStruct, *PSSBBERecPIIBwAnsStruct;

//20061122 增加下述测试项目
//7.多普勒动态范围调制器部分
typedef struct SSBBEModuDoppRangeAnsBlk
{
	//
	UINT	uiTestItem;		//测试项目，32位无符号整型数，00C5 0403H表示"多普勒动态范围测试"。
	//
	USHORT	usTestCh;		//测试启停控制命令，16位无符号整型数，D15 ~ D0位：00H表示停止测试；01H表示开始测试。
	USHORT	usReserved1;	//
	//
	short	siOffSet;		//调制频偏，16位有符号整型数，单位：KHz
	USHORT	usReserved2;	//
	//
	UINT	uiRsvd[5];		//

} SSBBEModuDoppRangeAnsStruct, *PSSBBEModuDoppRangeAnsStruct;

//8.多普勒动态范围接收机部分
typedef struct	SSBBERecDoppRangeAnsBlk
{
	//
	UINT	uiTestItem;		//测试项目，32位无符号整型数，00C5 0403H表示"多普勒动态范围测试"。
	//
	USHORT	usTestCh;		//测试启停控制命令，16位无符号整型数，D15 ~ D0位：00H表示停止测试；01H表示开始测试。
	short	siOffSet;		//调制频偏，16位有符号整型数，单位：KHz
	//	
	UCHAR	ucLock0;		//目标1锁定，1有效
	UCHAR	ucLock1;		//目标2锁定，1有效
	UCHAR	ucLock2;		//目标3锁定，1有效
    UCHAR	ucReserved;		//
	//
	UINT	uiTestSta;		//32位无符号整型数，D31~D0位，01H表示正在测试中，02H表示测试完毕。
	//
	UINT	uiRsvd[7];		//

} SSBBERecDoppRangeAnsStruct, *PSSBBERecDoppRangeAnsStruct;
//注：调制器只报响应参数，接收机报参数和响应内容

//9. 多普勒一次变化率调制器部分
typedef struct	SSBBEModuDoppRateAnsBlk
{
	//
	UINT	uiTestItem;		//测试项目，32位无符号整型数，00C5 0404H表示"多普勒一次变化率"。
	//
	USHORT	usTestCh;		//测试启停控制命令，16位无符号整型数，D15 ~ D0位：00H表示停止测试；01H表示开始测试。
	USHORT	usReserved;		//
	//
	UINT	uiSimRate;		//多普勒变化率，32位无符号整型数，量化单位：1 Hz/s，取值范围0～15000。
	//
	UINT	uiRsvd[5];		//

} SSBBEModuDoppRateAnsStruct, *PSSBBEModuDoppRateAnsStruct;

//10. 多普勒一次变化率接收机部分
typedef struct	SSBBERecDoppRateAnsBlk
{
	//
	UINT	uiTestItem;		//测试项目，32位无符号整型数，00C5 0404H表示"多普勒一次变化率"。
	//
	USHORT	usTestCh;		//测试启停控制命令，16位无符号整型数，D15 ~ D0位：00H表示停止测试；01H表示开始测试
	USHORT	usPllBw;		//载波环路带宽, 16位无符号整型数。D15 ~ D0位：
							//00H表示5Hz；01H表示10Hz；02H表示100Hz；03H表示500Hz；04H表示1kHz；05H表示2kHz。
	//	
	UCHAR	ucLock0;		//目标1锁定，1有效
	UCHAR	ucLock1;		//目标2锁定，1有效
	UCHAR	ucLock2;		//目标3锁定，1有效
    UCHAR	ucReserved;		//
	
	UINT	uiTestSta;		//32位无符号整型数，D31~D0位，01H表示正在测试中，02H表示测试完毕。
	UINT	uiRsvd[7];		//

} SSBBERecDoppRateAnsStruct, *PSSBBERecDoppRateAnsStruct;
//注：调制器只报响应参数，接收机报参数和响应内容

//11. 遥测解调时延调制器部分
typedef struct	SSBBEModuTMDelayAnsBlk
{	
	//
	UINT	uiTestItem;		//测试项目，32位无符号整型数，00C5 0405H表示"遥测解调时延"。
	//
	USHORT	usTestCh;		//测试启停控制命令，16位无符号整型数，D15 ~ D0位：00H表示停止测试；01H表示开始测试。
	USHORT	usReserved;		//
	//
	USHORT	usConvol;		//卷积编码，0表示不编码，1表示编码。
    USHORT	usG2Inv;		//G2支路反转，0表示不反转，1表示反转。
	//
	UINT	uiRsvd[5];

} SSBBEModuTMDelayAnsStruct, *PSSBBEModuTMDelayAnsStruct;

//12. 遥测解调时延接收机部分
typedef struct	SSBBERecTMDelayAnsBlk
{
	//
	UINT	uiTestItem;		//测试项目，32位无符号整型数，00C5 0405H表示"遥测解调时延"。
	//
	USHORT	usTestCh;		//测试启停控制命令，16位无符号整型数，D15 ~ D0位：00H表示停止测试；01H表示开始测试。
	USHORT	usReserved;		//
	//	
	UCHAR	ucLock0;		//目标1锁定，1有效
	UCHAR	ucLock1;		//目标2锁定，1有效
	UCHAR	ucLock2;		//目标3锁定，1有效
    UCHAR	ucReserved;		//
	
	
    UINT	uiDelay[3];		//目标1\2\3解调时延，单位25us
	UINT	uiTestSta;		//32位无符号整型数，D31~D0位，01H表示正在测试中，02H表示测试完毕。

	UINT	uiRsvd[4];
	
} SSBBERecTMDelayAnsStruct, *PSSBBERecTMDelayAnsStruct;
//注：调制器只报响应参数，接收机报参数和响应内容

//13. 捕获时间测试调制器部分
typedef struct SSBBEModulCaptureTStatusBlk
{
	//
	UINT	uiTestItem;		//测试项目，32位无符号整型数，00C5 0408H表示"捕获时间测试"。
	//
	USHORT	usTestCh;		//测试启停控制命令，16位无符号整型数，D15 ~ D0位：00H表示停止测试；01H表示开始测试。
	USHORT	usReserved;		//
	//	
	UINT	uiRsvd[6];

} SSBBEModulCaptureTStatusStruct, *PSSBBEModulCaptureTStatusStruct;

//14. 捕获时间测试接收机部分
typedef struct SSBBERecCaptureTStatusBlk
{
	//
	UINT	uiTestItem;		//测试项目，32位无符号整型数，00C5 0408H表示"捕获时间测试"。
	//
	USHORT	usTestCh;		//测试启停控制命令，16位无符号整型数，D15 ~ D0位：00H表示停止测试；01H表示开始测试。
	USHORT	usReserved;		//
	//	
	UCHAR	ucLock0;		//目标1锁定，1有效
	UCHAR	ucLock1;		//目标2锁定，1有效
	UCHAR	ucLock2;		//目标3锁定，1有效
    UCHAR	ucReserved;		//
	//	
    UINT	uiAcquTime[3];	//目标1\2\3捕获时间，单位ms
	UINT	uiTestSta;		//32位无符号整型数，D31~D0位，01H表示正在测试中，02H表示测试完毕。
	//
	UINT	uiRsvd;

	UINT	uiAcquTime2[3];	//对扩频模式2，表示测量通道1、2、3捕获时间，单位ms。

} SSBBERecCaptureTStatusStruct, *PSSBBERecCaptureTStatusStruct;
//注：调制器只报响应参数，接收机报参数和响应内容

typedef struct SSBBERecISDRStatusBlk
{
	UINT	uiTestItem;

	USHORT	usTestCh;
	USHORT	usReserved;
	
	BYTE	ucLock0;
	BYTE	ucLock1;
	BYTE	ucLock2;
	BYTE	ucReserved;
	
	UINT	uiSignalAmg[3];
	UINT	uiTestSta;
	UINT	uiSignalAmg1[3];
	UINT	uiRsv[1];

}SSBBERecISDRStatusStruct, *PSSBBERecISDRStatusStruct;

//自动化测试命令
typedef union SSBBEAutoTestCmdBlk
{
	//接收机：0xE638；调制器：0xE640
	//1.AGC控制精度调制器部分
	SSBBEModuAgcCtlCmdStruct	ModuAgcCtlCmd;
	//2.AGC控制精度接收机部分
	SSBBERecAgcCtlCmdStruct		RecAgcCtlCmd;
	//3.AGC时常数调制器部分
	SSBBEModuAgcTCmdStruct		ModuAgcTCmd;
	//4.AGC时常数接收机部分
	SSBBERecAgcTCmdStruct		RecAgcTCmd;
	//5. 等效环路带宽调制器部分
	SSBBEModuPIIBwCmdStruct		ModuPIIBwCmd;
	//6. 等效环路带宽接收机部分
	SSBBERecPIIBwCmdStruct		RecPIIBwCmd;
	//7.多普勒动态范围调制器部分
	SSBBEModuDoppRangeStruct	ModuDoppRangeCmd;
	//8.多普勒动态范围接收机部分
	SSBBERecDoppRangeStruct		RecDoppRangeCmd;
	//9. 多普勒一次变化率调制器部分
	SSBBEModuDoppRateStruct		ModuDoppRateCmd;
	//10. 多普勒一次变化率接收机部分
	SSBBERecDoppRateStruct		RecDoppRateCmd;
	//11. 遥测解调时延调制器部分
	SSBBEModuTMDelayStruct		ModuTMDelayCmd;
	//12. 遥测解调时延接收机部分
	SSBBERecTMDelayStruct		RecTMDelayCmd;
	//13.捕获时间测试调制器部分
	SSBBEModulCaptureTTestStruct ModuAcqTimeCmd;
	//14.捕获时间测试接收机部分
	SSBBERecCaptureTTestStruct RecAcqTimeCmd;
	//15. 中频信号幅度测试调制器部分
	SSBBEModuISDRCmdStruct		ModuISDRCmd;
	//16.中频信号幅度接收机部分
	SSBBERecISDRCmdStruct		RecISDRCmd;

} SSBBEAutoTestCmdStruct, *PSSBBEAutoTestCmdStruct;

//自动化测试状态
typedef union SSBBEAutoTestStatusBlk
{
	//接收机：0xE637；
	//1.AGC控制精度调制器部分
	//SSBBEModuAgcCtlAnsStruct	ModuAgcCtlAns;
	//2.AGC控制精度接收机部分
	SSBBERecAgcCtlAnsStruct		RecAgcCtlAns;
	//3.AGC时常数调制器部分
	//SSBBEModuAgcTAnsStruct		ModuAgcTAns;
	//4.AGC时常数接收机部分
	SSBBERecAgcTAnsStruct		RecAgcTAns;
	//5. 等效环路带宽调制器部分
	//SSBBEModuPIIBwAnsStruct		ModuPIIBwAns;
	//6. 等效环路带宽接收机部分
	SSBBERecPIIBwAnsStruct		RecPIIBwAns;
	//7.多普勒动态范围调制器部分
	//SSBBEModuDoppRangeAnsStruct	ModuDoppRangeAns;
	//8.多普勒动态范围接收机部分
	SSBBERecDoppRangeAnsStruct	RecDoppRangeAns;
	//9. 多普勒一次变化率调制器部分
	//SSBBEModuDoppRateAnsStruct	ModuDoppRateAns;
	//10. 多普勒一次变化率接收机部分
	SSBBERecDoppRateAnsStruct	RecDoppRateAns;
	//11. 遥测解调时延调制器部分
	//SSBBEModuTMDelayAnsStruct		ModuTMDelayAns;
	//12. 遥测解调时延接收机部分
	SSBBERecTMDelayAnsStruct		RecTMDelayAns;
	//13. 捕获时间测试调制器部分
	//SSBBEModulCaptureTStatusStruct	ModuCaptureT;
	//14. 捕获时间测试接收机部分
	SSBBERecCaptureTStatusStruct	RecAcqTimeAns;
	//16.中频信号幅度接收机部分
	SSBBERecISDRStatusStruct		RecISDRAns;

} SSBBEAutoTestStatusStruct, *PSSBBEAutoTestStatusStruct;

//自动化测试调制器状态
typedef union SSBBEAutoTestModulStatusBlk
{
	//调制器：0xE639
	//1.AGC控制精度调制器部分
	SSBBEModuAgcCtlAnsStruct	ModuAgcCtlAns;
	//3.AGC时常数调制器部分
	SSBBEModuAgcTAnsStruct		ModuAgcTAns;
	//5. 等效环路带宽调制器部分
	SSBBEModuPIIBwAnsStruct		ModuPIIBwAns;
	//7.多普勒动态范围调制器部分
	SSBBEModuDoppRangeAnsStruct	ModuDoppRangeAns;
	//9. 多普勒一次变化率调制器部分
	SSBBEModuDoppRateAnsStruct	ModuDoppRateAns;
	//11. 遥测解调时延调制器部分
	SSBBEModuTMDelayAnsStruct		ModuTMDelayAns;
	//13. 捕获时间测试调制器部分
	SSBBEModulCaptureTStatusStruct	ModuCaptureT;

} SSBBEAutoTestModulStatusStruct, *PSSBBEAutoTestModulStatusStruct;

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
/*----------------------------------------------------------------------*/
/*---------------------BPSK体制 结构体  ----------------------------------*/
/*----------------------------------------------------------------------*/


//************************ 设置结构 *****************************************//
//分集合成设置结构
typedef struct DDTBBEDivParaBlk 
{
	//0xE608
	USHORT	usWorkCtrl;			// 工作控制，16位无符号整型数。为1表示设置模块停止工作，为2表示设置模块开始工作。缺省值为2，界面不显示。
    USHORT  usDivCtrl;			// 16位无符号整型数,最低位为1表示手动，0表示自动。高15位保留置零。缺省值为0。当合成方式为3和4时，合成控制值为1。
	//
    USHORT  usDivType;			// 16位无符号整型数,为0表示最大比合成方式，为1表示等增益合成方式，为2表示选择合成方式，为3表示左旋单路，为4表示右旋单路，其它无定义。缺省值为3。
    USHORT  usDiffBn;			// 16位无符号整型数,单位Hz，设置值可为75、150、300、600、1200、2400和4800。缺省值为600。
	//
    USHORT  usADivWeight;		// A路（左旋）信号合成权重，16位无符号整型数。取值范围0～1024。缺省值512。
	USHORT  usBDivWeight;		// B路（右旋）信号合成权重，16位无符号整型数。取值范围0～1024。缺省值512。
									// 当合成控制设置为手动方式时，接收机根据此项设置进行分集合成；
									// 当合成控制设置为自动方式时，接收机将忽略此项设置，根据实际接收情况自动调整左旋和右旋合成权重。
									// 当合成方式为3时，ADivWeight为1024，BDivWeight为0；
									// 当合成方式为4时，ADivWeight为0，BDivWeight为1024。

	//
    USHORT  usAmEn;				// 16位无符号整型数,为1表示AM合成使能，0表示AM合成禁止。缺省值为1，不显示。
	UCHAR   ucDivHorO;			// 差模环锁定门限选择，1为手动，0为自动，不显示
	UCHAR	ucDivMen;			// 差模环锁定门限,缺省值0x1c(28)，不显示
}DDTBBEDivParaSet,*PDDTBBEDivParaSet;

//角误差解调设置结构
typedef struct DDTBBEAngERRParaBlk 
{
	//0xE610
	USHORT	usWorkCtrl;			// 工作控制，16位无符号整型数。为1表示设置模块停止工作，为2表示设置模块开始工作。缺省值为2。
	USHORT  usPhaseAmend;		// 相位修正（本地1KHz方波与解调出的1KHz方波的相位差），16位无符号整型数，单位0.1 ，取值范围：0~3600。缺省值为0。
    //
	SHORT	sAzimuAmend;		// 方位修正，16位有符号整型数。设置到硬件的值需要进行转换，转换公式为：AzimuAmend = 需修正的电压值 × 16384/5。缺省设置值为0。
	SHORT	sElevaAmend;		// 俯仰修正，16位有符号整型数。设置到硬件的值需要进行转换，转换方法同方位零值修正。缺省值为0。
	//
    ULONG   ulAzimuSlope;		// 32位无符号整型数，无量纲，取值范围：1 ~ 1600。 其中：要求斜率指的是伺服要求的斜率；实际斜率为实测的斜率。方位斜率（AzimuthSlope）的缺省设置值为100。
	//
    ULONG   ulElevaSlope;		// 32位无符号整型数，无量纲，取值范围：1 ~ 1600。计算公式同方位斜率。缺省设置值为100。
	//
}DDTBBEAngERRParaSet,*PDDTBBEAngERRParaSet;
		
//载波恢复模块设置结构
typedef struct DDTBBECARRParaBlk 
{
	//0xE612
	USHORT	usWorkCtrl;			// 工作控制，16位无符号整型数。为1表示设置模块停止工作，为2表示设置模块开始工作。缺省值为2。
	USHORT  usCarrCatch;		// 共模锁定门限，16位无符号整型数。默认值： 载波环路带宽〉1000时为1500， 载波环路带宽〈1000时为500
	//
	ULONG   ulCarrFreq;			// 载波频率，32位无符号整型数.单位Hz ，设置范围68000000-72000000，缺省值为70000000  
	//
	USHORT  usCarrFreqCapRge;	// 载波频率捕获范围 16位无符号整型数. 单位KHz,隐含正负号，设置范围0-1500，缺省值为150
	UCHAR   ucFftEn;			// 不显示 载波FFT辅助捕获使能，8位无符号整型数.为1表示允许FFT辅助载波频率捕获，为 0表示禁止FFT辅助。缺省值为1。
	UCHAR   ucCarrCloseLpEn;	// 不显示 载波闭环使能，为1表示允许载波闭环，为0表示禁止载波闭环。缺省值为1。
	//
	USHORT  usCarrBn;			// 载波环路带宽, 16位无符号整型数。单位Hz,设置值为100，500，2000，10000。缺省值为1000。
	UCHAR	ucReCapEn;			// 不显示 载波重捕使能，8位无符号整型数。为1表示设置载波自动重捕，为0表示设置载波手动重捕。此位每设置一次0，则接收机载波重捕一次。此设置项缺省值为1。
	UCHAR	ucCarrHorO;			// 保留 共模锁定门限设置 手动1，自动0
	//
	UCHAR   ucAGCCtlMode;		// 不显示 AGC控制方式，1为手动，0为自动。缺省值为0。
	UCHAR	InputSel;			// 不显示 输入选择，8位无符号整型。低4位为左旋输入口选择，高4位为右旋输入口选择。0表示端口1，1表示端口2，2表示端口3，3表示端口4。
	USHORT  usAgcTime;			// AGC时间常数，单位ms，根据任务要求，可为3，10、100和1000。缺省值为1000。
	//
    USHORT  usLAGCManulData;	// 不显示 左旋手动控制量,16位无符号整型数。设置范围从51 ~ 640，代表"最大增益"至"最小增益".缺省值为120。
    USHORT  usRAGCManulData;	// 不显示 右旋手动控制量,16位无符号整型数。设置范围从51 ~ 640，代表"最大增益"至"最小增益".缺省值为120。
	//
	UCHAR	ucModuType;			// 固定填0（调制体制，16位无符号整型数。0x0--PCM-BPSK ,0x01--PCM-QPSK ,0x02--PCM-SQPSK ,0x03--PCM-UQPSK，缺省值为1）
	UCHAR	ucGrayType;			// 保留 将Gray码视为QPSK调制体制的一项内容，只在QPSK体制有效。Gray编码选择：8位无符号整型数。D7 ~ D0位：10H表示无Gray编码；00H表示选择Gray0编码；
								// 01H表示选择Gray1编码；02H表示选择Gray2编码；03H表示选择Gray3编码；04H表示选择Gray4编码；
								// 05H表示选择Gray5编码；06H表示选择Gray6编码；07H表示选择Gray7编码，缺省值为10H
	UCHAR   ucIPowRate;			// 保留 I路功率因子，范围从1 ~ 10
    UCHAR   ucQPowRate;			// 保留 Q路功率因子，范围从1 ~ 10
								// I、Q两路功率比，低8位为I路功率因子，高8位为Q路功率因子。
								// 要求（I路功率因子/Q路功率因子）为最简分数的形式。缺省值为 1：1。
	//
    UCHAR	usPolarSel;			// 不显示 极性选择，隐含设置，不出现在交付用户界面中。此项值由对接实验确定，分别对"0"和"1"测试，当系统能正常工作时，此时的PolarSel（极性选择）就确定下来，作为固定设置，不可更改。缺省值为"1"。
	UCHAR	usOutputSel;		// 固定填2（8位无符号整型数，选择接收机解调数据的输出方式  0---I/Q合路  ，1----I/Q独立，2---I路单独；缺省设置值为0）。
	USHORT	usLoadCnt;          // 加载计数
	//
  	ULONG   ulIFBn;				// 中频带宽，单位Hz。设置值可为750kHz，1MHz，1.5MHz，2.4MHz，5MHz，10MHz，16MHz，20MHz。
	//
	UCHAR  ucPowerType;         // 保留 I/Q功率比设置方式，8位无符号整型数，00H表示分贝方式；01H表示功率比值方式。
	UCHAR  ucReserved;     
	short  RatiodB;             // 保留 I/Q功率分贝比，16位有符号整型数，量化单位：0.1 dB，取值范围：-10~10dB，缺省值为6dB。
								// I/Q功率比设置方式为分贝时有效。
}DDTBBECARRParaSet,*PDDTBBECARRParaSet;

//码同步设置结构
typedef struct DDTBBECodeSynBlk
{
	//0xE614
	USHORT  usWorkCtrl;			// 工作控制，16位无符号整型数。为1表示设置模块停止工作，为2表示设置模块开始工作。缺省值为2。
	USHORT  usCodeBn;			// 码环带宽，0表示1%，1表示0.5%，2表示0.1%。缺省值为0。
	//
	ULONG   ulCodeRate;			// 码率，32位无符号整型数。单位bps，范围从5kbps～2Mbps，每bps步进，缺省值为10000000。
	//
	UCHAR   ucBitOutEn;			// 保留 位流传输使能（En），1表示使用位流传输模式，0--不输出，1--输出I路，2--输出Q路。
	UCHAR   ucClkSel;			// 保留 位流输出时钟选择，1表示位流传输使用外时钟，0表示位流传输使用内时钟。缺省值为0。
	USHORT  usFIFOLen;          // 保留 传输FIFO的初始缓冲字节数，范围从0到65535，缺省值为32768。
	//
	USHORT	usCodeType;			// 0表示NRZ-L，1表示NRZ-M，2表示NRZ-S，3表示BIP-L，4表示BIP-M(93标准)，5表示BIP-S(93标准)，
								// 8表示RNRZ-L(15位)
	UCHAR	ucViterbi;			// 保留 8位无符号整型数， 0表示不译码，1表示方式一（（7，1/2），G2正相），2表示方式二（（7，1/2），G2反相）。缺省值为0。
	UCHAR	usHRSel;            // 保留 8位无符号整型数， 1为硬判决，0为软判决。缺省值为0。
	UCHAR   ucInputSel;         // 不显示 输入选择 0：中频输入 1：带时码模飞 默认设置为0
	UCHAR   ucReserved[5];      // 保留
	USHORT	usLoadCnt;          // 加载计数
   //
}DDTBBECodeSynParaSet,*PDDTBBECodeSynParaSet;

//帧同步器参数块结构
typedef struct DDTBBEFrameSynBlk
{ 
	//0xE616
	USHORT	usWorkCtrl;			// 工作控制，16位无符号整型数。为1表示设置模块停止工作，为2表示设置模块开始工作。缺省值为2。
	UCHAR	ucRevEn;			// 不显示 逆序帧同步使能，8位无符号整型数。为1表示进行逆序帧同步使能，0表示进行逆序帧同步禁止。缺省值为0。
	UCHAR	ucFrmPlace;			// 不显示 帧同步码组位置，0为帧首，1为帧尾，默认为1
    //
	USHORT	usFrameLen;			// 帧长，单位为字（ucWordLen），范围从4到4096，缺省值为512。
	UCHAR	ucWordLen;			// 字长，8位无符号整型数。单位bit，范围8-16，缺省值为8。
	UCHAR	ucSynCodeLen;		// 帧同步码组长，单位为位（bit），范围从8到32位。帧同步码组长必须为字长的整数倍。缺省值为32。
    //
	DWORDLONG  dwlFrmSyncCode;  //帧同步码组，低字节在前，高字节在后，码组取值向最低位靠齐。
								// 如果帧同步码组长度不足64位，高位应补零。缺省值为1ACFFC1D（16进制）
	//
	UCHAR	ucVerifyCheckFrame;	// 校核帧数，范围1-7，缺省值为3。
	UCHAR	ucLockCheckFrame;	// 锁定帧数，范围1-7，缺省值为3。
	UCHAR	ucReserved1[2];
	//
	UCHAR	ucSearchErrBits;	// 搜索容错位数， 范围0-7，缺省值为0。
	UCHAR	ucVerifyErrBits;	// 校核容错位数， 范围0-7，缺省值为0。
	UCHAR	ucLockErrBits;		// 锁定容错位数， 范围0-7，缺省值为0。
    UCHAR	ucReverved2;				
	//
	UCHAR	ucRandEn;			// 保留 解扰使能，8位无符号整型数。为1表示解扰使能，为0表示解扰禁止。
	UCHAR	ucReserved3;		
	USHORT	usRandPoly;			// 保留 解扰多项式，16位无符号整型数。每一位为1表示此阶有效，为0表示此阶无效。
	//
	USHORT	usPhase;			// 保留 解扰移位寄存器初相，16位无符号整型数。
	USHORT	usRandLength;		// 保留 解扰长度16位无符号数，5~20
	//
	UCHAR	ucSecretEn;			// 保留 解密使能，8位无符号整型数。为1表示解密使能，为0表示解密禁止。缺省为0
	UCHAR	OrderSel;			// 保留 解密解扰顺序选择，8位无符号整型数。当解扰使能和解密使能均有效时才有效。
								// 为0表示先解扰再解密，为1表示先解密再解扰。缺省为0。
	UCHAR	ucReserved4[2];
	//
	UCHAR	ucRSDecodeEn;		// 保留 RS译码使能，8位无符号整型数。为0表示Rs译码禁止，为1表示无虚拟填充RS译码使能，为2表示虚拟填充RS译码使能。
	UCHAR	ucInterleaver;		// 保留 交织深度，8位无符号整型数。当RS译码使能时有效，可选的值为1，2，3，4，5。缺省为1。
	USHORT	usLoadCnt;          // 加载计数
	UCHAR	ucReserved5[24];

}DDTBBEFrameSynParaSet,*PDDTBBEFrameSynParaSet;

//副帧同步器参数块结构
typedef struct DDTBBESubFrameSynBlk
{ 
	//0xE618
	USHORT	usWorkCtrl;			// 工作控制，16位无符号整型数。为1表示设置模块停止工作，为2表示设置模块开始工作。缺省值为2。
	UCHAR	ucSubFrmType;		// 副帧方式，8位无符号整型数。
								// 00H表示无副帧；01H表示ID副帧；02H表示循环副帧；03H表示反码副帧。缺省值为无副帧。
	UCHAR	ucReserved;		
	//
	USHORT	usSubFrmLen;		// 副帧长度，16位无符号整型数，单位：帧，取值范围：4 ~ 256 帧。
	USHORT	usIDSubFrmPos;		// ID副帧ID字位置，单位：字；取值范围：1 ~ 4096。缺省值为1。
	//
	UCHAR	ucIdBase;			// ID字基值，取值范围：0或1；缺省值为0
	UCHAR	ucIdDirect;			// ID计数方向，0：加计数，1：减计数。缺省值为0
	UCHAR	ucReserved1[2];		// 保留2个字节
	//
	/*
    ULONG   ulCycFrmSynCodeL;	// 循环副帧同步码组低32位，32位无符号整型数。低字节在前，高字节在后，码组取值向最低位靠齐。
								// 如果循环副帧同步码组长度不足32位，高位应补零。缺省值为1ACFFC1D（16进制）。
    ULONG   ulCycFrmSynCodeH;	// 循环副帧同步码组高32位，32位无符号整型数。低字节在前，高字节在后，码组取值向最低位靠齐。
								// 如果循环副帧同步码组长度不足64位，高位应补零。缺省值为00000000（16进制）。
	*/
	DWORDLONG  dwlCycFrmSyncCode;  //循环副帧同步码组,低字节在前，高字节在后，码组取值向最低位靠齐。
									// 如果帧同步码组长度不足64位，高位应补零。缺省值为1ACFFC1D（16进制）
	//
	USHORT	usCycFrmSynCodePos;	// 循环副帧码组位置,16位无符号整型数，量化单位：1 字，取值范围：1 ~ 4096 字
	UCHAR	Reserved2;			// 保留1个字节
	UCHAR	ucCycFrmSynCodeLen;	// 循环副帧码组长度,8位无符号整型数，量化单位：1 位，取值范围：8 ~ 64 位，缺省值为32。
	//
	UCHAR	ucCFSearchErrBits;	// 循环副帧搜索容错位数，取值范围：0 ~ 15 位；
	UCHAR	ucCFVerifyErrBits;	// 循环副帧校核容错位数，取值范围：0 ~ 15 位；
	UCHAR	ucCFLockErrBits;	// 循环副帧锁定容错位数，取值范围：0 ~ 15 位。
	UCHAR	Reserved3;			// 保留1个字节
	//
	UCHAR	ucSubFVerifyFrame;	// 副帧三态逻辑：校核检测帧数，取值范围：1 ~ 8 副帧；
	UCHAR	ucSubFLockFrame;	// 副帧三态逻辑：锁定检测帧数，取值范围：1 ~ 8 副帧。
	USHORT	usLoadCnt;          // 加载计数
}DDTBBESubFrameSynParaSet,*PDDTBBESubFrameSynParaSet;

// 引导跟踪解调参数设置
typedef struct BBELeadTrackSetBlk
{
	//0xE554
	UCHAR	ucSwitch;			//工作开关，8位无符号整型数。D7 ~ D0位：00H表示关；01H表示开。	
	UCHAR	ucMode;				//工作模式,8位无符号整型数。D7 ~ D0位：00H表示正常工作；01H表示自动校相
	USHORT	usSensitivity;		//检测灵敏度,16位无符号整型数，量化单位：1 mV/mil。缺省值为100mV/mil。
	//
	SHORT	sAzimuPoint;		//天线方位指向,16位有符号整型数，量化单位：0.01 mil。取值范围：-10mil ~ 10mil 。
	SHORT	sElevaPoint;		//天线俯仰指向,16位有符号整型数，量化单位：0.01 mil。取值范围：-10mil ~ 10mil 。
	//
	SHORT	sAzimuZero;			//方位零值修正,16位有符号整型数，量化单位：10/2^15 V。
	SHORT	sElevaZero;			//俯仰零值修正,16位有符号整型数，量化单位：10/2^15 V。
	//
	int		iAzimuSlope;		//方位斜率,32位有符号整型数。取值范围：-100000 ~ 100000。
	//
	int		iElevaSlope;		//俯仰斜率,32位有符号整型数。取值范围：-100000 ~ 100000。
	//
	USHORT	usMeasNum;			//测量点数,16位无符号整型数，表示校相时每个相位的方位、俯仰电压测量2^MeasNum次。取平均值上报
								//上报的测量数据是2^MeasNum个测量数据的平均值。
	UCHAR   ucReserved[2];
	//
	USHORT	usRefFreq;			//基准信号频率，16位无符号整型数，量化单位：1kHz，缺省值为25Hz。
	USHORT	usRefDelay;			//时延修正，16位有符号整型数，表示输入基准与本地基准的时延修正量，时延修正量0~3600,设置值为0~(2^16-1)。
	//
	ULONG   ulReserved[4];

} BBELeadTrackSetStruct, *PBBELeadTrackSetStruct;

///////////调制参数设置/////////////////////////
//  上行调制载波参数设置
typedef struct DDTBBEModuParaBlk
{ 
	//0xE620
    UCHAR   ucMode;			// 工作方式，8位无符号整型数。   
							// D7 ~ D0位：00H表示任务方式（调制器无信号输出）；01H表示自检方式（调制器输出中频信号）。
	UCHAR   ucUpConv;		// 上变频方式，8位无符号整型数。。D7 ~ D0位：00H表示Icos+Qsin；01H表示Icos-Qsin；02H表示Isin+Qcos；03H表示Isin-Qcos。缺省为1。
	                        // 不显示，具体值调试后确定
	USHORT  usCarIFsel;		// 输出载波中频信号的频率选择，16位无符号整型数。
							// D7~D0位：00H表示输出中频70MHz；01H表示输出中频140Hz信号。
	                        // 不显示，默认为00H
    //
    UINT    uiCarIF;		// 输出载波中频，32位无符号整型数，量化单位：1 Hz，取值范围根据ucCarIFsel设置而定，
							// 70 MHz中频时输出范围为69 M ~ 71 MHz，缺省为70MHz。
	//
 	UCHAR   ucReserved1;
	char    cLevel;			// 输出功率，8位有符号整型数，量化单位：1 dBm，取值范围：－60 ~ 0 dBm，缺省为－30 dBm。
	UCHAR   ucOnline;		// 加载/去载，8位无符号整型数。D7 ~ D0位：00H表示去载；01H表示加载。（控制中频信道的开关）
	UCHAR   ucModu;			// 加调/去调，8位无符号整型数。D7 ~ D0位：00H表示去调；01H表示加调
 	//
	short   sOffset;		// 上行多普勒预置，16位有符号整型数。量化单位：1 kHz，取值范围：－150 ~ 150 kHz，缺省值为0 kHz。	
	UCHAR   ucAmModu;		// 保留（调幅波模式选择, 00H表示关调幅；01H表示开。缺省值为关 ）
	UCHAR   ucAmDepth;		// 保留（调幅波的调制深度，扩大10倍后送给DSP)
	//
	UINT     uiAmFreq;		// 保留(调幅波频率，32位有符号整型数。)
	//
	UCHAR   ucNoiseSw;		// 噪声开关，8位无符号整型数，仅在自检工作方式时有效。00H表示关；01H表示开。缺省值为关。
	UCHAR   ucReserved2;
	USHORT  usC_N0;			// 输出载噪比，8位无符号整型数，量化单位：0.1 dBHz，取值范围：30 ~ 80 dBHz。
	//
	UCHAR   ucSimSw;		// 多普勒动态模拟开关，8位无符号整型数。 00H表示关；01H表示开。缺省值为关。
	UCHAR   ucReserved3;
	USHORT  usSimRange;		// 多普勒变化范围，16位无符号整型数，量化单位：1 kHz，取值范围：0 ~ 150 kHz，缺省为150 kHz。
	//
	USHORT  usSimRate;		// 多普勒变化率，16位无符号整型数，量化单位：1 Hz/s，取值范围：0 ~ 5 kHz/s，缺省为0 Hz/s。
	USHORT  usSimAcc;		// 多普勒加加速度，16位无符号整型数，量化单位：1 Hz/s2，取值范围：0 ~ 100Hz/s2，缺省为0 Hz/s2，不显示，默认为00H
	//
	UCHAR   ucInputSource;  // 调制信号来源选择，8位无符号整型数。
								// 00H表示调制信号来源于总线上的模飞数据；
								// 01H表示调制信号来源于DSP产生的自检信号；
								// 02H表示调制信号来源于CPCI的J3端口的信号；
								// 03H表示调制信号来源于CPCI的J4端口的信号；
								// 04H表示调制信号来源于基带通道的A/D采样后的信号；
								// 05H表示FPGA内部产生单频信号
	                        
	UCHAR   ucModesel;      // 输出调制体制选择，8位无符号整型数，固定填0
								// 00H表示调制体制为PCM-BPSK；
								// 01H 表示信号通道1的调制体制为PCM-QPSK；
								// 02H 表示信号通道1的调制体制为PCM-OQPSK；
								// 03H 表示信号通道1的调制体制为PCM-UQPSK；
								// 04H 表示输出AM信号   界面不设。
	UCHAR  ucSignalSel;     // 调制信号模式选择，8位无符号整型数。
	                        // 0表示I/Q合路，1表示I/Q独立，2表示I路单独 固定填2
	UCHAR  ucGrayType;		//保留 QPSK调制Gray编码选择：8位无符号整型数。D7 ~ D0位：10H表示无Gray编码；00H表示选择Gray0编码；
							//01H表示选择Gray1编码；02H表示选择Gray2编码；03H表示选择Gray3编码；04H表示选择Gray4编码；
							//05H表示选择Gray5编码；06H表示选择Gray6编码；07H表示选择Gray7编码；
	//
	UCHAR  ucIPowRate;      //保留  I路功率因子  
	UCHAR  ucQPowRate;      //保留  Q路功率因子  
	UCHAR  ucReserved4[2];
	//
	UCHAR  ucPowerType;     //保留 I/Q功率比设置方式，8位无符号整型数，00H表示分贝方式；01H表示功率比值方式。
	UCHAR  ucReserved5;
	short  RatiodB;         //保留 I/Q功率分贝比，16位有符号整型数，量化单位：0.1 dB，取值范围：-20~20dB，缺省值为6dB。
	                        //I/Q功率比设置方式为分贝时有效。
    //
	UINT   uiReserved5[4];
} DDTBBEModuParaSet, *PDDTBBEModuParaSet;

////////模拟源参数设置///////////////////////////
typedef struct
{   
	//0xE622
	UCHAR   ucReserved[2];		// 保留2个字节
	UCHAR   ucCodeType;		    // 码型设置,PCM码型选择：8位无符号整型数。D7 ~ D0位：00H表示调制码型为 NRZ_L ；
								// 01H表示调制码型为 NRZ_M ；02H表示调制码型为 NRZ_S ；03H表示调制码型为 BIP_L ；
								// 04H表示调制码型为 BIP_M ；05H表示调制码型为 BIP_S ；08H表示RNRZ-L(15位)
	UCHAR   ucReserved1;		
	//
	ULONG   ulCodeRate;		    // 码速率，32位无符号整型数，量化单位：1 bps，取值范围：5K bps ~ 2Mbps。
	//
	UCHAR   ucReserved2[4];		// 保留4个字节
	//
	UCHAR   ucConvol;			// 不显示 卷积编码，8位无符号整型数。00H表示否；01H表示是，默认为0。
	UCHAR   ucG2Inv;			// 不显示 卷积编码G2相位反转，8位无符号整型数。D7 ~ D0位：00H表示否；01H表示是，默认为0。
	UCHAR   ucG1G2Sw;			// 不显示 卷积编码g1g2互换，8位无符号整型数。D7 ~ D0位：00H表示正常输出；01H表示g1g2互换输出，默认为0。
	UCHAR   ucReserved3;		// 保留1个字节
	
	//
	USHORT  ucRandom;			// 不显示 加扰，8位无符号整型数。00H表示否；01H表示是。加扰开关只有在RS编码开关打开时设置才有效，默认为0。
	USHORT  ucRandPoly;		    // 保留 加扰多项式，16位无符号整型数。每一位为1表示此阶有效，为0表示此阶无效。
	//
	USHORT  usPhase;            // 保留 加扰移位寄存器初相，16位无符号整型数。	
	USHORT	usRandLength;		// 保留 加扰长度 16位无符号数，5~20
	//
	UCHAR   ucRS;				// 不显示 RS编码选择，8位无符号整型数。D7 ~ D0位：00H表示否；01H表示是。缺省为0
	UCHAR   ucInterleaver;	    // 不显示 交织深度，8位无符号整型数。当RS编码使能时有效，可选的值为1，2，3，4，5和8。缺省为1。
	UCHAR   ucViff;             // 不显示 虚拟填充开关，8位无符号整型数。只有在RS编码使能下才起作用0：无虚拟填充，1：有虚拟填充。缺省为0
	UCHAR   ucReserved5;
	//
	UCHAR   ucDecrypt;		    // 不显示 加密，8位无符号整型数。D7 ~ D0位：00H表示否；01H表示是，默认为0。
	UCHAR   ucKey;			    // 不显示 密钥选择，8位无符号整型数。
	UCHAR   ucReserved6[2];		// 保留2个字节
	//
    ULONG   ulReserved7;
	//
	UCHAR   ucWordLen;			//字长，单位bit,范围8-16
	UCHAR   ucFspLen;			//帧同步码组长，单位为1位（bit），范围从8到32位。帧同步码组长必须为字长的整数倍。
	USHORT  usFrmLen;			//帧长，单位为字（ucWordLen），范围从2到4096，缺省值为512。
	//
	DWORDLONG dwlFsp;			//帧同步码组，共8字节，总长64位，低字节在前，高字节在后，
								//码组取值向最低位靠齐。如果帧同步码组长度不足64位，高位应补零。
								//缺省值为1ACFFC1D（16进制）。
	//
	UCHAR   ucFsp_place;		//不显示 子帧同步码组的位置，D7~D0位，00H表示在帧首，01H表示在帧尾，缺省值为在帧尾。
	UCHAR   ucReserved4[3];		//保留3个字节
	//
	UCHAR   ucReserved8;		//保留1个字节
	UCHAR   ucSubFrmType;		//副帧方式，8位无符号整型数。D7 ~ D0位：00H表示无副帧；01H表示ID副帧；02H表示循环副帧；03H表示反码副帧。缺省值为无副帧。
	USHORT  usSubFrmLen;		//副帧长度，16位无符号整型数，单位：帧，取值范围：4 ~ 256 帧。
	//
	USHORT  usIdPlace;			//ID副帧ID字位置，单位：字；取值范围：1 ~ 4095；
	UCHAR   ucIdBase;			//ID字基值，取值范围：0或1；
	UCHAR   ucIdDirec;			//ID计数方向，0：加计数，1：减计数。
	//
	DWORDLONG   dwlCycFsp;		//循环副帧码组,64位无符号整型数。
	//
	USHORT  usCycFspPlace;		//循环副帧码组位置,16位无符号整型数，量化单位：1 字，取值范围：1 ~ 1024 字
	UCHAR   ucCycFspLen;		//循环副帧码组长度,8位无符号整型数，量化单位：1 位，取值范围：8 ~ 64 位
	UCHAR   ucReserved7;
	//
	UINT   uldata;				//固定码数据，32位无符号整型数
	//
	UINT   ucdatasel;			//固定码数据与随机数据选择，0：选择固定码数据，1：选择随机数据
	//
	UINT   uiReserved9[3];		

} DDTBBBEEmulatorSetStruct, *PDDTBBBEEmulatorSetStruct;

//加/解密工作参数设置块结构
typedef struct  
{
	//0x1304解密参数 0x1302加密参数
	UCHAR	Switch;						// 工作开关，8位无符号整型数。D7 ~ D0位：00H表示关；01H表示开。
	UCHAR	Reserved1[3];				// 保留3字节
	USHORT	UsEncryptPlaceLen;			// 未加密字段子帧位置有效字节的长度。
										// 16位无符号整型数，定义UnEncryptPlace中子帧波道有效字节的长度。
										// 取值范围0～1024。
	USHORT	UsSubFrameEncryptLen;		// 未加密字段副帧位置有效字节的长度。
										// 16位无符号整型数，定义UnEncryptPlace中副帧波道有效字节的长度。
										// 取值范围0～1024。
	
	UINT	UnEncryptPlace[1024];  
	
										// 未加密字段位置，32位无符号整型数，共1024个。
										// 以字为最小单位设置未加密字段，允许未加密字段的最大个数为1024字（不含子帧同步码组）。子帧同步头默认不加密。
										// 首先放置子帧位置，然后放置副帧位置。子帧位置的长度在UsFrameEncryptLen中指明，副帧位置的长度在UsSubFrameEncryptLen中指明。总长度不超过1024个32位无符号数。当数据为0时，表示未定义数据。
										// 子帧位置表示未加密的子帧波道号，即在一个全帧中该波道数据都是未加密数据，值域范围[1～8192]。
										// 副帧位置表示未加密数据在全帧中的位置。
	UCHAR	UcKeyPlaceLen;				// 密钥位置有效字节的长度。
										// 8位无符号整型数，定义KeyPlace中有效字节的长度。取值范围1～32。
	UCHAR	Reserved2[3];				// 保留3字节
	
	UINT	KeyPlace[32];				// 密钥位置，32位无符号整型数，共32个。
										// 以字为最小单位设置密钥位置，密钥位置总长为128bit，最大为32字。
	UCHAR	ucStateIdentifier;			// 状态标识符，8位无符号整型数。D7~D0位：00H表示状态1；01H表示状态2；02H表示状态3
	

	UCHAR	WordLength;					// 字长 
	UCHAR	LengthOfFrameSynCode;		// 子帧码组长
	UCHAR   LengthOfSubFrameSynCode;	// 循环副帧同步码组长      
	
	USHORT	LocationOfIdInFrame;		// ID副帧位置/循环副帧位置
	USHORT	FrameLength;				// 子帧长           	
	USHORT  SubframeLength;				// 副帧长 
	UCHAR   SubType;					// 副帧类型
										// 00H表示无副帧；01H表示ID副帧；
										// 02H表示循环副帧；03H表示反码副帧
	UCHAR	Reserved3;					// 保留个8位字
	UINT	Reserved4[29];				// 保留29个32位字

}EncryptParaStruct,*LPEncryptParaStruct;

//测试信号输出设置结构
typedef struct DDTBBETestOutSetBlk
{
	//接收机：0x115A；调制器：0x115C
	UCHAR	D1Out;				//数字检测信号输出选择，
								//8位无符号整型数。定义见"检测输出定义表"。
	UCHAR	Reserved1[3];		//保留3个字节
	//
	UCHAR	Reserved[24];		//保留24个字节

} DDTBBETestOutSetStruct, *PDDTBBETestOutSetStruct;

//************************ 状态结构 *****************************************//
//分集合成状态结构
typedef struct DDTBBEDivStatusBlk 
{
	//0xE607
	DDTBBEDivParaSet  DivPara;   //设置参数返回  
	//
	USHORT   usAgcVolt_A;		//16位无符号整型数，代表分集信道A的AGC电压，单位毫伏（mV），范围从0到65535。
								////AGC电压为dB线性，为0时代表输入中频信号功率为-70dBm，为5000mV时代表输入中频信号功率为+10dBm。
	USHORT   usAgcVolt_B;		//16位无符号整型数，代表分集信道B的AGC电压（AgcVoltB），单位毫伏（mV），范围从0到65535。
								////内容和含义同分集信道A的AGC电压（AgcVoltA）。
    //
	USHORT   usAgcVolt_S;		//16位无符号整型数，代表分集合成后的AGC电压（AgcVoltS），单位毫伏（mV），范围从0到65535。
								////内容和含义同分集信道A的AGC电压。
	UCHAR    ucReserved1[2];
	//
    ULONG    ulEb_A;			//32位无符号整型数，分集信道A的信号幅度。
    ULONG    ulN0_A;			//32位无符号整型数，分集信道A的噪声幅度。
    ULONG    ulEb_B;			//32位无符号整型数，分集信道A的信号幅度。
    ULONG    ulN0_B;			//32位无符号整型数，分集信道A的噪声幅度。
    ULONG    ulEb_S;			//32位无符号整型数，合成后的信号幅度。
    ULONG    ulN0_S;			//32位无符号整型数，合成后的噪声幅度。
	//
	USHORT   usSynFatorA;		//16位无符号整型数，指示了接收机分集信道A的实时合成系数。
	USHORT   usSynFatorB;		//16位无符号整型数，指示了接收机分集信道B的实时合成系数。
	//
    USHORT   usComLk;			//16位无符号整型数，为1表示共模环锁定，为0表示共模环失锁。
	USHORT   usDiffLk;			//16位无符号整型数，为1表示差模环锁定，为0表示差模环失锁。
}DDTBBEDivStatus,*PDDTBBEDivStatus;

//角误差解调设置结构
typedef struct DDTBBEAngERRStatusBlk 
{
    //0xE609
    DDTBBEAngERRParaSet   AngERRPara;    //设置返回参数
    //
    USHORT  usAngLk;			// 16位无符号整型数，为1表示角误差解调锁定，为0表示角误差解调失锁。
    SHORT	sAzimVolt;			// 16位有符号整型数，表示方位误差电压的实时测量值，单位unit。
    //
    SHORT   sElevVolt;			// 16位有符号整型数，表示俯仰误差电压的实时测量值，单位unit。
    UCHAR   ucReverved[2];
    //
}DDTBBEAngERRStatus,*PDDTBBEAngERRStatus;

//载波恢复模块状态结构
typedef struct DDTBBECARRStatusBlk 
{
	//0xE611
	DDTBBECARRParaSet   CARRPara;  //工作参数返回
	//
	int    iCarrDoppFreq;   //32位有符号整型数，单位0.01Hz。
	                        //例如当CarrDoppFreq等于-10000时，对应载波多普勒频率为0.01*（-10000）Hz，即-100Hz。
	//
	USHORT usCarrLk;        //16位无符号整型数，为1表示载波锁定，为0表示载波失锁。
	USHORT usCarrCapTime;   //载波捕获时间 16位无符号整型数，单位为ms，范围从0到65535。
	//
	USHORT usAgcVolt1;      //通道1（CH1）的AGC电压，32位无符号整型数。单位毫伏（mV），范围从0到65535。
							////各通道AGC电压为dB线性，为0时代表输入中频信号功率为-70dBm，为5000mV时代表输入中频信号功率为+10dBm。
    USHORT usAgcVolt2;      //通道2（CH2）的AGC电压，32位无符号整型数。单位毫伏（mV），范围从0到65535。含义同usAgcVolt1。
	//
	USHORT usAgcVolt3;      //通道3（CH3）的AGC电压，32位无符号整型数。单位毫伏（mV），范围从0到65535。含义同usAgcVolt1。
	USHORT usAgcVolt4;      //通道4（CH4）的AGC电压，32位无符号整型数。单位毫伏（mV），范围从0到65535。含义同usAgcVolt1。
	//
	ULONG  ulPdErrAv;       //鉴相误差幅度，32位无符号整型数，单位为"弧度"。
	ULONG  ulSignAv;        //信号幅度，32位无符号整型数，单位为"unit"。
	ULONG  ulAgcVpp;        //AGC电压峰峰值，32位无符号整型数，单位为"unit"。
	//
}DDTBBECARRStatus,*PDDTBBECARRStatus;

//码同步状态结构
typedef struct DDTBBECodeSynStatusBlk
{
    //0xE613
    DDTBBECodeSynParaSet   CodeSynPara;
    //
    USHORT nCodeLk;			// 码同步锁定标志  1-锁定 0--失锁
    UCHAR  ucReserved[2];
    //
    ULONG  ulEb;			// 码元幅度，32位无符号整型数。Eb/N0计算公式20logEb/N0。
    ULONG  ulN0;			// 噪声幅度
    //
    UCHAR  ucViterbiLk;		// 8位无符号整型数，为1表示维特比译码锁定，为0表示维特比译码失锁。
    UCHAR  ucReserved1[3];
	//
}DDTBBECodeSynStatus,*PDDTBBECodeSynStatus;

//帧同步器状态块结构
typedef struct DDTBBEFrameSynStatusBlk
{ 
    // 0xE615
    DDTBBEFrameSynParaSet    FrameSynPara;  //设置参数返回
    //
    UCHAR  ucFrmLk;			// 帧同步锁定标志，为1表示帧同步锁定，为0表示帧同步失锁
    UCHAR  ucReserved[3];   // 保留三字节
    //
}DDTBBEFrameSynStatus,*PDDTBBEFrameSynStatus;

//副帧同步器状态块结构
typedef struct DDTBBESubFrameSynStatusBlk
{ 
    // 0xE617
    DDTBBESubFrameSynParaSet    SubFrameSynPara;  //设置参数返回
    //
    UCHAR  ucSFrmLk;        // 帧同步锁定标志，为1表示帧同步锁定，为0表示帧同步失锁
    UCHAR  ucReserved[3];   // 保留三字节
	//
}DDTBBESubFrameSynStatus,*PDDTBBESubFrameSynStatus;

// 引导跟踪接收机状态
typedef struct BBELeadTrackStatusBlk
{
	//0xE553
	BBELeadTrackSetStruct tparam; //引导接收机参数
	//
	SHORT	sAzimuVolt;			//方位误差电压,16位有符号整型数，仅在正常工作时有效，量化单位：10/2^15 V
	SHORT	sElevaVolt;			//俯仰误差电压,16位有符号整型数，仅在正常工作时有效，量化单位：10/2^15 V
	//
	UCHAR   ucStatus;			//校准状态，8位无符号整型数，仅在自动校准时有效。
								//D7~ D0位：00H无效；01H示正在进行自动校准；FFH值表示自动校准结束。
	UCHAR	ucReserved;			//保留1个字节
	USHORT	usPhaseAdjust;		//相位修正值，16位无符号整型数
	//
	SHORT	sAzimuZero;			//方位零值修正,16位有符号整型数，量化单位：10/2^15 V。
	SHORT	sElevaZero;			//俯仰零值修正,16位有符号整型数，量化单位：10/2^15 V。
	//
	int		iAzimuSlope;		//方位斜率,32位有符号整型数,仅在方位拉偏自动校相结束时有效。
	//
	int		iElevaSlope;		//俯仰斜率,32位有符号整型数,仅在俯仰拉偏自动校准结束时有效。
	//
	ULONG	ulReserved[3];		//保留	
	
} BBELeadTrackStatusStruct, *PBBELeadTrackStatusStruct;


//调制状态结构
typedef struct DDTBBEModuStatusBlk  
{
	//0xE619 
	DDTBBEModuParaSet      ModuPara;  //设置返回结构   
	int					   iDopNco;	  //上行多普勒频偏的NCO值，32位有符号整型数，量化单位：工作时钟/2^32 Hz。
    
}DDTBBEModuStatus,*PDDTBBEModuStatus;

//解密状态 加密状态
typedef struct  
{//0x1303 解密状态 加密状态0x1301
	EncryptParaStruct ParaSet;
	UINT Reserved[4];
}EncryptStatusStruct,*LPEncryptStatusStruct;

////////上行调制模拟源状态///////////////////////////
//14个32位字
typedef struct
{   
	//0xE621
	DDTBBBEEmulatorSetStruct  EmuStatus;
	UINT	B000T[2];		// 当前时刻，48位无符号整型数，为0.1ms时码格式。
							// D3 ~ D0：量化单位为0.1ms；D7 ~ D4：量化单位为1ms；
							// D11 ~ D8：量化单位为10ms；D15 ~ D12：量化单位为100ms；
							// D19 ~ D16：量化单位为1s；D23 ~ D20：量化单位为10s；
							// D27 ~ D24：量化单位为1min；D31 ~ D28：量化单位为10min；
							// D35 ~ D32：量化单位为1hour；D37 ~ D36：量化单位为10hour；
							// D41 ~ D38：量化单位为1day；D45 ~ D42：量化单位为10day；
							// D47 ~ D46：量化单位为100day。
	UINT	ucReserved[2];
	//


} DDTBBBEEmulatorStaStruct, *PDDTBBBEEmulatorStaStruct;

//检测输出状态结构
typedef struct _ddt_bbe_test_out_status_
{
    //接收机E633 调制器E635
    DDTBBETestOutSetStruct tParams;

}DDTBBETestOutStatusBlk;

//////////////////////////////////////////////////////////////////////////

//S频段下变频器工作参数设置块结构
typedef struct
{   //S下变频器：0xE488
	USHORT usOnLine;			// 在线标志，0-在线 1-不在线
	USHORT usPowOut;			// 是否向LNA馈电，0-不馈电 1-馈电
	UINT   nIFOutFreq;			// 输出中频频率，S下变频器该值范围：68 MHz ~ 72 MHz，缺省值为70 MHz，量化单位：Hz
	UINT   nRFInFreq;			// 输入射频频率，S下变频器该值范围：2200MHz～2300MHz，量化单位：Hz
	SHORT  sAtten1;				// 数控衰减器1衰减量，0~65dB
	SHORT  sAtten2;				// 数控衰减器2衰减量，0~44dB
	SHORT  sRefSel;				// 参考选择0x0000=内参考，0x0001=外参考
	UINT   nUpdateFlag;			// 设置有效标志，按位解释，每位对应一种参数控制是否有效，对应位为1表示设置生效，为0表示无效，
	                            // 0xFFFFFFFF表示所有参数都有效，B0～B6分别对应参数OnLine～RefSel参数。
	USHORT usAlterMode;			// 衰减模式，1--自动, 0--手动，此参数仅在实时处理衰减时用，不加载到硬件
}BBEDownAlterParaStruct, *LPBBEDownAlterParaStruct;

//S频段下变频器工作状态参数块结构
typedef struct
{   //S下变频器：0xE489  33
	UCHAR  ucControlMode;			// 控制模式，00-分控 01-本控
	UCHAR  ucSelfCheckState;		// 自检状态，00H-正常，FFH-故障
	UCHAR  ucLockStatuc;			// 锁定指示，00-失锁 01-锁定
	USHORT usAlterMode;			// 两级下变频器的变频模式:0x0000＝两变频器均为高差，1＝两变频器均为低差，
												// 2＝第一级变频器为高差，第二级变频器为低差3＝第一级变频器为低差，第二级变频器为高差
	UINT   nRFInFreq;					// 实际输入射频信号频率，量化单位：Hz
	BBEDownAlterParaStruct ParaSet;	// 设置参数结构
}BBEDownAlterStatusStruct, *LPBBEDownAlterStatusStruct;

//S频段上变频器工作参数设置块结构
typedef struct
{   //S上变频器：0xE494
	USHORT usOnLine;				// 在线标志，0-在线 1-不在线
	UINT   nInFreq;					// 输入中频频率，S上变频器该值范围：68 MHz ~ 72 MHz，缺省值为70 MHz，量化单位：Hz
	UINT   nRFOutFreq;			// 输出射频频率， S上变频器该值范围：2200MHz～2300MHz，量化单位：Hz
}BBEUpAlterParaStruct, *LPBBEUpAlterParaStruct;

//S频段上变频器工作状态参数块结构
typedef struct
{   //S上变频器：0xE495
	UCHAR  ucControlMode;			// 控制模式，00-分控 01-本控
	UCHAR  ucSelfCheckState;		// 自检状态，00H-正常，FFH-故障
	UCHAR  ucLockStatuc;			// 锁定指示，00-失锁 01-锁定
	BBEUpAlterParaStruct ParaSet;	// 设置参数结构
}BBEUpAlterStatusStruct, *LPBBEUpAlterStatusStruct;

//Ka频段下变频器工作参数设置块结构
typedef struct
{   //0x15CA
	UCHAR ucSetFlag;			// 
	UCHAR ucRefSel;				// 参考选择0x00=内参考，0x01=外参考
	UCHAR ucOnLine1;			// 流1在线标志，0-在线 1-不在线
	UINT   nRFInFreq1;			// 流1输入射频频率，Ka下变频器该值范围：26500MHz～28350MHz，量化单位：10Hz
	UCHAR ucRsvd1[8];			// 保留
	UCHAR ucOnLine2;			// 流2在线标志，0-在线 1-不在线
	UINT   nRFInFreq2;			// 流2输入射频频率，Ka下变频器该值范围：26500MHz～28350MHz，量化单位：10Hz
	UCHAR ucRsvd2[8];			// 保留
}KaDCParaStruct, *LPKaDCParaStruct;

//Ka频段下变频器工作状态参数块结构
typedef struct
{   //0x15C9
	UCHAR  ucControlMode;			// 控制模式，00-分控 01-本控
	UCHAR  ucSelfCheckState;		// 自检状态，00H-正常，FFH-故障
	UCHAR  ucLockStatus1;			// 流1锁定指示，00-失锁 01-锁定
	UCHAR  ucLockStatus2;			// 流2锁定指示，00-失锁 01-锁定
	KaDCParaStruct ParaSet;		    // 设置参数结构
}KaDCStatusStruct, *LPKaDCStatusStruct;

//Ka频段上变频器工作参数设置块结构
typedef struct
{   //0x15CC
	UCHAR ucOnLine;				// 在线标志，0-在线 1-不在线
	UCHAR ucRefSel;				// 参考选择0x00=内参考，0x01=外参考
	UINT   nInFreq;				// 保留。
	UINT   nRFOutFreq;			// 输出射频频率， Ka上变频器该值范围：26500MHz～28350MHz，量化单位：10Hz
}KaUCParaStruct, *LPKaUCParaStruct;

//S频段上变频器工作状态参数块结构
typedef struct
{   //0x15CB
	UCHAR  ucControlMode;			// 控制模式，00-分控 01-本控
	UCHAR  ucSelfCheckState;		// 自检状态，00H-正常，FFH-故障
	UCHAR  ucLockStatuc;			// 锁定指示，00-失锁 01-锁定
	KaUCParaStruct ParaSet;		    // 设置参数结构
}KaUCStatusStruct, *LPKaUCStatusStruct;


//中频开关矩阵参数
typedef struct _sw_if_mat_para_struct_
{   //0xE498
	// 输入通道号含义：
	// n（n>=1且n<=7）：输入通道n（下变频器n）；n=8：检验
	// 输出通道号含义：
	// 0：无效； n（n>=1且n<=7）：输出通道n（基带n）；n=8：检测
    UCHAR ucOut1Sel;                //输出通道1对应输入通道号
    UCHAR ucOut2Sel;                //输出通道2对应输入通道号
    UCHAR ucOut3Sel;                //输出通道3对应输入通道号
    UCHAR ucOut4Sel;                //输出通道4对应输入通道号
    UCHAR ucOut5Sel;                //输出通道5对应输入通道号
    UCHAR ucOut6Sel;                //输出通道6对应输入通道号
    UCHAR ucOut7Sel;                //输出通道7对应输入通道号
    UCHAR ucOut8Sel;                //输出通道8对应输入通道号
    UCHAR ucOut9Sel;                //输出通道9对应输入通道号
    UCHAR ucOut10Sel;               //输出通道10对应输入通道号
    UCHAR ucOut11Sel;               //输出通道11对应输入通道号
    UCHAR ucOut12Sel;               //输出通道12对应输入通道号
    UCHAR ucOut13Sel;               //输出通道13对应输入通道号
    UCHAR ucOut14Sel;               //输出通道14对应输入通道号
    UCHAR ucOut15Sel;               //输出通道15对应输入通道号
    UCHAR ucOut16Sel;               //输出通道16对应输入通道号
	UCHAR ucOut17Sel;               //输出通道17对应输入通道号
    UCHAR ucOut18Sel;               //输出通道18对应输入通道号
    UCHAR ucOut19Sel;               //输出通道19对应输入通道号
    UCHAR ucOut20Sel;               //输出通道20对应输入通道号
    UCHAR ucOut21Sel;               //输出通道21对应输入通道号
    UCHAR ucOut22Sel;               //输出通道22对应输入通道号
    UCHAR ucOut23Sel;               //输出通道23对应输入通道号
    UCHAR ucOut24Sel;               //输出通道24对应输入通道号
    UCHAR ucOut25Sel;               //输出通道25对应输入通道号
    UCHAR ucOut26Sel;               //输出通道26对应输入通道号
    UCHAR ucOut27Sel;               //输出通道27对应输入通道号
    UCHAR ucOut28Sel;               //输出通道28对应输入通道号
    UCHAR ucOut29Sel;               //输出通道29对应输入通道号
    UCHAR ucOut30Sel;               //输出通道30对应输入通道号
    UCHAR ucOut31Sel;               //输出通道31对应输入通道号
    UCHAR ucOut32Sel;               //输出通道32对应输入通道号
	UCHAR ucOut33Sel;               //输出通道33对应输入通道号
    UCHAR ucOut34Sel;               //输出通道34对应输入通道号
    UCHAR ucOut35Sel;               //输出通道35对应输入通道号
    UCHAR ucOut36Sel;               //输出通道36对应输入通道号
    UCHAR ucOut37Sel;               //输出通道37对应输入通道号
    UCHAR ucOut38Sel;               //输出通道38对应输入通道号
    UCHAR ucOut39Sel;               //输出通道39对应输入通道号
    UCHAR ucOut40Sel;               //输出通道40对应输入通道号
    UCHAR ucOut41Sel;               //输出通道41对应输入通道号
    UCHAR ucOut42Sel;               //输出通道42对应输入通道号
    UCHAR ucOut43Sel;               //输出通道43对应输入通道号
    UCHAR ucOut44Sel;               //输出通道44对应输入通道号
    UCHAR ucOut45Sel;               //输出通道45对应输入通道号
    UCHAR ucOut46Sel;               //输出通道46对应输入通道号
    UCHAR ucOut47Sel;               //输出通道47对应输入通道号
    UCHAR ucOut48Sel;               //输出通道48对应输入通道号
	UCHAR ucOut49Sel;               //输出通道49对应输入通道号
    UCHAR ucOut50Sel;               //输出通道50对应输入通道号
}HK_IFSWMatPara;

typedef struct _sw_if_mat_para_ex_struct_
{
	HK_IFSWMatPara param;
	int paramCount;
}HK_IFSWMatParaEx;
//中频开关矩阵状态
typedef struct _sw_if_mat_statu_struct_
{   //0xE499
	UCHAR ucConMode;                //控制模式：0-分控 1-本控
    UCHAR ucSelfCheckStatu;         //自检状态，00H-正常，FFH-故障	

	// 输入通道号含义：
	// n（n>=1且n<=7）：输入通道n（下变频器n）；n=8：检验
	// 输出通道号含义：
	// 0：无效； n（n>=1且n<=16）：输出通道n（基带n）；n=8：检测
	HK_IFSWMatPara	tParam;
}HK_IFSWMatStatu;

//20170321 wl 开关矩阵响应指令
typedef struct _sw_if_mat_respose_struct_
{
    UCHAR ucResponse;   //
}HK_IFSWMatResponse;
//jc  单元参数控制命令响应
typedef struct _tag_unit_param_control_cmd
{
    UCHAR ucResponse;
}UPCTLCMD;

//jc 过程控制命令响应
typedef struct _tag_process_control_cmd
{
        UCHAR ucResponse;
}PCTLCMD;

//jc组参数设置命令响应
typedef struct _tag_group_param_set_cmd
{
        UCHAR ucResponse;
}GPSETCMD;
//信道控制组合参数
typedef struct CHCSetBlk
{   //0x15CE
	// 
    UCHAR ucOut1Sel;                //输出通道1对应输入通道号
    UCHAR ucOut2Sel;                //输出通道2对应输入通道号
    UCHAR ucOut3Sel;                //输出通道3对应输入通道号
    UCHAR ucOut4Sel;                //输出通道4对应输入通道号
    UCHAR ucOut5Sel;                //输出通道5对应输入通道号
    UCHAR ucOut6Sel;                //输出通道6对应输入通道号
    UCHAR ucOut7Sel;                //输出通道7对应输入通道号
    UCHAR ucOut8Sel;                //输出通道8对应输入通道号
    UCHAR ucOut9Sel;                //输出通道9对应输入通道号
    UCHAR ucOut10Sel;               //输出通道10对应输入通道号
    SHORT sAtten1;					//衰减1
    SHORT sAtten2;					//衰减2
    SHORT sAtten3;					//衰减3
    SHORT sAtten4;					//衰减4
    SHORT sAtten5;					//衰减5
    SHORT sAtten6;					//衰减6
    SHORT sAtten7;					//衰减7
    SHORT sAtten8;					//衰减8
	UCHAR ucRsvd[8];
}CHCSetStruct, PCHCSetStruct;

//信道控制组合状态
typedef struct CHCStatusBlk
{   //0x15CD
	UCHAR ucConMode;                //控制模式：0-分控 1-本控
    UCHAR ucSelfCheckStatu;         //自检状态，00H-正常，FFH-故障	

    //设置参数
	CHCSetStruct	tParam;
}CHCStatusStruct, PCHCStatusStruct;

// 时码器参数块
typedef struct _TIMECODEMACHINE_PARA
{   //0x14BD
	BYTE byTimeSource;
	
	DWORD dwToy;
	WORD wMilSecond;
	
	BYTE byT0Mode;
	
	DWORD dwToyT0;
	WORD wMilSecondT0;
	
	int dwTimeCorrect;
	
	BYTE byFreqSourceSelect;
}TIMECODEMACHINE_PARA, *PTIMECODEMACHINE_PARA;

// 时码器状态块
typedef struct _TIMECODEMACHINE_STATUS
{   //0x14BE
	TIMECODEMACHINE_PARA tParam;
	
	UCHAR byControlMode;			// 控制模式，00-分控 01-本控
	UCHAR bySelfCheckState;		// 自检状态，00H-正常，FFH-故障
	UCHAR byLockStatus;			// 锁定指示，00-失锁 >0-锁定
	BYTE bySatNum;
	
	BYTE byACThreshold;
	
	DWORD dwToy;
	WORD wMilSecond;
	
	DWORD dwToyT0;
	WORD wMilSecondT0;
}TIMECODEMACHINE_STATUS, *PTIMECODEMACHINE_STATUS;

//频谱仪参数数据结构
typedef struct _SPECTRUM_PARA
{   //0xE422
}SPECTRUM_PARA, *PSPECTRUM_PARA;

//频谱仪状态数据结构
typedef struct _SPECTRUM_DATA_STATUS
{   //0xE423
	WORD  wTraceData[501];          //频谱仪每屏数据
	FLOAT fMarkerFrequency;         //MARK频率 - 单位:Hz
	FLOAT fMarkerLevel;             //MARK的功率 - 单位见nUnit
	INT   iMarkPos;                 //MARK在屏幕上的位置
	FLOAT fStartFrequency;          //起始频率 - 单位:Hz
	FLOAT fStopFrequency;           //结束频率 - 单位:Hz
	FLOAT fCenterFrequency;         //中心频率 - 单位:Hz
	FLOAT fSpanFrequency;           //间距 - 单位:Hz
	FLOAT fRefLevel;                //参考功率 - 在频谱仪图象上参考功率在最上，从上往下变小 - 单位见nUnit
	FLOAT fATTLevel;                //参考功率 - 在频谱仪图象上参考功率在最上，从上往下变小 - 单位见nUnit
	FLOAT nRBW;                     //分辨带宽 - 单位:Hz
	FLOAT nVBW;                     //视觉带宽 - 单位:Hz
	FLOAT fSweepTime;               //扫描时间 - 单位ms
	INT   iDD;                      //纵轴每格坐标 - 0:10dB 1:5dB 2:dB 3:1dB
	INT   iUnit;                    //纵轴坐标单位 - 0:dBm 1:dBmV 2:dBmV 5:V 6:W
}SPECTRUM_DATA_STATUS, *PSPECTRUM_DATA_STATUS;

//GPS北斗授时仪参数设置
typedef struct _GPSBD_PARA
{   //0x1008
	//命令选择
	//0： 接收机时标输出
	//1： 位置初始化
	//2： 秒脉冲平移
	//3： 工作模式选择
	//4： 平滑时间
	INT nSelect;
	//工作模式
	//0： 四维定位
	//1： 位置保持
	BYTE byWorkMode;
	//初始化位置，X，Y，Z
	FLOAT	fX;
	FLOAT	fY;
	FLOAT	fZ;
	//GPS秒脉冲平移
	INT	nGPSShift;
	//BD秒脉冲平移
	INT	nBDShift;
	//时标输出模式
	//单GPS模式
	//单BD模式
	//转发B000码模式
	BYTE byTimeMode;
	//平滑时间
	INT	nSmoothTime;
}GPSBD_PARA, *PGPSBD_PARA;

//GPS北斗授时仪工作状态
typedef struct 
{   //0x1009
	DOUBLE fGPS_WS;		//GPS周秒	DOUBLE	s
	USHORT usGPS_week;	//GPS周	INTEGER	week
	BYTE   byGPS_second;//UTC秒(gps)	BYTE	s
	BYTE   byGPS_minute;//UTC分(gps)	BYTE	minute
	BYTE   byGPS_hour;	//UTC小时(gps)	BYTE	hour
	BYTE   byGPS_day;	//UTC日(gps)	BYTE	date
	BYTE   byGPS_month;	//UTC月(gps)	BYTE	month
	USHORT usGPS_year;	//UTC年(gps)	INTEGER	year
	BYTE   byBD_second;	//北斗时间UTC秒	BYTE	s
	BYTE   byBD_minute;	//北斗时间UTC分	BYTE	minute
	BYTE   byBD_hour;	//北斗时间UTC小时	BYTE	hour
	BYTE   byBD_day;	//北斗时间UTC日	BYTE	date
	BYTE   byBD_month;	//北斗时间UTC月	BYTE	month
	USHORT usBD_year;	//北斗时间UTC年	INTEGER	year
	USHORT usState;		//GPS/BD授时接收机状态	INTEGER		
                        //Bit0,1表示GPS/BD授时接收机时标输出模式
                        //00  GPS时标模式
                        //01  BD时标模式
                        //10  外部输入时标模式
                        //Bit2表示GPS/BD授时接收机工作模式
                        //0  四维定位工作模式
                        //1  位置保持工作模式
                        //Bit3表示GPS时间是否有效
                        //0  GPS时间无效
                        //1  GPS时间有效
                        //Bit4表示BD时间是否有效
                        //0  BD时间无效
                        //1  BD时间有效
                        //Bit5表示UTC时间（GPS系统时间）是否有效
						//0  UTC时间无效
						//1  UTC时间有效
						//Bit6 表示是否有外部输入时标
						//0  没有外部输入时标
						//1  存在外部输入时标
						//Bit7保留
						//Bit11～Bit8	参与授时的北斗卫星数
						//Bit15～Bit12	参与定位或授时的GPS卫星数
	BYTE   byMode;		//GPS/BD授时接收机定位模式及状态	BYTE		
						//GPS/BD授时接收机定位模式只有GPS定位模式
						//Bit7,6,5,4 GPS定位卫星数；
						//Bit3,2,1,0 GPS/BD授时接收机的定位状态
						//定义如下(hex)：
						//状态位定义: 
						//0	正常定位
						//1	还没有有效时间
						//2	有故障星
						//3	PDOP值太大
						//8	没有可用卫星
						//9	只有1颗可用卫星
						//A	只有2颗可用卫星
						//B	只有3颗可用卫星
						//C	只有4颗可用卫星(混合定位时)
						//D	高度超差（>1000km或<-1km）
						//E	频差>3.0 PPM
						//F	速度>13000 m/s	
	SHORT  sTime;	    //平滑次数	INTEGER	s
	FLOAT  fTimer_x;    //Timer_x	SINGLE	m
	FLOAT  fTimer_y;    //Timer_y	SINGLE	m
	FLOAT  fTimer_z;    //Timer_z	SINGLE	m
}GPSBDStatus;

//////////////////////////////////////////////////////////////////////////
//tdm added beleow //061127

//宏描述参数块，查询宏列表时使用的数据结构
typedef struct
{   //
	CHAR	ucModifyFlag;		//0: 修改当前配置区参数 ； 1：修改宏参数； 2:既修改到配置区同时修改宏
	WORD	wMacroNo;			//宏号
	CHAR	szTaskName[20];		//任务名称
	CHAR	szTaskSymbol[20];	//任务代号
}TaskDescribePara;

//Module ID 数据结构
typedef union MIDSTRUCT
{
	__int64 MID;
	struct
	{
		BYTE byAttenaNo;	//天线号,写参数配置区时使用
		BYTE byWorkMode;	//工作模式
		WORD wNID;			//流号
		WORD wTYPE;			//类型号
		WORD wSID;			//源分系统号
	};
}MIDStruct, *pMIDStruct;
//////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//主备机切换设置结构
// Michael Woo 20070312 Revise
struct THostSpareSwitchSet
{   
	//0x100E
	// 切换方式
	// 2: 自动；1：手动
	BYTE bySwitchMode;

	// 天线号
	BYTE byAntID;

	union
	{
		// 设置标志
		BYTE byFlag; 

		struct  
		{
			BYTE bSwitchMode: 1;
			BYTE bServer: 1;
			BYTE bBBE: 1;
			BYTE bLNA: 1;
			BYTE bDC: 1;
			BYTE bGPSBDTimer: 1;
			BYTE bTimeCoder: 1;
		}Flag;
	};	

	// 监控服务器工作机号
	WORD wServerWorkID;
	// 监控服务器备份机号
	WORD wServerBakID;
	// 基带工作机号
	WORD wBBEWorkID;
	// 基带备份机号
	WORD wBBEBakID;
	// LNA工作机号
	WORD wLNAWorkID;
	// LNA备份机号
	WORD wLNABakID;
	// 下边频器工作机号
	WORD wDCWorkID;
	// 下边频器备份机号
	WORD wDCBakID;
	// GPS/BD授时仪工作机号
	WORD wGPSTimerWorkID;
	// GPS/BD授时仪备份机号
	WORD wGPSTimerBakID;
	// 时码器工作机号
	WORD wTimeCoderWorkID;
	// 时码器备份机号
	WORD wTimeCoderBakID;
};
//标校塔位置参数结构
typedef struct _TOWER_POSITION
{
	FLOAT fBeginA;     //标校塔方位角,单位：弧度
	FLOAT fBeginE;     //标校塔俯仰角,单位：弧度
}TOWER_POSITION, *PTOWER_POSITION;
//站址位置设置结构
typedef struct _STATION_POSITION
{   //0x1010
	BYTE   byAntNum;   //天线号 - 
	BOOL   bValidFlag; //有效标志 为0或1
	                   //0：经度、纬度、高程输入有效 
	                   //1：X、Y、Z输入有效
	DOUBLE lfLon;      //经度 单位为度
	DOUBLE lfLat;      //纬度 单位为度
	FLOAT  fUight;     //高程 单位为米
	FLOAT  fX;         //单位为米
	FLOAT  fY;         //单位为米
	FLOAT  fZ;         //单位为米
	USHORT lowangle;   //最低仰角 单位为度
	TOWER_POSITION     TowerPos; //标校塔位置
}STATION_POSITION, *PSTATION_POSITION;
//轨道根数设置结构
typedef struct _ORBIT_PARA
{   //0x1012
	DWORD  dwSatID;	   //卫星分类码（卫星代号/宏号）
	WORD   wYear;	   //年
	WORD   wMonth;	   //月
	WORD   wDay;	   //日
	WORD   wHour;	   //时
	WORD   wMinute;    //分
	WORD   wSecond;    //秒
	DOUBLE lfGD_a;	   //卫星轨道根数,轨道半长轴,量化单位0.1m
	DOUBLE lfGD_e;	   //卫星轨道根数,轨道偏心率,量化单位2(-31)
	DOUBLE lfGD_i;	   //卫星轨道根数,轨道倾角,量化单位2(-24)度
	DOUBLE lfGD_o;	   //卫星轨道根数,升交点赤经,量化单位2(-22)度
	DOUBLE lfGD_w;	   //卫星轨道根数,近地点幅角,量化单位2(-22)度
	DOUBLE lfGD_m;	   //卫星轨道根数,平近点角,量化单位2(-22)度
}ORBIT_PARA, *PORBIT_PARA;

//任务同步参数块
typedef struct
{
	//
	char	ucModifyFlag;		//0: 修改当前配置区参数 ； 1：修改宏参数； 2:既修改到配置区同时修改宏
	BOOL	ucFirstPackFlag;	//是否为宏参数首包，0：不是，1：是
	WORD	wMacroNo;			//宏号
	char	MacroName[100];		//宏名称
	char	MacroInfo[200];		//宏描述
}TaskSynOperationPara;

typedef struct  
{
	WORD wTaskNo;
	WORD OpenType;//1;不提示，不保存 2,不提示，保存
}MacroExchange,*pMacroExchang;

//射频分路组合设置
typedef struct _RFUNITPARA
{   //0x1024
	UCHAR ucProjectSel;             //型号选择，01H：YQ2103  02H：YQ2102  03H:  YJ2201
	UCHAR ucChannelSelection;       //通道选择状态，按BIT位控制
	                                //BIT0：通道1状态 1：打开； 0：关闭
	                                //BIT1：通道2状态 1：打开； 0：关闭
	                                //BIT2：通道3状态 1：打开； 0：关闭
	                                //BIT3：通道4状态 1：打开； 0：关闭
	                                //BIT4：通道5状态 1：打开； 0：关闭
	                                //BIT5：通道6状态 1：打开； 0：关闭
	                                //BIT6：通道7状态 1：打开； 0：关闭
	                                //BIT7：通道8状态 1：打开； 0：关闭
}RFUNITPARA, *PRFUNITPARA;

//射频分路组合状态
typedef struct _RFUNITSTATUS
{   //0x1025
	UCHAR ucConMode;                //控制模式：0-分控 1-本控
	CHAR  chSelfCheckStatus;        //自检状态，0 不确定; >0  正常; <0 异常
	UCHAR ucProjectSel;             //型号选择，01H：YQ2103  02H：YQ2102  03H:  YJ2201
	UCHAR ucChannelSelection;       //通道选择状态，按BIT位控制
	                                //BIT0：通道1状态 1：打开； 0：关闭
	                                //BIT1：通道2状态 1：打开； 0：关闭
	                                //BIT2：通道3状态 1：打开； 0：关闭
	                                //BIT3：通道4状态 1：打开； 0：关闭
	                                //BIT4：通道5状态 1：打开； 0：关闭
	                                //BIT5：通道6状态 1：打开； 0：关闭
	                                //BIT6：通道7状态 1：打开； 0：关闭
	                                //BIT7：通道8状态 1：打开； 0：关闭
}RFUNITSTATUS, *PRFUNITSTATUS;

//系统时间同步设置状态结构体（信息元）
struct TSysTimeSyncStatus
{	//0x1019
	BYTE bySync;                    //同步计算机时间标志
	                                //0 不进行同步
	                                //1 同步计算机当前时间到时码器时间
	DWORD dwInterval;               //同步间隔，单位为ms，默认为3600000ms
};

//系统时间同步设置命令结构体
typedef TSysTimeSyncStatus TSysTimeSyncSet;

//本机状态信息元
typedef struct   _DEVICE_STATUS_INFO
{   //0x101A
	BYTE		  byDeviceType;     //本机设备类型  1：本机是监控服务器；  2：本机是监控客户端  3 本机是单机设备(基带等)
	WORD		  wDeviceID;        //本机设备号  1：一号机(A机)；  2：二号机(B机)
	WORD          wSID;             //本机分系统号 如:监控服务器为0x0100 监控客户端0x3400
	BYTE		  byHostOrBak;      //本机是主机还是备机  1：本机是主机；  2：本机是备机
}DEVICE_STATUS_INFO, *PDEVICE_STATUS_INFO;

//系统参数数据结构
typedef struct _SYSTEM_PARA
{   //0x101B
	UINT    nTaskCode;          //任务编码（M字段）
	UCHAR   ucSystemSSTTCOrTTC; //宏中缺省系统工作模式 0x01: 标准TTC模式；0x02: 扩频TTC模式；0x03: FM模式；0x04: CDMA模式；0x05: 信标模式
	UCHAR	ucTTCNo;			//TTC的缺省组号。小于15
	UCHAR	ucSSTTCNo;			//SSTTC的缺省组号。小于15
	UCHAR	ucFMNo;				//FM的缺省组号。小于15
	UCHAR	ucDataSendType;		//数据发送格式（新老格式）
	CHAR	szReserve[15];	    //保留字段
}SYSTEM_PARA, *PSYSTEM_PARA;

#define MAX_DEVICE_NUM  6
#define SERVER_NUM  2

union _DEVICESTATUS
{
	WORD wStatus; // 各设备工作状态
	struct 
	{
		unsigned AMCUStatus	: 2;  //天控器 是否正常: 0 不确定; 1  正常; 2 异常
		unsigned LNAStatus	: 2;  //LNA组合 是否正常: 0 不确定; 1  正常; 2 异常
		unsigned DCStatus	: 2;  //下变频器 是否正常: 0 不确定; 1  正常; 2 异常
		unsigned BBEStatus	: 2;  //基带 是否正常: 0 不确定; 1  正常; 2 异常
		unsigned UCStatus	: 2;  //S上变频器 是否正常: 0 不确定; 1  正常; 2 异常
	};
};

// 天线流相关设备工作状态
typedef struct _ANT_DEVICEWORKSTATUS
{
	//0x101C
	_DEVICESTATUS DeviceStatus[MAX_DEVICE_NUM][SERVER_NUM];		//下标1为设备号，下标2为设备连接的服务器号
}ANT_DEVICEWORKSTATUS, *PANT_DEVICEWORKSTATUS;

union _DEVICELNKSRV
{
	WORD wLnkSrv1; // 各LNA组合设备与监控服务器1号机连通状态
	struct 
	{
		unsigned AMCULnkSrv	: 2;  //天控器 与服务器1号机连通状态:  0 不确定; 1  正常; 2 异常
		unsigned LNALnkSrv	: 2;  //LNA组合 与服务器1号机连通状态:  0 不确定; 1  正常; 2 异常
		unsigned DCLnkSrv	: 2;  //下变频器 与服务器1号机连通状态:  0 不确定; 1  正常; 2 异常
		unsigned BBELnkSrv	: 2;  //基带 与服务器1号机连通状态:  0 不确定; 1  正常; 2 异常
		unsigned UCLnkSrv	: 2;  //S上变频器 与服务器1号机连通状态:  0 不确定; 1  正常; 2 异常
	};
};

// 天线流相关设备连通状态
typedef struct _ANT_DEVICELINKSERVER
{
	// 0x101D
	_DEVICELNKSRV LinkStatus[MAX_DEVICE_NUM][SERVER_NUM];		//下标1为设备号，下标2为设备连接的服务器号
}ANT_DEVICELINKSERVER, *PANT_DEVICELINKSERVER;


typedef struct _DEVICEANTNO
{
	WORD LNAAntNo;	// LNA组合对应的天线编号
	WORD DCAntNo;	// 下变频器对应的天线编号
	WORD BBEAntNo;	// 基带对应的天线编号
}DEVICEANTNO;

// 天线流相关设备对应的天线号信息元
typedef struct _ANT_DEVICEANTNO
{
	//0x1020
	DEVICEANTNO DeviceAntNo[MAX_DEVICE_NUM];
}ANT_DEVICEANTNO, *PANT_DEVICEANTNO;

//视频开关矩阵参数（为2102定义）
typedef struct _VFSM_SET_PARA
{  
	//0x101E

	// 输入通道号含义：
	// 0：无效； n（n>=1且n<=8）：输入通道n（基带n）
	// 输出通道号含义：
	// 0：无效； n（n>=1且n<=8）：输出通道n（角误差n）
    UCHAR ucOut1Sel;                //输出通道1对应输入通道号
    UCHAR ucOut2Sel;                //输出通道2对应输入通道号
    UCHAR ucOut3Sel;                //输出通道3对应输入通道号
    UCHAR ucOut4Sel;                //输出通道4对应输入通道号
    UCHAR ucOut5Sel;                //输出通道5对应输入通道号
    UCHAR ucOut6Sel;                //输出通道6对应输入通道号
    UCHAR ucOut7Sel;                //输出通道7对应输入通道号
    UCHAR ucOut8Sel;                //输出通道8对应输入通道号
}VFSM_SET_PARA, PVFSM_SET_PARA;

//视频开关矩阵状态（为2102定义）
typedef struct _VFSM_STATUS
{  
	//0x101F

 	UCHAR ucConMode;                //控制模式：0-分控 1-本控
    CHAR  chSelfCheckStatus;        //自检状态，0 不确定; >0  正常; <0 异常
	
	// 输入通道号含义：
	// 0：无效； n（n>=1且n<=8）：输入通道n（基带n）
	// 输出通道号含义：
	// 0：无效； n（n>=1且n<=8）：输出通道n（角误差n）
    UCHAR ucOut1Sel;                //输出通道1对应输入通道号
    UCHAR ucOut2Sel;                //输出通道2对应输入通道号
    UCHAR ucOut3Sel;                //输出通道3对应输入通道号
    UCHAR ucOut4Sel;                //输出通道4对应输入通道号
    UCHAR ucOut5Sel;                //输出通道5对应输入通道号
    UCHAR ucOut6Sel;                //输出通道6对应输入通道号
    UCHAR ucOut7Sel;                //输出通道7对应输入通道号
    UCHAR ucOut8Sel;                //输出通道8对应输入通道号
}VFSM_STATUS, *PVFSM_STATUS;

// 天线流无关设备加载标志
union ANTINDDEV_LOAD_FLAG
{
	WORD wAll;
	struct  
	{
		WORD nAMCU: 1;	// 天控器加载标志。1-加载；0-不加载
		WORD nLNA: 1;	// LNA控制组合加载标志。1-加载；0-不加载
		WORD nDC: 1;	// 下变频器加载标志。1-加载；0-不加载
		WORD nBBE: 1;	// 基带加载标志。1-加载；0-不加载
		WORD nEmu: 1;	// 模拟源加载标志。1-加载；0-不加载
		WORD nUC: 1;	// 上变频器加载标志。1-加载；0-不加载
		WORD nTimer: 1;	// 时码器加载标志。1-加载；0-不加载
		WORD nGPSBD: 1;	// GPS/BD授时仪加载标志。1-加载；0-不加载
	};
};

typedef struct _ANTPROCESSINFO
{
	// 0x1022

	// 天线流无关设备加载标志
	ANTINDDEV_LOAD_FLAG LoadFlag;

	// 站号
	BYTE bySatID;

	// 宏调用状态。0：初始化或调用完成；0x55：正在执行
	BYTE byMacroCallStatus;

	// 宏调用前宏号
	WORD wPreMacroID;

	// 当前调用宏宏号
	WORD wMacroID;

	// 工作模式切换状态。0：初始化或调用完成；0x55：正在执行
	BYTE byModeSwitchStatus;

	// 工作模式切换前的模式
	BYTE byPreMode;

	// 当前切换的模式
	BYTE byMode;

	// 工作方式切换状态。0：初始化或调用完成；0x55：正在执行
	BYTE byMannerSwitchStatus;

	// 工作方式切换前的方式
	BYTE byPreManner;

	// 当前切换的方式
	BYTE byManner;

    // 配置宏调用状态。0：初始化或调用完成；0x55：正在执行
    BYTE byCfgMacroCallStatus;
    
    // 配置宏调用前设备组合号
	INT nPreDeviceAsseble;

    // 当前调用配置宏的设备组合号
	INT nDeviceAsseble;

	// 保留字节
	BYTE byReserved[1];
}ANTPROCESSINFO, *PANTPROCESSINFO;

typedef struct _EMUPROCESSINFO
{
	// 0x1023
	
	// 宏调用状态。0：初始化或调用完成；0x55：正在执行
	BYTE byMacroCallStatus;

	// 宏调用前宏号
	WORD wPreMacroID;

	// 当前调用宏宏号
	WORD wMacroID;

	// 工作模式切换状态。0：初始化或调用完成；0x55：正在执行
	BYTE byModeSwitchStatus;

	// 工作模式切换前的模式
	BYTE byPreMode;

	// 当前切换的模式
	BYTE byMode;

	// 设备连接配置状态。0：初始化或调用完成；0x55：正在执行
	BYTE byDevConnStatus;

	// 当前配置天线
	BYTE byCurConfAntNo;

	// 保留
	BYTE byReserved[10];

}EMUPROCESSINFO, *PEMUPROCESSINFO;

// 天线流设备连接关系配置命令参数块
typedef struct _ANT_DEVICES_CONNECT
{
	// 0x1021

	// 配置标志
	union
	{
		WORD byConfigFlag;
		struct  
		{
			WORD bStaID: 1;
			WORD bAntNo: 1;
			WORD bLNANo: 1;
			WORD bLNAChnl: 1;
			WORD bDCNo: 1;
			WORD bBBENo: 1;
		};
	};

	BYTE byStaID;		// 站号
	BYTE byAntNo;		// 天线编号，与天控器对应，从1开始
	BYTE byLNANo;		// LNA控制组合编号，从1开始
	BYTE byLNAChnlSel;	// LNA选择通道选择状态,01H：A通道， 02H：B通道
	BYTE byDCNo;		// 下变频编号，从1开始
	BYTE byBBENo;		// 基带编号，从1开始

	// 天线流无关设备的加载标志
	ANTINDDEV_LOAD_FLAG LoadFlag;

	BYTE byReserved[2]; // 保留2Byte
}ANT_DEVICES_CONNECT, *PANT_DEVICES_CONNECT;

///////////////////////////////////////////////////////////////////////////////////////////////////
// BEGIN 天伺馈

//伺服设置数据结构
typedef struct _AMCUPARA
{   //0x0010
	BYTE   byTaskCode;                  //任务代号 0~FF
	BYTE   byWorkMode;                  //天线工作方式:1：待机；2：手动速度；3：命令位置；4：综合自跟踪；
														//5：数据基带1跟踪6：数据基带2跟踪7：数据基带3跟踪；
														//8：记忆跟踪（只上报不控制）9：数字引导10：程序跟踪11：收藏
	BYTE   byBand;                      //伺服带宽 1：高2：低。
	//20101202 眉县修改
	//INT    iAngleA;                     //方位角，精确到0.01度，用实际数值*100表示
	//INT    iAngleE;                     //俯仰角，精确到0.01度，用实际数值*100表示
	float    iAngleA;                     //方位角，精确到0.01度，用实际数值*100表示
	float    iAngleE;                     //俯仰角，精确到0.01度，用实际数值*100表示

	BYTE   byScanEnable;				//扫描使能 0--不使能，1--使能，使能时扫描参数、扫描速度、扫描范围有效
	BYTE   byScanMode;					//扫描参数：1--方位，2--俯仰，3--联合扫描
	BYTE   byScanSpeed;					//扫描速度：1－0.125度/秒，2－0.25度/秒，3－0.5度/秒，4－1度/秒，5－2度/秒；
	BYTE   byScanRange;					//扫描范围：1－0.3125度，2－0.625度，3－1.25度，4－2.5度，5－5度，6－10度；
}YQ250678ServoSetStruct, *PYQ250678ServoSetStruct;

typedef struct
{
    // 0x15CF
	BYTE TaskId;		//  任务代号, 0～FF
	
	//	20110622 hfn 注释有变化
	BYTE	Mode;			//	工作方式,1待机 2速度手控 3命令位置 4S1跟踪 5S2跟踪 6Ka1跟踪
	//           7Ka2跟踪 8记忆跟踪 9数字引导 10程序跟踪 11收藏 16 测试状态
	
	BYTE	Band;			//	伺服带宽, 1：高,2：低
	float	AngleA;		//	方位实时角,	单位：度 精确道0.01v	
	float	AngleE;		//	俯仰实时角,	单位：度 精确道0.01v	
	float	Speed_AZ;		//	方位速度，	单位：度/秒	 精确道0.01v
	float	Speed_EL;		//	俯仰速度，	单位：度/秒	 精确道0.01v
	float	AngleErrA1;		//	接收机1方位误差电压,	单位：伏 精确道0.01v
	float	AngleErrE1;		//	接收机1俯仰误差电压,	单位：伏 精确道0.01v
	float	AngleAgc1;			//	接收机1误差电压,	单位：伏 精确道0.01v
	BYTE	Lock1;				//	接收机1锁定
	float	AngleErrA2;		//	接收机2方位误差电压,	单位：伏 精确道0.01v
	float	AngleErrE2;		//	接收机2俯仰误差电压,	单位：伏 精确道0.01v
	float	AngleAgc2;			//	接收机2误差电压,	单位：伏 精确道0.01v
	BYTE	Lock2;				//	接收机2锁定
	float	AngleErrA3;		//	接收机3方位误差电压,	单位：伏 精确道0.01v
	float	AngleErrE3;		//	接收机3俯仰误差电压,	单位：伏 精确道0.01v
	float	AngleAgc3;			//	接收机3误差电压,	单位：伏 精确道0.01v
	BYTE	Lock3;				//	接收机3锁定
	
	//	20110622 hfn 增加
	float	AngleErrA4;		//	接收机4方位误差电压,	单位：伏 精确道0.01v
	float	AngleErrE4;		//	接收机4俯仰误差电压,	单位：伏 精确道0.01v
	float	AngleAgc4;			//	接收机4误差电压,	单位：伏 精确道0.01v
	BYTE	Lock4;				//	接收机4锁定
	
	BYTE	ControlMode;		//	分控、本控指示,	0：分控, 1：本控
	
	//	20110622 hfn 注释有变化
	UINT	ACU_Status;			//ACU状态 B0：方位顺预限位；B1：方位逆预限位；B2：方位顺终限位；B3：方位逆终限位；B4：方位过零；
	//B5：方位1#驱动器状态; B6：方位2#驱动器状态; B7：保留；B8：保留；B9：方位1#电机电枢故障；
	//B10：方位2#电机电枢故障；B11：保留；B12：保留；
	//B13：俯仰上预限位；B14：俯仰下预限位；B15：俯仰上终限位；B16：俯仰下终限位；B17：俯仰仓门；B18：保留；B19：保留；
	//B20：俯仰1#驱动器状态；B21：俯仰2#驱动器状态；B22：俯仰1#电机电枢故障；B23：俯仰2#电机电枢故障；
	//B24：急停；B25：ACU本/远控；B26：AZ跳码；B27：EL跳码；B28：编码故障；B29：时码故障；
	//20110901 yan deal
//	BYTE	RecvId;		//	接收机号,0：表示无效1：接收机1  2：接收机2  3：接收机3
}YQ2514ServoStatusStruct;

//////////////////////////////////////////////////////////////////////
// 指向控制命令信息Para字段定义
typedef struct POINT_STRUCTURE 
{
    DWORD PosisionAz;//量化单位：0.001°    // 范围：95.000°~205.000°或200.000°~265.000°
    DWORD PosisionEl;  //量化单位：0.001°    // 范围：10.000°~80.000°
}PointToCtrlParaDef;

//天线工作方式控制命令信息Para字段定义
typedef struct ANT_MODE
{
	BYTE AntMd;       //表示天线方式    // 0＝待机；1＝步进跟踪；2＝程序跟踪；
}AntMdCtrlParaDef;

//时间间隔控制命令信息Para字段定义
typedef struct TIME_INTERVAL
{
	BYTE TrkInterval; //跟踪/程引时间间隔，0跟踪时间无效，1~99表示xx分钟
}TimeIntervalCtrlParaDef;

//宏调用命令信息Para字段定义
typedef struct MACRO_CALL
{
	DWORD TaskId; // 装订的任务代号对应编码
}MasmCallDef;

//宏存储命令信息Para字段定义
// 天伺馈宏存储参数块
typedef struct MACRO_SAVE
{
	// 0x1028
	DWORD TaskId;    // 装订的任务代号对应编码
    DWORD PosisionAz;//量化单位：0.001°    // 范围：95.000°~205.000°或200.000°~265.000°

    DWORD PosisionEl;  //量化单位：0.001°    // 范围：10.000°~80.000°

    BYTE TrnsPol;//表示发射极化    // 0=左旋圆极化；1＝右旋圆极化；2＝水平线极化；3＝垂直线极化；4＝任意线极化

    BYTE RecPol; //表示接收极化 // 0=左旋圆极化；1＝右旋圆极化；2＝水平线极化；3＝垂直线极化；4＝任意线极化
    
    int PolAngle; //格式如数据元素表// 范围：-90°~90°
    
    DWORD FreqLeading;  //表示跟踪接收机频率值，量化0.01MHz//  范围：3700.00MHz~4200.00MHz
    
    BYTE MidFreqBW;//表示中频带宽//  0=TRK.RCVR中频带宽2.5kHz 1=TRK.RCVR中频带宽6.7kHz 2=TRK.RCVR中频带宽280kHz

    BYTE SubThreshold;//副瓣跟踪门限，量化单位：0.1； ; 
    BYTE Threshold;//跌落门限; 量化单位：0.1； 
	char TaskName[20];	//任务代号
	char Reversed[20];  //保留
}MasmSaveDef;

//测角精度统计命令信息Para字段定义
typedef struct STATISTICS_TIMES
{
    BYTE Times; //1~10次
}StaticsDef;

//测角修正参数设置命令信息Para字段定义 20110220 xj 
typedef struct MODIFY_ANGLE
{ 
	//int AzErrorMVal;//方位角系统差修正; 量化单位：0.01；
	//int ElErrorMVal;//俯仰角系统差修正; 量化单位：0.01；
	float AzErrorMVal;//方位角系统差修正; 量化单位：0.01；
	float ElErrorMVal;//俯仰角系统差修正; 量化单位：0.01；
}AntErrorMVal;

// 天伺馈测角精度统计结果信息元
typedef struct RStaticsDef //统计命令响应信息
{
	// 0x1029    
    BYTE	TestStatu;	   // 0：正在进行 1：结束
    DWORD	BeginDate;	   // 开始日期；20081019
    DWORD	BeginTime;	   // 开始时间；101015
    DWORD	EndDate;	   // 结束日期
    DWORD	EndTime;	   // 结束时间
    DWORD	Times;         // 现在正在跟踪第几次
    DWORD	AzEquValue;    // 方位均值，单位0.001度
    DWORD	AzSquDiff;     // 方位方差，单位0.001度
    DWORD	ElEquValue;    // 俯仰均值，单位0.001度
    DWORD	ElSquDiff;     // 俯仰方差，单位0.001度
}RStaticsDef;

//接收极化角设置命令信息Para字段定义
typedef struct ANT_RCV_POL_SET
{
	// 0x4D

	DWORD TaskId;    // 装订的任务代号对应编码
    BYTE RecPol; //表示接收极化。0=左旋圆极化；1＝右旋圆极化；2＝水平线极化；3＝垂直线极化
    int PolAngle; //格式如数据元素表，范围：-90°~90°； 量化单位：0.1；
}RcvPolSetCmd;

// END 天伺馈

// DTE上报状态，为简化设计，只截取DTE上报实时监控信息的前20个字节
union TDTEReportMonitorInfo
{
	// 0x1030
	struct  
	{
		WORD wSubB; // 0x0103
		DWORD dwData; // 信息打包日期，1950 年积日
		DWORD dwTime; // 信息打包时间，0.1 毫秒为量化单位 
		BYTE byMainSlave; // 主备机标志：0x33 表示当前主机为A 机；0xCC表示当前主机为 B机
		BYTE byRouter; // 测站路由方式：0x33 表示路由器方式；0x99 表示 HDLC主方式；0xCC表示HDLC 备方式 
		BYTE byControlFlag; // 系统控制方式：0x33 表示本控/远控，0xCC表示分控
	};

	// 保留1024字节
	BYTE byReservedAll[20];
};

//////////////////////////////////////////////////////////////////////////


//////////////////////检前记录器结构 begin//////////////////////
typedef struct 
{
	char    REMI_Flag[4];
	int     TimeOfDay;
	DWORD   SenderIP;
	short   SenderPort;
	BYTE    SenderID;
	BYTE    Recv_ID;
	WORD    Counter;
	short   DataLen;
	BYTE    Command;
}DataFrameStruct;

// 检前记录器工作状态结构
typedef struct 
{	// 0x1255
    BYTE    Ctrl;			// 控制方式：0=分控，1=本控（远控）
    WORD    Sys;			// 分系统状态：0=正常，其他非0值=故障。具体每一位代表的故障点：
							// bit0：记录回放板故障，
							// bit1：信号调理板故障，
							// bit2：信号监测板故障，
							// bit3：记录磁盘故障，
							// bit4：备份硬盘故障，
							// bit5：网络故障，
							// bit6：操作系统故障，
							// bit7：WRCS或NRCS软件故障；
							// 其余位保留，取值均为0。
    BYTE    Work;			// 分系统当前工作状态，取值范围0x0~0x0D、0xFF，具体定义如下：
							// 0x00：空闲；
							// 0x01：正在设置记录参数；
							// 0x02：正在等待定时记录；
							// 0x03：正在记录；
							// 0x04：正在回放；
							// 0x05：回放暂停；
							// 0x06：正在预览输入；
							// 0x07：正在备份；
							// 0x08：正在恢复；
							// 0x09：正在设置选项；
							// 0x0A：正在格式化；
							// 0x0B：正在自检；
							// 0x0C：正在删除文件；
							// 0x0D：正在查看文件；
							// 0xFF：正在忙（非以上状态）；
	BYTE    Report;			// 战斗报表更新指示。0=没有更新，1=有新的战斗报表生成。
							// MSMS可以据此发送"战斗报表查询"命令获得新的战斗报表， "战斗报表查询"后此标志自动复零。
    WORD    Files;			// 已有记录文件的数目
	BYTE    FlUpdate;		// 文件列表更新指示；0：无更新；1：文件列表从上次查询文件列表后有更新，如有新记录文件或删除文件
	BYTE    CapLack;		// 记录用磁盘容量不足告警；0：正常；1：容量不足告警
    ULONGLONG     Cap;			// 记录用磁盘阵列总容量，单位：MB //20081113 add
    ULONGLONG     CapLeft;		// 记录用磁盘阵列剩余容量，单位：MB //20081113 add
    //int     BCap;           // 备份记录用磁盘阵列总容量，单位：MB
    //int     BCapLeft;       // 备份用磁盘阵列剩余容量，单位：MB
	BYTE    Time[4];		// 记录分系统当前系统时间；前3字节依次为时、分、秒，16进制，第4字节保留
    BYTE    TCnt[4];		// 记录分系统此次开始记录或回放的录放时间计时；前3字节依次为时、分、秒，16进制，第4字节保留
    BYTE    IOLvl[9];		// 记录分系统输入输出信号电平指示；4个字节依次指示输入通道1、2和输出通道1、2的电平幅度，16进制，取值范围0~255
    //int     Clock;			// 记录分系统数据采样时钟频率，单位：Hz
    //BYTE    OutLevel;       // 当前输出电平设定值的负值，定义为：30~0对应输出电平设定值-30dBm~0dBm，30对应-30dBm，0对应0dBm //20081113 add
}StatusFindResStruct;

typedef struct 
{
	short    Year;
    BYTE     Month;
	BYTE     Date;
	BYTE     Hour;
	BYTE     Minute;
	BYTE     Second;
	BYTE     Reserved;
}RS_FullTime_t;
//B码格式
typedef struct
{
    USHORT   usDay;  //1-366
    BYTE     Hour;   //0-23
    BYTE     Minute; //0-59
    BYTE     Second; //0-59
    BYTE     ucLeap; //1为润年  0为平年
}TBRemoteTimeCode;
typedef struct 
{
	short    Year;
    BYTE     Month;
	BYTE     Date;
}RS_FullDate_t;

typedef struct 
{
	BYTE    Sver[4];
    RS_FullTime_t    SDate;
	BYTE    Pver[2];
	RS_FullDate_t    PDate;
}VersionResStruct;

typedef struct 
{
	short   TotalFrame;
	short   CurrFrame;
	short   TotalNum;
	short   FileCnt;	
}RevFileListStruct;

typedef struct 
{
	char    FileName[56];
	DWORD   Code;
	char    Name[16];
	short   Round;
    RS_FullTime_t    TStart;
    RS_FullTime_t    Tend;
	int     Sample;
    BYTE    DRAW;//20081113 add
    BYTE    Chn;
	int     LenMB;
    int     LenMS;
	char    Comment[64]; //20081113 修改
	short   AppdLen;
	BYTE    Append[4096];//20081113 修改
}FileIfoResStruct;

typedef struct 
{
	DWORD   Code;
	char    Name[16];
	short   Round;
    RS_FullTime_t    Timer;
	short   Length;
    BYTE    Chn;
    BYTE    CodeRate;//20081113 add
	char    Comment[64];//20081113 修改
	short   AppdLen;	
}RecordStruct;

typedef struct 
{
	char    FileName[56];
	BYTE    OutLvl;//20081113 add
	short   TStart;
	short   TLength;	
}PlayStruct;

typedef struct 
{
	char    FileName[56];
	short   TStart;
	short   TLength;
    int     SplitLen;	
}BackUpStruct;

typedef struct 
{
	char    FileName[56];
	DWORD   Code;
	char    Name[16];
	short   Round;
    RS_FullTime_t    TStart;
    RS_FullTime_t    Tend;
	int     LenMB;
    int     LenMS;
	char    Comment[64];
}WorkLogResStruct;

typedef struct 
{
	char    FileName[56];
	int     LenMB;
    int     LenMS;
	char    strComment[64];
}RS_FileList_t;

typedef struct 
{
	BYTE     Cmmand;
    BYTE     Reason;
}RejectStruct;

/*********************检前记录器**************************/
typedef struct _Record_Response
{
    char M_CODE[4]; //当前任务代码
    char M_NAME[16]; //当前任务名称
    int M_ROUND; //当前任务圈次
    char FILENAME[64]; //文件名
    char R_TIMER[8]; //定时记录开始时间
    char R_LENGTH[8]; //定长记录时间长度
    char R_COMMENT[101]; //备注信息
    int R_APPD_LEN; //附加记录信息的长度
    char RecordType[2]; //记录类型
    char TimeType[2]; //定时类型
    char tRecordTimeB[6];//定时记录开始时间
    char ucChannal1[2]; //通道选择1
    char ucChannal2[2]; //通道选择2
    char ucChannal3[2]; //通道选择3
    char ucChannal4[2]; //通道选择4
}RECORD_RESPONSE;

typedef struct _tag_RecordFileInfo
{
    char szFileName[64];    //已记录数据文件名
    RS_FullTime_t TStart;   //定时记录开始时间
    ULONGLONG ullFileLenB;  //已记录数据文件长度，单位：B
    int intFileLenmS;       //已记录数据文件时间长度，单位：毫秒
    UINT uiCodeRate1;       //PCM1码率1K~M	单位：bps
    UINT uiCodeRate2;       //PCM2码率1K~M	单位：bps
    UINT uiCodeRate3;       //PCM3码率1K~M	单位：bps
    UINT uiCodeRate4;       //PCM4码率1K~M	单位：bps
    UCHAR ucCodeType1;      //PCM1码型：1：NRZ-L、 2：NRZ-M、 3：NRZ-S、4：BIΦ-L、5：BIΦ-M、6：BIΦ-S、7：RNRZ-L
    UCHAR ucCodeType2;      //PCM2码型：1：NRZ-L、 2：NRZ-M、 3：NRZ-S、4：BIΦ-L、5：BIΦ-M、6：BIΦ-S、7：RNRZ-L
    UCHAR ucCodeType3;      //PCM3码型：1：NRZ-L、 2：NRZ-M、 3：NRZ-S、4：BIΦ-L、5：BIΦ-M、6：BIΦ-S、7：RNRZ-L
    UCHAR ucCodeType4;      //PCM4码型：1：NRZ-L、 2：NRZ-M、 3：NRZ-S、4：BIΦ-L、5：BIΦ-M、6：BIΦ-S、7：RNRZ-L
    UINT uiDataSize;        //可回放的PCM1、2、3、4数据总量 (单位512字节)
    char szComment[101];    //共101个字符，备注信息

}RECORDFILEINFO, *PRECORDFILEINFO;

//检前记录查询的文件列表
typedef struct _tag_RecordFileList
{
    short FL_FRTOTAL;   //文件列表总帧数
    short FL_FRNUM;     //文件列表当前帧数
    short FILESTOTAL;   //磁盘阵列中已记录文件总数量
    short FILECOUNT;    //本帧中发送的文件列表数量
    RECORDFILEINFO fileList[64];      //文件列表
}RECORDFILELIST, *PRECORDFILELIST;

//检前文件信息查询命令
typedef struct _tag_RecordFileInfoComm{
    USHORT usFileID;         //保留
    char   FILENAME[64];     //被查询详细的文件信息
    DWORD  M_CODE;           //保留
    char   M_NAME[16];       //保留
    short  M_ROUND;          //保留
    RS_FullTime_t R_TSTART;  //记录开始时间
    RS_FullTime_t R_TEND;    //记录结束时间
    int    R_SAMPLE;         //记录时的数据采样率
    BYTE   R_CHN;            //记录数据的通道数  取值范围1或2
    ULONGLONG R_LENB;        //记录文件长度 单位B
    int    R_LENMS;          //记录文件时间长度 单位毫秒
    char   R_COMMENT[101];        //备注信息
    short  R_APPD_LEN;       //保留

}RECORDFILEINFOCOMM,*PRECORDFILEINFOCOMM;


//检前记录命令以及响应
typedef struct _tag_RecordComm{
    DWORD M_CODE;          //保留
    char  M_NAME[16];      //保留
    short M_ROUND;         //保留
    char  FILENAME[64];    //文件名
    RS_FullTime_t R_TIMER; //定时记录开始时间
    short R_LENGTH;        //定时记录时间长度  取值范围0-3600，单位秒
    char  R_COMMENT[101];  //备注信息
    short R_APPD_LEN;      //附加记录信息长度  取值范围0-4096
    BYTE  RecordType;      //记录类型   0立即记录  1定时记录
    BYTE  TimeType;        //定时类型   0系统时    1B码
    TBRemoteTimeCode tRecordTimeB;  //定时记录开始时间
    BYTE  ucChannal1;      //通道选择1         1表示通道有效，0表示通道无效
    BYTE  ucChannal2;      //通道选择2         1表示通道有效，0表示通道无效
    BYTE  ucChannal3;      //通道选择3         1表示通道有效，0表示通道无效
    BYTE  ucChannal4;      //通道选择4         1表示通道有效，0表示通道无效
}RECORDCOMM,*PRECORDCOMM;

//回访命令及响应
typedef struct _tag_RecordPlayBack{
    char  FILENAME[64];
    UINT  uiTimeOffset;    //偏移时间 单位秒
    BOOL  bRepeatReplay;   //是否循环回放  1循环 0不循环
}RECORDPLAYBACK,*PRECORDPLAYBACK;

//记录参数设置
typedef struct _tag_Record{
    UINT uiCodeRate;           //码率  1K-2M  单位bps
    USHORT usLoopBandWidth;
    UCHAR  ucCodeType;
    UCHAR  ucChannel;
}Record,*PRecord;

/*********************检前记录器**************************/
/////////////////////检前记录器结构 end///////////////////////////////

/////////////////////////////////wangjie add 校相相关////////////////////////////////
//校相结果 
typedef struct 
{
	UINT   uiMacroID;           //任务标识，M字段
	DWORDLONG   uiComboID;           //设备组合号
	CHAR   cTaskSymbol[6];      //任务代号，6个字符
	BYTE   byRcvType;			// 接收类型；
								// 0x00：单通道单脉冲；
								// 0x01：多模馈源。
	UCHAR  ucCombType;           // 合成方式；缺省为0，界面设置
                                // 0x00：手动加权；（缺省）
                                // 0x01：最大比合成；
                                // 0x02：单左旋；
                                // 0x03：单右旋。
                                // 0x04：手动合成。

	double usAzimuPhase;  //方位相位  0~360 单位:度
	double usElevaPhase;  //俯仰相位  0~360 单位:度
	double usAzimuSlope;  //方位斜率
	double usElevaSlope;  //俯仰斜率
	USHORT usChanelError; //通道相位差 FM遥测有效    0~360 单位:度 
//	int    iPNDelay;      //时延修正   扩频模式有效  单位 ns
	//单通道单脉冲相关
	SHORT sAzimuPoint;		// 天线方位指向；界面设置。
							// 量化单位：0.001 mil。
	
	SHORT sElevaPoint;		// 天线俯仰指向；界面设置。
							// 量化单位：0.001 mil。
	USHORT usSensitivity;  // 检测灵敏度；界面设置。
	USHORT usMeasNum;     // 测量次数；界面设置。

	USHORT iPNDelay;		// 基准时延修正；界面设置。
							// 表示输入基准与本地基准的时延修正量，量化单位：216/3600。取值范围：00～359.990。
	__int64 i64DownRF;      //点频 added by yangf
}PhaseValueStruct;

//扩频自动校相上报状态信息元
typedef struct _ss_cmd_adjust_phase_statu
{
	//0x116BH
    UCHAR  ucProFlag;                   //过程标识 0－未启动  1－命令校相进行中  2－命令校相结束
	USHORT usMacroID;                   //宏参数代号 
	UCHAR  ucComboID;                   //设备组合号
	UCHAR  ucCmdType;                   //校相命令类型 00-00H：方位校相 01H：俯仰校相 02H：设定校相结果
                                        //03H：方位极性及灵敏度修正 04H：俯仰极性及灵敏度修正
                                        //05H：方位交叉耦合检查 06H：俯仰交叉耦合检查 07H：记录结果 08H：停止
    UCHAR  ucResult;                    //单次校相结果 00H--失败 01H--成功 02--正在校相
	UCHAR  ucDeflectType;               //偏移类型 0--方位正偏 1--方位负偏 2--俯仰正偏 3--俯仰负偏
	UCHAR  ucSignalPolar;               //校相信号旋向 0--左旋 1--右旋
	UCHAR  ucCarrLock;                  //载波锁定 0--失锁 1--锁定
	UCHAR  ucIPLock;                    //I路伪码锁定 0--失锁 1--锁定 模式2 测量支路锁定
	UCHAR  ucQPLock;                    //Q路伪码锁定 0--失锁 1--锁定 模式2 遥测支路锁定
	UCHAR  ucAZErrV;                    //方位误差电压 单位：0.1V
	UCHAR  ucELErrV;                    //俯仰误差电压 单位：0.1V
	UINT   uiAZPhase;                   //方位相位修正（零值），单位：0.01度
	UINT   uiELPhase;                   //俯仰相位修正（零值），单位：0.01度
	UINT   uiAZSlope;                   //方位斜率
	UINT   uiELSlope;                   //俯仰斜率
	UCHAR  ucAZCrossFac;                //方位交叉耦合系数(分母)
	UCHAR  ucELCrossFac;                //俯仰交叉耦合系数(分母)
}SSCmdAdjustPhaseStatu, *PSSCmdAdjustPhaseStatu;

//20070617 增加校相命令，和远三格式相比将校相自检门槛值改为测量点数
//C=0x0021，T=0x0021，本地命令R=0，远程命令R=1。
//Recieve phase cmd
typedef struct 
{
    UCHAR ucPhaseFlg;   //启停标志,
						//00H：方位校相,            01H：俯仰校相,            02H：设定校相结果
						//03H：方位极性及灵敏度修正,04H：俯仰极性及灵敏度修正,05H：方位交叉耦合检查
						//06H：俯仰交叉耦合检查,    07H：记录结果,			  08H：停止
	UCHAR ucSignalPolar; //信号旋向,00H：左旋,01H：右旋
	UCHAR ucAzimu;		//方位偏转方向,00H：正偏,01H：负偏
	UCHAR ucEleva;		//俯仰偏转方向,00H：正偏,01H：负偏
	//
	UCHAR usOffSet;		//偏移量,2~4,单位：Mil
	UCHAR ucPhasePatten;//校相方式 0--对塔，1--偏溃1，2--偏溃2
	UCHAR Reserved[2];
    //单通道单脉冲相关 20110830 wangjie
    SHORT sAzimuPoint;	// 天线方位指向。// 量化单位：0.001 mil。
    SHORT sElevaPoint;	// 天线俯仰指向。// 量化单位：0.001 mil。
    USHORT usSensitivity;  // 检测灵敏度。
    USHORT usMeasNum;      // 测量次数。
    
    USHORT iPNDelay;		// 基准时延修正。
    // 表示输入基准与本地基准的时延修正量，量化单位：216/3600。取值范围：00～359.990。
}PhaseCmdStruct, *PPhaseCmdStruct;

//自动校准保存命令参数结构
typedef struct   _AUTOCALIBRATE_SAVECMD_STRUCT
{
	CHAR szTaskCode[255];                  //任务代号
	WORD wAntNo;                           //天线号
	WORD wMacroNo;                         //宏号
	WORD wBBEWorkingSID;                   //基带主机分系统号
	WORD wLNAWorkingSID;                   //LNA主机分系统号
	WORD wDCWorkingSID;                    //下变频器主机分系统号
	BYTE bySystemMode;                     //模式号（和组号） 0x01: 标准1 0x11: 标准2 0x02 扩频1 0x12 扩频2 0x03 调频
	BYTE byInputSel;                       //旋向 0：左旋 1：右旋
	INT  nDeviceAssemble;                  //设备组合号 0~9
	MIDStruct i64TrackRevParaID;           //跟踪接收机模块标识号
	__int64 i64DownFreq;                   //点频added by yangf
	UINT  uiMacroID;                       //任务标识 by yangf
	BBEMainTrackSetStruct TTCTrackSetPara; //跟踪接收机参数设置结构

}AUTOCALIBRATE_SAVECMD_STRUCT, *PAUTOCALIBRATE_SAVECMD_STRUCT;

///////////////////////////////////////wangjie add end/////////////////////////////////////////////////////

// 信道增益装订结构体
typedef union 
{
	// 0x15F5
	UINT unRsvdAll[16];
	struct
	{
		UINT   uiMacroID;           //任务标识，M字段
		DWORDLONG   uiComboID;           //设备组合号
		CHAR   cTaskSymbol[10];     //任务代号，6个有效字符
		BYTE   byRcvType;			// 接收类型；
									// 0x00：单通道单脉冲；
									// 0x01：多模馈源。
		UCHAR  ucCombType;           // 合成方式；缺省为0，界面设置
									// 0x00：手动加权；（缺省）
									// 0x01：最大比合成；
									// 0x02：单左旋；
									// 0x03：单右旋。
									// 0x04：手动合成。

		DOUBLE lfGainL;				// 左旋信道增益,dB
		DOUBLE lfSysFiL;			// 左旋系统φ值，dBm/Hz
		DOUBLE lfFreqCorrL;			// 左旋频率修正，Hz
		DOUBLE lfGainR;				// 右旋信道增益,dB
		DOUBLE lfSysFiR;			// 右旋系统φ值，dBm/Hz
		DOUBLE lfFreqCorrR;			// 右旋频率修正，Hz
	};
}UChnlGain,*PUChnlGain;

// 中频矩阵配置宏参数块
struct TIFMCfgMacro
{
    // 0x15F9
    INT nIndex;
    CHAR szName[20];
    HK_IFSWMatPara IFMPara;
    CHCSetStruct CHCPara;
};

// 设备组合号
union TDeviceAssemble
{	
    struct  
    {
        DWORDLONG nAssembleID; // 组合号
    };
    BYTE abyReservedAll[40];
};

// 中频矩阵配置宏信息元
struct TIFMCfgMacroList
{
    // 0x15FA
    INT nMacroCnt; // 中频矩阵配置宏个数
    TIFMCfgMacro aCfgMacro[20];
};

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

}DATA_SAVE_ENABLE, *PDATA_SAVE_ENABLE;

//存盘命令及状态参数块结构
//C=0x201
//20070329 本结构定义了除遥测数据存盘外其它数据如测距测速等通用存盘命令和状态格式；
typedef struct BBEDataSaveCmdStatusBlk
{
	//命令：0x114A；状态：0x1149；
	//状态信息元流号：0~10-保留给遥测数据；

	//11-USB测距原始数据；12-USB测速原始数据；13-USB测距测速组合原始数据；
	//14-S频段扩频1测距原始数据；15-S频段扩频1测速原始数据；16-S频段扩频1测距测速组合原始数据；
	//17-S频段扩频2测量数据帧（含本地测量帧和下行测量帧）；18-S频段扩频2测距测量数据（硬件给出）；
	//19-S频段扩频2测速测量数据（硬件给出）；

	//21-UCB测距原始数据；22-UCB测速原始数据；23-UCB测距测速组合原始数据；
	//24-C频段扩频1测距原始数据；25-C频段扩频1测速原始数据；26-C频段扩频1测距测速组合原始数据；
	//27-C频段扩频2测量数据帧（含本地测量帧和下行测量帧）；28-C频段扩频2测距测量数据（硬件给出）；
	//29-C频段扩频2测速测量数据（硬件给出）；

	//31-采样前测距结果数据；32-采样前测速结果数据；33-采样前测距测速组合结果数据；
	//34-采样后测距结果数据；35-采样后测速结果数据；36-采样后测距测速组合结果数据；

	//50-角误差数据；
	//60-遥控环回指令数据；61-遥控环回脉冲数据；

	//70－遥测发送数据（发往中心机）；
	//71－测距发送数据（发往中心机）；72－测速发送数据（发往中心机）；73－测距测速发送数据（发往中心机）

	//上述定义流号加100为流2的定义；如111为USB测距原始数据流2状态；依次类推。

	WORD	wSID;					//产生数据的设备ID，可以是本地设备，也可能是远程传来的数据；
	WORD	wDataType;				//数据类型，即数据信息元类型号,如果采集和存盘同时进行，
	//可以不通过信息元读取数据；直接保存数据。
	UCHAR	ucStreamNum;			//流号，从0开始计数
	UCHAR	ucSaveEnabled;			//0：停止；1：启动；2：暂停；3：恢复；4：保持
	UCHAR	ucFileHeadFlag;			//有无文件头标志，0：无文件头；1：有文件头，；文件头指1k格式的头
	UCHAR	ucFileNameSpecified;	//是否指定文件名，
	//自动模式时：.\Data\模式名称+数据类型[任务号][流号]年月日时分.dat
	//如扩频1测距原始数据[FD][00]200703291018.dat
	//0：自动模式1，前缀有效（对于遥测数据：YCYS**，其它自定义） 
	//1：自动模式2，前缀有效（对于遥测数据：YYMMDD**，其它自定义） 
	//2：指定文件名,前缀无效，含路径；
	UCHAR	ucSuccedFlag;			//存盘成功标志；0：不成功   1：成功
	ULONG   ulFileSize;				//文件大小字节数；4字节	双字
	ULONG   ulDiskFreeSize;			//剩余空间字节数；4字节	双字
	UCHAR   FileNamePre[80];		//存盘文件名前缀；80字节	字符串	以NULL表示结尾
	UCHAR   FileName[248];			//存盘文件名；248字节	字符串	以NULL表示结尾

} BBEDataSaveCmdStatusStruct, *PBBEDataSaveCmdStatusStruct;


//跟踪接收机输出选择设置
typedef struct LeadRevOutSigPara
{
	// 0x17EE
	USHORT  usWorkMode;//工作模式，0表示分集合成，1表示单左旋工作，2表示单右旋工作
	USHORT  Reserved;//保留

	UCHAR	  ucCHWtMode;       //表示加权模式，0表示自动加权,1表示手动加权,缺省设为0	
    UCHAR	  ucCHWtL;          //左旋手动加权系数,范围是0～1，缺省值为0.5
    UCHAR	  ucCHWtR;          //右旋手动加权系数,范围是0～1，缺省值为0.5
	UCHAR     ucParaSet;        //参数更新标志

	USHORT usAEBw;//角误差电压输出带宽，取值范围5.0~100.0Hz，量化单位0.1Hz
	USHORT Pinhua_BW;//角误差平滑带宽，取值范围1Hz，10Hz，20Hz，100Hz，1000Hz
	
	UINT   Reserved1[5];//保留

}LeadRevOut,*pLeadRevOutSigPara;

//跟踪接收机输出状态
typedef struct LeadRevOutSigSta
{
	// 0x17ED
	LeadRevOut tParams;	
	USHORT     WorkMode;//工作模式，0:分集接收，1：单左旋工作，2：单右旋工作
	USHORT     AEBw;//角误差输出带宽。取值范围5.0~100.0Hz，量化单位0.1Hz。
	
	//
    USHORT	  usCHWtL;          //左旋信号加权值
    USHORT	  usCHWtR;          //右旋信号加权值
	
	UINT       uiRes[2];//保留两个字

}LeadRevOutSta,*pLeadRevOutSta;

typedef struct
{
	////  //0x17F4H
	DWORD     dwMemoryLoad;  //内存使用率
	DWORDLONG dwTotalPhys;  //总的物理内存 字节
	DWORDLONG dwAvailPhys;	//可用的物理内存 字节
	DWORDLONG dwTotalPageFile; //总的分页内存 字节
	DWORDLONG dwAvailPageFile; // 可用分页内存 字节
	DWORDLONG dwTotalVirtual;  //总的虚拟内存
	DWORDLONG dwAvailVirtual;  //可用虚拟内存
	
	DWORD     dwCpuLoad;  //Cpu使用率
	ULONG     uKeActiveProcessors; 
	BYTE      bKeNumberProcessors;
	ULONG     uPageSize; 
	ULONG     uMmNumberOfPhysicalPages; 
	ULONG     uMmLowestPhysicalPage; 
	ULONG     uMmHighestPhysicalPage; 
	ULONG     uAllocationGranularity; 
	
	UINT  uiRes[16];  //保留
}CPUMEMSTATUS_TEST_RESULT,*PCPUMEMSTATUS_TEST_RESULT;

typedef union  ErrProblemInfostruct//故障诊断结构
{
	struct  
	{
		BYTE byRsvdAll[1024];//共1024字节
	};
	// 0x17EF
	struct
	{
		UINT64  uiStaCnt; //故障诊断计数
		UCHAR  dataExchangeSysTelState;//与数据交互分系统通信状态：1表示正常，0表示故障
		UCHAR  SMCUTelState;//SMCU通信状态：1表示正常，0表示故障
		UCHAR  FMTMParaComparison;//调频遥测参数比对结果：1表示正常，0表示故障
		UCHAR dataExchangeSysTelStateB;//保留
		
		UCHAR  ErrParaIndex[32];//比对错误参数块号,ErrParaIndex[0]表示错误个数
		
		UCHAR  FMTMBEEOnlineState;//调频遥测基带在线状态：主用，备用，暂不用
		UCHAR  FMTMBEEBoardWorkState;//调频遥测基带板工作状态：1表示正常，0表示故障
		UCHAR  SupplyElectricity;//供电状态：1表示正常，0表示故障
		UCHAR  HardWareTemperature;//硬件温度：1表示正常，0表示故障
		
		USHORT HardWareTemperatureVal;//硬件温度值
		SHORT Power_L;//左旋功率，量化单位0.1dBm，显示范围-60.0dBm~0.0dBm
		
		SHORT Power_R;//右旋功率，量化单位0.1dBm，显示范围-60.0dBm~0.0dBm
		UCHAR  FMTMSamePllWorkState;//调频遥测共模环工作状态：锁定，失锁
		UCHAR  FMTMDiffPllWorkState;//调频遥测差模环工作状态：锁定，失锁
		
		UCHAR  FMTMCarrierCaptureState;//调频遥测载波捕获状态：捕获，未捕获
		UCHAR  FMTMBitSynState;//调频遥测位同步状态：同步，未同步
		UCHAR  FMTMFrameSynState;//调频遥测帧同步状态：同步，未同步
		UCHAR  subFrmSynState;//调频遥测副帧同步状态：同步，未同步

		UCHAR  TimeUnitState;//时码单元状态：锁定，失锁
		UCHAR   SendStatus;
		UINT64  SendTMDataBytes; //发送遥测数据字节数
		UINT64  SendFrameNum; //发送状态：遥测数据发送模块在发送一包数据后更新此状态
		//DWORD     dwMemoryLoad;  //内存使用率
		//DWORD     dwCpuLoad;  //Cpu使用率
		UCHAR	ucVcoLock;		// FPGA工作时钟锁定指示，8位无符号整型数。
		// 00H表示失锁；01H表示锁定。
		UCHAR	ucRamTest ;		// 内部RAM自检，8位无符号整型数。
		// 00H无效；0EH表示内部RAM读写异常；
		// 0FH表示内部RAM读写正常。界面显示状态。
		UCHAR	ucFpgaTest ;	// FPGA读写自检，8位无符号整型数。
		// 00H无效；0EH表示FPGA读写异常；0FH表示FPGA读写正常。界面显示状态。
		CPUMEMSTATUS_TEST_RESULT CPUMEMSTATUSTESTRESULT;
		//分集接收机和角误差输出参数匹配性检查
		UCHAR   ucRevWorkModeOutSel;
	}ErrorSta;
}ProblemInfopara,*pProblemInfopara;

//调频基带查询的系统状态
typedef struct FMTMRcvSysStatus
{
	// 0x17F2
	USHORT antennaTraceMode;//0：手动跟踪；1：自动跟踪；2：记忆跟踪；3：数字引导；4：程序引导；5等待
	UCHAR  Reserved[1024-2];//保留，共1024字节

}FMTMRcvSysSta,*pFMTMRcvSysSta;

//DTE传输状态
typedef struct FMTMDTETransStatus
{
	// 0x17F4
	UCHAR SendStatus; //发送状态：遥测数据发送模块在发送一包数据后更新此状态
	UINT64  SendPackNum;//发送包数
	UINT64	SendFrameNum;//发送帧数
	UCHAR  Reserved[1024-17];//保留，共1024字节

}FMTMDTETransSta,*pFMTMDTETransSta;

//SMCU传输状态信息
typedef struct SMCUTransStatus
{
	// 0x17F5
	UCHAR SendStatus;   //通讯状态
	UINT64  SendPackNum;//发送包数
	UINT64	SendFrameNum;//发送帧数
	UCHAR  Reserved[50];//保留，
	
}FMSmcuTransSta,*pFMSmcuTransSta;

//网络连接状态
typedef struct NetStatusBlk
{  // 0x1320
	UCHAR   DCUStatus;    //  监控 0 - 连接创建失败 1- 连接正常	
	UCHAR   ACUStatus;   //  ACU   0 - 连接创建失败 1- 连接正常
	UCHAR   CenStatus;   // 中心   0 - 连接创建失败 1- 连接正常	
	UCHAR   DTEStatus;   // DTE    0 - 连接创建失败 1- 连接正常

	UINT    uiReserved[6];	
}NetStatusStruct, *PNetStatusStruct;

//误码率测试命令及结果参数块结构
typedef struct
{
	////  //32H
	BYTE	ucStreamNum;
	BYTE	ucStartStopFlag;		///命令标志	1字节	0：停止  1：启动   2：暂停3：恢复  4：清零  5：保持
	BYTE    Buffer[68];				//用于保存标准文件名或标准值和挑路信息
	BYTE    ucStartCodeTime[6];		//实时启动测试的第一帧数据时间
	BYTE    ucCurrentCodeTime[6];   //实时测试的当前帧数据时间
	BYTE	ucStdFileUseFlag;		/// =0 表示使用自动统计标准值的方法
	// =1 表示使用标准值文件测试，Buffer[68]存放标准文件名，相对路径
	// = 2 表示使用Buffer[68]中定义的标准值和波道号进行测试，
	//      Buffer[0-31]中定义32路或16路波道值，Buffer[32-64]定义32路波道号，波道号为零表示不测。
	// =3 使用附加命令传递标准值和挑路信息
	FLOAT	fErrBitRate;			////误码率	4字节（0~1.0）
	double	dfErrBitNum;			////误码位数	8字节	双精度浮点
	double	dfTotalBitNum;			////已测总码位数	8字节	双精度浮点
	BYTE	ucSuccedFlag;			///命令成功标志		0：不成功   1：成功

}ERRBIT_TEST_RESULT,*PERRBIT_TEST_RESULT;


//误码率测试命令及结果参数块结构
typedef struct
{
	//322H
	BYTE	ucStreamNum;
	BYTE	ucStartStopFlag;		///命令标志	1字节	0：停止  1：启动   2：暂停3：恢复  4：清零  5：保持
	double    dMaxD;                 // ms 最大时延  
	double    dMinD;                 // ms 最小时延  
	double    dMeanD;                 // ms 最小时延  
	double    ddiff;                 // ms   包抖动
	UINT      uiTPackNum;             //   总包数
	UINT      uiLPackNum;             //   丢包数
	double    dLRatio;                // 丢包率 
	UINT      uidisorderNum;          //  乱序包数
	BYTE	  ucSuccedFlag;			///命令成功标志		0：不成功   1：成功
	UINT      uiResv[4];              // 保留 

}VOICE_TEST_RESULT,*PVOICE_TEST_RESULT;



//误帧统计命令及结果参数块结构
typedef struct
{
	//E605H
	BYTE	ucStreamNum;
	BYTE	ucStartStopFlag;		///命令标志	1字节	0：停止  1：启动  
	BYTE    ucStartCodeTime[6];		//实时启动测试的第一帧数据时间
	BYTE    ucCurrentCodeTime[6];   //实时测试的当前帧数据时间
	FLOAT	fErrBitRate;			////误帧率	4字节（0~1.0）
	double	dfErrBitNum;			////已测误帧数	8字节	双精度浮点
	double	dfTotalBitNum;			////已测总帧数	8字节	双精度浮点
	BYTE	ucSuccedFlag;			///命令成功标志		0：不成功   1：成功
	BYTE    Reserved[8] ;           //保留八字节

}ERRFRAME_TEST_RESULT,*PERRFRAME_TEST_RESULT;

//丢帧测试命令及结果参数块结构
typedef struct
{
	////  //33H
	BYTE	ucStreamNum;
	BYTE	ucStartStopFlag;  //命令标志	 1字节	0：停止  1：启动   2：暂停3：恢复  4：清零  5：保持
	BYTE	ucTestTypeFlag;   //测试类型	 1字节	0：依据时码  1：依据ID
	FLOAT	fLostFrameRate;   //丢帧率	     4字节（0~1.0）
	double	dfLostFrames;     //丢帧数	     8字节	双精度浮点
	double	dfTotalFrames;    //已测总帧数	 8字节	双精度浮点
	BYTE	ucSuccedFlag;     //命令成功标志 0：不成功   1：成功

}LOSTFRAME_TEST_RESULT,*PLOSTFRAME_TEST_RESULT;

typedef struct
{
	////  //0x17F6H
	UINT		uiMID;              //任务标识，由于系统标准接口是UINT，因此扩充。
	DWORDLONG   i64DeviceAssembleID;// 设备组合号
	DWORDLONG  dwSystemWorkMode;   //工作模式,01：Ka模式，02：Ka+S模式1（A机Ka，B机S），04：Ka+S模式2（B机Ka，A机S）08：S模式1（1个点频），10：S模式2（2个点频）
	DWORDLONG  uiDownFrq;         //下行工作频率
	UCHAR  ucTaskDescribe[6];		//任务代号
	
	UINT  uiRes[64];  //保留
}SystemParas,*PSystemParas;

// 公共网络包头 
typedef struct  
{
	// 0x1354
	UCHAR 	ucVersion;					// 版本号 
	USHORT	usMID;						// 任务代号，表示本次试验任务代号
	UINT	unSID;						// 信源地址，表示本帧信息发出站的地址
	UINT	unDID;						// 信宿地址，表示本帧信息接收站的地址
	UINT	unB;						// 信息类别，表示本帧信息的种类
	UINT	unNo;						// 包序号，该数据包在一次信息交换过程中的发送序号，0~65535重复计数
	UCHAR	ucFlag;						// 标志字段，定义如下：
	// 码位		标识			代码	  含义
	// b0		Direct_FLAG		0/1			表示信宿地址所指对象的性质，指向真实目标/执行仿真目标
	// b1		Ans_FLAG		0/1			表示对该包数据的应答要求，按默认策略进行应答/需要应答
	// b2		Tranfer_FLAG	0/1			表示该包数据的转发策略 执行默认转发/执行强制转发
	// b5~b3	Encry_FLAG					表示该包数据的明密状态以及是否需要加密
	//							000			信源未进行相关加/解密处理
	//							001			明态，需要进行加密处理
	//							010			密态，需要进行解密处理
	//							011			已经成功进行了加/解密处理
	//							100			加密部成功	
	//							101			解密不成功
	//							110			保留
	//							111	        保留
	// b7~b6	Save_FLAG		0	表明该包数据是否需要长久保存，00-收方自定； 01-永久保存
	UCHAR   ucReverse[4];				// 保留4个字节
	USHORT	usDate;						// 发送日期，信源方发送该数据包时刻相对于1950年1月1日的累计天数，1950年1月1日计为第1天
	UINT	unTime;						// 发送时标，信源方发送该包时刻对应的北京时，量化为0.1ms
	USHORT	usLen;						// 数据域长度(ZT+ZS+子帧数据)，该包内数据域的字节长度，计数范围为0~65535，若数据域的字节数大于65535，则对65536取模
	
}NetPackageHeadPara;

//增加链监信息结构
// 链监状态结构
typedef struct DataChanStatusBlk
{
	//0x15F1
	ULONG ulPackageNum;		// 接收(或发送)发送包数
	ULONG ulFrameNum;		// 接收(或发送)发送帧数，仅对遥测数据有效
	ULONG ulInValidPackageNum;	// 接收有效包数

	USHORT  usStatus;   //状态  0:网络通道未创建成功  1:正常发送（接收）  2:长时间无数据
	USHORT  usReserved;
	ULONG ulReserved[12];		// 保留字节
}DataChanStatusStruct, *PDataChanStatusStruct;

// added by yangf 20150911
// 信号分析数据
//     大小：16个32位字
//数据个数为3
typedef union
{
    // 0x1602
    struct
    {
        UINT unRsvdAll[16];
    };
    
    struct
    {
        UCHAR	B000T25us[6];		//时码，48位无符号整型数，为25us时码格式。
        //D15 ~ D0位：16位无符号整型数，表示1秒内的时间，量化单位：25 us。
        //D19 ~ D16：量化单位为1s；D23 ~ D20：量化单位为10s；
        //D27 ~ D24：量化单位为1min；D31 ~ D28：量化单位为10min；
        //D35 ~ D32：量化单位为1hour；D37 ~ D36：量化单位为10hour；
        //D41 ~ D38：量化单位为1day；D45 ~ D42：量化单位为10day；
        //D47 ~ D46：量化单位为100day。
UCHAR : 8;
UCHAR : 8;
        
        
        INT nDoplCom;                  // 多普勒频率值
        // 量化单位：工作时钟/2^32  Hz/s。
        INT nDoplDiv;
        //
        SHORT sPowLevel1;				//量化单位：0.1dBm
        SHORT sPowLevel2;
        
        //
        UINT nSigPow[3];                // 信号功率
        UINT nNosPow[3];                // 噪声功率
        
        //
        USHORT usA;                 // 方位误差电压（1024个相位）；
        // 量化单位：10/2^15 V。
        
        USHORT usE;                 // 俯仰误差电压（1024个相位）；
        // 量化单位：10/2^15 V。
        
        //
        USHORT usCoeffL;            // 左旋手动合成系数；（缺省0.5）
        // 仅在手动加权时有效。界面设置。
        // 量化单位：0.01，取值范围：0.01~1。
        
        USHORT usCoeffR;            // 右旋手动合成系数；
        // 仅在手动加权时有效。界面设置。
        // 量化单位：0.01，取值 = 1 - CombCoeffL。
        //增加
        USHORT	 usAgcVoltL;          //左旋通道的AGC电压,单位毫伏(mV),范围从0到5000,代表电压值为0～5V，对上报值进行除以1000的处理
        //各通道AGC电压为dB线性，为0时代表输入中频信号功率为-70dBm,
        //为5000mV时代表输入中频信号功率为+10dBm
        USHORT	 usAgcVoltR;          //右旋通道的AGC电压,单位毫伏(mV),范围从0到5000,代表电压值为0～5V，对上报值进行除以1000的处理
        //各通道AGC电压为dB线性，为0时代表输入中频信号功率为-70dBm,
        //为5000mV时代表输入中频信号功率为+10dBm
        //	
        UINT  uiCodeRate;//码率
        USHORT AGC_C;//分集合成AGC电压
        UCHAR  Lock_denote;//从低到高位：VcoLock,Time_Lock,AE_Lock,CarrLk,CarrDivLk,CodeLk,FrmLk,SubFrmLk分别代表
        //VCO时钟锁定，时码锁定，角误差锁定，载波共模环锁定，载波差模环锁定，码同步锁定，帧同步锁定，副帧同步锁定
        UCHAR unRsvd;
    };
} AnalyzerDataStruct;

typedef struct _DSZ_XTJK_DeviceLinkStatus
{   //0x0136
    union
    {
        ULONGLONG ddwDeviceLinkStatus;            //各天线不相关设备与服务器连通状态
        struct
        {
            unsigned FMBBELink1         : 1;  //FM基带1号机连通状态:  0 异常; 1  正常
            unsigned FMBBELink2         : 1;  //FM基带2号机连通状态:  0 异常; 1  正常
            unsigned FMBBELink3         : 1;  //FM基带3号机连通状态:  0 异常; 1  正常

            unsigned BPSKBBELink1       : 1;  //BPSK基带1号机连通状态:  0 异常; 1  正常
            unsigned BPSKBBELink2       : 1;  //BPSK基带2号机连通状态:  0 异常; 1  正常
            unsigned BPSKBBELink3       : 1;  //BPSK基带3号机连通状态:  0 异常; 1  正常

            unsigned SSBBELink1         : 1;  //SS基带1号机连通状态:  0 异常; 1  正常
            unsigned SSBBELink2         : 1;  //SS基带2号机连通状态:  0 异常; 1  正常
            unsigned SSBBELink3         : 1;  //SS基带3号机连通状态:  0 异常; 1  正常

            unsigned FMAKLink1          : 1;  //FM_AK基带1号机连通状态:  0 异常; 1  正常
            unsigned FMAKLink2          : 1;  //FM_AK基带2号机连通状态:  0 异常; 1  正常

            unsigned ZZMAKLink1         : 1;  //ZZM_AK基带1号连通状态:  0 异常; 1  正常
            unsigned ZZMAKLink2         : 1;  //ZZM_AK基带2号连通状态:  0 异常; 1  正常

            unsigned DTELink            : 1;  //DTE: 0 异常; 1  正常

            unsigned ServoLink          : 1;  //Servo连通状态:  0 异常; 1  正常

            unsigned DownAlterLink1     : 1;  //下变频器1号机连通状态:  0 异常; 1  正常
            unsigned DownAlterLink2     : 1;  //下变频器2号连通状态:  0 异常; 1  正常
            unsigned DownAlterLink3     : 1;  //下变频器3号连通状态:  0 异常; 1  正常
            unsigned UpAlterLink        : 1;  //上变频器连通状态:  0 异常; 1  正常

            unsigned IFSwitchLink       : 1;  //中频开关连通状态:  0 异常; 1  正常

            unsigned JQLink1            : 1;  //检前记录器1连通状态:  0 异常; 1  正常
            unsigned JQLink2            : 1;  //检前记录器2号机连通状态:  0 异常; 1  正常
            unsigned JQLink3            : 1;  //检前记录器3号机连通状态:  0 异常; 1  正常
        };
    }DeviceLinkStatusUnion;
}DSZ_XTJK_DeviceLinkStatus,*PDSZ_XTJK_DeviceLinkStatus;

/*********wl add 测试矩阵所需要的结构体***********/
//测试开关矩阵参数
typedef struct _test_sw_if_mat_para_struct_
{
    UCHAR ucOut1Sel;                //输出通道1对应输入通道号
    UCHAR ucOut2Sel;                //输出通道2对应输入通道号
    UCHAR ucOut3Sel;                //输出通道3对应输入通道号
}TEST_SWMATPara;

//测试开关矩阵状态
typedef struct _test_sw_if_mat_statu_struct_
{
    UCHAR ucConMode;                //控制模式：0-分控 1-本控
    UCHAR ucSelfCheckStatu;         //自检状态，00H-正常，FFH-故障
    TEST_SWMATPara	tParam;
}TEST_SWMATStatu;
/*********wl add 测试矩阵所需要的结构体***********/

/*********wl add 20170408 频率基准源监控所需要的结构体***********/
//频率基准源监控参数
typedef struct _Freq_BDcu_para_struct_
{
    UCHAR ucOut1Sel;                //输出通道1对应输入通道号
}FREQBDCUPara;

//频率基准源监控状态
typedef struct _Freq_BDcu_statu_struct_
{
    UCHAR ucConMode;                //控制模式：0-分控 1-本控
    UCHAR ucSelfCheckStatu;         //自检状态，00H-正常，FFH-故障
    FREQBDCUPara	tParam;
}FREQBDCUStatu;
/*********wl add 频率基准源监控所需要的结构体***********/

/*********wl add 20170408 射频切换开关监控所需要的结构体***********/
//射频切换开关监控参数
typedef struct _SswitchDcu_para_struct_
{
    UCHAR ucOut1Sel;                //输出通道1对应输入通道号
    UCHAR ucOut2Sel;                //输出通道2对应输入通道号
    UCHAR ucOut3Sel;                //输出通道3对应输入通道号
    UCHAR ucOut4Sel;                //输出通道4对应输入通道号
    UCHAR ucOut5Sel;                //输出通道5对应输入通道号
    UCHAR ucOut6Sel;                //输出通道6对应输入通道号
    SHORT  sAttenuation1;        //衰减1
    SHORT  sAttenuation2;        //衰减2
}SSWITCHDCUPara;

//射频切换开关监控状态
typedef struct _SswitchDcu_statu_struct_
{
    UCHAR ucConMode;                //控制模式：0-分控 1-本控
    UCHAR ucSelfCheckStatu;         //自检状态，00H-正常，FFH-故障
    SSWITCHDCUPara	tParam;         //数据参数结构体
    //UINT uUpdateFlag;               //有效标志    协议更改去掉
    //USHORT usAttenMode;             //衰减模式
}SSWITCHDCUStatu;
/*********wl add 射频切换开关监控所需要的结构体***********/

/*********wl add 20170408 射频分路组合监控所需要的结构体***********/
//射频分路组合监控参数
typedef struct _SLDCDcu_para_struct_
{
    UCHAR ucOut1Sel;                //输出通道1对应输入通道号
    UCHAR ucOut2Sel;                //输出通道1对应输入通道号
}SLDCDCUPara;

//射频分路组合监控状态
typedef struct _SLDcu_statu_struct_
{
    UCHAR ucConMode;                //控制模式：0-分控 1-本控
    UCHAR ucSelfCheckStatu;         //自检状态，00H-正常，FFH-故障
    SLDCDCUPara	tParam;
}SLDCDCUStatu;
/*********wl add 射频分路组合监控所需要的结构体***********/

/*********wl add 20170409 校零信标一体机监控所需要的结构体***********/
//校零信标一体机监控参数
typedef struct _zero_Calib_beacon_para_struct_
{
    USHORT ucOnlineMark;             //在线标志
    UINT ucInputFreq;                     //输入射频频率
    UINT ucOutputFreq;                  //输出射频频率
    UINT ucBeaconFreq;                  //信标输出频率
    SHORT  sAttenuation1;              //数控衰减器1
    SHORT  sAttenuation2;              //数控衰减器2
    USHORT usReferentSel;             //参考源选择
}ZEROCALIBBEACONPara;

//校零信标一体机监控状态
typedef struct _zero_Calib_beacon_statu_struct_
{
    UCHAR ucConMode;                //控制模式：0-分控 1-本控
    UCHAR ucSelfCheckStatu;       //自检状态：00H-正常，FFH-故障
    UCHAR ucLockStatu;                //锁定指示：00H-失锁，01H-锁定
    ZEROCALIBBEACONPara	tParam;     //数据参数结构体
}ZEROCALIBBEACONStatu;
/*********wl add 校零信标一体机监控所需要的结构体***********/

/*********jc add 20170409 SL频段车载综合测控系统所需要的结构体***********/
//Spectrum vehicle comprehensive measurement and control system
//标准TTC联试应答机监控和非相干扩频联试应答机监控所使用的参数信息
typedef struct _marco_search_para_struct_
{
    USHORT usMarcoNo;           //宏代号
    USHORT usReserved;          //保留
    CHAR cMarcoName[48];       //任务描述
    UINT uiM;                           //任务标识
    CHAR cTaskID[8];                //任务代号
}MARCOSEARCHPara;

typedef struct _combined_transponder_para_struct_
{
    MARCOSEARCHPara paras[100];     //参数宏查询结构体 最大支持100个
    UCHAR ucAnswerFlag;             //应答标识
    int nParaCount;                 //参数宏查询结构体的个数
}COMBTRANSPara;

//对于标准TTC联试应答机监控 为USB 其中目标仿真类型、目标仿真状态和已仿真数据帧 仅1个有效
//对于非相干扩频联试应答机监控 其中目标仿真类型、目标仿真状态和已仿真数据帧 仅前4个有效
//工作状态上报结构体
typedef struct _combined_transponder_statu_struct_
{
    USHORT usDevStat;           //设备状态2字节 0: 故障, 1: 正常
    UCHAR ucCarrOn;             //载波输出状态1字节, 0: 去载, 1: 加载
    UCHAR ucModuOn;          //数据加调状态1字节, 0: 去调, 1: 加调
    UCHAR ucSimuMode[6];    //目标仿真状态, 6字节, 0-动态仿真, 1-文件仿真.
    //目标仿真状态, 6字节, USB仅目标1有效, FFH-初态,  0-模飞停止,
    //1-模飞进行中, 2-模飞暂停, 3-文件模飞完成.
    UCHAR ucSimuOn[6];
    DWORDLONG dwlFrameNum[6];//已仿真数据帧数6*8字节, USB仅目标1有效, 刷新变化.
}COMBTRANSStatu;

/*********jc add SL频段车载综合测控系统所需要的结构体***********/

/*********wl add 20170409 铷钟机箱监控所需要的结构体***********/
//铷钟机箱监参数状态
typedef struct _r_clock_dcu_statu_struct_
{
    UCHAR ucTimeZone;                   //时间时区
    SHORT sYear;                          //年
    UCHAR ucMonth;                      //月
    UCHAR ucDay;                            //日
    UCHAR ucHour;                         //时
    UCHAR ucMin;                            //分
    UCHAR ucSec;                            //秒
    UCHAR ucReferentInvalid;        //参考有效
    UCHAR ucGPSStatus;              //GPS状态
    UCHAR ucNorthStatus;            //北斗状态
    UCHAR ucReferentSel;               //参考选择状态
    UCHAR ucOne10MHzInput;      //1路10MHz输出状态
    UCHAR ucTwo10MHzInput;      //2路10MHz输出状态
    UCHAR ucThird10MHzInput;      //3路10MHz输出状态
    UCHAR ucFour10MHzInput;      //4路10MHz输出状态
    UCHAR ucOne1PPSInput;       //1路1PPS输出状态
    UCHAR ucTwo1PPSInput;       //2路1PPS输出状态
    UCHAR ucSyncStatus;               //同步状态  0-未同步 1-同步
    UCHAR ucRCLOCK;                     //铷钟锁定状态 0-未锁定 1-锁定
    UCHAR ucDemodulateSec;        //解调秒状态 0-无  1-有
    UCHAR ucPunctual;                   //守时状态 0-未守时 1-守时
    UCHAR ucWarning;                    //告警状态 0-不告警 1-告警
    UCHAR ucPower;                       //电源状态 0-异常 1-正常
    UCHAR ucConMode;                //控制模式：0-本控 1-分控
    UCHAR ucTameCtrl;                //驯服模式 0-驯服 1-不驯服
    UCHAR ucSyncCtrl;                   //同步方式：0-手动 1-自动
}RCLOCKDCUStatu;
/*********wl add 铷钟机箱监所需要的结构体***********/
//多普勒补偿界面刷新用
typedef struct dopplerCompensate
{
    UCHAR compensateCtrl;
    UCHAR compensateType;
    UINT  compensatePeriod;
    USHORT resetSpeed;
    BOOL flag;
}DOPPLERCOMPENSATE, *PDOPPLERCOMPENSATE;


#pragma pack()

#endif // __DSZ_DATA_STRUCT_H__
