#include "platformresource.h"
#include "Global.h"
#include <QMessageBox>


PlatformResource::PlatformResource():m_c_nonValue(0)
{
    m_nIDCurrent = m_c_nonValue;
}

PlatformResource::~PlatformResource()
{

}

bool PlatformResource::getNode(QDomNodeList &nodeList)
{
    QDir dir;
    QString strPath = dir.currentPath();
    QString filePath = strPath + "/System/LxTsspConfig/LxTsspConfig.xml";

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

bool PlatformResource::getChlsUsedByDevCtl(QList<ICommunicationChannel*>& ChannelList)
{
    QDomNodeList nodeList;
    if(!getNode(nodeList))return false;
    QDomNode tmpNode;
    QDomElement tmpElement;
    int iCount = nodeList.size();
    int i = 0;
    for(;i < iCount; i++)
    {
        tmpNode = nodeList.at(i);
        tmpElement = tmpNode.toElement();
        QString nodeName = tmpElement.tagName();
        if(nodeName.toLower().compare("tm") == 0)
            break;
    }
    if(i == iCount)return false;

    nodeList = tmpNode.childNodes();
    iCount = nodeList.size();
    for(i = 0; i < iCount; i++)
    {
        tmpNode = nodeList.at(i);
        tmpElement = tmpNode.toElement();
        QString nodeName = tmpElement.tagName();
        if(nodeName.toLower().compare("module") != 0)
            continue;
        if (!tmpElement.hasAttribute("Note"))
            return false;

        QString strNote = tmpElement.attribute("Note");
        qDebug()<<strNote;
        if(strNote != "遥测遥控远程监控接口")continue;
        break;
    }
    if(i == iCount)return false;

    nodeList = tmpNode.childNodes();
    iCount = nodeList.size();
    QVector<QString> vChls;
    for(i = 0; i < iCount; i++)
    {
        tmpNode = nodeList.at(i);
        tmpElement = tmpNode.toElement();
        if (!tmpElement.hasAttribute("chl"))
            continue;
        vChls.push_back(tmpElement.attribute("chl"));
    }
    if(vChls.isEmpty())return false;
    ICommunicationChannel* pChannel = NULL;
    iCount = vChls.size();
    for(i = 0; i < iCount; ++i)
    {
        if(-1 == m_pChlMgr->GetChannel(vChls[i], pChannel))
            continue;
        ChannelList.append(pChannel);
    }
    return !ChannelList.isEmpty();
}
//1.获取对象管理器和状态管理器
bool PlatformResource::init(QString strName)
{
    bool isOK = false;
    do
    {
        //查找对象管理器
        m_pObjMgr = LxTsspObjectManager::Instance();
        if(NULL == m_pObjMgr)
            break;
        //状态对象管理器
        m_pStatMgr = m_pObjMgr->GetStateManager();
        if(NULL == m_pStatMgr)
            break;
        //消息对象管理器
        m_pMsgMgr = m_pObjMgr->GetMessageManager();
        if(NULL == m_pMsgMgr)
            break;
        if(-1 == m_pMsgMgr->MsgReceiverRegister(0xFFFF, 0xFFFF, strName, MsgRev))
        {
            QMessageBox msgBox;
            msgBox.setText("动作命令模块注册接收命令失败!");
            msgBox.exec();
            break;
        }
        //宏对象管理器
        m_pMacroMgr = m_pObjMgr->GetParamMacroManager();
        if(NULL == m_pMacroMgr)
            break;
        //日志对象
        m_pLogMgr = m_pObjMgr->GetLogManager();
        if(NULL == m_pLogMgr)
            break;
        //channel对象管理器
        m_pChlMgr = m_pObjMgr->GetCommunicationManager();
        if(NULL == m_pChlMgr)
            break;
        if(false == m_devMap.loadXML())
            break;
        isOK = true;
    }while(false);
    return isOK;
}

//2. 从信息元管理器中申请一个信息元（由信息元管理器申请一个节点和相应的数据空间）
//返回值：
//参数：wSrcSubSys:源分系统号；wDstSubSys:目的分系统号；wType:信息元类型号；
//      wID:信息元ID；
BYTE* PlatformResource::ApplyOneItem(ushort usStationID, WORD wSrcSubSys, WORD wDstSubSys, WORD wType,\
                                         WORD wID, int &dwLen)
{
    m_nIDCurrent = m_pStatMgr->GetID(usStationID,\
                                              wSrcSubSys,\
                                              wDstSubSys, wType, wID);
    return m_pStatMgr->ApplyOneItem(m_nIDCurrent, dwLen);
}

BYTE*  PlatformResource::FindOneItem(ushort usStationID,  WORD wSrcSubSys, WORD wDstSubSys, WORD wType,\
                                        WORD wID, int &dwLen)
{
    m_nIDCurrent = m_pStatMgr->GetID(usStationID,\
                                              wSrcSubSys,\
                                              wDstSubSys, wType, wID);
    return m_pStatMgr->FindOneItem(m_nIDCurrent, dwLen);
}

qint64 PlatformResource::getMacroIDFromModuleNameAndIndex(QString moduleName, int devID, int index)
{
    ITEM* pItem = m_devMap.getItem(moduleName);
    if(!pItem)return 0;
    SUBITEM* pSItem = pItem->getSubItemByDeviceID_SN(devID, index);
    if(!pSItem)return 0;
    SUBITEM& sItem = *pSItem;
    return GetID(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID, sItem.usTID, sItem.ucSN);
}

quint64 PlatformResource::GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
              uchar ucSn)
{
    quint64 ui64ID = 0;

    ui64ID = ((((quint64)usStationID & 0xFFFF) << 48) + (((quint64)ucSubSysID & 0xFF) << 40)
              + (((quint64)usDeviceID & 0xFFFF) << 24) + (((quint64)usType & 0xFFFF) << 8)
              + ((quint64)ucSn & 0xFF));

    return ui64ID;
}

UINT PlatformResource::GetTimeTag(ushort usStationID, uchar ucSubSysID, ushort usDeviceID,
                          ushort usType, uchar ucSn, bool* pOk)
{
    if(pOk)*pOk = true;
    if(!m_pStatMgr){
        if(pOk)*pOk = false;
        return 0;
    }
    quint64 _tempID = m_pStatMgr->GetID(usStationID, ucSubSysID, usDeviceID, usType, ucSn);
    return m_pStatMgr->GetTimeTag(_tempID);
} // IncreaseTimeTag
