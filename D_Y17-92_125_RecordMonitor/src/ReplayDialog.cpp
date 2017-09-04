#include "ReplayDialog.h"
#include "ui_ReplayDialog.h"
#include "../../Common/DataStruct2311.h"
#include <QString>
#include <QMessageBox>
RePlayDialog::RePlayDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RePlayDialog)
{
    ui->setupUi(this);
    QRegExp regExp("[0-9]{1,7}");
    ui->lineEdit_TimeOffset->setValidator(new QRegExpValidator(regExp, this));
    ui->comboBox_Replay->addItem("循环");
    ui->comboBox_Replay->addItem("不循环");
    ui->comboBox_Replay->setCurrentIndex(0);
    m_bFlag=false;
}

RePlayDialog::~RePlayDialog()
{
    delete ui;
}

void RePlayDialog::setTime(unsigned int uTime)
{
    m_Time = uTime;
}

void RePlayDialog::on_pushButton_Ok_clicked()
{
    if(ui->lineEdit_TimeOffset->text().isEmpty()){
        QMessageBox msgBox;
        QString strTmp = QString("偏移时间不为空!");
        msgBox.setText(strTmp);
        msgBox.exec();
        return;
    }
    if(ui->lineEdit_TimeOffset->text().toInt() > m_Time)
    {
        QMessageBox msgBox;
        QString strTmp = QString("输入的偏移时间不能大于记录时长!");
        msgBox.setText(strTmp);
        msgBox.exec();
        return;
    }
    m_Time = ui->lineEdit_TimeOffset->text().toUInt();
    m_bRepeatReplay = ui->comboBox_Replay->currentIndex() == 0 ? true : false;
    m_bFlag=true;
    accept();
}

void RePlayDialog::on_pushButton_Back_clicked()
{
    this->hide();
}
