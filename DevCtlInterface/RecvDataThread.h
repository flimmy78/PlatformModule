#ifndef RECVDATATHREAD_H
#define RECVDATATHREAD_H

#include <QDateTime>
#include <QThread>
#include <QUdpSocket>
#include "../Common/LxTsspCommonDefinition.h"
#include "../Common/TypeDefine.h"
#include "../Common/DataStruct2311.h"
#include "DataStructTTCDevCtlInterface.h"

#define MAX_RECV_BUFFER  (1024 * 8)     //最大数据长度 存在数据协议中有个包大概在7K左右

//通道处理以及函数结构体
typedef struct _tag_channelProcessor
{
    QString chlId;
    ICommunicationChannel *pSendChl;
    void (*fpProcessFunction)(QByteArray&);
}CHLPRSOR;

class RecvDataThread : public QThread
{
    Q_OBJECT
public:
    explicit RecvDataThread(QObject *parent = 0);
    ~RecvDataThread();

    void run();
    int SetPara(int num, int len);
    void setStop();

public slots:
    void ReadData();

private:
    bool isProcessControlCMD();
    bool isUnitParamSetCMD();
    bool isUnitParamSetCMD_TTC();
    bool isControlCMDResponse();
    bool isStatusInquiry();
    bool isControlResultReport();

    //区分具体的协议 判断是协议1还是协议2
    int checkProcotol(QByteArray& datagram);
    //bool isSwMat(QByteArray& datagram);
    //bool isBaseBound(QByteArray& datagram) { return true; }

    QVector<ICommunicationChannel*> m_ComChannelVec;
    QUdpSocket * m_pSocket;     //测试使用的UDP通道
    BOOL m_bStop;   //停止线程变量使用
    LxTsspObjectManager* m_pObjectManager;
    ILxTsspCommunicationManager *m_pCommunicationMgr;
    UCHAR *m_pDataBuffer;   //用于接收数据使用的临时缓存
    void updateDev(ICommunicationChannel* pChl);//用于处理设备联接信息


public:
    //初始化相关的通道
    bool initChlProcessor();
    //
    void processDatagram(QByteArray& datagram);
    //处理第1中协议数据
    void proceProcotol(QByteArray& datagram);
    //处理第2种协议数据
    void processBaseBound(QByteArray& datagram);
    //控制命令响应
    void processBaseBound_0xF100(char*);

    //控制结果上报
    void processBaseBound_0xF101(char*);
    void processBaseBound_0xF101_dataTrans(char * pData);
    void processBaseBound_0xF101_TTC(char * pData);
    void processBaseBound_0xF101_SS1(char * pData);
    void processBaseBound_0xF101_SS2(char * pData);

    //分机状态信息上报{
    void processBaseBound_0xF104(char*, const int);
    bool processFuncion(const NETSDFRM&);
    static void processBaseBoundDataTrans(BYTE*, const BYTE&);
    static void processBaseBoundTTC(BYTE*, const BYTE&);
    static void processBaseBoundSMCU1(BYTE*, const BYTE&);
    static void processBaseBoundSMCU2(BYTE *, const BYTE &);
    void (*m_pfProcessBaseboundDTSS)(BYTE *, const BYTE &);
//    static void processDownAlter1(QByteArray& datagram);
//    static void processDownAlter2(QByteArray& datagram);
//    static void processDownAlter3(QByteArray& datagram);
//    static void processUpAlter(QByteArray& datagram);
//    static void processPower(QByteArray& datagram);
    static void processNetSwitch(QByteArray& datagram);
//    static void processSMCU(QByteArray& datagram);
//    static void processTimer(QByteArray& datagram);
//    static void processRecord0(QByteArray& datagram);
//    static void processRecord1(QByteArray& datagram);
//    static void processRecord2(QByteArray& datagram);
//    static void processInnerLink(QByteArray& datagram);
    //}分机状态信息上报
private:
    //联试应答机监控模块处理函数
    void processCombTrans(PROCOTOL_PACK& proPack, bool bTTC = true);
    //校零信标一体机模块处理函数
    void processZEROSTDDcu(PROCOTOL_PACK& proPack);
    //射频分路组合接口模块处理函数
    void processSLDDcu(PROCOTOL_PACK& proPack);
    //射频切换开关监控模块处理函数
    void processSswitch(PROCOTOL_PACK& proPack);
    //测试开关矩阵模块处理函数
    void processTestSwitch(PROCOTOL_PACK& proPack);
    //频率基准源监控模块处理函数
    void processFreqBDcu(PROCOTOL_PACK& proPack);
    //铷钟机箱监控模块处理函数
    void processRCLOCKDcu(PROCOTOL_PACK& proPack);
    //中频开关矩阵模块处理函数
    void processNetSwitch(PROCOTOL_PACK& proPack);

    //解析铷钟机箱监控数据 此协议需要特殊处理
    void analyseRCLOCKDcu(UCHAR *pData, unsigned int nLen);
    //处理响应结果
    void processResponse(QString strDeviceName, UCHAR ucAnswerFlag);
    //获取响应返回的信息内容
    QString getAnswerFlag(UCHAR ucAnsFlag);
};

#endif // RECVDATATHREAD_H
