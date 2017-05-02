#include "stdafx.h"
#include "GP_Lens_Canon.h"


GP_Lens_Canon::GP_Lens_Canon()
{
}


GP_Lens_Canon::~GP_Lens_Canon()
{
}


//bool __stdcall GP_Lens_Canon::VerifyPreScene(TCHAR* pCommName, UINT pBaudRate /*= CBR_38400*/, UINT pParity /*= NOPARITY*/, UINT pByteSize /*= 8*/, UINT pStopBits /*= ONESTOPBIT*/)
//{
//	if (this->m_comHandle != INVALID_HANDLE_VALUE) //串口可能是打开的，先关闭串口
//	{
//		CloseHandle(this->m_comHandle);
//	}
//	if (SeriPortInitial(pCommName, pBaudRate, EVENPARITY))
//	{
//		return true;
//	}
//	else
//		return false;
//}


bool GP_Lens_Canon::InitCommPort(CString PortNo, UINT  baud)
{
	m_SerialPort = new CFuSerialPort(NULL);
	return m_SerialPort->InitPort(PortNo, baud,'E');
}

bool __stdcall GP_Lens_Canon::FocusControllerCommd()
{
	memset(m_SendDataBuffer, 0, 30);
	memset(m_ReceiveDataBuffer, 0, 50);
	m_SendDataBuffer[0] = 0x80;
	m_SendDataBuffer[1] = 0xC0;
	m_SendDataBuffer[2] = 0xBF;

	while (true)
	{
		m_SerialPort->Write(m_SendDataBuffer, 3);
		Delay(30);
		unsigned int com_length = 0;
		com_length = m_SerialPort->GetBytesInCOM();
		if (com_length >=3)
		{
			m_SerialPort->Read(m_ReceiveDataBuffer, com_length);
			return true;
		}
	}
}

bool __stdcall GP_Lens_Canon::ZoomControllerCommd()
{
	memset(m_SendDataBuffer, 0, 30);
	memset(m_ReceiveDataBuffer, 0, 50);
	m_SendDataBuffer[0] = 0x80;
	m_SendDataBuffer[1] = 0xC3;
	m_SendDataBuffer[2] = 0xBF;

	while (true)
	{
		m_SerialPort->Write(m_SendDataBuffer, 3);
		Delay(30);
		unsigned int com_length = 0;
		com_length = m_SerialPort->GetBytesInCOM();
		if (com_length >= 3)
		{
			m_SerialPort->Read(m_ReceiveDataBuffer, com_length);
			return true;
		}
	}
}


bool __stdcall GP_Lens_Canon::PcControllerCommd()
{
	memset(m_SendDataBuffer, 0, 30);
	memset(m_ReceiveDataBuffer, 0, 50);
	m_SendDataBuffer[0] = 0x80;
	m_SendDataBuffer[1] = 0xC6;
	m_SendDataBuffer[2] = 0xBF;

	while (true)
	{
		m_SerialPort->Write(m_SendDataBuffer, 3);
		Delay(30);
		unsigned int com_length = 0;
		com_length = m_SerialPort->GetBytesInCOM();
		if (com_length >= 3)
		{
			m_SerialPort->Read(m_ReceiveDataBuffer, com_length);
			return true;
		}
	}
}

bool __stdcall GP_Lens_Canon::LensVersionRequest(char* pLensVersionNumber)
{
	memset(m_SendDataBuffer, 0, 30);
	memset(m_ReceiveDataBuffer, 0, 50);
	m_SendDataBuffer[0] = 0xBE;
	m_SendDataBuffer[1] = 0x80;
	m_SendDataBuffer[2] = 0x80;
	m_SendDataBuffer[3] = 0x00;
	m_SendDataBuffer[4] = 0x00;
	m_SendDataBuffer[5] = 0x00;
	m_SendDataBuffer[6] = 0xBF;

	m_SerialPort->Write(m_SendDataBuffer, 7);
	Delay(30);
	unsigned int com_length = 0;
	com_length = m_SerialPort->GetBytesInCOM();
	if (com_length>=37)
	{
		m_SerialPort->Read(m_ReceiveDataBuffer, com_length);
		auto t=memcpy_s(pLensVersionNumber, com_length+1, (const char*)m_ReceiveDataBuffer, com_length +1);
		return true;
	}
	return false;
}


