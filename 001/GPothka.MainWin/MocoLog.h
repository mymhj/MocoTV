#pragma once

#include "stdafx.h"
#include "windows.h"


enum MsgType
{
	Info, Warn, Error, Debug
};
class MocoLog
{
public:
	MocoLog();
	~MocoLog();

	UINT Init(const TCHAR* pLogFileName);

	UINT CreateLogFile(HANDLE &hFile);

	UINT StatusOut(MsgType mt, const TCHAR* fmt, ...);

private:
	HANDLE m_hFile;
	TCHAR m_LogFile[MAX_PATH];
	CRITICAL_SECTION m_LogCriSection;

};

