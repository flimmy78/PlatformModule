#ifndef PLATFORMRESOURCE_H
#define PLATFORMRESOURCE_H

#include "../../Common/LxTsspCommonDefinition.h"
#include "../../Common/DeviceMap.h"
#include "../../Common/TypeDefine.h"
//主要用于管理平台提供的资源
class PlatformResource
{
public:
    PlatformResource();
    ~PlatformResource();
    bool init(QString );
    bool msgRevRegister(QString strName);
    BYTE* ApplyOneItem(ushort usStationID, WORD wSrcSubSys,
                       WORD wDstSubSys, WORD wType,
                       WORD wID, int &dwLen);
    //查找对象管理器
    LxTsspObjectManager* m_pObjMgr;
    //状态对象管理器
    ILxTsspStateManager* m_pStatMgr;
    //消息对象管理器
    ILxTsspMessageManager* m_pMsgMgr;
    //宏对象管理器
    ILxTsspParamMacroManager* m_pMacroMgr;
    //日志对象管理器
    ILxTsspLogManager* m_pLogMgr;
    //channel对象管理器
    ILxTsspCommunicationManager* m_pChlMgr;
    ILxTsspPublicInfoManager* m_pPublicInfo;
    CDeviceMap m_devMap;
    BYTE *FindOneItem(ushort usStationID, WORD wSrcSubSys, WORD wDstSubSys, WORD wType, WORD wID, int &dwLen);
};

#endif // PLATFORMRESOURCE_H
