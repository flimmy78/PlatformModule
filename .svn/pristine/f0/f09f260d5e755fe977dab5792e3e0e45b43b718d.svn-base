#include "SS2RSPDcuGUI.h"
#include "SS2RSPDcuGUIDlg.h"
#include "../../Common/Dcu_common_DataStruct.h"
#include <QDir>
#include <QDateTime>
#include "../../Common/Log.h"

namespace
{
    SS2RSPDcuGUIDlg* g_form = NULL;
}
extern quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
              uchar ucSn);

SS2RSPDcuGUI::SS2RSPDcuGUI(TLxTsspObjectInfo ObjectInfo)
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
QWidget* SS2RSPDcuGUI::ShowMe(QWidget* parent, const QString& strTitle)
{
    m_strTitle = strTitle;

    SS2RSPDcuGUIDlg* form;
    form = new SS2RSPDcuGUIDlg(parent);

    //设置对象配置信息
    form->SetObjInfo(m_ObjectInfo);
    //设置界面信息
    form->setText(strTitle, m_ParaSetStruct, m_MapMean, MapLineNum, 0, NULL);//根据需要
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
int SS2RSPDcuGUI::GetContextHelpID(char* szHelpID)
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
int SS2RSPDcuGUI::GetModuleInfo(TLxTsspModuleInfo& moduleInfo) const
{
    memset(&moduleInfo, 0, sizeof(TLxTsspModuleInfo));

    // 请按照实际模块的名称、标识、版本、开发人员填写。
    /// 公司名称,固定为“中国航天科技集团九院七〇四所”
    strcpy(moduleInfo.szCompany, "中国航天科技集团九院七〇四所\0");

    /// 模块名称，如：系统工作参数设置
    strcpy(moduleInfo.szModuleName, MODULENAME);

    /// 配置标识，如：D_Y17-61_008
    strcpy(moduleInfo.szConfigID, "D_Y17-92_131\0");

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
int SS2RSPDcuGUI::GetObjectInfo(TLxTsspObjectInfo& ObjectInfo) const
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
int SS2RSPDcuGUI::PreInitialize()
{
    return 1;
}

/**
 * 初始化 ：模块在此函数中执行必要的初始化工作。
 * @return 1：成功；0：未定义；-1：失败。
 */
int SS2RSPDcuGUI::Initialize()
{
    //日志信息
    TLxTsspLog tmpLog;
    QString strLog;

    //查找对象管理器
    LxTsspObjectManager* pObjectManager = LxTsspObjectManager::Instance();
    if(pObjectManager == NULL)
    {
        return -1;
    }

    //查找日志管理器
    m_pILxTsspLogManager = pObjectManager->GetLogManager();
    if(m_pILxTsspLogManager == NULL)
    {
        return -1;
    }

    QString stCurPath = QDir::currentPath();
    stCurPath = stCurPath + "/System/ModuleConfig/SS2RSPDcuGUI.xml";
    //遍历xlm文件，获取参数取值范围
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
        QString strInfo = QString("load device map failed %1");
        CLog::addLog(strInfo);
        return -1;
    }
    ITEM *pItem = m_DeviceMap.getItem(MODULENAME);
    if (pItem != NULL && pItem->vSubItem.size() > 0)
    {
        SUBITEM sItem = pItem->vSubItem.at(0);
        /*************************框架**********************************/
        m_ParaComSynID = GetID(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID,
                                     sItem.usTID, sItem.ucSN);
        /*************************框架**********************************/

        /*************************状态**********************************/
        m_StatusComSynID = GetID(sItem.usStationID, sItem.ucSubSysID, sItem.usDeviceID,
                                       sItem.usStateType, sItem.ucSN);
        /*************************状态**********************************/
    }

    //日志信息
    tmpLog.ucWarningLevel  = 1;
    strLog = QString("%1: 读取配置文件初始化调制参数成功！").arg(MODULENAME);
    CLog::addLog(strLog, 1);

    return 1;
}

//遍历xlm文件，获取参数取值范围
int SS2RSPDcuGUI::QueryParaBlock(QString ConfigPath)
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

int SS2RSPDcuGUI::GetAllMapValueStringByPara(int MapSum,QDomNode pInDocNode_Para)
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
int SS2RSPDcuGUI::Start()
{
    return 1;
}

/**
 * 停止 ：模块在此函数中停止功能的执行，尤其是包含线程的模块，应该在此结束线程。
 * @return 1：成功；0：未定义；-1：失败。
 */
int SS2RSPDcuGUI::Stop()
{
    return 1;
}

/**
 * 清理：模块在此函数中清理申请的资源，为进程退出做准备。
 * @return 1：成功；0：未定义；-1：失败。
 */
bool SS2RSPDcuGUI::ClearUp()
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
int SS2RSPDcuGUI::GetErrorNumber()
{
    return m_iErrCode;
}

/**
 * 取错误信息
 * @param iErrorNum：错误码。
 * @param[out] szErrorMsg：错误信息，描述性的错误信息，用于判断对象不能执行的原因。
 * @return 1：成功；0：未定义；-1：表示没有对应的错误信息；
 */
int SS2RSPDcuGUI::GetErrorMsg(int iErrorNo, QString& strErrorMsg)
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
bool SS2RSPDcuGUI::IsOnline()
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
                               0x0135,m_ObjectInfo.ucSN);

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
            pBBEGlobalStatus  =
                    (BBE_GlobalStatusStruct*)pStateMgr->FindOneItem(tempBBEGlobalID, stLen);
            if(pBBEGlobalStatus == NULL || stLen != sizeof(BBE_GlobalStatusStruct))
                return false;
        }

        // 1-fm 2-bpsk 4-ss
        if(pBBEGlobalStatus->DestMCSSysStatus.dwSystemMode !=2)//该模式 不是BPSK模式
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
void SS2RSPDcuGUI::SendMessage(TLxTsspMessage msg)
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
int SS2RSPDcuGUI::SetParameter(QMap<quint64, QByteArray> parameterList)
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
int SS2RSPDcuGUI::SetParameter(quint64 ui64ParamID, QByteArray parameter)
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
int SS2RSPDcuGUI::GetParameterID(QList<quint64>& pParameterIDList) const
{
    pParameterIDList += m_ParamList.keys();

    return m_ParamList.count();
}

