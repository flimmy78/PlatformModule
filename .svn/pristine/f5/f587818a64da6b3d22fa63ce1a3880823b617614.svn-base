#include "RecvDataThread_tmremote.h"

bool RecvDataThread::setDevs(QByteArray &datagram)
{
    memcpy(&m_allDS, datagram.data(), sizeof(m_allDS));
    m_bbe1.set(m_allDS.bbe1);
    m_bbe2.set(m_allDS.bbe2);
    m_netSwitchUp.set(m_allDS.nsUp);
    m_netSwitchDown.set(m_allDS.nsDown);
    m_alterDown1.set(m_allDS.alterDown1);
    m_alterDown2.set(m_allDS.alterDown2);
    m_alterUp1.set(m_allDS.alterUp1);
    m_alterUp2.set(m_allDS.alterUp2);
    m_adjustAlter.set(m_allDS.adjstAlter);
    m_devs.set(m_allDS.devs);
    if(!m_pConnectSt || !m_pTC)return false;
    memcpy(m_pConnectSt, &m_allDS.m_connectSt, sizeof(m_allDS.m_connectSt));
    //m_pConnectSt->bbe[1] = 1;//fix me;
    memcpy(m_pTC, &m_allDS.m_tc, sizeof(m_allDS.m_tc));
    return true;
}

bool RecvDataThread::setDevs(QByteArray &datagram, int nIndex)
{
    qDebug()<<nIndex<<"size:"<<datagram.size();
    switch(nIndex)
    {
    case 0:
        memcpy(&m_allDS.bbe1, datagram.data(), sizeof(m_allDS.bbe1));
        m_bbe1.set(m_allDS.bbe1);
        break;
    case 1:
        memcpy(&m_allDS.bbe2, datagram.data(), sizeof(m_allDS.bbe2));
        m_bbe2.set(m_allDS.bbe2);
        break;
    case 2:
        memcpy(&m_allDS.nsUp, datagram.data(), sizeof(m_allDS.nsUp));
        m_netSwitchUp.set(m_allDS.nsUp);
        break;
    case 3:
        memcpy(&m_allDS.nsDown, datagram.data(), sizeof(m_allDS.nsDown));
        m_netSwitchDown.set(m_allDS.nsDown);
        break;
    case 4:
        memcpy(&m_allDS.alterDown1, datagram.data(), sizeof(m_allDS.alterDown1));
        m_alterDown1.set(m_allDS.alterDown1);
        break;
    case 5:
        memcpy(&m_allDS.alterDown2, datagram.data(), sizeof(m_allDS.alterDown2));
        m_alterDown2.set(m_allDS.alterDown2);
        break;
    case 6:
        memcpy(&m_allDS.alterUp1, datagram.data(), sizeof(m_allDS.alterUp1));
        m_alterUp1.set(m_allDS.alterUp1);
        break;
    case 7:
        memcpy(&m_allDS.alterUp2, datagram.data(), sizeof(m_allDS.alterUp2));
        m_alterUp2.set(m_allDS.alterUp2);
        break;
    case 8:
        memcpy(&m_allDS.adjstAlter, datagram.data(), sizeof(m_allDS.adjstAlter));
        m_adjustAlter.set(m_allDS.adjstAlter);
        break;
    case 9:
        memcpy(&m_allDS.devs, datagram.data(), sizeof(m_allDS.devs));
        m_devs.set(m_allDS.devs);
        break;
    case 10:
        memcpy(&m_allDS.m_connectSt, datagram.data(), sizeof(m_allDS.m_connectSt));
        if(m_pConnectSt)
            memcpy(m_pConnectSt, &m_allDS.m_connectSt, sizeof(m_allDS.m_connectSt));
        break;
    case 11:
        memcpy(&m_allDS.m_tc, datagram.data(), sizeof(m_allDS.m_tc));
        if(m_pTC)
            memcpy(m_pTC, &m_allDS.m_tc, sizeof(m_allDS.m_tc));
        break;
    case 14:
        memcpy(&m_allDS.rm1, datagram.data(), sizeof(m_allDS.rm1));
        m_crm1.set(m_allDS.rm1);
        break;
    case 15:
        memcpy(&m_allDS.rm2, datagram.data(), sizeof(m_allDS.rm2));
        m_crm2.set(m_allDS.rm2);
        break;
    case 12:
        memcpy(&m_allDS.safeTC1, datagram.data(), sizeof(m_allDS.safeTC1));
        m_safeCtrl1.set(m_allDS.safeTC1);
        break;
    case 13:
        memcpy(&m_allDS.safeTC2, datagram.data(), sizeof(m_allDS.safeTC2));
        m_safeCtrl2.set(m_allDS.safeTC2);
        break;
    default:
        return false;
    }
    return true;
}


