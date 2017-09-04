/*** 更改说明
 *
 *
***/

#ifndef LXTSSPCOMMONDEFINITION_H
#define LXTSSPCOMMONDEFINITION_H

#include <QtCore/qglobal.h>
#include <QDomElement>
#include <QMap>
#include <QString>
#include <QList>
#include <QLibrary>

#ifndef LXTSSPOBJECTMANAGER_GLOBAL_H
#define LXTSSPOBJECTMANAGER_GLOBAL_H

#if defined(LXTSSPOBJECTMANAGER_GLOBAL_H)
#  define LXTSSPOBJECTMANAGERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LXTSSPOBJECTMANAGERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LXTSSPOBJECTMANAGER_GLOBAL_H

#define DECLARE_ILXTSSPOBJECT \
    public:\
    virtual int GetModuleInfo(TLxTsspModuleInfo& ModuleInfo) const;\
    virtual int GetObjectInfo(TLxTsspObjectInfo& ObjectInfo) const;\
    virtual int PreInitialize();\
    virtual int Initialize();\
    virtual int Start();\
    virtual int Stop();\
    virtual bool ClearUp();\
    virtual int GetErrorNumber();\
    virtual int GetErrorMsg(int iErrorNo, QString& strErrorMsg);\
    virtual bool IsOnline();\
    virtual void SendMessage(TLxTsspMessage msg);\
    virtual int SetParameter(QMap<quint64, QByteArray> parameterList);\
    virtual int SetParameter(quint64 ui64ParamID, QByteArray parameter);\
    virtual int GetParameterID(QList<quint64>& pParameterIDList) const;\
    virtual int GetParameter(QMap<quint64, QByteArray>& parameterList) const;\
    virtual int GetParameter(quint64 ui64ParamID, QByteArray& parameter) const;\
    virtual int GetState(QMap<quint64, TLxTsspDeviceState>& stateList) const;\
    virtual int GetState(quint64 ui64StatusID, TLxTsspDeviceState& status) const;\
    virtual int GetStateDirect(quint64 ui64StatusID, TLxTsspDeviceState& status);\
    virtual int WriteData(quint64 ui64DataID, uchar* aData, uint uiDataLen);\
    virtual int ReadData(quint64 ui64DataID, uchar* aData, uint& uiDataLen,\
    uint& uiReloadCount);


#pragma pack(1)

/**
 * 模块信息
 */
typedef struct ModuleInfoStruct
{
    /// 公司名称,固定为“中国航天科技集团九院七〇四所”
    char szCompany[64];
    /// 模块名称，如：系统工作参数设置
    char szModuleName[32];
    /// 配置标识，如：D_Y17-61_008
    char szConfigID[32];
    /// 版本
    char szVersion[16];
    /// 开发者，如：大卫.奥巴马
    char szCreator[24];
    /// 备注
    char szComment[64];

    ModuleInfoStruct()
    {
        strcpy(szCompany, "中国航天科技集团九院七〇四所");
    }

}TLxTsspModuleInfo;

/**
 * 对象信息
 */
typedef struct ObjectInfo
{
    /// 配置标识。如：D_Y17-61_008
    char szConfigID[32];
    /// 设备号（站号）
    unsigned short usStationID;
    /// 分系统号
    unsigned char ucSubSysID;
    /// 单机号
    unsigned short usLocalDID;
    /// 对象号。如：5002H
    unsigned short usObjectID;
    /// 相同对象的序号
    unsigned char ucSN;
    /// 对象别名
    char szAlias[64];
    /// 备注
    char szComment[64];
    /// 是否本地对象
    bool bLocalObj;
    /// 是否主用
    bool bMainUse;

}TLxTsspObjectInfo;

/**
 * 消息头部
 */
typedef struct MessageHeader
{
    ///为应用标识号。
    ///  发送控制命令时（T=01H），由发送方负责填写；
    ///  发送控制命令的应答时（T=02H），由命令应答方从收到的控制命令帧中取出并进行回填；
    ///  发送状态时（T=04H），由发送状态方负责填写；不用时保留填0。
    unsigned int uiHandle;

    ///为信息报文帧序号，它是动态唯一标识某个信息报文帧的号，
    ///  发送控制命令时（T=01H），由发送方负责填写；
    ///  发送控制命令的应答时（T=02H），从收到的控制命令帧中取出并进行回填；
    ///  发送状态时（T=04H），由发送状态方负责填写；
    ///  在某个应用过程中，SEQ从0开始每发送一帧状态信息加1，溢出归0。
    unsigned int uiSequence;

    ///为任务代号，其具体值根据任务而定，不用时保留填0。
    unsigned char M;

    ///信息报文类型码
    ///T    =01H
    ///  表示控制信息报文。接收到这类信息报文的应用程序将它当作命令对待，
    ///  按命令的具体格式去解释其中的数据，按命令的具体含义去执行，并给对方回送对应的响应信息报文。
    ///T    =02H
    ///  表示响应信息报文。接收到这类信息报文的应用程序将它当作相应命令的响应对待，
    ///  按命令响应的具体格式去解释其中的数据，并根据响应的具体结果决定作何处理。
    ///T    =04H
    ///  状态信息报文。接收到这类信息报文的应用程序将它当作状态对待，
    ///  按状态的具体格式去解释其中的数据，并根据状态的具体结果决定作何处理。接收方对状态信息报文不进行应答。
    unsigned char T;

    ///为整个信息报文帧的长度，
    ///信息报文首部（背景灰色）和报文数据区两部分的总长度，单位为字节，最大为32K字节。
    ///由此用户所发送的每个信息报文长度不能超过32K字节。
    ///接收方传输服务软件在完成一个信息报文的接收组装后，
    ///可用信息报文长度与实际接收到的报文长度来检查信息报文接收的正确性。
    unsigned short L;

    ///信息报文请求的传输服务有限级别
    ///b7~b4 初始版本（0000）
    ///b3~b0 优先级别
    ///  0000：  普通
    ///  0001：  实时
    ///  0010：  紧急处理
    ///  0011：  空闲
    unsigned char P;

    ///代码，提供信息报文类型的进一步信息，它与信息报文类型字段共同标识某一具体的信息报文
    unsigned short C;

    ///附加字。通常保留不用，如果使用往往用来对信息报文作附加说明或用作代码扩充字。
    unsigned char A;

    ///信源，发送方设备号，表明本协议数据来源的设备。
    unsigned short S;

    /// 信宿，接收方设备号，表明本协议数据送往的设备。
    /// D 〉0：D为目的设备号。报文数据区DATA中的格式内容依据报文的类型自行约定使用，
    ///       可以包括子包头，也可以直接定义数据内容。
    /// D = 0：报文数据区DATA 中包含多个（最少一个）目的设备的数据，
    ///       DATA中数据子包的包头再指出具体的设备（命令管理中心没有处理此种情况）。
    /// D = 0xFFFF：为报文数据广播，对所有的设备都发送。
    unsigned short D;

    ///保留字节。目前填0，供以后扩展使用。
    unsigned short R;

}TLxTsspMessageHeader;

/**
 * 子包数据报头部
 */
typedef struct SubMsgHeader
{
    /// SID: 为源分系统编号。表明本数据子包数据来源的分系统。
    unsigned short SID;
    /// DID: 为目的分系统编号。表明本数据子包数据送往的分系统。
    unsigned short DID;
    /// TID：为命令类型。针对设备板的命令（如参数加载），为设备板类型。
    unsigned short TID;
    /// SN：为同类命令的序号（从0开始）。
    unsigned short SN;
    /// O：为操作字由设备驱动接口使用，O =0表示对设备进行实际操作，O =1表示不操作。
    unsigned short O;
    /// Len: 为Dat数据所占的字节数。
    unsigned short Len;

}TLxTsspSubMsgHeader;

/**
 *消息结构
 */
typedef struct Message
{
    int iDataLen;           /// 消息长度，字节
    char* pData;            /// 消息

}TLxTsspMessage;

/**
 * 设备状态
 */
typedef struct DeviceState
{
    /// 更新计数
    unsigned int uiUpdateCount;
    /// 状态标识。
    /// 站号(2)/分系统号(1)/单机号(2)/数据块标识(2)/序号(1)。
    /// 站号(2)/分系统号(1)/单机号(2)不用时填0。
    unsigned long long ui64StatusID;
    /// 设备状态有效数据长度
    int iValidDataLen;
    /// 设备状态
    unsigned char aStatus[1024];

}TLxTsspDeviceState;

/**
 * 用户信息
 */
typedef struct UserInformation
{
    /// 用户名
    char szName[16];
    /// 密码
    char szPassword[16];
    /// 所属角色，用户权限，分为0:管理员、1:操作员和2:监视员，用户登录后运行时的实际权限。
    int iRole;
    /// 用户创建时间；计算机时间
    time_t tCreateTime;
    /// 是否本地用户，用户按照本地用户和远程用户分组管理。
    bool bLocalUser;
    /// 备注
    char szContent[32];
    /// 用户登录时使用的IP地址
    char szIP[32];
    /// 用户上次登录时间，采用计算机时。当用户退出时，更新用户信息，表示用户上次登录时间。
    time_t oldLoginTime;
    /// 用户本次登录时间，采用计算机时。当用户退出时，更新到上次登录时间。
    time_t newLoginTime;
    /// 未连接计数。已登录用户管理定时（如1秒）将此计数加1，用户定时（如1秒）将其清零。
    /// 如果该值超过某个值（如5秒），表示连接已断开或用户退出的消息未传达，则删除该用户。
    unsigned int uiDisconnectCount;
    ///管理的设备，用设备ID表示。当用户退出时，更新用户信息，表示用户上次管理的设备。
    unsigned int uiDeviceID[32];
    ///对管理的设备的权限。每套设备只有一个操作权限的用户，对于不同设备，用户的权限是不同的。
    unsigned int uiDevMgrRole[32];

}TLxTsspUserInformation;

/**
 * 参数宏
 */
typedef struct ParamMacroInformation
{
    /// 宏代号
    unsigned int uiID;
    /// 宏名称
    char szName[32];
    /// 宏描述
    char szDescription[128];
    /// 系统工作模式，下面是测通所标准化规范中的定义
    /// 下行：Byte0：b0：S频段标准TT&C；b1：S频段扩频模式一；b2：S频段扩频模式二；
    ///             b3：保留； b4：S频段调频遥测；b5：S频段数传；b6：C频段标准TT&C；
    ///             b7：C频段扩频模式一；
    ///      Byte1：b8：C频段扩频模式二；b9：X/Ka频段标准TT&C；b10：X/Ka频段扩频模式一；
    ///             b11：X/Ka频段扩频模式二； b12：保留；b13：X/Ka频段低速数传；
    ///             b14：X/Ka频段高速数传；b7：保留；
    ///      Byte2：保留；
    ///      Byte3：保留；
    /// 上行：Byte4：b18：S频段标准TT&C；b19：S频段扩频模式一；b20：S频段扩频模式二；
    ///             b21：保留； b22：S保留；b23：S频段数传；b24：C频段标准TT&C；
    ///             b25：C频段扩频模式一；
    ///      Byte5：b26：C频段扩频模式二；b27：X/Ka频段标准TT&C；
    ///             b28：X/Ka频段扩频模式一；b29：X/Ka频段扩频模式二；b30：保留；
    ///             b31：X/Ka频段低速数传；b32：保留；b33：保留；
    ///      Byte6：保留；
    ///      Byte7：保留；
    /// 按照上面的定义:
    ///      标准TT&C+数传模式为0x0000002100000021。
    ///      扩频模式一为0x0000000200000002；
    ///      扩频模式二为0x0000000400000004；
    qulonglong ulSysWorkMode;
    /// 二级工作模式。
    /// 当前指在标准TT&C+数传模式下标准TT&C、数传模式和标准TT&C+数传模式三种模式
    /// 定义分别为1、2、3。
    unsigned int uiSysWorkMode2;
    /// 任务代号。使用6个字节表示
    /// 分别对应目标1、2、3。
    /// 界面显示时，格式为：目标1/目标2/目标3；单目标时只显示：目标1。
    char aTaskID[8][7];
    /// 任务标识
    /// 分别对应目标1、2、3。
    /// 界面显示时，格式为：目标1/目标2/目标3；单目标时只显示：目标1。
    unsigned int aTaskMark[8];
    /// 目标个数
    unsigned char ucTargetSum;
    /// 是否同波束，0：否，1：是。
    unsigned char ucSameWave;
    /// 上行频率，单位：Hz，对应多个目标
    /// 界面显示时，格式为：目标1/目标2/目标3；单目标时只显示：目标1。
    qulonglong aUpFrq[8];
    /// 下行频率，单位：Hz，对应多个目标
    /// 界面显示时，格式为：目标1/目标2/目标3；单目标时只显示：目标1。
    qulonglong aDownFrq[8];

    /// 设备模式选择下标数组,最多支持256个设备
    unsigned int aDevModeIdx[256];
}TLxTsspParamMacroInformation;

/**
 * 参数结构，包括了参数属性
 */