/**
 * 取参数 ：获取本对象的参数，可以一次读取所有参数。
 * @param[out] parameterList：参数块ID和参数值的键值对的列表。
 * @return 大于等于0：参数块个数。-1：失败。
 */
int SS2RSPDcuGUI::GetParameter(QMap<quint64, QByteArray>& parameterList) const
{
    /*RCLOCKDCUStatu tmpComPara;          //参数

    RCLOCKDCUStatu *tmpComStatus = NULL;  //状态参数

    ILxTsspStateManager*  tmpStateMgr;
    //查找对象管理器
    LxTsspObjectManager* pObjectManager = LxTsspObjectManager::Instance();
    tmpStateMgr = pObjectManager->GetStateManager();

    DWORD stLen = sizeof(RCLOCKDCUStatu);
    if(tmpStateMgr != NULL)
        tmpComStatus  =
                (RCLOCKDCUStatu*)tmpStateMgr->FindOneItem(m_StatusComSynID, stLen);

    if(tmpComStatus != NULL)
    {
        //tmpComPara = tmpComStatus;
    }

    QByteArray  tmpCh;
    tmpCh.resize(sizeof(RCLOCKDCUStatu));
    memcpy(tmpCh.data(), tmpComStatus, sizeof(RCLOCKDCUStatu));
    parameterList.insert(m_ParaComSynID, tmpCh);

    parameterList.unite(m_ParamList);*/

    return m_ParamList.count();
}

