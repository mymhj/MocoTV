#pragma once
#include "stdafx.h"
#include "AppWorkPlant.h"
#include "GPothka.MainWinDlg.h"

//接受数据包数量
long CAppWorkPlant::m_ReceivedDataPackageCount;

//发送数据包数量
long CAppWorkPlant::m_SendDataPackageCount;

///当前帧发送给机器人的数据。一个完整包。
string CAppWorkPlant::m_localSendCompletePackageString;

//是否使用预设指令,true 是， false:跳过
bool CAppWorkPlant::isPresetCommand;

bool CAppWorkPlant::cmotion_end_repeat;

//提前计算好，将发送给机器人的数据。
vector<robotRunCommand> CAppWorkPlant::m_PresetSendDataColl;

//Maya倒入的指令，将发送给机器人的数据。
 vector<robotRunCommand> CAppWorkPlant::m_MayaSendDataColl;

//记录当前发送预设指令的索引。
int CAppWorkPlant::m_PresetDataSendPostioin;

	//轨迹重复中的机器人位置信息
vector<robotlocation> CAppWorkPlant::m_LocusRobotlocation;
//
	//轨迹重复中机器人的运行指令信息
vector<robotRunCommand>  CAppWorkPlant::m_LocusRunCommand;

//工作台构造函数
CAppWorkPlant::CAppWorkPlant(void){	
	isPresetCommand=false;
	this->m_PresetSendDataColl.reserve(10000);
	cmotion_end_repeat=false;
}

//工作台析构函数
CAppWorkPlant::~CAppWorkPlant(void){
}

//开启通讯服务，验证系统启动条件是否已经整备充分。整备充分返回ture，false代表失败。
bool CAppWorkPlant::StartServer(HWND pMainhWnd,HWND pVideoHwnd){

	//机器人客户端监听线程
	HANDLE hThead1,hThead2;
	this->m_MainHwnd=pMainhWnd;

	if(m_RockerModule.Init(pMainhWnd)==false)
	{
		//SendLogToWindows(m_MainHwnd,"服务开启失败，请检查摇杆设备是否正常！！！");
		return false;
	}

	///创建机器人客户端监控线程
	hThead1=(HANDLE)_beginthread(ThreadAssistFunc1,0,(PVOID)this);

	///创建控制逻辑和指令维护线程
	hThead2=(HANDLE)_beginthread(ThreadAssistFunc2,0,(PVOID)this);

	DWORD lpExitCode1,lpExitCode2;

	GetExitCodeThread(hThead1,&lpExitCode1);
	GetExitCodeThread(hThead2,&lpExitCode2);

	ResumeThread(hThead1);
	ResumeThread(hThead2);
	return true;
}

//创建通讯线程
void CAppWorkPlant::ThreadAssistFunc1(void *para){
	CAppWorkPlant result=*((CAppWorkPlant*)para);
	result.RobotClientListener();
}

//创建业务线程
void CAppWorkPlant::ThreadAssistFunc2(void *para){
	CAppWorkPlant result=*((CAppWorkPlant*)para);
	result.BusinesslogicMaintain();
}


