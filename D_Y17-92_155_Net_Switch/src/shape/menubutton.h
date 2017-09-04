#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <QPushButton>
#include <QMouseEvent>
#include <QAction>
#include <QList>
#include "../XmlReader.h"

class MenuButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MenuButton(const QString &text, bool bShowMenu, QList<PARAM_CONTENT> actionList, QWidget *parent=0);

signals:
    void checkedActionText(QString text);

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    void createActions(QList<PARAM_CONTENT> actionList);

    QList<QAction*> m_ActionList;
    bool m_bShowMenu;

private slots:
    void changeButtonText();
};

#endif // MENUBUTTON_H
