#pragma once
#include "CFuSerialPort.h" 



class  GP_Lens_Canon
{
public:
	GP_Lens_Canon();
	~GP_Lens_Canon();

	//���ڱ��ػ���-����
	byte m_SendDataBuffer[30];
	//���ڱ��ػ���-����
	byte m_ReceiveDataBuffer[50];
	//�������ݼ�����
	LONGLONG m_SendCounter;
	//�������ݼ�����
	LONGLONG m_ReceiveCounter;

public:

	//bool __stdcall VerifyPreScene(TCHAR* pCommName, UINT pBaudRate = CBR_38400, UINT pParity = NOPARITY, UINT  pByteSize = 8, UINT pStopBits = ONESTOPBIT);

	bool InitCommPort(CString PortNo, UINT  baud);

	bool __stdcall FocusControllerCommd();
	bool __stdcall ZoomControllerCommd();
	bool __stdcall PcControllerCommd();
	bool __stdcall LensVersionRequest(char* pLensVersionNumber);
	bool __stdcall LensTypeRequest(char* pLensTypeRequest);
	bool __stdcall InitCompletedCommd();

	int __stdcall IrisPriorityCommd(WORD nValue);
	int __stdcall IncommCommd(WORD nValue);
	int __stdcall IECommd(WORD nValue);
	int __stdcall VersionCommd();

	bool __stdcall GetControlSwitchCommd(WORD nValue);
	WORD __stdcall PcSwitchCommd(WORD nValue);

	int __stdcall GetLensMotorZoom(WORD &nValue);
	int __stdcall GetLensMotorFocus(WORD &nValue);
	int __stdcall GetLensMotorIris(WORD &nValue);
	
	int __stdcall SetLensMotorZoom(WORD nValue);
	int __stdcall SetLensMotorFocus(WORD nValue);
	int __stdcall SetLensMotorIris(WORD nValue);

	int __stdcall ZoomSpeedSet(WORD nValue);
	int __stdcall FocusSpeedSet(WORD nValue);
	void Delay(int MilliSeconds);
	CFuSerialPort* m_SerialPort;
};	


