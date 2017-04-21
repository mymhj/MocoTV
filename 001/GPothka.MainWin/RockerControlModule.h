#pragma once

#include <dinput.h>
#define WM_USER_COMMONDEVENT WM_USER + 2004  //消息msg
#define WM_STICK_EVENT WM_USER + 3000  //消息msg
#define DIRECTINPUT_VERSION 0x0800  //
const double ROBOTMINSTEP= 0.02;//最小步长



class CRockerControlModule
{
public:
	CRockerControlModule(void);

	~CRockerControlModule(void);
	//初始化函数
	bool Init(HWND hHwnd); 

	//存储游戏摇杆的状态信息
	DIJOYSTATE2 m_RockerState; 

	//lpdirectinputdevice8 DIDevice8接口指针
	LPDIRECTINPUTDEVICE8 m_IDirectInputDevice8; 

	//更新摇杆指令数据。m_RockerState
	BOOL UpDataRockerData();

	////获取更新指令状态
	//modes ObtainControlCommand();

	//获取操作指令数据
	robotRunCommand ObtainOperatingCommand();
	//将摇杆轴的当前值按照设定比例转换为对应的机器人A1...A6轴对应的指令值。
	float ConvertJoystickToRobotAx(long joystickAxisValue);

	//将摇杆轴的当前值按照设定比例转换为对应的机器人E1轴对应的指令值。
	float CRockerControlModule::ConvertJoystickToRobotEx(long joystickAxisValue);

	//枚举设备  //回调函数 ----目前对程序没太多影响。可以不关注。
	static BOOL CALLBACK DIEnumDevicesCallback(const DIDEVICEINSTANCE* lpddi, VOID* pvRef); //枚举对象


	static volatile LONG m_isPressed;
	static volatile LONG m_BtnIndex;

	//判断各轴是否超阈值
	void axisValue();

	//超阈值运动进行逐步降速
	float speedDown();

	//减速系数
	float temp=1;

	void caculate(float X, float Y, float Z, double A, double B, double C);

	void test();
	
};