typedef struct Parameter
{
    /// ID
    unsigned short usID;
    /// 名称
    char szName[32];
    /// 类型
    char szType[16];
    /// 系数
    float fFactor;
    /// 单位
    char szUnit[16];
    /// 最大值
    double lfMax;
    /// 最小值
    double lfMin;
    /// 参数值
    char szValue[16];
    /// 映射---值
    char szMapValue[16][16];
    /// 映射---含义
    char *szMapMeaning[16][32];

}TLxTsspParameter;

/**
 * 设备配置宏信息定义
 */
typedef struct DevConfigMacroInformation
{
    /// 代号
    ulong ulID;
    /// 名称
    char szName[32];
    /// 描述
    char szDescription[128];

}TLxTsspDevConfigMacroInformation;

/**
 * 设备信息
 */
typedef struct DeviceInformation
{
    /// 标识。2个字节。其中高字节标识设备类型，低字节标识该类型设备序号。
    /// 如0x0400表示基带设备，则0x0401表示基带1；0x0402表示基带2；
    unsigned int usID;
    /// 名称
    char szName[32];
    /// 描述
    char szDescription[128];
    /// 所属分系统
    unsigned char ucSubSys;
    /// 所属分系统序号
    unsigned char ucSubSysSN;
    /// 主用还是备用。1：主用；2：备用
    unsigned char ucMain;
    /// 对象名称
    char szObjectName[128];

}TLxTsspDeviceInformation;

/// 通道类型: 0-未知，还没有创建，1-UDP组播；2-UDP点对点；
///          3-TCP服务端；4-TCP客户端；5-HDLC；6-串口
enum ChannelType {UNKNOW, UDP_GROUP, UDP_P2P, TCP_SERVER,
                  TCP_CLIENT, HDLC, SERIAL};
/**
 * 通道接口属性
 */
//// 20150625(raod) 增加参数 ReadWrite, aV3TargetIP, ucTargetNum
typedef struct ChannelAttribute
{
    /// 名称
    char szName[32];
    /// 通道类型: 0-未知，还没有创建，1-UDP组播；11-指定源组播v3； 2-UDP点对点；
    ///          3-TCP服务端；4-TCP客户端；5-HDLC；6-串口
    int ChannelType;
    /// 通道读写属性: 1-读；2-写；3-读写同时
    int ReadWrite;
    /// 描述
    char szDescription[128];
    /// 本地IP
    unsigned char aLocalIP[16];
    /// 本地端口
    int iLocalPort;
    /// 远程IP
    unsigned char aRemoteIP[16];
    /// 远程端口
    int iRemotePort;
    /// 指定组播源IP（uchar TargetIP[32]的倍数）
    uchar* aV3TargetIP;
    /// 指定源个数。
    uchar ucTargetNum;

}TLxTsspChannelAttribute;

/**
 * 日志定义
 */
typedef struct LogStruct
{
    /// 日志记录时的时间，由日志管理模块统一填写，采用计算机时间还是时统时间？
    time_t time;
    /// 登录用户的名称
    char szUserName[16];
    /// 日志来源，使用设备号表示
    uint uiSource;
    /// 对象标识；对象管理器使用的标识
    char szObjectID[128];
    /// 日志生成者类别。平台对象生成的日志信息可以不对用户展示。
    /// 平台对象(1)、应用对象(2)、其他待定
    unsigned char ucSrcType;
    /// 下面3项可由具体项目定义
    /// 警告级别
    /// 提示(1)、告警(2)、错误(3)
    unsigned char ucWarningLevel;
    /// 主类型
    /// 分为用户操控(1)和设备工作(2)
    unsigned char ucMainType;
    /// 子类型
    /// 用户操控的子类型分为：0x001--界面操作；0x002--用户管理；
    /// 设备工作的子类型分为：0x001--设备状态；0x002--内部接口；0x003--外部部接口；
    unsigned char ucSubType;
    /// 日志内容
    char szContent[256];

}TLxTsspLog;

/// 模式
typedef struct WorkModeInfoStruct
{
    /// 模式ID
    char szID[16];
    /// 模式名称，对用户显示的模式友好名称
    char szName[64];
    /// 模式描述，描述模式的信息，仅用户显示。
    char szDescription[256];

}TLxTsspWorkModeInfoStruct;

//数据缓冲区数据头结构定义
typedef struct DataBufferProperty
{
    union
    {
        struct
        {
            //属性头长度
            short      sGIO_Length;
            //数据区总长度
            int       iLengthOfDataBuffer;
            //	描述信息
            char        szStreamDescription[50];
            //缓冲区最大帧（块）数
            int       iLimitOfBuffer;
            //缓冲区头指针（按帧（块）记数）
            int       iHeadOfBuffer;
            //每块的长度（按字节记数）
            int       iLenOfPerBlock;
            //缓冲区格式变化记数
            int       iNumOfBuffFormatChange;
            //73～80  起飞零点(To)                  8字节         二进制
            uchar       aZeroTime[8];
            //第一帧数据的时间
            //  年             4字节
            ulong       ulYear;
            //  月             2字节
            ushort      usMonth;
            //  日             2字节
            ushort      usDay;
            //  时             2字节
            ushort      usHour;
            //  分             2字节
            ushort      usMiniter;
            // 秒             2字节
            ushort      usSecond;
            //子帧长               2字节
            ushort      usFrameLength;
            //副帧长               2字节
            ushort  	usSubframeLength;
            //99             字  长               1字节
            uchar		ucWordLength;
            //100～103        码  率               4字节
            ulong		ulBaudRate;
            //104             同步码长             1字节
            uchar       ucLengthOfFrameSynCode;
            //105～108        同步码组             4字节
            uchar       aFrameSynCode[4];
            //副帧个数             1字节
            uchar       ucCountOfSubFrame;
            //副帧方式  1字节,00=反码,01=ID码,02=循环码,03=无副帧
            uchar       ucPaternOfSubFrame;
            //副帧同步码长          1字节      二进制
            uchar       ucLengthOfSubFrameSynCode;
            //副帧同步码组          4字节      二进制
            uchar       aSubFrameSynCode[4];
            //ID副帧初值            1字节
            uchar       ucStartIndexOfSubFrame;
            //ID副帧终值            1字节
            uchar       ucEndingIndexOfSubFrame;
            //ID副帧位置            2字节
            ushort      usLocationOfIdInFrame;
            //循环副帧位置           2字节
            ushort      usLocationOfCycleSubFrame;
            //	122，123       副帧1位置             2字节
            //	124，125       副帧2位置             2字节
            //				   ..
            //	184，185       副帧32位置           2字节
            ushort      usLocationOfSubFrame[32];
            // 加载计数
            ulong		ulReloadCounts;
            // 当前累计子帧数(子帧缓冲区)或全帧数(全帧信息元)
            uint uiFrameCounts;
            // 子帧同步码组位置，00H表示帧尾；01H表示帧首。
            // 在遥测解调参数设置中00表示在帧头，01表示在帧尾；
            uchar ucFspPlace;
            // 子帧同步码的高32位，此项为新加
            uint uiHighFsp;

        } Property;

        unsigned char Reserved[1024];

    };

} DataBufferPropertyStruct, *PDataBufferPropertyStruct;

#pragma pack()

typedef void (*UpdateProcessHandler)(unsigned char* pInfo, int& iInfoLen);

/// <summary>
/// 自动关机接口
/// </summary>
class ILxTsspAutoShutDown
{
public:
    virtual ~ILxTsspAutoShutDown() { }

public:
    /// <summary>
    /// 关闭计算机
    /// </summary>
    /// <returns>true表示成功，false表示失败</returns>
    virtual bool ShutDownComputer() = 0;

    /// <summary>
    /// 重新启动计算机
    /// </summary>
    /// <returns>true表示成功，false表示失败</returns>
    virtual bool RebootComputer() = 0;

};


/**
 * 所有对象都需要实现该接口。
 */
class ILxTsspObject
{
public:
    virtual ~ILxTsspObject() {}

public:
    /*--------------对象信息接口部分-------------------------------*/
    /**
     * 获取对象所属的模块信息：信息包括模块的公司名称、模块名称、配置标识、版本、开发人员等
     * ，用于实时运行时查看。
     * @param[out] ModuleInfo：模块信息。
     * @return 1：成功；0：未定义；-1：失败。
    */
    virtual int GetModuleInfo(TLxTsspModuleInfo& ModuleInfo) const = 0;

    /**
     * 获取对象相关信息。对象相关信息包括对象名称，对象的分系统号、设备号、流号，
     * 本地对象还是远程对象，主用还是备用等。
     * @param[out] ObjInfo：对象信息。
     * @return 1：成功；0：未定义；-1：失败。
     */
    virtual int GetObjectInfo(TLxTsspObjectInfo& ObjectInfo) const = 0;

    /*--------------对象控制接口部分-------------------------------*/
    /**
     * 预初始化：模块在此函数中执行必要的预初始化工作，如查找需要的对象，
     *  向对象管理器添加私有对象等。主用工作是准备对象运行的外界资源条件。
     * @return 1：成功；0：未定义；-1：失败。
     */
    virtual int PreInitialize() = 0;

    /**
     * 初始化 ：模块在此函数中执行必要的初始化工作。
     * @return 1：成功；0：未定义；-1：失败。
     */
    virtual int Initialize() = 0;

    /**
     * 启动：模块在此函数中启动功能的执行，尤其是包含线程的模块，应该在此启动线程；
     * @return 1：成功；0：未定义；-1：失败。
     */
    virtual int Start() = 0;

    /**
     * 停止 ：模块在此函数中停止功能的执行，尤其是包含线程的模块，应该在此结束线程。
     * @return 1：成功；0：未定义；-1：失败。
     */
    virtual int Stop() = 0;

    /**
     * 清理：模块在此函数中清理申请的资源，为进程退出做准备。
     * @return 1：成功；0：未定义；-1：失败。
     */
    virtual bool ClearUp() = 0;

    /*--------------对象故障诊断接口部分----------------------------*/
    /**
     * 取错误码： 取对象的错误码。
     * @return： 返回模块的错误码，用于判断模块不能执行的原因:
     *           0表示运行正常；-1表示没有配置信息；-2表示初始化失败；
     *           -3表示启动操作失败；-4表示停止操作失败；-5表示清理资源失败；
     *           其它错误依情况增加；
     */
    virtual int GetErrorNumber() = 0;

    /**
     * 取错误信息
     * @param iErrorNum：错误码。
     * @param[out] szErrorMsg：错误信息，描述性的错误信息，用于判断对象不能执行的原因。
     * @return 1：成功；0：未定义；-1：表示没有对应的错误信息；
     */
    virtual int GetErrorMsg(int iErrorNo, QString& strErrorMsg) = 0;

    /**
     * 是否在线 ：对于远程对象可能存在不在线情况，普通模块直接返回true即可。
     *          对于代理模块需要判断远程实际对象响应情况、通道通断情况来判断。
     * @return true：在线；false：不在线。
     */
    virtual bool IsOnline() = 0;

    /**
     * 发送消息 ：通过此函数可实现信息的直接发送，对于对象管理器，当此函数被调用时，
     *          向管理的所有对象发送此消息。
     * @param msg：消息。对象收到消息后，可以拷贝消息内容，但不应更改。
     */
    virtual void SendMessage(TLxTsspMessage msg) = 0;

    /**
     * 设置组参数 ：通过此函数把参数设置给对象，设备对象加载到具体设备；
     * @param parameterList：参数块列表,
     * quint64参数块标识，格式为：
     * 站号(2)/分系统号(1)/单机号(2)/数据块标识(2)/序号(1)。
     * 站号(2)/分系统号(1)/单机号(2)不用时填0。
     * @return 1：成功；-1：失败。
     */
    virtual int SetParameter(QMap<quint64, QByteArray> parameterList) = 0;

    /**
     * 设置参数 ：设置单个参数。
     * @param ui64ParamID：参数ID,格式为：
     * 站号(2)/分系统号(1)/单机号(2)/数据块标识(2)/序号(1)。
     * 站号(2)/分系统号(1)/单机号(2)不用时填0。
     * @param parameter：参数块数据
     * @return 1：成功；0：非本对象参数；-1：失败。
     */
    virtual int SetParameter(quint64 ui64ParamID, QByteArray parameter) = 0;

    /**
     * 取对象的参数ID ：取本对象需要的参数块的ID。在参数宏加载时，
     *      调用此函数确定对象需要的参数块，然后从参数宏中取出相应的参数，设置给该对象。
     * @param[out] pParameteIDList：参数块ID的列表。参数ID,格式为：
     * 站号(2)/分系统号(1)/单机号(2)/数据块标识(2)/序号(1)。
     * 站号(2)/分系统号(1)/单机号(2)不用时填0。
     * @return 大于等于0：参数块个数。-1：失败。
     */
    virtual int GetParameterID(QList<quint64>& pParameterIDList) const = 0;

