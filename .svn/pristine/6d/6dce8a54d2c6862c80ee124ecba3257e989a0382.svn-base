#include "connectstatus.h"
#include "util.h"
#include "Global.h"
#include "../../Common/Log.h"

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
    {0x00, 0x3B00, 0xEA0B , 0x00}, /*Upnet_switch*/
    {0x00, 0x3C00, 0xEA0D, 0x00}, /*Downnet_switch*/
    {0x00, 0x3D00, 0xEA0F, 0x00}, /*Testnet_switch*/

    {0x00, 0x2C00, 0xEA01, 0x00}, /*S/L分路组合*/
    {0x00, 0x2D00, 0xEA03, 0x00}, /*S切换开关*/

    {0x00, 0x2F00, 0xE489, 0x00}, /*down_alter1*/
    {0x00, 0x2F01, 0xE489, 0x00}, /*down_alter2*/
    {0x00, 0x2F02, 0xE489, 0x00}, /*down_alter3*/
    {0x00, 0x2E00, 0xE489, 0x00}, /*up_alter1*/
    {0x00, 0x2E01, 0xE489, 0x00}, /*up_alter2*/
    {0x00, 0x2E02, 0xE489, 0x00}, /*up_alter2*/
    {0x00, 0x3A00, 0xE495, 0x00}, /*Adjust_alter*/

    {0x00, 0x3E00, 0xEA11, 0x00}, /*频率基准源*/

    {0x00, 0x4A00, 0xEA13, 0x00}, /*信标一体机*/

    {0x00, 0x0500, 0x0135, 0x00}, /*联试应答机标准模式*/
    {0x01, 0x0500, 0x0135, 0x00}, /*联试应答机扩频模式*/

    {0x00, 0x3200, 0xE500, 0x00}, /*Spower*/
    {0x00, 0x3202, 0xE500, 0x00}, /*Lpower*/

    {0x00, 0x0800, 0xE467, 0x00}, /*TestU*/
    {0x00, 0x3F00, 0xEA20, 0x00}, /*RClock*/

    {0x00, 0x0400, 0x0135, 0x00},/*基带一调频模式，BPSK模式，SS模式*/

    {0x00, 0x0401, 0x0135, 0x00},/*基带二调频模式，BPSK模式 SS模式 */

    {0x00, 0x0402, 0x0135, 0x00},/*基带三调频模式，BPSK模式， SS模式*/

     {0x00, 0x01200, 0x1255, 0x00},/*record一*/

     {0x00, 0x01200, 0x1255, 0x01},/*record二 */

     {0x00, 0x01200, 0x1255, 0x02}/*record三*/


};
ConnectStatusCollect::ConnectStatusCollect(QObject *parent):
    QThread(parent),
    m_c_TestU(25)
{
    m_pConnectSt = NULL;
    bRunning_ = true;
    memset(timetag_, 0, sizeof(timetag_));
    memset(m_isTimeChange, 0, sizeof(m_isTimeChange));
    m_pRevThread = NULL;
}
// 1: 增加 0：未增加/未找到信息元
void ConnectStatusCollect::hasTimeChanged()
{
    for(uint i = 0; i < ARR_LEN(devicesConnectInfo); i++)
    {
        uint _tempTag = m_pPlatformRes->GetTimeTag(g_context.m_lsTsspObjInfo.usStationID,
                                   devicesConnectInfo[i].subsysID, devicesConnectInfo[i].deviceID,
                                   devicesConnectInfo[i].typeNO, devicesConnectInfo[i].sn);

        m_isTimeChange[i] = (timetag_[i] != _tempTag);
        timetag_[i] = _tempTag;
    }
}
//typedef struct
//{
//    char ak[2];     // ak[0]: ak_A ak[1]:ak_B, 0 未链接  1:FM 2:ZZM
//    char acu;       // 0 未链接, 1链接
//    char dte;       // 0 未链接, 1链接
//    char timer;
//    char upnet_switch;//dev监控
//    char downnet_switch;//dev监控
//    char testnet_switch;//dev监控
//    char slDivcom;//dev监控
//    char sSwitch;//dev监控
//    char down_alter[3]; //dev监控*
//    char up_alter[3];//dev监控*
//    char adjustup_alter;//dev监控*
//    char freqB;//dev监控
//    char zeroSTD;//dev监控
//    char usbRSP;//dev监控
//    char ss2RSP;//dev监控
//    char spower;//dev监控*
//    char lpower;//dev监控*
//    char testU;//dev监控*
//    char rClock;//dev监控
//    char bbe[3]; //dev监控   // 0 未链接  1:FM 2:BPSK 4:SS
//    char bbeControl[3];//基带是本控(1)还是分控(0)
//    char record[3];//dev监控* // 检前1，2，3：0未链接 1链接//
//    char spectrum;  // 频谱仪
//} DevicesConnectStatus;
void ConnectStatusCollect::PutTheTimeChangeReusltToStatusInfo()
{
    //测试计算机
    m_pConnectSt->testU = m_isTimeChange[m_c_TestU];
    // 基带-1
    m_pRevThread->m_bbe1.getDevInfo(m_pConnectSt->bbe[0], m_pConnectSt->bbeControl[0]);

    // 基带-2
    m_pRevThread->m_bbe2.getDevInfo(m_pConnectSt->bbe[1], m_pConnectSt->bbeControl[1]);

    // 基带-3
//    if(m_isTimeChange[15])
//    {
//        m_pConnectSt->bbe[2] = g_context.m_pBBBGlobalStatus[2].DestMCSSysStatus.dwSystemMode;
//    }
//    else
//        m_pConnectSt->bbe[2] = 0;

    m_pRevThread->m_netSwitchDown.getDevInfo(m_pConnectSt->downnet_switch);
    m_pRevThread->m_netSwitchUp.getDevInfo(m_pConnectSt->upnet_switch);
    m_pRevThread->m_devs.getDevInfo(m_pConnectSt);
//    m_pRevThread->m_alterDown1.getDevInfo(m_pConnectSt->down_alter[0]);
//    m_pRevThread->m_alterDown2.getDevInfo(m_pConnectSt->down_alter[1]);
//    m_pRevThread->m_alterUp1.getDevInfo(m_pConnectSt->up_alter[0]);
//    m_pRevThread->m_alterUp1.getDevInfo(m_pConnectSt->up_alter[0]);
//    m_pRevThread->m_adjustAlter.getDevInfo(m_pConnectSt->adjustup_alter);
    m_pRevThread->m_safeCtrl1.getDevInfo(m_pConnectSt->ak[0]);
    m_pRevThread->m_safeCtrl2.getDevInfo(m_pConnectSt->ak[1]);
    m_pRevThread->m_crm1.getDevInfo(m_pConnectSt->record[0]);
    m_pRevThread->m_crm2.getDevInfo(m_pConnectSt->record[1]);
}