//机器人端监听，监听机器人和当前主机的通讯。
//void CAppWorkPlant::RobotClientListener(void){
//
//	USHORT port=6008;
//	SOCKET listenerSocket=::socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
//	sockaddr_in addr;
//	addr.sin_family=AF_INET;
//	addr.sin_port=htons(port);
//	addr.sin_addr.S_un.S_addr=INADDR_ANY;
//
//	if(::bind(listenerSocket,(sockaddr*)&addr,sizeof(addr))==SOCKET_ERROR){
//		SendLogToWindows(m_MainHwnd,"创建本地Sockect监控失败，请检查网线是否插好！");
//		return;
//	}
//	SendLogToWindows(m_MainHwnd,"成功开启通讯服务");
//	::listen(listenerSocket,5);
//	fd_set socket_Set;		// 定义一个fd_set结构的变量，用于存储socket句柄。
//	FD_ZERO(&socket_Set);     //清空socket_set集合。
//	FD_SET(listenerSocket, &socket_Set); //将listenerSocket加入socket_set中。
//	while(TRUE)
//	{
//		
//		fd_set readfds = socket_Set;  //将socket_Set集合中的socket句柄拷贝一份到readfds中，当有事件发生时，select函数移除readfds集合中没有未决I/O操作的套节字句柄，然后返回
//		if(::select(0, &readfds, NULL, NULL, NULL) > 0)
//		{
//			// 通过将原来socket_Set集合与select处理过的readfds集合比较，
//			// 确定都有哪些套节字有未决I/O，并进一步处理这些I/O。
//			for(int i=0; i<(int)socket_Set.fd_count; i++)
//			{
//				if(FD_ISSET(socket_Set.fd_array[i], &readfds)) //检测当前socket句柄在调用select函数之后是否发生变化，发生变化时返回为真。
//				{
//					if(socket_Set.fd_array[i] == listenerSocket) //监听套节字接收到新连接
//					{
//						//验证是否有新的客户端请求添加链接。
//						if(socket_Set.fd_count < FD_SETSIZE)
//						{
//							sockaddr_in addrRemote;
//							int nAddrLen = sizeof(addrRemote);
//							SOCKET newSocket = ::accept(listenerSocket, (SOCKADDR*)&addrRemote, &nAddrLen);
//							FD_SET(newSocket, &socket_Set);
//							char * c_IP =inet_ntoa(addrRemote.sin_addr);
//							char c_logInfo[100]="与终端设备“";
//						    strcat_s(c_logInfo,100,c_IP);
//							strcat_s(c_logInfo,100,"”建立通讯连接，开始收发数据…");
//							SendLogToWindows(m_MainHwnd,c_logInfo);
//						}
//						else
//						{
//							continue;
//						}
//					}
//					else
//					{
//						//已经建立连接的机器人和当前主机进行循环通讯。
//						this->LoopReceiveAndSendFromRobot(socket_Set.fd_array[i]);
//					}
//				}
//			}
//		}
//		else{
//			break;
//		}
//	}
//	_endthreadex( 0 ); 
//}

