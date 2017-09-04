#ifndef PROCESSMODULE_H
#define PROCESSMODULE_H
#include "../../Common/LxTsspCommonDefinition.h"
#include "../../Common/TypeDefine.h"
#include "../../Common/DeviceMap.h"
#include "DataStructTTCRemoteInterface.h"

//定义发送的最大缓存数据
#define MAX_SEND_BUFFER_LEN     1024

//定义发送结构体
typedef struct _tag_DataContent
{
    int iDataLen;           /// 数据长度，字节
    char* pData;            /// 数据内容
    SUBITEM subItem;
    int iObjType;
    int seq;
}DATACONTENT;

//
//typedef void (*PFProcessMsg)(DATACONTENT &);
//PFProcessMsg GetProcessFunction(const QString& objName);

//void ProcessSwitchMatrix(DATACONTENT& msg);
//void ProcessUSBBBE_CJParaSet(DATACONTENT& msg);
//void ProcessDigitalSignalSourceParamSet(DATACONTENT& msg);
//void ProcessDiversitySynthesisParamSet(DATACONTENT& msg);
//void ProcessDDT_BBESystemParamSet(DATACONTENT& msg);
//void ProcessDigitalDemodulationParamSet(DATACONTENT& msg);
//void ProcessDigitalReceiverParamSet(DATACONTENT& msg);
//void ProcessBasebandMonitor(DATACONTENT& msg);
//void ProcessBasebandMonitorDataTrans(DATACONTENT& msg);
//void ProcessGroupCMD(DATACONTENT& msg);
//void ProcessGroupDataTrans(ILxTsspParameterMacro* macroObj, DATACONTENT msg);
//void ProcessGroupTTC(ILxTsspParameterMacro* macroObj, DATACONTENT msg);
//void ProcessGroupSMCU1(ILxTsspParameterMacro* macroObj, DATACONTENT msg);
//void ProcessGroupSMCU2(ILxTsspParameterMacro* macroObj, DATACONTENT msg);
//void ProcessGroupTC(ILxTsspParameterMacro* macroObj, DATACONTENT msg);
//wl add 处理联试应答机宏处理相关函数
//void ProcessRSPMarcoSearch(DATACONTENT& msg);
//void ProcessRSPMarcoCall(DATACONTENT& msg);
//void ProcessRSPMarcoCmdCtrl(DATACONTENT& msg);
//wl end
class CHelper
{
public:
    static UINT getDate();
    static UINT getTime();
    static UINT getBCD(int iYear, int iMonth, int iDay);
    static void sendData(uchar* pPack, int packLen, QString channelID);
    static QString getXmlPath(QString xmlFileName);
    //中频开关矩阵监控接口协议的处理与发送;
    static QByteArray sendSwMatPro(DATACONTENT& msg);
    //联试应答机基协议的处理与发送;
    static QByteArray sendRSPData(DATACONTENT& msg);
    //基带监控协议的处理与发送;
    static QByteArray sendBaseBandPro(DATACONTENT& msg);
    static void initDataTrans(MSCDataTransGroup &dt);
    static void initTTC(USBTTCGroup& ttc);
    static void initSMCU(SSBBEGROUPPARA& smcu);
    static void initTC(TCGroup&);
    static qint64 getMacroIDFromModuleNameAndIndex(QString moduleName, int index);    
    static bool HasDevicesConnectStatus();
    static bool updateSubitemOfMsgForGroupCmd(DATACONTENT& msg, const int& basebandIndex);
};

#endif // PROCESSMODULE_H
