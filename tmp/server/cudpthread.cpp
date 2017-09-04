#include "cudpthread.h"

CUdpThread::CUdpThread(QObject *parent) :
    QThread(parent)
{
    m_pSocket = new QUdpSocket(this);
}

CUdpThread::~CUdpThread()
{

}

void CUdpThread::ReadData()
{
    qDebug()<<"hello world.";
}

void CUdpThread::run()
{
    m_pSocket->bind(QHostAddress::AnyIPv4, 5050, QAbstractSocket::ShareAddress);
    connect(m_pSocket, SIGNAL(readyRead()), this, SLOT(ReadData()));
    exec();
}