//基于UDP通讯
void CAppWorkPlant::RobotClientListener(void){

	USHORT port = 49152;
	m_servAddr.sin_family = AF_INET;
	m_servAddr.sin_port = htons(port);
	m_servAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	m_ServerSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (::bind(m_ServerSock, (sockaddr*)&m_servAddr, sizeof(struct sockaddr_in)) == SOCKET_ERROR){

		return ;
	}

	int clientAddr_len = sizeof(struct sockaddr_in);
	SOCKADDR_IN clientAddr;//客户端地址。

	char robotLocation_char[2048] = { 0 };

	while (TRUE){
		memset(robotLocation_char, 0, strlen(robotLocation_char));
		//char robotLocation_char[2048] = { 0 };
		if (::recvfrom(m_ServerSock, robotLocation_char, 2048, 0, (SOCKADDR*)&clientAddr, &clientAddr_len) == SOCKET_ERROR)
		{
			return;
		}

		int length = strlen(robotLocation_char);
		int reEndIndex = rfind(robotLocation_char, "</IPOC>");
		int reStartIndex = rfind(robotLocation_char, "<IPOC>");
		int ipoc_length = reEndIndex - reStartIndex;
		//char str[30];
		char str[200];
		strncpy_s(str, robotLocation_char + reStartIndex + strlen("<IPOC>"), reEndIndex - reStartIndex - strlen("<IPOC>"));

		float aa = 0;
		float bb = 0;
		float cc = 0;


		sscanf_s(robotLocation_char, gRobotLocation_format_str, &gCurrentRobotLocation.X, &gCurrentRobotLocation.Y, &gCurrentRobotLocation.Z, &gCurrentRobotLocation.A, &gCurrentRobotLocation.B, &gCurrentRobotLocation.C,
			&aa, &aa, &aa, &aa, &aa, &aa,
			&gCurrentRobotLocation.A1, &gCurrentRobotLocation.A2, &gCurrentRobotLocation.A3, &gCurrentRobotLocation.A4, &gCurrentRobotLocation.A5, &gCurrentRobotLocation.A6,
			&bb, &bb, &bb, &bb, &bb, &bb,
			&gCurrentRobotLocation.E1, &gCurrentRobotLocation.E2, &gCurrentRobotLocation.E3, &gCurrentRobotLocation.E4, &gCurrentRobotLocation.E5, &gCurrentRobotLocation.E6,
			&cc, &cc, &cc, &cc, &cc, &cc, 
			&gCurrentRobotLocation.Delay,
			&gCurrentRobotLocation.IPOC);

		char *robotRunCommand_char = (char*)malloc(1024);
		memset(robotRunCommand_char, 0, sizeof(robotRunCommand_char));
		if (RobotControlCenter() == FALSE){
			//更新数据失败s
			return;
		}
			

		sprintf_s(robotRunCommand_char, 1000, gRunCommand_format_str,
			gCurrentRunCommand.X, gCurrentRunCommand.Y, gCurrentRunCommand.Z, gCurrentRunCommand.A, gCurrentRunCommand.B, gCurrentRunCommand.C,
			gCurrentRunCommand.A1, gCurrentRunCommand.A2, gCurrentRunCommand.A3, gCurrentRunCommand.A4, gCurrentRunCommand.A5, gCurrentRunCommand.A6,
			gCurrentRunCommand.E1,
			str);

	

		int sendLength = sendto(m_ServerSock, robotRunCommand_char, strlen(robotRunCommand_char) + 1, 0, (SOCKADDR*)&clientAddr, clientAddr_len);

		if (sendLength == SOCKET_ERROR) //发送数据出错 
		{
			return;
		}
		if (isPresetCommand == true)
		{
			this->m_LocusRunCommand.push_back(gCurrentRunCommand);
			this->m_LocusRobotlocation.push_back(gCurrentRobotLocation);
		}
	}
}


//业务逻辑进程，是独立与通讯进程的的进程单元。
void CAppWorkPlant::BusinesslogicMaintain(void){
	while (true)
	{
		Sleep(100);
		compareReceive("","");
	}
}


