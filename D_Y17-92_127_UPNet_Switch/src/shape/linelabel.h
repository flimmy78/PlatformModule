#ifndef LINELABEL_H
#define LINELABEL_H

#include "drawlabel.h"

class LineLabel : public DrawLabel
{
    Q_OBJECT
public:
    enum LLType{
        CENTER = 0,
        TOP = 1,
        BOTTOM = 2
    };
    explicit LineLabel(LLType type, QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);

private:
    LLType m_lineType;

};

#endif // LINELABEL_H
