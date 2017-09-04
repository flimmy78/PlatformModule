#include <QString>
#include <QDebug>
#include "CommonFunction.h"

CCommFunc::CCommFunc()
{
    m_DeviceStatusSyncID = 0;

    m_pDevConnStatus = NULL;

    m_pStateMgr = NULL;
    m_initIsOk = init();
}

CCommFunc::~CCommFunc()
{

}

CCommFunc *CCommFunc::getInstance()
{
    static CCommFunc ref;
    return &ref;
}

bool CCommFunc::exchange(const QString& buf, UINT& v)
{
    if(4 != buf.size())return false;
    char tmp[4];
    int pos = 3;
    for(int i = 0; i < 4; ++i)
    {
        tmp[pos--] = buf[i].toLatin1() - 48;
    }
    tmp[0] = tmp[0] |( tmp[1]<<4);
    tmp[1] = tmp[2];
    tmp[2] = tmp[3];
    tmp[1] = tmp[1] |( tmp[2]<<4);
    tmp[2] = tmp[3] = 0;
    memcpy(&v, tmp, 4);
    return true;
}

void CCommFunc::exchange(const UINT& v, QString& buf)
{
    char tmp[5] = {0};
    tmp[3] = (v & 0xF);
    tmp[2] = (v>>4)& 0xF;
    tmp[1] = (v>>8)& 0xF;
    tmp[0] = (v>>12)& 0xF;
    for(int i = 0; i < 4; ++i)
        tmp[i] += 48;
    buf = tmp;
}

void CCommFunc::lineEditPrecision(const QString &arg, QLineEdit *pLineEdit, int prec)
{
    QString s = arg;
    int count = s.count();
    bool flag = false;
    for(int i = 0 ; i < count; ++i)
    {
        if(s[i] == '.')
        {
            if(count - 1 - i > prec)
            {
                flag = true;
            }
        }
    }
    if(flag)
       pLineEdit->setText(s.left(count - 1));
}

DevicesConnectStatus * CCommFunc::getAllDeviceConnStatus()
{
    DevicesConnectStatus * pConn = NULL;
    DWORD stLen = sizeof(DevicesConnectStatus);
    if(m_pStateMgr != NULL)
        pConn = (DevicesConnectStatus*)m_pStateMgr->FindOneItem(m_DeviceStatusSyncID, stLen);

    return pConn;
}

bool CCommFunc::getAKType(int nType)
{
    m_pDevConnStatus = getAllDeviceConnStatus();
    if (m_pDevConnStatus == NULL)
        return false;

    return m_pDevConnStatus->ak[nType] == 0x0 ? false : true;
}

bool CCommFunc::getACUStatus()
{
    m_pDevConnStatus = getAllDeviceConnStatus();
    if (m_pDevConnStatus == NULL)
        return false;

    return m_pDevConnStatus->acu == 0x0 ? false : true;
}

bool CCommFunc::getTimerStatus()
{
    m_pDevConnStatus = getAllDeviceConnStatus();
    if (m_pDevConnStatus == NULL)
        return false;

    return m_pDevConnStatus->timer == 0x0 ? false : true;
}

bool CCommFunc::getUpnet_switchStatus()
{
    m_pDevConnStatus = getAllDeviceConnStatus();
    if (m_pDevConnStatus == NULL)
        return false;

    return m_pDevConnStatus->upnet_switch == 0x0 ? false : true;
}

bool CCommFunc::getDownNet_switchStatus()
{
    m_pDevConnStatus = getAllDeviceConnStatus();
    if (m_pDevConnStatus == NULL)
        return false;

    return m_pDevConnStatus->downnet_switch == 0x0 ? false : true;
}

bool CCommFunc::getTestNet_switchStatus()
{
    m_pDevConnStatus = getAllDeviceConnStatus();
    if (m_pDevConnStatus == NULL)
        return false;

    return m_pDevConnStatus->testnet_switch == 0x0 ? false : true;
}

bool CCommFunc::getSLDivcomStatus()
{
    m_pDevConnStatus = getAllDeviceConnStatus();
    if (m_pDevConnStatus == NULL)
        return false;

    return m_pDevConnStatus->slDivcom == 0x0 ? false : true;
}

bool CCommFunc::getSswitchStatus()
{
    m_pDevConnStatus = getAllDeviceConnStatus();
    if (m_pDevConnStatus == NULL)
        return false;

    return m_pDevConnStatus->sSwitch == 0x0 ? false : true;
}

bool CCommFunc::getDown_alterStatus(int nType)
{
    m_pDevConnStatus = getAllDeviceConnStatus();
    if (m_pDevConnStatus == NULL)
        return false;

    return m_pDevConnStatus->down_alter[nType] == 0x0 ? false : true;
}

bool CCommFunc::getUp_alterStatus(int nType)
{
    m_pDevConnStatus = getAllDeviceConnStatus();
    if (m_pDevConnStatus == NULL)
        return false;

    return m_pDevConnStatus->up_alter[nType] == 0x0 ? false : true;
}

bool CCommFunc::getAdjustAp_alterStatus()
{
    m_pDevConnStatus = getAllDeviceConnStatus();
    if (m_pDevConnStatus == NULL)
        return false;

    return m_pDevConnStatus->adjustup_alter == 0x0 ? false : true;
}

bool CCommFunc::getFreqBStatus()
{
    m_pDevConnStatus = getAllDeviceConnStatus();
    if (m_pDevConnStatus == NULL)
        return false;

    return m_pDevConnStatus->freqB == 0x0 ? false : true;
}

