#pragma once
//********************************************************************
// 创建时间：  2016/05/12  14:42 
// 文件名称：  GP_Logger.h 
// 作    者：  GP_Fore
//********************************************************************
// 功能说明：  提供写日志功能，支持多线程，支持可变形参数操作，支持写日志级别的设置
// 接    口：  SetLogLevel：设置写日志级别
//			   TraceKeyInfo：忽略日志级别，写关键信息
//             TraceError：写错误信息
//             TraceWarning：写警告信息
//             TraceInfo：写一般信息
// 备    注：  在有中文输出的环境下，记者使用setlocale(LC_ALL, "chs");先进行本地化设置。
//********************************************************************
#ifndef GP_LOGGER_H_
#define GP_LOGGER_H_
#include <Windows.h>
#include <stdio.h>
#include "GP_BaseObject.h"
#include <typeinfo.h>


//日志模块公共变量定义
#ifndef GP_LOGGER_DEFINE  
#define GP_LOGGER_DEFINE

#define WM_USERDEFINEWINDOWSINFO WM_USER+2000 //支持windows消息机制。

//日志级别的提示信息
//static const TCHAR * KEYINFOPREFIX = _T(" Key:");     //关键日志前缀
//static const TCHAR * ERRORPREFIX = _T(" Error: ");    //错误日志前缀
//static const TCHAR * WARNINGPREFIX = _T(" Warning:"); //警告日志前缀
//static const TCHAR * INFOPREFIX = _T(" Info: ");      //一般日志前缀
//static const int MAX_STR_LEN = 1024;               //最大缓存。



#endif


//日志类型
typedef enum EnumLogType
{
	ErrInfo=0,      //错误日志信息
	WarnInfo,       //警告日志信息
	KeyInfo,        //一般日志信息
	DataInfo        //数据跟踪信息
};

//typedef enum EnumLogLevel
//{
//	LogLevelAll = 0,    //所有信息都写日志
//	LogLevelMid,        //写错误、警告信息
//	LogLevelNormal,     //只写错误信息
//	LogLevelStop        //不写日志
//};

//static EnumLogType ErrLog = ErrInfo;

class GP_DLL_API GP_Logger
{
private:
	//默认构造函数
	GP_Logger();
	//析构函数
	virtual ~GP_Logger();
	//写文件操作
	void WriteStrToTxt(const TCHAR * strInfo);
	//获取当前系统时间
	TCHAR * GetCurrentTimeToTchar();
	//创建日志文件名称
	void GenerateLogName();

public:

	//控制日记记录的级别
	EnumLogLevel m_nLogLevel;

	void TraceLogger(EnumLogType pLogType, const TCHAR * strInfo, ...);
	//设置日志文件保存目录
	void UpdateLogDirectory(const TCHAR * strDirectoryPath);
	//将信息发送到指定的windows窗体
	void SendInfoToWindows(HWND hWnd, const TCHAR * strInfo, ...);
	//获取实例对象。
	static GP_Logger* GetInstance();
private:
	//写日志文件流
	FILE * m_pFileStream=NULL;
	//日志的路径
	TCHAR m_LogDirectoryPath[MAX_STR_LEN];
	//日志的名称
	TCHAR m_strCurLogName[MAX_STR_LEN];
	//线程同步的临界区变量
	CRITICAL_SECTION m_cs;
	//当前实例静态指针
	static GP_Logger* logger;
	//防止拷贝构造和赋值操作
	GP_Logger(const GP_Logger&);
	GP_Logger& operator=(const GP_Logger&);
};

#endif


