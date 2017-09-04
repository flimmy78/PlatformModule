#include "searchmarc.h"
#include <QDebug>
#include <QMessageBox>
#include "Global.h"
#include "../../Common/Log.h"
SearchMarc::SearchMarc(QObject *parent) :
    QThread(parent)
{
    m_pRes = NULL;
    m_bStop = false;
    m_pause = false;
}
SearchMarc::~SearchMarc()
{
    m_bStop = true;
}

bool SearchMarc::init(PlatformResource *pRes)
{
    m_pRes = pRes;
    return true;
}

void SearchMarc::run()
{
    ITEM* pItem = m_pRes->m_devMap.getItem("宏查询命令");
    if(!pItem || pItem->vSubItem.empty())
    {
        CLog::addLog("配置文件中无数传状态查询命令. 请检查配置文件DeviceMapping.xml\n");
        return;
    }
    QVector<SUBITEM>& vSubItem = pItem->vSubItem;
    //return;//fix me;
    int sleepInterval = 1;
    static char seq = 1
            ;
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
                CLog::addLog("宏命令消息发送失败.\n");
                qDebug() << "宏命令消息发送失败.\n";
                continue;
            }
        }
        sleep(sleepInterval);
    }
}

void SearchMarc::setStop()
{
    m_bStop = true;
}