    /**
     * 取参数 ：获取本对象的参数，可以一次读取所有参数。
     * @param[out] parameterList：参数块ID和参数值的键值对的列表。参数ID,格式为：
     * 站号(2)/分系统号(1)/单机号(2)/数据块标识(2)/序号(1)。
     * 站号(2)/分系统号(1)/单机号(2)不用时填0。
     * @return 大于等于0：参数块个数。-1：失败。
     */
    virtual int GetParameter(QMap<quint64, QByteArray>& parameterList) const = 0;

    /**
     * 取参数 ：获取本对象的参数，按照指定的ID读取。
     * @param ui64ParamID：参数块ID。格式为：
     * 站号(2)/分系统号(1)/单机号(2)/数据块标识(2)/序号(1)。
     * 站号(2)/分系统号(1)/单机号(2)不用时填0。
     * @param[out] parameter：参数块的值；
     * @return 1：成功；-1：失败。
     */
    virtual int GetParameter(quint64 ui64ParamID, QByteArray& parameter) const = 0;

    /**
     * 获取状态 ：获取本对象的状态，可以一次读取所有状态。
     * @param[out] stateList：状态块ID和状态值的键值对的列表。quint64为状态号，格式为：
     * 站号(2)/分系统号(1)/单机号(2)/数据块标识(2)/序号(1)。
     * 站号(2)/分系统号(1)/单机号(2)不用时填0。
     * 状态块标识(2)/序号(1)为必填项，其它根据需要填写。
     * @return 大于等于0：状态块个数。-1：失败。
     */
    virtual int GetState(QMap<quint64, TLxTsspDeviceState>& stateList) const = 0;

    /**
     * 取单个状态 ：根据状态ID取相应的状态，注意如果参数不加载到硬件，也要作为状态返回。
     * @param ui64StatusID:状态块ID。格式为：
     * 站号(2)/分系统号(1)/单机号(2)/数据块标识(2)/序号(1)。
     * 站号(2)/分系统号(1)/单机号(2)不用时填0。
     * @param[out] status：状态值。
     * @return 1：成功；-1：失败。
     */
    virtual int GetState(quint64 ui64StatusID, TLxTsspDeviceState& status) const = 0;

    /**
     * 直接取状态 ：直接从硬件读取状态，不取当前缓存的状态，强调实时性时使用。
     * @param ui64StatusID:状态块ID。格式为：
     * 站号(2)/分系统号(1)/单机号(2)/数据块标识(2)/序号(1)。
     * 站号(2)/分系统号(1)/单机号(2)不用时填0。
     * @param[out] status：状态值。
     * @return 1：成功；-1：失败。
     */
    virtual int GetStateDirect(quint64 ui64StatusID, TLxTsspDeviceState& status) = 0;

    /**
     * 向对象写数据 ：向对象写数据
     * @param ui64DataID：数据ID；格式如下：
     * 站号(2)/分系统号(1)/单机号(2)/数据块标识(2)/序号(1)。
     * 站号(2)/分系统号(1)/单机号(2)不用时填0。
     * @param uiDataSn：数据流号；
     * @param aData：数据缓冲区；
     * @param uiDataLen：数据字节长度。
     * @return 大于等于0：实际写入的数据长度；-1：失败。
     */
    virtual int WriteData(quint64 ui64DataID, uchar* aData, uint uiDataLen) = 0;

    /**
     * @brief ReadData从模块读取数据 ：从模块读取数据
     * @param ui64DataID,格式如下：
     * 站号(2)/分系统号(1)/单机号(2)/数据块标识(2)/序号(1)。
     * 站号(2)/分系统号(1)/单机号(2)不用时填0。
     * @param aData：数据缓冲区；
     * @param uiDataLen：数据缓冲区字节长度。
     * @param uiReloadCount：加载计数，计数变化意味着状态的改变。
     * @return  大于等于0：实际读取的数据长度；-1：失败。
     */
    virtual int ReadData(quint64 ui64DataID, uchar* aData, uint& uiDataLen,
                         uint& uiReloadCount) = 0;

}; // ILxTsspObject

/**
 * 用户管理
 */
class ILxTsspUserManager : public ILxTsspObject
{

public:
    /**
     * 取所有用户的信息 ：需要操作者的用户名及密码，用于判断其权限，
     *                 只有管理员权限的用户才可读取。
     * @param szUserName：调用者用户名；
     * @param szPassword：调用者密码；
     * @param[out] userList：用户信息列表；
     * @return 取到的用户个数，-1表示操作失败。
     */
    virtual int GetAllUsers(char* szUserName, char* szPassword, QList<TLxTsspUserInformation>& userList) = 0;

    /**
     * 保存用户信息 ：需要操作者的用户名及密码，用于判断权限，只有管理员权限的用户才能保存。
     * @param szUserName：调用者用户名；
     * @param szPassword：调用者密码；
     * @param userList：要保存的用户信息列表。
     * @return 返回保存的用户个数，-1表示操作失败
     */
    virtual int SaveUserInfo(char* szUserName, char* szPassword, QList<TLxTsspUserInformation> userList) = 0;

    /**
     * 更新一个用户的密码 ：任何权限的用户都可以更新自己的密码。
     * @param szUserName：调用者用户名；
     * @param szOldPassword：调用者原密码；
     * @param szNewPassword：新密码；
     * @return 返回值true表示成功，false表示更新失败，可能是用户名/密码不正确或操作失败。
     */
    virtual bool UpdateAUserPassword(char* szUserName, char* szOldPassword, char* szNewPassword) = 0;

    /**
     * 更新一个用户的信息 ：管理员权限可以操作。需要操作者的用户名及密码，用于判断其权限，
     *                  只有管理员权限的用户才能操作。
     * @param szUserName：调用者用户名；
     * @param szPassword：调用者密码；
     * @param szUpdateUserName：被更新的用户的名称；
     * @param newInfo：新的用户信息；
     * @return 返回1表示成功；-1表示不具有管理员权限；
     *          -2表示和已有用户名重复；-3表示原用户不存在。
     */
    virtual int UpdateAUserInformation(char* szUserName, char* szPassword, const TLxTsspUserInformation newInfo) = 0;

    /**
     * 添加一个用户 ：管理员权限可以操作。需要操作者的用户名及密码，用于判断其权限，
     *              只有管理员权限的用户才能操作。
     * @param szUserName：调用者用户名；
     * @param szPassword：调用者密码；
     * @param newInfo：新的用户信息；
     * @return 返回值1表示成功；-1表示不具有管理员权限；-2表示和已有用户名重复；
     *          -3表示参数错误。
     */
    virtual int AddAUserInformation(char* szUserName, char* szPassword, const TLxTsspUserInformation newInfo) = 0;

    /**
     * 删除一个用户 ：管理员权限可以操作。需要操作者的用户名及密码，用于判断其权限，
     *              只有管理员权限的用户才能操作
     * @param szUserName：调用者用户名；
     * @param szPassword：调用者密码；
     * @param szDelUserName：被删除的用户的名称；
     * @return 返回值1表示成功；-1表示不具有管理员权限；-2用户名不存在；-3表示参数错误。
     */
    virtual int DeleteAUserInformation(char* szUserName, char* szPassword, char* szDelUserName) = 0;

    /**
     * 取用户信息 ：进行用户名和密码的比对，正确则输出相应的用户的信息，错误输出空。
     *              没有此用户或用户名和密码比对错误则输出空。
     * @param szUserName：用户名；
     * @param szPassword：密码；
     * @param[out] userInfo：用户信息，含用户权限等信息。
     * @return 返回值1表示成功；-1表示待定；-2表示用户名不存在；
     */
    virtual int GetUserInfo(char* szUserName, char* szPassword, TLxTsspUserInformation& newInfo) = 0;


//////////////////////

    /**
     * 用户登录 ：通过用户信息管理读取用户信息，判断是否是合法用户，
     *          如果是合法用户则更新用户信息中的IP地址、本次登录时间，返回给调用者。
     *          当用户登录成功后，将用户信息添加到“已登录用户”列表.
     * @param szUserName：用户名；
     * @param szPassword：用户密码；
     * @param szIP：用户使用的IP地址。
     * @param uiLoginType: 登录方式，1--允许降级登录；2--强制登录；3--按照要求权限登录；
     * @param[out] UserInfo：用户信息，返回值为1时有效。
     * @return 1：按照要求权限登录成功；2: 降级登录成功; 3: 强制登录成功;
     * -1：参数错误;-2: 密码错误;-3:用户名错误; -4:没有配置IP，无法确定是本地用户还是远程用户。
     */
    virtual int Login(char* szUserName, char* szPassword, char* szIP, uint uiLoginType,
                      TLxTsspUserInformation& newInfo) = 0;

    /**
     * 用户退出。
     * @param szUserName：用户名；
     * @param szIP：用户使用的IP地址。
     * @return 成功；false：失败。失败的情况一般不需要处理。
     */
    virtual bool Logout(char* szUserName) = 0;

    /**
     * 取用户权限 ：取用户权限，一般用于判断操作用户是否有相应的权限。
     *              这里用户权限指在登录用户中的权限，不是实际权限。
     * @param szUserName：用户名。
     * @return 0：管理员；1：操作员；2：监视员。-1：无此用户。
     */
    virtual int GetUserRole(char* szUserName) = 0;

    /**
     * 取登录用户信息 ：取已登录用户的信息。
     * @param[out] userList：已登录的用户信息的列表
     * @return 大于等于0：已登录的用户的个数，-1表示操作失败
     */
    virtual int GetLoginUsersInformation(QList<TLxTsspUserInformation>& userList) = 0;

    /**
     * @brief GetLoginUsersInformation 取已登录的具备操作员及以上权限的用户信息
     * @param userInfo 用户信息
     * @return 1:成功；0:无用户; -1：失败
     */
    virtual int GetLoginedControlUserInformation(TLxTsspUserInformation& userInfo) = 0;

}; // IUserInfoManager

/**
 * 参数宏接口
 */
class ILxTsspParameterMacro
{
public:
    virtual ~ILxTsspParameterMacro() {}
    /**
     * 创建参数宏 ：按照给定的模板创建参数宏。
     * @param macroTemplate：参数宏的模板。
     * @return 1：成功；-1：失败。
     */
  //  virtual int CreateParamMacro(const QDomNode macroTemplate) = 0;
    virtual int CreateParamMacro(const TLxTsspParamMacroInformation& macroInfo) = 0;
    /**
     * 取宏信息 ： 获取参数宏的相关信息。
     * @param[out] macroInfo：宏信息。
     * @return 1：成功；-1：失败。
     */
    virtual int GetMacroInformation(TLxTsspParamMacroInformation& macroInfo) const = 0;

    /**
     * 设置宏信息 ：设置参数宏的相关信息。
     * @param macroInfo：宏信息。
     * @return 1：成功；-1：失败。
     */
    virtual int SetMacroInformation(const TLxTsspParamMacroInformation macroInfo) = 0;

    /**
     * 更新参数块 ：将参数块保存到当前宏中，如果参数块不存在则添加；
     * @param paramBlkID：参数块标识，格式为：
     * 站号(2)/分系统号(1)/单机号(2)/数据块标识(2)/序号(1)。
     * 站号(2)/分系统号(1)/单机号(2)不用时填0。
     * @param parameterBlock：参数块数据。
     * @return 1：成功；0：参数块不存在，添加；-1：失败。
     */
    virtual int UpdateParameterBlock(quint64 paramBlkID, QByteArray paramterBlock) = 0;

    /**
     * 更新参数块 ：将参数块保存到当前宏中，如果参数块不存在则添加；
     * @param paramBlkID：参数块标识，格式为：
     * 站号(2)/分系统号(1)/单机号(2)/数据块标识(2)/序号(1)。
     * 站号(2)/分系统号(1)/单机号(2)不用时填0。
     * @param paramList：参数列表。
     * @return 1：成功；0：参数块不存在，添加；-1：失败。
     */
    virtual int UpdateParameterBlock(quint64 paramBlkID, QList<TLxTsspParameter> paramterList) = 0;

    /**
     * 取参数块 ：从宏中取参数块数据。
     * @param paramBlkID：参数块标识，格式为：
     * 站号(2)/分系统号(1)/单机号(2)/数据块标识(2)/序号(1)。
     * 站号(2)/分系统号(1)/单机号(2)不用时填0。
     * @param[out] parameterBlock：参数块数据。
     * @return 1：成功，-1：失败。
     */
    virtual int GetParameterBlock(quint64 paramBlkID, QByteArray& paramterBlock) const = 0;

    /**
     * 取参数块 ：从宏中取参数块数据。
     * @param paramBlkID：参数块标识，格式为：
     * 站号(2)/分系统号(1)/单机号(2)/数据块标识(2)/序号(1)。
     * 站号(2)/分系统号(1)/单机号(2)不用时填0。
     * @param[out] paramList：参数列表，按顺序排列；
     * @param[out] paramSum：参数个数。
     * @return 1：成功，-1：失败。
     */
    virtual int GetParameterBlock(quint64 paramBlkID, QList<TLxTsspParameter>& paramterList) const = 0;

