#include "Global.h"
#include "XmlReader.h"

#include <QDomDocument>
#include <QFile>
#include <QDebug>

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
    readHeadNode(domElement);
    return true;
}

bool CXmlReader::loadXML(const QString &strPath,QDomElement &node)
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

    node = doc.documentElement();
    return true;  //Data子节点
}

int CXmlReader::init()
{
    QDir dir;
    QString strPath = dir.currentPath();
    QString strStructPath = strPath + "/TM/System/ModuleConfig/StructConfig.xml";
    QString strPackConfig = strPath + "/TM/System/ModuleConfig/PackConfig.xml";
    QDomElement node;
    loadXML(strStructPath, node);
    readStruct(node);
    loadXML(strPackConfig, node);
    readPakconf(node);

    return 0;
}

int CXmlReader::readStruct(QDomElement node)
{
    if (!node.hasChildNodes())
        return 1;
    QDomNodeList domList = node.childNodes();  //Data以下的所有节点。
    int iCount = domList.count();
    QString NodeName;
    for(int i = 0; i < iCount; i++)
    {
        QDomElement tempNode = domList.at(i).toElement();
        NodeName = tempNode.tagName();
        if(NodeName.toLower().compare("struct")!=0)
        {
            continue;
        }
        XML_STRUCTINFO staInfo;
        if(tempNode.hasAttribute("id"))
            staInfo.id = tempNode.attribute("id");
        if(tempNode.hasAttribute("len"))
            staInfo.len = tempNode.attribute("len").toInt();
        if(tempNode.hasAttribute("note"))
            staInfo.note = tempNode.attribute("note");
        if(tempNode.hasAttribute("type"))
            staInfo.type = tempNode.attribute("type");
        QDomNodeList TmpdomList = tempNode.childNodes();
        int TmpCount = TmpdomList.count();
        for(int j = 0; j < TmpCount; j++)
        {
            QDomElement tempNode2 = TmpdomList.at(j).toElement();
            QString nodeName1 = tempNode2.tagName();
            if(nodeName1.toLower().compare("para") != 0)
            {
                continue;
            }
            XML_STRUCT_PARA para;
            if (tempNode2.hasAttribute("id"))
                para.id = tempNode2.attribute("id");
            if (tempNode2.hasAttribute("name"))
                para.name = tempNode2.attribute("name");
            if (tempNode2.hasAttribute("note"))
                para.note = tempNode2.attribute("note");
            if (tempNode2.hasAttribute("num"))
                para.num = tempNode2.attribute("num").toInt();
            if (tempNode2.hasAttribute("type"))
                para.type = tempNode2.attribute("type");
            if (tempNode2.hasAttribute("precision"))
                para.precision = tempNode2.attribute("precision").toInt();
            if (tempNode2.hasAttribute("symbol"))
                para.symbol = tempNode2.attribute("symbol").toDouble();
            staInfo.paraList.insert(para.id,para);
        }
        m_mapStruct.insert(staInfo.id,staInfo);
    }
    return 0;
}

int CXmlReader::readPakconf(QDomElement node)
{
    if (!node.hasChildNodes())
        return 1;
    QDomNodeList domList = node.childNodes();  //Data以下的所有节点。
    int iCount = domList.count();
    QString NodeName;
    for(int i = 0; i < iCount; i++)
    {
        QDomElement tempNode = domList.at(i).toElement();
        NodeName = tempNode.tagName();
        if(NodeName.toLower().compare("package") != 0)
        //if(NodeName=="package")
        {
            continue;
        }
        XML_PACKAGEINFO packInfo;
        if(tempNode.hasAttribute("id"))
            packInfo.id = tempNode.attribute("id");
        if(tempNode.hasAttribute("name"))
            packInfo.name = tempNode.attribute("name");
        if(tempNode.hasAttribute("note"))
            packInfo.note = tempNode.attribute("note");

        readMembers(tempNode,packInfo);
        m_mapPackage.insert(packInfo.id,packInfo);
    }
    return 0;
}

