#ifndef CXMLREADER_H
#define CXMLREADER_H

#include <QDomDocument>
#include <QString>
#include <QList>
#include <QMap>
using namespace std;

typedef unsigned char UCHAR;

//组结构体信息内容
typedef struct _tag_Group_Content
{
    QString strName;
    QList<QString> paramNameList;
    QMap<int, UCHAR> paramValueMap;
}GROUP_CONTENT;

class CXmlReader
{
public:
    CXmlReader();
    ~CXmlReader();

public:
    bool loadXML(const QString & strPath);

private:
    void readGroup(QDomElement node);

public:
    QList<GROUP_CONTENT> m_GroupList;

private:
    QDomElement m_nodeRoot;
};

#endif // CXMLREADER_H
