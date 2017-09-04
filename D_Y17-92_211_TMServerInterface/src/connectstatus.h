#ifndef CONNECTSTATUSCOLLECT_H
#define CONNECTSTATUSCOLLECT_H
// 该线程用来整理于各分机的链接状态
#include <QThread>
#include "../../Common/Dcu_common_DataStruct.h"
#include "RecvDataThread_tmSrvr.h"


namespace DevCtl {

#pragma pack(1)
typedef struct _DevicesConnectInfoID
{
    uchar subsysID;
    ushort deviceID;
    ushort typeNO;
    uchar sn;
} DevicesConnectInfoID;
extern DevicesConnectInfoID devicesConnectInfo[];
#pragma pack()
class ConnectStatusCollect : public QThread
{
    Q_OBJECT
public:
    explicit ConnectStatusCollect(QObject *parent = 0);
    bool init(RecvDataThread* , PlatformResource* pRes);
    void run();
    bool bRunning() const;
    void setStop();
    RecvDataThread* m_pRevThread;
    DevicesConnectStatus* m_pConnectSt;
    PlatformResource* m_pPlatformRes;
private:
    void hasTimeChanged();
    void PutTheTimeChangeReusltToStatusInfo();
private:
    bool bRunning_;


    uint timetag_[1024];
    bool m_isTimeChange[1024];
};

}
#endif // CONNECTSTATUSCOLLECT_H
