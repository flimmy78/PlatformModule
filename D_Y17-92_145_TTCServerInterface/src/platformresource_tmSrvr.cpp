#include "platformresource_tmSrvr.h"
#include "Global.h"
#include <QMessageBox>


PlatformResource::PlatformResource()
{

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
        QString nodeName = tmpElement.tagName();
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
bool PlatformResource::init(QString strName, TLxTsspObjectInfo& objInfo)
{
    memcpy(&m_ObjectInfo, & objInfo, sizeof(objInfo));
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
//        if(-1 == m_pMsgMgr->MsgReceiverRegister(0xFFFF, 0xFFFF, strName, MsgRev))
//        {
//            QMessageBox msgBox;
//            msgBox.setText("动作命令模块注册接收命令失败!");
//            msgBox.exec();
//            break;
//        }
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
        m_pPublicInfo = m_pObjMgr->GetPublicInfoManager();
        if(!m_pPublicInfo)break;
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
    quint64 nID = m_pStatMgr->GetID(usStationID,\
                                              wSrcSubSys,\
                                              wDstSubSys, wType, wID);
    return m_pStatMgr->ApplyOneItem(nID, dwLen);
}

BYTE*  PlatformResource::FindOneItem(ushort usStationID,  WORD wSrcSubSys, WORD wDstSubSys, WORD wType,\
                                        WORD wID, int &dwLen)
{
    quint64 nID = m_pStatMgr->GetID(usStationID,\
                                              wSrcSubSys,\
                                              wDstSubSys, wType, wID);
    return m_pStatMgr->FindOneItem(nID, dwLen);
}

void PlatformResource::SendMessage(char* pData, unsigned short  usLen, unsigned short usCMD, unsigned short usTID)
{
    static uint seq = 0;
    TLxTsspMessageHeader header;
    memset(&header, 0, sizeof(header));
    TLxTsspSubMsgHeader subHeader;
    memset(&subHeader, 0, sizeof(subHeader));
    header.uiSequence = seq++;
    header.uiHandle = 1;
    header.S = m_pPublicInfo->GetLocalDeviceID();
    header.L = sizeof(TLxTsspMessageHeader) +sizeof(TLxTsspSubMsgHeader) + usLen;
    header.D = m_pPublicInfo->GetLocalDeviceID();//0x09;
    header.T = 1;
    header.M = 1;
    header.P = 1;
    header.A = 0;
    header.R = 0;
    header.C = usCMD;

    subHeader.Len = usLen;
    subHeader.SID = m_pPublicInfo->GetLocalDeviceID();//m_DObjectInfo.usStationID;
    subHeader.DID = m_ObjectInfo.usLocalDID;
    subHeader.TID = usTID;
    subHeader.SN = m_ObjectInfo.ucSubSysID;
    subHeader.O = 0;
    TLxTsspMessage message;
    message.pData =
            new char[sizeof(TLxTsspMessageHeader) + sizeof(TLxTsspSubMsgHeader) + usLen ];
    memcpy(message.pData, &header, sizeof(TLxTsspMessageHeader));
    memcpy(message.pData + sizeof(TLxTsspMessageHeader),
           &subHeader, sizeof(TLxTsspSubMsgHeader));
    memcpy(message.pData + sizeof(TLxTsspMessageHeader) + sizeof(TLxTsspSubMsgHeader),
           pData, usLen);
    message.iDataLen = sizeof(TLxTsspMessageHeader) + sizeof(TLxTsspSubMsgHeader) + usLen;

    m_pMsgMgr->FireMessage(message);
    delete[] message.pData;
}

