#pragma once

#ifndef GP_SERIPORT_H_
#define GP_SERIPORT_H_
#include "GP_Thread.h"
#include "GP_StopWatch.h"

class GP_DLL_API GP_SeriPort
{
public:
	GP_SeriPort();
	~GP_SeriPort();
	
public:
	GP_StopWatch m_StopWatch;
	CRITICAL_SECTION m_cs;
	HANDLE m_comHandle;

	//********************************************************************
	// 创建时间：  2016/09/20  16:37 
	// 作    者：  GP_Fore
	//********************************************************************
	// 函数说明：  
	// 参数列表：  TCHAR * pCommName             /** 设备名,"COM1",COM2等 */
	// 参数列表：  DWORD dwDesiredAccess         /** 访问模式,可同时读写 */
	// 参数列表：  DWORD dwShareMode			 /** 共享模式,0表示不共享 */
	// 参数列表：  LPSECURITY_ATTRIBUTES lpSecurityAttributes  /** 安全性设置,一般使用NULL */
	// 参数列表：  DWORD dwCreationDisposition    /**   该参数表示设备必须存在,否则创建失败  OPEN_EXISTING：默认参数表示设备必须存在,否则创建失败 */
	// 参数列表：  DWORD dwFlagsAndAttributes     /** 创建方式 0：默认参数为同步方式*/
	// 参数列表：  HANDLE hTemplateFile
	// 返回值:     bool
	// 备    注：  做了串口缓冲区清理     
	//********************************************************************
	bool OpenCommPort(TCHAR* pCommName, DWORD dwDesiredAccess = GENERIC_READ | GENERIC_WRITE, DWORD dwShareMode = 0, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL, DWORD dwCreationDisposition = OPEN_EXISTING, DWORD dwFlagsAndAttributes=0, HANDLE hTemplateFile=NULL);

	//********************************************************************
	// 创建时间：  2016/05/22  16:17 
	// 作    者：  GP_Fore
	//********************************************************************
	// 函数说明：  设置串口超时时间  
	// 参数列表：  DWORD ReadInterval  //读间隔超时
	// 参数列表：  DWORD ReadTotalMultiplier //读时间系数
	// 参数列表：  DWORD ReadTotalconstant  //读时间常量
	// 参数列表：  DWORD WriteTotalMultiplier  // 写时间系数
	// 参数列表：  DWORD WriteTotalconstant   //写时间常量
	// 返回值:     bool  成功 true;失败 false
	// 备    注：  注意：读总超时＝ReadTotalTimeoutMultiplier×10＋ReadTotalTimeoutConstant
	//					 可以看出：间隔超时和总超时的设置是不相关的，这可以方便通信程序灵活地设置各种超时。
	//					 如果所有写超时参数均为0，那么就不使用写超时。如果ReadIntervalTimeout为0，那么就不使用读间隔超时
	//                   必须先打开一个串口，保证m_comHandle指针不为空。串口没打开等都会导致设置出错
	//********************************************************************
	bool SetCommTimeout(DWORD ReadInterval = 0, DWORD ReadTotalMultiplier=0, DWORD ReadTotalconstant=0, DWORD WriteTotalMultiplier=0, DWORD WriteTotalconstant=0);

	//********************************************************************
	// 创建时间：  2016/05/22  16:17 
	// 作    者：  GP_Fore
	//********************************************************************
	// 函数说明： 设置串口DCB参数  
	// 参数列表： 波特率,默认为9600
	// 参数列表： UINT pParity 奇偶校验  NOPARITY:0（不奇偶校验） ODDPARITY:1 ……  是否进行奇偶校验,true表示需要奇偶校验,false表示不需要奇偶校验  
	// 参数列表： 数据位的个数,默认值为8个数据位
	// 参数列表： UINT pStopBits  注：0(ONESTOPBIT)：代表一个停止位，2(TWOSTOPBITS):代表两个停止位。 不支持1.5个停止位  默认为1个停止位
	// 返回值:    成功 true;失败 false
	// 备    注：        
	//********************************************************************
	bool SetCommDCBParam(UINT pBaudRate = CBR_9600, UINT pParity = NOPARITY, UINT  pByteSize = 8, UINT pStopBits = ONESTOPBIT);

