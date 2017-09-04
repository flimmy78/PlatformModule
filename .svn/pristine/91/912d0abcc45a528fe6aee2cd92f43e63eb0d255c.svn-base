#ifndef _DCU_TO_AK_INTERFACE_H_
#define _DCU_TO_AK_INTERFACE_H_ 
#include "TypeDefine.h"
// 此文件包含了系统监控计算机与安控计算机的接口协议
// 宏编辑命令下发给ak时，宏命令中带有目标号，先将参数部分放入对应目标号参数去，调用目标切换命令
#pragma pack(1)
namespace dcu_ak
{     
// 参数部分参见TCK_TC_DataStruct.h

//--------------------------------
// 控制命令部分
//--------------------------------

// 目标选择
typedef struct _CurrentObjectSetStruct
{
    USHORT  ObjID;      //目标号
    ULONG   SetCount;   //设置计数
}CurrentObjectSetStruct, *PCurrentObjectSetStruct;

// 主备切换
typedef struct _AKWorkMachineSwitchStruct
{
    UCHAR  ucCtrl;   //1：A主B备 2：A备B主	
} AKWorkMachineSwitchStruct, *PAKWorkMachineSwitchStruct;

// 系统关机
typedef struct _AKSystemShutDownStruct
{
    UCHAR  ucCtrl;   //1：关机	
} AKSystemShutDownStruct, *PAKSystemShutDownStruct;

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


} // namespace dcu_ak

#pragma pack()
#endif // _DCU_TO_AK_INTERFACE_H_
