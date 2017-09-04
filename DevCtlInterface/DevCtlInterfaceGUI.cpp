#include "DevCtlInterfaceGUI.h"
#include "DevCtlInterfaceGUIDlg.h"
#include "XmlReader.h"
#include "../Common/Dcu_common_DataStruct.h"
#include <QDir>
#include <QDateTime>
#include "../Common/DeviceMap.h"
#include "../Common/Log.h"
#include "ProcessModule.h"

#define DATA_SOURCE_ALLOC(_type, g_pDataSource, moduleName, index) \
    do{\
    pItem = g_context.m_devMap.getItem(moduleName);\
    if (NULL == pItem || pItem->vSubItem.size() <= index)return false;\
    SUBITEM sItem = pItem->vSubItem.at(index);\
    temdwLen = sizeof(_type);\
    g_pDataSource = (_type*)ApplyOneItem(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID, sItem.usStateType, sItem.ucSN, temdwLen);\
    if(!g_pDataSource)return false;\
    memset(g_pDataSource, 0, temdwLen);\
    }while(0);

namespace
{
    TTCDevCtlInterfaceGUIDlg* g_form = NULL;
}
extern quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
              uchar ucSn);

TTCDevCtlInterfaceGUI::TTCDevCtlInterfaceGUI(TLxTsspObjectInfo ObjectInfo)
{
    //初始化
    memcpy(&m_ObjectInfo, &ObjectInfo, sizeof(TLxTsspObjectInfo));
    memcpy(&g_context.m_lsTsspObjInfo,  &ObjectInfo, sizeof(TLxTsspObjectInfo));
    //错误代码初始化为无错误
    m_iErrCode = 0;

    //对于本对象的错误列表进行初始化
    m_ErrInfoList.insert(0, "无错误");
    //根据对象实际情况添加。
    //m_ErrInfoList.insert(1, "...");
}

/* ----------------------- ILxTsspShow ----------------------*/

/**
 * 显示界面 ：显示界面，当外部对象调用此函数时，本对象应弹出对应的界面。
 * @param parentForm：父窗口。
 * @return 界面对象的界面。
*/
QWidget* TTCDevCtlInterfaceGUI::ShowMe(QWidget* parent, const QString& strTitle)
{
    m_strTitle = strTitle;

    TTCDevCtlInterfaceGUIDlg* form;

    form = new TTCDevCtlInterfaceGUIDlg(parent);

    //设置对象配置信息
    form->SetObjInfo(m_ObjectInfo);
    //设置界面信息
    form->setText(strTitle,m_ParaSetStruct,m_MapMean,MapLineNum,0,NULL);//根据需要
    //关闭时删除窗口
    form->setAttribute(Qt::WA_DeleteOnClose);

    return form;
}

/**
 * 用户界面帮助ID接口 ：用户界面帮助ID接口，帮助功能根据此ID查找帮助文件中
 *          对应的页面并显示；此帮助ID由用户界面管理程序根据菜单名称等信息设置
 * @param[out] szHelpID：用户界面帮助ID。
 * @return szHelpID:长度。
*/
int TTCDevCtlInterfaceGUI::GetContextHelpID(char* szHelpID)
{
    strcpy(szHelpID, m_strTitle.toUtf8().data());

    return m_strTitle.toUtf8().length();
}

/*--------------对象信息接口部分-------------------------------*/
/**
 * 获取对象所属的模块信息：信息包括模块的公司名称、模块名称、配置标识、版本、开发人员等
 * ，用于实时运行时查看。
 * @param[out] ModuleInfo：模块信息。
 * @return 1：成功；0：未定义；-1：失败。
*/
int TTCDevCtlInterfaceGUI::GetModuleInfo(TLxTsspModuleInfo& moduleInfo) const
{
    memset(&moduleInfo, 0, sizeof(TLxTsspModuleInfo));

    // 请按照实际模块的名称、标识、版本、开发人员填写。
    /// 公司名称,固定为“中国航天科技集团九院七〇四所”
    strcpy(moduleInfo.szCompany, "中国航天科技集团九院七〇四所\0");

    /// 模块名称，如：系统工作参数设置
    strcpy(moduleInfo.szModuleName, "测控分机设备接口\0");

    /// 配置标识，如：D_Y17-61_008
    strcpy(moduleInfo.szConfigID, "D_Y17-92_213\0");

    /// 版本
    strcpy(moduleInfo.szVersion, "01.00\0");

    /// 开发者，如：大卫.奥巴马
    strcpy(moduleInfo.szCreator, "王磊\0");

    /// 备注
    strcpy(moduleInfo.szComment, "\0");

    return 1;
}

/**
 * 获取对象相关信息。对象相关信息包括对象名称，对象的分系统号、设备号、流号，
 * 本地对象还是远程对象，主用还是备用等。
 * @param[out] ObjInfo：对象信息。
 * @return 1：成功；0：未定义；-1：失败。
 */
int TTCDevCtlInterfaceGUI::GetObjectInfo(TLxTsspObjectInfo& ObjectInfo) const
{
    memcpy(&ObjectInfo, &m_ObjectInfo, sizeof(TLxTsspObjectInfo));

    return 1;
}

/*--------------对象控制接口部分-------------------------------*/
/**
 * 预初始化：模块在此函数中执行必要的预初始化工作，如查找需要的对象，
 *  向对象管理器添加私有对象等。主用工作是准备对象运行的外界资源条件。
 * @return 1：成功；0：未定义；-1：失败。
 */
int TTCDevCtlInterfaceGUI::PreInitialize()
{
    return 1;
}

/**
 * 初始化 ：模块在此函数中执行必要的初始化工作。
 * @return 1：成功；0：未定义；-1：失败。
 */
int TTCDevCtlInterfaceGUI::Initialize()
{
    //查找对象管理器
    LxTsspObjectManager* pObjectManager = LxTsspObjectManager::Instance();
    if (pObjectManager == NULL)
        return -1;

    //wl 修改DeviceMapping.xml读取文件信息
    QString strLog;
    bool isLoadSucess = g_context.m_devMap.loadXML();
    if(!isLoadSucess)
    {
        QString strLog = QString("初始化DeviceMapping.xml文件失败!");
        CLog::addLog(strLog, 1);
        return -1;
    }
    if(!initDataSource())
    {
        qDebug()<<DATA_SOURCE_ERR;
        CLog::addLog(DATA_SOURCE_ERR);
        return -1;
     }

    //日志信息
    strLog = "测控分机设备接口: 读取配置文件初始化调制参数成功！";
    CLog::addLog(strLog, 1);
    MsgRevRegister("TTCDevCtlInterface");
    g_context.initIDs();
    return 1;
}

