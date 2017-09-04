#ifndef CUDPTHREAD_H
#define CUDPTHREAD_H

#include <QThread>
#include <QUdpSocket>
class CUdpThread : public QThread
{
    Q_OBJECT
public:
    explicit CUdpThread(QObject *parent = 0);
    ~CUdpThread();
    void run();
private:
    QUdpSocket * m_pSocket;     //测试使用的UDP通道
signals:

public slots:
    void ReadData();
};

#endif // CUDPTHREAD_H