    /**
     * 删除参数块 ：删除参数块
     * @param paramBlkID：参数块标识，格式为：
     * 站号(2)/分系统号(1)/单机号(2)/数据块标识(2)/序号(1)。
     * 站号(2)/分系统号(1)/单机号(2)不用时填0。
     * @return 1：成功；0：不存在指定的参数块；-1：失败。
     */
    virtual int DeleteParameterBlock(quint64 paramBlkID) = 0;

    /**
     * 删除宏中所有的参数块 ：删除宏中所有的参数块，删除后为空的宏。
     * @return 1：成功；-1：失败。
     */
    virtual int DeleteAllParameterBlock() = 0;

    /**
     * 取参数块总数 ：取参数块总数
     * @return 大于等于0：参数块总数；-1：失败。
     */
    virtual int GetParameterBlockSum() const = 0;

    /**
     * 取参数块所有ID ：取参数块所有ID
     * @return 参数块IDlist。
     */
    virtual QList<quint64> GetParameterBlockKeys() const = 0;

    /**
     * 保存宏 ：保存宏;这里指保存到文件或数据库；按宏号保存。
     * @return 1：成功；-1：失败。
     */
    virtual int Save() = 0;

    /**
     * 保存宏到指定文件 ：保存宏到指定的文件
     * @param fileName：文件名。
     * @return 1：成功；-1：失败。
    */
    virtual int SaveAs(char* fileName) = 0;

}; // ILxTsspParameterMacro

/**
 * 参数宏管理
 */
class ILxTsspParamMacroManager : public ILxTsspObject
{
public:
    /**
     * 创建宏 ：创建宏。
     * @param macroObj：要创建的宏。
     * @return 1：成功；-1：失败。
     */
    virtual int CreateMacro(ILxTsspParameterMacro*& macroObj, const TLxTsspParamMacroInformation& macroInfo) = 0;

    /**
     * 添加宏 ：添加宏。
     * @param macroObj：要添加的宏。“参数宏管理”保存宏。
     * @return 1：成功；0：宏已存，更新；-1：失败。
     */
    virtual int AddMacro(ILxTsspParameterMacro* macroObj) = 0;

    /**
     * 删除宏 ： 删除指定的宏。删除宏文件。
     * @param uiMacroID：要删除的宏的代号
     * @return 1：成功；0：宏不存在；-1：失败。
     */
    virtual int DeleteMacro(uint uiMacroID) = 0;

    /**
     * 删除所有宏 ：删除所有宏
     * @return 1：成功；-1：失败。
     */
    virtual int DeleteAll() = 0;

    /**
     * 取指定宏 ： 按照指定的宏代号取参数宏。
     * @param macroID：宏号；
     * @param[out] macroObj：参数宏。
     * @return 1：成功；0：宏不存在；-1：失败。
     */
    virtual int GetMacro(uint uiMacroID, ILxTsspParameterMacro*& macroObj) = 0;

    /**
     * 取指定宏的描述信息 ：取指定宏的描述信息
     * @param macroID：宏号；
     * @param[out] macroInfo：宏信息；
     * @return 1：成功；0：宏不存在；-1：失败。
     */
    virtual int GetMacroInformation(uint uiMacroID,
                                    TLxTsspParamMacroInformation& macroInfo) const = 0;

    /**
     * 取所有宏的描述信息 ：取所有宏的描述信息，一般用于列表显示宏。
     * @param[out] macrosInfoList:宏信息列表。
     * @return 大于等于0：成功，宏个数；-1：失败。
     */
    virtual int GetAllMacroInforamtion(QList<TLxTsspParamMacroInformation>& macroInfoList) = 0;

    /**
         * 根据宏号打开宏文件，读取对应宏至内存，作为当前宏。
         * @param uiMacroID：需加载的宏号
         * @return 大于等于0：成功；-1：失败。
         */
    virtual int LoadParamMarcoByMacroID(uint uiMacroID) = 0;

    /**
         * 取当前系统使用的参数宏。
         * @return NULL：当前系统未加载参数宏；否则：当前系统使用的参数宏。
         */
    virtual ILxTsspParameterMacro* GetCurParamMarco() = 0;
}; // ILxTsspParamMacroManager

/**
 * 设备配置宏
 */
class ILxTsspDeviceConfigMacro
{
public:
    virtual ~ILxTsspDeviceConfigMacro() { }

public:
    /**
     * 创建设备配置宏 ：根据配置宏模板创建设备配置宏
     * @param configMacroTemplate：配置宏模板，Xml格式。
     * @return 1：成功； -1：失败。
     */
    virtual int CreateDevConfigMacro(const QDomNode configMacroTemplate) = 0;

    /**
     * 取配置宏信息 ：取配置宏信息
     * @param[out] configMacroInfo：配置宏信息
     * @return 1：成功； -1：失败。
     */
    virtual int GetConfigMacroInfo(TLxTsspDevConfigMacroInformation& configMacroInfo) = 0;

    /**
     * 取配置宏中包含的设备 ： 取设备配置宏中包含的单机设备。
     * @param[out] DevList：二维数组，第一维是测控链路节点下包含的设备；
     *              站号(2)/分系统号(1)/单机号(2)/数据块标识(2)/序号(1)。不用项填0。
     *              第二维是测控链路节点。
     * @return 大于等于0：节点（设备类型）个数；-1：失败。
     */
    virtual int GetDevice(QMap<quint64, QList<TLxTsspDeviceInformation> >& devList) = 0;

    /**
     * 设置配置宏中包含的设备 ：设置设备配置宏中包含的单机设备。
     * @param[out] DevList：二维数组，第一维是测控链路节点下包含的设备；
     *              站号(2)/分系统号(1)/单机号(2)/数据块标识(2)/序号(1)。不用项填0。
     *              第二维是测控链路节点。
     * @return 大于等于0：节点（设备类型）个数；-1：失败。
     */
    virtual int SetDevice(QMap<quint64, QList<TLxTsspDeviceInformation> > devList) = 0;

}; // ILxTsspDevConfigMacro

/**
 * 设备配置宏管理
 */
class ILxTsspDeviceConfigMacroManager : public ILxTsspObject
{
public:
    /**
     * 添加设备配置宏 ：添加设备配置宏。如果已存在（通过配置宏号判断），则更新。
     * @param configMacro：配置宏
     * @return 1：成功；0：已存在，更新；-1：更新失败。
     */
    virtual int AddDevConfigMacro(ILxTsspDeviceConfigMacro* configMacro) = 0;

    /**
     * 删除配置宏: 删除配置宏。
     * @param ulMacroID：宏号。
     * @return 1：成功；0：未找到指定的配置宏；-1：失败。
     */
    virtual int DeleteDevConfigMacro(uint uiMacroID) = 0;

    /**
     * 删除所有配置宏：删除所有配置宏
     * @return 1：成功； -1：更新失败。
     */
    virtual int DeleteAll() = 0;

    /**
     * 取指定配置宏 ：取指定配置宏
     * @param ulMacroID：宏号。
     * @param[out] configMacro：配置宏
     * @return 1：成功；0：未找到指定的配置宏；-1：失败。
     */
    virtual int GetConfigMacro(ulong ulMacroID, ILxTsspDeviceConfigMacro* configMacro) = 0;

    /**
     * 取指定配置宏的描述信息 ：取指定配置宏的描述信息。
     * @param ulMacroID：配置宏号。
     * @param[out] configMacroInfo：配置宏信息；
     * @return 1：成功；0：未找到指定的配置宏；-1：失败。
     */
    virtual int GetConfigMacroInformation(ulong ulMacroID,
                                          TLxTsspDevConfigMacroInformation& configMacroInfo) = 0;

    /**
     * 取所有设备配置宏的描述信息 ：取所有设备配置宏的描述信息
     * @param[out] macroInfoList:配置宏信息。
     * @return 大于等于0：配置宏个数；-1：失败。
     */
    virtual int GetAllConfigMacroInformation(QList<TLxTsspDevConfigMacroInformation>& macroInfoList) = 0;

}; // ILxTsspDevConfigMacroManager


/**
 * 任务宏定义
 */
typedef struct TaskMacroInformation
{
    /// 代号
    unsigned int uiID;
    /// 名称
    char szName[32];
    /// 描述
    char szDescription[128];
    /// 系统工作模式，8字节
    ulong ulSysWorkMode;
    /// 设备配置宏代号，8字节
    ulong ulDevConfigMacroID;
    /// 设备配置宏。任务宏保存时不保存本字段，运行时通过设备配置管理获取。
    ILxTsspDeviceConfigMacro* pDevConfigMacro;
    /// 参数宏代号
    unsigned int ulParamMacroID;
    /// 参数宏。任务宏保存时不保存本字段，运行时通过参数宏管理获取。
    ILxTsspParameterMacro* pParamMacro;

}TLxTsspTaskMacroInformation;

/**
 *
 */
class ILxTsspTaskMacroManager : public ILxTsspObject
{
public:
    /**
     * 添加任务宏 ：添加宏。
     * @param macro：要添加的宏。“任务宏管理”保存宏。
     * @return 1：成功；0：宏已存，更新；-1：失败。
     */
    virtual int AddTaskMacro(TLxTsspTaskMacroInformation* macro) = 0;

    /**
     * 删除任务宏 ： 删除指定的宏。删除宏文件。
     * @param macroID:要删除的宏的代号
     * @return 1：成功；0：宏不存在；-1：失败。
     */
    virtual int DeleteTaskMacro(uint macroID) = 0;

    /**
     * 删除所有宏 ：删除所有宏
     * @return 1：成功；-1：失败。
     */
    virtual bool DeleteAll() = 0;

    /**
     * 取指定宏 ： 按照指定的宏代号取任务宏。
     * @param macroID：宏号；
     * @param[out] macro：任务宏。
     * @return 1：成功；0：宏不存在；-1：失败。
     */
    virtual int GetMacro(uint macroID, TLxTsspTaskMacroInformation& macro) = 0;

    /**
     * 取指定宏的描述信息 ：取指定宏的描述信息
     * @param macroID：宏号；
     * @param[out] macroInfo：宏信息；
     * @return 1：成功；0：宏不存在；-1：失败。
     */
    virtual int GetMacroInformation(uint macroID,
                                    TLxTsspTaskMacroInformation& macroInfo) = 0;

    /**
     * 取所有宏的描述信息 ：取所有宏的描述信息，一般用于列表显示宏。
     * @param[out] macrosInfoList:宏信息列表。
     * @return 大于等于0：成功，宏个数；-1：失败。
     */
    virtual int GetAllMacroInformation(QList<TLxTsspTaskMacroInformation>& macrosInfoList) = 0;

    /**
     * 添加当前任务宏 ：添加当前任务宏。
     * @param macro：要添加的宏。“任务宏管理”将其作为当前任务宏临时保存，
     *      在程序退出时保存用于下次启动时恢复。调用者负责为任务宏中的参数宏、配置宏指针赋值。
     * @return 1：成功；0：宏已存，更新；-1：失败，参数宏指针为空；-2：配置宏指针为空。
     */
    virtual int AddCurrentTaskMacro(TLxTsspTaskMacroInformation macro) = 0;

    /**
     * 删除当前任务宏 ： 删除指定的宏。从当前宏列表中删除。
     * @param macroID：要删除的宏的代号
     * @return 1：成功；0：宏不存在；-1：失败。
     */
    virtual int DeleteCurrentTaskMacro(uint macroID) = 0;

    /**
     * 删除所有当前宏 ：删除所有当前宏。
     * @return 1：成功；-1：失败。
     */
    virtual bool DeleteCurrentAll() = 0;

    /**
     * 取指定当前宏 ：按照指定的宏代号取任务宏。
     * @param macroID：宏号；
     * @param[out] macro：任务宏。
     * @return 1：成功；0：宏不存在；-1：失败。
    */
    virtual int GetCurrentMacro(uint macroID, TLxTsspTaskMacroInformation& macro) const = 0;

}; // ILxTsspTaskMacroManager

//通道数据转发处理函数定义
typedef void (*ChannelDataTransferHandler)(unsigned char aData[], int& iDataLen,
                                           unsigned int& uiPackageCount,
                                           unsigned char aRemoteIP[],
                                           int& iRemotePort,int& iDataHeader);

/**
 * 通信通道
 */
class ICommunicationChannel
{
public:
    virtual ~ICommunicationChannel() { }

public:
    /**
     * 取通道ID ：取通道ID
     * @param[out] att：通道ID
     * @return 1：成功；-1：失败。
     */
    virtual int GetChannelID(QString& strChannelID) const = 0;
    /**
     * 取通道属性 ：取通道属性
     * @param[out] att：通道属性
     * @return 1：成功；-1：失败。
     */
    virtual int GetAttribute(TLxTsspChannelAttribute& att) const = 0;

    /**
     * 是否连接：判断是否已连接
     * @return true表示连接，false表示连接断开
     */
    virtual bool IsConnected() = 0;

    /**
        * 通道接收 ：从本地接收其他通道发来的数据，并存入接收缓冲区
        * @param aLocalIP：本地IP
        * @param iLocalPort：本地端口号
        * @return 大于等于0：接收数据的实际长度。-1：接收失败。
        */
    virtual int ReceivedData(uchar aLocalIp[], int iLocalPort)=0;

