#ifndef CXMLREADER_H
#define CXMLREADER_H

#include "Global.h"
#include <QDomDocument>
#include <QString>
#include <QList>
#include <QMap>

enum DATATYPE
{
    //DataType  0表示字符串 1表示整形 2表示时间串1 3表示时间串2
    //  4表示ip地址 5表示浮点型 6表示字符  7表示返回描述信息
    //  8表示LONGLONG  9表示判结果不等和解析子节点中的参数并按位解析  10表示布尔值
    TYPE_STR = 0,
    TYPE_INT,
    TYPE_DATETIME1, //格式为 年 月 日 时 分 秒 保留 8字节
    TYPE_DATETIME2, //格式为 天 时 分 秒 闰年
    TYPE_IP,
    TYPE_FLOAT,
    TYPE_CHAR,
    TYPE_DESC,
    TYPE_LONGLONG,
    TYPE_BIT,
    TYPE_BOOL,
};

typedef struct _tag_DataInfo
{
    QString strID;      //id
    QString strName;    //name
    unsigned int nLen;  //len
    QString strValue;   //value
    QString strDesc;    //desc
    QString strCodeID;  //codeid
    DATATYPE dataType;  //type

    QList<_tag_DataInfo> subInfoList;

}DATAINFO;

typedef struct _tag_HeadInfo
{
    QList<DATAINFO> infoList;

}HEADINFO;
typedef struct _tag_Struct_Para
{
    QString id;         //参数标识
    QString name;       //参数名称
    QString type;       //最终界面显示的参数类型
    QString note;       //备注
    QString unit;       //单位
    int num;            //数量
    double max;         //最大值
    double min;         //最小值
    double symbol;      //系数
    int bitNum;         //位域数量-1代表当前数据类型的满位域
    int precision;      //保留几位小数

}XML_STRUCT_PARA;
typedef struct _tag_StructInfo
{
    QString id;
    QString name;
    QString note;
    int len;
    QString type;
    QMap<QString,XML_STRUCT_PARA> paraList;
}XML_STRUCTINFO;
typedef struct _tag_Package_Addr
{
    int id;
    QString value;
    QString dst;
    QString vtype;
}XML_PACKAGE_ADDR;
typedef struct _tag_Package_Link
{
    int id;
    QString paraId;
    QMap<QString,XML_PACKAGE_ADDR> addrList;
}XML_PACKAGE_LINK;
typedef struct _tag_Package_Members
{
    QString id;
    QString type;
    QMap<int,XML_PACKAGE_LINK> linkList;
}XML_PACKAGE_MEMBERS;
typedef struct _tag_PackageInfo
{
    QString id;
    QString name;
    QString note;
    QMap<QString,XML_PACKAGE_MEMBERS> MembersList;
}XML_PACKAGEINFO;
#define XML_MAP_STRUCT QMap<QString,XML_STRUCTINFO>
#define XML_MAP_PACKAGE QMap<QString,XML_PACKAGEINFO>
class CXmlReader
{
public:
    CXmlReader();
    ~CXmlReader();

public:
    bool loadXML(const QString &strPath,QDomElement &node); 
    bool getDefaultValueOfSwMatPro(QDomElement &node, PROCOTOL_PACK& swMatPro);
    bool getDefaultValueOfNetSDFrm(QDomElement &node, NETSDFRM& netSDFrm);

public:
    int init();
    bool loadXML(const QString & strPath);

    static CXmlReader * instance(){static CXmlReader res;return &res;}
    XML_MAP_STRUCT getMapStruct(){return m_mapStruct;}
    XML_MAP_PACKAGE getMapPackage(){return m_mapPackage;}
private:
    int readStruct(QDomElement node);
    int readPakconf(QDomElement node);
    void readHeadNode(QDomElement node);

    void readCodeNode(QString strCodeID, QDomElement node, DATAINFO& codeinfo);

    void readParamNode(QDomElement node, DATAINFO& codeinfo);

    void readSubNode(QDomElement node, DATAINFO& codeinfo);
private:
    XML_MAP_STRUCT m_mapStruct;
    XML_MAP_PACKAGE m_mapPackage;
    int readMembers(QDomElement node, XML_PACKAGEINFO &packInfo);
    int readLink(QDomElement node, XML_PACKAGE_MEMBERS &members);
    int readAddr(QDomElement node, XML_PACKAGE_LINK &link);
public:
    HEADINFO m_BussInfo;

    QDomElement m_nodeRoot;
};
inline int xml_Init(){return CXmlReader::instance()->init();}
inline XML_MAP_STRUCT xml_GetMapStruct(){return CXmlReader::instance()->getMapStruct();}
inline XML_MAP_PACKAGE xml_GetMapPackage(){return CXmlReader::instance()->getMapPackage();}
#endif // CXMLREADER_H