//遍历xlm文件，获取参数取值范围
int TTCDevCtlInterfaceGUI::QueryParaBlock(QString ConfigPath)
{
    QDomDocument tmp_DomDoc;
    QFile file(ConfigPath);

    if (!file.open(QIODevice::ReadOnly))
    {
        return -1;
    }

    if (!tmp_DomDoc.setContent(&file))
    {
        file.close();
        return -1;
    }

    file.close();

    QDomNodeList nodeList = tmp_DomDoc.childNodes();

    QDomNode configNode;

    /// 找到configuration
    for(int i=0; i<nodeList.length(); i++)
    {
        if(nodeList.item(i).nodeName().compare("数据结构定义") == 0)
        {
            //取出节点
            configNode = nodeList.item(i);
            break;
        }
    }

    nodeList = configNode.childNodes();

    QDomNode commNode;
    //&由“&amp;”代替
    int iCountParaBlock = 0;         //参数块数
    for(int j=0; j<nodeList.length(); j++)
    {
        if(nodeList.item(j).nodeName().compare("参数块") == 0)
        {
            //取出节点
            commNode = nodeList.item(j);
            QDomNodeList nodeListchild = commNode.childNodes();
            for(int i=0; i<nodeListchild.length(); i++)
            {
                strcpy(m_ParaSetStruct[iCountParaBlock][i].cParaName,nodeListchild.item(i).attributes().namedItem("名称").nodeValue().toUtf8().data());
                strcpy(m_ParaSetStruct[iCountParaBlock][i].cParaID,nodeListchild.item(i).attributes().namedItem("ID").nodeValue().toUtf8().data());
                m_ParaSetStruct[iCountParaBlock][i].lParaNum = nodeListchild.item(i).attributes().namedItem("个数").nodeValue().toInt();
                strcpy(m_ParaSetStruct[iCountParaBlock][i].cParaType,nodeListchild.item(i).attributes().namedItem("类型").nodeValue().toUtf8().data());

                if(strcmp(m_ParaSetStruct[iCountParaBlock][i].cParaType,"float") == 0)
                {
                    m_ParaSetStruct[iCountParaBlock][i].MaxVal.fmax = nodeListchild.item(i).attributes().namedItem("最大值").nodeValue().toFloat();
                    m_ParaSetStruct[iCountParaBlock][i].MinVal.fmin = nodeListchild.item(i).attributes().namedItem("最小值").nodeValue().toFloat();
                    m_ParaSetStruct[iCountParaBlock][i].InitVal.lval = nodeListchild.item(i).childNodes().item(0).toElement().text().toFloat();
                }
                else if(strcmp(m_ParaSetStruct[iCountParaBlock][i].cParaType,"double") == 0)
                {
                    m_ParaSetStruct[iCountParaBlock][i].MaxVal.dmax = nodeListchild.item(i).attributes().namedItem("最大值").nodeValue().toDouble();
                    m_ParaSetStruct[iCountParaBlock][i].MinVal.dmin = nodeListchild.item(i).attributes().namedItem("最小值").nodeValue().toDouble();
                    m_ParaSetStruct[iCountParaBlock][i].InitVal.lval = nodeListchild.item(i).childNodes().item(0).toElement().text().toDouble();
                }
                else
                {
                    if(strcmp(m_ParaSetStruct[iCountParaBlock][i].cParaType,"hex") == 0)
                    {
                        m_ParaSetStruct[iCountParaBlock][i].MaxVal.imax = nodeListchild.item(i).attributes().namedItem("最大值").nodeValue().toLongLong(0,16);
                        m_ParaSetStruct[iCountParaBlock][i].MinVal.imin = nodeListchild.item(i).attributes().namedItem("最小值").nodeValue().toLongLong(0,16);
                        m_ParaSetStruct[iCountParaBlock][i].InitVal.lval = nodeListchild.item(i).childNodes().item(0).toElement().text().toLongLong(0,16);
                    }
                    else if(strcmp(m_ParaSetStruct[iCountParaBlock][i].cParaType,"map") == 0)
                    {
                        m_ParaSetStruct[iCountParaBlock][i].InitVal.lval = nodeListchild.item(i).childNodes().item(0).toElement().text().toLong();
                        GetAllMapValueStringByPara(m_MapSumFlag,nodeListchild.item(i).childNodes().item(1));
                        m_MapSumFlag++;
                    }
                    else
                    {
                        m_ParaSetStruct[iCountParaBlock][i].MaxVal.lmax = nodeListchild.item(i).attributes().namedItem("最大值").nodeValue().toLong();
                        m_ParaSetStruct[iCountParaBlock][i].MinVal.lmin = nodeListchild.item(i).attributes().namedItem("最小值").nodeValue().toLong();
                        m_ParaSetStruct[iCountParaBlock][i].InitVal.lval = nodeListchild.item(i).childNodes().item(0).toElement().text().toLong();
                    }
                }
            }
            iCountParaBlock++;
        }
    }
    return 1;
}

int TTCDevCtlInterfaceGUI::GetAllMapValueStringByPara(int MapSum,QDomNode pInDocNode_Para)
{
    QDomNodeList NodeList_MapItem = pInDocNode_Para.childNodes();
    long MapNum = NodeList_MapItem.length();
    MapLineNum[MapSum] = MapNum;

    for(int ii=0; ii<MapNum; ii++)
    {
        m_MapMean[MapSum][ii] = NodeList_MapItem.item(ii).attributes().namedItem("含义").nodeValue().toUtf8().data();
    }
    return 1;
}

/**
 * 启动：模块在此函数中启动功能的执行，尤其是包含线程的模块，应该在此启动线程；
 * @return 1：成功；0：未定义；-1：失败。
 */
int TTCDevCtlInterfaceGUI::Start()
{
    return 1;
}

/**
 * 停止 ：模块在此函数中停止功能的执行，尤其是包含线程的模块，应该在此结束线程。
 * @return 1：成功；0：未定义；-1：失败。
 */
int TTCDevCtlInterfaceGUI::Stop()
{
    return 1;
}

/**
 * 清理：模块在此函数中清理申请的资源，为进程退出做准备。
 * @return 1：成功；0：未定义；-1：失败。
 */
bool TTCDevCtlInterfaceGUI::ClearUp()
{
    MsgRevUnregister("TTCDevCtlInterface");
    return 1;
}

/*--------------对象故障诊断接口部分----------------------------*/
/**
 * 取错误码： 取对象的错误码。
 * @return： 返回模块的错误码，用于判断模块不能执行的原因:
 *           0表示运行正常；-1表示没有配置信息；-2表示初始化失败；
 *           -3表示启动操作失败；-4表示停止操作失败；-5表示清理资源失败；
 *           其它错误依情况增加；
 */
