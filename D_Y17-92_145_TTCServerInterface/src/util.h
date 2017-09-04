#ifndef UTIL_H
#define UTIL_H
#include "../../Common/TypeDefine.h"

#define ARR_LEN(x) (sizeof(x) / sizeof(x[0]))
template<typename x>
bool  in_array(const x arr[], x v, int len)
{
    if(len <= 0) return false;
    for(int i = 0; i < len; i++)
    {
        if(arr[i] == v)
            return true;
    }
    return false;
}

BOOL ApplyDataBuf(PUCHAR &pDataBuf, ushort usStationID, uchar ucSubSysID,
                  ushort usDeviceID, ushort usType, uchar ucSn, INT nDataBuflen);

BOOL IncreaseTimeTag(ushort usStationID, uchar ucSubSysID,
                  ushort usDeviceID, ushort usType, uchar ucSn);

UINT GetTimeTag(ushort usStationID, uchar ucSubSysID,
                  ushort usDeviceID, ushort usType, uchar ucSn);

#endif // UTIL_H
