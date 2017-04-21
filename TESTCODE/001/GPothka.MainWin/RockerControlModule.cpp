#pragma once
#include "stdafx.h"
#include "RockerControlModule.h"
#include <d3dx9math.h>
#include "GPothka.MainWinDlg.h"
//#include "GPothka.MainWinDlg.h"

volatile LONG CRockerControlModule::m_isPressed = FALSE; //触发添加关键帧事件

volatile LONG CRockerControlModule::m_BtnIndex = 0;  //记录摇杆上被按下的按键。

CRockerControlModule::CRockerControlModule(void)
{
}

CRockerControlModule::~CRockerControlModule(void)
{
}


//遥控杆初始化。
bool CRockerControlModule::Init(HWND mainHwnd)
{
	// DI8接口指针
	LPDIRECTINPUT8 pIDirectInput8; 
	// 实例句柄
	HINSTANCE  hInstance=GetModuleHandle(NULL);;

	GUID rockerGuid; //GUIDs

	HRESULT hr=0;

	hr=DirectInput8Create(hInstance,DIRECTINPUT_VERSION,IID_IDirectInput8,(void**)&pIDirectInput8,NULL);

	if(FAILED(hr)){
		OutputDebugString(_T("Create 失败-in InitJoystick/n"));
		return false;
	}
	hr=pIDirectInput8->EnumDevices(DI8DEVTYPE_JOYSTICK,DIEnumDevicesCallback,&rockerGuid,DIEDFL_ATTACHEDONLY);

	if(FAILED(hr))
	{
		OutputDebugString(_T("枚举设备失败 - in CDIJoystick::Initialise/n"));
		return false; 
	}
	hr=pIDirectInput8->CreateDevice(rockerGuid,&this->m_IDirectInputDevice8,NULL);

	if(FAILED(hr))
	{
		OutputDebugString(_T("创建设备失败 - in CDIJoystick::Initialise/n"));
		return false;
	}


	hr=m_IDirectInputDevice8->SetDataFormat(&c_dfDIJoystick2);
	if(FAILED(hr))
	{
		OutputDebugString(_T("设置数据格式失败 - in CDIJoystick::Initialise/n"));
		return false;
	}

	///设置协作等级。DISCL_BACKGROUND 后台模式；DISCL_FOREGROUND 前台模式；DISCL_EXCLUSIVE 独占模式；DISCL_NONEXCLUSIVE 非独占模式
	/// m_hWnd可以设置为NULL
	hr=m_IDirectInputDevice8->SetCooperativeLevel(NULL,DISCL_EXCLUSIVE|DISCL_FOREGROUND);
	if(FAILED(hr))
	{
		OutputDebugString(_T("设置协作等级失败 - in CDIJoystick::Initialise/n"));

	}


	///设置数据缓冲区
	DIPROPDWORD  dipbuffesize={
		{sizeof(DIPROPDWORD),
		sizeof(DIPROPHEADER),
		0,
		DIPH_DEVICE,
		},      //属性数据结构头
		9500  //数据
	};
	hr=m_IDirectInputDevice8->SetProperty(DIPROP_BUFFERSIZE,&dipbuffesize.diph);
	if(FAILED(hr))
	{
		OutputDebugString(_T("设置缓冲区失败 - in CDIJoystick::Initialise/n"));
		return false;
	}
	DIPROPRANGE AxisRange;
	ZeroMemory(&AxisRange,sizeof(DIPROPRANGE));
	AxisRange.diph.dwSize       = sizeof(DIPROPRANGE);
	AxisRange.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	AxisRange.diph.dwHow        = DIPH_DEVICE; //存取方式
	AxisRange.diph.dwObj        = DIPH_DEVICE;
	AxisRange.lMin              = JOYRANGMIN;
	AxisRange.lMax              = JOYRANGMAX;

	hr=m_IDirectInputDevice8->SetProperty(DIPROP_RANGE,&AxisRange.diph);
	if(FAILED(hr))
	{
		OutputDebugString(_T("设置游戏杆范围失败 - in CDIJoystick::Initialise/n"));
		//return false;
	}

	//设置死区范围

	DIPROPDWORD dipDeadZone;
	ZeroMemory(&dipDeadZone,sizeof(DIPROPDWORD));
	dipDeadZone.diph.dwSize=sizeof(DIPROPDWORD);
	dipDeadZone.diph.dwHeaderSize=sizeof(DIPROPHEADER);
	dipDeadZone.diph.dwHow=0;
	dipDeadZone.diph.dwObj=DIPH_DEVICE;
	dipDeadZone.dwData=JOYAXSISTEP;
	hr=m_IDirectInputDevice8->SetProperty(DIPROP_DEADZONE,&dipDeadZone.diph);
	if(FAILED(hr))
	{
		OutputDebugString(_T("设置死区范围失败 - in CDIJoystick::Initialise/n"));
		return false;
	}
	return true;
}


//回调函数 ----目前对程序没太多影响。可以不关注。
BOOL CALLBACK CRockerControlModule::DIEnumDevicesCallback(const DIDEVICEINSTANCE* lpddi, VOID* pvRef){
	*(GUID*) pvRef = lpddi->guidInstance;
	CRockerControlModule* obj=(CRockerControlModule*)pvRef;

	//return DIENUM_STOP;//扫描后停止
	return DIENUM_CONTINUE;  //扫描后继续扫描
}

