#include "DeviceMap.h"
#include <QDir>
#include <QDomDocument>
#include <QFile>
#include <QDebug>

CDeviceMap::CDeviceMap()
{

}

CDeviceMap::~CDeviceMap()
{

}

bool CDeviceMap::getNode(QDomNodeList &nodeList)
{
    QDir dir;
    QString strPath = dir.currentPath();
    QString filePath = strPath + "/TM/System/ModuleConfig/DeviceMapping.xml";

    QFile file(filePath);
    if  (!file.open(QIODevice::ReadOnly))
       return false;
    QDomDocument doc;
    if  (!doc.setContent(&file))
    {
       file.close();
       return false;
    }
    file.close();
    QDomNode node = doc.documentElement();
    nodeList = node.childNodes();
    return 0 != nodeList.size();
}

bool CDeviceMap::processSubItem(QDomNodeList &nodeList, QVector<SUBITEM> &vSubItem)
{
    int iCount = nodeList.count();
    for(int i = 0; i < iCount; i++)
    {
        QDomElement tmpNode = nodeList.at(i).toElement();
        QString nodeName = tmpNode.tagName();
        if(nodeName.toLower().compare("object") != 0)
            continue;
        SUBITEM tmpSubItem;
        memset(&tmpSubItem, 0, sizeof(SUBITEM));
        if (tmpNode.hasAttribute("CMD"))
            tmpSubItem.usCMD = tmpNode.attribute("CMD").toUShort(0, 16);
        if (tmpNode.hasAttribute("Type"))
            tmpSubItem.usTID = tmpNode.attribute("Type").toUShort(0, 16);
        if (tmpNode.hasAttribute("StateType"))
            tmpSubItem.usStateType = tmpNode.attribute("StateType").toUShort(0, 16);
        if (tmpNode.hasAttribute("StationID"))
            tmpSubItem.usStationID = tmpNode.attribute("StationID").toUShort(0, 16);
        if (tmpNode.hasAttribute("SubSysID"))
            tmpSubItem.ucSubSysID = tmpNode.attribute("SubSysID").toUShort(0, 16);
        if (tmpNode.hasAttribute("DeviceID"))
            tmpSubItem.usDeviceID = tmpNode.attribute("DeviceID").toUShort(0, 16);
        if (tmpNode.hasAttribute("SN"))
            tmpSubItem.ucSN = tmpNode.attribute("SN").toUShort(0, 16);
        if (tmpNode.hasAttribute("PDXPSMCU"))
            tmpSubItem.uPDXPSMCU = tmpNode.attribute("PDXPSMCU").toUInt(0, 16);
        if (tmpNode.hasAttribute("PDXPDEVICES"))
            tmpSubItem.uPDXPDEVICES = tmpNode.attribute("PDXPDEVICES").toUInt(0, 16);
        if (tmpNode.hasAttribute("UintNo"))
            tmpSubItem.usUintNo = tmpNode.attribute("UintNo").toUShort(0, 16);
        if (tmpNode.hasAttribute("ID"))
            strcpy(tmpSubItem.szChannel, tmpNode.attribute("ID").toUtf8().data());

        vSubItem.push_back(tmpSubItem);
        m_subItemList.push_back(tmpSubItem);
    }
    return true;
}

bool CDeviceMap::processItem(QDomNodeList &nodeList)
{
    int iCount = nodeList.count();
    bool OK = false;
    for(int i = 0; i < iCount; i++)
    {
        QDomNode tmpNode = nodeList.at(i);
        QDomElement tmpElement = tmpNode.toElement();
        QString nodeName = tmpElement.tagName();
        ITEM tmpItem;
        if(nodeName.toLower().compare("module") != 0)
            continue;
        if (!tmpElement.hasAttribute("ObjectName"))
            return false;
        tmpItem.strModuleName = tmpElement.attribute("ObjectName");
        if(!tmpElement.hasAttribute("ObjectType"))
            return false;
        tmpItem.iObjType = tmpElement.attribute("ObjectType").toInt(&OK, 16);
        QDomNodeList tmpNodeList  = tmpNode.childNodes();
        if(!processSubItem(tmpNodeList, tmpItem.vSubItem))
            return false;
        m_devList.append(tmpItem);
    }
    return 0 != m_devList.size();
}

