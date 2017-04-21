#pragma once
#include "FreedCommunication.h"
//#include "serialport.h"


#ifdef FREED_EXPORTS
#define FREED_API __declspec(dllexport)
#else
#define FREED_API __declspec(dllimport)
#endif

FREED_API void __stdcall Freed_Init(int port);

FREED_API void __stdcall Freed_SendD1Msg(float CameraPanAngle,float CameraTiltAngle,float CameraRollAngle,float CameraXPosition,float CameraYPosition,float CameraZPosition,WORD CameraZoom,WORD CameraFocus);

FREED_API void __stdcall Freed_Close();