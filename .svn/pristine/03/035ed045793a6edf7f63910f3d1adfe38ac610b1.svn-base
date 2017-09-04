#include "menubutton.h"
#include <QMenu>

//TODO:把菜单按钮独立出一个组件出来，通过抽象菜单项的处理逻辑扩展
MenuButton::MenuButton(const QString &text, bool bShowMenu, QList<PARAM_CONTENT> actionList, QWidget *parent) :
    QPushButton(text, parent)
{
    if (bShowMenu)
    {
        createActions(actionList);
    }
}

void MenuButton::SetFontSize(int nSize)
{
    QFont font("Arial", nSize);
    setFont(font);
}

void MenuButton::mousePressEvent(QMouseEvent *event)
{
    QPushButton::mousePressEvent(event);
    if(event->button() != Qt::LeftButton)
        return;

    QMenu menu(this);
    for (int i = 0; i < m_ActionList.size(); i++)
    {
        menu.addAction(m_ActionList.at(i));
    }
    menu.exec(event->globalPos());
}

void MenuButton::createActions(QList<PARAM_CONTENT> actionList)
{
    for (int i = 0; i < actionList.size(); i++)
    {
        QString strContent;
        strContent = actionList.at(i).strValue + "---" + actionList.at(i).strDesc;
        QAction * item = new QAction(strContent, this);
        QObject::connect(item, SIGNAL(triggered(bool)), this, SLOT(changeButtonText()));
        m_ActionList.push_back(item);
    }
}

void MenuButton::changeButtonText()
{
    QString actionText = qobject_cast<QAction *>(this->sender())->text();
    if (actionText.isEmpty())
        return;
    //对Action中的数据进行解析
    QStringList strArr =  actionText.split("---");
    if (strArr.size() < 2)
        return;
    setText(strArr.at(0));
    emit checkedActionText(strArr.at(1));
}
