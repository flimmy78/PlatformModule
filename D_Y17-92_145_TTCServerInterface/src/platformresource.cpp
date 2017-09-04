#include "platformresource.h"
#include "Global.h"
#include <QMessageBox>


PlatformResource::PlatformResource()
{

}

PlatformResource::~PlatformResource()
{

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
        m_pPublicInfo = m_pObjMgr->GetPublicInfoManager();
        if(NULL == m_pPublicInfo)
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
    quint64 nID = m_pStatMgr->GetID(usStationID,\
                                              wSrcSubSys,\
                                              wDstSubSys, wType, wID);
    return m_pStatMgr->ApplyOneItem(nID, dwLen);
}


//4. 从信息元管理器中查找一个信息元（由信息元管理器申请一个节点和相应的数据空间）
//返回值：NULL--找不到；其它--指向信息元节点数据区的指针；dwLen:返回实际的长度
//参数：wSrcSubSys:源分系统号；wDstSubSys:目的分系统号；wType:信息元类型号；
//      wID:信息元ID；dwLen:申请数据的长度（信息元数据区长度）
BYTE* PlatformResource::FindOneItem(ushort usStationID, WORD wSrcSubSys, WORD wDstSubSys, WORD wType,\
                                        WORD wID, int &dwLen)
{
    quint64 nID = m_pStatMgr->GetID(usStationID,\
                                              wSrcSubSys,\
                                              wDstSubSys, wType, wID);
    return m_pStatMgr->FindOneItem(nID, dwLen);
}

