

#include <imagehlp.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include "stdafx.h"
#include "LoggerModule.h"

//#pragma comment(lib, "DbgHelp.lib")

//Ĭ�Ϲ��캯��
Logger::Logger()
{
	//��ʼ��
	memset(m_strLogPath, 0, MAX_STR_LEN);
	memset(m_strCurLogName, 0, MAX_STR_LEN);
	m_pFileStream = NULL;
	//����Ĭ�ϵ�д��־����
	m_nLogLevel = EnumLogLevel::LogLevelNormal;
	//��ʼ���ٽ�������
	InitializeCriticalSection(&m_cs);
	//������־�ļ���
	GenerateLogName();
}

//���캯��
Logger::Logger(const char * strLogPath, EnumLogLevel nLogLevel):m_nLogLevel(nLogLevel)
{
	//��ʼ��
	m_pFileStream = NULL;
	strcpy_s(m_strLogPath, strLogPath);
	InitializeCriticalSection(&m_cs);
	CreateLogPath();
	GenerateLogName();
}


//��������
Logger::~Logger()
{
	//�ͷ��ٽ���
	DeleteCriticalSection(&m_cs);
	//�ر��ļ���
	if(m_pFileStream)
		fclose(m_pFileStream);
}

//д�ؼ���Ϣ�ӿ�
void Logger::TraceKeyInfo(const char * strInfo, ...)
{
	if(!strInfo)
		return;
	char prefix[MAX_STR_LEN]={0};
	char pTemp[MAX_STR_LEN] = {0};
	strcpy_s(prefix, GetCurrentTime());
	strcat_s(prefix, KEYINFOPREFIX);
	//��ȡ�ɱ��β�
	va_list arg_ptr = NULL;
	va_start(arg_ptr, strInfo);
	vsprintf_s(pTemp,sizeof(pTemp),strInfo, arg_ptr);
	strcat_s(prefix, pTemp);
	va_end(arg_ptr);
	//д��־�ļ�
	Trace(prefix);
	arg_ptr = NULL;

}

//д������Ϣ
void Logger::TraceError(const char* strInfo, ...)
{
	//�жϵ�ǰ��д��־����������Ϊ��д��־��������
	if(m_nLogLevel >= EnumLogLevel::LogLevelStop)
		return;
	if(!strInfo)
		return;
	char prefix[MAX_STR_LEN]={0};
	char pTemp[MAX_STR_LEN] = {0};
	strcpy_s(prefix, GetCurrentTime());
	strcat_s(prefix, ERRORPREFIX);
	va_list arg_ptr = NULL;
	va_start(arg_ptr, strInfo);
	vsprintf_s(pTemp,sizeof(pTemp), strInfo, arg_ptr);
	strcat_s(prefix, pTemp);
	va_end(arg_ptr);
	Trace(prefix);
	arg_ptr = NULL;
}

