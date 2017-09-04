#ifndef SEARCHSTATUSTHREAD_H
#define SEARCHSTATUSTHREAD_H

#include <QThread>
#include "../../Common/LxTsspCommonDefinition.h"
#include "platformresource.h"
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

    bool init(PlatformResource* pRes);
private:
    PlatformResource* m_pRes;
    bool m_bStop;
    bool m_pause;
};

#endif // SEARCHSTATUSTHREAD_H