int CXmlReader::readMembers(QDomElement node,XML_PACKAGEINFO &packInfo)
{
    if (!node.hasChildNodes())
        return 1;
    QDomNodeList domList = node.childNodes();  //Data以下的所有节点。
    int iCount = domList.count();
    QString NodeName;
    for(int i = 0; i < iCount; i++)
    {
        QDomElement tempNode = domList.at(i).toElement();
        NodeName = tempNode.tagName();
        if(NodeName.toLower().compare("members") != 0)
        {
            continue;
        }
        XML_PACKAGE_MEMBERS members;
        if(tempNode.hasAttribute("id"))
            members.id = tempNode.attribute("id");
        if(tempNode.hasAttribute("type"))
            members.type = tempNode.attribute("type");

        readLink(tempNode,members);
        packInfo.MembersList.insert(members.id,members);
    }
    return 0;
}

int CXmlReader::readLink(QDomElement node, XML_PACKAGE_MEMBERS &members)
{
    if (!node.hasChildNodes())
        return 1;
    QDomNodeList domList = node.childNodes();  //Data以下的所有节点。
    int iCount = domList.count();
    QString NodeName;
    for(int i = 0; i < iCount; i++)
    {
        QDomElement tempNode = domList.at(i).toElement();
        NodeName = tempNode.tagName();
        if(NodeName.toLower().compare("link") != 0)
        {
            continue;
        }
        XML_PACKAGE_LINK link;
        if(tempNode.hasAttribute("id"))
            link.id = tempNode.attribute("id").toInt();
        if(tempNode.hasAttribute("paraid"))
            link.paraId = tempNode.attribute("paraid");

        readAddr(tempNode,link);
        members.linkList.insert(link.id,link);
    }
    return 0;
}

int CXmlReader::readAddr(QDomElement node, XML_PACKAGE_LINK &link)
{
    if (!node.hasChildNodes())
        return 1;
    QDomNodeList domList = node.childNodes();  //Data以下的所有节点。
    int iCount = domList.count();
    QString NodeName;
    for(int i = 0; i < iCount; i++)
    {
        QDomElement tempNode = domList.at(i).toElement();
        NodeName = tempNode.tagName();
        if(NodeName.toLower().compare("addr") != 0)
        {
            continue;
        }
        XML_PACKAGE_ADDR addr;
        if(tempNode.hasAttribute("id"))
            addr.id = tempNode.attribute("id").toInt();
        if(tempNode.hasAttribute("value"))
            addr.value = tempNode.attribute("value");
        if(tempNode.hasAttribute("vtype"))
            addr.vtype = tempNode.attribute("vtype");
        if(tempNode.hasAttribute("dst"))
            addr.dst = tempNode.attribute("dst");

        link.addrList.insert(addr.value,addr);
    }
    return 0;
}

void CXmlReader::readHeadNode(QDomElement node)
{
    if (!node.hasChildNodes())
        return;

    m_nodeRoot = node;
    QDomNodeList domList = node.childNodes();  //Data以下的所有节点。
    int iCount = domList.count();
    QString NodeName;
    for(int i = 0; i < iCount; i++)
    {
        QDomElement tempNode = domList.at(i).toElement();
        NodeName = tempNode.tagName();
        if(NodeName.toLower().compare("HEAD") != 0)
        {
            continue;
        }
        QDomNodeList TmpdomList = tempNode.childNodes();
        int TmpCount = TmpdomList.count();
        for(int j = 0; j < TmpCount; j++)
        {
            QDomElement tempNode2 = TmpdomList.at(j).toElement();
            QString nodeName1 = tempNode2.tagName();
            if(nodeName1.toLower().compare("NODE") != 0)
            {
                continue;
            }
            DATAINFO info;
            info.nLen = 0;
            if (tempNode2.hasAttribute("id"))
                info.strID = tempNode2.attribute("id");
            if (tempNode2.hasAttribute("name"))
                info.strName = tempNode2.attribute("name");
            if (tempNode2.hasAttribute("len"))
                info.nLen = tempNode2.attribute("len").toInt();
            if (tempNode2.hasAttribute("value"))
                info.strValue = tempNode2.attribute("value");
            if (tempNode2.hasAttribute("dec"))
                info.strDesc = tempNode2.attribute("dec");
            if (tempNode2.hasAttribute("datatype"))
                info.dataType = (DATATYPE)(tempNode2.attribute("datatype").toInt());

            if (tempNode2.hasAttribute("codeid"))
            {
                info.strCodeID = tempNode2.attribute("codeid");
                QString strID = tempNode2.attribute("codeid");
                readCodeNode(strID, node, info);
            }
            m_BussInfo.infoList.push_back(info);
        }
    }
}

