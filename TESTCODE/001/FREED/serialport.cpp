#pragma once
#include <process.h>  
#include <iostream>
#include <cmath>
#include <cassert>
#include "SerialPort.h" 
//#include "Trace.h"

namespace
{
double ToRadians(double x)
{
	const double pi = 3.1415926536;
	return x / 180.0 * pi;
}
}

/** 线程退出标志 */   
bool CSerialPort::s_bExit = false;  
/** 当串口无数据时,sleep至下次查询间隔的时间,单位:秒 */   
const UINT SLEEP_TIME_INTERVAL = 5;  

//extern C_TraceList g_trace_list;
//extern bool g_start_signal;
 
CSerialPort::CSerialPort(void *param)
: m_param(param), m_hListenThread(INVALID_HANDLE_VALUE)  
{  
    m_hComm = INVALID_HANDLE_VALUE;  
    m_hListenThread = INVALID_HANDLE_VALUE;  
 
    InitializeCriticalSection(&m_csCommunicationSync);  
 
}  
 
CSerialPort::~CSerialPort(void)  
{  
    CloseListenTread();  
    ClosePort();  
    DeleteCriticalSection(&m_csCommunicationSync);  

	delete this->m_param;
}  
 
bool CSerialPort::InitPort( UINT portNo /*= 1*/,UINT baud /*= CBR_9600*/,char parity /*= 'O'*/,  
                            UINT databits /*= 8*/, UINT stopsbits /*= 1*/,DWORD dwCommEvents /*= EV_RXCHAR*/ )  
{  
 
    /** 临时变量,将制定参数转化为字符串形式,以构造DCB结构 */   
    char szDCBparam[50];  
    sprintf_s(szDCBparam, "baud=%d parity=%c data=%d stop=%d", baud, parity, databits, stopsbits);  
 
    /** 打开指定串口,该函数内部已经有临界区保护,上面请不要加保护 */   
    if (!openPort(portNo))  
    {  
        return false;  
    }  
 
    /** 进入临界段 */   
    EnterCriticalSection(&m_csCommunicationSync);  
 
    /** 是否有错误发生 */   
    BOOL bIsSuccess = TRUE;  
 
    /** 在此可以设置输入输出的缓冲区大小,如果不设置,则系统会设置默认值.  
     *  自己设置缓冲区大小时,要注意设置稍大一些,避免缓冲区溢出  
     */ 
    /*if (bIsSuccess )  
    {  
        bIsSuccess = SetupComm(m_hComm,10,10);  
    }*/ 
 
    /** 设置串口的超时时间,均设为0,表示不使用超时限制 */ 
    COMMTIMEOUTS  CommTimeouts;  
    CommTimeouts.ReadIntervalTimeout         = 0;  
    CommTimeouts.ReadTotalTimeoutMultiplier  = 0;  
    CommTimeouts.ReadTotalTimeoutConstant    = 0;  
    CommTimeouts.WriteTotalTimeoutMultiplier = 0;  
    CommTimeouts.WriteTotalTimeoutConstant   = 0;   
    if ( bIsSuccess)  
    {  
        bIsSuccess = SetCommTimeouts(m_hComm, &CommTimeouts);  
    }  
 
    DCB  dcb;  
    if ( bIsSuccess )  
    {  
        // 将ANSI字符串转换为UNICODE字符串  
        DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, szDCBparam, -1, NULL, 0);  
        wchar_t *pwText = new wchar_t[dwNum] ;  
        if (!MultiByteToWideChar (CP_ACP, 0, szDCBparam, -1, pwText, dwNum))  
        {  
            bIsSuccess = TRUE;  
        }  
 
        /** 获取当前串口配置参数,并且构造串口DCB参数 */   
        bIsSuccess = GetCommState(m_hComm, &dcb) && BuildCommDCB(pwText, &dcb) ;  
        /** 开启RTS flow控制 */   
        dcb.fRtsControl = RTS_CONTROL_ENABLE;   
 
        /** 释放内存空间 */   
        delete [] pwText;  
    }  
 
    if ( bIsSuccess )  
    {  
        /** 使用DCB参数配置串口状态 */   
        bIsSuccess = SetCommState(m_hComm, &dcb);  
    }  
          
    /**  清空串口缓冲区 */ 
    PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);  
 
    /** 离开临界段 */   
    LeaveCriticalSection(&m_csCommunicationSync);  
 
    return bIsSuccess==TRUE;  
}  
 
bool CSerialPort::InitPort( UINT portNo ,const LPDCB& plDCB )  
{  

    /** 打开指定串口,该函数内部已经有临界区保护,上面请不要加保护 */   
    if (!openPort(portNo))  
    {  
        return false;  
    }  
      
    /** 进入临界段 */   
    EnterCriticalSection(&m_csCommunicationSync);  
 
    /** 配置串口参数 */   
	if (!SetCommState(m_hComm, plDCB))  
    {  
        return false;  
    }  
 
    /**  清空串口缓冲区 */ 
    PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);  
 
    /** 离开临界段 */   
    LeaveCriticalSection(&m_csCommunicationSync);  
 
    return true;  
}  
 