bool CCommFunc::getZeroSTDStatus()
{
    m_pDevConnStatus = getAllDeviceConnStatus();
    if (m_pDevConnStatus == NULL)
        return false;

    return m_pDevConnStatus->zeroSTD == 0x0 ? false : true;
}

bool CCommFunc::getUSBRSPStatus()
{
    m_pDevConnStatus = getAllDeviceConnStatus();
    if (m_pDevConnStatus == NULL)
        return false;

    return m_pDevConnStatus->RSP == 0x1 ? true : false;
}

bool CCommFunc::getSS2RSPStatus()
{
    m_pDevConnStatus = getAllDeviceConnStatus();
    if (m_pDevConnStatus == NULL)
        return false;

    return m_pDevConnStatus->RSP == 0x2 ? true : false;
}

bool CCommFunc::getSPowerStatus()
{
    m_pDevConnStatus = getAllDeviceConnStatus();
    if (m_pDevConnStatus == NULL)
        return false;

    return m_pDevConnStatus->spower == 0x0 ? false : true;
}

bool CCommFunc::getLPowerStatus()
{
    m_pDevConnStatus = getAllDeviceConnStatus();
    if (m_pDevConnStatus == NULL)
        return false;

    return m_pDevConnStatus->lpower == 0x0 ? false : true;
}

bool CCommFunc::getTestUStatus()
{
    m_pDevConnStatus = getAllDeviceConnStatus();
    if (m_pDevConnStatus == NULL)
        return false;

    return m_pDevConnStatus->testU == 0x0 ? false : true;
}

bool CCommFunc::getrClockStatus()
{
    m_pDevConnStatus = getAllDeviceConnStatus();
    if (m_pDevConnStatus == NULL)
        return false;

    return m_pDevConnStatus->rClock == 0x0 ? false : true;
}

bool CCommFunc::getbbeStatus(int nType)
{
    m_pDevConnStatus = getAllDeviceConnStatus();
    if (m_pDevConnStatus == NULL)
        return false;

    return m_pDevConnStatus->bbe[nType] == 0x0 ? false : true;
}

bool CCommFunc::getbbeControlStatus()
{
    m_pDevConnStatus = getAllDeviceConnStatus();
    if (m_pDevConnStatus == NULL)
        return false;

    return m_pDevConnStatus->bbeControl == 0x0 ? false : true;
}

bool CCommFunc::getRecordStatus(int nType)
{
    m_pDevConnStatus = getAllDeviceConnStatus();
    if (m_pDevConnStatus == NULL)
        return false;

    return m_pDevConnStatus->record[nType] == 0x0 ? false : true;
}

bool CCommFunc::getSpectrumStatus()
{
    m_pDevConnStatus = getAllDeviceConnStatus();
    if (m_pDevConnStatus == NULL)
        return false;

    return m_pDevConnStatus->spectrum == 0x0 ? false : true;
}

int CCommFunc::getTargetNum(bool* pOk)
{
    if(pOk)*pOk = false;
    int num = 4;
    if(!m_initIsOk)return num;
    ITEM* pItem = m_DeviceMap.getItem("扩频目标数");
    if(!pItem || !pItem->vSubItem.size())return num;
    SUBITEM sItem = pItem->vSubItem[0];
    LxTsspObjectManager* pObjectManager = LxTsspObjectManager::Instance();
    if(pObjectManager == NULL)return num;
    m_pStateMgr = pObjectManager->GetStateManager();
    quint64 id = m_pStateMgr->GetID(sItem.usStationID,
                       sItem.ucSubSysID, sItem.usDeviceID, sItem.usStateType,
                       sItem.ucSN);
    int sizeOfMacroInfo = sizeof(TLxTsspParamMacroInformation);
    TLxTsspParamMacroInformation* pMacroInfo = (TLxTsspParamMacroInformation*)
    m_pStateMgr->FindOneItem(id, sizeOfMacroInfo);
    if(!pMacroInfo || sizeOfMacroInfo != sizeof(TLxTsspParamMacroInformation))return num;
    if(pMacroInfo->ucTargetSum < 1 || pMacroInfo->ucTargetSum > 4)return num;
    if(pOk)*pOk = true;
    return pMacroInfo->ucTargetSum;
}

bool CCommFunc::initDeviceMap()
{
    bool bRet = m_DeviceMap.loadXML();
    if (!bRet)
    {
        QString strInfo = QString(QObject::tr("加载DeviceMapping.xml文件失败！"));
        CLog::addLog(strInfo);
        return false;
    }
    return true;
}

bool CCommFunc::init()
{
    QString strInfo;
    bool bRet = initDeviceMap();
    if (!bRet)
    {
        //记录日志
        strInfo = QString(QObject::tr("初始化DeviceMapping.xml文件错误!"));
        CLog::addLog(strInfo);
        return false;
    }

    m_DeviceStatusSyncID = getDevicesID();

    return true;
}

qint64 CCommFunc::getDevicesID()
{
    qint64 val = 0;

    //查找对象管理器
    LxTsspObjectManager* pObjectManager = LxTsspObjectManager::Instance();
    if(pObjectManager == NULL)
    {
        return val;
    }

    m_pStateMgr = pObjectManager->GetStateManager();

    ITEM *pItem = m_DeviceMap.getItem(DEVICE_STATUS_NAME);
    if (pItem != NULL && pItem->vSubItem.size() > 0)
    {
        SUBITEM sItem = pItem->vSubItem.at(0);
        /*************************状态**********************************/
        val = pObjectManager->GetID(sItem.usStationID, sItem.ucSubSysID,
                                                     sItem.usDeviceID, sItem.usStateType, sItem.ucSN);
        /*************************状态**********************************/
    }
    return val;
}