void CXmlReader::readCodeNode(QString strCodeID, QDomElement node, DATAINFO &codeinfo)
{
    QDomNodeList domList = node.childNodes();  //Data以下的所有节点。
    int iCount = domList.count();
    QString NodeName;
    for(int i = 0; i < iCount; i++)
    {
       QDomElement tempNode = domList.at(i).toElement();
       NodeName = tempNode.tagName();
       if(NodeName.toLower().compare("CODE") != 0)
       {
           continue;
       }
       if (tempNode.hasAttribute("codeid"))
       {
          if (strCodeID != tempNode.attribute("codeid"))
          {
              continue;
          }
          QDomNodeList TmpdomList = tempNode.childNodes();
          int TmpCount = TmpdomList.count();
          for(int j = 0; j < TmpCount; j++)
          {
              QDomElement tempNode2 = TmpdomList.at(j).toElement();
              QString nodeName1 = tempNode2.tagName();
              if(nodeName1.toLower().compare("CMD") != 0)
              {
                  continue;
              }
              DATAINFO info;
              info.nLen = 0;
              if (tempNode2.hasAttribute("id"))
                 info.strID = tempNode2.attribute("id");
              if (tempNode2.hasAttribute("name"))
                 info.strName = tempNode2.attribute("name");
              if (tempNode2.hasAttribute("len"))
                 info.nLen = tempNode2.attribute("len").toInt();
              if (tempNode2.hasAttribute("value"))
                 info.strValue = tempNode2.attribute("value");
              if (tempNode2.hasAttribute("dec"))
                 info.strDesc = tempNode2.attribute("dec");
              if (tempNode2.hasAttribute("datatype"))
                 info.dataType = (DATATYPE)(tempNode2.attribute("datatype").toInt());
              if (tempNode2.hasAttribute("codeid"))
                  info.strCodeID = tempNode2.attribute("codeid");

              readParamNode(tempNode2, info);
              codeinfo.subInfoList.push_back(info);
          }
       }
    }
}

void CXmlReader::readParamNode(QDomElement node, DATAINFO &codeinfo)
{
    QDomNodeList domList = node.childNodes();  //Data以下的所有节点。
    int iCount = domList.count();
    QString NodeName;
    for(int i = 0; i < iCount; i++)
    {
        QDomElement tempNode = domList.at(i).toElement();
        NodeName = tempNode.tagName();
        if(NodeName.toLower().compare("PARAM") != 0)
        {
            continue;
        }
        DATAINFO info;
        info.nLen = 0;
        if (tempNode.hasAttribute("id"))
            info.strID = tempNode.attribute("id");
        if (tempNode.hasAttribute("name"))
            info.strName = tempNode.attribute("name");
        if (tempNode.hasAttribute("len"))
            info.nLen = tempNode.attribute("len").toInt();
        if (tempNode.hasAttribute("value"))
            info.strValue = tempNode.attribute("value");
        if (tempNode.hasAttribute("dec"))
            info.strDesc = tempNode.attribute("dec");
        if (tempNode.hasAttribute("datatype"))
            info.dataType = (DATATYPE)(tempNode.attribute("datatype").toInt());
        if (tempNode.hasAttribute("codeid"))
            info.strCodeID = tempNode.attribute("codeid");

        if (info.strCodeID != "")
            readCodeNode(info.strCodeID, m_nodeRoot, info);

        readSubNode(tempNode, info);
        codeinfo.subInfoList.push_back(info);
    }
}

