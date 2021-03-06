#include "ProcessModule.h"
#include "Global.h"
#include "XmlReader.h"
#include "../../Common/Log.h"
#include "DevCtlInterfaceGUI.h"
#include "DevCtlInterfaceGUI.h"
#include "DevCtlInterfaceGUIDlg.h"
#include "XmlReader.h"
#include "../../Common/Dcu_common_DataStruct.h"
#include <QDir>
#include <QDateTime>
#include <QTextCodec>
#include "../../Common/DeviceMap.h"
#include "../../Common/Log.h"
#include "../../Common/const.h"
#pragma pack(1)
#define INI_GROUP_STRUCT(MODULENAME, INDEX, STRUCT) \
    do{\
    QByteArray paramBlock;\
    qint64 tmpId = 0;\
    tmpId = CHelper::getMacroIDFromModuleNameAndIndex(MODULENAME, INDEX);\
    paramBlock.clear();\
    if(-1 == macroObj->GetParameterBlock(tmpId, paramBlock))\
{\
    CLog::addLog(QString(MODULENAME) + ", 宏参数获取失败.");\
    return;\
    }\
    memcpy(&STRUCT, paramBlock.data(), sizeof(STRUCT));\
    }while(false);

#define INI_GROUP_STRUCT_DEV(MODULENAME, DEV, INDEX, STRUCT) \
    do{\
    QByteArray paramBlock;\
    qint64 tmpId = 0;\
    tmpId = m_platformRes.getMacroIDFromModuleNameAndIndex(MODULENAME, DEV, INDEX);\
    paramBlock.clear();\
    if(-1 == macroObj->GetParameterBlock(tmpId, paramBlock))\
{\
    CLog::addLog(QString(MODULENAME) + ", 宏参数获取失败.");\
    return;\
    }\
    memcpy(&STRUCT, paramBlock.data(), sizeof(STRUCT));\
    }while(false);


int CDevCtlInterfaceGUI::getSN(int devID)
{
    if(0x400 == devID || 0x401 == devID)
    {
        if(m_connectStatu.m_pConnectSt->bbe[0] >= 5 && m_connectStatu.m_pConnectSt->bbe[0] <= 9)
            return (int)m_recvThread.m_bbe1.m_pBbeAbstract->DestMCSSysStatus.dwSystemMode;
        else if(m_connectStatu.m_pConnectSt->bbe[1] >= 5 && m_connectStatu.m_pConnectSt->bbe[1] <= 9)
            return (int)m_recvThread.m_bbe2.m_pBbeAbstract->DestMCSSysStatus.dwSystemMode;
        return -1;
    }
    else if(0x700 == devID || 0x701 == devID)
    {
        if(m_connectStatu.m_pConnectSt->ak[0] == 3)
        {
            return (int)m_recvThread.m_safeCtrl1.m_pMCSSysTarget4Status1->MCSSysStatu.ucSystemMode;
        }
        else if(m_connectStatu.m_pConnectSt->ak[1] == 3)
        {
            return (int)m_recvThread.m_safeCtrl2.m_pMCSSysTarget4Status1->MCSSysStatu.ucSystemMode;
        }
        return -1;
    }
    return -1;
}

bool CDevCtlInterfaceGUI::filter(TLxTsspMessageHeader *pCmdHead, TLxTsspSubMsgHeader *pCmdItemHead)
{
    if(0x12E == pCmdHead->C || 0x12A == pCmdHead->C || 0x201 == pCmdHead->C ||
            (0x7200 <= pCmdHead->C && 0x7206 >= pCmdHead->C) ||
            (0x7300 <= pCmdHead->C && 0x7304 >= pCmdHead->C) ||
            (0x7400 <= pCmdHead->C && 0x7404 >= pCmdHead->C) ||
            (0xFA01 <= pCmdHead->C && 0xFA04 >= pCmdHead->C) ||
            (0xFB01 <= pCmdHead->C && 0xFB08 >= pCmdHead->C) ||
            (0xFC01 <= pCmdHead->C && 0xFC08 >= pCmdHead->C) ||
            (0xFD01 <= pCmdHead->C && 0xFD08 >= pCmdHead->C) ||
            0xc0c == pCmdHead->C)
    {
        pCmdItemHead->O = getSN(pCmdItemHead->DID);
        SUBITEM* pSubItem = m_platformRes.m_devMap.getObject(pCmdHead->C, pCmdItemHead->O, pCmdItemHead->DID);
        if(!pSubItem)return true;
        pCmdItemHead->TID = pSubItem->usTID;
        char* pTmp = (char*)pCmdHead + sizeof(TLxTsspMessageHeader) + sizeof(TLxTsspSubMsgHeader);
        if(0x7300 == pCmdHead->C || 0x7400 == pCmdHead->C)pTmp[0] = 1;
        else if(0x7303 == pCmdHead->C || 0x7403 == pCmdHead->C)pTmp[0] = 2;

        if(0x7201 == pCmdHead->C)pTmp[0] = 1;
        else if(0x7202 == pCmdHead->C)pTmp[0] = 3;
        else if(0x7204 == pCmdHead->C)pTmp[0] = 1;
        else if(0x7205 == pCmdHead->C)pTmp[0] = 2;
    }
    else if(1793 == pCmdHead->C && 58521 == pCmdItemHead->TID)
        return true;
    else if(0x701 == pCmdHead->C){
        if(0xea01 == pCmdItemHead->TID || 0x1c07 == pCmdItemHead->TID || 0xe489 == pCmdItemHead->TID ||
                0xea03 == pCmdItemHead->TID || 0xea0f == pCmdItemHead->TID || 0xe495 == pCmdItemHead->TID ||
                0xea0b == pCmdItemHead->TID || 0xea0d == pCmdItemHead->TID || 0xea11 == pCmdItemHead->TID ||
                0xea20 == pCmdItemHead->TID)
            return true;
    }
    return false;
}
int CDevCtlInterfaceGUI::proMsg(TLxTsspMessage& msg)
{
    CHAR *pCmdFrm = msg.pData;
    int iLen = msg.iDataLen;
    TLxTsspMessageHeader *pCmdHead = (TLxTsspMessageHeader *)pCmdFrm;
    TLxTsspSubMsgHeader *pCmdItemHead = (TLxTsspSubMsgHeader *)(pCmdFrm + sizeof(TLxTsspMessageHeader));
    if (pCmdHead->L != iLen)
    {
        CLog::addLog("报文长度有误。");
        return 1;
    }
    else if(filter(pCmdHead, pCmdItemHead))//有些平台发来的包需要过滤
        return 1;

    //wl 20170327 修改传输参数问题
    DATACONTENT dataMsg;
    dataMsg.subItem.usCMD = pCmdHead->C;
    dataMsg.subItem.usTID = pCmdItemHead->TID;
    //如数传解调参数设置的模块,界面用O来区分相同结构体的不同实例
    dataMsg.subItem.ucSN = pCmdItemHead->O;//
    dataMsg.subItem.usDeviceID = pCmdItemHead->DID;
    dataMsg.subItem.ucSubSysID = pCmdItemHead->SN;
    bool isParam = true;
    ITEM* tmpItem = m_platformRes.m_devMap.getItem(dataMsg.subItem, isParam);

    if (!tmpItem){
        isParam = false;
        tmpItem = m_platformRes.m_devMap.getItem(dataMsg.subItem, false);
        if(!tmpItem){
            qDebug()<<"DeviceMapping.xml文件中没有该项, CMD:"<<QString::number(pCmdHead->C, 16)<<" TID:"
                   <<QString::number(pCmdItemHead->TID, 16);
            CLog::addLog(QString("DeviceMapping.xml文件中没有该项, CMD:") +
                         QString::number(pCmdHead->C) + " TID:" +
                         QString::number(pCmdItemHead->TID) + " SN_O: " +
                         QString::number(pCmdItemHead->O));
            return 1;
        }
    }
    PFProcessMsg fMsgProcess = GetProcessFunction(tmpItem->strModuleName);
    if (!fMsgProcess)
    {
        qDebug()<<"模块\""<<tmpItem->strModuleName<<"\"没有处理函数";
        CLog::addLog(QString("模块\"") + tmpItem->strModuleName + "\"没有处理函数");
        return 1;
    }
    dataMsg.iDataLen = pCmdItemHead->Len;
    dataMsg.pData = pCmdFrm + sizeof(TLxTsspMessageHeader) + sizeof(TLxTsspSubMsgHeader);
    m_platformRes.m_devMap.getSubItemContent(dataMsg.subItem, *tmpItem, isParam);
    dataMsg.iObjType = tmpItem->iObjType;
    dataMsg.seq = pCmdHead->uiSequence;
    ((CDevCtlInterfaceGUI*)this->*fMsgProcess)(dataMsg);
    return 1;
}

void CDevCtlInterfaceGUI::ProcessBasebandMonitorDataTrans(DATACONTENT& msg)
{
    char* tmpBuff = new char[msg.iDataLen + 3];
    //进行数据内容的封装
    //单元号
    int packLen = 0;
    memcpy(tmpBuff, (UCHAR*)&msg.subItem.usUintNo, 1);
    packLen += 1;
    //顺序号
    int indexLen = sizeof(unsigned short);
    unsigned short usIndex = (unsigned short)msg.seq;
    memcpy(tmpBuff + packLen, &usIndex, indexLen);
    ++usIndex;
    packLen += indexLen;
    memcpy(tmpBuff + packLen, msg.pData, msg.iDataLen);
    msg.iDataLen += 3;
    msg.pData = tmpBuff;
    qDebug()<<msg.subItem.szChannel;
    g_showDatagram.m_datagramCommSend = CHelper::sendBaseBandPro(msg);
    delete tmpBuff;
}

