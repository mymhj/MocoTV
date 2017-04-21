#ifndef COMMUNICATION
#define COMMUNICATION

//消息命令
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

//消息数据结构
typedef struct {
	BYTE MsgType;//消息类型
	BYTE CameraID;//摄像机id
	BYTE CMD;//命令内容
	BYTE CheckSum;//校验位
	//摄像机角度 单位：角度°   传输数据大小：3Byte   各位含义：bit23：符号位   bit22~15 8位整数，bit14~0小数
	//例：+180°= 5A0000 hex = 0 10110100 000000000000000 = + 180 . 0*1/32768 十进制
	float CameraPanAngle;//<PH><PM><PL>  Camera Pan Angle     范围：-180° ~ +180°
	int CameraPanAngle_2;//2进制表示
	float CameraTiltAngle;//<TH><TM><TL>  Camera Tilt Angle   范围：-90°  ~ +90°
	int CameraTiltAngle_2;//2进制表示
	float CameraRollAngle;//<RH><RM><RL>  Camera Roll Angle   范围：-180° ~ +180°
	int CameraRollAngle_2;//2进制表示
	//摄像机位置 单位：毫米mm    传输数据大小：3Byte   各位含义：bit23：符号位   bit22~6 整数，bit5~0小数
	//例：+131,072.0 mm  = 7FFFFF hex = 0 11111111111111111 111111 = + 131071 . 63*1/64  十进制
	float CameraXPosition;//<XH><XM><XL>  Camera X-Position          范围：-131,072.0 mm ~ +131,072.0 mm
	int CameraXPosition_2;
	float CameraYPosition;//<YH><YM><YL>  Camera Y-Position          范围：-131,072.0 mm ~ +131,072.0 mm
	int CameraYPosition_2;
	float CameraZPosition;//<HH><HM><HL>  Camera Height (Z-Position) 范围：-131,072.0 mm ~ +131,072.0 mm
	int CameraZPosition_2;
	//摄像机zoom和focus  传输数据大小：3Byte  根据不同镜头（需镜头标定）转换为实际zoom focus值
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
	BOOL InitPortF(int port);//打开串口，设置串口配置参数
	int m_COMWport;//端口号
	DWORD dwBaudrate;//波特率

	void Close();//关闭串口
	
	void SendOMsg();//发送offset消息
	void RecMsg();//接收消息

	WORD wMsgSent;//发送的消息数计数器
	WORD wMsgRec;//接受消息数计数器
	BYTE cTxBuffer[50];//发送消息缓存
	BYTE cRxBuf[50];//接受消息缓存

	FreedMsg msg;
};
#endif