bool CDeviceMap::loadXML()
{
    QDomNodeList nodeList;
    if(!getNode(nodeList))
        return false;
    if(!processItem(nodeList))
        return false;
    if(!processMapSidSys(nodeList))
        return false;
	return true;
}

ITEM* CDeviceMap::getItem(const SUBITEM& subItem, bool isParam)
{
    int size = m_devList.size();
    for(int i = 0; i < size; ++i)
    {
        ITEM& tmp = m_devList[i];
        if(subItemInItem(subItem, tmp, isParam))
            return &m_devList[i];
    }
    return NULL;		
}

bool CDeviceMap::subItemInItem(const SUBITEM& subItem, const ITEM& item, bool isParam)
{
    //bool CDeviceMap::subItemQqualCMD_TID_SN(const SUBITEM& v1, const SUBITEM& v2)
    bool (CDeviceMap::*pfSubItemEqual)(const SUBITEM& , const SUBITEM&) = NULL;
    if(isParam)pfSubItemEqual = &CDeviceMap::subItemQqualCMD_TID_SN_DEVID_SUBSYSID;
    else pfSubItemEqual = &CDeviceMap::subItemQqualCMD_TID_SN;
    const QVector<SUBITEM>& tmpSubItem  = item.vSubItem;
    for(int i = 0; i < tmpSubItem.size(); ++i)
    {
        //((CDevCtlInterfaceGUI*)this->*fMsgProcess)(dataMsg);
        if(((CDeviceMap*)this->*pfSubItemEqual)(tmpSubItem[i], subItem))
            return true;
    }
    return false;
}

unsigned char CDeviceMap::getUintNo(const QString &moduleName, unsigned short usCMD, unsigned short usTID)
{
    ITEM * pItem = getItem(moduleName);
    if (pItem == NULL)
        return 0;

    for(int i = 0; i < pItem->vSubItem.size(); i++)
    {
        if(pItem->vSubItem[i].usCMD == usCMD &&
               pItem->vSubItem[i].usTID == usTID)
            return pItem->vSubItem[i].usUintNo;
    }
    return 0;
}

unsigned char CDeviceMap::getUintNo(ITEM *pItem, unsigned short usCMD, unsigned short usTID)
{
    if (pItem == NULL)
        return 0;

    for(int i = 0; i < pItem->vSubItem.size(); i++)
    {
        if(pItem->vSubItem[i].usCMD == usCMD &&
               pItem->vSubItem[i].usTID == usTID)
            return pItem->vSubItem[i].usUintNo;
    }
    return 0;
}

unsigned char CDeviceMap::getUintNo(unsigned short usCMD, unsigned short usTID, unsigned char ucSN)
{
    int nSize = m_subItemList.size();
    for (int i = 0; i < nSize; i++)
    {
        if (m_subItemList.at(i).ucSN == ucSN && m_subItemList.at(i).usCMD == usCMD
                && m_subItemList.at(i).usTID == usTID)
            return m_subItemList.at(i).usUintNo;
    }
    return 0;
}

SUBITEM *CDeviceMap::getObject(unsigned short usCMD, unsigned char ucSN, unsigned int deviceID)
{
    int nSize = m_subItemList.size();
    for (int i = 0; i < nSize; i++)
    {
        if (m_subItemList.at(i).ucSN == ucSN &&
                m_subItemList.at(i).usDeviceID == deviceID &&
                m_subItemList[i].usCMD == usCMD)
            return (SUBITEM*)&(m_subItemList.at(i));
    }
    return NULL;
}

ITEM* CDeviceMap::getItem(const QString& moduleName)
{
    int size = m_devList.size();
    for(int i = 0; i < size; ++i)
    {
        ITEM& tmp = m_devList[i];
        if(moduleName == tmp.strModuleName)
            return &m_devList[i];
    }
    return NULL;
}

