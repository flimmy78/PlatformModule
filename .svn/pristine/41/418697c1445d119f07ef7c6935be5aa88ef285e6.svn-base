#ifndef CONNECTSTATUSCOLLECT_H
#define CONNECTSTATUSCOLLECT_H
// 该线程用来整理于各分机的链接状态
#include <QThread>
#include "../Common/Dcu_common_DataStruct.h"



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
    void run();
    bool bRunning() const;
    void setBRunning(bool bRunning);
private:
    void hasTimeChanged();
private:
    bool bRunning_;
    DevicesConnectStatus* pConnectSt_;

    uint timetag_[1024];
    bool isTimeChange_[1024];
};

}
#endif // CONNECTSTATUSCOLLECT_H
