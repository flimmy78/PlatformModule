#include "TMRemoteInterfaceGUI.h"
#include "TMRemoteInterfaceGUIDlg.h"
#include "XmlReader.h"
#include "../../Common/Dcu_common_DataStruct.h"
#include <QDir>
#include <QDateTime>
#include "../../Common/DeviceMap.h"
#include "../../Common/Log.h"
#include "ProcessModule.h"


namespace
{
    CDevCtlInterfaceGUIDlg* g_form = NULL;
}
extern quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
              uchar ucSn);

CDevCtlInterfaceGUI::CDevCtlInterfaceGUI(TLxTsspObjectInfo ObjectInfo)
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
QWidget* CDevCtlInterfaceGUI::ShowMe(QWidget* parent, const QString& strTitle)
{
    m_strTitle = strTitle;

    CDevCtlInterfaceGUIDlg* form;

    form = new CDevCtlInterfaceGUIDlg(parent);

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
int CDevCtlInterfaceGUI::GetContextHelpID(char* szHelpID)
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
int CDevCtlInterfaceGUI::GetModuleInfo(TLxTsspModuleInfo& moduleInfo) const
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
int CDevCtlInterfaceGUI::GetObjectInfo(TLxTsspObjectInfo& ObjectInfo) const
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
int CDevCtlInterfaceGUI::PreInitialize()
{
    return 1;
}

/**
 * 初始化 ：模块在此函数中执行必要的初始化工作。
 * @return 1：成功；0：未定义；-1：失败。
 */
int CDevCtlInterfaceGUI::Initialize()
{
    g_context.m_devMap.loadXML();
    //查找对象管理器
    if(false == m_platformRes.init(MODULE))
    {
        CLog::addLog("平台提供的资源申请失败。", 1);
        return -1;
    }
    if(false == m_recvThread.init(m_platformRes,&m_searchMarc))
    {
        CLog::addLog("接收线程初始化失败。", 1);
        return -1;
    }
    if(false == m_searchMarc.init(&m_platformRes))
    {
        CLog::addLog("宏线程初始化失败。", 1);
        return -1;
    }

    if(false == m_searchStatuThread.init(&m_platformRes))
    {
        CLog::addLog("状态线程初始化失败。", 1);
        return -1;
    }

     if(false == m_connectStatu.init(&m_recvThread, &m_platformRes))
    {
        CLog::addLog("链接状态处理线程初始化失败。", 1);
        return -1;
    }
    m_recvThread.start();
    m_searchStatuThread.start();
    m_searchMarc.start();
    m_connectStatu.start();
    g_context.initIDs();
    return 1;
}

//遍历xlm文件，获取参数取值范围
int CDevCtlInterfaceGUI::QueryParaBlock(QString )
{
    return 1;
}

int CDevCtlInterfaceGUI::GetAllMapValueStringByPara(int MapSum,QDomNode pInDocNode_Para)
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
int CDevCtlInterfaceGUI::Start()
{
    return 1;
}

/**
 * 停止 ：模块在此函数中停止功能的执行，尤其是包含线程的模块，应该在此结束线程。
 * @return 1：成功；0：未定义；-1：失败。
 */
int CDevCtlInterfaceGUI::Stop()
{
    return 1;
}

/**
 * 清理：模块在此函数中清理申请的资源，为进程退出做准备。
 * @return 1：成功；0：未定义；-1：失败。
 */
bool CDevCtlInterfaceGUI::ClearUp()
{
    return 1 == m_platformRes.m_pMsgMgr->MsgReceiverUnregister(0xFFFF, 0xFFFF, MODULE);
}

/*--------------对象故障诊断接口部分----------------------------*/
/**
 * 取错误码： 取对象的错误码。
 * @return： 返回模块的错误码，用于判断模块不能执行的原因:
 *           0表示运行正常；-1表示没有配置信息；-2表示初始化失败；
 *           -3表示启动操作失败；-4表示停止操作失败；-5表示清理资源失败；
 *           其它错误依情况增加；
 */
int CDevCtlInterfaceGUI::GetErrorNumber()
{
    return m_iErrCode;
}

/**
 * 取错误信息
 * @param iErrorNum：错误码。
 * @param[out] szErrorMsg：错误信息，描述性的错误信息，用于判断对象不能执行的原因。
 * @return 1：成功；0：未定义；-1：表示没有对应的错误信息；
 */
int CDevCtlInterfaceGUI::GetErrorMsg(int iErrorNo, QString& strErrorMsg)
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
bool CDevCtlInterfaceGUI::IsOnline()
{
    return false;
}

/**
 * 发送消息 ：通过此函数可实现信息的直接发送，对于对象管理器，当此函数被调用时，
 *          向管理的所有对象发送此消息。
 * @param msg：消息。对象收到消息后，可以拷贝消息内容，但不应更改。
 */
void CDevCtlInterfaceGUI::SendMessage(TLxTsspMessage )
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
int CDevCtlInterfaceGUI::SetParameter(QMap<quint64, QByteArray> )
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
int CDevCtlInterfaceGUI::SetParameter(quint64 ui64ParamID, QByteArray parameter)
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
int CDevCtlInterfaceGUI::GetParameterID(QList<quint64>& pParameterIDList) const
{
    pParameterIDList += m_ParamList.keys();

    return m_ParamList.count();
}

/**
 * 取参数 ：获取本对象的参数，可以一次读取所有参数。
 * @param[out] parameterList：参数块ID和参数值的键值对的列表。
 * @return 大于等于0：参数块个数。-1：失败。
 */
int CDevCtlInterfaceGUI::GetParameter(QMap<quint64, QByteArray>& ) const
{
    return -1;
}

/**
 * 取参数 ：获取本对象的参数，按照指定的ID读取。
 * @param ui64ParamID：参数块ID。
 * @param[out] parameter：参数块的值；
 * @return 1：成功；-1：失败。
 */
int CDevCtlInterfaceGUI::GetParameter(quint64 ui64ParamID, QByteArray& parameter) const
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
int CDevCtlInterfaceGUI::GetState(QMap<quint64, TLxTsspDeviceState>& stateList) const
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
int CDevCtlInterfaceGUI::GetState(quint64 ui64StatusID, TLxTsspDeviceState& status) const
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
int CDevCtlInterfaceGUI::GetStateDirect(quint64 ui64StatusID, TLxTsspDeviceState& )
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
int CDevCtlInterfaceGUI::WriteData(quint64 , uchar* , uint )
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
int CDevCtlInterfaceGUI::ReadData(quint64 , uchar* , uint& ,
                             uint& )
{
    return 0;
}

int CDevCtlInterfaceGUI::CreateDefaultMacroObj(ILxTsspParameterMacro* )
{
    return 1;
}

QWidget* CDevCtlInterfaceGUI::ShowMeInEdit(QWidget* parent, uchar ucType, ILxTsspParameterMacro* pMacroObj)
{
    g_form = new CDevCtlInterfaceGUIDlg(parent);

    g_form->SetObjInfo(m_ObjectInfo);
    g_form->setText(m_strTitle, m_ParaSetStruct, m_MapMean, MapLineNum, ucType, pMacroObj);
    g_form->setAttribute(Qt::WA_DeleteOnClose);

    return g_form;
}

int CDevCtlInterfaceGUI::SaveToMacroObj(ILxTsspParameterMacro* pMacroObj)
{
    if(g_form != NULL)
    {
        g_form->SaveToMacro(pMacroObj);
    }
    return 1;
}

