#ifndef DCU_COMMON_DATASTRUCT_H
#define DCU_COMMON_DATASTRUCT_H
#pragma pack(1)
/*
ak -|主 FM ZZM
     |
     |备 FM ZZM

     ACU

     DTE

     Timer

     NetSwitch

     DownAlter 3

     UpAlter

     Power

     BBE 3 FM BPSK SS

     record 3

     spectrum
     带有监控字样的，需要设备接口监控设备联接状态
  */

typedef struct
{
    char ak[2];     // ak[0]: ak_A ak[1]:ak_B, 0 未链接  1:FM 2:ZZM
    char acu;       // 0 未链接, 1链接
    char dte;       // 0 未链接, 1链接
    char timer;
    char Rsv[6];//保留
    //char net_switch;
    //char down_alter[3]; //
    //char up_alter;
    //char power;
    char bbe[3];    // 0 未链接  1:FM 2:BPSK 4:SS
    char record[3]; // 检前1，2，3：0未链接 1链接
    char spectrum;  // 频谱仪
    char upnet_switch;
    char downnet_switch;
    char testnet_switch;
    char slDivcom;
    char sSwitch;
    char down_alter[3]; //
    char up_alter[3];
    char adjustup_alter;
    char freqB;
    char zeroSTD;
    char RSP;// 1:USB 2:SS2
    char Rsv1;
    char spower;
    char lpower;
    char testU;
    char rClock;
    char bbeControl[3];//基带是本控(1)还是分控(0)
} DevicesConnectStatus;


#pragma pack()
#endif // DCU_COMMON_DATASTRUCT_H
