#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QTimer>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QTimer *m_timer;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_send_clicked();

private:
    Ui::MainWindow *ui;
    QUdpSocket m_socket;     //测试使用的UDP通道
};


#endif // MAINWINDOW_H