//д������Ϣ
void Logger::TraceWarning(const char * strInfo, ...)
{
	//�жϵ�ǰ��д��־����������Ϊֻд������Ϣ��������
	if(m_nLogLevel >= EnumLogLevel::LogLevelNormal)
		return;
	if(!strInfo)
		return;
		char prefix[MAX_STR_LEN]={0};
	char pTemp[MAX_STR_LEN] = {0};
	strcpy_s(prefix, GetCurrentTime());
	strcat_s(prefix, WARNINGPREFIX);
	va_list arg_ptr = NULL;
	va_start(arg_ptr, strInfo);
	vsprintf_s(pTemp,sizeof(pTemp), strInfo, arg_ptr);
	strcat_s(prefix, pTemp);
	va_end(arg_ptr);
	Trace(prefix);
	arg_ptr = NULL;
}
//дһ����Ϣ
void Logger::TraceInfo(const char * strInfo, ...)
{
	//�жϵ�ǰ��д��־����������ֻд����;�����Ϣ��������
	if(m_nLogLevel >= EnumLogLevel::LogLevelMid)
		return;
	if(!strInfo)
		return;
			char prefix[MAX_STR_LEN]={0};
	char pTemp[MAX_STR_LEN] = {0};
	strcpy_s(prefix, GetCurrentTime());
	strcat_s(prefix,INFOPREFIX);
	va_list arg_ptr = NULL;
	va_start(arg_ptr, strInfo);
	vsprintf_s(pTemp,sizeof(pTemp), strInfo, arg_ptr);
	strcat_s(prefix, pTemp);
	va_end(arg_ptr);
	Trace(prefix);
	arg_ptr = NULL;
}
//д״̬��Ϣ
void Logger::TraceState(HWND hWnd,const char * strInfo, ...){
	//�жϵ�ǰ��д��־����������ֻд����;�����Ϣ��������
	if(!strInfo)
		return;
	char prefix[MAX_STR_LEN]={0};
	char pTemp[MAX_STR_LEN] = {0};
	strcpy_s(prefix, GetCurrentTime());
	strcat_s(prefix,INFOPREFIX);
	va_list arg_ptr = NULL;
	va_start(arg_ptr, strInfo);
	vsprintf_s(pTemp,sizeof(pTemp), strInfo, arg_ptr);
	strcat_s(prefix, pTemp);
	va_end(arg_ptr);
	::SendMessage(hWnd,WM_USERLOGGRR,0,(LPARAM)&prefix); //ͬ��
	arg_ptr = NULL;
}


//��ȡϵͳ��ǰʱ��
char * Logger::GetCurrentTime()
{
	time_t curTime;
	tm pTimeInfo;
	time(&curTime);
	localtime_s(&pTimeInfo,&curTime);
	char temp[MAX_STR_LEN] = {0};
	sprintf_s(temp, "%02d:%02d:%02d", pTimeInfo.tm_hour, pTimeInfo.tm_min, pTimeInfo.tm_sec);
	char * pTemp = temp;
	return pTemp;   
}

//����д��־����
void Logger::SetLogLevel(EnumLogLevel nLevel)
{
	m_nLogLevel = nLevel;
}

//д�ļ�����
void Logger::Trace(const char * strInfo)
{
	if(!strInfo)
		return;
	try
	{
		//�����ٽ���
		EnterCriticalSection(&m_cs);
		//���ļ���û�д򿪣������´�
		if(!m_pFileStream)
		{
			char temp[1024] = {0};
			strcat_s(temp, m_strLogPath);
			strcat_s(temp, m_strCurLogName);
			fopen_s(&m_pFileStream,temp, "a+");
			if(!m_pFileStream)
			{
				return;
			}
		}
		//д��־��Ϣ���ļ���
		fprintf(m_pFileStream, "%s\n", strInfo);
		fflush(m_pFileStream);
		//�뿪�ٽ���
		LeaveCriticalSection(&m_cs);
	}
	//�������쳣�������뿪�ٽ�������ֹ����
	catch(...)
	{
		LeaveCriticalSection(&m_cs);
	}
}

//������־�ļ�������
void Logger::GenerateLogName()
{
	time_t curTime;
	tm pTimeInfo;
	time(&curTime);
	localtime_s(&pTimeInfo,&curTime);
	char temp[1024] = {0};
	//��־�������磺2013-01-01.log
	sprintf_s(temp, "%04d-%02d-%02d.log", pTimeInfo.tm_year+1900, pTimeInfo.tm_mon + 1, pTimeInfo.tm_mday);
	if(0 != strcmp(m_strCurLogName, temp))
	{
		strcpy_s(m_strCurLogName,temp);
		if(m_pFileStream)
			fclose(m_pFileStream);
		char temp[1024] = {0};
		strcat_s(temp, m_strLogPath);
		strcat_s(temp, m_strCurLogName);
		//��׷�ӵķ�ʽ���ļ���
		fopen_s(&m_pFileStream,temp, "a+");
	}

}

//������־�ļ���·��
void Logger::CreateLogPath()
{
	if(0 != strlen(m_strLogPath))
	{
		strcat_s(m_strLogPath, "\\");
	}
	//MakeSureDirectoryPathExists(m_strLogPath);
	CreateDirectory(m_strLogPath,NULL);
}

