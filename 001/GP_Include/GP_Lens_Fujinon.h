#pragma once
#include <GP_Thread.h>
#include <GP_SeriPort.h>
 


 


#define MIN_ZOOM	0
#define MAX_ZOOM	4095
#define MIN_FOCUS	0
#define MAX_FOCUS	4095
#define MIN_IRIS	0
#define MAX_IRIS	4095


#define   ConnectLens				 0x01
#define   LensName1					 0x11
#define   LensName2					 0x12
#define   OpenFNo					 0x13
#define   TeleendFocalLength		 0x14
#define   WideendFocalLength		 0x15
#define   MOD						 0x16
#define   IrisControl				 0x20
#define   ZoomControl				 0x21
#define   FocusControl				 0x22
#define   ZoomSpeedControl			 0x26
#define   FocusSpeedControl			 0x27
#define   IrisPosition			     0x30
#define   ZoomPosition			     0x31
#define   FocusPosition			     0x32
//����������
#define   MultipleData				 0x60
#define   MultipleDataSet			 0x70


 
class GP_DLL_API GP_Lens_Fujinon : public GP_SeriPort
{
public:
	GP_Lens_Fujinon();
	~GP_Lens_Fujinon();

	//���ڱ��ػ���-����
	byte m_SendDataBuffer[25];
	//���ڱ��ػ���-����
	byte m_ReceiveDataBuffer[25];
	//�������ݼ�����
	LONGLONG m_SendCounter;
	//�������ݼ�����
	LONGLONG m_ReceiveCounter;

public:

	//********************************************************************
	// ����ʱ�䣺  2016/07/08  15:10 
	// ��    �ߣ�  GP_Fore
	//********************************************************************
	// ����˵����  ���Դ򿪵Ĵ����Ƿ��Ѿ��뾵ͷ�ŷ��������ӡ�
	// ����ֵ:     int __stdcall
	// ��    ע��        
	//********************************************************************
	int __stdcall IsConnect();

	//********************************************************************
	// ����ʱ�䣺  2016/07/01  17:05 
	// ��    �ߣ�  GP_Fore
	//********************************************************************
	// ����˵����  ������������飬���ͨ���˶����ṩ�ĺ����ſ��á�
	// �����б�  TCHAR * pCommName
	// �����б�  UINT pBaudRate
	// �����б�   UINT pParity ��żУ��  NOPARITY:0������żУ�飩 ODDPARITY:1 ����
	// �����б�  UINT pByteSize
	// �����б�  UINT pStopBits  ע��0(ONESTOPBIT)������һ��ֹͣλ��2(TWOSTOPBITS):��������ֹͣλ�� ��֧��1.5��ֹͣλ  Ĭ��Ϊ1��ֹͣλ
	// ����ֵ:     bool __stdcall
	// ��    ע��        
	//********************************************************************
	int __stdcall VerifyPreScene(TCHAR* pCommName, UINT pBaudRate = CBR_38400, UINT pParity = NOPARITY, UINT  pByteSize = 8, UINT pStopBits = ONESTOPBIT);

	//********************************************************************
	// ����ʱ�䣺  2016/07/01  17:06 
	// ��    �ߣ�  GP_Fore
	//********************************************************************
	// ����˵����  ��þ�ͷ�����ֺ��ͺ�
	// �����б�  LPTSTR sStr
	// ����ֵ:     int __stdcall
	// ��    ע��  ����һ����������Ϊ50���ַ��Ŀ��ַ�����    
	//********************************************************************
	int __stdcall GetLensName(char* pLensName);


	//********************************************************************
	// ����ʱ�䣺  2016/09/19  11:34 
	// ��    �ߣ�  GP_Fore
	//********************************************************************
	// ����˵����  
	// �����б�  WORD & nZoomValue
	// �����б�  WORD & nFocusValue
	// �����б�  WORD & nIrisValue
	// ����ֵ:     int __stdcall
	// ��    ע��        
	//********************************************************************
	int __stdcall GetLensMotorZoomFocusIris(WORD &nZoomValue, WORD &nFocusValue, WORD &nIrisValue);
	int __stdcall SetLensMotorZoomFocusIris(WORD nZoomValue, WORD nFocusValue, WORD nIrisValue);

