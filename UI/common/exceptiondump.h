#ifndef EXCEPTIONDUMP_H
#define EXCEPTIONDUMP_H

#include <Windows.h>
#include <dbghelp.h>
#include <shellapi.h>
#include <shlobj.h>
#include <QDateTime>
#include <QProcess>
#include "CommonFun.h"
#include "EIni.h"
#include "ELog.h"
#include "FilePathManage.h"
#include "EServerIni.h"

#pragma comment( lib, "DbgHelp")

static LONG WINAPI MyUnhandledExceptionFilter(struct _EXCEPTION_POINTERS* ExceptionInfo)
{
    ELog::instance()->writeDebugLog(QStringLiteral("发生了异常，结束程序。"));
    //生成Dump文件名
    QString strFileName(FilePathManage::instance()->getDumpPath());
    strFileName += "_v";
    strFileName += QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");
    strFileName += "_id_";
    strFileName += QString::number(EIni::getUseId());
    strFileName += ".dmp";

    auto stdStr = strFileName.toStdWString();
    HANDLE lhDumpFile = CreateFile(stdStr.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);//L"TT_DumpFile.dmp"
    MINIDUMP_EXCEPTION_INFORMATION loExceptionInfo;
    loExceptionInfo.ExceptionPointers = ExceptionInfo;
    loExceptionInfo.ThreadId = GetCurrentThreadId();
    loExceptionInfo.ClientPointers = TRUE;

    MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),lhDumpFile, MiniDumpNormal, &loExceptionInfo, NULL, NULL);
    CloseHandle(lhDumpFile);

    QStringList arguments;
    arguments.append(strFileName);
    if(EServerIni::getServerModel() == testModel)
    {
        arguments.append("ac-dev");
    }
    else if(EServerIni::getServerModel() == releaseModel)
    {
        arguments.append("ac-prod");
    }

    QProcess process;
    bool ret = process.startDetached(FilePathManage::instance()->getUpdataDumpFilePath(),arguments);
    ELog::instance()->writeDebugLog("ret="+QString::number(ret)+"  "+FilePathManage::instance()->getUpdataDumpFilePath()+ ";" + arguments.at(0)+ ";" + arguments.at(1));
    return EXCEPTION_EXECUTE_HANDLER;
}

#endif // EXCEPTIONDUMP_H

