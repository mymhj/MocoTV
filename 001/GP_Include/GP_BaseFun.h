#pragma once
#include "stdafx.h"
#include "GP_BaseObject.h"


//********************************************************************
// 创建时间：  2016/07/15  16:45 
// 作    者：  GP_Fore
//********************************************************************
// 函数说明：  获得当前时间的字符串格式；"%02d:%02d:%02d"  时、分、秒
// 返回值:     GP_DLL_API TCHAR* __stdcall
// 备    注：        
//********************************************************************
//GP_DLL_API TCHAR* __stdcall NowTimeByChar();

GP_DLL_API TCHAR* __stdcall CharToTChar(char* _char);

GP_DLL_API char* __stdcall TCharToChar(TCHAR* _char);

GP_DLL_API char* __stdcall UnicodeToAnsi(const wchar_t* szStr);

GP_DLL_API wchar_t*  __stdcall AnsiToUnicode(const char* szStr);

//********************************************************************
// 创建时间：  2016/07/15  16:50 
// 作    者：  GP_Fore
//********************************************************************
// 函数说明：  在字符串source中，查找是否存在字符串match，并返回match在source的位置索引。
// 参数列表：  char * source
// 参数列表：  char * match
// 返回值:     int 返回值为-1时，为没有找到。
// 备    注：      
//********************************************************************
GP_DLL_API int Rfind(char*source,char* match);

//********************************************************************
// 创建时间：  2016/08/18  14:58 
// 作    者：  GP_Fore
//********************************************************************
// 函数说明：  将字符串，对应的二进制值以字符串的形式输出。
// 参数列表：  char * str_Source  需要转换的字符串。
// 参数列表：  char * str_Result  结果，需传递一个有足够空间的字符串。最小大小为str_Source的字符串长度*10
// 参数列表：  bool Separated     是否用空格将分割字符对应的字符串分开。true:10000001 10000002 false：1000000110000002
// 返回值:     bool               成功：true, 失败：fasle;
// 备    注：        
//********************************************************************
GP_DLL_API bool OutputByBinaryStringFormat(char* str_Source, char* str_Result, bool Separated);


//********************************************************************
// 创建时间：  2016/08/18  15:29 
// 作    者：  GP_Fore
//********************************************************************
// 函数说明：  将字符串，对应的16进制值以字符串的形式输出。
// 参数列表：  char * str_Source  需要转换的字符串。
// 参数列表：  char * str_Result  结果，需传递一个有足够空间的字符串。最小大小为str_Source的字符串长度*4
// 参数列表：  bool Separated     是否用空格将分割字符对应的字符串分开。true:10 10 false：1010
// 返回值:     GP_DLL_API bool    成功：true, 失败：fasle;
// 备    注：        
//********************************************************************
GP_DLL_API bool OutputByHexStringFormat(char* str_Source, char* str_Result, bool Separated);

