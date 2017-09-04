#include "GroupSwitch.h"
#include <QPen>
#include <QListWidget>

#include <QDebug>

GroupSwitch::GroupSwitch(QWidget *parent)
    : QWidget(parent)
{
    init();
    initCombox();
}

GroupSwitch::GroupSwitch(const QRectF &rect, const QList<QString> &contextList, QString outContext, QWidget *parent)
    : QWidget(parent), m_Rect(rect)
{
    m_bShowSpot = true;
    m_bShowArrow = true;
    m_contextList = contextList;
    m_strOutputContext = outContext;
    setMinimumSize(rect.width(), rect.height());
    init();
    initCombox();
}

GroupSwitch::~GroupSwitch()
{

}

void GroupSwitch::setDrawRect(const QRectF &rect)
{
    m_Rect = rect;
    setMinimumSize(rect.width(), rect.height());
}

void GroupSwitch::setOutContext(const QString &outContext)
{
    m_strOutputContext = outContext;
}

void GroupSwitch::setContextList(const QList<QString> &contextList)
{
    m_contextList = contextList;
    m_nContextCount = m_contextList.size();
}

void GroupSwitch::setSelectText(QString strContext)
{
    m_strSelectContext = strContext;
}

void GroupSwitch::setLineLen(unsigned int lineLen)
{
    m_nLineLen = lineLen;
}

void GroupSwitch::showInputArrow(bool bShow)
{
    m_bShowArrow = bShow;
}

void GroupSwitch::showOutputSpot(bool bShow)
{
    m_bShowSpot = bShow;
}

void GroupSwitch::setFont(const QFont &font)
{
    m_Font = font;
}

void GroupSwitch::setFont(int nSize)
{
    m_Font.setPointSize(nSize);
}

void GroupSwitch::setLineDistance(unsigned int lineDistance)
{
    m_nLineDistance = lineDistance;
    calcPoint();
    //计算移动的位置
    QPointF pt = m_OutPoint.at(1);
    int cX = m_nLineLen * 2 + m_nLineLen / 10 + + m_Rect.width() / 4;
    int cY = pt.ry() - 15;
    m_pComType->move(cX, cY);

    onChooseAccount(0);
}

void GroupSwitch::setCurrentIndex(int nSelIndex)
{
    onChooseAccount(nSelIndex);
}

int GroupSwitch::getCurrentIndex()
{
    if (m_pComType == NULL)
        return -1;

    return m_pComType->currentIndex();
}

void GroupSwitch::enabledCtrl(bool enable)
{
    m_pComType->setEnabled(enable);
}

void GroupSwitch::showCombox(bool show)
{
    if (show)
    {
        m_pComType->show();
    }
    else
    {
        m_pComType->hide();
    }
}

void GroupSwitch::paintEvent(QPaintEvent *)
{
    QPen pen;
    QPainter painter(this);

    //设置画笔的颜色
    pen.setWidth(1);
    painter.setPen(Qt::black);

    int nLineLen = m_nLineLen;
    /********************绘制输入信号内容************************/
    //绘制坐标点
    int cx = m_Rect.left();
    int cy = m_Rect.top();
    int nHight = m_nLineDistance;
    QPointF pTop, pBottom;
    for (int i = 0; i < m_nContextCount; i++)
    {
        //绘制文字
        QRectF r(cx, cy, nLineLen - cx, nHight);
        painter.setFont(m_Font);
        painter.drawText(r, Qt::AlignCenter, m_contextList.at(i));
        painter.setBackgroundMode(Qt::TransparentMode);

        //绘制线段
        cy += nHight;
        QPointF pB(cx, cy), pE(pB.rx() + nLineLen, cy);
        painter.drawLine(pB, pE);

        QPolygonF ployF;
        ployF.append(pB);
        ployF.append(pE);

        //绘制箭头
        drawLineArrow(painter, pE, Qt::black, m_bShowArrow);

        cy += nHight;

        if (i == 0) pTop = pE;
        if (i + 1 == m_nContextCount) pBottom = pE;
    }
    //绘制竖直连接线
    painter.drawLine(pTop, pBottom);
    /********************绘制输入信号内容************************/

    /********************绘制输出信号内容************************/
    //计算竖线的中间位置点
    QPointF pM = QPointF(pTop.rx(), pTop.ry() + (pBottom.ry() - pTop.ry()) / 2);
    QPointF pE = QPointF(pM.rx() + nLineLen, pM.ry());
    //绘制输出线
    painter.drawLine(pM, pE);

    //绘制输出线上点
    drawLineSpot(painter, pE, m_bShowSpot);
    /********************绘制输出信号内容************************/

    //绘制输出提示信息
    QRectF rectTip(m_nLineLen * 2 + m_nLineLen / 2, pE.ry() - nHight / 2, m_Rect.width() / 6, nHight);
    painter.drawText(rectTip, Qt::AlignLeft, m_strOutputContext);

    //绘制选种颜色的线
    drawGreenLine(painter, m_DrawPoint);
}

