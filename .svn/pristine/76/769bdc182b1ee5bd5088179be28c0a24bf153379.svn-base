
#include "../Common/TypeDefine.h"
#include "Global.h"

//BOOL ApplyDataBuf(PUCHAR &pDataBuf, ushort usStationID,
//                                                 uchar ucSubSysID, ushort usDeviceID,
//                                                 ushort usType, uchar ucSn, INT nDataBuflen)
//{
//    if(!global_variants.g_pILxTsspStateManager)
//        return false;
//    int iLen = nDataBuflen;
//    quint64 _tempID = global_variants.g_pILxTsspStateManager->GetID(usStationID, ucSubSysID,
//                                                                    usDeviceID, usType, ucSn);
//    pDataBuf = global_variants.g_pILxTsspStateManager->ApplyOneItem(_tempID, iLen);
//    if (NULL == pDataBuf || iLen != nDataBuflen)
//    {
//        return false;
//    }
//    else
//    {
//        memset(pDataBuf, 0, iLen);
//        return true;
//    }
//} // ApplyDataBuf

BOOL IncreaseTimeTag(ushort usStationID,
                                                    uchar ucSubSysID,
                                                    ushort usDeviceID,
                                                    ushort usType, uchar ucSn)
{
    if(!g_pILxTsspStateManager)
        return false;
    quint64 _tempID = g_pILxTsspStateManager->GetID(usStationID, ucSubSysID,
                                                                    usDeviceID, usType, ucSn);
    g_pILxTsspStateManager->IncreaseTimeTag(_tempID);
    return true;
} // IncreaseTimeTag

UINT GetTimeTag(ushort usStationID, uchar ucSubSysID, ushort usDeviceID,
                          ushort usType, uchar ucSn)
{
    if(!g_pILxTsspStateManager)
        return false;
    quint64 _tempID = g_pILxTsspStateManager->GetID(usStationID, ucSubSysID,
                                                                    usDeviceID, usType, ucSn);
    return g_pILxTsspStateManager->GetTimeTag(_tempID);
} // IncreaseTimeTag
