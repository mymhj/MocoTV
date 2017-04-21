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

	//��������
	HWND m_MainHwnd; 

	//ҡ�˿���ģ��
	CRockerControlModule m_RockerModule;

	//�켣�ظ�ģ��
	CTrackRepeatModule m_TrackRepeatModule;

	//�������ݰ�����
	static long m_ReceivedDataPackageCount;

	///��ǰ֡���͸������˵����ݡ�һ����������
	static string m_localSendCompletePackageString;

	//�������ݰ�����
	static long m_SendDataPackageCount;
	///�Ƿ�Ԥ��ָ�
	static bool isPresetCommand;

	//��ǰ����ã������͸������˵����ݡ�
	static vector<robotRunCommand> m_PresetSendDataColl;

	//Maya�����ָ������͸������˵����ݡ�
	static vector<robotRunCommand> m_MayaSendDataColl;

	//��¼��ǰ���͵��˵ڼ�֡��
	static int m_PresetDataSendPostioin;

	//�켣�ظ��еĻ�����λ����Ϣ
	static vector<robotlocation> m_LocusRobotlocation;

	//�켣�ظ��л����˵�����ָ����Ϣ
	static vector<robotRunCommand> m_LocusRunCommand;

	SYSTEMTIME m_StartTime;

	SYSTEMTIME m_EndTime;

	//���·�������ʵ�壻
	bool RobotControlCenter();


private:

	//�̸߳�������1-ͨѶ��
	static void __cdecl  ThreadAssistFunc1(void *para);
	//�̸߳�������1-�߼����ơ�
	static void __cdecl  ThreadAssistFunc2(void *para);

	//�����˶˼��������������˺͵�ǰ������ͨѶ��
	void RobotClientListener(void);
	//ҵ���߼����̣��Ƕ�����ͨѶ���̵ĵĽ��̵�Ԫ��
    void BusinesslogicMaintain(void);
	//ִ��ÿ��12������ܻ�����״̬���ݺͷ��͸�������ָ�����ݡ�
	void LoopReceiveAndSendFromRobot(SOCKET robotClientSocket);
	//������ܵ��Ĳ������Ļ����������ݡ�-���淶���ݵĴ���
	bool DelayStateHandle(char * robotLocation_char);
	//ͨ��ҡ�˸��µ�ǰ�����˿��Ƹ���״̬��
	void UpdateControlStateFromRocker();
	//֧��ҡ��ָ�����״̬��
	bool compareReceive(string String1, string String2);

public:
	static bool cmotion_end_repeat;
	sockaddr_in m_servAddr;
	SOCKET m_ServerSock;
};

