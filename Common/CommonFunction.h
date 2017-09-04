#ifndef _COMMOM_FUNCTION_H_
#define _COMMOM_FUNCTION_H_

#include "TypeDefine.h"
#include "Dcu_common_DataStruct.h"
#include "Log.h"
#include "DeviceMap.h"
#include "LxTsspCommonDefinition.h"

#include <QLineEdit>

#define DEVICE_STATUS_NAME       ("设备联接状态")
const int c_buffSize = 1024 * 4;//4K
typedef struct _tag_S_TMP_BUFF
{
    char buff[c_buffSize];
    int size;
}S_TMP_BUFF;
class CCommFunc
{
protected:
    CCommFunc();
    ~CCommFunc();

public:
    static CCommFunc * getInstance();
    static bool exchange(const QString& buf, UINT& v);
    static void exchange(const UINT& v, QString& buf);
    static void lineEditPrecision(const QString& arg, QLineEdit* pLineEdit, int prec);

    //获取所有设备连接状态值
    DevicesConnectStatus * getAllDeviceConnStatus();

    bool getAKType(int nType); // ak[0]: ak_A ak[1]:ak_B, 0 未链接  1:FM 2:ZZM
    bool getACUStatus();       // 0 未链接, 1链接
    bool getDTEStatus();       // 0 未链接, 1链接
    bool getTimerStatus();
    bool getUpnet_switchStatus();//dev监控
    bool getDownNet_switchStatus();//dev监控
    bool getTestNet_switchStatus();//dev监控
    bool getSLDivcomStatus();//dev监控
    bool getSswitchStatus();//dev监控
    bool getDown_alterStatus(int nType); //dev监控*
    bool getUp_alterStatus(int nType);//dev监控*
    bool getAdjustAp_alterStatus();//dev监控*
    bool getFreqBStatus();//dev监控
    bool getZeroSTDStatus();//dev监控
    bool getUSBRSPStatus();//dev监控
    bool getSS2RSPStatus();//dev监控
    bool getSPowerStatus();//dev监控*
    bool getLPowerStatus();//dev监控*
    bool getTestUStatus();//dev监控*
    bool getrClockStatus();//dev监控
    bool getbbeStatus(int nType); //dev监控   // 0 未链接  1:FM 2:BPSK 4:SS
    bool getbbeControlStatus();//基带是本控(1)还是分控(0)
    bool getRecordStatus(int nType);//dev监控* // 检前1，2，3：0未链接 1链接//
    bool getSpectrumStatus();  // 频谱仪

    //初始化DeviceMap.xml文件中的内容
    bool initDeviceMap();

    int getTargetNum(bool* pOk = NULL);//获取公共单元目标数
private:
    bool m_initIsOk;
    //初始化工作
    bool init();
    //获取设备状态数据元id
    qint64 getDevicesID();

private:
    CDeviceMap m_DeviceMap;
    qint64 m_DeviceStatusSyncID;        //设备状态数据元ID

    DevicesConnectStatus * m_pDevConnStatus;

    ILxTsspStateManager * m_pStateMgr;
};

#endif // _COMMOM_FUNCTION_H_