bool CDeviceMap::getSubItemContent(SUBITEM& subItem, ITEM& item, bool isParam)
{
    bool (CDeviceMap::*pfSubItemEqual)(const SUBITEM& , const SUBITEM&) = NULL;
    if(isParam)pfSubItemEqual = &CDeviceMap::subItemQqualCMD_TID_SN_DEVID_SUBSYSID;
    else pfSubItemEqual = &CDeviceMap::subItemQqualCMD_TID_SN;
    QVector<SUBITEM>& tmpSubItems = item.vSubItem;
    QVector<SUBITEM>::iterator iter = tmpSubItems.begin();
    for(; iter != tmpSubItems.end(); ++iter)
    {
        if(((CDeviceMap*)this->*pfSubItemEqual)(subItem, *iter))
        {
            subItem = *iter;
            return true;
        }
    }
    return false;
}

bool CDeviceMap::subItemQqualCMD_TID_SN_DEVID_SUBSYSID(const SUBITEM& v1, const SUBITEM& v2)
{
    return v1.usCMD == v2.usCMD &&
            v1.usTID == v2.usTID &&
            v1.ucSN == v2.ucSN &&
            v1.usDeviceID == v2.usDeviceID &&
            v1.ucSubSysID == v2.ucSubSysID;
}

bool CDeviceMap::subItemQqualCMD_TID_SN(const SUBITEM& v1, const SUBITEM& v2)
{
    return v1.usCMD == v2.usCMD &&
            v1.usTID == v2.usTID &&
            v1.ucSN == v2.ucSN;
}
bool CDeviceMap::processMapSidSys(QDomNodeList &nodeList)
{
    int iCount = nodeList.count();
    QDomNode tmpNode;
    QDomElement tmpElement ;
    bool findMap = false;
    QDomNodeList tmpNodeList;
    for(int i = 0; i < iCount; i++)
    {
        tmpNode = nodeList.at(i);
        tmpElement = tmpNode.toElement();
        QString nodeName = tmpElement.tagName();
        if(nodeName.toLower().compare("map") == 0)
        {
            findMap = true;
            tmpNodeList  = tmpNode.childNodes();
            break;
        }
    }
    if(!findMap)
        return false;
    //subMap

    iCount = tmpNodeList.count();
    for(int i = 0; i < iCount; i++)
    {
        tmpNode = tmpNodeList.at(i);
        tmpElement = tmpNode.toElement();
        QString nodeName = tmpElement.tagName();

        if(nodeName.toLower().compare("submap") != 0)
            continue;
        if (!tmpElement.hasAttribute("sid"))
            return false;
        bool OK = true;
        int sid = tmpElement.attribute("sid").toInt(&OK, 16);
        if(!tmpElement.hasAttribute("system"))
            return false;
        QString sys = tmpElement.attribute("system");
        if(!tmpElement.hasAttribute("bbe"))
            return false;
        QString bbe = tmpElement.attribute("bbe");

        m_mpSysSid[sid] = QPair<QString, QString>(sys, bbe);
    }
    return 0 != m_mpSysSid.size();
}
SUBITEM*  ITEM::getSubItemByDeviceID_SN(const unsigned short& deviceID, const unsigned char& sn)
{
    for(int i = 0; i < vSubItem.size(); ++i)
    {

        if(vSubItem[i].ucSN == sn && vSubItem[i].usDeviceID == deviceID)
            return &vSubItem[i];
    }
    return NULL;
}
SUBITEM*  ITEM::getSubItemBySubSysID(unsigned char subSysID)
{
    for(int i = 0; i < vSubItem.size(); ++i)
    {
        if(vSubItem[i].ucSubSysID == subSysID)
            return &vSubItem[i];
    }
    return NULL;
}

SUBITEM *_tag_Item::getSubItemBySubSysIDAndDevID(unsigned char subSysID, unsigned char devID)
{
    for(int i = 0; i < vSubItem.size(); ++i)
    {
        if(vSubItem[i].ucSubSysID == subSysID && vSubItem[i].usDeviceID == devID)
            return &vSubItem[i];
    }
    return NULL;
}

