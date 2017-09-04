/**
 * 本文件包含2311监控模块使用的调频安控基带数据结构。
 */
#ifndef __DCU_2311_DATASTRUCT_H__
#define __DCU_2311_DATASTRUCT_H__


#include "TCK_TC_DataStruct.h"
#pragma pack(1)

typedef struct _param_set_header
{
    UCHAR  ucUnitID;        // 单元标识
        USHORT usCmdCnt;        // 表示发送该命令的累计数从1开始
} PARAM_SET_HEADER;

//本机全局段主备信息列表结构 ID：E721 SN=0
struct TLocalHSInfo
{
    //本机IP地址
    char aLocalIP[2][32];

    //本机编号
    WORD wSerialNum;

    //工作机号 1－A机  2－B机
    UCHAR ucWorkID;

    //本机的主备机状态 0：备机  1：主机
    BYTE byHSStatus;

    //本机切换标志  1：是  0：不是
    BYTE bySwitch;

    //本机的状态  0：正常  1：异常
    BYTE bySysStatus;

    //切换方式 0：自动切换 1：手动切换
    BYTE bySwitchMode;

    //切换状态 0：不切换  1：正在切换
    BYTE bySwitchStatus;

    //切换原因 0：不切换  1：主机异常  2：本机指令 3：中心监控指令 4：主控站指令
    BYTE bySwitchReason;

    //发送帧计数
    DWORD dwSendFrmCnt;
};

//安控全局信息元
typedef union _AK_GLOBALSTATUS
{
    struct {
        char revr[1024];
    };
    struct {//0x0135
    UCHAR  ucCtrlMode;              //监控方式：00H分控 01本控
    UCHAR  ucDevOnlineStatus;       //设备在线状态：00H离线  01HA主B备 02B主A备
    UCHAR  ucSystemMode;          //1-FM_AK   2-ZZM_AK

    UCHAR  ucVcoLock;		        //FPGA工作时钟锁定指示，8位无符号整型数，00H表示失锁；01H表示锁定。
    UCHAR  ucRamTest;		        //内部RAM自检，8位无符号整型数。00H无效；0EH表示内部RAM读写异常；0FH表示内部RAM读写正常。
    UCHAR  ucFpgaTest;	            //FPGA读写自检，8位无符号整型数，00H无效；0EH表示FPGA读写异常；0FH表示FPGA读写正常。
    UCHAR  ucTimeBoardLock;         //时码板锁定指示,8位无符号整型数，D7 ~ D0位：00H表示失锁；01H表示锁定

    UCHAR  ucUpAlterLockStatuc;     //上变频锁定指示，00-失锁 01-锁定
    UCHAR  ucDownAlterLockStatuc;   //下变频锁定指示，00-失锁 01-锁定

    UCHAR  byOnline;                //0-去载 1-加载 软件环境退出时设置为去载
    UCHAR  byModu;                  //0-去调 1-加调

    UINT   uiModuFreq;              //调制频偏 1Hz
    DWORD   dwCarIF;                //输出载波中频，32位无符号整型数，量化单位：1 Hz，取值范围为68.5 M ~ 71.5 MHz，缺省为70 MHz

    UCHAR   ucWorkMode;     //時统模式  0：内時统 1：外時统
    UCHAR   ucRecv[3];

    TLocalHSInfo localInfo;
    char   szrsv[100];

    };
}AK_GLOBALSTATUS;


//--------------------------------------------------------------------------
////主备切换
//typedef struct AK_MAIN_BAK_CHANGE
//{
//	CMDID CmdID;			// 0x0001
//	USHORT usCmdCnt;		// 表示发送该命令的累计数从1开始

//	UCHAR  ucMainBanSel;	// 00HA主B备；01HA备B主

//}AK_MAIN_BAK_CHANGE,*PAK_MAIN_BAK_CHANGE;

////系统退出
//typedef struct AK_SYSTEM_QUIT
//{
	
//	CMDID CmdID;			// 0x0007
//	USHORT usCmdCnt;		// 表示发送该命令的累计数从1开始
	
//	UCHAR  ucSysQuit;	// 00H：系统关机
	
//}AK_SYSTEM_QUIT;

//
typedef struct _AK_STATUS
{
    ushort unit_1_begin;
    AK_GLOBALSTATUS global_status; //
    ushort unit_1_end;
    ushort unit_2_begin;
    ak::BBETCStatusStruct tcStatusStruct;//zzm 暂保留该参结构
    ushort unit_2_end;
    ushort unit_3_begin;
    ak::TTCBBEModuStatusStruct moduStatusStruct; //
    ushort unit_3_end;
    ushort unit_4_begin;
    ak::BBEUpAlterStatusStruct upAlterStatusStruct; //
    ushort unit_4_end;
    ushort unit_5_begin;
    ak::BBEDownAlterStatusStruct downAlterStatusStruct; //
    ushort unit_5_end;
    ushort unit_6_begin;
   ak:: TC_FRECFG_STRUCT frequent_cfg_struct; //
    ushort unit_6_end;
    ushort unit_7_begin;
    ak::CurrentObjectIDStruct objStruct; //
    ushort unit_7_end;
}MCS_AK_STATUS;

// ak
typedef struct _AK_SETTING_PARAM
{
    ushort unit_1_begin;
    ak::CurrentObjectIDStruct objStruct;
    ushort unit_1_end;
    ushort unit_2_begin;
    ak::BBETCSetupStruct  tcSetPara;//zzm 暂保留该参结构
    ushort unit_2_end;
    ushort unit_3_begin;
    ak::TC_MODU_PARA_STRUCT moduParam;
    ushort unit_3_end;
    ushort unit_4_begin;
    ak::BBEUpAlterParaStruct upAlterParaSet;
    ushort unit_4_end;
    ushort unit_5_begin;
    ak::BBEDownAlterParaStruct downParaSet;
    ushort unit_5_end;
}MCS_AK_SETTING_PARAM;

////AK上报状态结构
//typedef struct _AKSTATUSGROUP_REPORT
//{
//    CMD_HEAD CmdHead;
//    WORD  wNo;
//    MCS_AK_STATUS GroupStatus;
//}MCS_AKSTATUSGROUP_REPORT, *PMCS_AKSTATUSGROUP_REPORT;


#pragma pack()
#endif // __DCU_2311_DATASTRUCT_H__