int TTCDevCtlInterfaceGUI::GetErrorNumber()
{
    return m_iErrCode;
}

/**
 * 取错误信息
 * @param iErrorNum：错误码。
 * @param[out] szErrorMsg：错误信息，描述性的错误信息，用于判断对象不能执行的原因。
 * @return 1：成功；0：未定义；-1：表示没有对应的错误信息；
 */
int TTCDevCtlInterfaceGUI::GetErrorMsg(int iErrorNo, QString& strErrorMsg)
{
    if(m_ErrInfoList.contains(iErrorNo))
    {
        strErrorMsg = m_ErrInfoList[iErrorNo];
        return 1;
    }
    else
    {
        strErrorMsg = "无对应错误信息";
        return -1;
    }
}

/**
 * 是否在线 ：对于远程对象可能存在不在线情况，普通模块直接返回true即可。
 *          对于代理模块需要判断远程实际对象响应情况、通道通断情况来判断。
 * @return true：在线；false：不在线。
 */
bool TTCDevCtlInterfaceGUI::IsOnline()
{
    //对象初始化
    //查找对象管理器
    LxTsspObjectManager* pObjectManager = LxTsspObjectManager::Instance();
    ILxTsspStateManager*  pStateMgr= pObjectManager->GetStateManager();

    DWORD stLen = 0;

    //分机链接状态信息元ID
    quint64 tempDevicesConnectID = GetID(m_ObjectInfo.usStationID,
                                         0, 0x100,
                                         0x9013, m_ObjectInfo.ucSN);
    //基带全局信息元
    quint64 tempBBEGlobalID= GetID(m_ObjectInfo.usStationID,
                               0, m_ObjectInfo.usLocalDID,
                               0x0135, m_ObjectInfo.ucSN);

    //分机链监状态信息元
    DevicesConnectStatus* pDevicesConnectStatus = NULL;//分机链接状态信息元
    stLen = sizeof(pDevicesConnectStatus);
    pDevicesConnectStatus = (DevicesConnectStatus*)(pStateMgr->FindOneItem(tempDevicesConnectID,stLen));
    if(pDevicesConnectStatus == NULL || stLen != sizeof(DevicesConnectStatus))
    {
        return false;
    }

    ushort usTemp = m_ObjectInfo.usLocalDID - 0x400;
    if(pDevicesConnectStatus->bbe[usTemp] == 0)//未链接
    {
        return false;
    }
    else
    {
        BBE_GlobalStatusStruct* pBBEGlobalStatus = NULL;//基带全局信息元
        if(pBBEGlobalStatus == NULL)
        {
            stLen = sizeof(BBE_GlobalStatusStruct);
            pBBEGlobalStatus =
                    (BBE_GlobalStatusStruct*)pStateMgr->FindOneItem(tempBBEGlobalID, stLen);
            if(pBBEGlobalStatus == NULL || stLen != sizeof(BBE_GlobalStatusStruct))
                return false;
        }

        // 1-fm 2-bpsk 4-ss
        if(pBBEGlobalStatus->DestMCSSysStatus.dwSystemMode != 2)//该模式 不是BPSK模式
        {
            return false;
        }
    }
    return true;
}

/**
 * 发送消息 ：通过此函数可实现信息的直接发送，对于对象管理器，当此函数被调用时，
 *          向管理的所有对象发送此消息。
 * @param msg：消息。对象收到消息后，可以拷贝消息内容，但不应更改。
 */
void TTCDevCtlInterfaceGUI::SendMessage(TLxTsspMessage )
{
    return;
}

/**
 * 设置组参数 ：通过此函数把参数设置给对象，设备对象加载到具体设备；
 * @param parameterList：参数块列表,
 * quint64参数块标识，格式为：
 * 分系统号(1)/单机号(2)/参数块标识(2)/序号(1)/参数号(2)。共8个字节，
 * 当标识单元参数时，最后两个字节值填0。这里单机号是指单机类型号，即单机号的低字节为0。
 * 参数块标识(2)和序号(1)为必填项，其它根据需要填写。
 * @return 1：成功；-1：失败。
 */
int TTCDevCtlInterfaceGUI::SetParameter(QMap<quint64, QByteArray> )
{
    //对于参数组中的参数，首先判断是否本对象参数
//    foreach(quint64 key, parameterList.keys())
//    {
//        //参数加载到硬件、本地保存、本地使用。
//    }
    return 1;
}

/**
 * 设置参数 ：设置单个参数。
 * @param ui64ParamID：参数ID,格式为：
 * 分系统号(1)/单机号(2)/参数块标识(2)/序号(1)/参数号(2)。共8个字节，
 * @param parameter：参数块数据
 * @return 1：成功；0：非本对象参数；-1：失败。
 */
int TTCDevCtlInterfaceGUI::SetParameter(quint64 ui64ParamID, QByteArray parameter)
{
    if(m_ParamList.contains(ui64ParamID))
    {
        m_ParamList[ui64ParamID] = parameter;
        //参数加载到硬件、本地保存、本地使用。
        return 1;
    }
    return 0;
}

/**
 * 取对象的参数ID ：取本对象需要的参数块的ID。在参数宏加载时，
 *      调用此函数确定对象需要的参数块，然后从参数宏中取出相应的参数，设置给该对象。
 * @param[out] pParameteIDList：参数块ID的列表。
 * @return 大于等于0：参数块个数。-1：失败。
 */
int TTCDevCtlInterfaceGUI::GetParameterID(QList<quint64>& pParameterIDList) const
{
    pParameterIDList += m_ParamList.keys();

    return m_ParamList.count();
}

/**
 * 取参数 ：获取本对象的参数，可以一次读取所有参数。
 * @param[out] parameterList：参数块ID和参数值的键值对的列表。
 * @return 大于等于0：参数块个数。-1：失败。
 */
