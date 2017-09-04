#ifndef DCU2ACUINTERFACESTRUCT_H
#define DCU2ACUINTERFACESTRUCT_H
#include "TypeDefine.h"

#pragma pack(1)
typedef union _SERV_STATUS_STRUCT
{
    struct {
        char resv[1024];
    };
    struct
    {
        BYTE workMode;  // 伺服工作方式
        USHORT nAngleA; // 方位角；量纲：360.0/216（度）
        short nAngleE;     // 俯仰角；量纲：360.0/216（度）
    };
}SERV_STATUS_STRUCT;
#pragma pack()
#endif // DCU2ACUINTERFACESTRUCT_H
