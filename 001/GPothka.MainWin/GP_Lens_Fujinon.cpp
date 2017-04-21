#include "stdafx.h"
#include "GP_Lens_Fujinon.h"

 
 

GP_Lens_Fujinon::GP_Lens_Fujinon()
{

}

GP_Lens_Fujinon::~GP_Lens_Fujinon()
{

}



//int __stdcall GP_Lens_Fujinon::VerifyPreScene(TCHAR* pCommName, UINT pBaudRate /*= CBR_38400*/, UINT pParity /*= NOPARITY*/, UINT pByteSize /*= 8*/, UINT pStopBits /*= ONESTOPBIT*/)
//{
//	if (this->m_comHandle != INVALID_HANDLE_VALUE) //串口可能是打开的，先关闭串口
//	{
//		CloseHandle(this->m_comHandle);
//	}
//	if (SeriPortInitial(pCommName, pBaudRate))
//	{
//		return true;
//	}
//	else
//		return false;
//}


int __stdcall GP_Lens_Fujinon::InitComPort(CString  portNo, UINT  baud)
{
	//m_SerialPort = new CFuSerialPort(NULL);
     return m_SerialPort->InitPort(portNo, baud);
	 
}

int __stdcall GP_Lens_Fujinon::IsConnect()
{
	int i = 0;

	while (true)
	{
		m_SendDataBuffer[0] = 0x00;
		m_SendDataBuffer[1] = 0x01;
		m_SendDataBuffer[2] = 0x100 - m_SendDataBuffer[0] - m_SendDataBuffer[1];
		m_SerialPort->Write(m_SendDataBuffer, 3);
		Sleep(300);

		UINT ByteNum = m_SerialPort->GetBytesInCOM();
		if (ByteNum >= 3)
		{
			m_SerialPort->Read(m_ReceiveDataBuffer, 3);
			return true;
		}
		i++;
		if (i == 5)
		{
			return false;
		}
	}
	return true;
}

int __stdcall GP_Lens_Fujinon::GetLensName(char* pLensName)
{
	//if (IsConnect() == true);
	////double elap = 0;
	////m_StopWatch.StartTiming();
	////while (elap < 10){
	////	elap = m_StopWatch.Elapsed();
	////}
	//m_SendDataBuffer[0] = 0x00;
	//m_SendDataBuffer[1] = 0x11;
	//m_SendDataBuffer[2] = 0x100 - m_SendDataBuffer[0] - m_SendDataBuffer[1];
	//m_SerialPort.Write(m_SendDataBuffer, 3);
	//m_SerialPort.Read(this->m_ReceiveDataBuffer, com_length1);
	//memcpy_s(pLensName, com_length1 + 1, (const char*)m_ReceiveDataBuffer + 2, com_length1);
	//
	return 0;
}

int __stdcall GP_Lens_Fujinon::GetLensMotorZoom(WORD &ResultValue)
{
	m_SendDataBuffer[0] = 0x00;
	m_SendDataBuffer[1] = 0x31;
	m_SendDataBuffer[2] = 0x100 - m_SendDataBuffer[0] - m_SendDataBuffer[1];
	m_SerialPort->Write(m_SendDataBuffer, 3);
	
	memset(this->m_ReceiveDataBuffer, 0, 30);
	m_SerialPort->Read(this->m_ReceiveDataBuffer, 5);//10
	ResultValue = (m_ReceiveDataBuffer[2] << 4) + (m_ReceiveDataBuffer[3] >> 4);

	return false;
}

int __stdcall GP_Lens_Fujinon::SetLensMotorZoom(WORD nValue)
{
	memset(this->m_SendDataBuffer, 0, 30);
	m_SendDataBuffer[0] = 0x02;
	m_SendDataBuffer[1] = 0x21;
	m_SendDataBuffer[2] = (byte)(nValue >> 4);
	m_SendDataBuffer[3] = (byte)(nValue << 4);
	byte sum = m_SendDataBuffer[0] + m_SendDataBuffer[1] + m_SendDataBuffer[2] + m_SendDataBuffer[3];
	m_SendDataBuffer[4] = 0x100 - sum;
	m_SerialPort->Write(m_SendDataBuffer, 5);

	memset(this->m_ReceiveDataBuffer, 0, 30);
	m_SerialPort->Read(this->m_ReceiveDataBuffer, 3);

	return false;
}

int __stdcall GP_Lens_Fujinon::GetLensMotorFocus(WORD &nValue)
{
	memset(this->m_SendDataBuffer, 0, 30);
	m_SendDataBuffer[0] = 0x00;
	m_SendDataBuffer[1] = 0x32;
	m_SendDataBuffer[2] = 0x100 - m_SendDataBuffer[0] - m_SendDataBuffer[1];
	m_SerialPort->Write(m_SendDataBuffer, 3);

	memset(this->m_ReceiveDataBuffer, 0, 30);
	m_SerialPort->Read(this->m_ReceiveDataBuffer, 5);
	nValue = (m_ReceiveDataBuffer[2] << 4) + (m_ReceiveDataBuffer[3] >> 4);
	return false;
}

