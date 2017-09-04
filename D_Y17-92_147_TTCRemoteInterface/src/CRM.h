#ifndef CRM_H
#define CRM_H
#include "../../Common/MCSDataStruct.h"
#include "../../Common/MCSUSBDataStruct.h"
#include "../../Common/DataStruct2311.h"
#include "../../Common/SMCUHeadFile.h"
#include "../../Common/SMCUSSB2_BBE_CDMAStruct.h"
#include "../../Common/SMCU_BBE_CDMAStruct.h"
#include "../../Common/DeviceMap.h"
#include "../../Common/struct_header/Dcu_2311_AK_DataStruct.h"
#include "platformresource.h"
#include "../../Common/interfaces/cdatasource.h"

class CRM
{
public:
    CRM();

    bool init(USHORT deviceID, PlatformResource* );
    PlatformResource* m_pPlatformRes;
    USHORT m_devID;
    QString m_name;
    //检前记录工作状态结构
    StatusFindResStruct *m_pStatusWorkRes;
    quint64 m_resID;
    //文件信息列表查询结构体
    RECORDFILELIST *m_pFileListRes;
    quint64 m_filesListID;
    //查询单个文件信息结构体
    RECORDFILEINFOCOMM *m_pFileInfoRes;
    quint64 m_fileInfoID;

//{0x00, 0x01200, 0x1255, 0x02}/*record三*/
    quint64 m_countId;

    //处理检前记录设备相关协议信息
    void processRecordMonitor(QByteArray &datagram);
    void getMap(BYTE *, const int dataLen, QMap<int, int>& mapFB_FE);
//    void updateAbstractAK();
    void getDevInfo(char &bbe);
    void set(SRM &srm);

    long m_connect;
    long m_connectOld;
    bool m_change;
    void get(SRM &srm);
};

#endif // CRM_H