void CSerialPort::ClosePort()  
{  
    /** 如果有串口被打开，关闭它 */ 
    if( m_hComm != INVALID_HANDLE_VALUE )  
    {  
        CloseHandle( m_hComm );  
        m_hComm = INVALID_HANDLE_VALUE;  
    }  
}  
 
bool CSerialPort::openPort( UINT portNo )  
{  
    /** 进入临界段 */   
    EnterCriticalSection(&m_csCommunicationSync);  
 
    /** 把串口的编号转换为设备名 */   
    char szPort[50];  
    sprintf_s(szPort, "COM%d", portNo);  
 
    /** 打开指定的串口 */   
    m_hComm = CreateFileA(szPort,  /** 设备名,COM1,COM2等 */   
              GENERIC_READ | GENERIC_WRITE, /** 访问模式,可同时读写 */     
              0,                            /** 共享模式,0表示不共享 */   
              NULL,                         /** 安全性设置,一般使用NULL */   
              OPEN_EXISTING,                /** 该参数表示设备必须存在,否则创建失败 */   
              0,      
              0);      
 
    /** 如果打开失败，释放资源并返回 */   
    if (m_hComm == INVALID_HANDLE_VALUE)  
    {  
        LeaveCriticalSection(&m_csCommunicationSync);  
        return false;  
    }  
 
    /** 退出临界区 */   
    LeaveCriticalSection(&m_csCommunicationSync);  
 
    return true;  
}  
 
bool CSerialPort::OpenListenThread()  
{  
    /** 检测线程是否已经开启了 */   
    if (m_hListenThread != INVALID_HANDLE_VALUE)  
    {  
        /** 线程已经开启 */   
        return false;  
    }  
 
    s_bExit = false;  
//	g_start_signal = false;
    /** 线程ID */   
    UINT threadId;  
    /** 开启串口数据监听线程 */   
    m_hListenThread = (HANDLE)_beginthreadex(NULL, 0, ListenThread, this, 0, &threadId);  
    if (!m_hListenThread)  
    {  
        return false;  
    }  
    /** 设置线程的优先级,高于普通线程 */   
    if (!SetThreadPriority(m_hListenThread, THREAD_PRIORITY_ABOVE_NORMAL))  
    {  
        return false;  
    }  
 
    return true;  
}  
 
bool CSerialPort::CloseListenTread()  
{     
    if (m_hListenThread != INVALID_HANDLE_VALUE)  
    {  
        /** 通知线程退出 */   
        s_bExit = true;  
 
        /** 等待线程退出 */   
        Sleep(10);  
 
        /** 置线程句柄无效 */   
        CloseHandle( m_hListenThread );  
        m_hListenThread = INVALID_HANDLE_VALUE;  
    }  
    return true;  
}  
 
UINT CSerialPort::GetBytesInCOM()  
{  
    DWORD dwError = 0;  /** 错误码 */   
    COMSTAT  comstat;   /** COMSTAT结构体,记录通信设备的状态信息 */   
    memset(&comstat, 0, sizeof(COMSTAT));  
 
    UINT BytesInQue = 0;  
    /** 在调用ReadFile和WriteFile之前,通过本函数清除以前遗留的错误标志 */   
    if ( ClearCommError(m_hComm, &dwError, &comstat) )  
    {  
        BytesInQue = comstat.cbInQue; /** 获取在输入缓冲区中的字节数 */   
    }  
 
    return BytesInQue;  
}  
 
