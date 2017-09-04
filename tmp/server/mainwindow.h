#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cudpthread.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_pushButton_start_clicked();
    void ReadData();

private:
    Ui::MainWindow *ui;
    CUdpThread m_serverThread;
    QUdpSocket * m_pSocket;     //测试使用的UDP通道
};

#endif // MAINWINDOW_H