int TTCDevCtlInterfaceGUI::GetParameter(QMap<quint64, QByteArray>& ) const
{
//    DDTBBECodeSynParaSet tmpCodeSynPara;       //码同步参数
//    DDTBBEFrameSynParaSet tmpFrameSynPara;     //帧同步器参数
//    DDTBBESubFrameSynParaSet tmpSubFramePara;//副帧同步器参数

//    DDTBBECodeSynStatus* tmpCodeSynStatus = NULL;      //码同步参数
//    DDTBBEFrameSynStatus* tmpFrameSynStatus = NULL;     //帧同步器参数
//    DDTBBESubFrameSynStatus* tmpSubFrameSynStatus = NULL;//副帧同步器参数

//    ILxTsspStateManager* tmpStateMgr = NULL;
//    //查找对象管理器
//    LxTsspObjectManager* pObjectManager = LxTsspObjectManager::Instance();
//    tmpStateMgr = pObjectManager->GetStateManager();

//    DWORD stLen = sizeof(DDTBBECodeSynStatus);
//    quint64 tmpStatusID = GetID(m_ObjectInfo.usStationID,
//                                     m_ObjectInfo.ucSubSysID, m_ObjectInfo.usLocalDID, 0xE613,
//                                      m_ObjectInfo.ucSN);
//    quint64 tmpParaID = GetID(m_ObjectInfo.usStationID,
//                                     m_ObjectInfo.ucSubSysID, m_ObjectInfo.usLocalDID, 0xE614,
//                                      m_ObjectInfo.ucSN);
//    if(tmpStateMgr != NULL)
//        tmpCodeSynStatus  = (DDTBBECodeSynStatus*)tmpStateMgr->FindOneItem(tmpStatusID, stLen);

//    if(tmpCodeSynStatus != NULL)
//    {
//        tmpCodeSynPara = tmpCodeSynStatus->CodeSynPara;
//    }

//    QByteArray  tmpCh;
//    tmpCh.resize(sizeof(DDTBBECodeSynParaSet));
//    memcpy(tmpCh.data(), &tmpCodeSynPara, sizeof(DDTBBECodeSynParaSet));
//    parameterList.insert(tmpParaID, tmpCh);

//    parameterList.unite(m_ParamList);

//    stLen = sizeof(DDTBBEFrameSynStatus);
//    tmpStatusID = GetID(m_ObjectInfo.usStationID,
//                                     m_ObjectInfo.ucSubSysID, m_ObjectInfo.usLocalDID, 0xE615,
//                                      m_ObjectInfo.ucSN);
//    tmpParaID = GetID(m_ObjectInfo.usStationID,
//                                     m_ObjectInfo.ucSubSysID, m_ObjectInfo.usLocalDID, 0xE616,
//                                      m_ObjectInfo.ucSN);
//    if(tmpStateMgr != NULL)
//        tmpFrameSynStatus  =
//                (DDTBBEFrameSynStatus*)tmpStateMgr->FindOneItem(tmpStatusID, stLen);

//    if(tmpFrameSynStatus != NULL)
//    {
//        tmpFrameSynPara = tmpFrameSynStatus->FrameSynPara;
//    }

//    tmpCh.resize(sizeof(DDTBBEFrameSynParaSet));
//    memcpy(tmpCh.data(), &tmpFrameSynPara, sizeof(DDTBBEFrameSynParaSet));
//    parameterList.insert(tmpParaID, tmpCh);

//    parameterList.unite(m_ParamList);

//    stLen = sizeof(DDTBBESubFrameSynStatus);
//    tmpStatusID = GetID(m_ObjectInfo.usStationID,
//                                     m_ObjectInfo.ucSubSysID, m_ObjectInfo.usLocalDID, 0xE617,
//                                      m_ObjectInfo.ucSN);
//    tmpParaID = GetID(m_ObjectInfo.usStationID,
//                                     m_ObjectInfo.ucSubSysID, m_ObjectInfo.usLocalDID, 0xE618,
//                                      m_ObjectInfo.ucSN);
//    if(tmpStateMgr != NULL)
//        tmpSubFrameSynStatus  =
//                (DDTBBESubFrameSynStatus*)tmpStateMgr->FindOneItem(tmpStatusID, stLen);

//    if(tmpSubFrameSynStatus != NULL)
//    {
//        tmpSubFramePara = tmpSubFrameSynStatus->SubFrameSynPara;
//    }

//    tmpCh.resize(sizeof(DDTBBESubFrameSynParaSet));
//    memcpy(tmpCh.data(), &tmpSubFramePara, sizeof(DDTBBESubFrameSynParaSet));
//    parameterList.insert(tmpParaID, tmpCh);

//    parameterList.unite(m_ParamList);

//    return m_ParamList.count();
    return -1;
}

/**
 * 取参数 ：获取本对象的参数，按照指定的ID读取。
 * @param ui64ParamID：参数块ID。
 * @param[out] parameter：参数块的值；
 * @return 1：成功；-1：失败。
 */
int TTCDevCtlInterfaceGUI::GetParameter(quint64 ui64ParamID, QByteArray& parameter) const
{
    if(m_ParamList.contains(ui64ParamID))
    {
        parameter = m_ParamList[ui64ParamID];
        return 1;
    }
    return -1;
}

/**
 * 获取状态 ：获取本对象的状态，可以一次读取所有状态。
 * @param[out] stateList：状态块ID和状态值的键值对的列表。quint64为状态号，格式为：
 * 分系统号(1)/单机号(2)/状态块标识(2)/序号(1)/状态号(2)。共8个字节，
 * 当标识单元状态时，最后两个字节值填0。这里单机号是指实际单机号，即单机号的低字节不为0。
 * 状态块标识(2)/序号(1)为必填项，其它根据需要填写。
 * @return 大于等于0：状态块个数。-1：失败。
 */
int TTCDevCtlInterfaceGUI::GetState(QMap<quint64, TLxTsspDeviceState>& stateList) const
{
    stateList.unite(m_StateList);

    return m_StateList.count();
}

/**
 * 取单个状态 ：根据状态ID取相应的状态，注意如果参数不加载到硬件，也要作为状态返回。
 * @param ui64StatusID:状态块ID。
 * @param[out] status：状态值。
 * @return 1：成功；-1：失败。
 */
int TTCDevCtlInterfaceGUI::GetState(quint64 ui64StatusID, TLxTsspDeviceState& status) const
{
    if(m_StateList.contains(ui64StatusID))
    {
        status = m_StateList[ui64StatusID];
        return 1;
    }
    return -1;
}

/**
 * 直接取状态 ：直接从硬件读取状态，不取当前缓存的状态，强调实时性时使用。
 * @param ui64StatusID:状态块ID。
 * @param[out] status：状态值。
 * @return 1：成功；-1：失败。
 */
int TTCDevCtlInterfaceGUI::GetStateDirect(quint64 ui64StatusID, TLxTsspDeviceState& )
{
    if(m_StateList.contains(ui64StatusID))
    {
        //
    }
    return 1;
}

/**
 * 向对象写数据 ：向对象写数据
 * @param ui64DataID：数据ID；格式如下：
 * 分系统号(1)/单机号(2)/数据块标识(2)/序号(1)/数据号(2)。共8个字节，
 * 当表示单元数据时，最后两个字节值填0。
 * 数据块标识(2)/序号(1)为必填项，其它根据需要填写。
 * @param uiDataSn：数据流号；
 * @param aData：数据缓冲区；
 * @param uiDataLen：数据字节长度。
 * @return 大于等于0：实际写入的数据长度；-1：失败。
 */
int TTCDevCtlInterfaceGUI::WriteData(quint64 , uchar* , uint )
{
    return 0;
}

