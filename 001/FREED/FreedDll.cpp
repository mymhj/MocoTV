#include "FreedDll.h"

FCom Freed;

void __stdcall Freed_Init(int port)
{
	Freed.InitPortF(port);
}

void __stdcall Freed_SendD1Msg(float CameraPanAngle,float CameraTiltAngle,float CameraRollAngle,float CameraXPosition,float CameraYPosition,float CameraZPosition,WORD CameraZoom,WORD CameraFocus)
{
	Freed.msg.CMD=PollForPositionUpdate;
	Freed.msg.CameraPanAngle =CameraPanAngle;
	Freed.msg.CameraTiltAngle =CameraTiltAngle;
	Freed.msg.CameraRollAngle =CameraRollAngle;
	Freed.msg.CameraXPosition =CameraXPosition;
	Freed.msg.CameraYPosition =CameraYPosition;
	Freed.msg.CameraZPosition =CameraZPosition;
	Freed.msg.CameraZoom =CameraZoom;
	Freed.msg.CameraFocus =CameraFocus;
	Freed.SendOMsg();
}


void __stdcall Freed_Close()
{
	Freed.Close();
}