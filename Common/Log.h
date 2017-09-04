/********************************************************
* Copyright (C), 2016-2017, Yinovation
* File name : Log.h
* Author : 孙冀川  Date ：2017/03/18
* Description : 本文件用于Log相关的函数的声明。
* Other : 其它内容说明
*********************************************************/
#ifndef _LOG_H_
#define _LOG_H_

#include <QString>
class CLog
{
public:
    //添加信息到平台日志文件
    //日志级别 ： 提示(1)、告警(2)、错误(3)
    static void addLog(QString strInfo, int level = 1);
};

#endif // _LOG_H_