/**
 * 从模块读取数据 ：从模块读取数据
 * @param uiDataID：数据ID；
 * @param uiDataSn：数据流号；
 * @param[out] aData：数据缓冲区；
 * @param[out] uiDataLen：数据缓冲区字节长度。
 * @param[out] uiReloadCount：加载计数，计数变化意味着状态的改变。
 * @return 大于等于0：实际读取的数据长度；-1：失败。
 */
int TTCDevCtlInterfaceGUI::ReadData(quint64 , uchar* , uint& ,
                             uint& )
{
    return 0;
}

int TTCDevCtlInterfaceGUI::CreateDefaultMacroObj(ILxTsspParameterMacro* pMacroObj)
{
    /*码同步参数*/
    DDTBBECodeSynParaSet aDDTBBECodeSynPara;
    quint64 ParaDDTBBECodeSynID = GetID(m_ObjectInfo.usStationID,
                                     m_ObjectInfo.ucSubSysID, m_ObjectInfo.usLocalDID, 0xE614,
                                      m_ObjectInfo.ucSN);
    //工作控制
    aDDTBBECodeSynPara.usWorkCtrl = m_ParaSetStruct[0][0].InitVal.lval;
    //码型
    aDDTBBECodeSynPara.usCodeType = m_ParaSetStruct[0][6].InitVal.lval;
    //码环带宽
    aDDTBBECodeSynPara.usCodeBn = m_ParaSetStruct[0][1].InitVal.lval;
    //码率
    aDDTBBECodeSynPara.ulCodeRate = m_ParaSetStruct[0][2].InitVal.lval;
    //位流传输使能
    aDDTBBECodeSynPara.ucBitOutEn = m_ParaSetStruct[0][3].InitVal.lval;
    //位流输出时钟选择
    aDDTBBECodeSynPara.ucClkSel = m_ParaSetStruct[0][4].InitVal.lval;
    //传输FIFO的初始缓冲字节数
    aDDTBBECodeSynPara.usFIFOLen = m_ParaSetStruct[0][5].InitVal.lval;
    //Viterbi译码
    aDDTBBECodeSynPara.ucViterbi = m_ParaSetStruct[0][7].InitVal.lval;
    //判决方式
    aDDTBBECodeSynPara.usHRSel = m_ParaSetStruct[0][8].InitVal.lval;
    //取当前设置的参数，存入框架
    QByteArray byteArray;
    byteArray.resize(sizeof(aDDTBBECodeSynPara));
    memcpy(byteArray.data(), &aDDTBBECodeSynPara, sizeof(aDDTBBECodeSynPara));
    if(pMacroObj != NULL)
    {
        pMacroObj->UpdateParameterBlock(ParaDDTBBECodeSynID, byteArray);
    }
    /*码同步参数*/

    /*帧同步器参数*/
    DDTBBEFrameSynParaSet aDDTBBEFrameSynPara;
    quint64 ParaDDTBBEFrameSynID = GetID(m_ObjectInfo.usStationID,
                                     m_ObjectInfo.ucSubSysID, m_ObjectInfo.usLocalDID, 0xE616,
                                                 m_ObjectInfo.ucSN);
    //工作控制
    aDDTBBEFrameSynPara.usWorkCtrl = m_ParaSetStruct[1][0].InitVal.lval;
    //逆序帧同步使能
    aDDTBBEFrameSynPara.ucRevEn = m_ParaSetStruct[1][1].InitVal.lval;
    //帧长
    aDDTBBEFrameSynPara.usFrameLen = m_ParaSetStruct[1][3].InitVal.lval;
    //字长
    aDDTBBEFrameSynPara.ucWordLen = m_ParaSetStruct[1][4].InitVal.lval;
    //帧同步码组长
    aDDTBBEFrameSynPara.ucSynCodeLen = m_ParaSetStruct[1][5].InitVal.lval;
    //帧同步码组
    aDDTBBEFrameSynPara.dwlFrmSyncCode = m_ParaSetStruct[1][6].InitVal.lval;
    //校核帧数
    aDDTBBEFrameSynPara.ucVerifyCheckFrame = m_ParaSetStruct[1][7].InitVal.lval;
    //锁定帧数
    aDDTBBEFrameSynPara.ucLockCheckFrame = m_ParaSetStruct[1][8].InitVal.lval;
    //搜索容错位数
    aDDTBBEFrameSynPara.ucSearchErrBits = m_ParaSetStruct[1][9].InitVal.lval;
    //校核容错位数
    aDDTBBEFrameSynPara.ucVerifyErrBits = m_ParaSetStruct[1][10].InitVal.lval;
    //锁定容错位数
    aDDTBBEFrameSynPara.ucLockErrBits = m_ParaSetStruct[1][11].InitVal.lval;
    //解密使能
    aDDTBBEFrameSynPara.ucSecretEn = m_ParaSetStruct[1][16].InitVal.lval;
    //解密解扰顺序选择
    aDDTBBEFrameSynPara.OrderSel = m_ParaSetStruct[1][17].InitVal.lval;
    //交织深度
    aDDTBBEFrameSynPara.ucInterleaver = m_ParaSetStruct[1][19].InitVal.lval;
    //取当前设置的参数，存入框架
    byteArray.resize(sizeof(aDDTBBEFrameSynPara));
    memcpy(byteArray.data(), &aDDTBBEFrameSynPara, sizeof(aDDTBBEFrameSynPara));
    if(pMacroObj != NULL)
    {
        pMacroObj->UpdateParameterBlock(ParaDDTBBEFrameSynID, byteArray);
    }
    /*帧同步器参数*/

    /*副帧同步器参数*/
    DDTBBESubFrameSynParaSet aDDTBBESubFrameSynPara;
    quint64 ParaDDTBBESubFrameSynID = GetID(m_ObjectInfo.usStationID,
                                     m_ObjectInfo.ucSubSysID, m_ObjectInfo.usLocalDID, 0xE618,
                                                 m_ObjectInfo.ucSN);
    //工作控制
    aDDTBBESubFrameSynPara.usWorkCtrl = m_ParaSetStruct[2][0].InitVal.lval;
    //副帧方式
    aDDTBBESubFrameSynPara.ucSubFrmType = m_ParaSetStruct[2][1].InitVal.lval;
    //ID副帧ID字位置
    aDDTBBESubFrameSynPara.usIDSubFrmPos = m_ParaSetStruct[2][3].InitVal.lval;
    //循环副帧长度
    aDDTBBESubFrameSynPara.usSubFrmLen = m_ParaSetStruct[2][2].InitVal.lval;
    //ID字基值
    aDDTBBESubFrameSynPara.ucIdBase = m_ParaSetStruct[2][4].InitVal.lval;
    //ID计数方向
    aDDTBBESubFrameSynPara.ucIdDirect = m_ParaSetStruct[2][5].InitVal.lval;
    //循环副帧同步码组
    aDDTBBESubFrameSynPara.dwlCycFrmSyncCode = m_ParaSetStruct[2][6].InitVal.lval;
    //循环副帧码组位置
    aDDTBBESubFrameSynPara.usCycFrmSynCodePos = m_ParaSetStruct[2][7].InitVal.lval;
    //循环副帧码组长度
    aDDTBBESubFrameSynPara.ucCycFrmSynCodeLen = m_ParaSetStruct[2][8].InitVal.lval;
    aDDTBBESubFrameSynPara.ucCFSearchErrBits = m_ParaSetStruct[2][9].InitVal.lval;
    aDDTBBESubFrameSynPara.ucCFVerifyErrBits = m_ParaSetStruct[2][10].InitVal.lval;
    aDDTBBESubFrameSynPara.ucCFLockErrBits = m_ParaSetStruct[2][11].InitVal.lval;
    aDDTBBESubFrameSynPara.ucSubFVerifyFrame = m_ParaSetStruct[2][12].InitVal.lval;
    aDDTBBESubFrameSynPara.ucSubFLockFrame = m_ParaSetStruct[2][13].InitVal.lval;
    //取当前设置的参数，存入框架
    byteArray.resize(sizeof(aDDTBBESubFrameSynPara));
    memcpy(byteArray.data(), &aDDTBBESubFrameSynPara, sizeof(aDDTBBESubFrameSynPara));
    if(pMacroObj != NULL)
    {
        pMacroObj->UpdateParameterBlock(ParaDDTBBESubFrameSynID, byteArray);
    }
    /*副帧同步器参数*/
    return 1;
}