void CDevCtlInterfaceGUI::ProcessBasebandUSBRec(DATACONTENT& msg)
{
    char* tmpBuff = new char[msg.iDataLen + 3];
    //进行数据内容的封装
    //单元号
    int packLen = 0;
    memcpy(tmpBuff, (UCHAR*)&msg.subItem.usUintNo, 1);
    packLen += 1;
    //顺序号
    int indexLen = sizeof(unsigned short);
    unsigned short usIndex = (unsigned short)msg.seq;
    memcpy(tmpBuff + packLen, &usIndex, indexLen);
    ++usIndex;
    packLen += indexLen;
    memcpy(tmpBuff + packLen, msg.pData, msg.iDataLen);
    msg.iDataLen += 3;
    msg.pData = tmpBuff;
    qDebug()<<msg.subItem.szChannel;
    g_showDatagram.m_datagramCommSend = CHelper::sendBaseBandPro(msg);
    delete tmpBuff;
}
//jc 中频开关矩阵处理函数
void CDevCtlInterfaceGUI::ProcessSwitchMatrix(DATACONTENT& msg)
{
    g_showDatagram.m_datagramCommSend = CHelper::sendSwMatPro(msg);
}
bool CDevCtlInterfaceGUI::getChl(DATACONTENT& msg, int bbe1_bbe2, int ddt_tc_usb_ss1_ss2)
{
    if(0 == bbe1_bbe2)//基带1
    {
        if(5 == ddt_tc_usb_ss1_ss2)//ddt
        {
            memset(msg.subItem.szChannel, 0, 255);
            strcpy(msg.subItem.szChannel, "DDTSMCU1");
        }
        else if(6 == ddt_tc_usb_ss1_ss2)//tc
        {
            memset(msg.subItem.szChannel, 0, 255);
            strcpy(msg.subItem.szChannel, "TCSMCU1");
        }
        else if(7 == ddt_tc_usb_ss1_ss2)//ttc
        {
            memset(msg.subItem.szChannel, 0, 255);
            strcpy(msg.subItem.szChannel, "USBSMCU1");
        }
        else if(8 == ddt_tc_usb_ss1_ss2)//ss1
        {
            memset(msg.subItem.szChannel, 0, 255);
            strcpy(msg.subItem.szChannel, "SS1SMCU1");
        }
        else if(9 == ddt_tc_usb_ss1_ss2)//ss2
        {
            memset(msg.subItem.szChannel, 0, 255);
            strcpy(msg.subItem.szChannel, "SS2SMCU1");
        }
        else return false;
    }
    else if(1 == bbe1_bbe2)//基带2
    {
        if(5 == ddt_tc_usb_ss1_ss2)//ddt
        {
            memset(msg.subItem.szChannel, 0, 255);
            strcpy(msg.subItem.szChannel, "DDTSMCU2");
        }
        else if(6 == ddt_tc_usb_ss1_ss2)//tc
        {
            memset(msg.subItem.szChannel, 0, 255);
            strcpy(msg.subItem.szChannel, "TCSMCU2");
        }
        else if(7 == ddt_tc_usb_ss1_ss2)//ttc
        {
            memset(msg.subItem.szChannel, 0, 255);
            strcpy(msg.subItem.szChannel, "USBSMCU2");
        }
        else if(8 == ddt_tc_usb_ss1_ss2)//ss1
        {
            memset(msg.subItem.szChannel, 0, 255);
            strcpy(msg.subItem.szChannel, "SS1SMCU2");
        }
        else if(9 == ddt_tc_usb_ss1_ss2)//ss2
        {
            memset(msg.subItem.szChannel, 0, 255);
            strcpy(msg.subItem.szChannel, "SS2SMCU2");
        }
        else return false;
    }
    else return false;
    return true;
}
//组参数处理
void CDevCtlInterfaceGUI::ProcessGroupCMDBaseBandCommon(TLxTsspParamMacroInformation macroInfo,
                                                        ILxTsspParameterMacro* macroObj,DATACONTENT& msg,
                                                        QString baseBandName, int baseBandIndex)
{
    DevicesConnectStatus& devLinkStatu = *m_connectStatu.m_pConnectSt;
    if(devLinkStatu.bbe[baseBandIndex] == 0)
    {
        CLog::addLog(baseBandName + "不在线无法进行组参数设置！", 3);
        return;
    }
    if(devLinkStatu.bbeControl[baseBandIndex] == 0)
    {
        CLog::addLog(baseBandName + "处于分控无法进行组参数设置！", 3);
        return;
    }
    if(!getChl(msg, baseBandIndex, devLinkStatu.bbe[baseBandIndex]))
    {
        CLog::addLog(baseBandName + "通道获取失败！", 3);
        return;
    }
    int dev = baseBandIndex == 0 ? 0x400 : 0x401;
    // 基带
    if(macroInfo.aDevModeIdx[baseBandIndex] == 3 ||
            devLinkStatu.bbe[baseBandIndex] == 5)  // 数传
    {
        ProcessGroupDataTrans(macroObj, msg, baseBandName + "加载数传组参数！", dev);
    }
    else if(macroInfo.aDevModeIdx[baseBandIndex] == 4 ||
            devLinkStatu.bbe[baseBandIndex] == 6)  // TC
    {
        ProcessGroupTC(macroObj,msg, baseBandName + "加载TC组参数！", dev);
    }
    else if(macroInfo.aDevModeIdx[baseBandIndex] == 0 &&
            devLinkStatu.bbe[baseBandIndex] == 7)//5 TTC
    {
        ProcessGroupTTC(macroObj,msg, baseBandName + "加载TTC组参数！", dev);
    }
    else if(macroInfo.aDevModeIdx[baseBandIndex] == 1 &&
            devLinkStatu.bbe[baseBandIndex] == 8)// 6 ss1
    {
        ProcessGroupSMCU1(macroObj,msg, baseBandName + "加载ss1组参数！", dev);
    }
    else if(macroInfo.aDevModeIdx[baseBandIndex] == 2 &&
            devLinkStatu.bbe[baseBandIndex] == 9)//7 ss2
    {
        ProcessGroupSMCU2(macroObj,msg, baseBandName + "加载ss2组参数！", dev);
    }
    else
    {
        CLog::addLog(baseBandName + "当前模式与下发宏参数模式不对应，无法进行组参数设置！", 3);
    }
}
void CDevCtlInterfaceGUI::ProcessAlterInternal(ILxTsspParameterMacro* macroObj, QString mdlName, UCHAR index, char conn)
{
    if(0 != conn)
    {
        ITEM* pItem = m_platformRes.m_devMap.getItem(mdlName);
        if(NULL == pItem || pItem->vSubItem.size() <= index)return;
        SUBITEM sItem = pItem->vSubItem[index];
        ALTER_PARAM adAlPara;
        INI_GROUP_STRUCT(mdlName, index, adAlPara)
                ::SendCmdMessage((char*)&adAlPara, sizeof(adAlPara), sItem.usCMD, sItem.usTID);
    }

}
void CDevCtlInterfaceGUI::ProcessAlter(TLxTsspParamMacroInformation /*macroInfo*/,
                                       ILxTsspParameterMacro* macroObj,DATACONTENT& /*msg*/,
                                       QString /*baseBandName*/, int /*baseBandIndex*/)
{
    DevicesConnectStatus& devLinkStatu = *m_connectStatu.m_pConnectSt;
    if(0 != devLinkStatu.adjustup_alter)
    {
        ITEM* pItem = m_platformRes.m_devMap.getItem("校验变频器");
        if(NULL == pItem || pItem->vSubItem.size() <= 0)return;
        SUBITEM sItem = pItem->vSubItem[0];
        ADJUST_ALTER_PARAM adAlPara;
        INI_GROUP_STRUCT("校验变频器", 0, adAlPara)
                ::SendCmdMessage((char*)&adAlPara, sizeof(adAlPara), sItem.usCMD, sItem.usTID);
    }
    ProcessAlterInternal(macroObj, "上变频器", 0, devLinkStatu.up_alter[0]);
    ProcessAlterInternal(macroObj, "上变频器", 1, devLinkStatu.up_alter[1]);
    ProcessAlterInternal(macroObj, "下变频器", 0, devLinkStatu.down_alter[0]);
    ProcessAlterInternal(macroObj, "下变频器", 1, devLinkStatu.down_alter[1]);
}
void CDevCtlInterfaceGUI::ProcessGroupCMD(DATACONTENT& msg)
{
    uint macro = 0;
    memcpy(&macro, msg.pData, 4);
    TLxTsspParamMacroInformation macroInfo;
    m_platformRes.m_pMacroMgr->GetMacroInformation(macro, macroInfo);
    ILxTsspParameterMacro* macroObj = NULL;
    m_platformRes.m_pMacroMgr->GetMacro(macro, macroObj);
    ProcessGroupCMDBaseBandCommon(macroInfo, macroObj, msg, "基带一", 0);
    ProcessGroupCMDBaseBandCommon(macroInfo, macroObj, msg, "基带二", 1);
    //safe control
    DevicesConnectStatus& devLinkStatu = *m_connectStatu.m_pConnectSt;
    //msg.subItem.szChannel
    memset(msg.subItem.szChannel, 0, 255);
    strcpy(msg.subItem.szChannel, "LTCSMCU1");
    ProcessGroupSafeCtrlTC(macroInfo, macroObj, msg, devLinkStatu.ak[0], m_recvThread.m_safeCtrl1, "安控一", 0x700);
    memset(msg.subItem.szChannel, 0, 255);
    strcpy(msg.subItem.szChannel, "LTCSMCU2");
    ProcessGroupSafeCtrlTC(macroInfo, macroObj, msg, devLinkStatu.ak[1], m_recvThread.m_safeCtrl2, "安控二", 0x700);
}

void CDevCtlInterfaceGUI::ProcessGroupSafeCtrlTC(TLxTsspParamMacroInformation /*macroInfo*/,
                                                 ILxTsspParameterMacro* macroObj,DATACONTENT& msg, char ak, CTC& tc, QString name, int dev)
{
    if(ak == 0)
    {
        CLog::addLog(name + "不在线无法进行组参数设置！", 3);
        return;
    }//监控方式	0：无效， 1：分控， 2：本控
    if(tc.m_pMCSSysTarget4Status1->MCSSysStatu.ucMonitorMode != 2)
    {
        CLog::addLog(name + "处于分控或无效无法进行组参数设置！", 3);
        return;
    }
    ProcessGroupTC(macroObj, msg, name + "加载TC组参数！", dev);
}

void CDevCtlInterfaceGUI::ProcessGroupTC(ILxTsspParameterMacro* macroObj, DATACONTENT msg, QString log, int dev)
{
    TCGroup tc;
    CHelper::initTC(tc);
    INI_GROUP_STRUCT_DEV("单遥控系统工作参数设置", dev, 0, tc.GlobalSetPara1)
    INI_GROUP_STRUCT_DEV("单遥控调制参数设置", dev, 0, tc.UpModul3)
    INI_GROUP_STRUCT_DEV("单遥控遥控参数设置", dev, 0, tc.MCSSSTCParam5)
    INI_GROUP_STRUCT_DEV("单遥控扩频码参数设置", dev, 0, tc.mSSPNCode9)
    static USHORT seq = 0;
    tc.seq = seq ++;
    msg.pData = (char*)&tc;
    msg.iDataLen = sizeof(tc);
    g_showDatagram.m_datagramCommSend = CHelper::sendBaseBandPro(msg);
    CLog::addLog(log);
}

