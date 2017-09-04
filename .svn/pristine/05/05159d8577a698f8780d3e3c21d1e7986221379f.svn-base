#include "RecvDataThread.h"
#include <time.h>
#include <sys/time.h>
#include "Global.h"
#include <QtEndian>
#include "../../Common/Log.h"
#include "DataStructTTCDevCtlInterface.h"
#include "DevCtlInterface_global.h"
#include "util.h"
#include "alter.h"

bool CAlter::init(QString moduleName,USHORT T1, USHORT devID, PlatformResource* res)
{
    m_t1 = T1;
    m_moduleName = moduleName;
    m_pPlatformRes = res;
    DATA_SOURCE_ALLOC(ALTER, m_pAlter, moduleName, devID, 0);
    return true;
}
QString CAlter::getAnswerFlag(UCHAR ucAnsFlag)
{
    QString strAnswer("未知应答标识");
    switch (ucAnsFlag)
    {
    case 0xFF:
        strAnswer = QString("正常执行");
        break;
    case 0x61:
        strAnswer = QString("非法码");
        break;
    case 0x62:
        strAnswer = QString("帧校验出错");
        break;
    case 0x63:
        strAnswer = QString("条件不具备不执行");
        break;
    case 0x64:
        strAnswer = QString("超限码，参数超限");
        break;
    default:
        break;
    }
    return strAnswer;
}
void CAlter::process(QByteArray& datagram)
{
    ++m_connect;
    PROCOTOL_PACK proPack;
    memcpy(&proPack, datagram.data(), datagram.size());
    QString strInfo;
    //工作状态上报
    if(0x0701 == proPack.C && m_t1 == proPack.Data.T1)
    {
        memcpy(m_pAlter, &proPack.Data.Dat1.alter, proPack.Data.L1);
    }
    //处理中频开关矩阵响应信息
    else if (0x200 == proPack.C && 0xE498 == proPack.Data.T1)
    {
        UCHAR ucResponse = (UCHAR)proPack.Data.Dat1.answer;
        processResponse(m_moduleName, ucResponse);
    }
    else
    {
        strInfo = QString("%1: 收到未知协议报文!").arg(m_moduleName);
        CLog::addLog(strInfo);
    }
}
void CAlter::processResponse(QString strDeviceName, UCHAR ucAnswerFlag)
{
    QString strType("返回结果");
    QString strAnswer = getAnswerFlag(ucAnswerFlag);
    QString strInfo = QString("%1 %2: %3!").arg(strDeviceName, strType, strAnswer);
    CLog::addLog(strInfo);
}
void CAlter::getDevInfo(char& connect)
{
    connect = m_connect == m_connectOld ? char(0) : char(1);
    m_connectOld = m_connect;
}



