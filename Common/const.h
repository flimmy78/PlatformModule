#ifndef CONST_H
#define CONST_H

enum DEV:char{
    DOWNNetSwitch,  //下行开关矩阵
    UPNetSwitch,//上行开关矩阵
    DDTSMCU1,
    TCSMCU1,
    SS1SMCU1,
    SS2SMCU1,
    USBSMCU1,
    DDTSMCU2,
    TCSMCU2,
    SS1SMCU2,
    SS2SMCU2,
    USBSMCU2,
    DownAlter1,
    DownAlter2,
    UpAlter1,
    UpAlter2,
    UpAlter,
    LTCSMCU1,
    LTCSMCU2,
    DEV_SMCU,
    DEV_UNKOW
};

enum TYPE_P_M:char{
    PARAM,
    MACRO,
    STATUS
};


#endif