    /**
     * 通道读 ：读取缓冲区中数据。对于可读通道，通道模块创建读线程，读到数据后放到缓冲区，
     *      应用模块可以通过此函数读到缓存的数据。
     * @param[out] aData：数据缓冲区，调用者申请足够的空间；
     * @param[out|in] iDataLen:输入时为缓冲区字节长度
     * @param[out] uiPackageCount：包计数，每增加一包数据加1；
     * @param[out] aIP：数据包来源的IP；iPort：
     * @param[out] 数据包来源的端口号。
     * @param[out] iDataHeader：通道缓存的数据的头指针，
     *      由调用者通过“取当前接收数据的头指针”函数得到头指针，
     *      以后每次调用时传递给通道对象，实现依次读取缓冲区数据的功能。
     * @return 大于等于0：数据包长度；-1：读取失败。
     */
    virtual int ReadReceivedData(uchar* &aData, int& iDataLen, uint& uiPackageCount,
                                 uchar* &aIP, int& iPort, int& iDataHeader) = 0;

    /**
     * 取当前接收数据的头指针 ：取当前接收数据缓冲区的头指针
     * @return 大于等于0：接收数据缓冲区头指针。-1：失败。
     */
    virtual int GetReceivedDataBufferHeader() = 0;

    /**
     * 注册接收接收到的数据 ：注册接收接收到的数据。通道对象接收到数据后转发给注册的接收函数。
     * @param processFunction：调用者处理接收数据的函数。
     *          ChannelDataTransferHandler定义为：
     *                DataProcess（unsigned char aData[], int& iDataLen,
     *                          unsigned int& uiPackageCount,
     *                          unsigned char aRemoteIP[], int& iRemotePort,
     *                          int& iDataHeader）；
     *      参数定义见“通道读”，其中iDataHeader是下次写入数据的头指针。
     *      处理函数的数据缓冲区应足够大。szHostName：调用者对象名称。
     * @return 1：成功；0：已注册过；-1：失败。
     */
    virtual int RegisterReceiveReceivedData(ChannelDataTransferHandler processFunction,
                                            char* szHostName) = 0;

    /**
     * 注销接收接收到的数据 ：注销接收接收到的数据。即不再接收通道转发的数据。一般不需要使用。
     * @param processFunction：调用者处理接收数据的函数。
     *          ChannelDataTransferHandler定义为：
     *                DataProcess（unsigned char aData[], int& iDataLen,
     *                          unsigned int& uiPackageCount,
     *                          unsigned char aRemoteIP[], int& iRemotePort,
     *                          int& iDataHeader）；
     *      参数定义见“通道读”，其中iDataHeader是下次写入数据的头指针。
     *      处理函数的数据缓冲区应足够大。szHostName：调用者对象名称。
     * @return 1：成功；0：已注册过；-1：失败。
     */
    virtual int UnRegisterReceiveReceivedData(ChannelDataTransferHandler processFunction,
                                              char* szHostName) = 0;

    /**
     * 通道写 ：将数据写入通道；
     * @param aData：要写入的数据，
     * @param iDataLen表示要写入的数据的长度；
     * @return 大于等于0：写入数据的实际长度。-1：通道未创建。
     */
    virtual int SendData(uchar aData[], int iDataLen) = 0;

    /**
     * 发送数据到指定网络端点 ：发送数据到指定网络端点
     * @param aData：要写入的数据，
     * @param iDataLen表示要写入的数据的长度；
     * @param aRemoteIP：远程IP；
     * @param iRemotePort：远程端口号。
     * @return 大于等于0：写入数据的实际长度。-1：通道未创建。
     */
    virtual int SendDataToIPEndPoint(uchar aData[], int iDataLen, uchar* aRemoteIp,
                                     int iRemotePort) = 0;

    /**
     * 读取写入的数据 ：读取写入的数据。
     * @param aData：数据缓冲区，调用者申请足够的空间；
     * @param[in|out] iDataLen:输入时表示缓冲区的数据字节长度；
     * @param[out] uiPackageCnt：包计数，每增加一包数据加1；
     * @param[out] aRemoteIP：数据包来源的IP；
     * @param[out] iRemotePort：数据包来源的端口号。
     * @param[out] iDataHeader：返回时表示下一包数据的指针。通道缓存的数据的头指针，
     *      由调用者通过“取当前发送数据的头指针”函数得到头指针，
     *      以后每次调用时传递给通道对象，实现依次读取缓冲区数据的功能。
     * @return 大于等于0：数据包长度；-1：读取失败。
     */
    virtual int ReadSendedData(uchar* &aData, int& iDataLen, uint& uiPackageCnt,
                               uchar* &aRemoteIp, int& iRemotePort, int& iDataHeader) = 0;

    /**
     * 取当前发送数据的头指针 ：取当前发送数据缓冲区的头指针
     * @return 大于等于0：当前发送数据缓冲区的头指针。-1：失败。
     */
    virtual int GetSendedDataBufferHeader() = 0;

    /**
     * 注册接收发送的数据 ：注册接收发送的数据
     * @param processFunction：调用者处理发送的数据的函数；
     * @param szHostName：调用者对象名称。
     * @return 1：成功；0：已注册过；-1：失败。
     */
    virtual int RegisterReceiveSendedData(ChannelDataTransferHandler processFunction,
                                          char* szHostName) = 0;

    /**
     * 注销接收发送的数据 ：注销接收发送的数据。即不再接收通道转发的数据。一般不需要使用。
     * @param processFunction：调用者处理发送的数据的函数；
     * @param szHostName：调用者对象名称。
     * @return 1：成功；0：未注册过；-1：失败。
     */
    virtual int UnRegisterReceiveSendedData(ChannelDataTransferHandler processFunction,
                                            char* szHostName) = 0;

}; // ICommunicationChannel

/**
 * 通信管理
 */
class ILxTsspCommunicationManager : public ILxTsspObject
{
public:
    /**
     * 取所有通道 ：取当前通信管理器管理的所有通道。
     * @param[out] ChannelList：通道列表。
     * @return 大于等于0：通道个数。-1：失败。
     */
    virtual int GetAllChannel(QList<ICommunicationChannel*>& ChannelList) = 0;

    /**
     * 取通道对象 ： 按照通道名称取通道对象。
     * @param strChannelID：通道标识。
     * @param[out] pChannel：通道对象。null表示通道不存在。
     * @param 1：成功；-1：失败。
     */
    virtual int GetChannel(QString strChannelID, ICommunicationChannel* &pChannel) = 0;

    /**
     * 发送数据 ： 向指定通道发送数据。
     * @param strChannelID：通道标识；
     * @param aData：待发送的数据；
     * @param iDataLen：数据长度。
     * @return 大于等于0：实际发送的数据长度；-1：发送失败。
     */
    virtual int SendDataToChannel(QString strChannelID, uchar aData[], int iDataLen) = 0;

    /**
     * 发送数据到指定通道的指定地址 ： 向指定通道发送数据。
     * @param strChannelID：通道标识；
     * @param aData：待发送的数据；
     * @param iDataLen：数据长度；
     * @param aRemoteIP：远程IP；
     * @param iRemotePort：远程端口号。
     * @return 大于等于0：实际发送的数据长度；-1：发送失败。
     */
    virtual int SendDataToChannelAddress(QString strChannelID, uchar aData[],
                                         int iDataLen, uchar* aRemoteIP,
                                         int iRemotePort) = 0;

    /**
     * 创建通道 ： 创建通道。
     * @param strChannelID：通道标识；
     * @param ucType：通道类型, 1-UDP组播；11-指定源组播V3；2-UDP点对点；3-TCP服务端；
     *              4-TCP客户端；5-HDLC；
     * @param ucReadWrite：1-读； 2-写； 3-读写同时；
     * @param aLocalIP：本地IP；
     * @param iLocalPort:本地端口号
     * @param aRemoteIP：远程IP
     * @param iRemotePort:远程口号
     * @param aV3TargetIP：指定组播源IP（uchar TargetIP[32]的倍数）
     * @param ucTargetNum :指定源个数。
     * @param[out] pChannel：创建的通道对象。返回值为0或1时有效。
     * @return 1：成功；0：已创建；-1：创建失败。
     */
    //// 20150625(raod) 增加参数 ucReadWrite, aV3TargetIP, ucTargetNum
    virtual int CreateChannel(QString strChannelID, uchar ucType, uchar ucReadWrite, uchar* aLocalIP,
                              int iLocalPort, uchar* aRemoteIP, int iRemotePort, uchar* aV3TargetIP,
                              uchar ucTargetNum, ICommunicationChannel* &pChannel) = 0;

    /**
     * 删除通道 ：对于通道的删除，如果还有其他对象在引用该通道，则可能引发异常，所以在实现时应采用引用计数的方式，每次调用删除通道函数计数减一，当计数为0时，才真正删除通道对象。计数方式不好，如果一个调用者多次查找如何计数？并且动态创建和删除一般都是一个对象使用。
     * @param szChannelID：通道标识。
     * @return 1：成功；0：通道不存在；-1：删除失败。
     */
    virtual int DeleteChannel(QString strChannelID) = 0;

}; // ICommunicationManager

/**
 * 驱动API接口
 */
class ILxTsspDriverApi : public ILxTsspObject
{
public:
    /**
     * 取板号 ：板号，硬件板号，如调制器板为0x2511；
     * @return 板号，硬件板号，如调制器板为0x2511；
    */
    virtual uint GetBoardID() = 0;

    /**
     * 取索引表 ：取命令、参数、状态、数据索引表。索引表是标识的列表。使用无符号整型数标识，
     *          低字节（0）为命令序号，1、2字节为命令数据类型号，高字节（3）固定为0。
     * @param[out] cType：1-命令索引表，2-参数索引表，3-状态索引表，4-数据索引表；
     * @param[out] aTable：标识的列表，调用者应申请足够的空间。
     * @param[out] iTableLen：有效数据个数。
     * @return 1：成功，-1：设备ID错误，-2：参数错误
     *          -3：设备访问错误，-4：调用DbbApi.dll错误。
     */
    virtual int GetIndexTable(uchar ucType, uint aTable[], int& iTableLen) = 0;

    /**
     * 设置参数 ： 向硬件板设置参数。
     * @param uiParamID：参数标识；低字节（0）为命令序号，1、2字节为命令数据类型号，
     *                  高字节（3）固定为0。
     * @param Parameter：参数值；
     * @param iParamLen：数据有效字节长度。
     * @return 1：成功，-1：设备ID错误，-2：参数错误 -3：设备访问错误，
     *          -4：调用DbbApi.dll错误。
     */
    virtual int SetDeviceParameter(uint uiParamID, uchar aParameter[], int iParamLen) = 0;

    /**
     * 获取状态 ： 从硬件板读取指定的状态。
     * @param uiStatusID：状态标识；低字节（0）为命令序号，1、2字节为命令数据类型号，
     *          高字节（3）固定为0。
     * @param[in|out] iStatusLen：输入时表示缓冲区长度,输出时状态缓冲区中有效数据长度。
     * @param[out] Status：状态缓冲区；
     * @param[out] uiUpdateCount:状态更新计数。
     * @return 1：成功，-1：设备ID错误，-2：参数错误 -3：设备访问错误，
     *          -4：调用DbbApi.dll错误。
     */
    virtual int GetDeviceState(uint uiStatusID, uchar aStatus[], int& iStatusLen,
                               uint& uiUpdateCount) = 0;

    /**
     * 获取所有状态 ：获取本驱动API管理的硬件板的所有状态。
     * @param[out] allState：状态；
     * @param[out] iNum：状态个数。
     * @return 大于等于0：成功，状态个数；-1：失败。
     */
    virtual int GetDeviceAllState(TLxTsspDeviceState allState[], int& iNum) = 0;

    /**
     * 向硬件板写数据 ：向硬件板写数据
     * @param uiDataID：数据标识，低字节（0）为命令序号，1、2字节为命令数据类型号，
     *        高字节（3）固定为0；
     * @param aData：数据缓冲区；
     * @param uiDataLen：数据有效长度。
     * @param[out] uiDataLen：写入的数据长度；
     * @return 大于等于0：写入的数据长度；-1：失败。
     */
    virtual int WriteDataToDriver(uint uiDataID, uchar aData[], uint& uiDataLen) = 0;

    /**
     * 从硬件板读取数据 ：从硬件板读取数据。
     * @param uiDataID：数据标识，低字节（0）为命令序号，1、2字节为命令数据类型号，
     *          高字节（3）固定为0；
     * @param aData：数据缓冲区；
     * @param[in|out] uiDataLen：缓冲区长度,输出时为数据有效长度。
     * @param[out] aData：数据；
     * @param[out] uiReloadCount：硬件加载计数。
     * @return 大于0：数据有效长度；0：没有读到数据，此时可能加载计数变化，需要处理。
     *          -1：失败。
     */
    virtual int ReadDataFromDriver(uint uiDataID, uchar aData[], uint& uiDataLen,
                                   uint& uiReloadCnt) = 0;