bool __stdcall GP_Lens_Canon::LensTypeRequest(char* pLensTypeRequest)
{

	memset(m_SendDataBuffer, 0, 30);
	memset(m_ReceiveDataBuffer, 0, 50);
	m_SendDataBuffer[0] = 0xBE;
	m_SendDataBuffer[1] = 0x80;
	m_SendDataBuffer[2] = 0x81;
	m_SendDataBuffer[3] = 0x00;
	m_SendDataBuffer[4] = 0x00;
	m_SendDataBuffer[5] = 0x00;
	m_SendDataBuffer[6] = 0xBF;

	m_SerialPort->Write(m_SendDataBuffer, 7);
	Sleep(1000);
	Delay(30);
	unsigned int com_length = 0;
	com_length = m_SerialPort->GetBytesInCOM();
	if (com_length>=41)
	{
		m_SerialPort->Read(m_ReceiveDataBuffer, com_length);
		memcpy_s(pLensTypeRequest, com_length - 6, (const char*)m_ReceiveDataBuffer, com_length - 7);
		return true;
	}
	return false;
}


bool __stdcall GP_Lens_Canon::InitCompletedCommd()
{
	memset(m_SendDataBuffer, 0, 30);
	memset(m_ReceiveDataBuffer, 0, 50);
	m_SendDataBuffer[0] = 0x86;
	m_SendDataBuffer[1] = 0xC0;
	m_SendDataBuffer[2] = 0x00;
	m_SendDataBuffer[3] = 0x00;
	m_SendDataBuffer[4] = 0x00;
	m_SendDataBuffer[5] = 0xBF;

	m_SerialPort->Write(m_SendDataBuffer, 6);
	Delay(30);
	unsigned int com_length = 0;
	com_length = m_SerialPort->GetBytesInCOM();

	if (com_length >= 6)
	{
		m_SerialPort->Read(m_ReceiveDataBuffer, com_length);
		return true;
	}
	return false;
}

int __stdcall GP_Lens_Canon::IrisPriorityCommd(WORD nValue)
{
	memset(m_SendDataBuffer, 0, 30);
	m_SendDataBuffer[0] = 0x82;
	m_SendDataBuffer[1] = nValue;
	m_SendDataBuffer[2] = 0xBF;

	m_SerialPort->Write(m_SendDataBuffer, 3);
	Delay(30);
	unsigned int com_length = 0;
	com_length = m_SerialPort->GetBytesInCOM();
	if (com_length >= 6)
	{
		m_SerialPort->Read(m_ReceiveDataBuffer, com_length);
		return true;
	}
	return false;
}

int __stdcall GP_Lens_Canon::IncommCommd(WORD nValue)
{
	memset(m_SendDataBuffer, 0, 30);
	m_SendDataBuffer[0] = 0x85;
	m_SendDataBuffer[1] = nValue;
	m_SendDataBuffer[2] = 0xBF;

	m_SerialPort->Write(m_SendDataBuffer, 3);
	Delay(30);
	unsigned int com_length = 0;
	com_length = m_SerialPort->GetBytesInCOM();
	
	if (com_length >= 3)
	{
		m_SerialPort->Read(m_ReceiveDataBuffer, com_length);
		return true;
	}
	return false;
}

int __stdcall GP_Lens_Canon::IECommd(WORD nValue)
{
	memset(m_SendDataBuffer, 0, 30);
	m_SendDataBuffer[0] = 0x84;
	m_SendDataBuffer[1] = nValue;
	m_SendDataBuffer[2] = 0xBF;

	m_SerialPort->Write(m_SendDataBuffer, 3);
	Delay(30);
	unsigned int com_length = 0;
	com_length = m_SerialPort->GetBytesInCOM();
	if (com_length >= 3)
	{
		m_SerialPort->Read(m_ReceiveDataBuffer, com_length);
		return true;
	}
	return false;
}