void CDevCtlInterfaceGUI::ProcessGroupDataTrans(ILxTsspParameterMacro* macroObj, DATACONTENT msg, QString log, int dev)
{
    MSCDataTransGroup dt;
    CHelper::initDataTrans(dt);

    INI_GROUP_STRUCT_DEV("DDT_BBE系统参数设置", dev, 0, dt.MCSSysParam)
    INI_GROUP_STRUCT_DEV("数传接收机参数设置", dev, 0, dt.MCSDDTRev)
    INI_GROUP_STRUCT_DEV("数传解调参数设置", dev, 0, dt.MCSDDTTMI)
    INI_GROUP_STRUCT_DEV("数传解调参数设置", dev, 1, dt.MCSDDTTMQ)
    INI_GROUP_STRUCT_DEV("数传信号源参数设置", dev, 0, dt.MCSDDTTMSimul)
    INI_GROUP_STRUCT_DEV("分集合成参数设置", dev, 0, dt.MCSDivCom)
    //INI_GROUP_STRUCT("存储转发单元设置", 0, dt.MCSDDTReplay)
    static USHORT seq = 0;
    dt.seq = seq ++;
    msg.pData = (char*)&dt;
    msg.iDataLen = sizeof(dt);
    g_showDatagram.m_datagramCommSend = CHelper::sendBaseBandPro(msg);
    CLog::addLog(log);
}

void CDevCtlInterfaceGUI::ProcessGroupTTC(ILxTsspParameterMacro* macroObj, DATACONTENT msg, QString log, int dev)
{
    USBTTCGroup ttc;
    CHelper::initTTC(ttc);
    INI_GROUP_STRUCT_DEV("USBBBE系统参数设置", dev, 0, ttc.MCSSysDataTTC1)
    INI_GROUP_STRUCT_DEV("USBBBE中频接收机参数设置", dev, 0, ttc.MCSUSBRev2)
    //INI_GROUP_STRUCT("USBBBE中频接收机参数设置", 1, ttc.MCSUSBRev3)
    INI_GROUP_STRUCT_DEV("USBBBE调制参数设置", dev, 0, ttc.MCSUSBUpModulParam4)
    INI_GROUP_STRUCT_DEV("USBBBE测距参数设置", dev, 0, ttc.MCSUSBCjParam5)
    //INI_GROUP_STRUCT("USBBBE测距参数设置", 1, ttc.MCSUSBCjParam6)
    INI_GROUP_STRUCT_DEV("USBBBE遥控参数设置", dev, 0, ttc.MCSUSBTCParam7)
    INI_GROUP_STRUCT_DEV("USBBBE遥测参数设置", dev, 0, ttc.MCSUSBTMParam8)
    INI_GROUP_STRUCT_DEV("USBBBE模拟源参数设置", dev, 0, ttc.MCSUSBTMSimulParamC)
    static WORD seq = 0;
    ttc.MCSSysDataTTC1.ucSystemOut = ttc.MCSUSBRev2.ucRevChannel;
    ttc.MCSSysDataTTC1.ucRTPPT = 1;
    ttc.MCSSysDataTTC1.ucDDTOutFormat = 1;
    ttc.seq = seq++;
    msg.pData = (char*)&ttc;
    msg.iDataLen = sizeof(ttc);
    g_showDatagram.m_datagramCommSend = CHelper::sendBaseBandPro(msg);
    CLog::addLog(log);
}

void CDevCtlInterfaceGUI::ProcessGroupSMCU1(ILxTsspParameterMacro* macroObj, DATACONTENT msg, QString log, int dev)
{
    SSBBEGROUPPARA smcu;
    CHelper::initSMCU(smcu);
    INI_GROUP_STRUCT_DEV("扩频1系统工作参数设置", dev, 0, smcu.GlobalSetPara1)
    INI_GROUP_STRUCT_DEV("模式1接收参数设置", dev, 0, smcu.mcsSSRec2)
    INI_GROUP_STRUCT_DEV("模式1调制参数设置", dev, 0, smcu.UpModul3)
    INI_GROUP_STRUCT_DEV("模式1测距测速参数设置", dev, 0, smcu.MCSSSCjParam4)
    INI_GROUP_STRUCT_DEV("扩频遥控参数设置", dev, 0, smcu.MCSSSTCParam5)
    INI_GROUP_STRUCT_DEV("扩频遥测解调参数设置", dev, 0, smcu.MCSSSTMParam6)
    INI_GROUP_STRUCT_DEV("扩频遥测模拟源参数设置", dev, 0, smcu.MCSSSTMSimulParam8)
    INI_GROUP_STRUCT_DEV("模式1扩频码设置", dev, 0, smcu.mSSPNCode9)
    static WORD seq = 0;
    smcu.seq = seq++;
    msg.pData = (char*)&smcu;
    msg.iDataLen = sizeof(smcu);
    g_showDatagram.m_datagramCommSend = CHelper::sendBaseBandPro(msg);
    //ProcessDopplerInMacro(msg, 41, seq);
    CLog::addLog(log);
}

void CDevCtlInterfaceGUI::ProcessGroupSMCU2(ILxTsspParameterMacro* macroObj, DATACONTENT msg, QString log, int dev)
{
    SS2BBEGROUPPARA smcu;
    CHelper::initSMCU2(smcu);
    INI_GROUP_STRUCT_DEV("扩频2系统工作参数设置", dev, 0, smcu.GlobalSetPara1)
    INI_GROUP_STRUCT_DEV("模式2接收参数设置", dev, 0, smcu.mcsSSRec2)
    INI_GROUP_STRUCT_DEV("模式2调制参数设置", dev, 0, smcu.UpModul3)
    INI_GROUP_STRUCT_DEV("模式2测距测速参数设置", dev, 0, smcu.MCSSSCjParam4)
    INI_GROUP_STRUCT_DEV("模式2遥控参数设置", dev, 0, smcu.MCSSSTCParam5)
    INI_GROUP_STRUCT_DEV("扩频遥测解调参数设置", dev, 1, smcu.MCSSSTMParam6)
    INI_GROUP_STRUCT_DEV("扩频遥测模拟源参数设置", dev, 1, smcu.MCSSSTMSimulParam8)
    INI_GROUP_STRUCT_DEV("模式2扩频码设置", dev, 0, smcu.mSSPNCode9)
    static WORD seq = 0;
    smcu.seq = seq++;
    msg.pData = (char*)&smcu;
    msg.iDataLen = sizeof(smcu);
    g_showDatagram.m_datagramCommSend = CHelper::sendBaseBandPro(msg);
    //ProcessDopplerInMacro(msg, 40, seq);
    CLog::addLog(log);
}
void CDevCtlInterfaceGUI::ProcessDopplerInMacro(DATACONTENT& msg, UCHAR unitNo, USHORT seq)
{
    struct{
        UCHAR unitNum;
        char m;
        USHORT seq;
        UCHAR chl;
        UCHAR compensateCtrl;
        UCHAR compensateType;
        UINT  compensatePeriod;
        USHORT resetSpeed;
    }dopplerCom;
    dopplerCom.unitNum = unitNo;
    dopplerCom.m = 'M';
    dopplerCom.seq = seq;
    dopplerCom.chl = 1;
    dopplerCom.compensateCtrl = 2;
    dopplerCom.compensateType = 2;//补偿方式
    dopplerCom.compensatePeriod = 1;//补偿周期
    dopplerCom.resetSpeed = 0;//回零速率
    msg.iObjType = 0xF001;
    msg.pData = (char*)&dopplerCom;
    msg.iDataLen = sizeof(dopplerCom);
    g_showDatagram.m_datagramCommSend = CHelper::sendBaseBandPro(msg);
}
void CDevCtlInterfaceGUI::ProcessControlCMD(DATACONTENT& msg)
{
    static USHORT usSeq = 0;
    char* tmpBuff = new char[msg.iDataLen + 3];
    memcpy(tmpBuff, msg.pData, 1);
    //tmpBuff[0] = msg.pData[0];//过程命令标识
    tmpBuff[1] = 'M';
    memcpy(tmpBuff+2, &usSeq, 2);//序号
    ++usSeq;
    memcpy(tmpBuff + 4, msg.pData+1, msg.iDataLen - 1);
    msg.pData = tmpBuff;
    msg.iDataLen += 3;
    g_showDatagram.m_datagramCommSend = CHelper::sendBaseBandPro(msg);
    delete tmpBuff;
}

void CDevCtlInterfaceGUI::ProcessControlCMD1(DATACONTENT& msg)
{
    DWORDLONG tmpLong;
    if(101 == msg.subItem.usUintNo)
    {
        uchar tmp;
        memcpy(&tmp, msg.pData, 1);
        tmpLong = tmp;
        msg.pData = (char*)&tmpLong;
        msg.iDataLen = sizeof(tmpLong);
    }
    char* tmpBuff = new char[ msg.iDataLen + 4];
    memcpy(tmpBuff, &msg.subItem.usUintNo, 1);//过程命令标识
    tmpBuff[1] = 'M';
    USHORT usSeq = (USHORT)msg.seq;
    memcpy(tmpBuff+2, &usSeq, 2);//序号
    memcpy(tmpBuff + 4, msg.pData, msg.iDataLen);
    msg.pData = tmpBuff;
    msg.iDataLen += 4;
    qDebug()<<msg.subItem.szChannel;
    g_showDatagram.m_datagramCommSend = CHelper::sendBaseBandPro(msg);
    delete tmpBuff;
}

void CDevCtlInterfaceGUI::ProcessControlCMD2(DATACONTENT& msg)
{
    DWORDLONG tmpLong;
    if(101 == msg.subItem.usUintNo)
    {
        uchar tmp;
        memcpy(&tmp, msg.pData, 1);
        tmpLong = tmp;
        msg.pData = (char*)&tmpLong;
        msg.iDataLen = sizeof(tmpLong);
    }
    char* tmpBuff = new char[ msg.iDataLen + 4];
    memcpy(tmpBuff, &msg.subItem.usUintNo, 1);//过程命令标识
    tmpBuff[1] = 'M';
    USHORT usSeq = (USHORT)msg.seq;
    memcpy(tmpBuff+2, &usSeq, 2);//序号
    memcpy(tmpBuff + 4, msg.pData, msg.iDataLen);
    msg.pData = tmpBuff;
    msg.iDataLen += 4;
    int posOfData = msg.iDataLen - 1;
    if(103 == msg.subItem.usUintNo)getMainMachineValue(msg.pData[posOfData]);
    g_showDatagram.m_datagramCommSend = CHelper::sendBaseBandPro(msg);
    if(getChl(msg.subItem))
    {
        //主备切换：1主，2备
        if(103 == msg.subItem.usUintNo)
            msg.pData[posOfData] = msg.pData[posOfData] == 1 ? 2 : 1;
        g_showDatagram.m_datagramCommSend = CHelper::sendBaseBandPro(msg);
    }
    delete tmpBuff;
}

