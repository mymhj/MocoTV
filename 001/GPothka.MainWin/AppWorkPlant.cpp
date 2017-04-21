#pragma once
#include "stdafx.h"
#include "AppWorkPlant.h"
#include "GPothka.MainWinDlg.h"

//�������ݰ�����
long CAppWorkPlant::m_ReceivedDataPackageCount;

//�������ݰ�����
long CAppWorkPlant::m_SendDataPackageCount;

///��ǰ֡���͸������˵����ݡ�һ����������
string CAppWorkPlant::m_localSendCompletePackageString;

//�Ƿ�ʹ��Ԥ��ָ��,true �ǣ� false:����
bool CAppWorkPlant::isPresetCommand;

bool CAppWorkPlant::cmotion_end_repeat;

//��ǰ����ã������͸������˵����ݡ�
vector<robotRunCommand> CAppWorkPlant::m_PresetSendDataColl;

//Maya�����ָ������͸������˵����ݡ�
 vector<robotRunCommand> CAppWorkPlant::m_MayaSendDataColl;

//��¼��ǰ����Ԥ��ָ���������
int CAppWorkPlant::m_PresetDataSendPostioin;

	//�켣�ظ��еĻ�����λ����Ϣ
vector<robotlocation> CAppWorkPlant::m_LocusRobotlocation;
//
	//�켣�ظ��л����˵�����ָ����Ϣ
vector<robotRunCommand>  CAppWorkPlant::m_LocusRunCommand;

//����̨���캯��
CAppWorkPlant::CAppWorkPlant(void){	
	isPresetCommand=false;
	this->m_PresetSendDataColl.reserve(10000);
	cmotion_end_repeat=false;
}

//����̨��������
CAppWorkPlant::~CAppWorkPlant(void){
}

//����ͨѶ������֤ϵͳ���������Ƿ��Ѿ�������֡�������ַ���ture��false����ʧ�ܡ�
bool CAppWorkPlant::StartServer(HWND pMainhWnd,HWND pVideoHwnd){

	//�����˿ͻ��˼����߳�
	HANDLE hThead1,hThead2;
	this->m_MainHwnd=pMainhWnd;

	if(m_RockerModule.Init(pMainhWnd)==false)
	{
		//SendLogToWindows(m_MainHwnd,"������ʧ�ܣ�����ҡ���豸�Ƿ�����������");
		return false;
	}

	///���������˿ͻ��˼���߳�
	hThead1=(HANDLE)_beginthread(ThreadAssistFunc1,0,(PVOID)this);

	///���������߼���ָ��ά���߳�
	hThead2=(HANDLE)_beginthread(ThreadAssistFunc2,0,(PVOID)this);

	DWORD lpExitCode1,lpExitCode2;

	GetExitCodeThread(hThead1,&lpExitCode1);
	GetExitCodeThread(hThead2,&lpExitCode2);

	ResumeThread(hThead1);
	ResumeThread(hThead2);
	return true;
}

//����ͨѶ�߳�
void CAppWorkPlant::ThreadAssistFunc1(void *para){
	CAppWorkPlant result=*((CAppWorkPlant*)para);
	result.RobotClientListener();
}

//����ҵ���߳�
void CAppWorkPlant::ThreadAssistFunc2(void *para){
	CAppWorkPlant result=*((CAppWorkPlant*)para);
	result.BusinesslogicMaintain();
}


