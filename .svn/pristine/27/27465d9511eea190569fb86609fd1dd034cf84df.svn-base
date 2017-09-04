#ifndef CXMLREADER_H
#define CXMLREADER_H

#include <QDomDocument>
#include <QString>
#include <QList>
using namespace std;

//参数信息内容
typedef struct _tag_Param_Content
{
    QString strValue;
    QString strDesc;

    _tag_Param_Content()
    {
        strValue = strDesc = "";
    }
}PARAM_CONTENT;

//数据参数结构相关参数
typedef struct _tag_Num_Param
{
    bool bFix;  //是否固定
    QList<PARAM_CONTENT> paramList;

    _tag_Num_Param()
    {
        bFix = false;
        paramList.clear();
    }
}NUM_PARAM;

//数据内容
typedef struct _tag_IFM_Config
{
    int matrixNum;              //二维矩阵数组个数
    NUM_PARAM num1;     //数据值1
    NUM_PARAM num2;     //数据值2
    NUM_PARAM menu;     //菜单数据内容
    _tag_IFM_Config()
    {
        matrixNum = 0;
    }
}IFM_CONFIG;

class CXmlReader
{
public:
    CXmlReader();
    ~CXmlReader();

public:
    bool loadXML(const QString & strPath);

private:
    void readMatrix(QDomElement node, int &num);
    void readNum(QString strID, QDomElement node, NUM_PARAM &num);
    void readMenu(QDomElement node, NUM_PARAM &menu);

public:
    IFM_CONFIG m_IMFConfig;

private:
    QDomElement m_nodeRoot;
};

#endif // CXMLREADER_H
