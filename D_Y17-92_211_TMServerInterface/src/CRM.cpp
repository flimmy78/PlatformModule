#include "CRM.h"
#include "../../Common/Log.h"
#include "DataStructTMServerInterface.h"
#include "Global.h"


CRM::CRM()
{
    m_connect = m_connectOld = 0;
    m_resID = 0;
    m_filesListID = 0;
    m_fileInfoID = 0;
}
bool CRM::init(USHORT deviceID, PlatformResource* pRes)
{
    m_devID = deviceID;
    m_name = m_devID == 0x1200 ? "检前1" : "检前2";
    m_pPlatformRes = pRes;
    DATA_SOURCE_ALLOC(StatusFindResStruct, m_pStatusWorkRes, "检前记录设备监控", deviceID, 0)
    DATA_SOURCE_ALLOC(RECORDFILELIST, m_pFileListRes, "检前记录设备监控", deviceID, 1)
    DATA_SOURCE_ALLOC(RECORDFILEINFOCOMM, m_pFileInfoRes, "检前记录设备监控", deviceID, 2)
    return true;
}

void CRM::get(SRM& rm)
{
    memcpy(&rm.m_StatusWorkRes, m_pStatusWorkRes, sizeof(rm.m_StatusWorkRes));
    memcpy(&rm.m_FileInfoRes, m_pFileInfoRes, sizeof(rm.m_FileInfoRes));
    memcpy(&rm.m_FileListRes, m_pFileListRes, sizeof(rm.m_FileListRes));
}

void CRM::getMap(BYTE* pDataField, const int dataLen, QMap<int, int>& mapFB_FE)
{
    for(int i = 0; i < dataLen; ++i)
    {
        int tmp = pDataField[i];
        QPair<int, int> pairFB_FE;
        if(0xFB != tmp)continue;
        pairFB_FE.first = i;
        for(int j = i; j < dataLen; ++j)
        {
            tmp = pDataField[j];
            if(0xFE != tmp)continue;
            i = pairFB_FE.second = j;
            break;
        }
        mapFB_FE[pairFB_FE.first] = pairFB_FE.second;
    }
}

void CRM::processRecordMonitor(QByteArray &datagram)
{
    m_pPlatformRes->m_pStatMgr->IncreaseTimeTag(m_resID);
    m_pPlatformRes->m_pStatMgr->IncreaseTimeTag(m_filesListID);
    m_pPlatformRes->m_pStatMgr->IncreaseTimeTag(m_fileInfoID);
    ++m_connect;
    QString log;
    DataFrameStruct rm;
    int rmLen = sizeof(DataFrameStruct);
    memcpy(&rm, datagram.data(), rmLen);
    switch(rm.Command)
    {
    case 0x31:
        //挂钩命令响应
        log = m_name + "挂钩命令响应";
        CLog::addLog(log);
        break;
    case 0x33:
        //状态查询命令响应
        memcpy(m_pStatusWorkRes, datagram.data() + rmLen, sizeof(StatusFindResStruct));
        break;
    case 0x37:
        //文件列表查询命令响应
        memcpy(m_pFileListRes, datagram.data() + rmLen, sizeof(RECORDFILELIST));
        break;
    case 0x39:
        //文件信息查询命令响应
        memcpy(m_pFileInfoRes, datagram.data() + rmLen, sizeof(RECORDFILEINFOCOMM));
        break;
    case 0x3B:
        //记录命令响应
        memcpy(m_pStatusWorkRes, datagram.data() + rmLen, sizeof(StatusFindResStruct));
        break;
    case 0x3D:
        //停止命令响应
        log = m_name + "停止命令响应";
        CLog::addLog(log);
        break;
    case 0x41:
        //回放命令响应
        memcpy(m_pStatusWorkRes, datagram.data() + rmLen, sizeof(StatusFindResStruct));
        break;
    case 0x49:
        //关机命令响应  记录日志即可
        //memcpy(m_pStatusWorkRes, datagram.data() + rmLen, sizeof(StatusFindResStruct));
        log = m_name + "关机命令响应";
        CLog::addLog(log);
        break;
    case 0x7F:
        //拒收命令响应  拒收时返回的状态信息造成界面闪烁问题 将其注释掉
        //memcpy(m_pStatusWorkRes, datagram.data() + rmLen, sizeof(StatusFindResStruct));
        break;
    case 0x4D:
        //记录参数设置命令响应
        memcpy(m_pStatusWorkRes, datagram.data() + rmLen, sizeof(StatusFindResStruct));
        break;
    case 0x4F:
        //文件改名设置命令响应
        //memcpy(m_pStatusWorkRes, datagram.data() + rmLen, sizeof(StatusFindResStruct));
        log = m_name + "文件改名设置命令响应";
        CLog::addLog(log);
        break;
    case 0x51:
        //修改备注设置命令响应
        //memcpy(m_pStatusWorkRes, datagram.data() + rmLen, sizeof(StatusFindResStruct));
        log = m_name + "修改备注设置命令响应";
        CLog::addLog(log);
        break;
    case 0x53:
        //回放参数设置命令响应
        //memcpy(m_pStatusWorkRes, datagram.data() + rmLen, sizeof(StatusFindResStruct));
        log = m_name + "回放参数设置命令响应";
        CLog::addLog(log);
        break;
    default:
        break;
    }
}

void CRM::getDevInfo(char &connect)
{
    connect = m_connect == m_connectOld ? char(0) : char(1);
    m_connectOld = m_connect;
}
