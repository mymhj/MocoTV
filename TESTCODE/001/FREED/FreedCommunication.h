#ifndef COMMUNICATION
#define COMMUNICATION

//��Ϣ����
#define   StopStreamMode					 0x00
#define   StartStreamMode					 0x01
#define   StopFreezeMode					 0x02
#define   StartFreezeMode					 0x03
#define   PollForPositionUpdate				 0xD1
#define   RequestSystemStatus				 0xD2
#define   RequestSystemParameters			 0xD3
#define   RequestFirstTargetData			 0xD4
#define   RequestNextTargetPata				 0xD5
#define   RequestFirstImagePoint			 0xD6
#define   RequestNextImagePoint				 0xD7
#define   RequestNextEEPROMData				 0xD8
#define   RequestCameraCalibrationValues	 0xDA
#define   RequestDiagnosticMode				 0xDB



#include <windows.h>

//��Ϣ���ݽṹ
typedef struct {
	BYTE MsgType;//��Ϣ����
	BYTE CameraID;//�����id
	BYTE CMD;//��������
	BYTE CheckSum;//У��λ
	//������Ƕ� ��λ���Ƕȡ�   �������ݴ�С��3Byte   ��λ���壺bit23������λ   bit22~15 8λ������bit14~0С��
	//����+180��= 5A0000 hex = 0 10110100 000000000000000 = + 180 . 0*1/32768 ʮ����
	float CameraPanAngle;//<PH><PM><PL>  Camera Pan Angle     ��Χ��-180�� ~ +180��
	int CameraPanAngle_2;//2���Ʊ�ʾ
	float CameraTiltAngle;//<TH><TM><TL>  Camera Tilt Angle   ��Χ��-90��  ~ +90��
	int CameraTiltAngle_2;//2���Ʊ�ʾ
	float CameraRollAngle;//<RH><RM><RL>  Camera Roll Angle   ��Χ��-180�� ~ +180��
	int CameraRollAngle_2;//2���Ʊ�ʾ
	//�����λ�� ��λ������mm    �������ݴ�С��3Byte   ��λ���壺bit23������λ   bit22~6 ������bit5~0С��
	//����+131,072.0 mm  = 7FFFFF hex = 0 11111111111111111 111111 = + 131071 . 63*1/64  ʮ����
	float CameraXPosition;//<XH><XM><XL>  Camera X-Position          ��Χ��-131,072.0 mm ~ +131,072.0 mm
	int CameraXPosition_2;
	float CameraYPosition;//<YH><YM><YL>  Camera Y-Position          ��Χ��-131,072.0 mm ~ +131,072.0 mm
	int CameraYPosition_2;
	float CameraZPosition;//<HH><HM><HL>  Camera Height (Z-Position) ��Χ��-131,072.0 mm ~ +131,072.0 mm
	int CameraZPosition_2;
	//�����zoom��focus  �������ݴ�С��3Byte  ���ݲ�ͬ��ͷ���辵ͷ�궨��ת��Ϊʵ��zoom focusֵ
	WORD CameraZoom;//<ZH><ZM><ZL> Camera Zoom 
	WORD CameraFocus;//<FH><FM><FL>  Camera Focus 
private:
	int convert_angle_to_byte(float data);
	int convert_position_to_byte(float data);
	int convert_camera_to_byte(float data);
} FreedMsg;

class FCom
{
public:
	BOOL InitPortF(int port);//�򿪴��ڣ����ô������ò���
	int m_COMWport;//�˿ں�
	DWORD dwBaudrate;//������

	void Close();//�رմ���
	
	void SendOMsg();//����offset��Ϣ
	void RecMsg();//������Ϣ

	WORD wMsgSent;//���͵���Ϣ��������
	WORD wMsgRec;//������Ϣ��������
	BYTE cTxBuffer[50];//������Ϣ����
	BYTE cRxBuf[50];//������Ϣ����

	FreedMsg msg;
};
#endif










