#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QThread>
#include <QTimer>
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
    m_timer = new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(on_pushButton_send_clicked()));
    m_timer->start(1000);
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    m_timer->stop();
    delete m_timer;
    delete ui;
}

void MainWindow::on_pushButton_send_clicked()
{
    //    m_socket.bind(QHostAddress::AnyIPv4, 5050, QAbstractSocket::ShareAddress);
    QHostAddress mcast_addr("225.225.0.225");
    //    m_socket.setSocketOption(QAbstractSocket::MulticastLoopbackOption, 0);
    //    m_socket.joinMulticastGroup(mcast_addr);
    QByteArray tmp;
    QDir dir;
    QFile file(dir.currentPath() + "/status.txt");
    //qDebug() << dir.currentPath() + "/status.txt";
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "文件打开失败!";
        return;
    }

    tmp = file.read(43649);
    file.close();
//    tmp.append(2);
    m_socket. writeDatagram(tmp, mcast_addr, 5050);
    qDebug() << "ok";


}
#pragma pack()