/**
 * 取参数 ：获取本对象的参数，按照指定的ID读取。
 * @param ui64ParamID：参数块ID。
 * @param[out] parameter：参数块的值；
 * @return 1：成功；-1：失败。
 */
int SS2RSPDcuGUI::GetParameter(quint64 ui64ParamID, QByteArray& parameter) const
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
int SS2RSPDcuGUI::GetState(QMap<quint64, TLxTsspDeviceState>& stateList) const
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
int SS2RSPDcuGUI::GetState(quint64 ui64StatusID, TLxTsspDeviceState& status) const
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
int SS2RSPDcuGUI::GetStateDirect(quint64 ui64StatusID, TLxTsspDeviceState& status)
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
int SS2RSPDcuGUI::WriteData(quint64 ui64DataID, uchar* aData, uint uiDataLen)
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
int SS2RSPDcuGUI::ReadData(quint64 ui64DataID, uchar* aData, uint& uiDataLen,
                             uint& uiReloadCount)
{
    return 0;
}

int SS2RSPDcuGUI::CreateDefaultMacroObj(ILxTsspParameterMacro* pMacroObj)
{
    /*RCLOCKDCUStatu tmpComPara;          //参数
    //时间区域
    tmpComPara.sTimeZone = m_ParaSetStruct[0][0].InitVal.ival;
    //参考有效状态
    tmpComPara.ucReferentInvalid = m_ParaSetStruct[0][1].InitVal.ival;
    //参考选择状态
    tmpComPara.ucReferentSel = m_ParaSetStruct[0][2].InitVal.ival;
    //同步状态
    tmpComPara.ucSyncStatus = m_ParaSetStruct[0][3].InitVal.ival;
    //铷钟锁定状态
    tmpComPara.ucRCLOCK = m_ParaSetStruct[0][4].InitVal.ival;
    //解调秒状态
    tmpComPara.ucDemodulateSec = m_ParaSetStruct[0][5].InitVal.ival;
    //守时状态
    tmpComPara.ucPunctual = m_ParaSetStruct[0][6].InitVal.ival;
    //告警状态
    tmpComPara.ucWarning = m_ParaSetStruct[0][7].InitVal.ival;
    //电源状态
    tmpComPara.ucPower = m_ParaSetStruct[0][8].InitVal.ival;
    //分本控状态
    tmpComPara.ucConMode = m_ParaSetStruct[0][9].InitVal.ival;
    //驯控状态
    tmpComPara.ucTameCtrl = m_ParaSetStruct[0][10].InitVal.ival;
    //同步方式
    tmpComPara.ucSyncCtrl = m_ParaSetStruct[0][11].InitVal.ival;
    //取当前设置的参数，存入框架
    QByteArray byteArray;
    byteArray.resize(sizeof(RCLOCKDCUStatu));
    memcpy(byteArray.data(), &tmpComPara, sizeof(RCLOCKDCUStatu));
    if(pMacroObj != NULL)
    {
        pMacroObj->UpdateParameterBlock(m_ParaComSynID, byteArray);
    }*/
    return 1;
}

QWidget* SS2RSPDcuGUI::ShowMeInEdit(QWidget* parent,uchar ucType,ILxTsspParameterMacro* pMacroObj)
{
    g_form = new SS2RSPDcuGUIDlg(parent);

    g_form->SetObjInfo(m_ObjectInfo);
    g_form->setText(m_strTitle, m_ParaSetStruct, m_MapMean, MapLineNum, ucType, pMacroObj);
    g_form->setAttribute(Qt::WA_DeleteOnClose);

    return g_form;
}

int SS2RSPDcuGUI::SaveToMacroObj(ILxTsspParameterMacro* pMacroObj)
{
    if(g_form != NULL)
    {
        g_form->SaveToMacro(pMacroObj);
    }
    return 1;
}
