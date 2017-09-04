#ifndef ALTER_H
#define ALTER_H

#include <QDateTime>
#include <QThread>
#include <QUdpSocket>
#include "../../Common/LxTsspCommonDefinition.h"
#include "../../Common/TypeDefine.h"
#include "../../Common/DataStruct2311.h"
#include "DataStructTTCDevCtlInterface.h"
#include "platformresource.h"
#include "cbaseband.h"
#include "../../Common/Dcu_common_DataStruct.h"
#define MAX_RECV_BUFFER  (1024 * 8)     //最大数据长度 存在数据协议中有个包大概在7K左右

//变频器
class CAlter
{
public:
    CAlter(){m_connectOld = m_connect = 0;}
    bool init(QString moduleName, USHORT T1, USHORT devID, PlatformResource* res);
    PlatformResource* m_pPlatformRes;
    ALTER* m_pAlter;
    //中频开关矩阵模块处理函数
    void process(QByteArray& datagram);
    //处理响应结果
    void processResponse(QString strDeviceName, UCHAR ucAnswerFlag);
    QString getAnswerFlag(UCHAR ucAnsFlag);

    void getDevInfo(char& connect);
    //m_connect == m_connectOld ? 未链接 : 链接
    int m_connect;//自增
    int m_connectOld;//保存m_connect
    QString m_moduleName;
    USHORT m_t1;
};

#endif // RECVDATATHREAD_H