void CDevCtlInterfaceGUI::getMainMachineValue(char& value)
{
    if(value != 0)return;
    if(m_connectStatu.m_pConnectSt->bbe[0] == 0 && m_connectStatu.m_pConnectSt->bbe[1] == 0)return;
    if(m_connectStatu.m_pConnectSt->bbe[0] != 0)
    {
        switch(m_connectStatu.m_pConnectSt->bbe[0])
        {
        case 7://usb
            value = m_recvThread.m_bbe1.m_pUSBAbstract->ucBBEWorkID == 1 ? 2 : 1;
            break;
        case 8://ss1
            value = m_recvThread.m_bbe1.m_pSS1Abstract->ucBBEWorkID == 1 ? 2 : 1;
            break;
        case 9://ss2
            value = m_recvThread.m_bbe1.m_pSS2Abstract->ucBBEWorkID == 1 ? 2 : 1;
            break;
        default:
            ;
        }
        return;
    }
    if(m_connectStatu.m_pConnectSt->bbe[1] != 0)
    {
        switch(m_connectStatu.m_pConnectSt->bbe[1])
        {
        case 7://usb
            value = m_recvThread.m_bbe2.m_pUSBAbstract->ucBBEWorkID == 1 ? 2 : 1;
            break;
        case 8://ss1
            value = m_recvThread.m_bbe2.m_pSS1Abstract->ucBBEWorkID == 1 ? 2 : 1;
            break;
        case 9://ss2
            value = m_recvThread.m_bbe2.m_pSS2Abstract->ucBBEWorkID == 1 ? 2 : 1;
            break;
        default:
            ;
        }
    }
}

bool CDevCtlInterfaceGUI::getChl(SUBITEM& subItem)
{
    QString tmp = subItem.szChannel;
    if(tmp == "LTCSMCU1")//基带一
    {
        memset(subItem.szChannel, 0, 255);
        memcpy(subItem.szChannel,"LTCSMCU2", 8);
    }
    else if(tmp == "USBSMCU1")
    {
        memset(subItem.szChannel, 0, 255);
        memcpy(subItem.szChannel,"USBSMCU2", 8);
    }
    else if(tmp == "SS1SMCU1")
    {
        memset(subItem.szChannel, 0, 255);
        memcpy(subItem.szChannel,"SS1SMCU2", 8);
    }
    else if(tmp == "SS2SMCU1")
    {
        memset(subItem.szChannel, 0, 255);
        memcpy(subItem.szChannel,"SS2SMCU2", 8);
    }
    else if(tmp == "LTCSMCU2")//基带二
    {
        memset(subItem.szChannel, 0, 255);
        memcpy(subItem.szChannel,"LTCSMCU1", 8);
    }
    else if(tmp == "USBSMCU2")
    {
        memset(subItem.szChannel, 0, 255);
        memcpy(subItem.szChannel,"USBSMCU1", 8);
    }
    else if(tmp == "SS1SMCU2")
    {
        memset(subItem.szChannel, 0, 255);
        memcpy(subItem.szChannel,"SS1SMCU1", 8);
    }
    else if(tmp == "SS2SMCU2")
    {
        memset(subItem.szChannel, 0, 255);
        memcpy(subItem.szChannel,"SS2SMCU1", 8);
    }
    else return false;
    return true;
}

void CDevCtlInterfaceGUI::ProcessControlCMD3(DATACONTENT& msg)
{
    if(!getChlUsedByMainMachine(msg.subItem))return;
    QString chl = msg.subItem.szChannel;
    DWORDLONG tmpLong;
    if(101 == msg.subItem.usUintNo)
    {
        uchar tmp;
        memcpy(&tmp, msg.pData, 1);
        tmpLong = tmp;
        msg.pData = (char*)&tmpLong;
        msg.iDataLen = sizeof(tmpLong);
    }
    else if(9 == msg.subItem.usUintNo)//ss1 ss2 上行载波输出 界面测控开始 测控结束
    {
        m_upCarrOut.ucCarrOut = msg.pData[0];
        CBaseband* pBbe = NULL;
        if(chl == "SS1SMCU1")
        {
            pBbe = &m_recvThread.m_bbe1;
            m_upCarrOut.cOutElectricalLevel = pBbe->m_ss1.m_pMCSSSUTargetTCSSUpModulStatu3->SSUpModulStatu.tParams.cWaveOutLevel;
        }
        else if(chl == "SS1SMCU2")
        {
            pBbe = &m_recvThread.m_bbe2;
            m_upCarrOut.cOutElectricalLevel = pBbe->m_ss1.m_pMCSSSUTargetTCSSUpModulStatu3->SSUpModulStatu.tParams.cWaveOutLevel;
        }
        else if(chl == "SS2SMCU1")
        {
            pBbe = &m_recvThread.m_bbe1;
            m_upCarrOut.cOutElectricalLevel = pBbe->m_ss2.m_pMCSUpModuleStatu4_3->m_MCSSSUpModulStatus.tParams.cWaveOutLevel;
        }
        else if(chl == "SS2SMCU2")
        {
            pBbe = &m_recvThread.m_bbe2;
            m_upCarrOut.cOutElectricalLevel = pBbe->m_ss2.m_pMCSUpModuleStatu4_3->m_MCSSSUpModulStatus.tParams.cWaveOutLevel;
        }
        m_upCarrOut.chlSel = 5;
        msg.iDataLen = sizeof(m_upCarrOut);
        msg.pData = (char*)&m_upCarrOut;
    }

    char* tmpBuff = new char[ msg.iDataLen + 4];
    memcpy(tmpBuff, &msg.subItem.usUintNo, 1);//过程命令标识
    tmpBuff[1] = 'M';
    USHORT usSeq = (USHORT)msg.seq;
    memcpy(tmpBuff+2, &usSeq, 2);//序号
    memcpy(tmpBuff + 4, msg.pData, msg.iDataLen);
    msg.pData = tmpBuff;
    msg.iDataLen += 4;

    g_showDatagram.m_datagramCommSend = CHelper::sendBaseBandPro(msg);
    if(14 == msg.subItem.usUintNo)
    {
        tmpBuff[0] = 15;//usb 遥控加调
        g_showDatagram.m_datagramCommSend = CHelper::sendBaseBandPro(msg);
    }

    delete tmpBuff;
}


void CDevCtlInterfaceGUI::ProcessControlCMD4(DATACONTENT& msg)
{
    g_showDatagram.m_datagramCommSend = CHelper::sendBaseBandPro(msg);
}

void CDevCtlInterfaceGUI::Process_SS1_SS2_SaveControlCMD(DATACONTENT &msg)
{
    struct{
        UCHAR unitNo;
        char m;
        USHORT seq;
        UINT uiM;
        UCHAR type;
    }save;
    static USHORT seq = 0;
    save.unitNo = msg.subItem.usUintNo;
    save.seq = seq++;
    save.type = msg.pData[0];
    msg.iDataLen = sizeof(save);
    msg.pData = (char*)&save;
    CBaseband* pbbe = NULL;
    QString chl = msg.subItem.szChannel;
    if(chl == "SS2SMCU1" || chl == "SS1SMCU1")pbbe = &m_recvThread.m_bbe1;
    else if(chl == "SS2SMCU2" || chl == "SS1SMCU2")pbbe = &m_recvThread.m_bbe2;
    else return;
    if(msg.subItem.usUintNo == 0x20){//ss1
        for(int i = 0; i < MAX_TARGET_COUNT; ++i){
            save.uiM = pbbe->m_ss1.m_pMCSSysTarget4Status1->targetSysStatu[i].tParams.uiM;
            g_showDatagram.m_datagramCommSend = CHelper::sendBaseBandPro(msg);
        }
    }else if(msg.subItem.usUintNo == 0x1F){//ss2
        for(int i = 0; i < MAX_TARGET_COUNT; ++i){
            save.uiM = pbbe->m_ss2.m_pMCSSysTarget4Status1->MCSSSB2TargetSysStatu[i].tParams.uiM;
            g_showDatagram.m_datagramCommSend = CHelper::sendBaseBandPro(msg);
        }
    }else return;
}
//处理存盘控制命令使用
void CDevCtlInterfaceGUI::ProcessSaveControlCMD(DATACONTENT &msg)
{
    //检前记录存盘命令处理
    if (msg.subItem.usCMD == 0x8201 && msg.subItem.usTID == 0)
    {
        bool bRM1 = (msg.subItem.usDeviceID == 0x1200) ? true : false;
        //检前记录存盘处理命令 发送记录存盘指令到设备上
        if (msg.pData[0] == 0x01)
        {
            //记录存储 开始
            RECORDCOMM rr;
            uint nStructLen = sizeof(RECORDCOMM);
            memset(&rr, 0, nStructLen);
            //当前时间
            QDateTime datetime = QDateTime::currentDateTime();
            QString date = datetime.toString("yyyyMMddhhmmss");

            //保留
            rr.M_CODE = 0;
            //保留
            memset(rr.M_NAME, 0, 16);
            memset(rr.R_COMMENT, 0, 101);
            memset(rr.FILENAME, 0, 64);
            //保留
            rr.M_ROUND = 0;
            //定时记录开始时间
            short Year = date.mid(0, 4).toUShort();
            BYTE Month = date.mid(4, 2).toUShort(0, 16);
            BYTE Day = date.mid(6, 2).toUShort(0, 16);
            BYTE Hour = date.mid(8, 2).toUShort(0, 16);
            BYTE Minute = date.mid(10, 2).toUShort(0, 16);
            BYTE Second = date.mid(12, 2).toUShort(0, 16);
            rr.R_TIMER.Year = Year;
            rr.R_TIMER.Month = Month;
            rr.R_TIMER.Date = Day;
            rr.R_TIMER.Hour = Hour;
            rr.R_TIMER.Minute = Minute;
            rr.R_TIMER.Second = Second;
            rr.R_TIMER.Reserved = 0;
            rr.tRecordTimeB.usDay = Day;
            rr.tRecordTimeB.Hour = Hour;
            rr.tRecordTimeB.Minute = Minute;
            rr.tRecordTimeB.Second = Second;
            rr.tRecordTimeB.ucLeap = 0; //1-表示闰年，0-表示平年

            //文件名为日期生成的文件名称
            QTextCodec *pCodec = QTextCodec::codecForName("GB2312");
            QByteArray ba = pCodec->fromUnicode(date);
            memcpy(rr.FILENAME, ba.data(), ba.length());
            rr.R_LENGTH = 0;
            //备注为空
            //QByteArray ba1 = m_pCodec->fromUnicode(recordDialog.m_strComment);
            //memcpy(rr.R_COMMENT, ba1.data(), ba1.length());
            rr.R_APPD_LEN = 0;//recordDialog.m_AppLen;
            rr.RecordType = 0;
            rr.TimeType = 0;
            rr.ucChannal1 = 0;
            rr.ucChannal2 = 0;
            rr.ucChannal3 = 0;
            rr.ucChannal4 = 0;
            BYTE cmd = 0x3a;
            ProcessRecordMonitorSaveCMD((char *)&rr, nStructLen, cmd, (char *)msg.subItem.szChannel, bRM1);
        }
        else if (msg.pData[0] == 0x02)
        {
            //记录存储停止
            qDebug() << "RecordMonitor save stop";
            BYTE cmd = 0x3c;
            char szBuf = { 0 };
            uint nLen = 0;
            ProcessRecordMonitorSaveCMD(&szBuf, nLen, cmd, (char *)msg.subItem.szChannel, bRM1);
        }
    }
}