//更新摇杆指令数据。m_RockerState
BOOL CRockerControlModule::UpDataRockerData(){

	HRESULT hr;
	int i=0;
	while(true)
	{
		i++;
		hr=m_IDirectInputDevice8->Poll();
		hr=m_IDirectInputDevice8->Acquire();
		ZeroMemory(&m_RockerState,sizeof(m_RockerState));
		hr=m_IDirectInputDevice8->GetDeviceState(sizeof(DIJOYSTATE2),&m_RockerState);

		if(SUCCEEDED(hr))
		{
			return TRUE;
		}
		else if(i>30)
		{
			return FALSE;
		}
	}
}

////将摇杆传回的值转换为相应的机械臂控制指令。
//robotRunCommand CRockerControlModule::ObtainOperatingCommand(){
//	robotRunCommand robotRunCommand;
//
//	//memset(&robotRunCommand,0,sizeof(robotRunCommand));	
//	if(((m_RockerState.rgbButtons[0]&0x80)||(m_RockerState.rgbButtons[6]&0x80)||(m_RockerState.rgbButtons[2]&0x80)
//		|| (m_RockerState.rgbButtons[3] & 0x80) || (m_RockerState.rgbButtons[4] & 0x80) || (m_RockerState.rgbButtons[5] & 0x80))&&robotLock==0){
//		//A1 按钮1
//			if(m_RockerState.rgbButtons[0]&0x80){
//				if (abs(m_RockerState.lY) >= abs(m_RockerState.lX))
//				{
//					//robotRunCommand.A1 = ConvertJoystickToRobotAx(m_RockerState.lY)*0.05;
//					robotRunCommand.A1 = 0;
//				}
//				else
//				{
//					robotRunCommand.A = temp*ConvertJoystickToRobotAx(m_RockerState.lX)*0.05;
//					caculate(0, 0, 0, robotRunCommand.A, 0, 0);
//					if (rollLock == 1)
//					{
//						robotRunCommand.A = 0;
//						robotRunCommand.B = 0;
//						robotRunCommand.C = 0;
//					}
//					else if (rollLock == 0)
//					{
//						robotRunCommand.A = cmd_A;
//						robotRunCommand.B = cmd_B;
//						robotRunCommand.C = cmd_C;
//					}
//				}
//			}
//			//A2 按钮2
//			if(m_RockerState.rgbButtons[6]&0x80){
//				if (abs(m_RockerState.lY) >= abs(m_RockerState.lX))
//				{
//					//robotRunCommand.A2 = ConvertJoystickToRobotAx(m_RockerState.lY)*0.05;
//					robotRunCommand.A2 = 0;
//				}
//				else
//				{
//					robotRunCommand.B = temp*ConvertJoystickToRobotAx(m_RockerState.lX)*0.05;
//					caculate(0, 0, 0, 0, robotRunCommand.B, 0);
//					robotRunCommand.A = cmd_A;
//					robotRunCommand.B = cmd_B;
//					robotRunCommand.C = cmd_C;
//
//				}
//			}
//			//A3
//			if(m_RockerState.rgbButtons[2]&0x80){
//				if (abs(m_RockerState.lY) >= abs(m_RockerState.lX))
//				{
//					//robotRunCommand.A3 = ConvertJoystickToRobotAx(m_RockerState.lY)*0.05;
//					robotRunCommand.A3 = 0;
//				}
//				else
//				{
//					robotRunCommand.C = temp*ConvertJoystickToRobotAx(m_RockerState.lX)*0.05;
//					caculate(0, 0, 0, 0, 0, robotRunCommand.C);
//					if (rollLock == 1)
//					{
//						robotRunCommand.A = 0;
//						robotRunCommand.B = 0;
//						robotRunCommand.C = 0;
//					}
//					else if (rollLock == 0)
//					{
//						robotRunCommand.A = cmd_A;
//						robotRunCommand.B = cmd_B;
//						robotRunCommand.C = cmd_C;
//					}
//
//				}
//			}
//			//A4
//			if(m_RockerState.rgbButtons[3]&0x80){
//				if (abs(m_RockerState.lY) >= abs(m_RockerState.lX))
//				{
//					//robotRunCommand.A4 = ConvertJoystickToRobotAx(m_RockerState.lY)*0.05;
//					robotRunCommand.A4 = 0;
//				}
//				else
//				{
//					robotRunCommand.X = temp*ConvertJoystickToRobotAx(m_RockerState.lX)*0.2;
//					caculate(robotRunCommand.X, 0, 0, 0, 0, 0);
//					robotRunCommand.X = cmd_X;
//					robotRunCommand.Y = cmd_Y;
//					robotRunCommand.Z = cmd_Z;
//				}
//			}
//			//A5
//			if(m_RockerState.rgbButtons[4]&0x80){
//				if (abs(m_RockerState.lY) >= abs(m_RockerState.lX))
//				{
//					//robotRunCommand.A5 = ConvertJoystickToRobotAx(m_RockerState.lY)*0.05;
//					robotRunCommand.A5 = 0;
//				}
//				else
//				{
//					robotRunCommand.Y = temp*ConvertJoystickToRobotAx(m_RockerState.lX)*0.2;
//					caculate(0, robotRunCommand.Y, 0, 0, 0, 0);
//					robotRunCommand.X = cmd_X;
//					robotRunCommand.Y = cmd_Y;
//					robotRunCommand.Z = cmd_Z;
//				}
//			}
//			//A6
//			if(m_RockerState.rgbButtons[5]&0x80){
//				if (abs(m_RockerState.lY) >= abs(m_RockerState.lX))
//				{
//					//robotRunCommand.A6 = ConvertJoystickToRobotAx(m_RockerState.lY)*0.05;
//					robotRunCommand.A6 = 0;
//				}
//				else
//				{
//					robotRunCommand.Z = temp*ConvertJoystickToRobotAx(m_RockerState.lX)*0.2;
//					caculate(0, 0, robotRunCommand.Z, 0, 0, 0);
//					robotRunCommand.X = cmd_X;
//					robotRunCommand.Y = cmd_Y;
//					robotRunCommand.Z = cmd_Z;
//				}
//			}
//
//	}
//
//	//E1轴
//	if(m_RockerState.rgbButtons[8]&0x80){
//		float stepNum;//计算当前发送偏移量和上一偏移指令差值
//	    stepNum=ConvertJoystickToRobotEx(m_RockerState.lX)*0.6-gCurrentRunCommand.E1;
//		float sign;//偏移量符号
//		sign=ConvertJoystickToRobotEx(m_RockerState.lX)*0.6/fabs(ConvertJoystickToRobotEx(m_RockerState.lX)*0.6);
//		if(fabs(stepNum)>0.2&&fabs(gCurrentRunCommand.E1)<fabs(ConvertJoystickToRobotEx(m_RockerState.lX)*0.6))
//			{
//			robotRunCommand.E1 = gCurrentRunCommand.E1 + 0.175*stepNum;
//			}
//		else
//			{
//			robotRunCommand.E1 = ConvertJoystickToRobotEx(m_RockerState.lX)*0.6;
//			}
//		robotRunCommand.E1 = temp*robotRunCommand.E1*1.5;
//	}
//
//	axisValue();
//	temp = speedDown();
//
//	if (abs(m_RockerState.lRx)>=400&&cameraLock==0)
//	{
//		if (m_RockerState.rgbButtons[25] & 0x80)
//		{
//			focCon = 1;
//			if (camera == "Fujinong")
//				focVal = m_RockerState.lRx + 2048;
//			else if (camera == "Canon")
//				focVal = 0.5*m_RockerState.lRx ;
//
//		}
//		else if (m_RockerState.rgbButtons[26] & 0x80)
//		{
//			zomCon = 1;
//			if (camera == "Fujinong")
//				focVal = m_RockerState.lRx + 2048;
//			else if (camera == "Canon")
//				focVal = 0.5*m_RockerState.lRx;
//		}
//		else
//		{
//			if (camera == "Fujinong")
//			if (focVal != 2048)
//			{
//				focCon = 1;
//				zomCon = 1;
//				focVal = 2048;
//			}
//			else if (camera == "Canon")
//			if (focVal != 0)
//			{
//				focCon = 1;
//				zomCon = 1;
//				focVal = 0;
//			}
//		}
//	}
//	else if (abs(m_RockerState.lRx) >= 100 && cameraLock == 0)
//	{
//		if(m_RockerState.rgbButtons[25] & 0x80)
//		{
//			focCon = 1;
//			if (camera == "Fujinong")
//				focVal = 2048;
//			else if (camera == "Canon")
//				focVal = 0;
//			
//		}
//	   else if (m_RockerState.rgbButtons[26] & 0x80)
//	   {
//         zomCon = 1;
//		   if (camera == "Fujinong")
//			   focVal = 2048;
//		   else if (camera == "Canon")
//			   focVal = 0;
//	   }
//	}
//
//return robotRunCommand;
//
//}