QWidget* TTCDevCtlInterfaceGUI::ShowMeInEdit(QWidget* parent, uchar ucType, ILxTsspParameterMacro* pMacroObj)
{
    g_form = new TTCDevCtlInterfaceGUIDlg(parent);

    g_form->SetObjInfo(m_ObjectInfo);
    g_form->setText(m_strTitle, m_ParaSetStruct, m_MapMean, MapLineNum, ucType, pMacroObj);
    g_form->setAttribute(Qt::WA_DeleteOnClose);

    return g_form;
}

int TTCDevCtlInterfaceGUI::SaveToMacroObj(ILxTsspParameterMacro* pMacroObj)
{
    if(g_form != NULL)
    {
        g_form->SaveToMacro(pMacroObj);
    }
    return 1;
}

bool TTCDevCtlInterfaceGUI::initDataSource()
{
    int temdwLen = 0;
    /**********************中频开关矩阵申请共享内存**********************************/
    //中频开关矩阵
    ITEM *pItem = g_context.m_devMap.getItem("中频开关矩阵监控");
    if (pItem->vSubItem.size() > 0)
    {
        //申请实时状态结构体共享内存
        SUBITEM sItem = pItem->vSubItem.at(0);
        temdwLen = sizeof(HK_IFSWMatStatu);
        g_pIFSWMatStatuResponse =  (HK_IFSWMatStatu*)ApplyOneItem(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID,
                                                                sItem.usStateType, sItem.ucSN, temdwLen);
        if(!g_pIFSWMatStatuResponse)
            return false;
        memset(g_pIFSWMatStatuResponse, 0, temdwLen);
#if 0   //不需要将响应的内容放入到共享内存中，响应的结果应写入到日志文件中
        //wl add 添加中频命令应答响应结构体
        SUBITEM sItem1 = pItem->vSubItem.at(1);
        temdwLen = sizeof(HK_IFSWMatResponse);
        g_pIFSWMatResponse =  (HK_IFSWMatResponse*)ApplyOneItem(sItem1.usStationID, sItem1.usSubSysID, sItem1.usDeviceID,
                                                            sItem1.usStateType, sItem1.usSN, temdwLen);
        memset(g_pIFSWMatResponse, 0, temdwLen);
#endif // 0
    }
    else
        return false;
    /**********************中频开关矩阵申请共享内存**********************************/

    //MCSDDTRevStatusStruct, g_pMCSDDTRevStatus, "数传接收机参数设置", 0)
//    do{
//        pItem = g_context.m_devMap.getItem( "数传接收机参数设置");
//        if (NULL == pItem || pItem->vSubItem.size() <= 0)return false;
//        SUBITEM sItem = pItem->vSubItem.at(0);
//        temdwLen = sizeof(MCSDDTRevStatusStruct);
//        g_pMCSDDTRevStatus = (MCSDDTRevStatusStruct*)ApplyOneItem(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID, sItem.usStateType, sItem.ucSN, temdwLen);
//        if(!g_pMCSDDTRevStatus)return false;
//        memset(g_pMCSDDTRevStatus, 0, temdwLen);
//    }while(0);


    /**********************数传基带模块申请共享内存**********************************/
    pItem = g_context.m_devMap.getItem("DDT_BBE系统参数设置");
    if (pItem != NULL && pItem->vSubItem.size() > 0)
    {
        SUBITEM sItem = pItem->vSubItem.at(0);
        temdwLen = sizeof(MCSSysDataStatusStruct);
        //DDT_BBE系统参数设置
        g_pMCSSysStatus = (MCSSysDataStatusStruct*)ApplyOneItem(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID,
                                                                sItem.usStateType, sItem.ucSN, temdwLen);
        if(g_pMCSSysStatus == NULL)
            return false;
        memset(g_pMCSSysStatus, 0, temdwLen);
    }
    else
        return false;
    /**********************数传基带模块申请共享内存**********************************/


    /**********************USBBBE中频接收机参数设置申请共享内存**********************************/
    DATA_SOURCE_ALLOC(SMCU2::MCSSSB2PNCODEStatusStruct4, g_p2MCSSSPNCODEStatu4_9, "模式2扩频码设置", 0)
    DATA_SOURCE_ALLOC(SMCU::MCSSSPNCODEStatusStruct4, g_pMCSSSPNCODEStatu4_9, "模式1扩频码设置", 0)
//    DATA_SOURCE_ALLOC(SMCU::MCSSSTMSimulStatusStruct4, g_p2MCSSSTMSimulStatu4_8, "扩频遥测模拟源参数设置", 1)
    DATA_SOURCE_ALLOC(SMCU::MCSSSTMSimulStatusStruct4, g_pMCSSSTMSimulStatu4_8, "扩频遥测模拟源参数设置", 0)
//    DATA_SOURCE_ALLOC(SMCU::MCSSSTMStatusStruct4, g_p2MCSSSTMStatusStruct4_6, "扩频遥测解调参数设置", 1)
    DATA_SOURCE_ALLOC(SMCU::MCSSSTMStatusStruct4, g_pMCSSSTMStatusStruct4_6, "扩频遥测解调参数设置", 0)
//    DATA_SOURCE_ALLOC(SMCU::MCSSSTCStatusStruct4, g_p2MCSSSTCStatusStruct4_5, "扩频遥控参数设置", 1)
    DATA_SOURCE_ALLOC(SMCU::MCSSSTCStatusStruct4, g_pMCSSSTCStatusStruct4_5, "扩频遥控参数设置", 0)
//    DATA_SOURCE_ALLOC(SMCU::MCSSSCjStatusStruct4, g_p2MCSSSCjStatu4_4, "模式2测距测速参数设置", 0)
    DATA_SOURCE_ALLOC(SMCU::MCSSSCjStatusStruct4, g_pMCSSSCjStatu4_4, "模式1测距测速参数设置", 0)
//    DATA_SOURCE_ALLOC(SMCU::MCSSSUTargetTCSSUpModulStatusStruct, g_p2MCSSSUTargetTCSSUpModulStatu3, "模式2调制参数设置", 0)
    DATA_SOURCE_ALLOC(SMCU::MCSSSUTargetTCSSUpModulStatusStruct, g_pMCSSSUTargetTCSSUpModulStatu3, "模式1调制参数设置", 0)
//    DATA_SOURCE_ALLOC(SMCU::MCSSSRevStatusStruct4, g_p2MCSSSRev4Statu4_2, "模式2接收参数设置", 0)
    DATA_SOURCE_ALLOC(SMCU::MCSSSRevStatusStruct4, g_pMCSSSRev4Statu4_2, "模式1接收参数设置", 0)
    DATA_SOURCE_ALLOC(SMCU::MCSSysTarget4StatusStruct, g_pMCSSysTarget4Status1, "扩频系统工作参数设置", 0)
//    DATA_SOURCE_ALLOC(SMCU::MCSSysTarget4StatusStruct, g_p2MCSSysTarget4Status1, "扩频系统工作参数设置", 1)

    DATA_SOURCE_ALLOC(MCSSysDataStatusStruct, g_pMCSSysDataStatusTTC1, "USBBBE系统参数设置", 0)
    DATA_SOURCE_ALLOC(MCSUSBTCStatusStruct, g_pMCSUSBTCStatus7, "USBBBE遥控参数设置", 0)
    DATA_SOURCE_ALLOC(MCSUSBUpModulStatusStruct, g_pMCSUSBUpModulStatus4, "USBBBE调制参数设置", 0)
    DATA_SOURCE_ALLOC(MCSUSBCjStatusStruct, g_pMCSUSBCjStatus6, "USBBBE测距参数设置", 1)
    DATA_SOURCE_ALLOC(MCSUSBCjStatusStruct, g_pMCSUSBCjStatus5, "USBBBE测距参数设置", 0)
    DATA_SOURCE_ALLOC(MCSUSBTMSimulStatusStruct, g_pMCSUSBTMSimulStatus12, "USBBBE模拟源参数设置", 0)
    DATA_SOURCE_ALLOC(MCSUSBTMStatusStruct, g_pMCSUSBTMStatus8, "USBBBE遥测参数设置", 0)
    DATA_SOURCE_ALLOC(MCSUSBRevStatusStruct, g_pMCSUSBRevStatu3, "USBBBE中频接收机参数设置", 1)
    DATA_SOURCE_ALLOC(MCSUSBRevStatusStruct, g_pMCSUSBRevStatu2, "USBBBE中频接收机参数设置", 0)
    DATA_SOURCE_ALLOC(MCSDDTRevStatusStruct, g_pMCSDDTRevStatus, "数传接收机参数设置", 0)
    DATA_SOURCE_ALLOC(MCSDDTTMQStatusStruct, g_pMCSDDTTMQStatusI, "数传解调参数设置", 0)
    DATA_SOURCE_ALLOC(MCSDDTTMQStatusStruct, g_pMCSDDTTMQStatusQ, "数传解调参数设置", 1)
    DATA_SOURCE_ALLOC(MCSDDTTMSimulStatusStruct, g_pMCSDDTTMSimulStatus, "数传信号源参数设置", 0)
    DATA_SOURCE_ALLOC(SCBBETestOutStatusStruct, g_pSCBBETestOutStatu, "数传检测输出参数设置", 0)

    DATA_SOURCE_ALLOC(DevicesConnectStatus, g_context.m_pDeviceLinkStatus, "设备联接状态", 0)
//    /**********************数传基带模块申请共享内存**********************************/

    /**********************分集合成参数设置**********************************/
    pItem = g_context.m_devMap.getItem("分集合成参数设置");
    if (NULL == pItem || pItem->vSubItem.size() > 0)
    {
        SUBITEM sItem = pItem->vSubItem.at(0);
        temdwLen = sizeof(MCSDivComStatusStruct);
        g_pMCSDivComStatus = (MCSDivComStatusStruct*)ApplyOneItem(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID,
                                                                sItem.usStateType, sItem.ucSN, temdwLen);
        if(!g_pMCSDivComStatus)
            return false;
        memset(g_pMCSDivComStatus, 0, temdwLen);
    }
    else
        return false;
//    /**********************分集合成参数设置**********************************/

//    /**********************过程控制命令**********************************/
//    pItem = g_context.m_devMap.getItem("过程控制命令");
//    if (pItem->vSubItem.size() > 0)
//    {
//        SUBITEM sItem = pItem->vSubItem.at(0);
//        temdwLen = sizeof(PCREPORT);
//        g_pPCReport = (PCREPORT*)ApplyOneItem(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID,
//                                                                sItem.usStateType, sItem.ucSN, temdwLen);
//        if(!g_pPCReport)
//            return false;
//        memset(g_pPCReport, 0, temdwLen);
//    }
//    else
//        return false;
//    /**********************过程控制命令**********************************/

    /**********************射频分路组合监控**********************************/
    pItem = g_context.m_devMap.getItem("射频分路组合监控");
    if (pItem != NULL && pItem->vSubItem.size() > 0)
    {
        SUBITEM sItem = pItem->vSubItem.at(0);
        temdwLen = sizeof(SLDCDCUStatu);
        g_pSLDDcuParaStatu = (SLDCDCUStatu*)ApplyOneItem(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID,
                                                                sItem.usStateType, sItem.ucSN, temdwLen);
        if (g_pSLDDcuParaStatu == NULL)
            return false;
        memset(g_pSLDDcuParaStatu, 0, temdwLen);
    }
    else
        return false;
    /**********************射频分路组合监控**********************************/

    /**********************射频切换开关监控**********************************/
    pItem = g_context.m_devMap.getItem("射频切换开关监控");
    if (pItem != NULL && pItem->vSubItem.size() > 0)
    {
        SUBITEM sItem = pItem->vSubItem.at(0);
        temdwLen = sizeof(SSWITCHDCUStatu);
        g_pSswitchDcuStatu = (SSWITCHDCUStatu*)ApplyOneItem(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID,
                                                                sItem.usStateType, sItem.ucSN, temdwLen);
        if(g_pSswitchDcuStatu == NULL)
            return false;
        memset(g_pSswitchDcuStatu, 0, temdwLen);
    }
    else
        return false;
    /**********************射频切换开关监控**********************************/

    /**********************测试开关矩阵监控**********************************/
    pItem = g_context.m_devMap.getItem("测试开关矩阵监控");
    if (pItem != NULL && pItem->vSubItem.size() > 0)
    {
        SUBITEM sItem = pItem->vSubItem.at(0);
        temdwLen = sizeof(TEST_SWMATStatu);
        g_pTstSwMatStatu = (TEST_SWMATStatu*)ApplyOneItem(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID,
                                                                sItem.usStateType, sItem.ucSN, temdwLen);
        if(!g_pTstSwMatStatu)
            return false;
        memset(g_pTstSwMatStatu, 0, temdwLen);
    }
    else
        return false;
    /**********************测试开关矩阵**********************************/

    /**********************频率基准源监控**********************************/
    pItem = g_context.m_devMap.getItem("频率基准源监控");
    if (pItem != NULL && pItem->vSubItem.size() > 0)
    {
        SUBITEM sItem = pItem->vSubItem.at(0);
        temdwLen = sizeof(FREQBDCUStatu);
        g_pFreDcuStatu = (FREQBDCUStatu*)ApplyOneItem(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID,
                                                                sItem.usStateType, sItem.ucSN, temdwLen);
        if(!g_pFreDcuStatu)
            return false;
        memset(g_pFreDcuStatu, 0, temdwLen);
    }
    else
        return false;
    /**********************频率基准源监控**********************************/

    /**********************校零信标一体机监控**********************************/
    pItem = g_context.m_devMap.getItem("校零信标一体机监控");
    if (pItem != NULL && pItem->vSubItem.size() > 0)
    {
        SUBITEM sItem = pItem->vSubItem.at(0);
        temdwLen = sizeof(ZEROCALIBBEACONStatu);
        g_pZCBeaconStatu = (ZEROCALIBBEACONStatu*)ApplyOneItem(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID,
                                                                sItem.usStateType, sItem.ucSN, temdwLen);
        if (g_pZCBeaconStatu == NULL)
            return false;
        memset(g_pZCBeaconStatu, 0, temdwLen);
    }
    else
        return false;
    /**********************校零信标一体机监控**********************************/

    /**********************标准TTC联试应答机监控**********************************/
    pItem = g_context.m_devMap.getItem("标准TTC联试应答机监控");
    if (pItem != NULL && pItem->vSubItem.size() > 0)
    {
        SUBITEM sItem = pItem->vSubItem.at(0);
        //申请工作状态状共享内存数据
        temdwLen = sizeof(COMBTRANSStatu);
        g_pCombTransStatuTTC = (COMBTRANSStatu*)ApplyOneItem(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID,
                                                                sItem.usStateType, sItem.ucSN, temdwLen);
        if (g_pCombTransStatuTTC == NULL)
            return false;
        memset(g_pCombTransStatuTTC, 0, temdwLen);
    }
    else
        return false;

    //申请宏参数查询共享内存结构数据
    pItem = g_context.m_devMap.getItem(RSP_MACRO_SEARCH);
    if (pItem != NULL && pItem->vSubItem.size() > 0)
    {
        //申请宏参数查询共享内存数据
        SUBITEM sItem1 = pItem->vSubItem.at(0);
        temdwLen = sizeof(COMBTRANSPara);
        g_pCombTransParaTTC = (COMBTRANSPara*)ApplyOneItem(sItem1.usStationID, sItem1.ucSubSysID, sItem1.usDeviceID,
                                                                sItem1.usStateType, sItem1.ucSN, temdwLen);
        if(g_pCombTransParaTTC == NULL)
            return false;
        memset(g_pCombTransParaTTC, 0, temdwLen);
    }
    else
        return false;
    /**********************标准TTC联试应答机监控**********************************/

    /**********************非相干扩频联试应答机监控**********************************/
    pItem = g_context.m_devMap.getItem("非相干扩频联试应答机监控");
    if (pItem !=NULL && pItem->vSubItem.size() > 0)
    {
        SUBITEM sItem = pItem->vSubItem.at(0);
        //申请工作状态状共享内存数据
        temdwLen = sizeof(COMBTRANSStatu);
        g_pCombTransStatuFXG = (COMBTRANSStatu*)ApplyOneItem(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID,
                                                                sItem.usStateType, sItem.ucSN, temdwLen);
        if (g_pCombTransStatuFXG == NULL)
            return false;
        memset(g_pCombTransStatuFXG, 0, temdwLen);
    }
    else
        return false;

    //申请宏参数查询共享内存结构数据
    pItem = g_context.m_devMap.getItem(RSP_MACRO_SEARCH);
    if (pItem != NULL && pItem->vSubItem.size() > 1)
    {
        //申请宏参数查询共享内存数据
        SUBITEM sItem1 = pItem->vSubItem.at(1);
        temdwLen = sizeof(COMBTRANSPara);
        g_pCombTransParaFXG = (COMBTRANSPara*)ApplyOneItem(sItem1.usStationID, sItem1.ucSubSysID, sItem1.usDeviceID,
                                                                sItem1.usStateType, sItem1.ucSN, temdwLen);
        if (g_pCombTransParaFXG == NULL)
            return false;
        memset(g_pCombTransParaFXG, 0, temdwLen);
    }
    else
        return false;
    /**********************非相干扩频联试应答机监控**********************************/

    /*****************************铷钟机箱监控***************************************/
    pItem = g_context.m_devMap.getItem("铷钟机箱监控");
    if (pItem !=NULL && pItem->vSubItem.size() > 0)
    {
        SUBITEM sItem = pItem->vSubItem.at(0);
        //申请工作状态状共享内存数据
        temdwLen = sizeof(RCLOCKDCUStatu);
        g_pRCLOCKDCUStatu = (RCLOCKDCUStatu*)ApplyOneItem(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID,
                                                                sItem.usStateType, sItem.ucSN, temdwLen);
        if (g_pRCLOCKDCUStatu == NULL)
            return false;
        memset(g_pRCLOCKDCUStatu, 0, temdwLen);
    }
    else
        return false;
    /*****************************铷钟机箱监控***************************************/
    return true;
}
