#include "stdafx.h"
#include "MocoLog.h"


MocoLog::MocoLog()
{
	m_hFile = NULL;
	//Init(_T("SBS"));
	InitializeCriticalSection(&m_LogCriSection);
}


MocoLog::~MocoLog()
{
	if (m_hFile)
	{
		CloseHandle(m_hFile);
	}

	DeleteCriticalSection(&m_LogCriSection);
}


UINT MocoLog::Init(const TCHAR* pLogFileName)
{
	UINT ErrorCode;
	TCHAR MocoAppPath[MAX_PATH];
	TCHAR MocoAppDir[MAX_PATH];
	if (!GetModuleFileName(NULL,MocoAppPath,MAX_PATH))
	{
		return GetLastError();
	}
	if (!PathRemoveFileSpec(MocoAppPath))
	{
		return GetLastError();
	}
	_stprintf_s(MocoAppDir, _T("%s\\Log"), MocoAppPath);
	if (!(CreateDirectory(MocoAppDir,NULL)) && (GetLastError() != 183))
	{
		int err = GetLastError();
		AfxMessageBox(_T("日志路径创建失败"));
		return err;
	}
	SYSTEMTIME  SysTime;
	GetLocalTime(&SysTime);
	_stprintf_s(m_LogFile, _T("%s\\%s_%02d%02d%02d_%02d%02d%02d.Log"), MocoAppDir, pLogFileName, SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute, SysTime.wSecond);
	ErrorCode = CreateLogFile(m_hFile);
	if (ErrorCode)
	{
		return ErrorCode;
	}
	return 0;
}


UINT MocoLog::CreateLogFile(HANDLE &hFile)
{
	hFile = CreateFile(m_LogFile,                // name of the write
		GENERIC_WRITE,          // open for writing
		FILE_SHARE_READ,        // do not share
		NULL,                   // default security
		CREATE_ALWAYS,          // overwrite existing
		FILE_ATTRIBUTE_NORMAL,  // normal file
		NULL);                  // no attr. template

	if (hFile == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(_T("创建日志文件失败"));
		return GetLastError();
	}
	return 0;

}

UINT MocoLog::StatusOut(MsgType Msg, const TCHAR* fmt, ...)
{
	static const TCHAR *MT[] = { _T("Info"), _T("Warn"), _T("Error"), _T("Debug") };
	TCHAR MsgBuff[MAX_PATH] = { 0 };

	va_list argptr;
	va_start(argptr, fmt);//_vsntprintf
	int cbBuf = _vsntprintf_s(MsgBuff, MAX_PATH, _TRUNCATE, fmt, argptr) + 1;
	TCHAR *lpBuf = new TCHAR[cbBuf];
	_vsntprintf_s(lpBuf, cbBuf, _TRUNCATE, fmt, argptr);
	va_end(argptr);
	TCHAR MocoLogBuff[MAX_PATH] = { 0 };

	SYSTEMTIME SysTime;
	GetLocalTime(&SysTime);
	_stprintf_s(MocoLogBuff, _T("[%02d-%02d-%02d %02d:%02d:%02d] [%s] %s\r\n"), SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute, SysTime.wSecond,
		MT[Msg], MsgBuff);

	EnterCriticalSection(&m_LogCriSection);

	DWORD ReturnCharNumber;
	if (!WriteFile(m_hFile, MocoLogBuff, _tcslen(MocoLogBuff)*sizeof(TCHAR), &ReturnCharNumber, NULL))
	{
		return FALSE;
	}

	LeaveCriticalSection(&m_LogCriSection);

	return TRUE;
}
