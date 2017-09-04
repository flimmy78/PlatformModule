#include "SSBBE_TMParaSetGUI.h"
#include "SSBBE_TMParaSetGUIDlg.h"
#include "../../Common/Dcu_common_DataStruct.h"
#include <QDir>
#include <QDateTime>
#include "../../Common/Log.h"
namespace
{
SSBBE_TMParaSetGUIDlg* g_form = NULL;
}
extern quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
                     uchar ucSn);
SSBBE_TMParaSetGUI::SSBBE_TMParaSetGUI(TLxTsspObjectInfo ObjectInfo)
{
    //初始化
    memcpy(&m_ObjectInfo, &ObjectInfo, sizeof(TLxTsspObjectInfo));

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
QWidget* SSBBE_TMParaSetGUI::ShowMe(QWidget* parent, const QString& strTitle)
{
    m_strTitle = strTitle;

    SSBBE_TMParaSetGUIDlg* form;

    form = new SSBBE_TMParaSetGUIDlg(parent);

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
int SSBBE_TMParaSetGUI::GetContextHelpID(char* szHelpID)
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
int SSBBE_TMParaSetGUI::GetModuleInfo(TLxTsspModuleInfo& moduleInfo) const
{
    memset(&moduleInfo, 0, sizeof(TLxTsspModuleInfo));

    // 请按照实际模块的名称、标识、版本、开发人员填写。
    /// 公司名称,固定为“中国航天科技集团九院七〇四所”
    strcpy(moduleInfo.szCompany, "中国航天科技集团九院七〇四所\0");

    /// 模块名称，如：系统工作参数设置
    strcpy(moduleInfo.szModuleName, MODULENAME);

    /// 配置标识，如：D_Y17-61_008
    strcpy(moduleInfo.szConfigID, "D_Y15-188_053\0");

    /// 版本
    strcpy(moduleInfo.szVersion, "01.00\0");

    /// 开发者，如：大卫.奥巴马
    strcpy(moduleInfo.szCreator, "温艳\0");

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
int SSBBE_TMParaSetGUI::GetObjectInfo(TLxTsspObjectInfo& ObjectInfo) const
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
int SSBBE_TMParaSetGUI::PreInitialize()
{
    return 1;
}

/**
 * 初始化 ：模块在此函数中执行必要的初始化工作。
 * @return 1：成功；0：未定义；-1：失败。
 */
int SSBBE_TMParaSetGUI::Initialize()
{
    //日志信息
    TLxTsspLog tmpLog;
    QString strLog;

    //查找对象管理器
    LxTsspObjectManager* pObjectManager = LxTsspObjectManager::Instance();

    if(pObjectManager == NULL){
        return -1;
    }

    //查找日志管理器
    m_pILxTsspLogManager = pObjectManager->GetLogManager();
    if(m_pILxTsspLogManager == NULL){
        return -1;
    }

    QString stCurPath = QDir::currentPath();
    stCurPath = stCurPath + "/System/ModuleConfig/SSBBE_TMParaSetGUI.xml";
    //遍历xml文件，获取参数取值范围
    m_MapSumFlag = 0;
    if(QueryParaBlock(stCurPath) < 0)
    {
        m_iErrCode = -1;

        //日志信息
        tmpLog.ucWarningLevel  = 3;
        strLog = QString("%1: 读取配置文件失败！").arg(MODULENAME);
        CLog::addLog(strLog, 3);
        return -1;
    }

    bool bRet = m_DeviceMap.loadXML();
    if (!bRet)
    {
        QString strInfo = QString("load device map failed %1").arg(MODULENAME);
        CLog::addLog(strInfo);
        return -1;
    }
    ITEM *pItem = m_DeviceMap.getItem(MODULENAME);
    if (pItem != NULL && pItem->vSubItem.size() > 0)
    {
        SUBITEM& sItem = *(pItem->getSubItemByDeviceID_SN(m_ObjectInfo.usLocalDID, m_ObjectInfo.ucSN));
        /*************************框架**********************************/
        m_ParaTMPID = GetID(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID,
                            sItem.usTID, sItem.ucSN);
        /*************************框架**********************************/

        /*************************状态**********************************/
        m_StatusTMPID = GetID(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID,
                              sItem.usStateType, sItem.ucSN);
        /*************************状态**********************************/
    }

    //日志信息
    tmpLog.ucWarningLevel  = 1;
    strLog = QString("%1: 读取配置文件初始化调制参数成功！").arg(MODULENAME);
    CLog::addLog(strLog, 1);
    return 1;
}

//遍历xml文件，获取参数取值范围
int SSBBE_TMParaSetGUI::QueryParaBlock(QString ConfigPath)
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

int SSBBE_TMParaSetGUI::GetAllMapValueStringByPara(int MapSum,QDomNode pInDocNode_Para)
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
int SSBBE_TMParaSetGUI::Start()
{
    return 1;
}

/**
 * 停止 ：模块在此函数中停止功能的执行，尤其是包含线程的模块，应该在此结束线程。
 * @return 1：成功；0：未定义；-1：失败。
 */
int SSBBE_TMParaSetGUI::Stop()
{
    return 1;
}

/**
 * 清理：模块在此函数中清理申请的资源，为进程退出做准备。
 * @return 1：成功；0：未定义；-1：失败。
 */
bool SSBBE_TMParaSetGUI::ClearUp()
{
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
int SSBBE_TMParaSetGUI::GetErrorNumber()
{
    return m_iErrCode;
}

/**
 * 取错误信息
 * @param iErrorNum：错误码。
 * @param[out] szErrorMsg：错误信息，描述性的错误信息，用于判断对象不能执行的原因。
 * @return 1：成功；0：未定义；-1：表示没有对应的错误信息；
 */
int SSBBE_TMParaSetGUI::GetErrorMsg(int iErrorNo, QString& strErrorMsg)
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
bool SSBBE_TMParaSetGUI::IsOnline()
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
                               0x0135,2);
//SS1BBE_GlobalStatusStruct* m_pSS1Abstract;
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
        SS1BBE_GlobalStatusStruct* pBBEGlobalStatus = NULL;//基带全局信息元
        if(pBBEGlobalStatus == NULL)
        {
            stLen = sizeof(SS1BBE_GlobalStatusStruct);
            pBBEGlobalStatus  =
                    (SS1BBE_GlobalStatusStruct*)pStateMgr->FindOneItem(tempBBEGlobalID, stLen);
            if(pBBEGlobalStatus == NULL || stLen != sizeof(SS1BBE_GlobalStatusStruct))
                return false;
        }

        // 1-fm 2-bpsk 4-ss
        if(pBBEGlobalStatus->ucSystemWorkMode != c_SS1)//该模式 不是BPSK模式
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
void SSBBE_TMParaSetGUI::SendMessage(TLxTsspMessage msg)
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
int SSBBE_TMParaSetGUI::SetParameter(QMap<quint64, QByteArray> parameterList)
{
    //对于参数组中的参数，首先判断是否本对象参数
    foreach(quint64 key, parameterList.keys())
    {
        //参数加载到硬件、本地保存、本地使用。
    }

    return 1;
}

/**
 * 设置参数 ：设置单个参数。
 * @param ui64ParamID：参数ID,格式为：
 * 分系统号(1)/单机号(2)/参数块标识(2)/序号(1)/参数号(2)。共8个字节，
 * @param parameter：参数块数据
 * @return 1：成功；0：非本对象参数；-1：失败。
 */
int SSBBE_TMParaSetGUI::SetParameter(quint64 ui64ParamID, QByteArray parameter)
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
int SSBBE_TMParaSetGUI::GetParameterID(QList<quint64>& pParameterIDList) const
{
    pParameterIDList += m_ParamList.keys();

    return m_ParamList.count();
}

/**
 * 取参数 ：获取本对象的参数，可以一次读取所有参数。
 * @param[out] parameterList：参数块ID和参数值的键值对的列表。
 * @return 大于等于0：参数块个数。-1：失败。
 */
int SSBBE_TMParaSetGUI::GetParameter(QMap<quint64, QByteArray>& parameterList) const
{
    SMCU::MCSSSTMParamStruct4 tmpMCSSSTMParam;       //码同步参数

    SMCU::MCSSSTMStatusStruct4* tmpMCSSSTMStatus = NULL;      //码同步参数

    ILxTsspStateManager*  tmpStateMgr;
    //查找对象管理器
    LxTsspObjectManager* pObjectManager = LxTsspObjectManager::Instance();
    tmpStateMgr = pObjectManager->GetStateManager();

    DWORD stLen = sizeof(SMCU::MCSSSTMStatusStruct4);
    if(tmpStateMgr != NULL)
        tmpMCSSSTMStatus  =
                (SMCU::MCSSSTMStatusStruct4*)tmpStateMgr->FindOneItem(m_StatusTMPID, stLen);

    if(tmpMCSSSTMStatus != NULL)
    {
        for (int i = 0; i < MAX_TARGET_COUNT; i++)
            tmpMCSSSTMParam.SSTMParam[i] = tmpMCSSSTMStatus->SSTMStatu[i].tParams;
    }

    QByteArray  tmpCh;
    tmpCh.resize(sizeof(SMCU::MCSSSTMParamStruct4));
    memcpy(tmpCh.data(), &tmpMCSSSTMParam, sizeof(SMCU::MCSSSTMParamStruct4));
    parameterList.insert(m_ParaTMPID, tmpCh);

    parameterList.unite(m_ParamList);

    return m_ParamList.count();
}

/**
 * 取参数 ：获取本对象的参数，按照指定的ID读取。
 * @param ui64ParamID：参数块ID。
 * @param[out] parameter：参数块的值；
 * @return 1：成功；-1：失败。
 */
int SSBBE_TMParaSetGUI::GetParameter(quint64 ui64ParamID, QByteArray& parameter) const
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
int SSBBE_TMParaSetGUI::GetState(QMap<quint64, TLxTsspDeviceState>& stateList) const
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
int SSBBE_TMParaSetGUI::GetState(quint64 ui64StatusID, TLxTsspDeviceState& status) const
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
int SSBBE_TMParaSetGUI::GetStateDirect(quint64 ui64StatusID, TLxTsspDeviceState& status)
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
int SSBBE_TMParaSetGUI::WriteData(quint64 ui64DataID, uchar* aData, uint uiDataLen)
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
int SSBBE_TMParaSetGUI::ReadData(quint64 ui64DataID, uchar* aData, uint& uiDataLen,
                                 uint& uiReloadCount)
{
    return 0;
}

int SSBBE_TMParaSetGUI::CreateDefaultMacroObj(ILxTsspParameterMacro* pMacroObj)
{
    /*码同步参数*/
    SMCU::MCSSSTMParamStruct4 aMCSSSTMParam;
    for (int nIndex = 0; nIndex < MAX_TARGET_COUNT; nIndex++)
    {
        //帧同步码组位置
        aMCSSSTMParam.SSTMParam[nIndex].ucFspPlace = m_ParaSetStruct[nIndex][1].InitVal.lval + 1;
        //码型
        aMCSSSTMParam.SSTMParam[nIndex].uiCodeStyle = m_ParaSetStruct[nIndex][2].InitVal.lval + 1;
        //译码方式
        aMCSSSTMParam.SSTMParam[nIndex].ucCodingMd = m_ParaSetStruct[nIndex][4].InitVal.lval + 1;
        //码速率
        aMCSSSTMParam.SSTMParam[nIndex].uiTM_CodeRate = m_ParaSetStruct[nIndex][3].InitVal.lval;
        //Viterbi编码
        aMCSSSTMParam.SSTMParam[nIndex].ucViterbiMd = m_ParaSetStruct[nIndex][5].InitVal.lval + 1;
        //RS码类型
        aMCSSSTMParam.SSTMParam[nIndex].ucRSMd = m_ParaSetStruct[nIndex][7].InitVal.lval + 1;
        //RS码坐标
        aMCSSSTMParam.SSTMParam[nIndex].ucBasiSel = m_ParaSetStruct[nIndex][8].InitVal.lval + 1;
        //RS译码交织深度
        aMCSSSTMParam.SSTMParam[nIndex].ucInterleav = m_ParaSetStruct[nIndex][9].InitVal.lval;
        //维特比译码输出相位
        aMCSSSTMParam.SSTMParam[nIndex].ucG2Inv = m_ParaSetStruct[nIndex][6].InitVal.lval + 1;
        //格式长
        aMCSSSTMParam.SSTMParam[nIndex].ucSframeLen = m_ParaSetStruct[nIndex][12].InitVal.lval;

        //帧长
        aMCSSSTMParam.SSTMParam[nIndex].usFrameLen = m_ParaSetStruct[nIndex][10].InitVal.lval;
        //帧同步码组长
        aMCSSSTMParam.SSTMParam[nIndex].ucFrameSynGLen = m_ParaSetStruct[nIndex][13].InitVal.lval + 1;
        //帧同步码组
        aMCSSSTMParam.SSTMParam[nIndex].uiFrameSynG = m_ParaSetStruct[nIndex][14].InitVal.lval;
        //字长
        aMCSSSTMParam.SSTMParam[nIndex].ucWordLen = m_ParaSetStruct[nIndex][11].InitVal.lval;
        //解扰使能
        aMCSSSTMParam.SSTMParam[nIndex].ucUnturbCtrl = m_ParaSetStruct[nIndex][15].InitVal.lval + 1;
        //解扰多项式长
        aMCSSSTMParam.SSTMParam[nIndex].ucRandLen = m_ParaSetStruct[nIndex][16].InitVal.lval + 1;
        //解扰多项式
        aMCSSSTMParam.SSTMParam[nIndex].uiRandPoly = m_ParaSetStruct[nIndex][17].InitVal.lval;
        //解扰初相
        aMCSSSTMParam.SSTMParam[nIndex].uiPhase = m_ParaSetStruct[nIndex][18].InitVal.lval;
        //副帧码组长
        aMCSSSTMParam.SSTMParam[nIndex].ucCycFrmSynCodeLen = m_ParaSetStruct[nIndex][20].InitVal.lval + 1;
        //副帧码组
        aMCSSSTMParam.SSTMParam[nIndex].uiCycFrmSynCode = m_ParaSetStruct[nIndex][22].InitVal.lval;
        //副帧同步方式
        aMCSSSTMParam.SSTMParam[nIndex].ucSFrameSynMd = m_ParaSetStruct[nIndex][19].InitVal.lval + 1;
        //副帧码组位置
        aMCSSSTMParam.SSTMParam[nIndex].usCycFrmSynCodePos = m_ParaSetStruct[nIndex][21].InitVal.lval + 1;
        //ID字基值
        aMCSSSTMParam.SSTMParam[nIndex].ucIDBase = m_ParaSetStruct[nIndex][24].InitVal.lval + 1;
        //ID计数方向
        aMCSSSTMParam.SSTMParam[nIndex].ucIDDirect = m_ParaSetStruct[nIndex][25].InitVal.lval + 1;
        //ID计数位置
        aMCSSSTMParam.SSTMParam[nIndex].usIDSubFrmPos = m_ParaSetStruct[nIndex][23].InitVal.lval;


        //帧同步容错搜索容错位数
        aMCSSSTMParam.SSTMParam[nIndex].ucFrSynSerLiErr = m_ParaSetStruct[nIndex][26].InitVal.lval;
        //帧同步容错校核容错位数
        aMCSSSTMParam.SSTMParam[nIndex].ucFrSynVerLiErr = m_ParaSetStruct[nIndex][27].InitVal.lval;
        //帧同步容错锁定容错位数
        aMCSSSTMParam.SSTMParam[nIndex].ucFrSynLoLiErr = m_ParaSetStruct[nIndex][28].InitVal.lval;
        //帧同步容错校核检测帧数
        aMCSSSTMParam.SSTMParam[nIndex].ucFrSynVerCheck = m_ParaSetStruct[nIndex][29].InitVal.lval + 1;
        //帧同步容错搜索容错位数
        aMCSSSTMParam.SSTMParam[nIndex].ucFrSynLoCheck = m_ParaSetStruct[nIndex][30].InitVal.lval + 1;
        //帧同步容错搜索容错位数
        aMCSSSTMParam.SSTMParam[nIndex].ucCFSearchErrBits = m_ParaSetStruct[nIndex][31].InitVal.lval;
        //帧同步容错搜索容错位数
        aMCSSSTMParam.SSTMParam[nIndex].ucCFVerifyErrBits = m_ParaSetStruct[nIndex][32].InitVal.lval;
        //帧同步容错搜索容错位数
        aMCSSSTMParam.SSTMParam[nIndex].ucCFLockErrBits = m_ParaSetStruct[nIndex][33].InitVal.lval;
        //帧同步容错搜索容错位数
        aMCSSSTMParam.SSTMParam[nIndex].ucSubFVerifyFrame = m_ParaSetStruct[nIndex][34].InitVal.lval + 1;
        //帧同步容错搜索容错位数
        aMCSSSTMParam.SSTMParam[nIndex].ucSubFLockFrame = m_ParaSetStruct[nIndex][35].InitVal.lval + 1;

        //卫星载波锁定波道
        aMCSSSTMParam.SSTMParam[nIndex].usSatCarrLockChnl = m_ParaSetStruct[nIndex][36].InitVal.lval;
        //卫星载波锁定波道
        aMCSSSTMParam.SSTMParam[nIndex].ucSatCarrLockBit = m_ParaSetStruct[nIndex][37].InitVal.lval;
        //卫星载波锁定波道
        aMCSSSTMParam.SSTMParam[nIndex].usSatLPNLockChnl = m_ParaSetStruct[nIndex][38].InitVal.lval;
        //卫星载波锁定波道
        aMCSSSTMParam.SSTMParam[nIndex].ucSatLPNLockBit = m_ParaSetStruct[nIndex][39].InitVal.lval;
        //卫星载波锁定波道
        aMCSSSTMParam.SSTMParam[nIndex].usSatSPNLockChnl = m_ParaSetStruct[nIndex][40].InitVal.lval;
        //卫星载波锁定波道
        aMCSSSTMParam.SSTMParam[nIndex].ucSatSPNLockBit = m_ParaSetStruct[nIndex][41].InitVal.lval;
    }
    //取当前设置的参数，存入框架
    QByteArray byteArray;
    byteArray.resize(sizeof(aMCSSSTMParam));
    memcpy(byteArray.data(),&aMCSSSTMParam,sizeof(aMCSSSTMParam));
    if(pMacroObj!=NULL)
    {
        pMacroObj->UpdateParameterBlock(m_ParaTMPID,byteArray);
    }
    return 1;
}

QWidget* SSBBE_TMParaSetGUI::ShowMeInEdit(QWidget* parent,uchar ucType,ILxTsspParameterMacro* pMacroObj)
{
    g_form = new SSBBE_TMParaSetGUIDlg(parent);

    g_form->SetObjInfo(m_ObjectInfo);
    g_form->setText(m_strTitle, m_ParaSetStruct, m_MapMean, MapLineNum, ucType, pMacroObj);
    g_form->setAttribute(Qt::WA_DeleteOnClose);

    return g_form;
}

int SSBBE_TMParaSetGUI::SaveToMacroObj(ILxTsspParameterMacro* pMacroObj)
{
    if(g_form!=NULL)
    {
        g_form->SaveToMacro(pMacroObj);
    }

    return 1;
}
