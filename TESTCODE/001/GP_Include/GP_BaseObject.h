#pragma once
// ����Ŀ���ԣ�c++Ԥ���������壬���GP_DLL_EXPORT�Ķ��塣�������ڶ�̬�������ɺ�ͷ�ļ�����ֱ��
// ��������Ŀ��ʹ�ã�����Ҫ�������޸�ͷ�ļ���
#ifdef  GP_DLL_EXPORT
#define GP_DLL_API  __declspec(dllexport)
#define GP_DLL_C_FUN  extern "C" __declspec(dllexport)
#else
#define GP_DLL_API __declspec(dllimport)
#define GP_DLL_C_FUN extern "C" __declspec(dllimport)
#endif