    /**
     * 强制清空缓冲区 ：强制清空指定数据缓冲区。
     * @param uiDataID：数据标识。
     * @return 1：成功；-1：失败。
     */
    virtual int ForceClearRamData(uint uiDataID) = 0;

    /**
     * 读时码 ：读取时码
     * @param[out] aTimeCode：字节数组,缺省为8个字节。调用者应申请足够的空间。
     * @param[out|in] iLen：数组长度,输出时有效长度。
     * @return 大于0：成功，时码长度；0：不支持读时码；-1：数组长度不够，读取失败。
     */
    virtual int GetTimeCode(uchar aTimeCode[], int& iLen) = 0;

    /**
     * 设置特定信息 ：设置特定信息，设置驱动API需要的一些特定信息，一般由项目确定。
     *     例如：在扩频多目标不同波束和同波束时，驱动API控制不同的设备板，所以需要动态设置。
     * @param szSpecialInfo：以字符串形式表示的特殊信息，由具体项目约定，程序需要解析；
     * @return 1：成功；0：不需要的信息；-1：失败。
     */
    virtual void SetSpecialInformation(uchar* szSpecialInfo) = 0;

    /**
     * 创建数据读取信号 ：创建数据读取信号，这样读取数据就可以等待有数据信号后才取读。
     *          可以加快处理
     * @param uiDataID：数据标识。低字节（0）为序号，1、2字节为数据类型号，
     *          高字节（3）固定为0；
     * @param[out] sa：信号。
     * @return 1：成功；-1：设备ID错误；-2：参数错误；-3：设备访问错误。
    */
    virtual int CreateDataSignal(uint uiDataID, struct sigaction* sa) = 0;

    /**
     * 释放数据读取信号 ：释放数据读取信号
     * @param uiDataID：数据标识。低字节（0）为序号，1、2字节为数据类型号，
     *          高字节（3）固定为0；
     * @return 1：成功；-1：设备ID错误；-2：参数错误；-3：设备访问错误。
    */
    virtual int DereferenceDataSignal(uint uiDataID) = 0;

}; // ILxTsspDriveApi

/**
 * 硬件访问管理接口
 */
class ILxTsspHAManager : public ILxTsspObject
{

public:
    /**
     * 获取包含指定标识的驱动API ：获取包含指定标识的驱动API
     * @param uiID：标识。低字节（0）为序号，1、2字节为数据类型号，
     *          高字节（3）固定为0；
     * @param[out] driverApi：驱动API，失败返回为null。
     * @return 1：成功；-1：失败。
     */
    virtual int GetAPIInclude(uint uiID, ILxTsspDriverApi* driveApi) = 0;

    /**
     * 取当前设备管理器管理的所有驱动API ：取当前设备管理器管理的所有驱动API
     * @param[out] driverApiList：驱动API列表。
     * @return 大于等于0：驱动API个数；-1：失败。
     */
    virtual int GetAllDriverApi(QList<ILxTsspDriverApi*>& driverApiList) = 0;

}; // ILxTsspHDManager

/// 消息处理函数定义
typedef void (*MessageProcessingHandler)(TLxTsspMessage msg);

/**
 * 消息管理接口
 */
class ILxTsspMessageManager : public ILxTsspObject
{
public:
    /**
     * 消息接收者注册 ：消息接收者注册。
     * @param msgT、msgC：见消息定义，当MsgT和MsgC都是0xFFFF时，则接收所有消息。
     * @param strRecvName：接收者名称，一般为对象名称。
     * @param msgRecvFunc：接收者的消息处理函数。一般可以直接使用接收者函数
     *          “void SendMessage(TILxTsspMessage msg);”或相同定义的函数。
     * @return 1：成功；-1：失败。
     */
    virtual int MsgReceiverRegister(ushort msgT, ushort msgC, QString strRecvName,
                                    MessageProcessingHandler msgRecvFunc) = 0;

    /**
     * 消息接收者退订 ：消息接收者退订信息。
     * @param msgT、msgC：见消息定义，当MsgT和MsgC都是0xFFFF时，则接收所有消息。
     * @param strRecvName：接收者名称，一般为对象名称。
     * @return 1：成功；-1：失败。
     */
    virtual int MsgReceiverUnregister(ushort msgT, ushort msgC, QString strrecvName) = 0;

    /**
     * 发送消息 ：应用模块调用此函数来发送消息。
     * @param msg：要发送的消息。
     * @return 1：成功；-1：失败。
     */
    virtual int FireMessage(TLxTsspMessage msg) = 0;

    /**
     * 取命令唯一序号 ：消息序号是唯一的，发送模块每次发送命令前从消息管理器申请，
     *          接收模块发送应答时使用此序号，以实现应答和命令的对应关系；
     *          对于不需要应答的消息（命令、数据和状态消息），必须填写0；
     *          对于需要应答的命令消息必须填写从消息管理器申请的消息唯一序号；
     *          消息管理模块维护此序号，当应用模块读取时，加1返回；
     * @return 消息序号。
     */
    virtual uint GetSequenceNum() = 0;

    /**
     * 判断消息是否注册 ：判断消息是否注册。
     * @param msgT、msgC：见消息定义。
     * @return 1：已注册；-1：未注册。
    */
    virtual int IsMessageRegistered(ushort msgT, ushort msgC) = 0;

    /**
     * @brief AddDestination 添加消息的目的地。
     * 通过添加目的地，则消息可以转发给用户设计的通道，该通道可能不是通信通道，而是用户的消息处理类。
     * @param usMsgDest 消息的目的地，如果为0，则表示接收所有消息，方便调试用。
     * @param szRecver 消息接收者名称
     * @param pMsgRecvChnl 接收通道，实现ICommunicationChannel接口的对象。
     * @return 1:成功；-1:失败
     */
    virtual int AddDestination(unsigned short usMsgDest, unsigned char * szRecver,
                               ICommunicationChannel *pMsgRecvChnl) = 0;

    /**
     * @brief DeleteDestination
     * @param usMsgDest 消息的目的地。
     * @return 1:成功；-1:失败
     */
    virtual int DeleteDestination(unsigned short usMsgDest) = 0;

}; // ILxTsspMessageManager

/**
 * 界面显示接口
 */
class ILxTsspShow : public ILxTsspObject
{
public:
    /**
     * 显示界面 ：显示界面，当外部对象调用此函数时，本对象应弹出对应的界面。
     * @param parentForm：父窗口。
     * @return 界面对象的界面。
    */
    virtual QWidget* ShowMe(QWidget* parent, const QString& strTitle) = 0;

    /**
     * 取宏缺省参数：
     * @param pMacroObj：当前参数宏指针
     * @return 1：成功；-1：失败。
    */
    virtual int CreateDefaultMacroObj(ILxTsspParameterMacro* pMacroObj) = 0;

    /**
     * 显示界面：显示界面，当外部对象调用此函数时，本对象应弹出对应的界面。
     * @param parent：父窗口。
     * @param ucType：界面类型，0：参数设置界面，1：参数宏界面。
     * @param pMacroObj：当前参数宏指针
     * @return 界面对象的界面。
    */
    virtual QWidget* ShowMeInEdit(QWidget* parent, uchar ucType, ILxTsspParameterMacro* pMacroObj) = 0;

    /**
     * 保存宏参数：
     * @param pMacroObj：当前参数宏指针
     * @return 1：成功；-1：失败。
    */
    virtual int SaveToMacroObj(ILxTsspParameterMacro* pMacroObj) = 0;

    /**
     * 用户界面帮助ID接口 ：用户界面帮助ID接口，帮助功能根据此ID查找帮助文件中
     *          对应的页面并显示；此帮助ID由用户界面管理程序根据菜单名称等信息设置
     * @param[out] szHelpID：用户界面帮助ID。
     * @return szHelpID:长度。
    */
    virtual int GetContextHelpID(char* szHelpID) = 0;

}; // ILxTsspShow

/**
 * 界面管理接口
 */
class ILxTsspUIManager : public ILxTsspObject
{
public:

   // virtual ~ILxTsspUIManager(){}

    /**
     * 显示界面 ：显示界面。
     * @param parentForm：父窗口。
     * @return 界面对象的界面。
     */
    virtual void Show() = 0;

}; // ILxTsspInterfaceManager


typedef void (*LogHandler)(TLxTsspLog log);
/**
 * 日志管理接口
 */
class ILxTsspLogManager : public ILxTsspObject
{
public:
    /**
     * 添加一条日志 ：添加一条日志。
     * @param log：日志。
     * @return 1：成功；-1：失败。
    */
    virtual int Add(TLxTsspLog log) = 0;

    /**
     * 注册接收日志 ： 接收者通过注册来接收日志。
     * @param receiverFunction：接收者的日志处理函数。
     *          LogHandler定义为(*Handler)(TLxTsspLog log);
     * @param hostName：接收者名称，一般使用对象名称或对象ID转换成的字符串。
     * @return 1：成功；-1：失败。
     */
    virtual int LogReceiverRegister(LogHandler receiverFunction, QString strhostName) = 0;

    /**
     * 注销接收日志 ： 接收者通过注销来接收日志。
     * @param hostName：接收者名称，一般使用对象名称或对象ID转换成的字符串。
     * @return 1：成功；-1：失败。
     */
    virtual int LogReceiverUnRegister(QString strhostName) = 0;

    /**
     * 查询日志的起止时间 ：查询当前保存的日志的起止时间。
     * @param[out] tStartTime:日志开始时间;
     * @param[out] tEndTime:日志结束时间;
     * @return 1：成功；-1：失败。
     */
    virtual int QueryStartAndEndTime(time_t& tStartTime, time_t& tEndTime) = 0;

    /**
     * 查询日志 ：查询命令中列出了所有索引项，最终的查询结果是所有索引项的交集；
     *      按照时间查询使用tStartTime和tEndTime，如果tStartTime为null，
     *      则从开始记录日志的时间开始，如果tEndTime为null，则到最新的日志记录结束，
     *      如果都为null则返回所有的日志；uchar类型索引项如果为0则不作为索引项，
     *      string为null则不作为索引项，不作为索引项表示无论该项为什么值都认为满足要求；
     * @param tStartTime：日志开始时间；
     * @param tEndTime：日志结束时间；
     * @param szUserName：登录用户名称；
     * @param uiSource：日志来源；设备号。
     * @param szObjectID：写入日志的对象名称；
     * @param ucSrcType：日志生成者类别，平台对象(1)、应用对象(2)、其他待定；
     * @param ucWarningLevel：警告级别，提示(1)、告警(2)、错误(3)；
     * @param ucMainType：主类型，分为用户操控(1)和设备工作(2)；
     * @param ucSubType：子类型，用户操控的子类型分为：
     *          0x001--界面操作；0x002--用户管理；
     *          设备工作的子类型分为：
     *          0x001--设备状态；0x002--内部接口；0x003--外部部接口；
     * @param[out] logList：返回的日志列表。
     * @return 大于等于0：日志个数；-1：失败。
     */
    virtual int Query(time_t& tStartTime, time_t& tEndTime, char szUserName[],
                      uint uiSource, char szObjectID[], uchar ucSrcType,
                      uchar ucWarningLevel, uchar ucMainType, uchar ucSubType,
                      QList<TLxTsspLog>& logList) = 0;

    /**
     * 删除日志 ：删除日志
     * @see Query
     * @return 1：成功；-1失败，可能是没有权限删除不成功。
    */
    virtual int Delete(time_t& tStartTime, time_t& tEndTime, char szUserName[],
                       uint uiSource, char szObjectID[], uchar ucSrcType,
                       uchar ucWarningLevel, uchar ucMainType, uchar ucSubType) = 0;

}; // ILxTsspLogManager


/// 公共信息类型号，按序排列，100以下为公共信息，项目应用从100开始。
//站代号，2个字节无符号数
#define PUBLIC_STATIONID    1
//分系统号，1个字节无符号数
#define PUBLIC_SUBSYSID     2
//设备号，2个字节无符号数
#define PUBLIC_DEVICEID     3
//是否本地控制，此为初始状态。bool
#define PUBLIC_ISLOCALCONTROL   4
//模式。所有包含的模式，结构为TLxTsspWorkModeInfoStruct，依次排列，根据数据长度判断个数。
#define PUBLIC_ALLWORKMODE      5
//当前模式。模式ID。char szID[16];
#define PUBLIC_CURRENTMODE    6
//是否显示模式选择对话框。bool
#define PUBLIC_SHOWMODESELECT   7
//是否正在进行模式切换。bool
#define PUBLIC_ISMODESWITCH     8
//用户信息。指当前具有操作员权限的用户。结构为TLxTsspUserInformation。
#define PUBLIC_USERINFO       9
//下一个模式，用于模式切换
#define PUBLIC_NEXTMODE       10

/**
 * 公共信息管理接口
 */
