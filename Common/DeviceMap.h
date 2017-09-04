#ifndef _DEVICEMAP_H
#define _DEVICEMAP_H

#include <QString>
#include <QList>
#include <QPair>
#include <QVector>
#include <QDomNode>
#include <QMap>
#include <QPair>

#define LOCAL_CTRL   ("本控")
#define REMOTE_CTRL   ("分控")
#define NORMA      ("正常")
#define FAULT       ("故障")
//常量设置
const int c_SS2=9;
const int c_SS1=8;
const int c_TTC=7;
const int c_DDT=5;
const int c_TC =6;



//wl 协议封装、解析、共享内存和参数节点ID所需要的结构体
typedef struct _tag_subItem
{
    unsigned short usCMD;
    unsigned short usTID;
    unsigned short usStateType;
    unsigned short usStationID;
    unsigned char ucSubSysID;
    unsigned short usDeviceID;
    unsigned char ucSN;
    unsigned short usUintNo;
    unsigned int uPDXPSMCU;
    unsigned int uPDXPDEVICES;
    char szChannel[255];        //平台通道名称
}SUBITEM;

typedef struct _tag_Item
{
    QVector<SUBITEM> vSubItem;
    QString strModuleName;
    int iObjType;//0: 参数设置, 1: 过程控制.
    SUBITEM*  getSubItemByDeviceID_SN(const unsigned short& deviceID, const unsigned char& sn);
    SUBITEM*  getSubItemBySubSysID(unsigned char subSysID);
    SUBITEM*  getSubItemBySubSysIDAndDevID(unsigned char subSysID, unsigned char devID);
} ITEM;

class CDeviceMap
{
public:
    CDeviceMap();
    ~CDeviceMap();

	//xml格式必须是正确的，如果不正确，返回false；filePath必须存在，如果不存在返回false。
    bool loadXML();

    ITEM* getItem(const SUBITEM& subItem, bool isParam = true);
    ITEM* getItem(const QString& moduleName);
    bool subItemInItem(const SUBITEM& subItem, const ITEM& item, bool isParam);
    //通过Code和CmdType获取发送的编号
    unsigned char getUintNo(const QString& moduleName, unsigned short usCMD, unsigned short usTID);
    unsigned char getUintNo(ITEM *pItem, unsigned short usCMD, unsigned short usTID);
    unsigned char getUintNo(unsigned short usCMD, unsigned short usTID, unsigned char ucSN);

    //根据单元号和设备地址获取所使用元数据内容
    SUBITEM * getObject(unsigned short, unsigned char, unsigned int);

    //sid 体制 设备
    QMap<int, QPair<QString, QString> > m_mpSysSid;

    bool getSubItemContent(SUBITEM &subItem, ITEM &item, bool isParam = true);
    bool subItemQqualCMD_TID_SN(const SUBITEM &v1, const SUBITEM &v2);
private:
    bool getNode(QDomNodeList& nodeList);
    bool processItem(QDomNodeList& nodeList);
    bool processSubItem(QDomNodeList& nodeList, QVector<SUBITEM>& subItem);
    bool processMapSidSys(QDomNodeList& nodeList);
    inline bool subItemQqualCMD_TID_SN_DEVID_SUBSYSID(const SUBITEM &v1, const SUBITEM &v2);
private:
    QList<ITEM> m_devList;
    QList<SUBITEM> m_subItemList;

};

#endif // _DEVICEMAP_H
