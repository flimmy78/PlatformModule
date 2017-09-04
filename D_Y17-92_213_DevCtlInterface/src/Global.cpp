#define __ONCE_TEMP_

#include "Global.h"
#include <QMessageBox>
#include <QMap>
#include "ProcessModule.h"
#include "connectstatus.h"
#include <qdebug.h>
#include "../../Common/Log.h"

CDevCtlInterfaceGUI* g_devCtrl = NULL;
const int CGlobalContext::NODEV = -1;
void CGlobalContext::initIDs()
{
    DevCtl::DevicesConnectInfoID* dcInfo = DevCtl::devicesConnectInfo;
    if(NULL == g_pILxTsspStateManager)return;
    int i = 13;
    m_ddtId[0] = g_pILxTsspStateManager->GetID(g_context.m_lsTsspObjInfo.usStationID,
                                            dcInfo[i].subsysID, dcInfo[i].deviceID,
                                            dcInfo[i].typeNO, dcInfo[i].sn);//基带1
    quint64 tmp = m_ddtId[0];
    int size = 1;
    uchar *ptmp = g_pILxTsspStateManager->ApplyOneItem(tmp, size);
    i += 1;
    m_ddtId[1] = g_pILxTsspStateManager->GetID(g_context.m_lsTsspObjInfo.usStationID,
                                            dcInfo[i].subsysID, dcInfo[i].deviceID,
                                            dcInfo[i].typeNO, dcInfo[i].sn);//基带2
    tmp = m_ddtId[1];
    ptmp = g_pILxTsspStateManager->ApplyOneItem(tmp, size);
    i += 1;
    m_ddtId[2] = g_pILxTsspStateManager->GetID(g_context.m_lsTsspObjInfo.usStationID,
                                            dcInfo[i].subsysID, dcInfo[i].deviceID,
                                            dcInfo[i].typeNO, dcInfo[i].sn);//基带3
    tmp =m_ddtId[2];
    ptmp = g_pILxTsspStateManager->ApplyOneItem(tmp, size);
}

//3. 从信息元管理器中释放一个信息元（由信息元管理器申请一个节点和相应的数据空间）
//返回值：TRUE--释放成功；FALSE:释放失败；
//参数：wSrcSubSys:源分系统号；wDstSubSys:目的分系统号；wType:信息元类型号；
//      wID:信息元ID；
BOOL RemoveOneItem(ushort usStationID, WORD wSrcSubSys, WORD wDstSubSys, WORD wType,\
                                        WORD wID)
{
    if (FALSE == GetNeedManager())
    {
        return FALSE;
    }

    quint64 nID = g_pILxTsspStateManager->GetID(usStationID,\
                                              wSrcSubSys,\
                                              wDstSubSys,wType,wID);
    return g_pILxTsspStateManager->RemoveOneItem(nID);
}

//4. 从信息元管理器中查找一个信息元（由信息元管理器申请一个节点和相应的数据空间）
//返回值：NULL--找不到；其它--指向信息元节点数据区的指针；dwLen:返回实际的长度
//参数：wSrcSubSys:源分系统号；wDstSubSys:目的分系统号；wType:信息元类型号；
//      wID:信息元ID；dwLen:申请数据的长度（信息元数据区长度）
BYTE* FindOneItem(ushort usStationID, WORD wSrcSubSys, WORD wDstSubSys, WORD wType,\
                                        WORD wID, int &dwLen)
{
    if(FALSE == GetNeedManager())
    {
        return NULL;
    }

    quint64 nID = g_pILxTsspStateManager->GetID(usStationID,\
                                              wSrcSubSys,\
                                              wDstSubSys, wType, wID);
    return g_pILxTsspStateManager->FindOneItem(nID, dwLen);
}