//逻辑控制函数，通过预设状态控制系统当前执行的是实时控制、预设控制、暂停等操作。
bool CAppWorkPlant::RobotControlCenter()
{
	//通过获取摇杆数据更新当前系统逻辑控制状态
	this->UpdateControlStateFromRocker();

	if(gRunState.Suspend==true){
		
		memset(&gCurrentRunCommand,0,sizeof(gCurrentRunCommand));	

		//修改暂停功能为急停，清空预设指令
		m_PresetSendDataColl.clear();

		return true;
	}
	else
	{
		//如果是true,那么当前执行预设指令。可能当前在执行：单步运动、复位运动、轨迹重复中的一类运动。
		if(isPresetCommand==true)
		{
			int size=this->m_PresetSendDataColl.size();
			cmotion_end_repeat=false;
			if(this->m_PresetDataSendPostioin< size){
				if(this->m_PresetDataSendPostioin==0)
				{
					SYSTEMTIME stLocal,stLocal1;
					 GetLocalTime(&m_StartTime); 
					this->m_LocusRobotlocation.clear();
				}
				gCurrentRunCommand=this->m_PresetSendDataColl[this->m_PresetDataSendPostioin];
		
				this->m_PresetDataSendPostioin++;

				return true;
			}
			else{
				cmotion_end_repeat=true;
				this->m_PresetSendDataColl.clear();
				this->m_PresetDataSendPostioin=0;
				this->m_LocusRobotlocation.push_back(gCurrentRobotLocation);
				memset(&gCurrentRunCommand,0,sizeof(gCurrentRunCommand));
				isPresetCommand=false;
				GetLocalTime(&m_EndTime);

				softControl = 0;

			}
		}

		//通过软件控制
		else if (softControl == 1)
		{
			if (gCurrentRobotLocation.E1<-1400.0 || gCurrentRobotLocation.E1>1600.0
				|| gCurrentRobotLocation.A1<-55.0 || gCurrentRobotLocation.A1>85.0
				|| gCurrentRobotLocation.A2<-120.0 || gCurrentRobotLocation.A2>-90.0
				|| gCurrentRobotLocation.A3<35.0 || gCurrentRobotLocation.A3>115.0
				|| gCurrentRobotLocation.A4<-345.0 || gCurrentRobotLocation.A4>345.0
				|| gCurrentRobotLocation.A5<-95.0 || gCurrentRobotLocation.A5>95.0
				|| gCurrentRobotLocation.A6<-175.0 || gCurrentRobotLocation.A6>175.0)
			{
				double runCommand;
				//E1
				if (gCurrentRobotLocation.E1 < -1400.0)
				{
					tempState = 1;
					if (gCurrentRunCommand.E1 > 0)
					{
						runCommand = gCurrentRunCommand.E1;
						memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
						gCurrentRunCommand.E1 = runCommand;
					}
					else
					{
						memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
					}
				}
				else if (gCurrentRobotLocation.E1 > 1600.0)
				{
					tempState = 2;
					if (gCurrentRunCommand.E1 < 0)
					{
						runCommand = gCurrentRunCommand.E1;
						memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
						gCurrentRunCommand.E1 = runCommand;
					}
					else
					{
						memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
					}
				}
				//A1
				else if (gCurrentRobotLocation.A1 < -55.0)
				{
					tempState = 3;
					if (gCurrentRunCommand.A1 > 0)
					{
						runCommand = gCurrentRunCommand.A1;
						memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
						gCurrentRunCommand.A1 = runCommand;
					}
					else
					{
						memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
					}
				}
				else if (gCurrentRobotLocation.A1 > 85.0)
				{
					tempState = 4;
					if (gCurrentRunCommand.A1 < 0)
					{
						runCommand = gCurrentRunCommand.A1;
						memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
						gCurrentRunCommand.A1 = runCommand;
					}
					else
					{
						memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
					}
				}
				//A2
				else if (gCurrentRobotLocation.A2 < -120.0)
				{
					tempState = 5;
					if (gCurrentRunCommand.A2 > 0)
					{
						runCommand = gCurrentRunCommand.A2;
						memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
						gCurrentRunCommand.A2 = runCommand;
					}
					else
					{
						memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
					}
				}
				else if (gCurrentRobotLocation.A2 > -80.0)
				{
					tempState = 6;
					if (gCurrentRunCommand.A2 < 0)
					{
						runCommand = gCurrentRunCommand.A2;
						memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
						gCurrentRunCommand.A2 = runCommand;
					}
					else
					{
						memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
					}
				}
				//A3
				else if (gCurrentRobotLocation.A3 < 35.0)
				{
					tempState = 7;
					if (gCurrentRunCommand.A3 > 0)
					{
						runCommand = gCurrentRunCommand.A3;
						memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
						gCurrentRunCommand.A3 = runCommand;
					}
					else
					{
						memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
					}
				}
				else if (gCurrentRobotLocation.A3 > 115.0)
				{
					tempState = 8;
					if (gCurrentRunCommand.A3 < 0)
					{
						runCommand = gCurrentRunCommand.A3;
						memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
						gCurrentRunCommand.A3 = runCommand;
					}
					else
					{
						memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
					}
				}
				//A4
				else if (gCurrentRobotLocation.A4 < -345.0)
				{
					tempState = 9;
					if (gCurrentRunCommand.A4 > 0)
					{
						runCommand = gCurrentRunCommand.A4;
						memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
						gCurrentRunCommand.A4 = runCommand;
					}
					else
					{
						memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
					}
				}
				else if (gCurrentRobotLocation.A4 > 345.0)
				{
					tempState = 10;
					if (gCurrentRunCommand.A4 < 0)
					{
						runCommand = gCurrentRunCommand.A4;
						memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
						gCurrentRunCommand.A4 = runCommand;
					}
					else
					{
						memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
					}
				}
				//A5
				else if (gCurrentRobotLocation.A5 < -95.0)
				{
					tempState = 11;
					if (gCurrentRunCommand.A5 > 0)
					{
						runCommand = gCurrentRunCommand.A5;
						memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
						gCurrentRunCommand.A5 = runCommand;
					}
					else
					{
						memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
					}
				}
				else if (gCurrentRobotLocation.A5 > 95.0)
				{
					tempState = 12;
					if (gCurrentRunCommand.A5 < 0)
					{
						runCommand = gCurrentRunCommand.A5;
						memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
						gCurrentRunCommand.A5 = runCommand;
					}
					else
					{
						memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
					}
				}
				//A6
				else if (gCurrentRobotLocation.A6 < -175.0)
				{
					tempState = 13;
					if (gCurrentRunCommand.A6 > 0)
					{
						runCommand = gCurrentRunCommand.A6;
						memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
						gCurrentRunCommand.A6 = runCommand;
					}
					else
					{
						memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
					}
				}
				else if (gCurrentRobotLocation.A6 > 175.0)
				{
					tempState = 14;
					if (gCurrentRunCommand.A6 < 0)
					{
						runCommand = gCurrentRunCommand.A6;
						memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
						gCurrentRunCommand.A6 = runCommand;
					}
					else
					{
						memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
					}
				}
				else
				{
					memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
				}

				::PostMessageA(this->m_MainHwnd, WM_LIMIT_STATE, tempState, 0);

				return true;
			}
			//通过摇杆实时控制。
			else{

				gCurrentRunCommand = m_RockerModule.ObtainOperatingCommand();

				if (gCurrentRobotLocation.E1<-1400.0 || gCurrentRobotLocation.E1>1600.0
					|| gCurrentRobotLocation.A1<-55.0 || gCurrentRobotLocation.A1>85.0
					|| gCurrentRobotLocation.A2<-120.0 || gCurrentRobotLocation.A2>-90.0
					|| gCurrentRobotLocation.A3<35.0 || gCurrentRobotLocation.A3>115.0
					|| gCurrentRobotLocation.A4<-345.0 || gCurrentRobotLocation.A4>345.0
					|| gCurrentRobotLocation.A5<-95.0 || gCurrentRobotLocation.A5>95.0
					|| gCurrentRobotLocation.A6<-175.0 || gCurrentRobotLocation.A6>175.0)
				{
					double runCommand;
					//E1
					if (gCurrentRobotLocation.E1 < -1400.0)
					{
						tempState = 1;
						if (gCurrentRunCommand.E1 > 0)
						{
							runCommand = gCurrentRunCommand.E1;
							memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
							gCurrentRunCommand.E1 = runCommand;
						}
						else
						{
							memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
						}
					}
					else if (gCurrentRobotLocation.E1 > 1600.0)
					{
						tempState = 2;
						if (gCurrentRunCommand.E1 < 0)
						{
							runCommand = gCurrentRunCommand.E1;
							memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
							gCurrentRunCommand.E1 = runCommand;
						}
						else
						{
							memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
						}
					}
					//A1
					else if (gCurrentRobotLocation.A1 < -55.0)
					{
						tempState = 3;
						if (gCurrentRunCommand.A1 > 0)
						{
							runCommand = gCurrentRunCommand.A1;
							memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
							gCurrentRunCommand.A1 = runCommand;
						}
						else
						{
							memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
						}
					}
					else if (gCurrentRobotLocation.A1 > 85.0)
					{
						tempState = 4;
						if (gCurrentRunCommand.A1 < 0)
						{
							runCommand = gCurrentRunCommand.A1;
							memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
							gCurrentRunCommand.A1 = runCommand;
						}
						else
						{
							memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
						}
					}
					//A2
					else if (gCurrentRobotLocation.A2 < -120.0)
					{
						tempState = 5;
						if (gCurrentRunCommand.A2 > 0)
						{
							runCommand = gCurrentRunCommand.A2;
							memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
							gCurrentRunCommand.A2 = runCommand;
						}
						else
						{
							memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
						}
					}
					else if (gCurrentRobotLocation.A2 > -80.0)
					{
						tempState = 6;
						if (gCurrentRunCommand.A2 < 0)
						{
							runCommand = gCurrentRunCommand.A2;
							memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
							gCurrentRunCommand.A2 = runCommand;
						}
						else
						{
							memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
						}
					}
					//A3
					else if (gCurrentRobotLocation.A3 < 35.0)
					{
						tempState = 7;
						if (gCurrentRunCommand.A3 > 0)
						{
							runCommand = gCurrentRunCommand.A3;
							memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
							gCurrentRunCommand.A3 = runCommand;
						}
						else
						{
							memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
						}
					}
					else if (gCurrentRobotLocation.A3 > 115.0)
					{
						tempState = 8;
						if (gCurrentRunCommand.A3 < 0)
						{
							runCommand = gCurrentRunCommand.A3;
							memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
							gCurrentRunCommand.A3 = runCommand;
						}
						else
						{
							memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
						}
					}
					//A4
					else if (gCurrentRobotLocation.A4 < -345.0)
					{
						tempState = 9;
						if (gCurrentRunCommand.A4 > 0)
						{
							runCommand = gCurrentRunCommand.A4;
							memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
							gCurrentRunCommand.A4 = runCommand;
						}
						else
						{
							memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
						}
					}
					else if (gCurrentRobotLocation.A4 > 345.0)
					{
						tempState = 10;
						if (gCurrentRunCommand.A4 < 0)
						{
							runCommand = gCurrentRunCommand.A4;
							memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
							gCurrentRunCommand.A4 = runCommand;
						}
						else
						{
							memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
						}
					}
					//A5
					else if (gCurrentRobotLocation.A5 < -95.0)
					{
						tempState = 11;
						if (gCurrentRunCommand.A5 > 0)
						{
							runCommand = gCurrentRunCommand.A5;
							memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
							gCurrentRunCommand.A5 = runCommand;
						}
						else
						{
							memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
						}
					}
					else if (gCurrentRobotLocation.A5 > 95.0)
					{
						tempState = 12;
						if (gCurrentRunCommand.A5 < 0)
						{
							runCommand = gCurrentRunCommand.A5;
							memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
							gCurrentRunCommand.A5 = runCommand;
						}
						else
						{
							memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
						}
					}
					//A6
					else if (gCurrentRobotLocation.A6 < -175.0)
					{
						tempState = 13;
						if (gCurrentRunCommand.A6 > 0)
						{
							runCommand = gCurrentRunCommand.A6;
							memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
							gCurrentRunCommand.A6 = runCommand;
						}
						else
						{
							memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
						}
					}
					else if (gCurrentRobotLocation.A6 > 175.0)
					{
						tempState = 14;
						if (gCurrentRunCommand.A6 < 0)
						{
							runCommand = gCurrentRunCommand.A6;
							memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
							gCurrentRunCommand.A6 = runCommand;
						}
						else
						{
							memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
						}
					}
					else
					{
						memset(&gCurrentRunCommand, 0, sizeof(gCurrentRunCommand));
					}

					::PostMessageA(this->m_MainHwnd, WM_LIMIT_STATE, tempState, 0);
					return true;
				}
			}
		}
	}
		return true;
}

//通过摇杆更新当前机器人控制各种状态。
void CAppWorkPlant::UpdateControlStateFromRocker(){
	this->m_RockerModule.UpDataRockerData();

}


//支撑摇杆指令控制状态。
bool CAppWorkPlant::compareReceive(string String1, string String2)
{
	return 0;
}