bool CDevCtlInterfaceGUI::getChlUsedByMainMachine(SUBITEM& subItem)
{
    QString chlID = subItem.szChannel;
    if(m_connectStatu.m_pConnectSt->bbe[0] == 0 && m_connectStatu.m_pConnectSt->bbe[1] == 0)return false;
    CBaseband* pTmpBbe = &m_recvThread.m_bbe1;
    uchar ucTmpBBEWorkID = 1;
    if(m_connectStatu.m_pConnectSt->bbe[0] == 0 && m_connectStatu.m_pConnectSt->bbe[1] != 0)
    {
        //基带工作机号	  1：A套； 2：B套
        if(chlID == "USBSMCU1")
        {
            memset(subItem.szChannel, 0, 255);
            memcpy(subItem.szChannel,"USBSMCU2", 8);
        }
        else if(chlID == "SS1SMCU1")
        {
            memset(subItem.szChannel, 0, 255);
            memcpy(subItem.szChannel,"SS1SMCU2", 8);
        }
        else if(chlID == "SS2SMCU1")
        {
            memset(subItem.szChannel, 0, 255);
            memcpy(subItem.szChannel,"SS2SMCU2", 8);
        }
        else return false;
        return true;
    }
    else if(m_connectStatu.m_pConnectSt->bbe[0] != 0 && m_connectStatu.m_pConnectSt->bbe[1] != 0)
    {
        if(chlID == "USBSMCU1")
        {
            if(pTmpBbe->m_pUSBAbstract->ucBBEWorkID != ucTmpBBEWorkID)
            {
                memset(subItem.szChannel, 0, 255);
                memcpy(subItem.szChannel,"USBSMCU2", 8);
            }
        }
        else if(chlID == "SS1SMCU1")
        {
            if(pTmpBbe->m_pSS1Abstract->ucBBEWorkID != ucTmpBBEWorkID)
            {
                memset(subItem.szChannel, 0, 255);
                memcpy(subItem.szChannel,"SS1SMCU2", 8);
            }
        }
        else if(chlID == "SS2SMCU1")
        {
            if(pTmpBbe->m_pSS2Abstract->ucBBEWorkID != ucTmpBBEWorkID)
            {
                memset(subItem.szChannel, 0, 255);
                memcpy(subItem.szChannel,"SS2SMCU2", 8);
            }
        }
        else return false;
        return true;
    }
    return true;
}

void CDevCtlInterfaceGUI::ProcessBaseBandStatusCheck(DATACONTENT& msg)
{
    g_showDatagram.m_datagramStatuSend = CHelper::sendBaseBandPro(msg);
}

void CDevCtlInterfaceGUI::ProcessSswitchDcu(DATACONTENT& msg)
{
    g_showDatagram.m_datagramCommSend = CHelper::sendSwMatPro(msg);
}

void CDevCtlInterfaceGUI::ProcessCtrlCmd(DATACONTENT &msg)
{
    g_showDatagram.m_datagramCommSend = CHelper::sendCtrlData(msg);
}

void CDevCtlInterfaceGUI::ProcessRSPMarcoSearch(DATACONTENT& msg)
{
    g_showDatagram.m_datagramCommSend = CHelper::sendRSPData(msg);
}

void CDevCtlInterfaceGUI::ProcessRSPMarcoCall(DATACONTENT& msg)
{
    g_showDatagram.m_datagramCommSend = CHelper::sendRSPData(msg);
}

void CDevCtlInterfaceGUI::ProcessRSPMarcoCmdCtrl(DATACONTENT& msg)
{
    g_showDatagram.m_datagramCommSend = CHelper::sendRSPData(msg);
}

PFProcessMsg CDevCtlInterfaceGUI::GetProcessFunction(const QString& objName)
{
    QMap<QString, PFProcessMsg> namePF;
    namePF["DDT_BBE系统参数设置"] = &CDevCtlInterfaceGUI::ProcessBasebandMonitorDataTrans;//
    namePF["数传接收机参数设置"] = &CDevCtlInterfaceGUI::ProcessBasebandMonitorDataTrans;//
    namePF["数传解调参数设置"] = &CDevCtlInterfaceGUI::ProcessBasebandMonitorDataTrans;//
    namePF["分集合成参数设置"] = &CDevCtlInterfaceGUI::ProcessBasebandMonitorDataTrans;//
    namePF["数传信号源参数设置"] = &CDevCtlInterfaceGUI::ProcessBasebandMonitorDataTrans;//
    namePF["数传检测输出参数设置"] = &CDevCtlInterfaceGUI::ProcessBasebandMonitorDataTrans;//6
    namePF["USBBBE中频接收机参数设置"] = &CDevCtlInterfaceGUI::ProcessBasebandMonitorDataTrans;//
    namePF["USBBBE遥测参数设置"] = &CDevCtlInterfaceGUI::ProcessBasebandMonitorDataTrans;//
    namePF["USBBBE模拟源参数设置"] = &CDevCtlInterfaceGUI::ProcessBasebandMonitorDataTrans;//
    namePF["USBBBE测距参数设置"] = &CDevCtlInterfaceGUI::ProcessBasebandMonitorDataTrans;//
    namePF["USBBBE调制参数设置"] = &CDevCtlInterfaceGUI::ProcessBasebandMonitorDataTrans;//
    namePF["USBBBE遥控参数设置"] = &CDevCtlInterfaceGUI::ProcessBasebandMonitorDataTrans;//6
    namePF["USBBBE系统参数设置"] = &CDevCtlInterfaceGUI::ProcessBasebandMonitorDataTrans;//
    namePF["扩频1系统工作参数设置"] = &CDevCtlInterfaceGUI::ProcessBasebandMonitorDataTrans;//
    namePF["扩频2系统工作参数设置"] = &CDevCtlInterfaceGUI::ProcessBasebandMonitorDataTrans;//
    namePF["模式1接收参数设置"] = &CDevCtlInterfaceGUI::ProcessBasebandMonitorDataTrans;//找不到结构，延后。
    namePF["模式1调制参数设置"] = &CDevCtlInterfaceGUI::ProcessBasebandMonitorDataTrans;//
    namePF["扩频遥控参数设置"] = &CDevCtlInterfaceGUI::ProcessBasebandMonitorDataTrans;//
    namePF["扩频遥测解调参数设置"] = &CDevCtlInterfaceGUI::ProcessBasebandMonitorDataTrans;//6
    namePF["模式1测距测速参数设置"] = &CDevCtlInterfaceGUI::ProcessBasebandMonitorDataTrans;//
    namePF["扩频遥测模拟源参数设置"] = &CDevCtlInterfaceGUI::ProcessBasebandMonitorDataTrans;//
    namePF["扩频角误差输出参数设置"] = &CDevCtlInterfaceGUI::ProcessBasebandMonitorDataTrans;//找不到结构，延后
    namePF["模式1扩频码设置"] = &CDevCtlInterfaceGUI::ProcessBasebandMonitorDataTrans;//
    namePF["扩频检测输出控制"] = &CDevCtlInterfaceGUI::ProcessBasebandMonitorDataTrans;//找不到结构，延后
    namePF["模式2遥控参数设置"] = &CDevCtlInterfaceGUI::ProcessBasebandMonitorDataTrans;//
    namePF["模式2接收参数设置"] = &CDevCtlInterfaceGUI::ProcessBasebandMonitorDataTrans;//6找不到结构，延后。
    namePF["模式2调制参数设置"] = &CDevCtlInterfaceGUI::ProcessBasebandMonitorDataTrans;//
    namePF["模式2测距测速参数设置"] = &CDevCtlInterfaceGUI::ProcessBasebandMonitorDataTrans;//
    namePF["模式2扩频码设置"] = &CDevCtlInterfaceGUI::ProcessBasebandMonitorDataTrans;//
    namePF["检前记录设备监控"] = &CDevCtlInterfaceGUI::ProcessRecordMonitor;//找不到结构，延后
    namePF["测试开关矩阵监控"] = &CDevCtlInterfaceGUI::ProcessCtrlCmd;//
    namePF["上行中频开关矩阵监控"] = &CDevCtlInterfaceGUI::ProcessSwitchMatrix;//
    namePF["下行中频开关矩阵监控"] = &CDevCtlInterfaceGUI::ProcessSwitchMatrix;//
    namePF["射频切换开关监控"] = &CDevCtlInterfaceGUI::ProcessCtrlCmd;//
    namePF["射频分路组合监控"] = &CDevCtlInterfaceGUI::ProcessCtrlCmd;//
    namePF["标准TTC联试应答机监控"] = &CDevCtlInterfaceGUI::ProcessSwitchMatrix;//
    namePF["非相干扩频联试应答机监控"] = &CDevCtlInterfaceGUI::ProcessSwitchMatrix;//
    namePF["校零信标一体机监控"] = &CDevCtlInterfaceGUI::ProcessCtrlCmd;//
    namePF["铷钟机箱监控"] = &CDevCtlInterfaceGUI::ProcessCtrlCmd;//没实现
    namePF["频率基准源监控"] = &CDevCtlInterfaceGUI::ProcessCtrlCmd;//
    namePF["状态查询命令"] = &CDevCtlInterfaceGUI::ProcessBaseBandStatusCheck;
    namePF["检前状态查询命令"] = &CDevCtlInterfaceGUI::ProcessRecordMonitorSearchStatus;
    //wl add 联试应答机相关处理函数
    namePF[RSP_MACRO_SEARCH] = &CDevCtlInterfaceGUI::ProcessRSPMarcoSearch;
    namePF[RSP_MACRO_CALL] = &CDevCtlInterfaceGUI::ProcessRSPMarcoCall;
    namePF[RSPDCU_CMD] = &CDevCtlInterfaceGUI::ProcessRSPMarcoCmdCtrl;
    //wl end
    namePF["过程控制命令"] = &CDevCtlInterfaceGUI::ProcessControlCMD;//
    namePF["过程控制命令1"] = &CDevCtlInterfaceGUI::ProcessControlCMD1;
    namePF["过程控制命令2"] = &CDevCtlInterfaceGUI::ProcessControlCMD2;
    namePF["过程控制命令3"] = &CDevCtlInterfaceGUI::ProcessControlCMD3;
    namePF["过程控制命令4"] = &CDevCtlInterfaceGUI::ProcessControlCMD4;
    namePF["存盘控制命令"] = &CDevCtlInterfaceGUI::ProcessSaveControlCMD; //wl 添加存盘控制命令 只转发指令不生存信息源
    namePF["存盘控制命令SS"] = &CDevCtlInterfaceGUI::Process_SS1_SS2_SaveControlCMD;//ss1 ss2存盘
    namePF["送数控制"] = &CDevCtlInterfaceGUI::ProcessSendDataControlCMD;//
    namePF["模式切换"] = &CDevCtlInterfaceGUI::ProcessModeChangeControlCMD;//
    namePF["系统工作方式切换"] = &CDevCtlInterfaceGUI::ProcessWorkWayChangeControlCMD;//
    namePF["组参数设置命令"] = &CDevCtlInterfaceGUI::ProcessGroupCMD;
    namePF["remote_server宏处理"] = &CDevCtlInterfaceGUI::ProcessRemoteServerMacro;

    namePF["单遥控扩频码参数设置"] = &CDevCtlInterfaceGUI::ProcessBasebandMonitorDataTrans;
    namePF["单遥控调制参数设置"] = &CDevCtlInterfaceGUI::ProcessBasebandMonitorDataTrans;
    namePF["单遥控遥控参数设置"] = &CDevCtlInterfaceGUI::ProcessBasebandMonitorDataTrans;
    namePF["单遥控系统工作参数设置"] = &CDevCtlInterfaceGUI::ProcessBasebandMonitorDataTrans;
    QMap<QString, PFProcessMsg>::iterator it = namePF.find(objName);
    if (it == namePF.end())
        return NULL;
    return *it;
}

