#ifndef RECORDDIALOG_H
#define RECORDDIALOG_H

#include <QDialog>
#include "../../Common/TypeDefine.h"
#include "RecordMonitorGUIDlg.h"
#include <QString>
namespace Ui {
class RecordDialog;
}

class RecordDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RecordDialog(ushort id,RecordMonitorGUIDlg *parent = 0);
    ~RecordDialog();
    QString m_strFileName;
    short m_Length;
    QString m_strComment;
    short m_AppLen;
    BYTE m_RecordType;
    BYTE m_TimeType;
    BYTE m_Channal_1;
    BYTE m_Channal_2;
    BYTE m_Channal_3;
    BYTE m_Channal_4;
    bool m_bFlag;
    ushort m_usID;
private slots:
    void on_pushButton_Ok_clicked();
    void on_pushButton_Back_clicked();

private:
    bool readConfigXml();
    RecordMonitorGUIDlg* m_pParent;
    Ui::RecordDialog *ui;
};

#endif // RECORDDIALOG_H
