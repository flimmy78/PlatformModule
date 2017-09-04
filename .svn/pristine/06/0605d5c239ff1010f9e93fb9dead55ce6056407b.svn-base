#ifndef RECVDATATHREAD_H
#define RECVDATATHREAD_H

#include <QDateTime>
#include <QThread>
#include <QUdpSocket>
#include "../../Common/LxTsspCommonDefinition.h"
#include "../../Common/TypeDefine.h"
#include "headers.h"

#define MAX_RECV_BUFFER  (1024 * 8)     //最大数据长度 存在数据协议中有个包大概在7K左右

//通道处理以及函数结构体
typedef struct _tag_channelProcessor
{
    QString chlId;
    ICommunicationChannel *pSendChl;
    void (*fpProcessFunction)(QByteArray&);
}CHLPRSOR;
namespace TTCSrvr {
class RecvDataThread : public QThread
{
    Q_OBJECT
public:
    explicit RecvDataThread(QObject *parent = 0);
    ~RecvDataThread();

    void run();
    void setStop();
    bool init(PlatformResource& res);
    PlatformResource* m_pPlatformRes;
    bool setDevs(QByteArray&);
    bool getDevs(QByteArray&);
    SAllDataSource m_allDS;
public slots:
    void ReadData();

public:
    CBaseband m_bbe1;
    CBaseband m_bbe2;
    CBaseband m_bbe3;
    CNetSwitch m_netSwitchUp;
    CNetSwitch m_netSwitchDown;
    CAlter m_alterDown1;
    CAlter m_alterDown2;
    CAlter m_alterUp1;
    CAlter m_alterUp2;
    CAdjustAlter m_adjustAlter;
    CDevices m_devs;
private:
    //区分具体的协议 判断是协议1还是协议2
    int checkProcotol(QByteArray& datagram);
    QVector<ICommunicationChannel*> m_ComChannelVec;
    QUdpSocket * m_pSocket;     //测试使用的UDP通道
    BOOL m_bStop;   //停止线程变量使用
    UCHAR *m_pDataBuffer;   //用于接收数据使用的临时缓存
    SAllDataSource m_dataSource;

public:
    //初始化相关的通道
    bool initChlProcessor();
    //
    void processDatagram(QByteArray& datagram, QString chlID, ICommunicationChannel* pChl);
    //处理第1中协议数据
    void proceProcotol(QByteArray& datagram);
};
}


#endif // RECVDATATHREAD_H
