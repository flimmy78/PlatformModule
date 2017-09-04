#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#pragma pack(1)
typedef struct
{
    char filename[50];
    ushort fileNameLen;
    char fileData[20000];
    uint fileDataLen;
}TMTCMACRO;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_pSocket = new QUdpSocket();
}

MainWindow::~MainWindow()
{
    delete m_pSocket;
    delete ui;
}

void MainWindow::on_pushButton_start_clicked()
{
    m_pSocket->bind(QHostAddress::AnyIPv4, 8234, QAbstractSocket::ShareAddress);
    QHostAddress mcast_addr("225.225.0.225");
    m_pSocket->setSocketOption(QAbstractSocket::MulticastLoopbackOption, 0);
    m_pSocket->joinMulticastGroup(mcast_addr);
    connect(m_pSocket, SIGNAL(readyRead()), this, SLOT(ReadData()));
}

void MainWindow::ReadData()
{
    qDebug() << "hello world.";

    while (m_pSocket->hasPendingDatagrams())
    {
        //TMTCMACRO tmp;
        QByteArray data;
        data.resize(m_pSocket->pendingDatagramSize());
        m_pSocket->readDatagram(data.data(), data.size());
        qDebug() << data.size();
        //memcpy(&tmp, data.data(), data.size());
        QDir dir;
        //QByteArray tmpba;
//        tmpba.resize(tmp.fileNameLen);
//        memcpy(tmpba.data(), tmp.filename, tmp.fileNameLen);
//        QFile file(dir.currentPath() + "/" + tmpba.data());
//        qDebug() << dir.currentPath() + "/" + tmpba.data();
        QFile file(dir.currentPath() + "/status.txt");
        qDebug() << dir.currentPath() + "/status.txt" ;
        if (!file.open(QIODevice::ReadWrite))
        {
            //qDebug() <<QString(tmpba.data()) + "打开失败!";
            return;
        }
        //qDebug() << tmp.fileDataLen;
        file.write(data.data(), data.size());
        file.close();
    }
}
#pragma pack()
