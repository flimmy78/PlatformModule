#include "Matrix.h"
#include <QPainter>
#include <QPen>
#include <QDebug>

Matrix::Matrix(int nNum, QWidget *parent) : QWidget(parent)
{
    m_nNum = nNum;
}

Matrix::~Matrix()
{
}

void Matrix::SetShowType(QString strType, QString strStatus)
{
    m_strType = strType;
    m_strStatus = strStatus;
}

void Matrix::SetCellFlag(int nRow, int nCol, int nValue)
{
    if (((nRow > m_nNum) || (nRow <= 0))
            || ((nCol > m_nNum) || (nCol) <= 0))
        return;
    //现将同一行的其它灯清除掉
    for (int i = 0; i < m_nNum; i++)
    {
        m_nCellFalg[nRow - 1][i] = 0;
    }
    m_nCellFalg[nRow - 1][nCol -1] = nValue;
    this->update();
}

void Matrix::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    //获取窗体的活动区
    QRect rect = this->geometry();
    //绘制外面的黑色背景框
    painter.fillRect(10, 10, rect.width() - 10, rect.width() - 10, Qt::black);
    painter.save();

    QPen pen;
    pen.setWidth(2); //设置宽度
    pen.setBrush(Qt::red); //设置颜色
    painter.setPen(pen); //选中画笔
    //要绘制的单元个数
    int nNum = m_nNum;
    int pointSize = 10 / (16 / m_nNum);
    //绘制文字
    QFont font("Arial", pointSize);
    painter.setFont(font);

    //计算间距
    int nSpance = (rect.width() - 10 - 10 - 10 - 50) / nNum;
    int nLeftPos = 20;  //左上角左边位置
    int nTopPos = nSpance + 50; //左上角上边距位置
    //绘制 类型：分控
    QString strType = QString(tr("控制状态:%1  \r\n自检状态:%2")).arg(m_strType).arg(m_strStatus);
    QRectF rtType(nLeftPos - 5, nTopPos - 3 * nSpance, font.weight() * 4, nSpance * 2);
    painter.drawText(rtType, Qt::AlignLeft, strType);

    pen.setWidth(2); //设置宽度
    pen.setBrush(Qt::white); //设置颜色
    QFont font1("Arial", pointSize);
    painter.setPen(pen); //选中画笔
    painter.setFont(font1);
    //绘制上面的输入文字
    QString strIn(tr("输入"));
    QRectF rtIn(nLeftPos + nSpance * nNum / 2 - 5, nTopPos - 2 * nSpance, font.weight(), nSpance);
    painter.drawText(rtIn, Qt::AlignCenter, strIn);
    //绘制二维线条
    for (int nRow= 0; nRow < nNum; nRow++)
    {
        for(int nCol = 0; nCol < nNum; nCol++)
        {
            QRectF rect(nLeftPos + nSpance * nCol, nTopPos + nSpance * nRow, nSpance, nSpance);
            m_nCellRect[nRow][nCol] = rect;
            painter.drawRect(rect);
        }
        QRectF rtH(nLeftPos + nSpance * nRow, nTopPos - nSpance, nSpance, nSpance);
        painter.drawText(rtH, Qt::AlignCenter, QString::number(nRow + 1));

        QRectF rtV(nLeftPos + nSpance * nNum, nTopPos + nSpance * nRow, nSpance, nSpance);
        painter.drawText(rtV, Qt::AlignCenter, QString::number(nRow + 1));
    }
    QRectF rtOut(nLeftPos + nSpance * (nNum + 1) + 5, nTopPos + nSpance * nNum / 2 - font.weight() / 2, nSpance, font.weight());
    painter.drawText(rtOut, Qt::AlignVCenter, QString(tr("输\n出")));

    //绘制二维矩阵中的圆形
    painter.restore();
    painter.setBrush(Qt::green); //设置绿色画刷
    for (int i = 0; i < nNum; i++)
    {
        for(int j = 0; j < nNum; j++)
        {
            if (m_nCellFalg[i][j] == 1)
            {
                qDebug() << "light : " << i << "  " << j;
                QRectF rect(m_nCellRect[i][j].left() + 3, m_nCellRect[i][j].top() + 3,
                            m_nCellRect[i][j].width() - 6, m_nCellRect[i][j].height() - 6);
                painter.drawEllipse(rect);
            }
        }
    }
}