void GroupSwitch::init()
{
    m_nLineLen = 180;
    m_Font.setPointSize(9);
    m_nLineDistance = 15;
    m_bShowCombox = true;

    m_nContextCount = m_contextList.size();
    //计算绘制线的长度
    int nRectLen = m_Rect.width();
    int nLineLen = m_nLineLen;
    if (nRectLen < nLineLen * 4)
        nLineLen = nRectLen / 4;

    //计算相关坐标点
    calcPoint();
}

void GroupSwitch::initCombox()
{
    m_pComType = new QComboBox(this);
    m_pComType->setFixedWidth(120);
//    m_pComType->setEditable(true);  //设置可编辑
    //下拉选项高度
    m_pComType->setStyleSheet("border: 1px solid darkgray;"
                    "selection-background-color: lightgray;"
                    "QComboBox QAbstractItemView::item{height:30px;}");
    m_pComType->clear();
    //将配置信息显示在列表框中
    QListWidget *pLW = new QListWidget();
    pLW->setObjectName("QCombox");
    m_pComType->setModel(pLW->model());
    m_pComType->setView(pLW);
    for (int i = 0; i < m_nContextCount; i++)
    {
        m_pComType->addItem(m_contextList.at(i));
    }
    //当列表框内容发生改变后出发数据改变
    //wl 20170511 下拉列表变化不进行绘制线
    //QObject::connect(m_pComType, SIGNAL(currentIndexChanged(int)), this, SLOT(onChooseAccount(int )));

    if (m_nContextCount > 0)
    {
        m_pComType->setCurrentIndex(0);
        onChooseAccount(0);
    }

    //计算移动的位置
    int cX = m_nLineLen * 2 + m_nLineLen / 10 + m_Rect.width() / 4;
    int cY = m_nContextCount * m_nLineDistance;
    m_pComType->move(cX, cY);
}

void GroupSwitch::calcPoint()
{
    m_PolyMap.clear();
    m_OutPoint.clear();
    int nLineLen = m_nLineLen;
    int cx = m_Rect.left();
    int cy = m_Rect.top();
    int nHight = m_nLineDistance;
    QPointF pTop, pBottom;
    for (int i = 0; i < m_nContextCount; i++)
    {
        //绘制线段
        cy += nHight;
        QPointF pB(cx, cy), pE(pB.rx() + nLineLen, cy);
        QPolygonF ployF;
        ployF.append(pB);
        ployF.append(pE);

        m_PolyMap.insert(i, ployF);

        cy += nHight;

        if (i == 0) pTop = pE;
        if (i + 1 == m_nContextCount) pBottom = pE;
    }

    QPointF pM = QPointF(pTop.rx(), pTop.ry() + (pBottom.ry() - pTop.ry()) / 2);
    QPointF pE = QPointF(pM.rx() + nLineLen, pM.ry());

    m_OutPoint.append(pM);
    m_OutPoint.append(pE);
}

void GroupSwitch::drawLineArrow(QPainter &painter, QPointF pt, Qt::GlobalColor style, bool bShow)
{
    //不显示箭头直接返回
    if (!bShow)
        return;

    //绘制其它
    painter.setBrush(style);
    //绘制箭头
    QPolygon trianglePolygon;
    trianglePolygon.append(QPoint(pt.rx(), pt.ry()));
    trianglePolygon.append(QPoint(pt.rx() - 4, pt.ry() - 4));
    trianglePolygon.append(QPoint(pt.rx() - 4, pt.ry() + 4));
    painter.drawPolygon(trianglePolygon);
}

void GroupSwitch::drawLineSpot(QPainter &painter, QPointF pt, bool bShow)
{
    //不显示点直接返回
    if (!bShow)
        return;

    //绘制一个点号
    painter.setRenderHints(QPainter::Antialiasing);
    painter.drawEllipse(pt.rx() - 5, pt.ry() - 2, 4, 4);
}

void GroupSwitch::drawGreenLine(QPainter &painter, QPolygonF &poly)
{
    if (poly.size() <= 0)
        return;

    QPen pen;

    //设置画笔的颜色
    pen.setWidth(3);
    painter.setPen(Qt::green);
    painter.drawPolyline(poly);

    if (poly.size() < 4)
        return;

    //将箭头绘制成绿色
    drawLineArrow(painter, poly.at(1), Qt::green, m_bShowArrow);

    //
    drawLineSpot(painter, poly.at(3), m_bShowSpot);
}

void GroupSwitch::onChooseAccount(int nIndex)
{
    m_DrawPoint.clear();
    QMap<int, QPolygonF>::iterator iPoly = m_PolyMap.find(nIndex);
    if (iPoly != m_PolyMap.end())
    {
        m_DrawPoint = iPoly.value();
    }

    m_DrawPoint += m_OutPoint;

    update();
}
