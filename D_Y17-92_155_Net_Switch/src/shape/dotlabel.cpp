#include "dotlabel.h"
#include <QPainter>

DotLabel::DotLabel(int pointSize, int dotDiameter,
                   const QString &text, QWidget *parent) :
    DrawLabel(pointSize, dotDiameter, text, parent)
{
}

void DotLabel::paintEvent(QPaintEvent *event)
{
    drawLabelText();
    DrawLabel::paintEvent(event);//绘制水平中间直线

    //绘制点
    QPainter painter(this);
    painter.setBrush(Qt::black);
    painter.setRenderHints(QPainter::Antialiasing);
    painter.drawEllipse(this->width() - m_drawShapeWidth,
                        (this->height() - m_drawShapeWidth) / 2,
                        m_drawShapeWidth, m_drawShapeWidth);
//    painter.drawEllipse(this->width() - m_drawShapeWidth/2, this->height()/2,
//                        m_drawShapeWidth/2, m_drawShapeWidth/2);
}