void CDevCtlInterfaceGUI::ProcessModeChangeControlCMD(DATACONTENT& msg)//usb ss1 ss2之间的切换
{
//    if(idx == 0)
//    {
//        _ucTemp = 1;
//        strTemp = "标准TTC";
//    }
//    else if(idx == 1)
//    {
//        _ucTemp  = 2;
//        strTemp = "扩频模式一";
//    }
//    else if(idx == 2)
//    {
//        _ucTemp = 3;
//        strTemp = "扩频模式二";
//    }
    UCHAR tmp = msg.pData[0];
    if(!getChlUsedByMainMachineForModeChange(msg.subItem))return;
    struct{
        UCHAR unitNo;
        char m;
        USHORT seq;
        UI64 mode;
    }modeChange;
    modeChange.unitNo = 101;//模式切换
    modeChange.m = 'M';
    if(1 == tmp)
        modeChange.mode = 0x0000000100000001;
    else if(2 == tmp)
        modeChange.mode = 0x0000000200000002;
    else if(3 == tmp)
        modeChange.mode = 0x0000000400000004;
    else return;
    msg.iDataLen = sizeof(modeChange);
    msg.pData = (char*)&modeChange;
    QByteArray tmpDatagram;
    tmpDatagram += CHelper::sendBaseBandPro(msg);
    getChl(msg.subItem);
    tmpDatagram += CHelper::sendBaseBandPro(msg);
    g_showDatagram.m_datagramCommSend = tmpDatagram;
}

void CDevCtlInterfaceGUI::ProcessWorkWayChangeControlCMD(DATACONTENT& msg)//实战任务 中频闭环等系统工作方式之间的切换
{
    if(!getChlUsedByMainMachineForModeChange(msg.subItem))return;
    struct{
        UCHAR unitNo;
        char m;
        USHORT seq;
        CHAR way;
    }workWay;
    workWay.unitNo = 102;//模式切换
    workWay.m = 'M';
    workWay.way = msg.pData[0];
    msg.iDataLen = sizeof(workWay);
    msg.pData = (char*)&workWay;
    QByteArray tmp;
    tmp += CHelper::sendBaseBandPro(msg);
    getChl(msg.subItem);
    tmp += CHelper::sendBaseBandPro(msg);
    g_showDatagram.m_datagramCommSend = tmp;
}
bool CDevCtlInterfaceGUI::getChlUsedByMainMachineForModeChange(SUBITEM& subItem)
{
//    case 7://usb
//        value = m_recvThread.m_bbe1.m_pUSBAbstract->ucBBEWorkID == 1 ? 2 : 1;
//        break;
//    case 8://ss1
//        value = m_recvThread.m_bbe1.m_pSS1Abstract->ucBBEWorkID == 1 ? 2 : 1;
//        break;
//    case 9://ss2
//        value = m_recvThread.m_bbe1.m_pSS2Abstract->ucBBEWorkID == 1 ? 2 : 1;
    QString chlID = subItem.szChannel;
    if(m_connectStatu.m_pConnectSt->bbe[0] == 0 && m_connectStatu.m_pConnectSt->bbe[1] == 0)return false;
    CBaseband* pTmpBbe = &m_recvThread.m_bbe1;
    uchar ucTmpBBEWorkID = 1;
    if(m_connectStatu.m_pConnectSt->bbe[0] == 0 && m_connectStatu.m_pConnectSt->bbe[1] != 0)
    {
        //基带工作机号	  1：A套； 2：B套
        //if(chlID == "USBSMCU1")
        if(7 == m_connectStatu.m_pConnectSt->bbe[1])
        {
            memset(subItem.szChannel, 0, 255);
            memcpy(subItem.szChannel,"USBSMCU2", 8);
        }
        //else if(chlID == "SS1SMCU1")
        else if(8 == m_connectStatu.m_pConnectSt->bbe[1]){
            memset(subItem.szChannel, 0, 255);
            memcpy(subItem.szChannel,"SS1SMCU2", 8);
        }
        //else if(chlID == "SS2SMCU1")
        else if(9 == m_connectStatu.m_pConnectSt->bbe[1]){
            memset(subItem.szChannel, 0, 255);
            memcpy(subItem.szChannel,"SS2SMCU2", 8);
        }
        else return false;
        return true;
    }
    else if(m_connectStatu.m_pConnectSt->bbe[0] != 0 && m_connectStatu.m_pConnectSt->bbe[1] == 0)
    {
        //基带工作机号	  1：A套； 2：B套
        //if(chlID == "USBSMCU1")
        if(7 == m_connectStatu.m_pConnectSt->bbe[0])
        {
            memset(subItem.szChannel, 0, 255);
            memcpy(subItem.szChannel,"USBSMCU1", 8);
        }
        //else if(chlID == "SS1SMCU1")
        else if(8 == m_connectStatu.m_pConnectSt->bbe[0]){
            memset(subItem.szChannel, 0, 255);
            memcpy(subItem.szChannel,"SS1SMCU1", 8);
        }
        //else if(chlID == "SS2SMCU1")
        else if(9 == m_connectStatu.m_pConnectSt->bbe[0]){
            memset(subItem.szChannel, 0, 255);
            memcpy(subItem.szChannel,"SS2SMCU1", 8);
        }
        else return false;
        return true;
    }
    else if(m_connectStatu.m_pConnectSt->bbe[0] != 0 && m_connectStatu.m_pConnectSt->bbe[1] != 0)
    {
        //if(chlID == "USBSMCU1")
        if(7 == m_connectStatu.m_pConnectSt->bbe[0])
        {
            if(pTmpBbe->m_pUSBAbstract->ucBBEWorkID != ucTmpBBEWorkID)
                setchl(subItem, "USBSMCU2");
            else setchl(subItem, "USBSMCU1");
        }
        //else if(chlID == "SS1SMCU1")
        else if(8 == m_connectStatu.m_pConnectSt->bbe[0]){
            if(pTmpBbe->m_pSS1Abstract->ucBBEWorkID != ucTmpBBEWorkID)
                setchl(subItem, "SS1SMCU2");
            else setchl(subItem, "SS1SMCU1");
        }
        //else if(chlID == "SS2SMCU1")
        else if(9 == m_connectStatu.m_pConnectSt->bbe[0]){
            if(pTmpBbe->m_pSS2Abstract->ucBBEWorkID != ucTmpBBEWorkID)
                setchl(subItem, "SS2SMCU2");
            else setchl(subItem, "SS2SMCU1");
        }
        else return false;
        return true;
    }
    return true;
}

