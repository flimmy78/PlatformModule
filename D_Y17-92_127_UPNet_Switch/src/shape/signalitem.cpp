#include "signalitem.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QAction>
#include <QMenu>

//TODO:添加Item宽度可设置？？？不然该控件的宽度不能小于400
SignalItem::SignalItem(int height, bool bFix1, const QString &inputSignalName,
                       const QString &inputId, LineLabel::LLType type,
                       bool bFix2, const QString &outputId, const QString &outputSignalName,
                       QList<PARAM_CONTENT> paramList,
                       QWidget *parent) : QWidget(parent)
{
    m_bLeftFix = bFix1;
    m_bRightFix = bFix2;
    m_MenuList = paramList;

    //计算标签尺寸和标签字体尺寸、按钮尺寸和按钮字体尺寸
    int btnPointSize = 10;
    static const int s_BTN_SIDE = height < 3 ?
                height : (height - 2 > 2 ? height - 2 : 2);//空出2作为按钮下边距
    if(s_BTN_SIDE < 10)
        btnPointSize = s_BTN_SIDE - 2 > 2 ? s_BTN_SIDE - 2 : 1;//4,3,2,1BTN_SIDE按钮字体都为1
    int labelPointSize = btnPointSize < 4 ? 1 : btnPointSize - 2;//5,4,3,2,1BTN_SIDE标签字体都为1

    //加入输入源信号名三角标签
    int triangleWidth = s_BTN_SIDE / 4 < 2 ? 1 : s_BTN_SIDE / 4;
    QHBoxLayout *mainLayout = new QHBoxLayout;
    m_arrowInputLabel = new ArrowLabel(labelPointSize, triangleWidth, s_BTN_SIDE / 2, inputSignalName, this);
    m_arrowInputLabel->setFixedSize(160, height);
    mainLayout->addWidget(m_arrowInputLabel);

    //加入输入源按钮
    static const QString s_BTN_QSS("QPushButton{ border: 1px solid #000; font-size: %1px; }");
    m_numInputBtn = new MenuButton(inputId, bFix1, paramList, this);
    m_numInputBtn->setFixedSize(s_BTN_SIDE, s_BTN_SIDE);
    m_numInputBtn->setStyleSheet(s_BTN_QSS.arg(btnPointSize));
    mainLayout->addWidget(m_numInputBtn);
    QObject::connect(m_numInputBtn, SIGNAL(checkedActionText(QString)), m_arrowInputLabel, SLOT(setText(QString)));

    //加入十字架分隔线标签
    LineLabel *lineLabel = new LineLabel(type, this);
    lineLabel->setFixedSize(40, height);
    mainLayout->addWidget(lineLabel);

    //加入输出源按钮
    m_numOutputBtn = new MenuButton(outputId, bFix2, paramList, this);
    m_numOutputBtn->setFixedSize(s_BTN_SIDE, s_BTN_SIDE);
    m_numOutputBtn->setStyleSheet(s_BTN_QSS.arg(btnPointSize));
    mainLayout->addWidget(m_numOutputBtn);

    //加入输出源信号名点标签
    int dotDiameter = s_BTN_SIDE / 6 < 2 ? 1 : s_BTN_SIDE / 6;
    m_dotOutputLabel = new DotLabel(labelPointSize, dotDiameter, outputSignalName, this);
    m_dotOutputLabel->setFixedSize(160, height);
    mainLayout->addWidget(m_dotOutputLabel);
    QObject::connect(m_numOutputBtn, SIGNAL(checkedActionText(QString)), m_dotOutputLabel, SLOT(setText(QString)));

    mainLayout->addStretch();
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(-1, 0, -1, 0);//去除布局边距
    this->setLayout(mainLayout);
    this->setFixedHeight(height);
}

void SignalItem::setInfo(QString strIndex)
{
    bool bExist = false;
    QString strDesc("");
    for (int i = 0; i < m_MenuList.size(); i++)
    {
        QString strValue = m_MenuList.at(i).strValue;
        //qDebug() << strValue << " " << strIndex;
        if (strIndex == strValue)
        {
            strDesc = m_MenuList.at(i).strDesc;
            bExist = true;
            break;
        }
    }
    if (!bExist)
        return;
    //qDebug() << strIndex;

    if(m_bLeftFix)
    {
        m_arrowInputLabel->setText(strDesc);
        m_numInputBtn->setText(strIndex);
    }
    if(m_bRightFix)
    {
        m_numOutputBtn->setText(strIndex);
        m_dotOutputLabel->setText(strDesc);
    }
}

QString SignalItem::getInfo()
{
    QString strText(tr(""));
    if(m_bLeftFix)
    {
        strText = m_numInputBtn->text();
    }
    if(m_bRightFix)
    {
        strText = m_numOutputBtn->text();
    }
    return strText;
}

void SignalItem::SetLabelFontSize(int nSize)
{
    m_numInputBtn->SetFontSize(nSize);
    m_numOutputBtn->SetFontSize(nSize);

    m_arrowInputLabel->SetFontSize(nSize);
    m_dotOutputLabel->SetFontSize(nSize);
}
