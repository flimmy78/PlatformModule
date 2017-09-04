#include "connectstatus.h"
#include "util.h"
#include "Global.h"
#include "../Common/Log.h"

namespace DevCtl {

DevicesConnectInfoID devicesConnectInfo[] =
{
{0x00, 0x0700, 0x0135, 0x00},/*安控基带一调频模式*/
{0x01, 0x0700, 0x0135, 0x00},/*安控基带一ZZM模式*/
{0x00, 0x0701, 0x0135, 0x00},/*安控基带二调频模式*/
{0x01, 0x0701, 0x0135, 0x00},/*安控基带二ZZM模式*/

{0x00, 0x0200, 0x0135, 0x00}, /*ACU*/
{0x00, 0x0300, 0x0135, 0x00}, /*DTE*/
{0x00, 0x4100, 0x1C07, 0x00}, /*Timer*/
{0x00, 0x1700, 0xE499, 0x00}, /*net_switch*/

{0x00, 0x2C06, 0xE489, 0x00}, /*down_alter1*/
{0x00, 0x2C04, 0xE489, 0x00}, /*down_alter2*/
{0x00, 0x2C02, 0xE489, 0x00}, /*down_alter3*/
{0x00, 0x2C00, 0xE495, 0x00}, /*up_alter*/

{0x00, 0x6C00, 0xE500, 0x00}, /*power*/

{0x00, 0x0400, 0x0135, 0x00},/*基带一调频模式，BPSK模式，SS模式*/

{0x00, 0x0401, 0x0135, 0x00},/*基带二调频模式，BPSK模式 SS模式 */

{0x00, 0x0402, 0x0135, 0x00},/*基带三调频模式，BPSK模式， SS模式*/

{0x00, 0x01200, 0x1255, 0x00},/*record一*/

{0x00, 0x01200, 0x1255, 0x01},/*record二 */

{0x00, 0x01200, 0x1255, 0x02}/*record三*/

};
ConnectStatusCollect::ConnectStatusCollect(QObject *parent):
    QThread(parent)
{
    pConnectSt_ = NULL;
    bRunning_ = true;
    memset(timetag_, 0, sizeof(timetag_));
    memset(isTimeChange_, 0, sizeof(isTimeChange_));

}
// 1: 增加 0：未增加/未找到信息元
void ConnectStatusCollect::hasTimeChanged()
{
    for(uint i = 0; i < ARR_LEN(devicesConnectInfo); i++)
    {
        uint _tempTag = GetTimeTag(g_context.m_lsTsspObjInfo.usStationID,
                                   devicesConnectInfo[i].subsysID, devicesConnectInfo[i].deviceID,
                                   devicesConnectInfo[i].typeNO, devicesConnectInfo[i].sn);

        isTimeChange_[i] = (timetag_[i] != _tempTag);
        timetag_[i] = _tempTag;
        if(13 == i)
            qDebug()<<_tempTag;
    }
}
/*
typedef struct
{
    char ak[2];     // ak[0]: ak_A ak[1]:ak_B, 0 未链接  1:FM 2:ZZM
    char acu;       // 0 未链接, 1链接
    char dte;       // 0 未链接, 1链接
    char timer;
    char net_switch;
    char down_alter[3]; //
    char up_alter;
    char power;
    char bbe[3]; // 0 未链接  1:FM 2:BPSK 3:SS
har record[3]; // 检前1，2，3：0未链接 1链接
    char spectrum;  // 频谱仪
} DevicesConnectStatus;*/
static void PutTheTimeChangeReusltToStatusInfo(DevicesConnectStatus* pInfo,
                                               bool isChanged[])
{
    // 安控基带-1
    if(isChanged[0])
        pInfo->ak[0] = 1;
    else
        pInfo->ak[0] = 0;

    // 安控基带-2
    if(isChanged[2])
        pInfo->ak[1] = 1;
    else
        pInfo->ak[1] = 0;

    pInfo->acu = isChanged[4] ? 1 : 0;
    pInfo->dte = isChanged[5] ? 1 : 0;
    pInfo->timer = isChanged[6] ? 1 : 0;
    pInfo->net_switch = isChanged[7] ? 1 : 0;
    pInfo->down_alter[0] = isChanged[8] ? 1 : 0;
    pInfo->down_alter[1] = isChanged[9] ? 1 : 0;
    pInfo->down_alter[2] = isChanged[10] ? 1 : 0;
    pInfo->up_alter = isChanged[11] ? 1 : 0;
    pInfo->power = isChanged[12] ? 1 : 0;

    // 基带-1
    if(isChanged[13])
    {
        pInfo->bbe[0] = g_context.m_pBBBGlobalStatus[0].DestMCSSysStatus.dwSystemMode;
    }
    else
        pInfo->bbe[0] = 0;

    // 基带-2
    if(isChanged[14])
    {
        pInfo->bbe[1] = g_context.m_pBBBGlobalStatus[1].DestMCSSysStatus.dwSystemMode;
    }
    else
        pInfo->bbe[1] = 0;

    // 基带-3
    if(isChanged[15])
    {
        pInfo->bbe[2] = g_context.m_pBBBGlobalStatus[2].DestMCSSysStatus.dwSystemMode;
    }
    else
        pInfo->bbe[2] = 0;

    pInfo->record[0] = isChanged[16] ? 1 : 0;
    pInfo->record[1] = isChanged[17] ? 1 : 0;
    pInfo->record[2] = isChanged[18] ? 1 : 0;
}

void ConnectStatusCollect::ConnectStatusCollect::run()
{
    if(g_context.m_pDeviceLinkStatus == NULL)
    {
        CLog::addLog("设备状态数据源不创建失败.");
        return;
    }
    // 判断信息元是否更新以确定链接状态
    while(bRunning())
    {
        hasTimeChanged();
        PutTheTimeChangeReusltToStatusInfo(g_context.m_pDeviceLinkStatus, isTimeChange_);
        sleep(5);
    }
}




bool ConnectStatusCollect::ConnectStatusCollect::bRunning() const
{
    return bRunning_;
}

void ConnectStatusCollect::ConnectStatusCollect::setBRunning(bool bRunning)
{
    bRunning_ = bRunning;
}


}// namespace
