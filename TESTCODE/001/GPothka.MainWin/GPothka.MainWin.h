
// GPothka.MainWin.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CGPothkaMainWinApp:
// �йش����ʵ�֣������ GPothka.MainWin.cpp
//

class CGPothkaMainWinApp : public CWinApp
{
public:
	CGPothkaMainWinApp();
	ULONG_PTR m_GdiplusToken;

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CGPothkaMainWinApp theApp;