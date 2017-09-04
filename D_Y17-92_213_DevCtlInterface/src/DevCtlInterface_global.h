#ifndef TTCDEVCTLINTERFACE_GLOBAL_H
#define TTCDEVCTLINTERFACE_GLOBAL_H

#include <QtCore/qglobal.h>
//#pragma execution_character_set("utf-8")

#if defined(TTCDEVCTLINTERFACESLATE_LIBRARY)
#  define TTCDEVCTLINTERFACESHARED_EXPORT Q_DECL_EXPORT
#else
#  define TTCDEVCTLINTERFACESHARED_EXPORT Q_DECL_IMPORT
#endif

//将程序中用到的一些常量组织成宏, 以便后续的维护.
#define DATA_SOURCE_ERR "信息源申请失败.\n"
//begin 模块名字.
#define NET_SWITCH "中频开关矩阵"
//end 模块名字.

//begin 模块全局变量 global context
#define GC_INIT_PRO_XML_ERR "测控分机设备接口模块内部协议XML加载错误!请查看\"TTCDevCtlInterfacePro.xml\"是否存在."
//end 模块全局变量

//begin 过程控制命令用到的宏PCP: process control response
#define PCR_START "开始测试"
#define PCR_FINISH "测试完成"
#define PCR_STOP "终止测试"

#define PCR_TYPE_1 "遥测"
#define PCR_TYPE_2 "数传I(I+Q)路(低速)"
#define PCR_TYPE_3 "数传Q路(低速)"
#define PCR_ROTATION_1 "左旋"
#define PCR_ROTATION_2 "右旋"
#define PCR_STATUS_1 "开始统计"
#define PCR_STATUS_2 "正在统计"
#define PCR_STATUS_3 "终止统计"
#define PCR_TOTAL_COD "总码元数: "
#define PCR_ERR_COD "误码元数: "
#define PCR_TOTAL_FRM "总帧数: "
#define PCR_ERR_FRM "误帧数: "

#define PCR_WORK_WAY_1 "任务方式"
#define PCR_WORK_WAY_2 "联试应答机无线闭环"
#define PCR_WORK_WAY_3 "联试应答机有线闭环"
#define PCR_WORK_WAY_4 "校零变频器无线闭环"
#define PCR_WORK_WAY_5 "校零变频器有线闭环"
#define PCR_WORK_WAY_6 "偏馈校零变频器闭环"
#define PCR_WORK_WAY_7 "射频模拟源有线闭环"
#define PCR_WORK_WAY_8 "信标方式"
#define PCR_WORK_WAY_9 "中频闭环"
#define PCR_WORK_WAY_10 "记录回放"
#define PCR_WORK_WAY_11 "其他方式"
#define PCR_WORK_WAY_12 "扩跳中频动态模拟"
#define PCR_WORK_WAY_13 "扩跳射频(联试应答机闭环)动态模拟"
#define PCR_WORK_WAY_14 "扩跳射频(校零变频器闭环)动态模拟"
//end 过程控制命令用到的宏
#define MODULE "DevCtlInterface"
//begin 日志
#define NET_SWITCH_CHL_GET_ERR "中频矩阵channel名字获取失败, 请查看配置文件DeviceMapping.xml.\n"
#define PC_INFO_SOURCE_ERR "过程控制上报信息源申请失败.\n"
//end 日志
#endif // TTCDEVCTLINTERFACE_GLOBAL_H