//摇杆控制
robotRunCommand CRockerControlModule::ObtainOperatingCommand(){
	robotRunCommand robotRunCommand;
	float step;

	//实时控制
	if (((m_RockerState.rgbButtons[16] & 0x80) || (m_RockerState.rgbButtons[17] & 0x80) || (m_RockerState.rgbButtons[18] & 0x80)
		|| (m_RockerState.rgbButtons[19] & 0x80) || (m_RockerState.rgbButtons[20] & 0x80) || (m_RockerState.rgbButtons[21] & 0x80)
		|| (m_RockerState.rgbButtons[25] & 0x80) || (m_RockerState.rgbButtons[26] & 0x80) || (m_RockerState.rgbButtons[27] & 0x80) || (m_RockerState.rgbButtons[28] & 0x80)
		|| (m_RockerState.lX != 0) || (m_RockerState.lY != 0) || (m_RockerState.lZ != 0) || (m_RockerState.lRx != 0)) && robotLock == 0){

		//C5 X+
		if (m_RockerState.rgbButtons[16] & 0x80){
			robotRunCommand.X = temp*ConvertJoystickToRobotAx(1200)*0.2;//0.12
			caculate(robotRunCommand.X, 0, 0, 0, 0, 0);
			robotRunCommand.X = cmd_X;
			robotRunCommand.Y = cmd_Y;
			robotRunCommand.Z = cmd_Z;
		}
		//C6 X-
		else if (m_RockerState.rgbButtons[17] & 0x80){
			robotRunCommand.X = temp*ConvertJoystickToRobotAx(-1200)*0.2;//0.12
			caculate(robotRunCommand.X, 0, 0, 0, 0, 0);
			robotRunCommand.X = cmd_X;
			robotRunCommand.Y = cmd_Y;
			robotRunCommand.Z = cmd_Z;
		}
		//C7 Y+
		if (m_RockerState.rgbButtons[18] & 0x80){
			robotRunCommand.Y = temp*ConvertJoystickToRobotAx(1200)*0.2;//0.12
			caculate(0, robotRunCommand.Y, 0, 0, 0, 0);
			robotRunCommand.X = cmd_X;
			robotRunCommand.Y = cmd_Y;
			robotRunCommand.Z = cmd_Z;
		}
		//C8 Y-
		else if (m_RockerState.rgbButtons[19] & 0x80){
			robotRunCommand.Y = temp*ConvertJoystickToRobotAx(-1200)*0.2;//0.12
			caculate(0, robotRunCommand.Y, 0, 0, 0, 0);
			robotRunCommand.X = cmd_X;
			robotRunCommand.Y = cmd_Y;
			robotRunCommand.Z = cmd_Z;
		}
		//C9 Z+
		if (m_RockerState.rgbButtons[20] & 0x80){
			robotRunCommand.Z = temp*ConvertJoystickToRobotAx(1200)*0.2;
			caculate(0, 0, robotRunCommand.Z, 0, 0, 0);
			robotRunCommand.X = cmd_X;
			robotRunCommand.Y = cmd_Y;
			robotRunCommand.Z = cmd_Z;
		}
		//C10 Z-
		else if (m_RockerState.rgbButtons[21] & 0x80){
			robotRunCommand.Z = temp*ConvertJoystickToRobotAx(-1200)*0.2;
			caculate(0, 0, robotRunCommand.Z, 0, 0, 0);
			robotRunCommand.X = cmd_X;
			robotRunCommand.Y = cmd_Y;
			robotRunCommand.Z = cmd_Z;
		}
		////摇杆前、后、左、右推 
		if ((abs(m_RockerState.lY) > 100 || abs(m_RockerState.lX) >100) || (abs(m_RockerState.lZ) >100))
		{
			//前后推
			if((abs(m_RockerState.lY) >= abs(m_RockerState.lX)) && (abs(m_RockerState.lY) > 100))
			{
				robotRunCommand.B = temp*ConvertJoystickToRobotAx(m_RockerState.lY)*0.05;
				caculate(0, 0, 0, 0, robotRunCommand.B, 0);
				robotRunCommand.A = cmd_A;
				robotRunCommand.B = cmd_B;
				robotRunCommand.C = cmd_C;
				
			}
			//左右推
			else if((abs(m_RockerState.lY) <= abs(m_RockerState.lX)) && (abs(m_RockerState.lX) > 100))
			{
				robotRunCommand.A = temp*ConvertJoystickToRobotAx(m_RockerState.lX)*0.05;
				caculate(0, 0, 0, robotRunCommand.A, 0, 0);
				if (rollLock == 1)
				{
					robotRunCommand.A = 0;
					robotRunCommand.B = 0;
					robotRunCommand.C = 0;
				}
				else if (rollLock == 0)
				{
					robotRunCommand.A = cmd_A;
					robotRunCommand.B = cmd_B;
					robotRunCommand.C = cmd_C;
				}
			}
			//摇杆左右旋
			else if ((abs(m_RockerState.lZ) > 100))
			{
				robotRunCommand.C = temp*ConvertJoystickToRobotAx(m_RockerState.lZ)*0.05;
				caculate(0, 0, 0, 0, 0, robotRunCommand.C);
				if (rollLock == 1)
				{
					robotRunCommand.A = 0;
					robotRunCommand.B = 0;
					robotRunCommand.C = 0;
				}
				else if (rollLock == 0)
				{
					robotRunCommand.A = cmd_A;
					robotRunCommand.B = cmd_B;
					robotRunCommand.C = cmd_C;
				}
			}
		}
		
		//C16 E1+
		if (m_RockerState.rgbButtons[25] & 0x80)
		{
			robotRunCommand.E1 = gE1Step*sWholeSpeedE1;
			float stepNum;//计算当前发送偏移量和上一偏移指令差值
			stepNum = robotRunCommand.E1 - gCurrentRunCommand.E1;
			if (fabs(stepNum) > 0.2)
			{
				robotRunCommand.E1 = gCurrentRunCommand.E1 + 0.02;
			}
			cmdE1 = robotRunCommand.E1;
		}
		//C17 E1-
		else if (m_RockerState.rgbButtons[26] & 0x80)
		{
			robotRunCommand.E1 =-1* gE1Step*sWholeSpeedE1;
			float stepNum;//计算当前发送偏移量和上一偏移指令差值
			stepNum = robotRunCommand.E1 - gCurrentRunCommand.E1;
			if (fabs(stepNum) > 0.2)
			{
				robotRunCommand.E1 = gCurrentRunCommand.E1 - 0.02;
			}
			cmdE1 = robotRunCommand.E1;
		}
		
		/*	axisValue();
			temp = speedDown();*/

		//Zoom C14左右旋
		if (abs(m_RockerState.lRx) >= 300 && cameraLock == 0)
		{
			zomCon = 1;
			if (camera == "Fujinon")
				//zomVal = 0.05*m_RockerState.lRx;
				zomVal = m_RockerState.lRx + 2048;
			else if (camera == "Canon")
				zomVal = 20*m_RockerState.lRx+32768;
				

		}
		else if (abs(m_RockerState.lRx) >= 100 && cameraLock == 0)
		{
			zomCon = 1;
			if (camera == "Fujinon")
				zomVal = 2048;
			else if (camera == "Canon")
				zomVal = 32768;
		}
		else
		{
			if (camera == "Fujinon")
				zomVal = 2048;
			else if (camera == "Canon")
			{
				zomCon = 0;
				zomVal = 32768;
			}
		}

		//Focus C15左右旋
		if (m_RockerState.rgbButtons[27] & 0x80)
		{
			focCon = 1;
			if (camera == "Fujinon")
				focVal = 30;
			else if (camera == "Canon")
				focVal = 800;
		}
		else if (m_RockerState.rgbButtons[28] & 0x80)
		{
			focCon = 1;
			if (camera == "Fujinon")
				focVal = -30;
			else if (camera == "Canon")
				focVal = -800;
		}
	}



	//摇杆停止时E1缓冲
		else if (cmdE1 != 0)
		{
			if (cmdE1 > 0)
			{
				cmdE1 = cmdE1 - 0.02;
				if (cmdE1 < 0)
				{
					cmdE1 = 0;
				}
				robotRunCommand.E1 = cmdE1;
			}
			if (cmdE1 < 0)
			{
				cmdE1 = cmdE1 + 0.02;
				if (cmdE1 > 0)
				{
					cmdE1 = 0;
				}
				robotRunCommand.E1 = cmdE1;
			}
		}
	//主窗体快捷功能
	if (((m_RockerState.rgbButtons[12] & 0x80) || (m_RockerState.rgbButtons[0] & 0x80) || (m_RockerState.rgbButtons[1] & 0x80) || (m_RockerState.rgbButtons[2] & 0x80)
		|| (m_RockerState.rgbButtons[13] & 0x80) || (m_RockerState.rgbButtons[3] & 0x80) || (m_RockerState.rgbButtons[4] & 0x80) || (m_RockerState.rgbButtons[5] & 0x80)
		|| (m_RockerState.rgbButtons[14] & 0x80) || (m_RockerState.rgbButtons[6] & 0x80) || (m_RockerState.rgbButtons[7] & 0x80) || (m_RockerState.rgbButtons[8] & 0x80)) && robotLock == 0)
	{
		HWND hWnd = ::FindWindow(NULL, _T("MOCO-TV"));      //得到对话框的句柄
		//C1、1、2、3
		if (m_RockerState.rgbButtons[12] & 0x80)
		{
			if (btnPresh == 0)
			{
				restoration = 1;
				::PostMessageA(hWnd, WM_LIMIT_STATE, 12, 0);
				btnPresh = 1;
			}
			
		}
		if (m_RockerState.rgbButtons[0] & 0x80)
		{
			if (btnPresh == 0)
			{
				if (restoration == 1||restoration==2)
				{
					::PostMessageA(hWnd, WM_STICK_EVENT, 0, 0);
					btnPresh = 1;
					//单步运动状态
					restoration = 2;
				}
			}
			
		}
		if (m_RockerState.rgbButtons[1] & 0x80)
		{
			if (btnPresh == 0)
			{
				//检查是否复位
				if (restoration == 1)
				{
					::PostMessageA(hWnd, WM_STICK_EVENT, 1, 0);
					btnPresh = 1;
					restoration = 0;
				}
			}
			
		}
		if (m_RockerState.rgbButtons[2] & 0x80)
		{
			if (btnPresh == 0)
			{
				::PostMessageA(hWnd, WM_STICK_EVENT, 2, 0);
				btnPresh = 1;
			}
		}

		//C2、4、5、6
		if (m_RockerState.rgbButtons[13] & 0x80)
		{
			if (btnPresh == 0)
			{
				sWholeSpeedE1 += 0.1;
				if (sWholeSpeedE1 > 0.8)
				{
					sWholeSpeedE1 = 0.8;
				}
				btnPresh = 1;
			}
			
		}
		if (m_RockerState.rgbButtons[3] & 0x80)
		{
			if (btnPresh == 0)
			{
				sWholeSpeedE1 -= 0.1;
				if (sWholeSpeedE1 <= 0)
				{
					sWholeSpeedE1 = 0.1;
				}
				btnPresh = 1;
			}
			
		}
		if (m_RockerState.rgbButtons[4] & 0x80)
		{
			if (btnPresh == 0)
			{
				sWholeSpeed += 0.1;
				if (sWholeSpeed > 0.8)
				{
					sWholeSpeed = 0.8;
				}
				btnPresh = 1;
			}
		}
		if (m_RockerState.rgbButtons[5] & 0x80)
		{
			if (btnPresh == 0)
			{
				sWholeSpeed -= 0.1;
				if (sWholeSpeed <= 0)
				{
					sWholeSpeed = 0.1;
				}
				btnPresh = 1;
			}

		}
		//C3、7、8、9
		if (m_RockerState.rgbButtons[14] & 0x80)
		{
			if (btnPresh == 0)
			{
				::PostMessageA(hWnd, WM_STICK_EVENT, 14, 0);
				btnPresh = 1;
			}
			
		}
		if (m_RockerState.rgbButtons[6] & 0x80)
		{
			if (btnPresh == 0)
			{
				::PostMessageA(hWnd, WM_STICK_EVENT, 6, 0);
				btnPresh = 1;
			}
	
		}
		if (m_RockerState.rgbButtons[7] & 0x80)
		{
			
			if (btnPresh == 0)
			{
				::PostMessageA(hWnd, WM_STICK_EVENT, 7, 0);
				btnPresh = 1;
			}	
		}
		if (m_RockerState.rgbButtons[8] & 0x80)
		{
			if (btnPresh == 0)
			{
				::PostMessageA(hWnd, WM_STICK_EVENT, 8, 0);
				btnPresh = 1;
			}
		}
	}
	else
	{
		btnPresh = 0;
	}

	return robotRunCommand;

}


