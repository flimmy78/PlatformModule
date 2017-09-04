#include "../../Common/LxTsspCommonDefinition.h"
#include "SLDCDcuGUI.h"

QMap<quint64, void*> g_ObjectList;
/// 获取模块的信息，包括模块的公司名称、模块名称、配置标识、版本、开发人员等。
/// __declspec(dllexport)
extern "C" int GetModuleInfo(TLxTsspModuleInfo& moduleInfo)
{
    // 请按照实际模块的名称、标识、版本、开发人员填写。
    /// 公司名称,固定为“中国航天科技集团九院七〇四所”
    strcpy(moduleInfo.szCompany, "中国航天科技集团九院七〇四所\0");

    /// 模块名称，如：系统工作参数设置
    strcpy(moduleInfo.szModuleName, MODULENAME);

    /// 配置标识，如：D_Y17-61_008
    strcpy(moduleInfo.szConfigID, "D_Y17-92_129\0");

    /// 版本
    strcpy(moduleInfo.szVersion, "01.00\0");

    /// 开发者，如：大卫.奥巴马
    strcpy(moduleInfo.szCreator, "王磊\0");

    /// 备注
    strcpy(moduleInfo.szComment, "\0");

    return 1;
}

/// 获取模块配置格式。模块使用时的配置格式，以示例形式给出，用于系统配置。
/// 类似原来文档目录下的模块配置信息.txt中的内容。
///  __declspec(dllexport)
extern "C" int GetConfigFormat(QString& strConfigFormat)
{
    // 在单机上，一般情况下StationID、SubSysID、 DevID不需要配置
    strConfigFormat =
            QString("<Module Name=\"libSLDCDcu.so\" StationID=\"2311\" SubSysID=\"04\" DevID=\"0400\" Note=%1> \n "
            "<Object ID=\"5161\" Num=\"1\" IndexStart=\"0\" Alias =\" \" Note=%1>...</Object> \n "
            "... \n </Module>").arg(MODULENAME);

            return 1;
}

/// 获取模块资源需求信息。
/// 给出模块（类）运行时使用的资源，即外部运行环境，以及模块（类）的输出等信息，
/// 用于系统配置。类型原来文档目录下的资源使用.txt中的内容。
///  __declspec(dllexport)
extern "C" int GetResourceReq(QString& strResourceReq)
{
    // 没有固定格式，内容供配置时参考。
    strResourceReq = "通过设备管理对象设置参数，读取参数；通过宏管理对象读取宏参数，设置宏，调用宏。";

    return 1;
}

/// 获取模块版本变更信息。模块版本的变更过程、内容、变更时间、人员等。
/// 类似原来文档目录下的readme.txt中的内容。
///  __declspec(dllexport)
extern "C" int GetUpdateInfo(QString& strUpdateInfo)
{
    strUpdateInfo =
            "------------------------------------------------------------\n \
            版本号：V1.00       \n \
            变更日期：2016-12-24 \n \
            变更地点：704所      \n \
            更改人： 王磊        \n \
            更改原因：文件建立。  \n \
            变更内容：模块生成。   \n \
            备注：无。          \n  \
            ------------------------------------------------------------\n ";

    return 1;
}

/**
 * @brief 取数据标识号
 * @param usStationID：站号
 * @param ucSubSysID：分系统号
 * @param usDeviceID：分机号
 * @param usType：数据类型号
 * @param ucSn：序号
 * @return 数据标识号
 */
quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
              uchar ucSn)
{
    quint64 ui64ID = 0;

    ui64ID = ((((quint64)usStationID & 0xFFFF) << 48) + (((quint64)ucSubSysID & 0xFF) << 40)
              + (((quint64)usDeviceID & 0xFFFF) << 24) + (((quint64)usType & 0xFFFF) << 8)
              + ((quint64)ucSn & 0xFF));

    return ui64ID;
}

/// 创建应用对象。根据配置信息创建对象。
///  __declspec(dllexport)
extern "C" int CreateObjects(const QDomNode &xmlConfigInfo,
                             QMap<quint64, void*>& ObjList)
{
    ushort usStationID = xmlConfigInfo.attributes().namedItem("StationID").nodeValue().toUShort(0, 16);
    uchar ucSubsysID = xmlConfigInfo.attributes().namedItem("SubSysID").nodeValue().toUShort(0, 16);
    ushort usDeviceID = xmlConfigInfo.attributes().namedItem("DeviceID").nodeValue().toUShort(0, 16);

    QDomNodeList nodeList = xmlConfigInfo.childNodes();

    QString objAlias, objNote;
    ushort usObjID = 0;
    int iObjNum = 0;
    int iStart = 0;
    int iStop = 0;
    quint64 ui64ObjectID;

    for(int i=0; i<nodeList.length(); i++)
    {
        usObjID = nodeList.item(i).attributes().namedItem("ID").nodeValue().toUShort(0, 16);
        iObjNum = nodeList.item(i).attributes().namedItem("Num").nodeValue().toInt();
        iStart = nodeList.item(i).attributes().namedItem("IndexStart").nodeValue().toInt();

        objAlias = nodeList.item(i).attributes().namedItem("Alias").nodeValue();
        objNote = nodeList.item(i).attributes().namedItem("Note").nodeValue();

        iStop = iStart + iObjNum;

        if(usObjID == 0x6004)
        {
            for(int k=iStart; k<iStop; k++)
            {
                ui64ObjectID = GetID(usStationID, ucSubsysID, usDeviceID + k, usObjID, 0);
                TLxTsspObjectInfo ObjectInfo;
                memset(&ObjectInfo, 0, sizeof(TLxTsspObjectInfo));
                ObjectInfo.usStationID = usStationID;
                ObjectInfo.bLocalObj = true;
                ObjectInfo.bMainUse = true;
                ObjectInfo.ucSN = 0;
                ObjectInfo.ucSubSysID = ucSubsysID;
                ObjectInfo.usObjectID = usObjID;
                ObjectInfo.usLocalDID = usDeviceID + k;
                strcpy(ObjectInfo.szConfigID, "D_Y17-92_129");
                strcpy(ObjectInfo.szAlias, objAlias.toUtf8().data());
                strcpy(ObjectInfo.szComment, objNote.toUtf8().data());
                g_ObjectList.insert(ui64ObjectID, new SLDCDcuGUI(ObjectInfo));
            }
        }
    }

    ObjList.unite(g_ObjectList);

    return 1;
}

/// 销毁对象。在进程关闭时调用，此时所有对象已停止并清理了占用的资源。由对象创建者销毁对象。
/// __declspec(dllexport)
extern "C" int DestroyObjects()
{
    foreach (quint64 key, g_ObjectList.keys())
    {
        delete g_ObjectList[key];
        g_ObjectList[key] = NULL;
    }
    return 1;
}

