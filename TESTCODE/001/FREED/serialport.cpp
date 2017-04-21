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

/** �߳��˳���־ */   
bool CSerialPort::s_bExit = false;  
/** ������������ʱ,sleep���´β�ѯ�����ʱ��,��λ:�� */   
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
 
    /** ��ʱ����,���ƶ�����ת��Ϊ�ַ�����ʽ,�Թ���DCB�ṹ */   
    char szDCBparam[50];  
    sprintf_s(szDCBparam, "baud=%d parity=%c data=%d stop=%d", baud, parity, databits, stopsbits);  
 
    /** ��ָ������,�ú����ڲ��Ѿ����ٽ�������,�����벻Ҫ�ӱ��� */   
    if (!openPort(portNo))  
    {  
        return false;  
    }  
 
    /** �����ٽ�� */   
    EnterCriticalSection(&m_csCommunicationSync);  
 
    /** �Ƿ��д����� */   
    BOOL bIsSuccess = TRUE;  
 
    /** �ڴ˿���������������Ļ�������С,���������,��ϵͳ������Ĭ��ֵ.  
     *  �Լ����û�������Сʱ,Ҫע�������Դ�һЩ,���⻺�������  
     */ 
    /*if (bIsSuccess )  
    {  
        bIsSuccess = SetupComm(m_hComm,10,10);  
    }*/ 
 
    /** ���ô��ڵĳ�ʱʱ��,����Ϊ0,��ʾ��ʹ�ó�ʱ���� */ 
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
        // ��ANSI�ַ���ת��ΪUNICODE�ַ���  
        DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, szDCBparam, -1, NULL, 0);  
        wchar_t *pwText = new wchar_t[dwNum] ;  
        if (!MultiByteToWideChar (CP_ACP, 0, szDCBparam, -1, pwText, dwNum))  
        {  
            bIsSuccess = TRUE;  
        }  
 
        /** ��ȡ��ǰ�������ò���,���ҹ��촮��DCB���� */   
        bIsSuccess = GetCommState(m_hComm, &dcb) && BuildCommDCB(pwText, &dcb) ;  
        /** ����RTS flow���� */   
        dcb.fRtsControl = RTS_CONTROL_ENABLE;   
 
        /** �ͷ��ڴ�ռ� */   
        delete [] pwText;  
    }  
 
    if ( bIsSuccess )  
    {  
        /** ʹ��DCB�������ô���״̬ */   
        bIsSuccess = SetCommState(m_hComm, &dcb);  
    }  
          
    /**  ��մ��ڻ����� */ 
    PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);  
 
    /** �뿪�ٽ�� */   
    LeaveCriticalSection(&m_csCommunicationSync);  
 
    return bIsSuccess==TRUE;  
}  
 
bool CSerialPort::InitPort( UINT portNo ,const LPDCB& plDCB )  
{  

    /** ��ָ������,�ú����ڲ��Ѿ����ٽ�������,�����벻Ҫ�ӱ��� */   
    if (!openPort(portNo))  
    {  
        return false;  
    }  
      
    /** �����ٽ�� */   
    EnterCriticalSection(&m_csCommunicationSync);  
 
    /** ���ô��ڲ��� */   
	if (!SetCommState(m_hComm, plDCB))  
    {  
        return false;  
    }  
 
    /**  ��մ��ڻ����� */ 
    PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);  
 
    /** �뿪�ٽ�� */   
    LeaveCriticalSection(&m_csCommunicationSync);  
 
    return true;  
}  
 
void CSerialPort::ClosePort()  
{  
    /** ����д��ڱ��򿪣��ر��� */ 
    if( m_hComm != INVALID_HANDLE_VALUE )  
    {  
        CloseHandle( m_hComm );  
        m_hComm = INVALID_HANDLE_VALUE;  
    }  
}  
 
bool CSerialPort::openPort( UINT portNo )  
{  
    /** �����ٽ�� */   
    EnterCriticalSection(&m_csCommunicationSync);  
 
    /** �Ѵ��ڵı��ת��Ϊ�豸�� */   
    char szPort[50];  
    sprintf_s(szPort, "COM%d", portNo);  
 
    /** ��ָ���Ĵ��� */   
    m_hComm = CreateFileA(szPort,  /** �豸��,COM1,COM2�� */   
              GENERIC_READ | GENERIC_WRITE, /** ����ģʽ,��ͬʱ��д */     
              0,                            /** ����ģʽ,0��ʾ������ */   
              NULL,                         /** ��ȫ������,һ��ʹ��NULL */   
              OPEN_EXISTING,                /** �ò�����ʾ�豸�������,���򴴽�ʧ�� */   
              0,      
              0);      
 
    /** �����ʧ�ܣ��ͷ���Դ������ */   
    if (m_hComm == INVALID_HANDLE_VALUE)  
    {  
        LeaveCriticalSection(&m_csCommunicationSync);  
        return false;  
    }  
 
    /** �˳��ٽ��� */   
    LeaveCriticalSection(&m_csCommunicationSync);  
 
    return true;  
}  
 