//将摇杆轴的当前值按照设定比例转换为对应的机器人A1...A6轴对应的指令值。
//float CRockerControlModule::ConvertJoystickToRobotAx(long joystickAxisValue){
//	return ((int)(joystickAxisValue/JOYAXSISTEP))*gA1Step*gWholeSpeed;
//}

//A1-A6摇杆控制加速
float CRockerControlModule::ConvertJoystickToRobotAx(long joystickAxisValue){
	float  result = (((float)joystickAxisValue / JOYAXSISTEP))*gE1Step*sWholeSpeed;
	if (fabs(result) < 0.0001)
	{
		result = 0;
	}
	return result;
}

//将摇杆轴的当前值按照设定比例转换为对应的机器人E1轴对应的指令值。
float CRockerControlModule::ConvertJoystickToRobotEx(long joystickAxisValue){
	float  result = (((float)joystickAxisValue / JOYAXSISTEP))*gE1Step*sWholeSpeedE1;
	if (fabs(result) < 0.0001)
	{
		result = 0;
	}
	return result;
}

void CRockerControlModule::axisValue()
{
	//姿态数据阈值判断
	if (gCurrentRobotLocation.E1<-3900.0 || gCurrentRobotLocation.E1>3900.0)
	{
		if (LocationE1 == 0)
		{
			LocationE1 = m_RockerState.lX;
		}
		return ;
	}
	else if (gCurrentRobotLocation.A1<-170.0 || gCurrentRobotLocation.A1>170.0) 
	{
		if (LocationA1 == 0)
		{
			LocationA1 = m_RockerState.lX;
		}
		return ;
	}
	else if (gCurrentRobotLocation.A2<-120.0 || gCurrentRobotLocation.A2>20.0)
	{
		if (LocationA2 == 0)
		{
			LocationA2 = m_RockerState.lX;
		}
		return ;
	}
	else if (gCurrentRobotLocation.A3<-105.0 || gCurrentRobotLocation.A3>140.0)
	{
		if (LocationA3 == 0)
		{
			LocationA3 = m_RockerState.lX;
		}
		return ;
	}
	else if (gCurrentRobotLocation.A4<-335.0 || gCurrentRobotLocation.A4>335.0)
	{
		if (LocationA4 == 0)
		{
			LocationA4 = m_RockerState.lX;
		}
		return;
	}
	else if (gCurrentRobotLocation.A5<-105.0 || gCurrentRobotLocation.A5>85.0)
	{
		if (LocationA5 == 0)
		{
			LocationA5 = m_RockerState.lX;
		}
		return ;
	}
	else if (gCurrentRobotLocation.A6<-80.0 || gCurrentRobotLocation.A6>100.0)
	{
		if (LocationA6 == 0)
		{
			LocationA6 = m_RockerState.lX;
		}
		return ;
	}
	else
	{
		LocationE1 = 0;
		LocationA1 = 0;
		LocationA2 = 0;
		LocationA3 = 0;
		LocationA4 = 0;
		LocationA5 = 0;
		LocationA6 = 0;
		return ;
	}
	return ;
}

