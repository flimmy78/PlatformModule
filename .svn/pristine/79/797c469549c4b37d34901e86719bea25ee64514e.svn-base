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
  */
typedef struct
{
    char ak[2];     // ak[0]: ak_A ak[1]:ak_B, 0 未链接  1:FM 2:ZZM
    char acu;       // 0 未链接, 1链接
    char dte;       // 0 未链接, 1链接
    char timer;
    char net_switch;
    char down_alter[3]; //
    char up_alter;
    char power;
    char bbe[3];    // 0 未链接  1:FM 2:BPSK 4:SS
    char record[3]; // 检前1，2，3：0未链接 1链接
    char spectrum;  // 频谱仪
} DevicesConnectStatus;

#pragma pack()
#endif // DCU_COMMON_DATASTRUCT_H
