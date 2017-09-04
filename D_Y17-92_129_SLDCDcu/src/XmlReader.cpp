#include "XmlReader.h"

#include <QFile>

CXmlReader::CXmlReader()
{

}

CXmlReader::~CXmlReader()
{

}

bool CXmlReader::loadXML(const QString &strPath)
{
    QDomDocument doc;
    QFile file(strPath);
    if (!file.open(QIODevice::ReadOnly))
       return false;

    if (!doc.setContent(&file))
    {
       file.close();
       return false;
    }
    file.close();

    QDomElement domElement = doc.documentElement();  //Data子节点
    readGroup(domElement);
    return true;
}

void CXmlReader::readGroup(QDomElement node)
{
    if (!node.hasChildNodes())
        return;
    QDomNodeList domList = node.childNodes();  //Data以下的所有节点。
    int iCount = domList.count();
    QString NodeName;
    for(int i = 0; i < iCount; i++)
    {
        QDomElement tempNode = domList.at(i).toElement();
        NodeName = tempNode.tagName();
        if(NodeName.toUpper().compare("GROUP") != 0)
        {
            continue;
        }
        GROUP_CONTENT group;
        if (tempNode.hasAttribute("name"))
        {
            group.strName  = tempNode.attribute("name");
        }
        QDomNodeList TmpdomList = tempNode.childNodes();
        int TmpCount = TmpdomList.count();
        for(int j = 0; j < TmpCount; j++)
        {
            QDomElement tempNode2 = TmpdomList.at(j).toElement();
            QString nodeName1 = tempNode2.tagName();
            if(nodeName1.toUpper().compare("PARAM") != 0)
            {
                continue;
            }
            if (tempNode2.hasAttribute("name"))
                group.paramNameList.append(tempNode2.attribute("name"));
            if (tempNode2.hasAttribute("value"))
            {
                UCHAR ucValue = (tempNode2.attribute("value")).toUShort(0, 16);
                group.paramValueMap.insert(j, ucValue);
            }
        }
        m_GroupList.push_back(group);
    }
}
