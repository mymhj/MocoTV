#pragma once

#include <dinput.h>
#define WM_USER_COMMONDEVENT WM_USER + 2004  //��Ϣmsg
#define WM_STICK_EVENT WM_USER + 3000  //��Ϣmsg
#define DIRECTINPUT_VERSION 0x0800  //
const double ROBOTMINSTEP= 0.02;//��С����



class CRockerControlModule
{
public:
	CRockerControlModule(void);

	~CRockerControlModule(void);
	//��ʼ������
	bool Init(HWND hHwnd); 

	//�洢��Ϸҡ�˵�״̬��Ϣ
	DIJOYSTATE2 m_RockerState; 

	//lpdirectinputdevice8 DIDevice8�ӿ�ָ��
	LPDIRECTINPUTDEVICE8 m_IDirectInputDevice8; 

	//����ҡ��ָ�����ݡ�m_RockerState
	BOOL UpDataRockerData();

	////��ȡ����ָ��״̬
	//modes ObtainControlCommand();

	//��ȡ����ָ������
	robotRunCommand ObtainOperatingCommand();
	//��ҡ����ĵ�ǰֵ�����趨����ת��Ϊ��Ӧ�Ļ�����A1...A6���Ӧ��ָ��ֵ��
	float ConvertJoystickToRobotAx(long joystickAxisValue);

	//��ҡ����ĵ�ǰֵ�����趨����ת��Ϊ��Ӧ�Ļ�����E1���Ӧ��ָ��ֵ��
	float CRockerControlModule::ConvertJoystickToRobotEx(long joystickAxisValue);

	//ö���豸  //�ص����� ----Ŀǰ�Գ���û̫��Ӱ�졣���Բ���ע��
	static BOOL CALLBACK DIEnumDevicesCallback(const DIDEVICEINSTANCE* lpddi, VOID* pvRef); //ö�ٶ���


	static volatile LONG m_isPressed;
	static volatile LONG m_BtnIndex;

	//�жϸ����Ƿ���ֵ
	void axisValue();

	//����ֵ�˶������𲽽���
	float speedDown();

	//����ϵ��
	float temp=1;

	void caculate(float X, float Y, float Z, double A, double B, double C);

	void test();
	
};