void CDevCtlInterfaceGUI::setchl(SUBITEM &subItem, QString chl)
{
    memset(subItem.szChannel, 0, 255);
    memcpy(subItem.szChannel,chl.toStdString().c_str(), chl.size());
}
void CDevCtlInterfaceGUI::ProcessSendDataControlCMD(DATACONTENT& msg)
{
//    char* tmpBuff = new char[ msg.iDataLen + 4];
//    memcpy(tmpBuff, &msg.subItem.usUintNo, 1);//过程命令标识
//    tmpBuff[1] = 'M';
//    USHORT usSeq = (USHORT)msg.seq;
//    memcpy(tmpBuff+2, &usSeq, 2);//序号
//    memcpy(tmpBuff + 4, msg.pData, msg.iDataLen);
    UCHAR ctrl1 = msg.pData[0] == 1 ? 1 : 2;
    UCHAR ctrl2 = msg.pData[1] == 1 ? 1 : 2;
    //ddt
    struct{
        UCHAR unitNo;
        char m;
        USHORT seq;
        UCHAR type;
        UCHAR mode;
        UCHAR ctrl;
    }ddt;
    ddt.unitNo = 52;
    ddt.m = 'M';
    ddt.type = 100;//全部
    ddt.mode = 2;//人工
    ddt.ctrl = ctrl1;

    msg.pData = (char*)&ddt;
    msg.iDataLen = sizeof(ddt);
    //基带一 ddt usb
    setchl(msg.subItem, "DDTTCSMCU1");
    CHelper::sendBaseBandPro(msg);
    setchl(msg.subItem, "USBSMCU1");
    CHelper::sendBaseBandPro(msg);
    //基带二 ddt usb
    ddt.ctrl = ctrl2;
    setchl(msg.subItem, "DDTTCSMCU2");
    CHelper::sendBaseBandPro(msg);
    setchl(msg.subItem, "USBSMCU2");
    CHelper::sendBaseBandPro(msg);

    //基带 ss
    struct{
        UCHAR unitNo;
        char m;
        USHORT seq;
        UINT uiM;
        UCHAR type;
        UCHAR mode;
        UCHAR ctrl;
    }ss;
    ss.m = 'M';
    ss.type = 100;//全部
    ss.mode = 2;//人工
    msg.pData = (char*)&ss;
    msg.iDataLen = sizeof(ss);
    QByteArray tmp;
    for(int i = 0; i < MAX_TARGET_COUNT; ++i)
    {
        //ss1 基带一
        SMCU::MCSTargetSysParamStruct* pSs1Param= &m_recvThread.m_bbe1.m_ss1.m_pMCSSysTarget4Status1->targetSysStatu[i].tParams;
        ss.uiM = pSs1Param->uiM;
        ss.ctrl = ctrl1;
        ss.unitNo = 29;
        setchl(msg.subItem, "SS1SMCU1");
        tmp += CHelper::sendBaseBandPro(msg);
        //ss1 基带二
        pSs1Param= &m_recvThread.m_bbe2.m_ss1.m_pMCSSysTarget4Status1->targetSysStatu[i].tParams;
        ss.uiM = pSs1Param->uiM;
        ss.ctrl = ctrl2;
        setchl(msg.subItem, "SS1SMCU2");
        tmp += CHelper::sendBaseBandPro(msg);

        //ss2 基带一
        SMCU2::MCSSSB2TargetSysParamStruct* pSs2Param= &m_recvThread.m_bbe1.m_ss2.m_pMCSSysTarget4Status1->MCSSSB2TargetSysStatu[i].tParams;
        ss.uiM = pSs2Param->uiM;
        ss.ctrl = ctrl1;
        ss.unitNo = 28;
        setchl(msg.subItem, "SS2SMCU1");
        tmp += CHelper::sendBaseBandPro(msg);
        //ss2 基带二
        pSs2Param= &m_recvThread.m_bbe2.m_ss2.m_pMCSSysTarget4Status1->MCSSSB2TargetSysStatu[i].tParams;
        ss.uiM = pSs2Param->uiM;
        ss.ctrl = ctrl2;
        setchl(msg.subItem, "SS2SMCU2");
        tmp += CHelper::sendBaseBandPro(msg);
    }
    g_showDatagram.m_datagramCommSend = tmp;
}

void CDevCtlInterfaceGUI::ProcessRecordMonitorSearchStatus(DATACONTENT& msg)
{
    static uint nCount = 0;
    DataFrameStruct ds;
    //        typedef struct
    //        {
    //            char    REMI_Flag[4];
    //            int     TimeOfDay;
    //            DWORD   SenderIP;
    //            short   SenderPort;
    //            BYTE    SenderID;
    //            BYTE    Recv_ID;
    //            WORD    Counter;
    //            short   DataLen;
    //            BYTE    Command;
    //        }DataFrameStruct;
    //记录分系统监控信息帧标志
    memcpy(ds.REMI_Flag, "RSMI", 4);
    //每日从零时开始累积秒数
    QDate curDate = QDate::currentDate();
    ds.TimeOfDay = QTime::currentTime().secsTo(QTime(curDate.year(), curDate.month(), curDate.day()));
    //发送方IP地址
    char szlocalIP[] = { 192, 168, 151, 3 };
    memcpy((void*)&ds.SenderIP, szlocalIP, 4);
    //发送方UDP端口
    short sPort = 2000;
    ds.SenderPort = Local2Port(sPort);
    //发送方A/B机指示
    ds.SenderID = 'A';
    //接收方A/B机指示
    ds.Recv_ID = 'B';
    //监控命令发送计数
    ds.Counter = nCount++;
    //数据长度
    ds.DataLen = 1;
    ds.Command = 0x32;
    CHelper::sendData((uchar*)&ds, sizeof(DataFrameStruct), msg.subItem.szChannel);
    g_showDatagram.m_datagramStatuSend.clear();
    g_showDatagram.m_datagramStatuSend.resize(sizeof(ds));
    memcpy(g_showDatagram.m_datagramStatuSend.data(), &ds, sizeof(ds));
}

void CDevCtlInterfaceGUI::ProcessRecordMonitorSaveCMD(const char *pData, unsigned int &nLen,
                                                      const BYTE &byteCmd, char *pChannel, bool bRM1)
{
    static uint nCount1 = 0;
    static uint nCount2 = 0;
    uchar szSendData[1024] = { 0 };
    unsigned int nSendLen = 0;
    DataFrameStruct ds;
    uint nStructLen = sizeof(DataFrameStruct);
    memset(&ds, 0, nStructLen);
    //记录分系统监控信息帧标志
    memcpy(ds.REMI_Flag, "RSMI", 4);
    //每日从零时开始累积秒数
    QDate curDate = QDate::currentDate();
    ds.TimeOfDay = QTime::currentTime().secsTo(QTime(curDate.year(), curDate.month(), curDate.day()));
    //发送方IP地址
    char szlocalIP[] = { 192, 26, 3, 100 };
    memcpy((void*)&ds.SenderIP, szlocalIP, 4);
    //发送方UDP端口
    short sPort = 2000;
    ds.SenderPort = Local2Port(sPort);
    //发送方A/B机指示
    ds.SenderID = 'A';
    //接收方A/B机指示
    ds.Recv_ID = 'B';
    //监控命令发送计数
    //检前记录1和2不同使用的计数器不一样
    if (bRM1)
        ds.Counter = nCount1++;
    else
        ds.Counter = nCount2++;
    //数据长度
    ds.DataLen = 1 + nLen;
    ds.Command = byteCmd;
    memcpy(szSendData, (uchar*)&ds, nStructLen);
    nSendLen = nStructLen;
    memcpy(szSendData + nStructLen, pData, nLen);
    nSendLen += nLen;
    CHelper::sendData(szSendData, nSendLen, pChannel);
    g_showDatagram.m_datagramCommSend.clear();
    g_showDatagram.m_datagramCommSend.resize(nSendLen);
    memcpy(g_showDatagram.m_datagramCommSend.data(), szSendData, nSendLen);
}

short CDevCtlInterfaceGUI::Local2Port(short sValue)
{
    short sRetVal = 0;
    QByteArray tgSource;
    QDataStream tgStream(&tgSource, QIODevice::ReadWrite);
    tgStream.setByteOrder(QDataStream::BigEndian);
    tgStream << sRetVal;
    memcpy(&sValue, tgSource.data(), sizeof(short));
    return sRetVal;
}

void CDevCtlInterfaceGUI::ProcessRecordMonitor(DATACONTENT& msg)
{
    CHelper::sendData((uchar*)msg.pData, msg.iDataLen, msg.subItem.szChannel);
    g_showDatagram.m_datagramCommSend.clear();
    g_showDatagram.m_datagramCommSend.resize(msg.iDataLen);
    memcpy(g_showDatagram.m_datagramCommSend.data(), msg.pData, msg.iDataLen);
}

void CDevCtlInterfaceGUI::ProcessRemoteServerMacro(DATACONTENT& msg)
{
    QByteArray datagram;
    datagram.resize(msg.iDataLen);
    memcpy(datagram.data(), msg.pData, datagram.size());
    char tmp = datagram[datagram.size()-1];
    char buf[4096];
    datagram.remove(datagram.size()-1, 1);
    memcpy(buf, datagram.data(), datagram.size());
    ICommunicationChannel* pChannel = NULL;
    if(tmp == DDTSMCU1)
    {
        m_platformRes.m_pChlMgr->GetChannel("DDTSMCU1",pChannel);
    }
    else if(tmp == TCSMCU1)
    {
        m_platformRes.m_pChlMgr->GetChannel("TCSMCU1",pChannel);
    }
    else if(tmp == USBSMCU1)
    {
        m_platformRes.m_pChlMgr->GetChannel("USBSMCU1",pChannel);
    }
    else if(tmp == SS1SMCU1)
    {
        m_platformRes.m_pChlMgr->GetChannel("SS1SMCU1",pChannel);
    }
    else if(tmp==SS2SMCU1)
    {
        m_platformRes.m_pChlMgr->GetChannel("SS2SMCU1",pChannel);
    }
    else if(tmp == DDTSMCU2)
    {
        m_platformRes.m_pChlMgr->GetChannel("DDTSMCU2",pChannel);
    }
    else if(tmp == TCSMCU2)
    {
        m_platformRes.m_pChlMgr->GetChannel("TCSMCU2",pChannel);
    }
    else if(tmp == USBSMCU2)
    {
        m_platformRes.m_pChlMgr->GetChannel("USBSMCU2",pChannel);
    }
    else if(tmp == SS1SMCU2)
    {
        m_platformRes.m_pChlMgr->GetChannel("SS1SMCU2",pChannel);
    }
    else if(tmp==SS2SMCU2)
    {
        m_platformRes.m_pChlMgr->GetChannel("SS2SMCU2",pChannel);
    }
}

UINT CHelper::getBCD(int iYear, int iMonth, int iDay)
{
    //转换年
    int tmp = iYear % 10;
    int bcd = 0;
    bcd = bcd | tmp << 16;
    iYear = iYear / 10;
    tmp = iYear % 10;
    bcd = bcd | tmp << 20;
    iYear = iYear / 10;
    tmp = iYear % 10;
    bcd = bcd | tmp << 24;
    iYear = iYear / 10;
    tmp = iYear % 10;
    bcd = bcd | tmp << 28;

    //转换月
    tmp = iMonth % 10;
    bcd = bcd | tmp << 8;
    tmp = (iMonth / 10) % 10;
    bcd = bcd | tmp << 12;

    //转换日
    tmp = iDay % 10;
    bcd = bcd | tmp ;
    tmp = (iDay / 10) % 10;
    bcd = bcd | tmp << 4;

    return bcd;
}

UINT CHelper::getDate()
{
    QDate date = QDate::currentDate();
    int year(0), month(0), day(0);
    date.getDate(&year, &month, &day);
    return getBCD(year, month, day);
}

UINT CHelper::getTime()
{
    QTime time = QTime::currentTime();
    QTime time0(0, 0);
    UINT tmp = (UINT)(time0.msecsTo(time));
    return tmp;
}