//�����˶˼��������������˺͵�ǰ������ͨѶ��
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
//		SendLogToWindows(m_MainHwnd,"��������Sockect���ʧ�ܣ����������Ƿ��ã�");
//		return;
//	}
//	SendLogToWindows(m_MainHwnd,"�ɹ�����ͨѶ����");
//	::listen(listenerSocket,5);
//	fd_set socket_Set;		// ����һ��fd_set�ṹ�ı��������ڴ洢socket�����
//	FD_ZERO(&socket_Set);     //���socket_set���ϡ�
//	FD_SET(listenerSocket, &socket_Set); //��listenerSocket����socket_set�С�
//	while(TRUE)
//	{
//		
//		fd_set readfds = socket_Set;  //��socket_Set�����е�socket�������һ�ݵ�readfds�У������¼�����ʱ��select�����Ƴ�readfds������û��δ��I/O�������׽��־����Ȼ�󷵻�
//		if(::select(0, &readfds, NULL, NULL, NULL) > 0)
//		{
//			// ͨ����ԭ��socket_Set������select�������readfds���ϱȽϣ�
//			// ȷ��������Щ�׽�����δ��I/O������һ��������ЩI/O��
//			for(int i=0; i<(int)socket_Set.fd_count; i++)
//			{
//				if(FD_ISSET(socket_Set.fd_array[i], &readfds)) //��⵱ǰsocket����ڵ���select����֮���Ƿ����仯�������仯ʱ����Ϊ�档
//				{
//					if(socket_Set.fd_array[i] == listenerSocket) //�����׽��ֽ��յ�������
//					{
//						//��֤�Ƿ����µĿͻ�������������ӡ�
//						if(socket_Set.fd_count < FD_SETSIZE)
//						{
//							sockaddr_in addrRemote;
//							int nAddrLen = sizeof(addrRemote);
//							SOCKET newSocket = ::accept(listenerSocket, (SOCKADDR*)&addrRemote, &nAddrLen);
//							FD_SET(newSocket, &socket_Set);
//							char * c_IP =inet_ntoa(addrRemote.sin_addr);
//							char c_logInfo[100]="���ն��豸��";
//						    strcat_s(c_logInfo,100,c_IP);
//							strcat_s(c_logInfo,100,"������ͨѶ���ӣ���ʼ�շ����ݡ�");
//							SendLogToWindows(m_MainHwnd,c_logInfo);
//						}
//						else
//						{
//							continue;
//						}
//					}
//					else
//					{
//						//�Ѿ��������ӵĻ����˺͵�ǰ��������ѭ��ͨѶ��
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

//����UDPͨѶ
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
	SOCKADDR_IN clientAddr;//�ͻ��˵�ַ��

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
			//��������ʧ��s
			return;
		}
			

		sprintf_s(robotRunCommand_char, 1000, gRunCommand_format_str,
			gCurrentRunCommand.X, gCurrentRunCommand.Y, gCurrentRunCommand.Z, gCurrentRunCommand.A, gCurrentRunCommand.B, gCurrentRunCommand.C,
			gCurrentRunCommand.A1, gCurrentRunCommand.A2, gCurrentRunCommand.A3, gCurrentRunCommand.A4, gCurrentRunCommand.A5, gCurrentRunCommand.A6,
			gCurrentRunCommand.E1,
			str);

	

		int sendLength = sendto(m_ServerSock, robotRunCommand_char, strlen(robotRunCommand_char) + 1, 0, (SOCKADDR*)&clientAddr, clientAddr_len);

		if (sendLength == SOCKET_ERROR) //�������ݳ��� 
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


//ҵ���߼����̣��Ƕ�����ͨѶ���̵ĵĽ��̵�Ԫ��
void CAppWorkPlant::BusinesslogicMaintain(void){
	while (true)
	{
		Sleep(100);
		compareReceive("","");
	}
}


//�߼����ƺ�����ͨ��Ԥ��״̬����ϵͳ��ǰִ�е���ʵʱ���ơ�Ԥ����ơ���ͣ�Ȳ�����
bool CAppWorkPlant::RobotControlCenter()
{
	//ͨ����ȡҡ�����ݸ��µ�ǰϵͳ�߼�����״̬
	this->UpdateControlStateFromRocker();

	if(gRunState.Suspend==true){
		
		memset(&gCurrentRunCommand,0,sizeof(gCurrentRunCommand));	

		//�޸���ͣ����Ϊ��ͣ�����Ԥ��ָ��
		m_PresetSendDataColl.clear();

		return true;
	}
	else
	{
		//�����true,��ô��ǰִ��Ԥ��ָ����ܵ�ǰ��ִ�У������˶�����λ�˶����켣�ظ��е�һ���˶���
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

		//ͨ���������
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
			//ͨ��ҡ��ʵʱ���ơ�
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

//ͨ��ҡ�˸��µ�ǰ�����˿��Ƹ���״̬��
void CAppWorkPlant::UpdateControlStateFromRocker(){
	this->m_RockerModule.UpDataRockerData();

}


//֧��ҡ��ָ�����״̬��
bool CAppWorkPlant::compareReceive(string String1, string String2)
{
	return 0;
}
