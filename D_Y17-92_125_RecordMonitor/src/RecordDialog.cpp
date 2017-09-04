#include "RecordDialog.h"
#include "ui_recorddialog.h"
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
RecordDialog::RecordDialog(ushort id,RecordMonitorGUIDlg *parent) :
    QDialog(parent),
    ui(new Ui::RecordDialog)
{
    ui->setupUi(this);
    m_pParent = parent;
    m_bFlag = false;
    QRegExp regExp("[0-9]{1,4}");
    //码速率
    QRegExp regExp_Len("[A-Za-z0-9]{1,64}");
    ui->lineEdit_Length->setValidator(new QRegExpValidator(regExp, this));
    ui->lineEdit_APPD_LEN->setValidator(new QRegExpValidator(regExp, this));

    //ui->lineEdit_FileName->setValidator(new QRegExpValidator(regExp_Len, this));
    //ui->lineEdit_Comment->setValidator(new QRegExpValidator(regExp_Len, this));
    ui->comboBox_Channal_1->addItem("无效");
    ui->comboBox_Channal_1->addItem("有效");
    ui->comboBox_Channal_1->setCurrentIndex(1);
    ui->comboBox_Channal_1->hide();
    ui->comboBox_Channal_2->addItem("无效");
    ui->comboBox_Channal_2->addItem("有效");
    ui->comboBox_Channal_2->setCurrentIndex(1);
    ui->comboBox_Channal_2->hide();
    ui->comboBox_Channal_3->addItem("无效");
    ui->comboBox_Channal_3->addItem("有效");
    ui->comboBox_Channal_3->setCurrentIndex(1);
    ui->comboBox_Channal_3->hide();
    ui->comboBox_Channal_4->addItem("无效");
    ui->comboBox_Channal_4->addItem("有效");
    ui->comboBox_Channal_4->setCurrentIndex(1);
    ui->comboBox_Channal_4->hide();
    ui->comboBox_RecordType->addItem("立即记录");
    ui->comboBox_RecordType->addItem("定时记录");
    ui->comboBox_RecordType->setCurrentIndex(0);
    ui->comboBox_TimeType->addItem("系统时");
    ui->comboBox_TimeType->addItem("B码");
    ui->comboBox_TimeType->setCurrentIndex(0);
    ui->groupBox->hide();
    m_Length = 0;
    m_AppLen = 0;
    m_usID = id;
    readConfigXml();
}

bool RecordDialog::readConfigXml()
{
    QDir dir;
    QString strPath = dir.currentPath();
    QString filePath = strPath + "/TM/System/ModuleConfig/RecordMonitor.xml";
    qDebug()<<filePath;
    QFile file(filePath);
    if  (!file.open(QIODevice::ReadOnly))
        return false;
    QDomDocument doc;
    if  (!doc.setContent(&file))
    {
        file.close();
        return false;
    }
    file.close();
    QDomNode node = doc.documentElement();
    QDomNodeList nodeList = node.childNodes();
    int iCount = nodeList.count();
    for(int i = 0;i<iCount;i++)
    {
        QDomElement tmpNode = nodeList.at(i).toElement();
        QString nodeName = tmpNode.tagName();
        if(nodeName.toLower().compare("mode") != 0)
            continue;
        if (tmpNode.hasAttribute("id"))
        {
            ushort id = tmpNode.attribute("id").toUShort(0, 16);
            if(id!=m_usID)
                continue;
            QDomNodeList ctrlList = tmpNode.childNodes();
            for(int j = 0;j<ctrlList.count();j++)
            {
                QDomElement tmpCtrl = ctrlList.at(j).toElement();
                nodeName = tmpCtrl.tagName();
                if(nodeName.toLower().compare("ctrl") != 0)
                    continue;
                if (tmpCtrl.hasAttribute("objectName"))
                {
                    QString objectName = tmpCtrl.attribute("objectName");
                    QComboBox* pComboBox = this->findChild<QComboBox*>(objectName);
                    if(pComboBox != 0)
                    {
                        QRect rect;
                        QString strValue;
                        bool isShow;
                        bool isEnabled;
                        if (tmpCtrl.hasAttribute("geometry"))
                            strValue = tmpCtrl.attribute("geometry");
                        QStringList posList = strValue.split(",");
                        Q_ASSERT_X(posList.count() == 4,"RecordDialog","geometry count error");
                        rect.setRect(posList[0].toInt(),posList[1].toInt(),
                                posList[2].toInt(),posList[3].toInt());
                        pComboBox->setGeometry(rect);
                        if (tmpCtrl.hasAttribute("isShow"))
                            isShow =(bool)tmpCtrl.attribute("isShow").toInt();
                        pComboBox->setHidden(!isShow);
                        if (tmpCtrl.hasAttribute("isEnabled"))
                            isEnabled =(bool)tmpCtrl.attribute("isEnabled").toInt();
                        pComboBox->setEnabled(isEnabled);
                    }
                }
            }
        }
    }
}
RecordDialog::~RecordDialog()
{
    delete ui;
}

void RecordDialog::on_pushButton_Ok_clicked()
{
    if(ui->lineEdit_FileName->text().isEmpty()){
        QMessageBox msgBox;
        QString strTmp = QString("文件名不为空!");
        msgBox.setText(strTmp);
        msgBox.exec();
        return;
    }
    /*if(ui->lineEdit_Length->text().isEmpty()){
        QMessageBox msgBox;
        QString strTmp = QString("定时记录时间长度不为空!");
        msgBox.setText(strTmp);
        msgBox.exec();
        return;
    }
    if(ui->lineEdit_APPD_LEN->text().isEmpty()){
        QMessageBox msgBox;
        QString strTmp = QString("附加记录信息长度不为空!");
        msgBox.setText(strTmp);
        msgBox.exec();
        return;
    }*/
    /*if(ui->lineEdit_Comment->text().isEmpty()){
        QMessageBox msgBox;
        QString strTmp = QString("备注不为空!");
        msgBox.setText(strTmp);
        msgBox.exec();
        return;
    }*/
    if(ui->lineEdit_Comment->text().length() > 50)
    {
        QMessageBox msgBox;
        QString strTmp = QString("备注内容过长，请重新输入!");
        msgBox.setText(strTmp);
        msgBox.exec();
        return;
    }

    m_strFileName = ui->lineEdit_FileName->text();
    if(m_pParent->fileExists(m_strFileName)){
        QMessageBox msgBox;
        msgBox.setStandardButtons(QMessageBox::Ok|QMessageBox::Cancel);
        QString strTmp = QString("文件已存在，是否覆盖？");
        msgBox.setText(strTmp);
        if(QMessageBox::Cancel == msgBox.exec())return;
    }
    //m_Length = ui->lineEdit_Length->text().toShort();
    m_strComment = ui->lineEdit_Comment->text();
    //m_AppLen = ui->lineEdit_APPD_LEN->text().toShort();
    m_RecordType = ui->comboBox_RecordType->currentIndex();
    m_TimeType = ui->comboBox_TimeType->currentIndex();
    m_Channal_1 = ui->comboBox_Channal_1->currentIndex();
    m_Channal_2 = ui->comboBox_Channal_2->currentIndex();
    m_Channal_3 = ui->comboBox_Channal_3->currentIndex();
    m_Channal_4 = ui->comboBox_Channal_4->currentIndex();
    m_bFlag = true;
    accept();
}

void RecordDialog::on_pushButton_Back_clicked()
{
    this->hide();
}


