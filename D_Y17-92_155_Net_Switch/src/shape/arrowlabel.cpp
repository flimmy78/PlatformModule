#include "arrowlabel.h"
#include <QPainter>

ArrowLabel::ArrowLabel(int pointSize, int triangleWidth, int triangleHeight,
                       const QString &text, QWidget *parent) :
    DrawLabel(pointSize, triangleWidth, text, parent),
    m_triangleHeight(triangleHeight)
{
}

void ArrowLabel::paintEvent(QPaintEvent *event)
{
    drawLabelText();
    DrawLabel::paintEvent(event);//绘制水平中间直线

    //绘制三角形
    static const int s_LINE_END_Y = this->height() / 2;
    static const int s_LINE_END_X = this->width() - m_drawShapeWidth;
    QPainter painter(this);
    QPolygon trianglePolygon;
    trianglePolygon.append(QPoint(s_LINE_END_X, s_LINE_END_Y - m_triangleHeight / 2));
    trianglePolygon.append(QPoint(s_LINE_END_X, s_LINE_END_Y + m_triangleHeight / 2));
    trianglePolygon.append(QPoint(this->width(), s_LINE_END_Y));
    painter.setBrush(Qt::black);
    painter.drawPolygon(trianglePolygon);
}
