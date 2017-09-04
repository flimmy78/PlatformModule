/********************************************************
* Copyright (C), 2016-2017, Yinovation
* File name : Log.cpp
* Author : 孙冀川  Date ：2017/03/18
* Description : 本文件用于Log相关的函数的实现。
* Version : 版本信息
* History ： 
*       Date      Author      Version      Modify
********************************************************/
#include "Log.h"
#include <QDateTime>
#include "LxTsspCommonDefinition.h"

void CLog::addLog(QString strInfo, int level)
{
    //日志信息
    TLxTsspLog tmpLog;
    QString strLog;
    memset(&tmpLog, 0, sizeof(TLxTsspLog));
    tmpLog.ucWarningLevel = level;
    strLog += strInfo;
    strcpy(tmpLog.szContent, strLog.toUtf8().data());
    tmpLog.time = QDateTime::currentDateTime().toTime_t();
    LxTsspObjectManager *objMgr = LxTsspObjectManager::Instance();
    if(!objMgr)
        return;
    ILxTsspLogManager *logMgr = objMgr->GetLogManager();
    if (!logMgr)
        return;
    //logMgr->Add(tmpLog);
}
