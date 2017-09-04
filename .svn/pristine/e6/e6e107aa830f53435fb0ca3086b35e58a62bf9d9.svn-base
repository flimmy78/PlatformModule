#ifndef ARROWLABEL_H
#define ARROWLABEL_H

#include "drawlabel.h"

class ArrowLabel : public DrawLabel
{
    Q_OBJECT
public:
    explicit ArrowLabel(int pointSize, int triangleWidth, int triangleHeight,
                        const QString &text, QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);

private:
    int m_triangleHeight;

};

#endif // ARROWLABEL_H