int __stdcall GP_Lens_Canon::VersionCommd()
{
	memset(m_SendDataBuffer, 0, 30);
	//WORD SetHeadZero = 0x7F;
	m_SendDataBuffer[0] = 0x90;
	m_SendDataBuffer[1] = 0xC0;
	//m_SendDataBuffer[2] = ((byte)nValue) >> 14;
	//m_SendDataBuffer[3] = ((byte)(nValue >> 7)) & SetHeadZero;
	//m_SendDataBuffer[4] = ((byte)nValue)& SetHeadZero;

	m_SendDataBuffer[2] = 0x00;
	m_SendDataBuffer[3] = 0x02;
	m_SendDataBuffer[4] = 0x00;

	m_SendDataBuffer[5] = 0xBF;

	m_SerialPort->Write(m_SendDataBuffer, 6);
	Delay(30);
	unsigned int com_length = 0;
	com_length = m_SerialPort->GetBytesInCOM();

	if (com_length >= 6)
	{
		m_SerialPort->Read(m_ReceiveDataBuffer, com_length);
		return true;
	}
	return false;
}

bool __stdcall GP_Lens_Canon::GetControlSwitchCommd(WORD nValue)
{
	memset(m_SendDataBuffer, 0, 30);
	memset(m_ReceiveDataBuffer, 0, 50);
	m_SendDataBuffer[0] = 0xBE;
	m_SendDataBuffer[1] = 0x85;
	m_SendDataBuffer[2] = nValue;
	m_SendDataBuffer[3] = 0x01;
	m_SendDataBuffer[4] = 0x00;
	m_SendDataBuffer[5] = 0x00;
	m_SendDataBuffer[6] = 0xBF;

	m_SerialPort->Write(m_SendDataBuffer, 7);
	Delay(30);
	unsigned int com_length = 0;
	com_length = m_SerialPort->GetBytesInCOM();
	
	if (com_length >= 9)
	{
		m_SerialPort->Read(m_ReceiveDataBuffer, com_length);
		return true;
	}
	return false;
}

WORD __stdcall GP_Lens_Canon::PcSwitchCommd(WORD nValue)
{
	memset(m_SendDataBuffer, 0, 30);
	memset(m_ReceiveDataBuffer, 0, 50);
	m_SendDataBuffer[0] = 0xBE;
	m_SendDataBuffer[1] = 0x85;
	m_SendDataBuffer[2] = nValue;
	m_SendDataBuffer[3] = 0x01;
	m_SendDataBuffer[4] = 0x00;
	m_SendDataBuffer[5] = 0x02;
	m_SendDataBuffer[6] = 0x00;
	m_SendDataBuffer[7] = 0x50;
	m_SendDataBuffer[8] = 0xBF;


	m_SerialPort->Write(m_SendDataBuffer, 9);
	Delay(30);
	unsigned int com_length = 0;
	com_length = m_SerialPort->GetBytesInCOM();

	if (com_length >= 9)
	{
		m_SerialPort->Read(m_ReceiveDataBuffer, com_length);
		return m_ReceiveDataBuffer[7];
	}
	return false;
}
int __stdcall GP_Lens_Canon::GetLensMotorZoom(WORD &nValue)
{
	memset(m_SendDataBuffer, 0, 30);
	memset(m_ReceiveDataBuffer, 0, 50);
	m_SendDataBuffer[0] = 0x94;
	m_SendDataBuffer[1] = 0xC1;
	m_SendDataBuffer[2] = 0xBF;

	m_SerialPort->Write(m_SendDataBuffer, 3);
	Delay(20);
	unsigned int com_length = 0;
	com_length = m_SerialPort->GetBytesInCOM();
	if (com_length>=6)
	{
		m_SerialPort->Read(m_ReceiveDataBuffer, com_length);
	}
	nValue = (WORD)m_ReceiveDataBuffer[4] + ((WORD)m_ReceiveDataBuffer[2] << 14) + ((WORD)m_ReceiveDataBuffer[3] << 7);
	return false;
}

