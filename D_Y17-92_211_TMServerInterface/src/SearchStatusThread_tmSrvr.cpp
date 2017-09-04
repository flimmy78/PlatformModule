#include "SearchStatusThread_tmSrvr.h"
#include <QDebug>
#include <QMessageBox>
#include "Global.h"
#include "../../Common/Log.h"
SearchStatusThread::SearchStatusThread(QObject *parent) :
    QThread(parent)
{
    m_bStop = false;
    m_pause = false;
    m_pThread = NULL;
}

SearchStatusThread::~SearchStatusThread()
{
    m_bStop = true;
}

bool SearchStatusThread::init(PlatformResource& res, RecvDataThread& thread)
{
    m_pPlatformRes = &res;
    m_pThread = &thread;
    return true;
}
void SearchStatusThread::run()
{
    return;
    if(!m_pThread)
    {
        CLog::addLog("数据源获取失败。线程退出。");
        return;
    }

    QList<ICommunicationChannel*> chls;
    bool ok = m_pPlatformRes->getChlsUsedByDevCtl(chls);
    if(!ok || !chls.size())
    {
        CLog::addLog("状态查询时，通道获取失败。");
        return;
    }
    const int msleepInterval = 500;
    static USHORT seq = 0;
    QByteArray buff;
    while(1)
    {
        if(m_pause)
        {
            msleep(msleepInterval);
            continue;
        }
        if (m_bStop)
            break;
        seq++;
        QList<ICommunicationChannel*>::iterator iter = chls.begin(), iterEnd = chls.end();
        buff.clear();
        m_pThread->getDevs(buff);
        for(; iter != iterEnd; ++iter)
        {
            if(!buff.size())continue;
            int ret1_1 = (*iter)->SendData((uchar*)buff.data(), buff.size());
            if(-1 == ret1_1)
            {
                CLog::addLog("状态查询命令消息发送失败.\n");
                qDebug() << "状态查询命令消息发送失败.\n";
                continue;
            }
        }
        sleep(2);
        msleep(msleepInterval);
    }
}

void SearchStatusThread::setStop()
{
    m_bStop = true;
    wait();
}