void ConnectStatusCollect::ConnectStatusCollect::run()
{
    // 判断信息元是否更新以确定链接状态
    while(bRunning())
    {
        hasTimeChanged();
        PutTheTimeChangeReusltToStatusInfo();
        sleep(3);
    }
}

bool ConnectStatusCollect::ConnectStatusCollect::bRunning() const
{
    return bRunning_;
}

void ConnectStatusCollect::ConnectStatusCollect::setStop()
{
    bRunning_ = false;
    wait();
}

bool ConnectStatusCollect::init(RecvDataThread* pRevThread, PlatformResource* pRes)
{
    m_pPlatformRes = pRes;
    do{
    ITEM* pItem = m_pPlatformRes->m_devMap.getItem("设备联接状态");
    if (NULL == pItem /*|| pItem->vSubItem.size() <= index*/)return false;
    SUBITEM* sItem = pItem->getSubItemByDeviceID_SN( 0x100, 0);
    if(NULL == sItem)return false;
    int temdwLen = sizeof(DevicesConnectStatus);
    m_pConnectSt = (DevicesConnectStatus*)m_pPlatformRes->FindOneItem(sItem->usStationID, sItem->ucSubSysID, sItem->usDeviceID, sItem->usStateType, sItem->ucSN, temdwLen);\
    if(m_pConnectSt)break;
    m_pConnectSt = (DevicesConnectStatus*)m_pPlatformRes->ApplyOneItem(sItem->usStationID, sItem->ucSubSysID, sItem->usDeviceID, sItem->usStateType, sItem->ucSN, temdwLen);\
    if(!m_pConnectSt)return false;
    memset(m_pConnectSt, 0, temdwLen);
    }while(0);
    //DATA_SOURCE_ALLOC(DevicesConnectStatus, m_pConnectSt, "设备联接状态", 0x100, 0)
    m_pRevThread = pRevThread;
    return m_pRevThread;
}
}// namespace