UINT WINAPI CSerialPort::ListenThread( void* pParam )  
{  
 /*   / ** 得到本类的指针 * /   
    CSerialPort *pSerialPort = reinterpret_cast<CSerialPort*>(pParam);  

	C_CamLocation abs_location(*(C_CamLocation *)(pSerialPort->GetParam()));

	assert(abs_location.abs_location.rotateZ == 0.0);
 
    // 线程循环,轮询方式读取串口数据  
    while (!pSerialPort->s_bExit)   
    {  
        UINT BytesInQue = pSerialPort->GetBytesInCOM();  
        / ** 如果串口输入缓冲区中无数据,则休息一会再查询 * /   
        if ( BytesInQue == 0)  
        {  
            Sleep(SLEEP_TIME_INTERVAL);  
            continue;  
        }  
 
        / ** 读取输入缓冲区中的数据并输出显示 * / 
        unsigned char cRecved = 0x00;  
		unsigned char check_sum;
		U_ThreeChar temp_char;
		C_TraceNode temp_node;
        do 
        {  
            cRecved = 0x00;
			check_sum = 0x40;
			if(pSerialPort->ReadChar(cRecved))  //1
			{  
				if(cRecved != 0xA0)
				{
					exit(-1);
				}
				else
				{
					check_sum -= cRecved;
				}
			} 
			else
			{
				exit(-2);
			}

			if(pSerialPort->ReadChar(cRecved))  //2
            {  
                check_sum -= cRecved;
            }
			else
			{
				exit(-2);
			}

			if(pSerialPort->ReadChar(cRecved))  //3
            {  
				temp_char.SetFirst(cRecved);
				check_sum -= cRecved;
            }
			else
			{
				exit(-2);
			}

			if(pSerialPort->ReadChar(cRecved))  //4
            {  
				temp_char.SetSecond(cRecved);
				check_sum -= cRecved;
            }
			else
			{
				exit(-2);
			}

			if(pSerialPort->ReadChar(cRecved))  //5
            {  
				temp_char.SetThird(cRecved);
				check_sum -= cRecved;
            }
			else
			{
				exit(-2);
			}

			double pan = (temp_char.data - 0x058730) / double(0x04F1A0) * 360.0 - 180.0;

			if(pSerialPort->ReadChar(cRecved))  //6
            {  
				temp_char.SetFirst(cRecved);
				check_sum -= cRecved;
            }
			else
			{
				exit(-2);
			}

			if(pSerialPort->ReadChar(cRecved))  //7
            {  
				temp_char.SetSecond(cRecved);
				check_sum -= cRecved;
            }
			else
			{
				exit(-2);
			}

			if(pSerialPort->ReadChar(cRecved))  //8
            {  
				temp_char.SetThird(cRecved);
				check_sum -= cRecved;
            }
			else
			{
				exit(-2);
			}

			double tilt = (temp_char.data - 0x058730) / double(0x04F1A0) * 360.0 - 180.0;

			if(pSerialPort->ReadChar(cRecved))  //9
            {  
				temp_char.SetFirst(cRecved);
				check_sum -= cRecved;
            }
			else
			{
				exit(-2);
			}

			if(pSerialPort->ReadChar(cRecved))  //10
            {  
				temp_char.SetSecond(cRecved);
				check_sum -= cRecved;
            }
			else
			{
				exit(-2);
			}

			if(pSerialPort->ReadChar(cRecved))  //11
            {  
				temp_char.SetThird(cRecved);
				check_sum -= cRecved;
            }
			else
			{
				exit(-2);
			}

			//////////////////////////////

			if(pSerialPort->ReadChar(cRecved))  //12
            {  
				temp_char.SetFirst(cRecved);
				check_sum -= cRecved;
            }
			else
			{
				exit(-2);
			}

			if(pSerialPort->ReadChar(cRecved))  //13
            {  
				temp_char.SetSecond(cRecved);
				check_sum -= cRecved;
            }
			else
			{
				exit(-2);
			}

			if(pSerialPort->ReadChar(cRecved))  //14
            {  
				temp_char.SetThird(cRecved);
				check_sum -= cRecved;
            }
			else
			{
				exit(-2);
			}

			/////////////////////////////////////////

			if(pSerialPort->ReadChar(cRecved))  //15 check sum
            {
				if(check_sum != unsigned char(cRecved))
				{
					exit(-3);
				}
				else//pass check
				{
					/ *std::cout<<"A0"<<std::endl;
					std::cout<<1<<std::endl;
					std::cout<<pan<<std::endl;
					std::cout<<tilt<<std::endl;
					std::cout<<std::endl;
					std::cout<<std::endl;
					std::cout<<unsigned int(check_sum)<<std::endl;

					std::cout<<*(int *)(pSerialPort->GetParam())<<std::endl;* /

					if(g_start_signal)
					{
						temp_node.focalLength = 74.975;//焦距获取尚未完成，用以固定值代替

						//MAYA中旋转单位是角度
						temp_node.location.rotateX = abs_location.abs_location.rotateX + tilt;//MAYA和昭特云台沿X轴都是右手坐标
						temp_node.location.rotateY = abs_location.abs_location.rotateY - pan;//MAYA是右手坐标，昭特云台是左手坐标
						temp_node.location.rotateZ = abs_location.abs_location.rotateZ;//昭特云台无法沿Z轴旋转

						temp_node.location.translateX = abs_location.abs_location.translateX - std::cos(ToRadians(temp_node.location.rotateX) + abs_location.rel_radian) * std::sin(ToRadians(temp_node.location.rotateY)) * abs_location.rel_distance;
						temp_node.location.translateY = abs_location.abs_location.translateY + std::sin(ToRadians(temp_node.location.rotateX) + abs_location.rel_radian) * abs_location.rel_distance;
						temp_node.location.translateZ = abs_location.abs_location.translateZ - std::cos(ToRadians(temp_node.location.rotateX) + abs_location.rel_radian) * std::cos(ToRadians(temp_node.location.rotateY)) * abs_location.rel_distance;

						g_trace_list.Push(temp_node);
					}
				}
            }
			else
			{
				exit(-2);
			}

        }while(--BytesInQue);  
    }  
 */
    return 0;  
}  
bool CSerialPort::ReadChar( unsigned char &cRecved )  
{  
    BOOL  bResult     = TRUE;  
    DWORD BytesRead   = 0;  
    if(m_hComm == INVALID_HANDLE_VALUE)  
    {  
        return false;  
    }  
 
    /** 临界区保护 */   
    EnterCriticalSection(&m_csCommunicationSync);  
 
    /** 从缓冲区读取一个字节的数据 */   
    bResult = ReadFile(m_hComm, &cRecved, 1, &BytesRead, NULL);  
    if ((!bResult))  
    {   
        /** 获取错误码,可以根据该错误码查出错误原因 */   
        DWORD dwError = GetLastError();  
 
        /** 清空串口缓冲区 */   
        PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);  
        LeaveCriticalSection(&m_csCommunicationSync);  
 
        return false;  
    }  
 
    /** 离开临界区 */   
    LeaveCriticalSection(&m_csCommunicationSync);  
 
    return (BytesRead == 1);  
 
}  
 
