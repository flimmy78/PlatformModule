#ifndef SEARCHSTATUSTHREAD_H
#define SEARCHSTATUSTHREAD_H

#include <QThread>
#include "../../Common/LxTsspCommonDefinition.h"
#include "platformresource_tmSrvr.h"
#include "../../Common/CommonFunction.h"
#include "RecvDataThread_tmSrvr.h"
//添加状态查询线程，用于查询设备状态使用
class SearchStatusThread : public QThread
{
    Q_OBJECT
public:
    explicit SearchStatusThread(QObject *parent = 0);
    ~SearchStatusThread();

    virtual void run();
    void setStop();
    void pause(){m_pause = true;}
    void continu(){m_pause = false;}
    bool init(PlatformResource&, RecvDataThread&);
private:
    RecvDataThread* m_pThread;
    bool m_bStop;
    bool m_pause;
    PlatformResource* m_pPlatformRes;
    S_TMP_BUFF* m_pBuff;
};

#endif // SEARCHSTATUSTHREAD_H
