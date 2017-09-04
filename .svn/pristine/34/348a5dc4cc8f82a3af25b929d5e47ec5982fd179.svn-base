#ifndef MATRIX_H
#define MATRIX_H

#include <QWidget>

//定义最大的开关矩阵的个数
#define MAX_NUM     32

class Matrix : public QWidget
{
    Q_OBJECT
public:
    explicit Matrix(int nNum, QWidget *parent = 0);
    ~Matrix();

    void SetShowType(QString strType = tr("分控"), QString strStatus = tr("正常"));
    void SetCellFlag(int nRow, int nCol, int nValue = 1);
    void SetFontSize(int nSize);

protected:
    void paintEvent(QPaintEvent *);

private:
    int m_nNum;     //二维矩阵的表格行列数
    QString m_strType;  //显示分控还是主控
    QString m_strStatus;  //显示自检状态  正常还是故障

    int m_nCellFalg[MAX_NUM][MAX_NUM];  //显示灯的标记
    QRectF m_nCellRect[MAX_NUM][MAX_NUM];   //显示灯的区域

    int m_nFontSize;
};

#endif // MATRIX_H
