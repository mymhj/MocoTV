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
//……待补充
#define   MultipleData				 0x60
#define   MultipleDataSet			 0x70


 
class GP_DLL_API GP_Lens_Fujinon : public GP_SeriPort
{
public:
	GP_Lens_Fujinon();
	~GP_Lens_Fujinon();

	//串口本地缓存-发送
	byte m_SendDataBuffer[25];
	//串口本地缓存-接受
	byte m_ReceiveDataBuffer[25];
	//发送数据计数器
	LONGLONG m_SendCounter;
	//接受数据计数器
	LONGLONG m_ReceiveCounter;

public:

	//********************************************************************
	// 创建时间：  2016/07/08  15:10 
	// 作    者：  GP_Fore
	//********************************************************************
	// 函数说明：  测试打开的串口是否已经与镜头伺服建立连接。
	// 返回值:     int __stdcall
	// 备    注：        
	//********************************************************************
	int __stdcall IsConnect();

	//********************************************************************
	// 创建时间：  2016/07/01  17:05 
	// 作    者：  GP_Fore
	//********************************************************************
	// 函数说明：  对象工作场景检查，检查通过此对象提供的函数才可用。
	// 参数列表：  TCHAR * pCommName
	// 参数列表：  UINT pBaudRate
	// 参数列表：   UINT pParity 奇偶校验  NOPARITY:0（不奇偶校验） ODDPARITY:1 ……
	// 参数列表：  UINT pByteSize
	// 参数列表：  UINT pStopBits  注：0(ONESTOPBIT)：代表一个停止位，2(TWOSTOPBITS):代表两个停止位。 不支持1.5个停止位  默认为1个停止位
	// 返回值:     bool __stdcall
	// 备    注：        
	//********************************************************************
	int __stdcall VerifyPreScene(TCHAR* pCommName, UINT pBaudRate = CBR_38400, UINT pParity = NOPARITY, UINT  pByteSize = 8, UINT pStopBits = ONESTOPBIT);

	//********************************************************************
	// 创建时间：  2016/07/01  17:06 
	// 作    者：  GP_Fore
	//********************************************************************
	// 函数说明：  获得镜头的名字和型号
	// 参数列表：  LPTSTR sStr
	// 返回值:     int __stdcall
	// 备    注：  传递一个长度至少为50个字符的空字符串。    
	//********************************************************************
	int __stdcall GetLensName(char* pLensName);


	//********************************************************************
	// 创建时间：  2016/09/19  11:34 
	// 作    者：  GP_Fore
	//********************************************************************
	// 函数说明：  
	// 参数列表：  WORD & nZoomValue
	// 参数列表：  WORD & nFocusValue
	// 参数列表：  WORD & nIrisValue
	// 返回值:     int __stdcall
	// 备    注：        
	//********************************************************************
	int __stdcall GetLensMotorZoomFocusIris(WORD &nZoomValue, WORD &nFocusValue, WORD &nIrisValue);
	int __stdcall SetLensMotorZoomFocusIris(WORD nZoomValue, WORD nFocusValue, WORD nIrisValue);

	//********************************************************************
	// 创建时间：  2016/09/20  15:42 
	// 作    者：  GP_Fore
	//********************************************************************
	// 函数说明：  
	// 参数列表：  WORD & nZoomValue
	// 参数列表：  WORD & nFocusValue
	// 返回值:     int __stdcall
	// 备    注：        
	//********************************************************************
	int __stdcall GetLensMotorZoomFocus(WORD &nZoomValue, WORD &nFocusValue);
	int __stdcall SetLensMotorZoomFocus(WORD nZoomValue, WORD nFocusValue);
 
	//********************************************************************
	// 创建时间：  2016/09/19  11:34 
	// 作    者：  GP_Fore
	//********************************************************************
	// 函数说明：  
	// 参数列表：  WORD & ResultValue   接卸后的数据。
	// 参数列表：  WORD & OriginalValue 直接接受到的数据，没有解析2个字节。
	// 返回值:     int __stdcall
	// 备    注：        
	//********************************************************************
	int __stdcall GetLensMotorZoom(WORD &ResultValue, WORD &OriginalValue);
	int __stdcall GetLensMotorFocus(WORD &nValue);
	int __stdcall GetLensMotorIris(WORD &nValue);


	int __stdcall SetLensMotorZoom(WORD nValue, WORD &OriginalValue);
	int __stdcall SetLensMotorFocus(WORD nValue);
	int __stdcall SetLensMotorIris(WORD nValue);



	
	//********************************************************************
	// 创建时间：  2016/07/08  15:29 
	// 作    者：  GP_Fore
	//********************************************************************
	// 函数说明：  控制近红外线波长
	// 参数列表：  BYTE nValue
	// 返回值:     int __stdcall
	// 备    注：        
	//********************************************************************
	int __stdcall SetSwitch0Host(BYTE nValue);
	int __stdcall GetSwitch0Host(char* sNote, WORD &nValue);

	//********************************************************************
	// 创建时间：  2016/07/08  15:30 
	// 作    者：  GP_Fore
	//********************************************************************
	// 函数说明：  控制Iris的自动和远程控制方式，已经开启和关闭。
	// 参数列表：  BYTE nValue
	// 返回值:     int __stdcall
	// 备    注：  0xCF:iris自动调光圈关闭模式。  11001111   0xEF自动调光圈开启模式
	// 备    注：  0xDF:iris远程控制关闭模式。  11011111   0xFF远程控制开启模式
	//********************************************************************
	int __stdcall SetSwitch2Host(BYTE nValue);
	int __stdcall GetSwitch2Host(char* sNote, WORD &nValue);

	//********************************************************************
	// 创建时间：  2016/07/08  15:31 
	// 作    者：  GP_Fore
	//********************************************************************
	// 函数说明：  显示控制方面
	// 参数列表：  BYTE nValue
	// 返回值:     int __stdcall
	// 备    注：        
	//********************************************************************
	int __stdcall SetSwitch3Host(BYTE nValue);
	int __stdcall GetSwitch3Host(char* sNote, WORD &nValue);


	//********************************************************************
	// 创建时间：  2016/07/08  15:34 
	// 作    者：  GP_Fore
	//********************************************************************
	// 函数说明：  控制Iris,Zoom,Focus为本地模式，还是下位机模式
	// 参数列表：  BYTE nValue
	// 返回值:     int __stdcall
	// 备    注：  0xFC:iris本地模式，focus、zoom下位机模式。  11111100
	// 备    注：  0xFA:zoom本地模式，focus、iris下位机模式。  11111010
	// 备    注：  0xF9:focus本地模式，zoom、iris下位机模式。  11111001
	//********************************************************************
	int __stdcall SetSwitch4Host(BYTE nValue);
	int __stdcall GetSwitch4Host(char* sNote, WORD &nValue);

};

