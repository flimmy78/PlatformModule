#ifndef DOTLABEL_H
#define DOTLABEL_H

#include "drawlabel.h"

class DotLabel : public DrawLabel
{
    Q_OBJECT
public:
    explicit DotLabel(int pointSize, int dotDiameter,
                      const QString &text, QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);

};

#endif // DOTLABEL_H
