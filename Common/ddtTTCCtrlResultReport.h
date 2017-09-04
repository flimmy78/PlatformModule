//这个文件中的内容是数传与TTC控制结果上报的结构体。
//因为结构文件将其放在一起，所以也就将他们全部放入这个文件中。
#ifndef _DDT_TTCCTRLRESULTREPORT_H_
#define _DDT_TTCCTRLRESULTREPORT_H_
#include "TypeDefine.h"
#define FILE_NAME_LEN 256
#define NUM_PAIRS 100
#pragma pack(1)
//基带分系统公共命令{
typedef struct
{
    UCHAR change;
    UCHAR valid;
}IOCHANGE;//内外时统切换105
typedef struct
{
    UCHAR type;
    UINT IF_FRE;
    CHAR valid;
}IF_FRESET;//中频频率设置106
typedef struct
{

    int longitude;
    int latitude;
    CHAR valid;
}SITEBINDING;//站址装订(慎用)113
typedef struct
{
    IOCHANGE innerOuterChange105;
    IF_FRESET if_fre106;
    SITEBINDING siteBinding113;
}PCRCOMM;
//}基带分系统公共命令


typedef struct
{
    UINT id;//任务标识
    UCHAR taskDescribe[6];//任务代号
    CHAR valid;
}TCISSUE;//task code issue任务代码下发.(1)

typedef struct
{
    UCHAR sel;//通道选择
    CHAR valid;
}TCSEL;//track channel selection跟踪通道选择(2)

typedef struct
{
    UINT id;//任务标识
    DWORDLONG upFreq;//上行频率
    DWORDLONG downFreq;//下行频率
    UINT distanceTo0;//应答机距离零值
    CHAR valid;
}TZBOUND;//Transponder zero bound应答机零值装订(3)
typedef struct
{
    UINT id;
    UINT64 deviceID;
    DWORDLONG upFreq;//上行频率
    DWORDLONG downFreq;//下行频率
    UINT attachID1;
    UINT attachID2;
    UINT meanValueTo0;//距离零值均值
    UINT varianceTo0;//距离零值方差
    CHAR valid;
}EQUIPMENTZERO;//设备校零(4)
typedef struct
{
        UINT id;//任务标识
        UINT date;
        UINT time;
        UINT aTrackSemi_major;//a轨道半长轴
        UINT eTrackEccentricityRatio;//e轨道偏心率
        UINT iTrackInclination;//i轨道倾角
        int omegaLowerCaseRAAN;//right ascension of ascending node,RAAN, omega升交点赤经
        int omegaUpperCaseAP;//argument of perigee近地点幅角
        int MMeanAnomaly;//mean anomaly平近点角
        CHAR valid;
}NTISSUE;//number of track issue轨道根数下发(监控)(8)
typedef struct
{
    UCHAR lateralSoundChoice;//Lateral sound choice 侧音选择
    UCHAR sendStatu;//发送状态
    CHAR valid;
}DMSTONE;//distance measurement single tone测距发单音(17)
typedef struct
{
    UINT subcarrierFrequency;//副载波频率
    UCHAR  sendStatu;//发送状态
    CHAR valid;
}TCSTONE;//telecontrol single tone遥控发单音(18)
typedef struct
{
    UINT subcarrierFrequency;//副载波频率
    UCHAR  sendStatu;//发送状态
    CHAR valid;
}TSSSTONE;//telemetering simulated source single tone遥测模拟源发单音(20)
typedef struct
{
    UCHAR tstStatu;
    UCHAR directionOfTurning;
    UINT doubleCatchTime;
    UCHAR valid;
}DRTTST;//Double catch time test双捕时间测试(21)
typedef struct
{
    UCHAR tstStatu;
    UCHAR directionOfTurning;
    UINT timeToCatch;
    UCHAR valid;
}TTCTST;//time to catch test距捕时间测试(22)
typedef struct
{
    UCHAR tstStatu;
    UCHAR directionOfTurning;
    UINT stdTTCRevCatchTime;
    UCHAR valid;
}STRCTST;//标准TTC接收捕获测试 std TTC rev catch test;(23)
//typedef struct
//{
//}SCTST;//扫描特性测试scan charactor test(26)
typedef struct
{
    UCHAR tstStatus;//测试状态
    UCHAR directionOfTurning;//动态范围
    UINT freDynamicRange;
    UCHAR valid;
}STDRTST;//std TTC dynamic range test标准TTC/动态范围测试(31)
typedef struct
{
    UINT catchTime;
    UCHAR valid;
}DTRCTTST;//(32)数传接收捕获时间测试DATA TRANS Rev catch time test
typedef struct
{
    UCHAR tstStatus;//测试状态
    UINT range;//数传频率动态范围
    UCHAR valid;
}DFDRTST;//Digital frequency dynamic range test数传频率动态范围测试(37)

typedef struct
{
    UCHAR type;//类别
    UCHAR rotation;//旋向
    UCHAR sstat;//统计状态
    UINT totalCod;//总码元数
    UINT errCod;//误码数
    UINT totalFrm;//总帧数
    UINT errFrm;//误帧数
    UCHAR valid;
}RTEFSTAT;//Real time error frame statistaics实时误码帧统计(38)