class ILxTsspPublicInfoManager : public ILxTsspObject
{
public:
    /**
     * @brief 根据信息的ID来获取信息。得到的是信息的拷贝。
     * @param uiInfoID为信息的ID，从0开始排序；
     * @param[in|out] pInfo为获取的信息，调用者应根据信息长度申请足够的内存；
     * @param[in|out] iInfoLen为信息字节长度，输入时为调用者申请的字节长度,
     *              返回时为返回的字节长度。
     * @return 0 -- 无此信息, 1 -- 成功，-1 -- 输入的信息类型错误。
     */
    virtual int GetOneInfoItem(unsigned int uiInfoID, unsigned char* pInfo,
                               int& iInfoLen) const = 0;

    /**
     * @brief 根据信息的ID来获取信息。得到的是信息的指针。
     * @param uiInfoID为信息的ID，从0开始排序。
     * @param[out] pInfo为信息指针；
     * @param[out] iInfoLen为信息字节长度。
     * @return 0 -- 无此信息, 1 -- 成功，-1 -- 输入的信息类型错误。
     */
    virtual int GetOneInfoItemPointer(unsigned int uiInfoID,
                                      const unsigned char* &pInfo, int& iInfoLen) const = 0;

    /**
     * @brief 根据信息的ID来更新信息。如果信息不存在则添加。公共信息管理根据
     *				iInfoLen申请内存，并拷贝pInfo内容。如果变化了启动更新回调。
     * @param uiInfoID为信息的ID，从0开始排序；
     * @param pInfo为信息指针；
     * @param infolen为信息长度。
     * @return 0 -- 无此信息,添加成功, 1 -- 更新成功，2 -- 成功，输入的信息长度和已有信息长度不一致。
     */
    virtual int UpdateOneInfoItem(unsigned int uiInfoID,
                                  unsigned char* pInfo, int iInfoLen) = 0;

    /**
     * @brief 订阅消息更新通知
     * @param uiInfoID为信息的ID，
     * @param processFunction为回调处理函数。
     * @return 0 -- 无此信息, 1 -- 成功，-1 -- 失败。
     */
    virtual int SubscribeUpdateMessage(unsigned int uiInfoID,
                                       UpdateProcessHandler processFunction) = 0;


    /**
     * @brief 取本地站号，从配置文件而来
     * #define PUBLIC_STATIONID    1
     */
    virtual ushort GetLocalStationID() = 0;

    /**
     * @brief 取本地分系统号，从配置文件而来
     * #define PUBLIC_SUBSYSID     2
     */
    virtual uchar GetLocalSubSysID() = 0;

    /**
     * @brief 取本地设备号，从配置文件而来
     * #define PUBLIC_DEVICEID     3
     */
    virtual ushort GetLocalDeviceID() = 0;

    /**
     * @brief 本控还是分控，true表示本地控制；false表示远程控制
     * #define PUBLIC_ISLOCALCONTROL   4
     */
    virtual bool IsDeviceLocalControl() = 0;

    /**
     * @brief 设置设备本地控制方式
     * @param bLocalControl:true：本地控制；false：远程控制
     */
    virtual void SetLocalControlManner(bool bLocalControl) = 0;

    /**
     * @brief 取所有模式信息
     * @param 所有模式信息,模式信息从配置文件中来
     * #define PUBLIC_ALLWORKMODE      5
     * @return 模式个数
     */
    virtual int GetAllModeInfo(QList<TLxTsspWorkModeInfoStruct>& allModeInfo) = 0;

    /**
     * @brief 取当前模式信息
     * #define PUBLIC_CURRENTMODE    6
     */
    virtual TLxTsspWorkModeInfoStruct GetCurrentModeInfo() = 0;

    /**
     * @brief 设置当前模式，当前模式在程序初始启动时从配置信息中读取，程序退出时写入到配置文件。
     * 在程序运行中切换模式时，由切换模块更改。
     * @param newModeID:新工作模式的ID.
     * @return 1：成功；0：新的模式即为当前模式；-1：无此模式。
     */
    virtual int SetCurrentMode(char newModeID[]) = 0;

    /**
     * @brief 是否显示模式选择界面，是否显示由配置文件配置
     * #define PUBLIC_SHOWMODESELECT   7
     * @return true：显示；false：不显示。
     */
    virtual bool IsShowModeSelect() = 0;

    /**
     * @brief 设置模式切换状态。
     * 在模式切换开始时，由模式切换模块设置为模式切换中，即参数为true；
     * 模式切换结束后，由模式切换模块设置为false；
     * @param bSwitch:true：正在进行模式切换；false：未进行模式切换。
     */
    virtual void SetModeSwitching(bool bSwitch) = 0;

    /**
     * @brief 是否正在进行模式切换
     * #define PUBLIC_ISMODESWITCH     8
     */
    virtual bool IsModeSwitching() = 0;

    /**
     * @brief 取当前具有操作员权限的用户信息。
     * #define PUBLIC_USERINFO       9
     * @return
     */
    virtual TLxTsspUserInformation GetUserInfo() = 0;

    /**
     * @brief 设置当前具有操作员权限的用户信息。
     * @param userInfo:用户信息
     * @return true:成功，false:失败
     */
    virtual bool SetUserInfo(TLxTsspUserInformation userInfo) = 0;


}; // ILxTsspPublicInfoManager


/////模块的导出接口
//
typedef int (*PGetModuleInfo)(TLxTsspModuleInfo& moduleInfo);
//
typedef int (*PGetConfigFormat)(QString& strConfigFormat);
//
typedef int (*PGetResourceReq)(QString& strResourceReq);
//
typedef int (*PGetUpdateInfo)(QString& strUpdateInfo);
//
typedef int (*PCreateObjects)(const QDomNode xmlConfigInfo,
                              QMap<quint64, void*>& ObjList);
//
typedef int (*PDestroyObjects)();

typedef struct ModuleExportStruct
{
    /// 模块动态库
    QLibrary* pDLL;
    /// 获取模块的信息，包括模块的公司名称、模块名称、配置标识、版本、开发人员等。
    PGetModuleInfo pGetModuleInfo;
    /// 获取模块配置格式。模块使用时的配置格式，以示例形式给出，用于系统配置。
    /// 类似原来文档目录下的模块配置信息.txt中的内容。
    PGetConfigFormat pGetConfigFormat;
    /// 获取模块资源需求信息。
    /// 给出模块（类）运行时使用的资源，即外部运行环境，以及模块（类）的输出等信息，
    /// 用于系统配置。类型原来文档目录下的资源使用.txt中的内容。
    PGetResourceReq pGetResourceReq;
    /// 获取模块版本变更信息。模块版本的变更过程、内容、变更时间、人员等。
    /// 类似原来文档目录下的readme.txt中的内容。
    PGetUpdateInfo pGetUpdateInfo;
    /// 创建应用对象。根据配置信息创建对象。
    PCreateObjects pCreateObjects;
    /// 销毁对象。在进程关闭时调用，此时所有对象已停止并清理了占用的资源。由对象创建者销毁对象。
    PDestroyObjects pDestroyObjects;
    /// 模块的配置信息
    QDomNode configInfo;
    /// 创建的对象列表
    QMap<quint64, void*> objectList;

} TModuleExport; // ILxTsspModuleExport

/**
 * @brief 状态管理。可以看作网关平台状态信息元管理。
 */
class ILxTsspStateManager : public ILxTsspObject
{
public:
    /**
     * @brief 取状态标识号
     * @param usStationID：站号
     * @param ucSubSysID：分系统号
     * @param usDeviceID：分机号
     * @param usType：状态类型号
     * @param ucSn：序号
     * @return 状态标识号
     */
    virtual quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
                               uchar ucSn) = 0;

    /**
     * @brief ApplyOneItem从状态管理器中申请一个状态信息元（由信息元管理器申请一个节点和相应的数据空间）
     * @param ui64StatusID:状态标识号
     * @param iLen: 输入时表示申请长度，返回时表示申请到的实际长度
     * @return NULL--申请失败；其它--指向信息元节点数据区的指针；uiLen:返回申请到的实际长度
     */
    virtual uchar* ApplyOneItem(quint64 ui64StatusID, int &iLen) = 0;

    /**
     * @brief RemoveOneItem从信息元管理器中释放一个信息元（由信息元管理器申请一个节点和相应的数据空间）
     * @param ui64StatusID:状态标识号
     * @return 返回值：TRUE--释放成功；FALSE:释放失败；
     */
    virtual bool RemoveOneItem(quint64 ui64StatusID) = 0;

    /**
     * @brief FindOneItem从信息元管理器中查找一个信息元（由信息元管理器申请一个节点和相应的数据空间）
     * @param ui64StatusID:状态标识号
     * @param iLen:数据区的长度
     * @return 返回值：NULL--找不到；其它--指向信息元节点数据区的指针；
     */
    virtual uchar* FindOneItem(quint64 ui64StatusID, int &iLen) = 0;

    /**
     * @brief UpdateOneItem:更新信息元管理器中的一个节点内容
     * @param ui64StatusID:状态标识号
     * @param pData:新的数据
     * @param iDataLen:新数据的长度（信息元数据区长度）
     * @return TRUE--成功；FALSE--失败；
     */
    virtual bool UpdateOneItem(quint64 ui64StatusID, uchar* pData, int &iDataLen) = 0;

    /**
     * @brief QueryOneItem:获取信息元管理器中的一个节点
     * @param ui64StatusID:状态标识号
     * @param pData:用来存放数据的缓冲区指针
     * @param iDataLen:为pData缓冲区的长度
     * @return:TRUE--成功；FALSE--失败；成功时：pData中为数据；iDataLen:返回实际读到的长度
     */
    virtual bool QueryOneItem(quint64 ui64StatusID, uchar* pData, int &iDataLen) = 0;

    /**
     * @brief IncreaseTimeTag:信息元管理器中的一个节点的时间字加1。
     * @param ui64StatusID:状态标识号
     * @return 1 --成功； 0--更新失败（找不到信息元）
     */
    virtual uint IncreaseTimeTag(quint64 ui64StatusID) = 0;

    /**
     * @brief GetTimeTag 取信息元管理器中的一个节点的时间字。
     * @param ui64StatusID:状态标识号
     * @return 节点的时间字
     */
    virtual uint GetTimeTag(quint64 ui64StatusID) = 0;

    /**
     * @brief GetOneItemByIndex:按照序号查找信息元,从0开始
     * @param uiIndex:序号
     * @param ui64StatusID:状态标识号
     * @param pData:用来存放数据的缓冲区指针
     * @param iDataLen:为pData缓冲区的长度
     * @param uiTimeTag 状态更新计数
     * @return TRUE--成功；FALSE--失败；成功时：pData中为数据；iDataLen:返回实际读到的长度
     */
    virtual bool GetOneItemByIndex(int iIndex, quint64& ui64StatusID, uchar* pData,
                                   int &iDataLen, uint& uiTimeTag) = 0;

    /**
     * @brief GetItemCounts:获取所有信息元的个数
     * @return 信息元的个数
     */
    virtual int GetItemCounts() = 0;

};

/// 数据缓冲区接口
class ILxTsspDataBuffer
{
public:
    virtual ~ILxTsspDataBuffer(){}

public:
    /**
     * @brief GetDataID 取数据缓冲区标识号
     * @return 缓冲区标识号
     */
    virtual quint64 GetDataID() = 0;

    /**
     * @brief GetReloadCount 取加载计数
     * @return 加载计数
     */
    virtual uint GetReloadCount() = 0;

    /**
     * @brief GetDataBlockLen 块长(字节数)：缓冲区中数据块的长度；
     * @return 块长(字节数)
     */
    virtual int GetDataBlockLen() = 0;

    /**
     * @brief GetBlockSum 取总块数
     * @return 总块数
     */
    virtual int GetBlockSum() = 0;

    /**
     * @brief GetDataBufferHeader 取当前头指针位置
     * @return 缓冲区中当前头指针位置
     */
    virtual int GetDataBufferHeader() = 0;

    /**
     * @brief GetBufferStatus 取缓冲区状态；
     * @return 缓冲区状态
     */
    virtual DataBufferPropertyStruct GetBufferStatus() = 0;

    /**
     * @brief GetNextDataBlock 取下一帧数据
     * @param iHeader 调用者拥有的最新指针，调用后本模块进行加1处理，如果已到最大值则自动回0；
     *        如果没有读取到数据，返回时值不变，如果读到数据，返回时为下一帧数据的位置值
     * @param aData 数据，由调用者申请空间；
     * @param iDataLen 输入时表示pData指向的缓冲区长度，输出时表示实际拷贝的数据长度。
     * @param uiReloadCount 加载计数，当加载计数变化时表示进行了一次参数加载，此时帧结构可能已变化，调用者应进行相应处理；
     * @return 没有读到数据返回值为0，iHeader不变化，pData中数据无效，加载计数按实际情况返回；
     *         如果data长度不够则返回-1，uiHeader不变化，aData中数据无效；
     *         其它异常(参数错误)返回-2；
     */
    virtual int GetNextDataBlock(int& iHeader, uchar* pData, int& iDataLen, uint& uiReloadCount) = 0;