int __stdcall GP_Lens_Fujinon::GetLensMotorIris(WORD &nValue)
{
	memset(this->m_SendDataBuffer, 0, 30);
	m_SendDataBuffer[0] = 0x00;
	m_SendDataBuffer[1] = 0x30;
	m_SendDataBuffer[2] = 0x100 - m_SendDataBuffer[0] - m_SendDataBuffer[1];
	m_SerialPort->Write(m_SendDataBuffer, 3);


	memset(this->m_ReceiveDataBuffer, 0, 30);
	m_SerialPort->Read(this->m_ReceiveDataBuffer, 5);
	nValue = (m_ReceiveDataBuffer[2] << 4) + (m_ReceiveDataBuffer[3] >> 4);
	//nValue = m_ReceiveDataBuffer[2] << 8 + m_ReceiveDataBuffer[3];
	return false;
}

int __stdcall GP_Lens_Fujinon::SetLensMotorFocus(WORD nValue)
{
	memset(this->m_SendDataBuffer, 0, 30);
	m_SendDataBuffer[0] = 0x02;
	m_SendDataBuffer[1] = 0x22;
	m_SendDataBuffer[2] = (byte)(nValue >> 4);
	m_SendDataBuffer[3] = (byte)(nValue << 4);

	byte sum = m_SendDataBuffer[0] + m_SendDataBuffer[1] + m_SendDataBuffer[2] + m_SendDataBuffer[3];
	m_SendDataBuffer[4] = 0x100 - sum;
	m_SerialPort->Write(m_SendDataBuffer, 5);
	m_SerialPort->Read(this->m_ReceiveDataBuffer, 3);

	return true;
 
}

int __stdcall GP_Lens_Fujinon::SetLensMotorIris(WORD nValue)
{
	memset(this->m_SendDataBuffer, 0, 30);
	m_SendDataBuffer[0] = 0x02;
	m_SendDataBuffer[1] = 0x20;
	m_SendDataBuffer[2] = (byte)(nValue >> 4);
	m_SendDataBuffer[3] = (byte)(nValue << 4); 
	byte sum = m_SendDataBuffer[0] + m_SendDataBuffer[1] + m_SendDataBuffer[2] + m_SendDataBuffer[3];
	m_SendDataBuffer[4] = 0x100 - sum;
	m_SerialPort->Write(m_SendDataBuffer, 5);

	memset(this->m_ReceiveDataBuffer, 0, 30);
	m_SerialPort->Read(this->m_ReceiveDataBuffer, 3);
	return false;
}

int __stdcall GP_Lens_Fujinon::GetLensMotorZoomFocusIris(WORD &nZoomValue, WORD &nFocusValue, WORD &nIrisValue)
{
//	memset(this->m_ReceiveDataBuffer, 0, 30);
//	m_SendDataBuffer[0] = 0x03;
//	m_SendDataBuffer[1] = 0x70;
//	m_SendDataBuffer[2] = 0x30;
//	m_SendDataBuffer[3] = 0x31;
//	m_SendDataBuffer[4] = 0x32;
//	byte sum = m_SendDataBuffer[0] + m_SendDataBuffer[1] + m_SendDataBuffer[2] + m_SendDataBuffer[3] + m_SendDataBuffer[4];
//	m_SendDataBuffer[5] = 0x100 - sum;
//	m_SerialPort.Write(m_SendDataBuffer, 6);
//	memset(this->m_ReceiveDataBuffer, 0, 30);
//	unsigned int com_length1 = 0;
//	bool dd = this->ComBufferDataLength(com_length1, 100); //此处com_length1 应该为3
//	if (dd)
//	{
//		this->ReceiveData(this->m_ReceiveDataBuffer, com_length1);
//	}
//	 
//
//	memset(this->m_ReceiveDataBuffer, 0, 30);
//	m_SendDataBuffer[0] = 0x00;
//	m_SendDataBuffer[1] = 0x60;
//	 sum = m_SendDataBuffer[0] + m_SendDataBuffer[1];
//	m_SendDataBuffer[2] = 0x100 - sum;
//	this->SendData(m_SendDataBuffer, 3);
//	memset(this->m_ReceiveDataBuffer, 0, 30);
//	 com_length1 = 0;
//	 dd = this->ComBufferDataLength(com_length1, 100); //此处com_length1 应该为3
//	if (dd)
//	{
//		this->ReceiveData(this->m_ReceiveDataBuffer, com_length1);
//	}
	return false;
}

int __stdcall GP_Lens_Fujinon::SetLensMotorZoomFocusIris(WORD nZoomValue, WORD nFocusValue, WORD nIrisValue)
{


	return true;
}

