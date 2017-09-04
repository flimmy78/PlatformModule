#ifndef ALTER_H
#define ALTER_H

#include <QDateTime>
#include <QThread>
#include <QUdpSocket>
#include "../../Common/LxTsspCommonDefinition.h"
#include "../../Common/TypeDefine.h"
#include "../../Common/DataStruct2311.h"
#include "DataStructTMServerInterface.h"
#include "platformresource_tmSrvr.h"
#include "../../Common/Dcu_common_DataStruct.h"
#include "../../Common/interfaces/cdatasource.h"
#define MAX_RECV_BUFFER  (1024 * 8)     //最大数据长度 存在数据协议中有个包大概在7K左右

//变频器
class CAlter
{
public:
    CAlter(){m_connect = char(0);}
    bool init(QString moduleName, USHORT T1, USHORT devID, PlatformResource* res);
    PlatformResource* m_pPlatformRes;
    ALTER* m_pAlter;
    //中频开关矩阵模块处理函数
    void process(QByteArray& datagram);
    //处理响应结果
    void processResponse(QString strDeviceName, UCHAR ucAnswerFlag);
    QString getAnswerFlag(UCHAR ucAnsFlag);
    void set(SAlter&);
    void get(SAlter&);
    void getDevInfo(char& connect);
    char m_connect;//0未联接, 1联接
    QString m_moduleName;
    USHORT m_t1;
};

#endif // RECVDATATHREAD_H
