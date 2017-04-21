#pragma once
//********************************************************************
// ����ʱ�䣺  2016/05/05  19:42 
// �ļ����ƣ�  GP_Thread.h 
// ��    �ߣ�  GP_Fore
//********************************************************************
// ����˵��    �Զ����̻߳������
// ��    ע��  http://www.cppblog.com/Chosen/archive/2013/10/07/203567.aspx
//********************************************************************
#ifndef GP_THREAD_H_
#define GP_THREAD_H_
#include <Windows.h>
#include <process.h>
#include <wtypes.h>
#include "GP_BaseObject.h"

class GP_DLL_API GP_Thread
{
public:
	GP_Thread();
	virtual ~GP_Thread();
	bool m_TheadRun;
	
	HANDLE m_ThreadHandle;
	int StartTask();
	int TerminateTask();
	static unsigned __stdcall ThreadFunc(void *para);
protected:
	virtual bool ExecuteTask() = 0;
};

#endif