bool CSerialPort::Read( unsigned char cRecved[] ,int length)  
{  
    BOOL  bResult     = TRUE;  
    DWORD BytesRead   = 0;  
    if(m_hComm == INVALID_HANDLE_VALUE)  
    {  
        return false;  
    }  
 
    /** 临界区保护 */   
    EnterCriticalSection(&m_csCommunicationSync);  
 
    /** 从缓冲区读取一个字节的数据 */   
    bResult = ReadFile(m_hComm, &cRecved, length, &BytesRead, NULL);  
    if ((!bResult))  
    {   
        /** 获取错误码,可以根据该错误码查出错误原因 */   
        DWORD dwError = GetLastError();  
 
        /** 清空串口缓冲区 */   
        PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);  
        LeaveCriticalSection(&m_csCommunicationSync);  
 
        return false;  
    }  
 
    /** 离开临界区 */   
    LeaveCriticalSection(&m_csCommunicationSync);  
 
    return (BytesRead == 1);  
 
}  
 
bool CSerialPort::Write( unsigned char* pData, unsigned int length )  
{  
	for(int i=0;i<29;i++)
	{
		char a;
		pData[i]=i;
		a=pData[i];
		int j=i;
	}
	
	BOOL   bResult     = TRUE;  
    DWORD  BytesToSend = 0;  
    if(m_hComm == INVALID_HANDLE_VALUE)  
    {  
        //return false;  
    }  
 
    /** 临界区保护 */   
    EnterCriticalSection(&m_csCommunicationSync);  
 
    /** 向缓冲区写入指定量的数据 */   
    bResult = WriteFile(m_hComm, pData, length, &BytesToSend, NULL);  
    if (!bResult)    
    {  
        DWORD dwError = GetLastError();  
        /** 清空串口缓冲区 */   
        PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);  
        LeaveCriticalSection(&m_csCommunicationSync);  
 
        return false;  
    }  
 
    /** 离开临界区 */   
    LeaveCriticalSection(&m_csCommunicationSync);  
 
    return true;  
}  
/** 读BYTE数组 */
bool CSerialPort::WriteBYTE(BYTE pData[], unsigned int length )  
{  
	for(int i=0;i<29;i++)
	{
		BYTE a;
		a=pData[i];
		int j=i;
	}

	BYTE temp;
	temp=0xAA;
	
	BOOL   bResult     = TRUE;  
    DWORD  BytesToSend = 0;  
    if(m_hComm == INVALID_HANDLE_VALUE)  
    {  
        return false;  
    }  
 
    /** 临界区保护 */   
    EnterCriticalSection(&m_csCommunicationSync);  
 
    /** 向缓冲区写入指定量的数据 */   
    bResult = WriteFile(m_hComm, pData, length, &BytesToSend, NULL);  
    if (!bResult)    
    {  
        DWORD dwError = GetLastError();  
        /** 清空串口缓冲区 */   
        PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);  
        LeaveCriticalSection(&m_csCommunicationSync);  
 
        return false;  
    }  
 
    /** 离开临界区 */   
    LeaveCriticalSection(&m_csCommunicationSync);  
 
    return true;  
}  
void CSerialPort::close()
{
	 if( m_hComm != INVALID_HANDLE_VALUE )  
    {  
        CloseHandle( m_hComm );  
        m_hComm = INVALID_HANDLE_VALUE;
    }  
}