int __stdcall GP_Lens_Canon::GetLensMotorFocus(WORD &nValue)
{
	memset(m_SendDataBuffer, 0, 30);
	memset(m_ReceiveDataBuffer, 0, 50);
	m_SendDataBuffer[0] = 0x94;
	m_SendDataBuffer[1] = 0xC3;
	m_SendDataBuffer[2] = 0xBF;

	m_SerialPort->Write(m_SendDataBuffer, 3);
	Delay(20);
	unsigned int com_length = 0;
	com_length = m_SerialPort->GetBytesInCOM();

	if (com_length >= 6)
	{
		m_SerialPort->Read(m_ReceiveDataBuffer, com_length);
	}
	nValue = (WORD)m_ReceiveDataBuffer[4] + ((WORD)m_ReceiveDataBuffer[2] << 14) + ((WORD)m_ReceiveDataBuffer[3] << 7);
	return false;
}

int __stdcall GP_Lens_Canon::GetLensMotorIris(WORD &nValue)
{
	memset(m_SendDataBuffer, 0, 30);
	memset(m_ReceiveDataBuffer, 0, 50);
	m_SendDataBuffer[0] = 0x94;
	m_SendDataBuffer[1] = 0xC6;
	m_SendDataBuffer[2] = 0xBF;

	m_SerialPort->Write(m_SendDataBuffer, 3);
	Delay(30);
	unsigned int com_length = 0;
	com_length = m_SerialPort->GetBytesInCOM();

	if (com_length >= 6)
	{
		m_SerialPort->Read(m_ReceiveDataBuffer, com_length);
	}
	nValue = (WORD)m_ReceiveDataBuffer[4] + ((WORD)m_ReceiveDataBuffer[2] << 14) + ((WORD)m_ReceiveDataBuffer[3] << 7);
	return false;
}

int __stdcall GP_Lens_Canon::SetLensMotorZoom(WORD nValue)
{
	memset(m_SendDataBuffer, 0, 30);
	memset(m_ReceiveDataBuffer, 0, 50);
	WORD SetHeadZero = 0x7F;
	m_SendDataBuffer[0] = 0x87;
	m_SendDataBuffer[1] = 0xC0;
	m_SendDataBuffer[2] = (byte)(nValue >> 14);
	m_SendDataBuffer[3] = (byte)((nValue >> 7)&SetHeadZero);
	m_SendDataBuffer[4] = (byte)(nValue &SetHeadZero);
	m_SendDataBuffer[5] = 0xBF;

	m_SerialPort->Write(m_SendDataBuffer, 6);
	Delay(20);
	unsigned int com_length = 0;
	com_length = m_SerialPort->GetBytesInCOM();

	if (com_length >= 6)
	{
		m_SerialPort->Read(m_ReceiveDataBuffer, com_length);
		return true;
	}
	return false;
}

int __stdcall GP_Lens_Canon::SetLensMotorFocus(WORD nValue)
{
	memset(m_SendDataBuffer, 0, 30);
	memset(m_ReceiveDataBuffer, 0, 50);
	WORD SetHeadZero = 0x7F;
	m_SendDataBuffer[0] = 0x88;
	m_SendDataBuffer[1] = 0xC0;
	m_SendDataBuffer[2] = (byte)(nValue >> 14);
	m_SendDataBuffer[3] = ((byte)(nValue >> 7)) & SetHeadZero;
	m_SendDataBuffer[4] = ((byte)(nValue)) & SetHeadZero;
	m_SendDataBuffer[5] = 0xBF;

	m_SerialPort->Write(m_SendDataBuffer, 6);
	Delay(20);
	unsigned int com_length = 0;
	com_length = m_SerialPort->GetBytesInCOM();

	if (com_length >= 6)
	{
		m_SerialPort->Read(m_ReceiveDataBuffer, com_length);
		return true;
	}
	return false;
}