//5. 更新信息元管理器中的一个节点内容，并通过DM发送出去(数据长度不能超过8K<8000>字节)
//返回值：NULL--更新失败；其它--指向信息元节点数据区的指针；dwSrcDataLen:返回实际的长度
//参数：wSrcSubSys:源分系统号；wDstSubSys:目的分系统号；wType:信息元类型号；
//      wID:信息元ID；pSrcData:新的数据；
//      dwSrcDataLen:新数据的长度（信息元数据区长度）
BYTE* UpdateOneItem(ushort usStationID, WORD wSrcSubSys, WORD wDstSubSys, WORD wType,\
                                  WORD wID, LPBYTE pSrcData, int &dwSrcDataLen)
{
    if (FALSE == GetNeedManager())
    {
        return NULL;
    }

    quint64 nID = g_pILxTsspStateManager->GetID(usStationID,\
                                              wSrcSubSys,\
                                              wDstSubSys, wType, wID);
    g_pILxTsspStateManager->UpdateOneItem(nID, pSrcData, dwSrcDataLen);

    int dwLen;
    return g_pILxTsspStateManager->FindOneItem(nID, dwLen);
}
//wl 20170331 目前存在问题为多通道接收数据问题，使用此种方法存在问题？
//启动链监线程
//if (bLinkThread == false)
//{
//    s_link = new RecvDataThread();
//    s_link->start();
//    bLinkThread = true;
//    //wl 20170331 启动状态发送线程实时发送设备状态信息到设备?
//    g_pSearchThread = new SearchStatusThread(0);
//    g_pSearchThread->start();
//    //jc 20170426 设备状态信息线程
//    g_pDeviceConnectStatu = new DevCtl::ConnectStatusCollect();
//    g_pDeviceConnectStatu->start();
//}
//wl 20170331 退出系统时停止和释放线程
//if (g_pSearchThread != NULL)
//{
//    g_pSearchThread->setStop();
//    delete g_pSearchThread;
//    g_pSearchThread = NULL;
//}
//if(NULL != s_link)
//{
//    s_link->setStop();
//    delete s_link;
//    s_link = NULL;
//}
//if(NULL != g_pDeviceConnectStatu)
//{
//    g_pDeviceConnectStatu->setBRunning(false);
//    delete g_pDeviceConnectStatu;
//    g_pDeviceConnectStatu = NULL;
//}
//退出系统时停止和释放线程


void MsgRev(TLxTsspMessage msg)
{
    g_devCtrl->proMsg(msg);
}

int SendCmdMessage(char* pData, unsigned short  usLen, unsigned usC, unsigned short usTID)
{
    LxTsspObjectManager* pObjectManager = LxTsspObjectManager::Instance();
    if(pObjectManager == NULL)
        return -1;
    ILxTsspPublicInfoManager* pPublicInfo = pObjectManager->GetPublicInfoManager();
    static uint seq = 0;
    TLxTsspMessageHeader header;
    memset(&header, 0, sizeof(TLxTsspMessageHeader));
    TLxTsspSubMsgHeader subHeader;
    memset(&subHeader, 0, sizeof(TLxTsspSubMsgHeader));
    header.uiSequence = seq++;
    header.uiHandle = 1;
    header.S = pPublicInfo->GetLocalDeviceID();
    header.L = sizeof(TLxTsspMessageHeader) +sizeof(TLxTsspSubMsgHeader) + usLen;
    header.D = pPublicInfo->GetLocalDeviceID();//0x09;
    header.T = 1;
    header.M = 1;
    header.P = 1;
    header.A = 0;
    header.R = 0;
    header.C = usC;

    subHeader.Len = usLen;
    subHeader.SID = pPublicInfo->GetLocalDeviceID();//m_DObjectInfo.usStationID;
    subHeader.DID = pPublicInfo->GetLocalDeviceID();//1024;
    subHeader.TID = usTID;
    subHeader.SN = 0;
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
    int iRet = 0;
    g_MessageManager = pObjectManager->GetMessageManager();
    if(g_MessageManager)
    {
    iRet= g_MessageManager->FireMessage(message);
    }
    delete[] message.pData;
    return iRet;
}

