#include "SearchStatusThread_tmremote.h"
#include <QDebug>
#include <QMessageBox>
#include "Global.h"
#include "../../Common/Log.h"
SearchStatusThread::SearchStatusThread(QObject *parent) :
    QThread(parent)
{
    m_pRes = NULL;
    m_bStop = false;
    m_pause = false;
}

SearchStatusThread::~SearchStatusThread()
{
    m_bStop = true;
}

bool SearchStatusThread::init(PlatformResource *pRes)
{
    m_pRes = pRes;
    return true;
}

void SearchStatusThread::run()
{
    ITEM* pItem = m_pRes->m_devMap.getItem("Remote状态查询命令");
    if(!pItem || pItem->vSubItem.empty())
    {
        CLog::addLog("配置文件中无数传状态查询命令. 请检查配置文件DeviceMapping.xml\n");
        return;
    }
    QVector<SUBITEM>& vSubItem = pItem->vSubItem;
    //return;//fix me;
    int sleepInterval = 2;
    static char seq = 2;
    while(1)
    {
        if(m_pause)
        {
            sleep(sleepInterval);
            continue;
        }
        if (m_bStop)
            break;

        QVector<SUBITEM>::iterator iter = vSubItem.begin();
        QVector<SUBITEM>::const_iterator iterEnd = vSubItem.end();

        for(; iter != iterEnd; ++iter)
        {
            int ret1_1 = SendCmdMessage((char*)&seq, 1, iter->usCMD, iter->usTID);
            if(-1 == ret1_1)
            {
                CLog::addLog("状态查询命令消息发送失败.\n");
                qDebug() << "状态查询命令消息发送失败.\n";
                continue;
            }
        }
        sleep(sleepInterval);
    }
}

void SearchStatusThread::setStop()
{
    m_bStop = true;
}