int __stdcall GP_Lens_Fujinon::SetSwitch2Host(BYTE nValue)
{
	m_SendDataBuffer[0] = 0x01;
	m_SendDataBuffer[1] = 0x41;
	m_SendDataBuffer[2] = nValue;
	byte sum = m_SendDataBuffer[0] + m_SendDataBuffer[1] + m_SendDataBuffer[2];
	m_SendDataBuffer[3] = 0x100 - sum;
	m_SerialPort->Write(m_SendDataBuffer, 4);
	
	m_SerialPort->Read(this->m_ReceiveDataBuffer, 3);
	return false;

}

int __stdcall GP_Lens_Fujinon::GetSwitch2Host(char* sNote, WORD &nValue)
{
	m_SendDataBuffer[0] = 0x00;
	m_SendDataBuffer[1] = 0x52;
	byte sum = m_SendDataBuffer[0] + m_SendDataBuffer[1];
	m_SendDataBuffer[2] = 0x100 - sum;
	m_SerialPort->Write(m_SendDataBuffer, 3);
	unsigned int com_length1 = 0;
	m_SerialPort->Read(this->m_ReceiveDataBuffer, com_length1);  //00 44 BC
		nValue = m_ReceiveDataBuffer[2];
		switch (m_ReceiveDataBuffer[2]){
		case 0xF8:
			sNote = "";
			break;
		case 0xFF:
			sNote = "";
			break;
		default:
			sNote = "";
			break;
			return true;
		
	}
	sNote = "中国";
	return false;
}

int __stdcall GP_Lens_Fujinon::SetSwitch4Host(BYTE nValue)
{
	m_SendDataBuffer[0] = 0x01;
	m_SendDataBuffer[1] = 0x44;
	m_SendDataBuffer[2] = nValue;
	byte sum = m_SendDataBuffer[0] + m_SendDataBuffer[1] + m_SendDataBuffer[2];
	m_SendDataBuffer[3] = 0x100 - sum;
	m_SerialPort->Write(m_SendDataBuffer, 4);
	Sleep(100);
	m_SerialPort->Read(this->m_ReceiveDataBuffer, 3);  //00 44 BC
		
	return false;
}

int __stdcall GP_Lens_Fujinon::GetSwitch4Host(char* sNote, WORD &nValue)
{
	m_SendDataBuffer[0] = 0x00;
	m_SendDataBuffer[1] = 0x54;
	byte sum = m_SendDataBuffer[0] + m_SendDataBuffer[1];
	m_SendDataBuffer[2] = 0x100 - sum;
	bool ab = m_SerialPort->Write(m_SendDataBuffer, 3);

	m_SerialPort->Read(m_ReceiveDataBuffer,4);
	//unsigned int com_length1 = 0;
	//bool dd = this->ComBufferDataLength(com_length1, 100);
	//if (dd)
	//{
	//	this->ReceiveData(this->m_ReceiveDataBuffer, com_length1);  //00 44 BC
	//	nValue = m_ReceiveDataBuffer[2];
	//	switch (m_ReceiveDataBuffer[2]){
	//		case 0xF8:
	//			sNote = "";
	//			break;
	//		case 0xFF:
	//			sNote = "";
	//			break;
	//		default:
	//			sNote = "";
	//			break;
	//}
	//	return true;
	//}
	//sNote = "";
	return false;
}

int __stdcall GP_Lens_Fujinon::SetMotorSpeedsFocus(WORD nValue)

{
	memset(this->m_ReceiveDataBuffer, 0, 30);
	m_SendDataBuffer[0] = 0x02;
	m_SendDataBuffer[1] = 0x27;
	m_SendDataBuffer[2] = (byte)(nValue >> 4);
	m_SendDataBuffer[3] = (byte)(nValue << 4);
	m_SendDataBuffer[4] = 0x100 - m_SendDataBuffer[0] - m_SendDataBuffer[1] - m_SendDataBuffer[2] - m_SendDataBuffer[3];
	m_SerialPort->Write(m_SendDataBuffer, 5);
	m_SerialPort->Read(this->m_ReceiveDataBuffer, 3);
	return false;
}


int __stdcall GP_Lens_Fujinon::SetMotorSpeedsZoom(WORD nValue)

{
	memset(this->m_ReceiveDataBuffer, 0, 30);
	m_SendDataBuffer[0] = 0x02;
	m_SendDataBuffer[1] = 0x26;
	m_SendDataBuffer[2] = (byte)(nValue >> 4);
	m_SendDataBuffer[3] = (byte)(nValue << 4);
	m_SendDataBuffer[4] = 0x100 - m_SendDataBuffer[0] - m_SendDataBuffer[1] - m_SendDataBuffer[2] - m_SendDataBuffer[3];
	m_SerialPort->Write(m_SendDataBuffer, 5);
	m_SerialPort->Read(this->m_ReceiveDataBuffer, 3);
	return false;
}







