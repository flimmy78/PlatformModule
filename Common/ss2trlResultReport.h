#ifndef _SS2TRLRESULTREPORT_H_
#define _SS2TRLRESULTREPORT_H_
#include "ss1trlResultReport.h"
#pragma pack(1)

typedef ss1TCISSUE ss2TCISSUE;//(1)task code issue任务代码下发.(1)

typedef ss1TZBOUND ss2TZBOUND;//Transponder zero bound应答机零值装订(3)

typedef ss1EQUIPMENTZERO ss2EQUIPMENTZERO;//设备校零(4)

typedef ss1NTISSUE ss2NTISSUE;//number of track issue轨道根数下发(监控)(8)

typedef SSTTCRCTTST ss2SSTTCRCTTST;//(11)扩频TTC接收捕获时间测试  spread spectrum TTC receive catch time test

typedef SSTDRTST ss2SSTDRTST;//(18)spread spectrum  TTC dynamic range test扩频TTC/动态范围测试

typedef ss1RTEFSTAT ss2RTEFSTAT;//(19)Real time error frame statistaics实时误码帧统计

typedef ss1BERTST ss2BERTST;//误码率测试bit error rate test;(20)

typedef ss1FERTST ss2FERTST;//Frame error rate test误帧率测试(21)

typedef ss1RCSREITST ss2RCSREITST;//Remote control small ring error instruction test遥控小环误指令测试(22)

typedef ss1DMRETST ss2DMRETST;//(23)测距随机误差测试distance measurement random error test

typedef ss1DMRETST ss2SMRETST;//(24)测速随机误差测试speed measurement random error test

typedef ss1DOPPLERSIMU ss2DOPPLERSIMU;//(26)多普勒模拟Doppler simulation

typedef ss1BBDTSSEL ss2BBDTSSEL;//(27)基带数字测试信号选择base band digital test signal selection

typedef ss1DMDRRPORT ss2DMDRRPORT;//(29)distance Measurement data report测距数据上报(报监控)

typedef ss1SMDREPORT ss2SMDREPORT;//(30)测速数据上报(报监控)speed measurement data report

typedef struct
{
    UINT tId;
    ss1TTCDSAVE TTCdSave;
}ss2TTCDSAVE;//(31)TTC数据存盘 Data save

typedef ss1BZCRINQUIRE ss2BZCRINQUIRE;//(32)基带校零结果查询 baseband zero calibration result inquire

typedef ss1R0DEDUCTION ss2R0DEDUCTION;//(35)responder 0 deduction 应答机0值扣除

typedef ss1DPFPCRANGE ss2DPFPCRANGE;//(38)下行多普勒频率预置和捕获范围 down doppler freq preset catch range

typedef struct
{
    UINT64 deviceCombinationID;
    UINT64 downFreq;
    UCHAR chl;
    UCHAR sel;
    UCHAR reponderSel;
    UCHAR numOfCurvePoints;
    AGCCBPAIR pairs[NUM_PAIRS];
    UCHAR valid;
}ss2AGCCBIND;//(39)AGC曲线装订 AGC curve binding

typedef DCCTRL ss2DCCTRL;//(40)多普勒补偿控制doppler compensation control

typedef struct
{
    ss2TCISSUE tcIssue1;
    ss2TZBOUND tzBound3;
    ss2EQUIPMENTZERO eZero4;
    ss2NTISSUE ntIssue8;
    ss2SSTTCRCTTST rctTst11[2][4];
    ss2SSTDRTST ssTone18[2][4];
    ss2RTEFSTAT drtTst19;
    ss2BERTST ttcTst20;
    ss2FERTST strcTst21;
    ss2RCSREITST rcsreiTst22;
    ss2DMRETST dmreTst23;
    ss2SMRETST smreTst24;
    ss2DOPPLERSIMU dSimu26;
    ss2BBDTSSEL bbdtsSel27;
    ss2DMDRRPORT dmdrrPort29;
    ss2SMDREPORT smdrePort30;
    ss2TTCDSAVE ttcdSave31;
    ss2BZCRINQUIRE bzcrInquire32;
    ss2R0DEDUCTION r0Decuction35;
    ss2DPFPCRANGE dpfpcRange38;
    ss2AGCCBIND AGCcBind39;
    DCCTRL dcCtrl40;
}PCRSS2;//ss1过程控制命令上报
#pragma pack()
#endif