	//********************************************************************
	// ����ʱ�䣺  2016/09/20  15:42 
	// ��    �ߣ�  GP_Fore
	//********************************************************************
	// ����˵����  
	// �����б�  WORD & nZoomValue
	// �����б�  WORD & nFocusValue
	// ����ֵ:     int __stdcall
	// ��    ע��        
	//********************************************************************
	int __stdcall GetLensMotorZoomFocus(WORD &nZoomValue, WORD &nFocusValue);
	int __stdcall SetLensMotorZoomFocus(WORD nZoomValue, WORD nFocusValue);
 
	//********************************************************************
	// ����ʱ�䣺  2016/09/19  11:34 
	// ��    �ߣ�  GP_Fore
	//********************************************************************
	// ����˵����  
	// �����б�  WORD & ResultValue   ��ж������ݡ�
	// �����б�  WORD & OriginalValue ֱ�ӽ��ܵ������ݣ�û�н���2���ֽڡ�
	// ����ֵ:     int __stdcall
	// ��    ע��        
	//********************************************************************
	int __stdcall GetLensMotorZoom(WORD &ResultValue, WORD &OriginalValue);
	int __stdcall GetLensMotorFocus(WORD &nValue);
	int __stdcall GetLensMotorIris(WORD &nValue);


	int __stdcall SetLensMotorZoom(WORD nValue, WORD &OriginalValue);
	int __stdcall SetLensMotorFocus(WORD nValue);
	int __stdcall SetLensMotorIris(WORD nValue);



	
	//********************************************************************
	// ����ʱ�䣺  2016/07/08  15:29 
	// ��    �ߣ�  GP_Fore
	//********************************************************************
	// ����˵����  ���ƽ������߲���
	// �����б�  BYTE nValue
	// ����ֵ:     int __stdcall
	// ��    ע��        
	//********************************************************************
	int __stdcall SetSwitch0Host(BYTE nValue);
	int __stdcall GetSwitch0Host(char* sNote, WORD &nValue);

	//********************************************************************
	// ����ʱ�䣺  2016/07/08  15:30 
	// ��    �ߣ�  GP_Fore
	//********************************************************************
	// ����˵����  ����Iris���Զ���Զ�̿��Ʒ�ʽ���Ѿ������͹رա�
	// �����б�  BYTE nValue
	// ����ֵ:     int __stdcall
	// ��    ע��  0xCF:iris�Զ�����Ȧ�ر�ģʽ��  11001111   0xEF�Զ�����Ȧ����ģʽ
	// ��    ע��  0xDF:irisԶ�̿��ƹر�ģʽ��  11011111   0xFFԶ�̿��ƿ���ģʽ
	//********************************************************************
	int __stdcall SetSwitch2Host(BYTE nValue);
	int __stdcall GetSwitch2Host(char* sNote, WORD &nValue);

	//********************************************************************
	// ����ʱ�䣺  2016/07/08  15:31 
	// ��    �ߣ�  GP_Fore
	//********************************************************************
	// ����˵����  ��ʾ���Ʒ���
	// �����б�  BYTE nValue
	// ����ֵ:     int __stdcall
	// ��    ע��        
	//********************************************************************
	int __stdcall SetSwitch3Host(BYTE nValue);
	int __stdcall GetSwitch3Host(char* sNote, WORD &nValue);


	//********************************************************************
	// ����ʱ�䣺  2016/07/08  15:34 
	// ��    �ߣ�  GP_Fore
	//********************************************************************
	// ����˵����  ����Iris,Zoom,FocusΪ����ģʽ��������λ��ģʽ
	// �����б�  BYTE nValue
	// ����ֵ:     int __stdcall
	// ��    ע��  0xFC:iris����ģʽ��focus��zoom��λ��ģʽ��  11111100
	// ��    ע��  0xFA:zoom����ģʽ��focus��iris��λ��ģʽ��  11111010
	// ��    ע��  0xF9:focus����ģʽ��zoom��iris��λ��ģʽ��  11111001
	//********************************************************************
	int __stdcall SetSwitch4Host(BYTE nValue);
	int __stdcall GetSwitch4Host(char* sNote, WORD &nValue);

};

