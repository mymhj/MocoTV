

#include <imagehlp.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include "stdafx.h"
#include "LoggerModule.h"

//#pragma comment(lib, "DbgHelp.lib")

//默认构造函数
Logger::Logger()
{
	//初始化
	memset(m_strLogPath, 0, MAX_STR_LEN);
	memset(m_strCurLogName, 0, MAX_STR_LEN);
	m_pFileStream = NULL;
	//设置默认的写日志级别
	m_nLogLevel = EnumLogLevel::LogLevelNormal;
	//初始化临界区变量
	InitializeCriticalSection(&m_cs);
	//创建日志文件名
	GenerateLogName();
}

//构造函数
Logger::Logger(const char * strLogPath, EnumLogLevel nLogLevel):m_nLogLevel(nLogLevel)
{
	//初始化
	m_pFileStream = NULL;
	strcpy_s(m_strLogPath, strLogPath);
	InitializeCriticalSection(&m_cs);
	CreateLogPath();
	GenerateLogName();
}


//析构函数
Logger::~Logger()
{
	//释放临界区
	DeleteCriticalSection(&m_cs);
	//关闭文件流
	if(m_pFileStream)
		fclose(m_pFileStream);
}

//写关键信息接口
void Logger::TraceKeyInfo(const char * strInfo, ...)
{
	if(!strInfo)
		return;
	char prefix[MAX_STR_LEN]={0};
	char pTemp[MAX_STR_LEN] = {0};
	strcpy_s(prefix, GetCurrentTime());
	strcat_s(prefix, KEYINFOPREFIX);
	//获取可变形参
	va_list arg_ptr = NULL;
	va_start(arg_ptr, strInfo);
	vsprintf_s(pTemp,sizeof(pTemp),strInfo, arg_ptr);
	strcat_s(prefix, pTemp);
	va_end(arg_ptr);
	//写日志文件
	Trace(prefix);
	arg_ptr = NULL;

}

//写错误信息
void Logger::TraceError(const char* strInfo, ...)
{
	//判断当前的写日志级别，若设置为不写日志则函数返回
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

//写警告信息
void Logger::TraceWarning(const char * strInfo, ...)
{
	//判断当前的写日志级别，若设置为只写错误信息则函数返回
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
//写一般信息
void Logger::TraceInfo(const char * strInfo, ...)
{
	//判断当前的写日志级别，若设置只写错误和警告信息则函数返回
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
//写状态信息
void Logger::TraceState(HWND hWnd,const char * strInfo, ...){
	//判断当前的写日志级别，若设置只写错误和警告信息则函数返回
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
	::SendMessage(hWnd,WM_USERLOGGRR,0,(LPARAM)&prefix); //同步
	arg_ptr = NULL;
}


//获取系统当前时间
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

//设置写日志级别
void Logger::SetLogLevel(EnumLogLevel nLevel)
{
	m_nLogLevel = nLevel;
}

//写文件操作
void Logger::Trace(const char * strInfo)
{
	if(!strInfo)
		return;
	try
	{
		//进入临界区
		EnterCriticalSection(&m_cs);
		//若文件流没有打开，则重新打开
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
		//写日志信息到文件流
		fprintf(m_pFileStream, "%s\n", strInfo);
		fflush(m_pFileStream);
		//离开临界区
		LeaveCriticalSection(&m_cs);
	}
	//若发生异常，则先离开临界区，防止死锁
	catch(...)
	{
		LeaveCriticalSection(&m_cs);
	}
}

//创建日志文件的名称
void Logger::GenerateLogName()
{
	time_t curTime;
	tm pTimeInfo;
	time(&curTime);
	localtime_s(&pTimeInfo,&curTime);
	char temp[1024] = {0};
	//日志的名称如：2013-01-01.log
	sprintf_s(temp, "%04d-%02d-%02d.log", pTimeInfo.tm_year+1900, pTimeInfo.tm_mon + 1, pTimeInfo.tm_mday);
	if(0 != strcmp(m_strCurLogName, temp))
	{
		strcpy_s(m_strCurLogName,temp);
		if(m_pFileStream)
			fclose(m_pFileStream);
		char temp[1024] = {0};
		strcat_s(temp, m_strLogPath);
		strcat_s(temp, m_strCurLogName);
		//以追加的方式打开文件流
		fopen_s(&m_pFileStream,temp, "a+");
	}

}

//创建日志文件的路径
void Logger::CreateLogPath()
{
	if(0 != strlen(m_strLogPath))
	{
		strcat_s(m_strLogPath, "\\");
	}
	//MakeSureDirectoryPathExists(m_strLogPath);
	CreateDirectory(m_strLogPath,NULL);
}

