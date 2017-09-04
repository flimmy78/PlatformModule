#ifndef ARROWLABEL_H
#define ARROWLABEL_H

#include "drawlabel.h"

class ArrowLabel : public DrawLabel
{
    Q_OBJECT
public:
    explicit ArrowLabel(int pointSize, int triangleWidth, int triangleHeight,
                        const QString &text, QWidget *parent = 0);

    void SetFontSize(int nSize = 10);

protected:
    void paintEvent(QPaintEvent *event);

private:
    int m_triangleHeight;
    int m_nFontSize;
};

#endif // ARROWLABEL_H