	//********************************************************************
	// 创建时间：  2016/05/22  16:44 
	// 作    者：  GP_Fore
	//********************************************************************
	// 函数说明：  场景验证，如果返回为true,使用默认参数，打开了串口。可以正常使用发送和接受函数了。
	// 参数列表：  TCHAR * pCommName
	// 参数列表：  UINT pBaudRate
	// 参数列表：  UINT pParity 奇偶校验  NOPARITY:0（不奇偶校验） ODDPARITY:1 …… 
	// 参数列表：  UINT pByteSize
	// 参数列表：  UINT pStopBits 注：0(ONESTOPBIT)：代表一个停止位，2(TWOSTOPBITS):代表两个停止位。 不支持1.5个停止位  默认为1个停止位
	// 返回值:     成功 true;失败 false
	// 备    注：        
	//********************************************************************
	bool SeriPortInitial(TCHAR* pCommName, UINT pBaudRate = CBR_9600, UINT pParity = NOPARITY, UINT  pByteSize = 8, UINT pStopBits = ONESTOPBIT);
	
	//********************************************************************
	// 创建时间：  2016/05/22  17:11 
	// 作    者：  GP_Fore
	//********************************************************************
	// 函数说明：  将指定字符串发送到当前串口
	// 参数列表：  TCHAR * pSendData
	// 返回值:     bool 成功 true;失败 false
	// 备    注：  pSendData 字符串中间如果有0x00，会只识别0x00之前的数据，这个时候希望使用SendData(byte pSendData[], unsigned int pDataLength)     
	//********************************************************************
    bool SendData(unsigned char* pSendData, unsigned int pDataLength);
	//bool SendData(byte pSendData[], unsigned int pDataLength);



	//********************************************************************
	// 创建时间：  2016/06/15  14:38 
	// 作    者：  GP_Fore
	//********************************************************************
	// 函数说明：  获取当前打的com口缓冲区的数据长度。
	// 参数列表：  unsigned int& comBufferSize  返回值，当前缓冲区的数据长度。
	// 返回值:     bool 成功true;,失败 false;
	// 备    注：        
	//********************************************************************

	//********************************************************************
	// 创建时间：  2016/07/08  10:28 
	// 作    者：  GP_Fore
	//********************************************************************
	// 函数说明：  获取当前打的com口缓冲区的数据长度。
	// 参数列表：  unsigned int& comBufferSize  返回值，当前缓冲区的数据长度。
	// 参数列表：  unsigned int pWaitTime 单位为：毫秒  等待时间，
	// 返回值:     bool
	// 备    注：        
	//********************************************************************
	bool ComBufferDataLength(unsigned int& comBufferSize,unsigned int pWaitTime=0);

	//********************************************************************
	// 创建时间：  2016/06/15  14:17 
	// 作    者：  GP_Fore
	//********************************************************************
	// 函数说明：  函数不会等待，缓冲中有多少接受多少，并且立即返回。
	// 参数列表：  char * pReceiveData，返回接受的串口数据
	// 返回值:     bool
	// 备    注：  pReceiveData 参数在函数内开辟内存，外部使用完需要用free(pReceiveData)进行内存空间释放;    
	//********************************************************************
	bool ReceiveChar(unsigned char &pReceiveChar);
	bool ReceiveByte(byte &pReceiveByte);

	//********************************************************************
	// 创建时间：  2016/05/25  16:33 
	// 作    者：  GP_Fore
	//********************************************************************
	// 函数说明：  接收指定长度的字符串，如果当前串口缓存中的数据长度没有达到pDataLength指定的长度，那么此函数就会等待。
	//             在调用此函数之前可以使用ComBufferDataLength函数，确认串口缓存的数据长度。再指定pDataLength。
	// 参数列表：  char pReceiveData[]
	// 参数列表：  unsigned int pDataLength
	// 返回值:     bool
	// 备    注：        
	//********************************************************************
	bool ReceiveData(char pReceiveData[],const unsigned int pDataLength);
	bool ReceiveData(byte pReceiveData[], const unsigned int pDataLength);
	bool ClosePort();
public:

};

#endif