void CHelper::sendData(uchar* pPack, int packLen, QString strID)
{
    m_pObjectManager = LxTsspObjectManager::Instance();
    if (!m_pObjectManager)
        return ;
    mpCommunicationMgr = (ILxTsspCommunicationManager *)m_pObjectManager->GetCommunicationManager();
    if (!mpCommunicationMgr)
        return ;
    ICommunicationChannel *pChannel = NULL;
    mpCommunicationMgr->GetChannel(strID, pChannel);
    if (NULL == pChannel)
    {
        CLog::addLog("get channel failed.........." );
        return ;
    }
    if(-1 == pChannel->SendData(pPack, packLen)){
        CLog::addLog("channel send pack failed.........." );
        return ;
    }
    return ;
}

QByteArray CHelper::sendSwMatPro(DATACONTENT& msg)
{
    PROCOTOL_PACK swMatPro;
    memset(&swMatPro, 0, sizeof(swMatPro));
    swMatPro.H = 0x01;
    swMatPro.SEQ = msg.seq;
    swMatPro.A = 0x02;
    swMatPro.C = msg.subItem.usCMD;
    swMatPro.Data.T1 = msg.subItem.usTID;
    swMatPro.S = swMatPro.Data.S1 = msg.subItem.uPDXPSMCU;
    swMatPro.D = swMatPro.Data.D1 = msg.subItem.uPDXPDEVICES;
    memcpy(&swMatPro.Data.Dat1, msg.pData, msg.iDataLen);
    swMatPro.L = sizeof(PROCOTOL_PACK) - sizeof(swMatPro.Data.Dat1) + msg.iDataLen;
    swMatPro.T = 0x01;
    swMatPro.Data.L1 = msg.iDataLen;
    CHelper::sendData((uchar*)&swMatPro, swMatPro.L, msg.subItem.szChannel);
    QByteArray datagram;
    datagram.resize(sizeof(swMatPro));
    memcpy(datagram.data(), &swMatPro, datagram.size());
    return datagram;
}

QByteArray CHelper::sendRSPData(DATACONTENT &msg)
{
    PROCOTOL_PACK pack;
    memset(&pack, 0, sizeof(pack));
    pack.H = 0x00;
    pack.SEQ = msg.seq;
    pack.M = 0x00;
    pack.T = 0x01;
    pack.L = sizeof(PROCOTOL_PACK) - sizeof(pack.Data.Dat1) + msg.iDataLen;
    pack.C = msg.subItem.usCMD;
    pack.A = 0x02;
    pack.S = pack.Data.S1 = msg.subItem.uPDXPSMCU;
    pack.D = pack.Data.D1 = msg.subItem.uPDXPDEVICES;
    pack.R = 0x00;
    pack.Data.T1 = msg.subItem.usTID;
    memcpy(&pack.Data.Dat1, msg.pData, msg.iDataLen);
    pack.Data.L1 = msg.iDataLen;
    CHelper::sendData((uchar*)&pack, pack.L, msg.subItem.szChannel);
    QByteArray datagram;
    datagram.resize(sizeof(pack));
    memcpy(datagram.data(), &pack, datagram.size());
    return datagram;
}

QByteArray CHelper::sendBaseBandPro(DATACONTENT& msg)
{
    //业务数据的长度为 序号长度 + 顺序号长度 + 数据包长度
    //进行网络协议数据帧封装
    NETSDFRM netSDF;
    memset(&netSDF, 0, sizeof(netSDF));
    netSDF.Date = CHelper::getDate();
    netSDF.Time = CHelper::getTime();
    netSDF.DataLen = msg.iDataLen;
    netSDF.BID = msg.iObjType;
    netSDF.DID = msg.subItem.uPDXPDEVICES;
    netSDF.SID = msg.subItem.uPDXPSMCU;
    unsigned char szBuf[MAX_SEND_BUFFER_LEN] = { 0 };
    int packLen = 0;
    int nHeadLen = sizeof(NETSDFRM);
    memcpy(szBuf + packLen, &netSDF, nHeadLen);
    packLen += nHeadLen;
    //封装具体的数据内容
    memcpy(szBuf + packLen, msg.pData, msg.iDataLen);
    packLen += msg.iDataLen;
    CHelper::sendData(szBuf, packLen, msg.subItem.szChannel);
    QByteArray datagram = (char*)szBuf;
    datagram.resize(packLen);
    memcpy(datagram.data(), szBuf, packLen);
    return datagram;
}

QByteArray CHelper::sendCtrlData(DATACONTENT &msg)
{
    PROCOTOL_PACK pack;
    memset(&pack, 0, sizeof(pack));
    pack.H = 0x00;
    pack.SEQ = msg.seq;
    pack.M = 0x00;
    pack.T = 0x01;
    pack.L = sizeof(PROCOTOL_PACK) - sizeof(pack.Data.Dat1) + msg.iDataLen;
    pack.C = msg.subItem.usCMD;
    pack.A = 0x02;
    pack.S = pack.Data.S1 = msg.subItem.uPDXPSMCU;
    pack.D = pack.Data.D1 = msg.subItem.uPDXPDEVICES;
    pack.R = 0x00;
    pack.Data.T1 = msg.subItem.usTID;
    memcpy(&pack.Data.Dat1, msg.pData, msg.iDataLen);
    pack.Data.L1 = msg.iDataLen;
    CHelper::sendData((uchar*)&pack, pack.L, msg.subItem.szChannel);
    QByteArray datagram;
    datagram.resize(sizeof(pack));
    memcpy(datagram.data(), &pack, datagram.size());
    return datagram;
}

QString CHelper::getXmlPath(QString xmlFileName)
{
    QDir dir;
    QString strPath = dir.currentPath();
    QString filePath = strPath + "/TM/System/ModuleConfig/" + xmlFileName;
    return filePath;
}

void CHelper::initDataTrans(MSCDataTransGroup &dt)
{
    dt.wUnit1Head  = 0x01FB;
    dt.wUnit1End   = 0x01FE;
    dt.wUnit15Head = 0x0FFB;
    dt.wUnit15End  = 0x0FFE;
    dt.wUnit16Head = 0x10FB;
    dt.wUnit16End  = 0x10FE;
    dt.wUnit17Head = 0x11FB;
    dt.wUnit17End  = 0x11FE;
    dt.wUnit18Head = 0x12FB;
    dt.wUnit18End  = 0x12FE;
    dt.wUnit19Head = 0x13FB;
    dt.wUnit19End  = 0x13FE;
    dt.wUnit20Head = 0x14FB;
    dt.wUnit20End  = 0x14FE;
}

void CHelper::initTTC(USBTTCGroup& ttc)
{
    ttc.wUnit1Head = 0x01FB;
    ttc.wUnit1End  = 0x01FE;
    ttc.wUnit2Head = 0x02FB;
    ttc.wUnit2End  = 0x02FE;
    //    ttc.wUnit3Head = 0x00FB;
    //    ttc.wUnit3End  = 0x00FE;
    ttc.wUnit4Head = 0x04FB;
    ttc.wUnit4End  = 0x04FE;
    ttc.wUnit5Head = 0x05FB;
    ttc.wUnit5End  = 0x05FE;
    //    ttc.wUnit6Head = 0x00FB;
    //    ttc.wUnit6End  = 0x00FE;
    ttc.wUnit7Head = 0x07FB;
    ttc.wUnit7End  = 0x07FE;
    ttc.wUnit8Head = 0x08FB;
    ttc.wUnit8End  = 0x08FE;
    ttc.wUnitCHead = 0x0CFB;
    ttc.wUnitCEnd  = 0x0CFE;
}

void CHelper::initSMCU(SSBBEGROUPPARA& smcu)
{
    smcu.wUnit1Head = 0x01FB;
    smcu.wUnit1End  = 0x01FE;
    smcu.wUnit2Head = 0x02FB;
    smcu.wUnit2End  = 0x02FE;
    smcu.wUnit3Head = 0x03FB;
    smcu.wUnit3End  = 0x03FE;
    smcu.wUnit4Head = 0x04FB;
    smcu.wUnit4End  = 0x04FE;
    smcu.wUnit5Head = 0x05FB;
    smcu.wUnit5End  = 0x05FE;
    smcu.wUnit6Head = 0x06FB;
    smcu.wUnit6End  = 0x06FE;
    smcu.wUnit8Head = 0x08FB;
    smcu.wUnit8End  = 0x08FE;
    smcu.wUnit9Head = 0x09FB;
    smcu.wUnit9End  = 0x09FE;
}
void CHelper::initSMCU2(SS2BBEGROUPPARA& smcu)
{
    smcu.wUnit1Head = 0x01FB;
    smcu.wUnit1End  = 0x01FE;
    smcu.wUnit2Head = 0x02FB;
    smcu.wUnit2End  = 0x02FE;
    smcu.wUnit3Head = 0x03FB;
    smcu.wUnit3End  = 0x03FE;
    smcu.wUnit4Head = 0x04FB;
    smcu.wUnit4End  = 0x04FE;
    smcu.wUnit5Head = 0x05FB;
    smcu.wUnit5End  = 0x05FE;
    smcu.wUnit6Head = 0x06FB;
    smcu.wUnit6End  = 0x06FE;
    smcu.wUnit8Head = 0x08FB;
    smcu.wUnit8End  = 0x08FE;
    smcu.wUnit9Head = 0x09FB;
    smcu.wUnit9End  = 0x09FE;
}
void CHelper::initTC(TCGroup &tc)
{
    tc.wUnit1Head = 0x01FB;
    tc.wUnit1End  = 0x01FE;
    tc.wUnit3Head = 0x03FB;
    tc.wUnit3End  = 0x03FE;
    tc.wUnit5Head = 0x05FB;
    tc.wUnit5End  = 0x05FE;
    tc.wUnit9Head = 0x09FB;
    tc.wUnit9End  = 0x09BE;
}
extern quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
                     uchar ucSn);

bool CHelper::HasDevicesConnectStatus()
{
    if(g_context.m_pDeviceLinkStatus == NULL)
    {
        TLxTsspObjectInfo& tmpInfo = g_context.m_lsTsspObjInfo;
        quint64 _id_temp = g_pILxTsspStateManager->GetID(tmpInfo.usStationID,
                                                         tmpInfo.ucSubSysID, g_context.m_usDeviceID,0x9013, 0);
        int iLen = sizeof(DevicesConnectStatus);
        g_context.m_pDeviceLinkStatus =
                (DevicesConnectStatus*)g_pILxTsspStateManager->FindOneItem(_id_temp, iLen);
        if(iLen != sizeof(DevicesConnectStatus) || g_context.m_pDeviceLinkStatus == NULL)
            return false;
    }
    return true;
}
#pragma pack()
