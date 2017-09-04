#include "linelabel.h"
#include <QPainter>

LineLabel::LineLabel(LineLabel::LLType type, QWidget *parent) :
    DrawLabel(parent),
    m_lineType(type)
{
}

void LineLabel::paintEvent(QPaintEvent *event)
{
    DrawLabel::paintEvent(event);//绘制水平中间直线

    //绘制垂直中间直线
    QPainter painter(this);
    QPolygon vCenterLine;
    if(m_lineType == TOP){
        vCenterLine.append(QPoint(this->width() / 2, this->height() / 2));
        vCenterLine.append(QPoint(this->width() / 2, this->height()));
    }
    else if(m_lineType == BOTTOM){
        vCenterLine.append(QPoint(this->width() / 2, 0));
        vCenterLine.append(QPoint(this->width() / 2, this->height() / 2));
    }
    else{
        vCenterLine.append(QPoint(this->width() / 2, 0));
        vCenterLine.append(QPoint(this->width() / 2, this->height()));
    }
    painter.drawPolygon(vCenterLine);
}