float CRockerControlModule::speedDown()
{
	if (LocationE1 != 0)
	{
		if (LocationE1*m_RockerState.lX >= 0)
		{
			if (k >=0.02)
			{
				k = k - 0.02;
			}
			else
			{
				k = 0;
			}
		}
		
		else
		{

			k = 1.0;

		}
	}

	else if (LocationA1 != 0)
	{
		if (LocationA1*m_RockerState.lX>=0)
		{
			if (k >= 0.05)
			{
				k = k - 0.05;
			}
			else
			{
				k = 0;
			}
		}
		else
		{

			k = 1.0;

		}
	}

	else if (LocationA2 != 0)
	{
		if (LocationA2*m_RockerState.lX >= 0)
		{
			if (k >= 0.05)
			{
				k = k - 0.05;
			}
			else
			{
				k = 0;
			}
		}
		else
		{

			k = 1.0;

		}
	}
	else if (LocationA3 != 0)
	{
		if (LocationA3*m_RockerState.lX >= 0)
		{
			if (k >= 0.05)
			{
				k = k - 0.05;
			}
			else
			{
				k = 0;
			}
		}
		else
		{

			k = 1.0;

		}
	}

	else if (LocationA4 != 0)
	{
		if (LocationA4*m_RockerState.lX >= 0)
		{
			if (k >= 0.05)
			{
				k = k - 0.05;
			}
			else
			{
				k = 0;
			}
		}
		else
		{

			k = 1.0;

		}
	}

	else if (LocationA5 != 0)
	{
		if (LocationA5*m_RockerState.lX >= 0)
		{
			if (k >= 0.05)
			{
				k = k - 0.05;
			}
			else
			{
				k = 0;
			}
		}
		else
		{

			k = 1.0;

		}
	}

	else if (LocationA6 != 0)
	{
		if (LocationA6*m_RockerState.lX >= 0)
		{
			if (k >= 0.05)
			{
				k = k - 0.05;
			}
			else
			{
				k = 0;
			}
		}
		else
		{

			k = 1.0;

		}
	}

	else
	{
		k = 1.0;
	}

	return k;
}

