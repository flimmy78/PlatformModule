#include <QByteArray>

#ifndef CSHOWDATAGRAM_H
#define CSHOWDATAGRAM_H

class CShowDatagram
{
public:
    CShowDatagram();
    void setDatagramStatuRev();
    void setDatagramStatuSend();
    void setDatagramCommRev();
    void setDatagramCommSend();
    QByteArray m_datagramStatuRev;//状态查询
    QByteArray m_datagramStatuSend;//状态查询
    QByteArray m_datagramCommRev;//除状态查询
    QByteArray m_datagramCommSend;//除状态查询
    quint64 m_id;
    bool m_stopStatu;
    bool m_stopComm;

};

#endif // CSHOWDATAGRAM_H
