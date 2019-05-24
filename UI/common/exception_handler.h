#pragma once
#include <dbghelp.h>   
#include <shellapi.h>   
#include <shlobj.h>
#include <QSettings>
#include "roomcommondatamgr.h"

#pragma comment( lib, "DbgHelp")

static LONG WINAPI MyUnhandledExceptionFilter(struct _EXCEPTION_POINTERS* ExceptionInfo)
{
	//获取路径
	QString strRunPath = QCoreApplication::applicationDirPath();
	strRunPath += "/readme.txt";
	QSettings setting(strRunPath, QSettings::IniFormat);
	QString version = setting.value("data/version").toString();

	//获取当前时间
	QDateTime curDateTime = QDateTime::currentDateTime();
	QString strCurDateTime = curDateTime.toString("yyyy-MM-dd_hh-mm-ss-dd");//TT_DumpFile__v1.0.3.27_2015-11-22_21-06-30-周日_155

	//当前用户ID
	SHOW_USER_INFO userInfo;
	RoomCommonDataMgr::instance()->getCurrentUserInfo(userInfo);
	qint64 nUserID = userInfo.m_nUserID;
	QString strUserID = QString("%1").arg(nUserID);

	//生成Dump文件名
	QString strFileName("TT");
	strFileName += "_v";
	strFileName += version;
	strFileName += "_t";
	strFileName += strCurDateTime;
	strFileName += "_u";
	strFileName += strUserID;
	strFileName += ".dmp";

	HANDLE lhDumpFile = CreateFile(strFileName.toStdWString().c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);//L"TT_DumpFile.dmp"
	MINIDUMP_EXCEPTION_INFORMATION loExceptionInfo;
	loExceptionInfo.ExceptionPointers = ExceptionInfo;
	loExceptionInfo.ThreadId = GetCurrentThreadId();
	loExceptionInfo.ClientPointers = TRUE;

	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),lhDumpFile, MiniDumpNormal, &loExceptionInfo, NULL, NULL);
	CloseHandle(lhDumpFile);
	return EXCEPTION_EXECUTE_HANDLER;
}