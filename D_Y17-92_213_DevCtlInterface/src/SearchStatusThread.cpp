#include "SearchStatusThread.h"
#include <QDebug>
#include <QMessageBox>
#include "Global.h"
#include "../../Common/Log.h"
SearchStatusThread::SearchStatusThread(QObject *parent) :
    QThread(parent)
{
    m_bStop = false;
    m_pause = false;
}

SearchStatusThread::~SearchStatusThread()
{
    m_bStop = true;
}

void SearchStatusThread::run()
{
    ITEM* pItem = g_context.m_devMap.getItem("状态查询命令");

    if(!pItem || pItem->vSubItem.empty())
    {
        CLog::addLog("配置文件中无数传状态查询命令. 请检查配置文件DeviceMapping.xml\n");
        return;
    }
    ITEM* pRMItem = g_context.m_devMap.getItem("检前状态查询命令");
    if(!pRMItem || pRMItem->vSubItem.empty())
    {
        CLog::addLog("配置文件中无数传状态查询命令. 请检查配置文件DeviceMapping.xml\n");
        return;
    }
    QVector<SUBITEM>& vSubItem = pItem->vSubItem;
    QVector<SUBITEM>& rmVSubItem = pRMItem->vSubItem;
    //return;//fix me;
    int sleepInterval = 1;
    static USHORT seq = 0;
    //return; //fix me;
    while(1)
    {
        if(m_pause)
        {
            sleep(sleepInterval);
            continue;
        }
        if (m_bStop)
            break;
        seq++;
        QVector<SUBITEM>::iterator iter = vSubItem.begin();
        QVector<SUBITEM>::iterator iterEnd = vSubItem.end();
        for(; iter != iterEnd; ++iter)
        {
            int ret1_1 = SendCmdMessage((char*)&seq, 2, iter->usCMD, iter->usTID);
            if(-1 == ret1_1)
            {
                CLog::addLog("状态查询命令消息发送失败.\n");
                qDebug() << "状态查询命令消息发送失败.\n";
                continue;
            }
        }
        iter = rmVSubItem.begin();
        iterEnd = rmVSubItem.end();
        BYTE search = 0x32;
        for(; iter != iterEnd; ++iter)
        {
            int ret1_1 = SendCmdMessage((char*)&search, 1, iter->usCMD, iter->usTID);
            if(-1 == ret1_1)
            {
                CLog::addLog("状态查询命令消息发送失败.\n");
                qDebug() << "检前状态查询命令消息发送失败.\n";
                continue;
            }
        }
        sleep(sleepInterval);
    }
}

void SearchStatusThread::setStop()
{
    m_bStop = true;
    wait();
}