void CXmlReader::readSubNode(QDomElement node, DATAINFO &codeinfo)
{
    QDomNodeList domList = node.childNodes();  //Data以下的所有节点。
    int iCount = domList.count();
    QString NodeName;
    for(int i = 0; i < iCount; i++)
    {
        QDomElement tempNode = domList.at(i).toElement();
        NodeName = tempNode.tagName();
        if(NodeName.toLower().compare("SUBNODE") != 0)
        {
            continue;
        }
        DATAINFO info;
        info.nLen = 0;
        if (tempNode.hasAttribute("id"))
            info.strID = tempNode.attribute("id");
        if (tempNode.hasAttribute("name"))
            info.strName = tempNode.attribute("name");
        if (tempNode.hasAttribute("len"))
            info.nLen = tempNode.attribute("len").toInt();
        if (tempNode.hasAttribute("value"))
            info.strValue = tempNode.attribute("value");
        if (tempNode.hasAttribute("dec"))
            info.strDesc = tempNode.attribute("dec");
//        if (tempNode.hasAttribute("offset"))
//            info.strOffset = tempNode.attribute("offset");
        if (tempNode.hasAttribute("datatype"))
            info.dataType = (DATATYPE)(tempNode.attribute("datatype").toInt());
        if (tempNode.hasAttribute("codeid"))
            info.strCodeID = tempNode.attribute("codeid");

        codeinfo.subInfoList.push_back(info);
    }
}

bool CXmlReader::getDefaultValueOfSwMatPro(QDomElement &node, PROCOTOL_PACK &swMatPro)
{
    const QString U0F = "U0F";
    const QString U0B = "U0B";
    const QString _struct = "struct";
    const QString id = "id";
    QDomNodeList domList = node.childNodes();
    int iCount = domList.count();
    if(!iCount)
        return false;
    QString NodeName;
    bool isOKOfU0F = false, isOKOfU0B = false;
    for(int i = 0; i < iCount; i++)
    {
        QDomElement tmpNode = domList.at(i).toElement();
        NodeName = tmpNode.tagName();
        if(NodeName.toLower().compare(_struct) != 0)
            continue;
        if(!tmpNode.hasAttribute(id))
            continue;
        if(U0F == tmpNode.attribute(id))
        {
            QDomNodeList tmpList = tmpNode.childNodes();
            swMatPro.H = tmpList.at(0).toElement().attribute("value").toUInt(0, 16);
            swMatPro.M = (UCHAR)tmpList.at(2).toElement().attribute("value").toUShort(0, 16);
            swMatPro.T = (UCHAR)tmpList.at(3).toElement().attribute("value").toUInt(0, 16);
            swMatPro.C = tmpList.at(6).toElement().attribute("value").toUShort(0, 16);
            swMatPro.Data.S1 = swMatPro.S = tmpList.at(8).toElement().attribute("value").toUShort(0, 16);
            swMatPro.Data.D1 = swMatPro.D= tmpList.at(9).toElement().attribute("value").toUShort(0, 16);
            isOKOfU0F = true;
         }
        else if(U0B == tmpNode.attribute(id))
        {
            QDomNodeList tmpList = tmpNode.childNodes();
            swMatPro.Data.T1 = tmpList.at(2).toElement().attribute("value").toUShort(0, 16);
            isOKOfU0B = true;
        }
    }
    return isOKOfU0B && isOKOfU0F;
}

bool CXmlReader::getDefaultValueOfNetSDFrm(QDomElement &node, NETSDFRM &netSDFrm)
{
    const QString U01 = "U01";
    const QString _struct = "struct";
    const QString id = "id";
    QDomNodeList domList = node.childNodes();
    int iCount = domList.count();
    if(!iCount)
        return false;
    QString NodeName;
    bool isOKOfU01 = false;
    for(int i = 0; i < iCount; i++)
    {
        QDomElement tmpNode = domList.at(i).toElement();
        NodeName = tmpNode.tagName();
        if(NodeName.toLower().compare(_struct) != 0)
            continue;
        if(!tmpNode.hasAttribute(id))
            continue;
        if(U01 == tmpNode.attribute(id))
        {
            QDomNodeList tmpList = tmpNode.childNodes();
            netSDFrm.SID = tmpList.at(0).toElement().attribute("value").toUInt(0, 16);
            netSDFrm.DID = tmpList.at(1).toElement().attribute("value").toUInt(0, 16);
            netSDFrm.BID = tmpList.at(2).toElement().attribute("value").toUInt(0, 16);
            netSDFrm.Ver = tmpList.at(3).toElement().attribute("value").toUShort(0, 16);
            isOKOfU01 = true;
         }
    }
    return isOKOfU01;
}
