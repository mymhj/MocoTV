#pragma once
#include <vector>
#include "stdafx.h"
#include "RockerControlModule.h"
#include "TrackRepeatModule.h"
#define WM_LIMIT_STATE WM_USER + 4000 


class CAppWorkPlant
{
public:
	CAppWorkPlant(void);
	~CAppWorkPlant(void);
    bool StartServer(HWND pMainhWnd,HWND pVideoHwnd);

	//主窗体句柄
	HWND m_MainHwnd; 

	//摇杆控制模块
	CRockerControlModule m_RockerModule;

	//轨迹重复模块
	CTrackRepeatModule m_TrackRepeatModule;

	//接受数据包数量
	static long m_ReceivedDataPackageCount;

	///当前帧发送给机器人的数据。一个完整包。
	static string m_localSendCompletePackageString;

	//发送数据包数量
	static long m_SendDataPackageCount;
	///是否预设指令；
	static bool isPresetCommand;

	//提前计算好，将发送给机器人的数据。
	static vector<robotRunCommand> m_PresetSendDataColl;

	//Maya倒入的指令，将发送给机器人的数据。
	static vector<robotRunCommand> m_MayaSendDataColl;

	//记录当前发送到了第几帧。
	static int m_PresetDataSendPostioin;

	//轨迹重复中的机器人位置信息
	static vector<robotlocation> m_LocusRobotlocation;

	//轨迹重复中机器人的运行指令信息
	static vector<robotRunCommand> m_LocusRunCommand;

	SYSTEMTIME m_StartTime;

	SYSTEMTIME m_EndTime;

	//更新发送数据实体；
	bool RobotControlCenter();


private:

	//线程辅助函数1-通讯。
	static void __cdecl  ThreadAssistFunc1(void *para);
	//线程辅助函数1-逻辑控制。
	static void __cdecl  ThreadAssistFunc2(void *para);

	//机器人端监听，监听机器人和当前主机的通讯。
	void RobotClientListener(void);
	//业务逻辑进程，是独立与通讯进程的的进程单元。
    void BusinesslogicMaintain(void);
	//执行每隔12毫秒接受机器人状态数据和发送给机器人指令数据。
	void LoopReceiveAndSendFromRobot(SOCKET robotClientSocket);
	//处理接受到的不完整的机器人脏数据。-不规范数据的处理
	bool DelayStateHandle(char * robotLocation_char);
	//通过摇杆更新当前机器人控制各种状态。
	void UpdateControlStateFromRocker();
	//支撑摇杆指令控制状态。
	bool compareReceive(string String1, string String2);

public:
	static bool cmotion_end_repeat;
	sockaddr_in m_servAddr;
	SOCKET m_ServerSock;
};