typedef struct
{
    UCHAR type;//类别
    UCHAR rotation;//旋向
    UCHAR sstat;//测试状态
    UINT totalCod;//总码元数
    UINT errCod;//误码数
    UCHAR valid;
}BERTST;//误码率测试bit error rate test;(39)

typedef struct
{
    UCHAR type;//类别
    UCHAR rotation;//旋向
    UCHAR sstat;//测试状态
    UINT totalCod;//总帧元数
    UINT errCod;//误帧数
    UCHAR valid;
}FERTST;//Frame error rate test误帧率测试(40)

typedef struct
{
    UCHAR tstCtl;//测试状态
    UINT totalIns;//测试总指令数
    UINT errIns;//测试误指令数
    UCHAR valid;
}RCSREITST;//Remote control small ring error instruction test遥控小环误指令测试(41)

typedef struct
{
    UCHAR tstStatu;
    UCHAR directionOfTurning;
    UINT hasStatisticsPoints;
    UINT distanceMeasurementMeanValue;
    UINT distanceMeasurementRandomDiffer;
    UCHAR valid;
}DMRETST;//(43)测距随机误差测试distance measurement random error test

typedef struct
{
    UCHAR tstStatu;
    UCHAR directionOfTurning;
    UINT hasStatisticsPoints;
    INT speedMeasurementMeanValue;
    INT speedMeasurementRandomDiffer;
    UCHAR valid;
}SMRETST;//(44)测速随机误差测试speed measurement random error test
typedef struct
{
    UCHAR type;
    UCHAR dopplerSwitch;
    USHORT range;
    SHORT rateOfChange;
    UCHAR valid;
}DOPPLERSIMU;//(49)多普勒模拟Doppler simulation
typedef struct
{
    UCHAR sel;
    UCHAR valid;
}BBDTSSEL;//(50)基带数字测试信号选择base band digital test signal selection
typedef struct
{
    UCHAR chlID;
    UINT timeScale;
    INT value;
    UCHAR dataValid;
    UCHAR valid;
}DMDRRPORT; //(53)distance Measurement data report测距数据上报(报监控)
typedef DMDRRPORT
SMDREPORT;//(54)测速数据上报(报监控)speed measurement data report
typedef struct
{
    CHAR fileName[FILE_NAME_LEN];
    UCHAR valid;
}TTCDSAVE;//(55)TTC数据存盘 Data save
typedef TTCDSAVE DDTDSAVE;//(56)DDT数据存盘 Data save
typedef EQUIPMENTZERO BZCRINQUIRE;//(57)基带校零结果查询 baseband zero calibration result inquire
typedef struct
{
    UCHAR statu;
    UCHAR valid;
}R0DEDUCTION;//(60)responder 0 deduction 应答机0值扣除
typedef struct
{
    UINT value;
    UCHAR valid;
}DFVBINDING;//(61)距离预报值装订distance forecast value binding
typedef struct
{
    UINT64 deviceCombinationID;
    UCHAR valid;
}DCIDISSUE;//(63)device combination ID issue设备组合号下发
typedef struct
{
    UCHAR telecontrolPerformancePeriod;
    UCHAR valid;
}TCPPERIOD;//(65)遥控工作期telecontrol performance period
typedef struct
{
    UCHAR revUnit;
    SHORT presetFreq;
    SHORT catchRange;
    UCHAR valid;
}DPFPCRANGE;//(66)下行多普勒频率预置和捕获范围 down doppler freq preset catch range
//(67){{
typedef struct
{
    USHORT AGC;
    USHORT S_phi;
}AGCCBPAIR;// AGC curve binding pair

typedef struct
{
    UINT64 deviceCombinationID;
    UINT64 downFreq;
    UCHAR reponderSel;
    UCHAR numOfCurvePoints;
    AGCCBPAIR pairs[NUM_PAIRS];
    UCHAR valid;
}AGCCBIND;//}}(67)AGC曲线装订 AGC curve binding

typedef struct
{
    TCISSUE tcIssue1;
    TCSEL tcSel2;
    TZBOUND tzBound3;
    EQUIPMENTZERO eZero4;
    NTISSUE ntIssue8;
    DMSTONE dmStone17;
    TCSTONE tcsTone18;
    TSSSTONE tsssTone20;
    DRTTST drtTst21;
    TTCTST ttcTst22;
    STRCTST strcTst23;
    STDRTST stdrTst31;
    DTRCTTST dtrctTst32;
    DFDRTST dfdrTst37;
    RTEFSTAT rtefStat38;
    BERTST berTst39;
    FERTST ferTst40;
    RCSREITST rcsreiTst41;
    DMRETST dmreTst43;
    SMRETST smreTst44;
    DOPPLERSIMU dSimu49;
    BBDTSSEL bbdtsSel50;
    DMDRRPORT dmdrrPort53;
    SMDREPORT smdrePort54;
    TTCDSAVE ttcdSave55;
    DDTDSAVE ddtdSave56;
    BZCRINQUIRE bzcrInquire57;
    R0DEDUCTION r0Decuction60;
    DFVBINDING dfvBind61;
    DCIDISSUE dcIDIssue63;
    TCPPERIOD tcpPeriod65;
    DPFPCRANGE dpfpcRange66;
    AGCCBIND AGCcBind67;
}PCRDATATRANS_TTC;//数传和USB过程控制结果上报

#pragma pack()
#endif
