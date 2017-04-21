#pragma once
// 在项目属性，c++预处理器定义，添加GP_DLL_EXPORT的定义。这样后期动态库在生成后，头文件可以直接
// 用来在项目中使用，不需要再重新修改头文件。
#ifdef  GP_DLL_EXPORT
#define GP_DLL_API  __declspec(dllexport)
#define GP_DLL_C_FUN  extern "C" __declspec(dllexport)
#else
#define GP_DLL_API __declspec(dllimport)
#define GP_DLL_C_FUN extern "C" __declspec(dllimport)
#endif