void CRockerControlModule::caculate(float X, float Y, float Z, double A, double B, double C)
{
	//test();
	D3DXMATRIX rotationX;
	D3DXMATRIX rotationY;
	D3DXMATRIX rotationZ;
	D3DXMATRIX rotation;
	D3DXMATRIX rotationEnd;
	D3DXMATRIX cameraRotation;
	D3DXMATRIX cameraXYZ;
	D3DXMATRIX rotationInver;

	D3DXMATRIX temp;
	D3DXMATRIX tempInver;

	double AngleA, AngleB, AngleC, AngleTemp;

	if (fabs(gCurrentRobotLocation.A) < 0.05)
		gCurrentRobotLocation.A = 0;
	if (fabs(gCurrentRobotLocation.B) < 0.05)
		gCurrentRobotLocation.B = 0;
	if (fabs(gCurrentRobotLocation.C) < 0.05)
		gCurrentRobotLocation.A = 0;

	AngleA = gCurrentRobotLocation.A*angleToRad*(-1);
	AngleB = gCurrentRobotLocation.B*angleToRad*(-1);
	AngleC = gCurrentRobotLocation.C*angleToRad*(-1);

	//绕X轴旋转矩阵
	D3DXMatrixRotationX(&rotationX, AngleC);

	//绕Y轴
	D3DXMatrixRotationY(&rotationY, AngleB);

	//绕Z轴
	D3DXMatrixRotationZ(&rotationZ, AngleA);

	//计算摇杆控制旋转矩阵
	if (A != 0)
	{
		AngleTemp = A*angleToRad*(-1);
		D3DXMatrixRotationZ(&temp, AngleTemp);
	}
	else if (B != 0)
	{
		AngleTemp = B*angleToRad*(-1);
		D3DXMatrixRotationY(&temp, AngleTemp);
	}
	else if (C != 0)
	{
		AngleTemp = C*angleToRad*(-1);
		D3DXMatrixRotationX(&temp, AngleTemp);
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (i != j)
					temp(i, j) = 0;
				else
					temp(i, j) = 1;
			}
		}
		if (X != 0)
		{
			temp(0, 3) = X;
		}
		else if (Y != 0)
		{
			temp(1, 3) = Y;
		}
		else if (Z != 0)
		{
			temp(2, 3) = Z;
		}

	}

	//计算工具坐标系到底座的旋转矩阵
	rotation = rotationZ*rotationY*rotationX;
	rotation(0, 3) = gCurrentRobotLocation.X;
	rotation(1, 3) = gCurrentRobotLocation.Y;
	rotation(2, 3) = gCurrentRobotLocation.Z;


	////////////////////////////////
	cameraXYZ = rotation*temp;

	cameraPan = atan2(cameraXYZ(1, 0), cameraXYZ(0, 0));//1
	cameraRoll = atan2(cameraXYZ(2, 1), cameraXYZ(2, 2));//3
	cameraTilt = atan2(-1 * cameraXYZ(2, 0), cameraXYZ(2, 2) / cos(cameraRoll));//2

	/*if (fabs(cameraPan) < 0.0005)
		cameraPan = 0;
	if (fabs(cameraRoll) < 0.0005)
		cameraRoll = 0;
	if (fabs(cameraTilt) < 0.0005)
		cameraTilt = 0;*/

	//绕X轴旋转矩阵
	D3DXMatrixRotationX(&rotationX, -1*cameraRoll);

	//绕Y轴
	D3DXMatrixRotationY(&rotationY, -1*cameraTilt);

	//绕Z轴
	D3DXMatrixRotationZ(&rotationZ, -1*cameraPan);

	//结束位置旋转矩阵
	rotationEnd = rotationZ*rotationY*rotationX;

	////////////////////////////////

	D3DXMatrixInverse(&rotationInver, 0, &rotation);
	
	cameraRotation = rotationEnd*rotationInver;
	
	//cameraRotation = rotation*temp*rotationInver;

	D3DXVECTOR4 cameraLocation(cameraXYZ(0, 3), cameraXYZ(1, 3), cameraXYZ(2, 3), 1);

	//ZYX顺序
	//cameraPan = atan2(cameraRotation(1, 0), cameraRotation(0, 0));//1
	//cameraRoll = atan2(cameraRotation(2, 1), cameraRotation(2, 2));//3
	//cameraTilt = atan2(-1 * cameraRotation(2, 0), cameraRotation(2, 2) / cos(cameraRoll));//2


	//XYZ顺序
	cameraRoll = atan2(-1 * cameraRotation(1, 2), cameraRotation(2, 2));//1
	cameraPan = atan2(-1 * cameraRotation(0, 1), cameraRotation(0, 0));//2
	cameraTilt = atan2(cameraRotation(0, 2), cameraRotation(2, 2) / cos(cameraRoll));//3

	cameraLocationX = cameraLocation.x;
	cameraLocationY = cameraLocation.y;
	cameraLocationZ = cameraLocation.z;

	cameraTilt = cameraTilt / angleToRad;
	cameraRoll = cameraRoll / angleToRad;
	cameraPan = cameraPan / angleToRad;

	cmd_X = cameraLocationX - gCurrentRobotLocation.X;
	cmd_Y = cameraLocationY - gCurrentRobotLocation.Y;
	cmd_Z = cameraLocationZ - gCurrentRobotLocation.Z;

	if (A != 0 || B != 0 || C != 0)
	{
		
		cmd_A = cameraPan;
		cmd_B = cameraTilt;
		cmd_C = cameraRoll;
	}
	else
	{
		cmd_A = 0;
		cmd_B = 0;
		cmd_C = 0;
	}

}

