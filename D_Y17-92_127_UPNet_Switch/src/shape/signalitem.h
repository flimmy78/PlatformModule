#ifndef SIGNALITEM_H
#define SIGNALITEM_H

#include <QPushButton>
#include <QWidget>
#include "linelabel.h"
#include "menubutton.h"
#include "arrowlabel.h"
#include "dotlabel.h"
#include "../XmlReader.h"

class SignalItem : public QWidget
{
    Q_OBJECT
public:
    explicit SignalItem(int height, bool bFix1, const QString &inputSignalName,
                        const QString &inputId, LineLabel::LLType type,
                        bool bFix2, const QString &outputId, const QString &outputSignalName,
                        QList<PARAM_CONTENT> paramList, QWidget *parent = 0);
    void setInfo(QString strIndex);
    QString getInfo();

    void SetLabelFontSize(int nSize = 10);

private:
    bool m_bLeftFix;      //左边是否使用弹出菜单
    bool m_bRightFix;   //右边是否使用弹出菜单
    QList<PARAM_CONTENT> m_MenuList;    //菜单显示的列白

    MenuButton *m_numInputBtn;  //输入的按钮
    MenuButton *m_numOutputBtn; //输出的按钮

    ArrowLabel *m_arrowInputLabel;  //输入显示的内容
    DotLabel *m_dotOutputLabel;         //输出显示的内容
};

#endif // SIGNALITEM_H
