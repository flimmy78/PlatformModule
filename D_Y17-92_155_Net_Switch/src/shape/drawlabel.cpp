#include "drawlabel.h"
#include <QDialog>
#include <QPainter>

DrawLabel::DrawLabel(QWidget *parent) :
    QLabel(parent),
    m_pointSize(10),
    m_drawShapeWidth(0)
{
}

DrawLabel::DrawLabel(int pointSize, int drawShapeWidth,
                     const QString &text, QWidget *parent) :
    QLabel(text, parent),
    m_pointSize(pointSize),
    m_drawShapeWidth(drawShapeWidth)
{
}

void DrawLabel::paintEvent(QPaintEvent *)
{
    //绘制标签中间水平直线
    QPainter painter(this);
    QPolygon linePolyon;
    linePolyon.append(QPoint(0, this->height() / 2));
    linePolyon.append(QPoint(this->width() - m_drawShapeWidth,
                             this->height() / 2));
    painter.drawPolygon(linePolyon);
}

void DrawLabel::drawLabelText()
{
    static const QFont font("宋体", m_pointSize, QFont::Normal, false);
    QPainter painter(this);
    painter.save();
    painter.setFont(font);
    QFontMetrics fontMetrics(font);
    QRect fontRect = fontMetrics.boundingRect(this->text());
    painter.drawText(this->width() / 2 - fontRect.width() / 2,//水平居中
                     0 + fontMetrics.ascent(), this->text());
    painter.restore();
}