int __stdcall GP_Lens_Canon::SetLensMotorIris(WORD nValue)
{
	memset(m_SendDataBuffer, 0, 30);
	memset(m_ReceiveDataBuffer, 0, 50);
	WORD SetHeadZero = 0x7F;
	m_SendDataBuffer[0] = 0x96;
	m_SendDataBuffer[1] = 0xC0;
	m_SendDataBuffer[2] = (byte)(nValue >> 14);
	m_SendDataBuffer[3] = (byte)((nValue >> 7)&SetHeadZero);
	m_SendDataBuffer[4] = (byte)(nValue &SetHeadZero);
	m_SendDataBuffer[5] = 0xBF;

	m_SerialPort->Write(m_SendDataBuffer, 6);
	Delay(30);
	unsigned int com_length = 0;
	com_length = m_SerialPort->GetBytesInCOM();

	if (com_length >= 6)
	{
		m_SerialPort->Read(m_ReceiveDataBuffer, com_length);
		return true;
	}
	return false;
}

int __stdcall GP_Lens_Canon::ZoomSpeedSet(WORD nValue)
{
	memset(m_SendDataBuffer, 0, 30);
	memset(m_ReceiveDataBuffer, 0, 50);
	WORD SetHeadZero = 0x7F;
	m_SendDataBuffer[0] = 0x87;
	m_SendDataBuffer[1] = 0xC1;
	m_SendDataBuffer[2] = (byte)(nValue >> 14);
	m_SendDataBuffer[3] = (byte)((nValue >> 7)&SetHeadZero);
	m_SendDataBuffer[4] = (byte)(nValue &SetHeadZero);
	m_SendDataBuffer[5] = 0xBF;

	m_SerialPort->Write(m_SendDataBuffer, 6);
	Delay(20);
	unsigned int com_length = 0;
	com_length = m_SerialPort->GetBytesInCOM();

	if (com_length >= 6)
	{
		m_SerialPort->Read(m_ReceiveDataBuffer, com_length);
		return true;
	}
	return false;
}


int __stdcall GP_Lens_Canon::FocusSpeedSet(WORD nValue)
{
	memset(m_SendDataBuffer, 0, 30);
	memset(m_ReceiveDataBuffer, 0, 50);
	WORD SetHeadZero = 0x7F;
	m_SendDataBuffer[0] = 0x88;
	m_SendDataBuffer[1] = 0xC1;
	m_SendDataBuffer[2] = (byte)(nValue >> 14);
	m_SendDataBuffer[3] = (byte)((nValue >> 7)&SetHeadZero);
	m_SendDataBuffer[4] = (byte)(nValue &SetHeadZero);
	m_SendDataBuffer[5] = 0xBF;

	m_SerialPort->Write(m_SendDataBuffer, 6);
	Delay(20);
	unsigned int com_length = 0;
	com_length = m_SerialPort->GetBytesInCOM();

	if (com_length >= 6)
	{
		m_SerialPort->Read(m_ReceiveDataBuffer, com_length);
		return true;
	}
	return false;
}


void GP_Lens_Canon::Delay(int MilliSeconds)
{
	LARGE_INTEGER LagInteg;
	LONGLONG InitFreq, UltiFerq;
	double ClockFreq, SpaceTime;

	QueryPerformanceFrequency(&LagInteg);
	ClockFreq = LagInteg.QuadPart;
	QueryPerformanceCounter(&LagInteg);
	InitFreq = (double)LagInteg.QuadPart;
	do
	{
		QueryPerformanceCounter(&LagInteg);
		UltiFerq = (double)LagInteg.QuadPart;
		SpaceTime = (UltiFerq - InitFreq) / ClockFreq*1000;
	} while (SpaceTime <= MilliSeconds);
}