void CRockerControlModule::test()
{
	D3DXMATRIX rotationX;
	D3DXMATRIX rotationY;
	D3DXMATRIX rotationZ;
	D3DXMATRIX rotation;
	D3DXMATRIX rotationEnd;
	D3DXMATRIX cameraRotation;
	D3DXMATRIX cameraXYZ;
	D3DXMATRIX rotationInver;

	D3DXMATRIX temp;
	D3DXMATRIX tempInver;

	double AngleA, AngleB, AngleC, AngleTemp;
	double A, B, C;

	AngleA = -155.83*angleToRad*(-1);
	AngleB = 63.69*angleToRad*(-1);
	AngleC = -22.24 * angleToRad*(-1);
	A = 0* angleToRad*(-1);
	B = 0* angleToRad*(-1);
	C = 1.2* angleToRad*(-1);

	//绕X轴旋转矩阵
	D3DXMatrixRotationX(&rotationX, AngleC);

	//绕Y轴
	D3DXMatrixRotationY(&rotationY, AngleB);

	//绕Z轴
	D3DXMatrixRotationZ(&rotationZ, AngleA);

	rotation = rotationZ*rotationY*rotationX;


	//绕X轴旋转矩阵
	D3DXMatrixRotationX(&rotationX, C);

	//绕Y轴
	D3DXMatrixRotationY(&rotationY, B);

	//绕Z轴
	D3DXMatrixRotationZ(&rotationZ, A);

	temp =rotation*rotationX;
	
	//AngleA = -163.18*angleToRad*(-1);
	//AngleB = 77.07*angleToRad*(-1);
	//AngleC = -24.32 * angleToRad*(-1);

	////绕X轴旋转矩阵
	//D3DXMatrixRotationX(&rotationX, AngleC);

	////绕Y轴
	//D3DXMatrixRotationY(&rotationY, AngleB);

	////绕Z轴
	//D3DXMatrixRotationZ(&rotationZ, AngleA);

	//cameraXYZ = rotationZ*rotationY*rotationX;


	//zyx
	cameraPan = atan2(temp(1, 0), temp(0, 0));//1
	cameraRoll = atan2(temp(2, 1), temp(2, 2));//3
	cameraTilt = atan2(-1 * temp(2, 0), temp(2, 2) / cos(cameraRoll));//2

	cameraPan = cameraPan / angleToRad;
	cameraTilt =cameraTilt / angleToRad;
	cameraRoll =cameraRoll / angleToRad;

	D3DXMatrixInverse(&rotationInver, 0, &rotation);

	cameraXYZ = temp*rotationInver;

	cameraRoll = atan2(-1 * cameraXYZ(1, 2), cameraXYZ(2, 2));//1
	cameraPan = atan2(-1 * cameraXYZ(0, 1), cameraXYZ(0, 0));//2
	cameraTilt = atan2(cameraXYZ(0, 2), cameraXYZ(2, 2) / cos(cameraRoll));//3

	cameraPan = cameraPan / angleToRad;
	cameraTilt = cameraTilt / angleToRad;
	cameraRoll = cameraRoll / angleToRad;

	int k =99;
	return;
	
}
