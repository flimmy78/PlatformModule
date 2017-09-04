#ifndef _SS1TRLRESULTREPORT_H_
#define _SS1TRLRESULTREPORT_H_
#include "ddtTTCCtrlResultReport.h"
#pragma pack(1)
typedef struct
{
    UCHAR sel;
    TCISSUE content;//内容一协
}ss1TCISSUE;//(1)task code issue任务代码下发.(1)

typedef TZBOUND ss1TZBOUND;//Transponder zero bound应答机零值装订(3)

typedef struct
{
    UCHAR sel;
    EQUIPMENTZERO qz0;
}ss1EQUIPMENTZERO;//设备校零(4)

typedef NTISSUE ss1NTISSUE;//number of track issue轨道根数下发(监控)(8)

typedef struct
{
    UCHAR statu;
    UCHAR directionOfTurning;   //-1 作为左边索引  值为1-2
    UCHAR chlType;
    UCHAR chl;                 //-1 作为右边的索引 值为1-4
    UINT time;
    UCHAR valid;
}SSTTCRCTTST;//(13)扩频TTC接收捕获时间测试  spread spectrum TTC receive catch time test

typedef struct
{
    UCHAR statu;
    UCHAR directionOfTurning;
    USHORT TTC5Hz;
    USHORT TTC10Hz;
    USHORT TTC100Hz;
    USHORT TTC500Hz;
    USHORT TTC1KHz;
}SSTTCTCLBANDWITH;//(16)扩频TTC载波环路带宽 spread spectrum TTC The carrier loop bandwidth

////捕获范围
////no=20 SS1  no=18  SS2
//typedef struct
//{
//    UCHAR ucCtl;//1开始 2终止
//    UCHAR ucLR;//旋向 1-左旋  2-右旋
//    UCHAR ucChnael;// 1-遥测  2-测距
//    UCHAR ucTarget;//通道号1-4 通道1~4
//}SS12CapRangeTestCmd;

typedef struct
{
    UCHAR tstStatus;//测试状态
    UCHAR directionOfTurning;//动态范围
    UCHAR chlType;
    UCHAR chl;
    UINT freDynamicRange;
    UCHAR valid;
}SSTDRTST;//(20)spread spectrum  TTC dynamic range test扩频TTC/动态范围测试

typedef struct
{
    UCHAR type;//类别
    UCHAR rotation;//旋向
    UCHAR chl;
    UCHAR sstat;//统计状态
    UINT totalCod;//总码元数
    UINT errCod;//误码数
    UINT totalFrm;//总帧数
    UINT errFrm;//误帧数
    UCHAR valid;
}ss1RTEFSTAT;//(21)Real time error frame statistaics实时误码帧统计

typedef struct
{
    UCHAR type;//类别
    UCHAR rotation;//旋向
    UCHAR chl;
    UCHAR sstat;//测试状态
    UINT totalCod;//总码元数
    UINT errCod;//误码数
    UCHAR valid;
}ss1BERTST;//误码率测试bit error rate test;(22)

typedef struct
{
    UCHAR type;//类别
    UCHAR rotation;//旋向
    UCHAR chl;
    UCHAR sstat;//测试状态
    UINT totalCod;//总帧元数
    UINT errCod;//误帧数
    UCHAR valid;
}ss1FERTST;//Frame error rate test误帧率测试(23)

typedef struct
{
    UCHAR tstCtl;//测试状态
    UCHAR chl;
    UINT totalIns;//测试总指令数
    UINT errIns;//测试误指令数
    UCHAR valid;
}ss1RCSREITST;//Remote control small ring error instruction test遥控小环误指令测试(24)

typedef struct
{
    UCHAR tstStatu;
    UCHAR directionOfTurning;
    UCHAR chl;
    UINT hasStatisticsPoints;
    UINT meanValue;
    UINT randomDiffer;
    UCHAR valid;
}ss1DMRETST;//(25)测距随机误差测试distance measurement random error test


typedef ss1DMRETST ss1SMRETST;//(26)测速随机误差测试speed measurement random error test

typedef struct
{
    UCHAR chl;
    UCHAR dopplerSwitch;
    USHORT range;
    SHORT rateOfChange;
    UCHAR PCode;
    UCHAR valid;
}ss1DOPPLERSIMU;//(27)多普勒模拟Doppler simulation

typedef BBDTSSEL ss1BBDTSSEL;//(28)基带数字测试信号选择base band digital test signal selection

typedef DMDRRPORT ss1DMDRRPORT;//(30)distance Measurement data report测距数据上报(报监控)

typedef DMDRRPORT ss1SMDREPORT;//(31)测速数据上报(报监控)speed measurement data report

typedef TTCDSAVE ss1TTCDSAVE;//(32)TTC数据存盘 Data save

typedef struct
{
    UCHAR chl;
    EQUIPMENTZERO eZero;
}ss1BZCRINQUIRE;//(33)基带校零结果查询 baseband zero calibration result inquire

typedef R0DEDUCTION ss1R0DEDUCTION;//(36)responder 0 deduction 应答机0值扣除

typedef struct
{
    UCHAR chl;
    DPFPCRANGE dCR;
}ss1DPFPCRANGE;//(39)下行多普勒频率预置和捕获范围 down doppler freq preset catch range

typedef struct
{
    UINT64 deviceCombinationID;
    UCHAR chl;
    UINT64 downFreq;
    UCHAR reponderSel;
    UCHAR numOfCurvePoints;
    AGCCBPAIR pairs[NUM_PAIRS];
    UCHAR valid;
}ss1AGCCBIND;//(40)AGC曲线装订 AGC curve binding

typedef struct
{
    UCHAR chl;
    UCHAR statu;
}DCCTRL;//(41)多普勒补偿控制doppler compensation control

typedef struct
{
    ss1TCISSUE tcIssue1;
    ss1TZBOUND tzBound3;
    ss1EQUIPMENTZERO eZero4;
    ss1NTISSUE ntIssue8;
    SSTTCRCTTST rctTst13[2][4];
    SSTTCTCLBANDWITH clBandwidth16;
    SSTDRTST ssTone20[2][4];
    ss1RTEFSTAT drtTst21;
    ss1BERTST ttcTst22;
    ss1FERTST strcTst23;
    ss1RCSREITST rcsreiTst24;
    ss1DMRETST dmreTst25;
    ss1SMRETST smreTst26;
    ss1DOPPLERSIMU dSimu27;
    ss1BBDTSSEL bbdtsSel28;
    ss1DMDRRPORT dmdrrPort30;
    ss1SMDREPORT smdrePort31;
    ss1TTCDSAVE ttcdSave32;
    ss1BZCRINQUIRE bzcrInquire33;
    ss1R0DEDUCTION r0Decuction36;
    ss1DPFPCRANGE dpfpcRange39;
    ss1AGCCBIND AGCcBind40;
    DCCTRL dcCtrl41;
}PCRSS1;//ss1过程控制命令上报
#pragma pack()
#endif
