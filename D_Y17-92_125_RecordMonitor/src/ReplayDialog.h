#ifndef REPLAYDIALOG_H
#define REPLAYDIALOG_H

#include <QDialog>

namespace Ui {
class RePlayDialog;
}

class RePlayDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RePlayDialog(QWidget *parent = 0);
    ~RePlayDialog();

public:
    void setTime(unsigned int uTime);

private slots:
    void on_pushButton_Ok_clicked();
    void on_pushButton_Back_clicked();

public:
    unsigned int m_Time;
    bool m_bRepeatReplay;
    bool m_bFlag;

private:
    Ui::RePlayDialog *ui;
};

#endif // REPLAYDIALOG_H