    /**
     * @brief GetDataBlocks 取数据
     * @param iHeader 调用者拥有的最新指针，调用后本模块每读取一块数据则加1，如果已到最大值则自动回0；
     *        如果没有读取到数据，返回时值不变，如果读到数据，返回时为下一帧数据的位置值；
     * @param aData 数据，多块数据不分隔，总长度由iDataLen指定
     * @param iDataLen 总数据长度,输入时表示pData指向的缓冲区长度，输出时表示实际拷贝的数据长度。
     * @param iBlockSum 块数，输入时表示要取的数据块数，输出时表示取到的数据块数
     * @param uiReloadCount 加载计数，实现时注意不用跨加载计数拷贝数据，即加载计数变化后的数据应下一次拷贝
     * @return 总有效数据长度，0表示没有读到数据，-1表示读取错误 -2其它异常(参数错误)
     */
    virtual int GetDataBlocks(int& iHeader, uchar* pData, int& iDataLen, int& iBlockSum, uint& uiReloadCount) = 0;

    /**
     * @brief GetLastDataBlock 取最新数据块
     * @param pData 数据
     * @param iDataLen 数据长度.输入时表示pData指向的缓冲区长度，输出时表示实际拷贝的数据长度。
     * @param uiReloadCount 加载计数
     * @return 数据有效长度，0表示没有读到数据，-1表示读取错误 -2其它异常(参数错误)
     */
    virtual int GetLastDataBlock(uchar* pData, int& iDataLen, uint& uiReloadCount) = 0;

    /**
     * @brief GetBufferStatusPointer 取缓冲区状态指针
     * @return 缓冲区状态指针
     */
    virtual DataBufferPropertyStruct* GetBufferStatusPointer() = 0;

    /**
     * @brief GetBufferDataStartPos取缓冲区数据起始位置，不包含GIO，结合缓冲区头、块长、块数和加载计数，可以按照原来的方式操作。
     * @return 缓冲区数据起始位置
     */
    virtual uchar* GetBufferDataStartPos() = 0;

    /**
     * @brief AddData 向缓冲区添加数据。
     * 收到数据后，比对数据长度是否快长的整数倍，如果不是则返回-1；
     * 如果是，则将数据拷贝到缓冲区，并更新头指针。返回1。
     * @param pData 要写入到数据的指针。
     * @param iDataLen 数据长度，以字节为单位。
     * @return
     */
    virtual int AddDataToBuffer(uchar* pData, int iDataLen) = 0;

    /**
     * @brief SubscribeNewData 订阅，通过订阅接收信息，就可以不需要通过查询读取信息，处理更加及时
     * @param processFunction订阅者接收信息的函数
     * @param strHostName订阅者对象ID
     * @return 成功返回true，失败返回false
     */
    virtual bool SubscribeNewData(UpdateProcessHandler processFunction, quint64 ui64HostID) = 0;

    /**
     * @brief UnSubscribeNewData 退订，退订后不再接收信息，一般不需要退订，如果在程序退出时使用本函数要处理对象不在线等异常
     * @param ui64HostID 订阅者对象ID
     * @return 成功返回true，失败返回false
     */
    virtual bool UnSubscribeNewData(quint64 ui64HostID) = 0;

};

/**
 * @brief 数据缓冲区管理。可以看作网关平台数据信息元管理。
 */
class ILxTsspDataBufferManager : public ILxTsspObject
{
public:
    /**
     * @brief 取数据标识号
     * @param usStationID：站号
     * @param ucSubSysID：分系统号
     * @param usDeviceID：分机号
     * @param usType：数据类型号
     * @param ucSn：序号
     * @return 数据标识号
     */
    virtual quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
                               uchar ucSn) = 0;

    /**
     * @brief ApplyOneItem从数据管理器中申请一个数据信息元（由信息元管理器申请一个节点和相应的数据空间）
     * @param ui64DataID:数据标识号
     * @param iBlockLen:每块数据的长度
     * @param iBlockNum:总块数
     * @return 数据缓冲区的接口指针,成功后也应通过接口函数判断块长和块数是否和申请的值一致; NULL--申请失败；
     */
    virtual ILxTsspDataBuffer* ApplyOneItem(quint64 ui64DataID, int iBlockLen, int iBlockNum) = 0;


    /**
     * @brief AddOneItem 添加一个数据缓冲区对象。
     * @param pDataBuffer 数据缓冲区对象的接口指针
     * @return 返回值：TRUE--成功；FALSE:失败；只有要添加的数据缓冲区对象已存在（标识相同）才失败。
     */
    virtual bool AddOneItem(ILxTsspDataBuffer* pDataBuffer) = 0;

    /**
     * @brief RemoveOneItem从信息元管理器中释放一个信息元（由信息元管理器申请一个节点和相应的数据空间）
     * @param ui64DataID:数据标识号
     * @return 返回值：TRUE--释放成功；FALSE:释放失败；
     */
    virtual bool RemoveOneItem(quint64 ui64DataID) = 0;

    /**
     * @brief FindOneItem从信息元管理器中查找一个信息元（由信息元管理器申请一个节点和相应的数据空间）
     * @param ui64DataID:数据标识号
     * @param uiLen: 输入时表示申请长度，返回时表示实际长度
     * @return 返回值：NULL--找不到；其它--指向信息元节点数据区的指针；uiLen:返回实际的长度
     */
    virtual ILxTsspDataBuffer* FindOneItem(quint64 ui64DataID) = 0;

    /**
     * @brief GetOneItemByIndex:按照序号查找信息元,从0开始
     * @param uiIndex:序号
     * @return 数据缓冲区的指针 NULL--失败；
     */
    virtual ILxTsspDataBuffer* GetOneItemByIndex(int iIndex) = 0;

    /**
     * @brief GetItemCounts:获取所有信息元的个数
     * @return 信息元的个数
     */
    virtual int GetItemCounts() = 0;

};

/**
 * @brief 数据缓冲区管理。可以看作网关平台数据信息元管理。
 */
class ILxTsspModeSwitch : public ILxTsspObject
{
public:
    /**
     * @brief LoadCommonModules加载公共模块
     * @return 1:Success; -1:fail
     */
    virtual int LoadCommonModules() = 0;

    /**
     * @brief SwitchToNewMode
     * @param strNewModeID
     * @return 1:Success; 0:Current mode; -1:fail
     */
    virtual int SwitchToNewMode(QString strNewModeID) = 0;

    /**
     * @brief GetCommModuleList,For Debug
     * @param commModuleList
     * @return 1:Success; -1:fail
     */
    virtual int GetCommModuleList(QMap<QString, TModuleExport>& commModuleList) = 0;

    /**
     * @brief GetModeModuleList,For Debug
     * @param modeModuleList
     * @return 1:Success; -1:fail
     */
    virtual int GetModeModuleList(QMap<QString, TModuleExport>& modeModuleList) = 0;

};

/**
 * @brief 对象管理接口
 */
class LXTSSPOBJECTMANAGERSHARED_EXPORT LxTsspObjectManager : public ILxTsspObject
{
    DECLARE_ILXTSSPOBJECT
    public:
        LxTsspObjectManager();

    /**
     * 取对象管理器 ： 取对象管理器
     * @return 返回对象管理器。
     */
    static LxTsspObjectManager* Instance();

    /**
     * @brief 取对象标识号
     * @param usStationID：站号
     * @param ucSubSysID：分系统号
     * @param usDeviceID：分机号
     * @param usType：数据类型号
     * @param ucSn：序号
     * @return 对象标识号
     */
    quint64 GetID(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
                               uchar ucSn);

    /**
     * @brief 查找对象，并得到对象指针，调用者将其转换为合适的指针来使用。
     * @param usStationID：站号
     * @param ucSubSysID：分系统号
     * @param usDeviceID：分机号
     * @param usType：对象号
     * @param ucSn：序号
     * @return 要查找的对象，null标识未找到。
     */
    void* GetObject(ushort usStationID, uchar ucSubSysID, ushort usDeviceID, ushort usType,
                    uchar ucSn);
    /**
     * @brief 取对象:查找对象，并得到对象指针，调用者将其转换为合适的指针来使用。
     * @param ui64ObjectID：对象ID。
     * @return 要查找的对象，null标识未找到。
     */
    void* GetObject(quint64 ui64ObjectID);

    /**
     * 添加对象 ：添加对象。一般由对象添加私有对象，如数据采集模块添加管理
     *          循环缓冲区的数据管理对象。
     * @param ui64ObjectID：对象ID。
     * @param pNewObj：要添加的对象，不要求实现ILxTsspObject接口。
     * @param iObjType：对象类型，0：公共对象，1：和模式相关的对象。
     * @return 1：成功；0：对象已存在，通过名称判断；-1：失败。
     */
    int AddObject(quint64 ui64ObjectID, void* pObject, int iObjType = 0);

    /**
     * @brief 移除模式相关对象，主要用于模式切换。
     * @return 1：成功；0：当前模式无对象；-1：失败。
     */
    int RemoveModeObjects();

    /**
     * @brief 取用户管理对象
     * @return 未查找到该对象则返回NULL，找到则返回该对象接口的指针
     */
    ILxTsspUserManager* GetUserManager();

    /**
     * @brief 取用户管理对话框对象
     * @return 未查找到该对象则返回NULL，找到则返回该对象接口的指针
     */
    ILxTsspShow* GetUserManageDialog();

    /**
     * @brief 取用户登录对话框对象
     * @return 未查找到该对象则返回NULL，找到则返回该对象接口的指针
     */
    ILxTsspShow* GetUserLoginDialog();

    /**
     * @brief 取登录用户显示对话框对象
     * @return 未查找到该对象则返回NULL，找到则返回该对象接口的指针
     */
    ILxTsspShow* GetLoginedUserShowDialog();

    /**
     * @brief 取日志管理对象
     * @return 未查找到该对象则返回NULL，找到则返回该对象接口的指针
     */
    ILxTsspLogManager* GetLogManager();

    /**
     * @brief 取日志查看对象
     * @return 未查找到该对象则返回NULL，找到则返回该对象接口的指针
     */
    ILxTsspShow* GetLogShowForm();

    /**
     * @brief 取参数宏管理对象
     * @return 未查找到该对象则返回NULL，找到则返回该对象接口的指针
     */
    ILxTsspParamMacroManager* GetParamMacroManager();

    /**
     * @brief 取配置宏管理对象
     * @return 未查找到该对象则返回NULL，找到则返回该对象接口的指针
     */
    ILxTsspDeviceConfigMacroManager* GetDevConfigMacroManager();

    /**
     * @brief 取任务宏管理对象
     * @return 未查找到该对象则返回NULL，找到则返回该对象接口的指针
     */
    ILxTsspTaskMacroManager* GetTaskMacroManager();

    /**
     * @brief 取通信管理对象
     * @return 未查找到该对象则返回NULL，找到则返回该对象接口的指针
     */
    ILxTsspCommunicationManager* GetCommunicationManager();

    /**
     * @brief 取硬件访问管理对象
     * @return 未查找到该对象则返回NULL，找到则返回该对象接口的指针
     */
    ILxTsspHAManager* GetHAManager();

    /**
     * @brief 取消息管理对象
     * @return 未查找到该对象则返回NULL，找到则返回该对象接口的指针
     */
    ILxTsspMessageManager* GetMessageManager();

    /**
     * @brief 取界面管理对象
     * @return 未查找到该对象则返回NULL，找到则返回该对象接口的指针
     */
    ILxTsspUIManager* GetUIManager();

    /**
    * @brief 取公共信息管理对象
    * @return 未查找到该对象则返回NULL，找到则返回该对象接口的指针
    */
   ILxTsspPublicInfoManager* GetPublicInfoManager();

   /**
    * @brief 取状态管理对象
    * @return 未查找到该对象则返回NULL，找到则返回该对象接口的指针
    */
   ILxTsspStateManager* GetStateManager();

   /**
    * @brief 取数据缓冲区管理对象
    * @return 未查找到该对象则返回NULL，找到则返回该对象接口的指针
    */
   ILxTsspDataBufferManager* GetDataBufferManager();

    /**
     * @brief 取自动关机对象
     * @return 未查找到该对象则返回NULL，找到则返回该对象接口的指针
     */
    ILxTsspAutoShutDown* GetAutoShutDown();

protected:
    static LxTsspObjectManager* _Instance;

private:
    /// 公共模块的对象名称和对象的映射表
    QMap<quint64, void*> m_CommObjectList;

    /// 和模式相关的对象名称和对象的映射表
    QMap<quint64, void*> m_ModeObjectList;

    ILxTsspPublicInfoManager* m_pPublicInfo;

protected:
    //对象信息
    TLxTsspObjectInfo m_ObjectInfo;
    //错误列表
    QMap<int, QString> m_ErrInfoList;
    //错误代码
    int m_iErrCode;
    //参数列表
    QMap<quint64, QByteArray> m_ParamList;
    //状态列表
    QMap<quint64, TLxTsspDeviceState> m_StateList;

}; // LxTsspObjectManager

#endif // LXTSSPCOMMONDEFINITION_H

