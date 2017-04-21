#pragma once
//********************************************************************
// ����ʱ�䣺  2016/05/12  14:42 
// �ļ����ƣ�  GP_Logger.h 
// ��    �ߣ�  GP_Fore
//********************************************************************
// ����˵����  �ṩд��־���ܣ�֧�ֶ��̣߳�֧�ֿɱ��β���������֧��д��־���������
// ��    �ڣ�  SetLogLevel������д��־����
//			   TraceKeyInfo��������־����д�ؼ���Ϣ
//             TraceError��д������Ϣ
//             TraceWarning��д������Ϣ
//             TraceInfo��дһ����Ϣ
// ��    ע��  ������������Ļ����£�����ʹ��setlocale(LC_ALL, "chs");�Ƚ��б��ػ����á�
//********************************************************************
#ifndef GP_LOGGER_H_
#define GP_LOGGER_H_
#include <Windows.h>
#include <stdio.h>
#include "GP_BaseObject.h"
#include <typeinfo.h>


//��־ģ�鹫����������
#ifndef GP_LOGGER_DEFINE  
#define GP_LOGGER_DEFINE

#define WM_USERDEFINEWINDOWSINFO WM_USER+2000 //֧��windows��Ϣ���ơ�

//��־�������ʾ��Ϣ
//static const TCHAR * KEYINFOPREFIX = _T(" Key:");     //�ؼ���־ǰ׺
//static const TCHAR * ERRORPREFIX = _T(" Error: ");    //������־ǰ׺
//static const TCHAR * WARNINGPREFIX = _T(" Warning:"); //������־ǰ׺
//static const TCHAR * INFOPREFIX = _T(" Info: ");      //һ����־ǰ׺
//static const int MAX_STR_LEN = 1024;               //��󻺴档



#endif


//��־����
typedef enum EnumLogType
{
	ErrInfo=0,      //������־��Ϣ
	WarnInfo,       //������־��Ϣ
	KeyInfo,        //һ����־��Ϣ
	DataInfo        //���ݸ�����Ϣ
};

//typedef enum EnumLogLevel
//{
//	LogLevelAll = 0,    //������Ϣ��д��־
//	LogLevelMid,        //д���󡢾�����Ϣ
//	LogLevelNormal,     //ֻд������Ϣ
//	LogLevelStop        //��д��־
//};

//static EnumLogType ErrLog = ErrInfo;

class GP_DLL_API GP_Logger
{
private:
	//Ĭ�Ϲ��캯��
	GP_Logger();
	//��������
	virtual ~GP_Logger();
	//д�ļ�����
	void WriteStrToTxt(const TCHAR * strInfo);
	//��ȡ��ǰϵͳʱ��
	TCHAR * GetCurrentTimeToTchar();
	//������־�ļ�����
	void GenerateLogName();

public:

	//�����ռǼ�¼�ļ���
	EnumLogLevel m_nLogLevel;

	void TraceLogger(EnumLogType pLogType, const TCHAR * strInfo, ...);
	//������־�ļ�����Ŀ¼
	void UpdateLogDirectory(const TCHAR * strDirectoryPath);
	//����Ϣ���͵�ָ����windows����
	void SendInfoToWindows(HWND hWnd, const TCHAR * strInfo, ...);
	//��ȡʵ������
	static GP_Logger* GetInstance();
private:
	//д��־�ļ���
	FILE * m_pFileStream=NULL;
	//��־��·��
	TCHAR m_LogDirectoryPath[MAX_STR_LEN];
	//��־������
	TCHAR m_strCurLogName[MAX_STR_LEN];
	//�߳�ͬ�����ٽ�������
	CRITICAL_SECTION m_cs;
	//��ǰʵ����ָ̬��
	static GP_Logger* logger;
	//��ֹ��������͸�ֵ����
	GP_Logger(const GP_Logger&);
	GP_Logger& operator=(const GP_Logger&);
};

#endif


