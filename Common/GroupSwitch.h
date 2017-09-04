#ifndef GROUPSWITCH_H
#define GROUPSWITCH_H

#include <QWidget>
#include <QList>
#include <QString>
#include <QRectF>
#include <QPainter>
#include <QMap>
#include <QPolygonF>
#include <QComboBox>

class GroupSwitch : public QWidget
{
    Q_OBJECT
public:
    explicit GroupSwitch(QWidget *parent = 0);
    explicit GroupSwitch(const QRectF &rect, const QList<QString> &contextList,
                         QString outContext, QWidget *parent = 0);
    ~GroupSwitch();

public:
    //设置绘制区域
    void setDrawRect(const QRectF &rect);
    //设置输出文字
    void setOutContext(const QString &outContext);
    //设置输入文字列表
    void setContextList(const QList<QString> &contextList);
    //设置选中的数据内容
    void setSelectText(QString strContext);
    //设置画线的长度
    void setLineLen(unsigned int lineLen);
    //输入端是否需要画箭头
    void showInputArrow(bool bShow = true);
    //输出端是否显示点
    void showOutputSpot(bool bShow = true);
    //设置字体的大小
    void setFont(const QFont& font);
    void setFont(int nSize = 9);
    //设置线与线之间的间距
    void setLineDistance(unsigned int lineDistance = 15);

    //设置当前选择的输入通道
    void setCurrentIndex(int nSelIndex = 0);
    //获取当前选择的通道
    int getCurrentIndex();
    //选择框是否可用
    void enabledCtrl(bool enable = true);
    //设置是否显示右边的下拉列表
    void showCombox(bool show = true);

protected:
    void paintEvent(QPaintEvent *);

private slots:
    void onChooseAccount(int nIndex);

private:
    //初始化组选框控件
    void init();
    void initCombox();
    void calcPoint();
    void drawLineArrow(QPainter &painter, QPointF pt, Qt::GlobalColor style, bool bShow = true);
    void drawLineSpot(QPainter &painter, QPointF pt, bool bShow = true);

    void drawGreenLine(QPainter &painter, QPolygonF &poly);

private:
    QRectF m_Rect;                  //绘制的区间范围
    QString m_strSelectContext;     //界面上选择的文字 用于绘制其它颜色使用
    unsigned int m_nLineLen;        //绘制线的长度
    bool m_bShowSpot;               //是否显示点
    bool m_bShowArrow;              //是否显示箭头
    QList<QString> m_contextList;   //绘制的数据列表
    QFont m_Font;                   //绘制字体的大小
    QString m_strOutputContext;     //输出提示信息
    unsigned int m_nLineDistance;   //输出线之间的线间距
    bool m_bShowCombox;             //是否显示右边的下拉列表

    unsigned int m_nContextCount;

    QMap<int, QPolygonF> m_PolyMap;

    QPolygonF m_OutPoint;

    QPolygonF m_DrawPoint;

    QComboBox *m_pComType;
};

#endif // GROUPSWITCH_H
