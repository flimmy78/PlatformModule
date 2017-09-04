#ifndef DOTLABEL_H
#define DOTLABEL_H

#include "drawlabel.h"

class DotLabel : public DrawLabel
{
    Q_OBJECT
public:
    explicit DotLabel(int pointSize, int dotDiameter,
                      const QString &text, QWidget *parent = 0);

    void SetFontSize(int nSize = 10);

protected:
    void paintEvent(QPaintEvent *event);

    int m_nFontSize;
};

#endif // DOTLABEL_H
