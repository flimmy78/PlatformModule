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
    readMatrix(domElement, m_IMFConfig.matrixNum);
    readNum("1", domElement, m_IMFConfig.num1);
    readNum("2", domElement, m_IMFConfig.num2);
    readMenu(domElement, m_IMFConfig.menu);
    return true;
}

void CXmlReader::readMatrix(QDomElement node, int &num)
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
        if(NodeName.toUpper().compare("MATRIX") != 0)
        {
            continue;
        }
        if (tempNode.hasAttribute("num"))
        {
            num = tempNode.attribute("num").toInt();
            break;
        }
    }
}

void CXmlReader::readNum(QString strID, QDomElement node, NUM_PARAM &num)
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
        if(NodeName.toUpper().compare("NUM") != 0)
        {
            continue;
        }
        QString strTmp;
        if (tempNode.hasAttribute("id"))
        {
            strTmp = tempNode.attribute("id");
        }
        if (strID != strTmp)
        {
            continue;
        }
        if (tempNode.hasAttribute("fix"))
        {
            num.bFix = tempNode.attribute("fix").toInt();
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
            PARAM_CONTENT content;
            if (tempNode2.hasAttribute("value"))
                content.strValue = tempNode2.attribute("value");
            if (tempNode2.hasAttribute("desc"))
                content.strDesc = tempNode2.attribute("desc");
            num.paramList.push_back(content);
        }
    }
}

void CXmlReader::readMenu(QDomElement node, NUM_PARAM &menu)
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
        if(NodeName.toUpper().compare("MENU") != 0)
        {
            continue;
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
            PARAM_CONTENT content;
            if (tempNode2.hasAttribute("value"))
                content.strValue = tempNode2.attribute("value");
            if (tempNode2.hasAttribute("desc"))
                content.strDesc = tempNode2.attribute("desc");
            menu.paramList.push_back(content);
        }
    }
}