bool CSerialPort::OpenListenThread()  
{  
    /** ����߳��Ƿ��Ѿ������� */   
    if (m_hListenThread != INVALID_HANDLE_VALUE)  
    {  
        /** �߳��Ѿ����� */   
        return false;  
    }  
 
    s_bExit = false;  
//	g_start_signal = false;
    /** �߳�ID */   
    UINT threadId;  
    /** �����������ݼ����߳� */   
    m_hListenThread = (HANDLE)_beginthreadex(NULL, 0, ListenThread, this, 0, &threadId);  
    if (!m_hListenThread)  
    {  
        return false;  
    }  
    /** �����̵߳����ȼ�,������ͨ�߳� */   
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
        /** ֪ͨ�߳��˳� */   
        s_bExit = true;  
 
        /** �ȴ��߳��˳� */   
        Sleep(10);  
 
        /** ���߳̾����Ч */   
        CloseHandle( m_hListenThread );  
        m_hListenThread = INVALID_HANDLE_VALUE;  
    }  
    return true;  
}  
 
UINT CSerialPort::GetBytesInCOM()  
{  
    DWORD dwError = 0;  /** ������ */   
    COMSTAT  comstat;   /** COMSTAT�ṹ��,��¼ͨ���豸��״̬��Ϣ */   
    memset(&comstat, 0, sizeof(COMSTAT));  
 
    UINT BytesInQue = 0;  
    /** �ڵ���ReadFile��WriteFile֮ǰ,ͨ�������������ǰ�����Ĵ����־ */   
    if ( ClearCommError(m_hComm, &dwError, &comstat) )  
    {  
        BytesInQue = comstat.cbInQue; /** ��ȡ�����뻺�����е��ֽ��� */   
    }  
 
    return BytesInQue;  
}  
 
UINT WINAPI CSerialPort::ListenThread( void* pParam )  
{  
 /*   / ** �õ������ָ�� * /   
    CSerialPort *pSerialPort = reinterpret_cast<CSerialPort*>(pParam);  

	C_CamLocation abs_location(*(C_CamLocation *)(pSerialPort->GetParam()));

	assert(abs_location.abs_location.rotateZ == 0.0);
 
    // �߳�ѭ��,��ѯ��ʽ��ȡ��������  
    while (!pSerialPort->s_bExit)   
    {  
        UINT BytesInQue = pSerialPort->GetBytesInCOM();  
        / ** ����������뻺������������,����Ϣһ���ٲ�ѯ * /   
        if ( BytesInQue == 0)  
        {  
            Sleep(SLEEP_TIME_INTERVAL);  
            continue;  
        }  
 
        / ** ��ȡ���뻺�����е����ݲ������ʾ * / 
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
						temp_node.focalLength = 74.975;//�����ȡ��δ��ɣ����Թ̶�ֵ����

						//MAYA����ת��λ�ǽǶ�
						temp_node.location.rotateX = abs_location.abs_location.rotateX + tilt;//MAYA��������̨��X�ᶼ����������
						temp_node.location.rotateY = abs_location.abs_location.rotateY - pan;//MAYA���������꣬������̨����������
						temp_node.location.rotateZ = abs_location.abs_location.rotateZ;//������̨�޷���Z����ת

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
 
    /** �ٽ������� */   
    EnterCriticalSection(&m_csCommunicationSync);  
 
    /** �ӻ�������ȡһ���ֽڵ����� */   
    bResult = ReadFile(m_hComm, &cRecved, 1, &BytesRead, NULL);  
    if ((!bResult))  
    {   
        /** ��ȡ������,���Ը��ݸô�����������ԭ�� */   
        DWORD dwError = GetLastError();  
 
        /** ��մ��ڻ����� */   
        PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);  
        LeaveCriticalSection(&m_csCommunicationSync);  
 
        return false;  
    }  
 
    /** �뿪�ٽ��� */   
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
 
    /** �ٽ������� */   
    EnterCriticalSection(&m_csCommunicationSync);  
 
    /** �ӻ�������ȡһ���ֽڵ����� */   
    bResult = ReadFile(m_hComm, &cRecved, length, &BytesRead, NULL);  
    if ((!bResult))  
    {   
        /** ��ȡ������,���Ը��ݸô�����������ԭ�� */   
        DWORD dwError = GetLastError();  
 
        /** ��մ��ڻ����� */   
        PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);  
        LeaveCriticalSection(&m_csCommunicationSync);  
 
        return false;  
    }  
 
    /** �뿪�ٽ��� */   
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
 
    /** �ٽ������� */   
    EnterCriticalSection(&m_csCommunicationSync);  
 
    /** �򻺳���д��ָ���������� */   
    bResult = WriteFile(m_hComm, pData, length, &BytesToSend, NULL);  
    if (!bResult)    
    {  
        DWORD dwError = GetLastError();  
        /** ��մ��ڻ����� */   
        PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);  
        LeaveCriticalSection(&m_csCommunicationSync);  
 
        return false;  
    }  
 
    /** �뿪�ٽ��� */   
    LeaveCriticalSection(&m_csCommunicationSync);  
 
    return true;  
}  
/** ��BYTE���� */
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
 
    /** �ٽ������� */   
    EnterCriticalSection(&m_csCommunicationSync);  
 
    /** �򻺳���д��ָ���������� */   
    bResult = WriteFile(m_hComm, pData, length, &BytesToSend, NULL);  
    if (!bResult)    
    {  
        DWORD dwError = GetLastError();  
        /** ��մ��ڻ����� */   
        PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_RXABORT);  
        LeaveCriticalSection(&m_csCommunicationSync);  
 
        return false;  
    }  
 
    /** �뿪�ٽ��� */   
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