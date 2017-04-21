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
	// ����ʱ�䣺  2016/09/20  16:37 
	// ��    �ߣ�  GP_Fore
	//********************************************************************
	// ����˵����  
	// �����б�  TCHAR * pCommName             /** �豸��,"COM1",COM2�� */
	// �����б�  DWORD dwDesiredAccess         /** ����ģʽ,��ͬʱ��д */
	// �����б�  DWORD dwShareMode			 /** ����ģʽ,0��ʾ������ */
	// �����б�  LPSECURITY_ATTRIBUTES lpSecurityAttributes  /** ��ȫ������,һ��ʹ��NULL */
	// �����б�  DWORD dwCreationDisposition    /**   �ò�����ʾ�豸�������,���򴴽�ʧ��  OPEN_EXISTING��Ĭ�ϲ�����ʾ�豸�������,���򴴽�ʧ�� */
	// �����б�  DWORD dwFlagsAndAttributes     /** ������ʽ 0��Ĭ�ϲ���Ϊͬ����ʽ*/
	// �����б�  HANDLE hTemplateFile
	// ����ֵ:     bool
	// ��    ע��  ���˴��ڻ���������     
	//********************************************************************
	bool OpenCommPort(TCHAR* pCommName, DWORD dwDesiredAccess = GENERIC_READ | GENERIC_WRITE, DWORD dwShareMode = 0, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL, DWORD dwCreationDisposition = OPEN_EXISTING, DWORD dwFlagsAndAttributes=0, HANDLE hTemplateFile=NULL);

	//********************************************************************
	// ����ʱ�䣺  2016/05/22  16:17 
	// ��    �ߣ�  GP_Fore
	//********************************************************************
	// ����˵����  ���ô��ڳ�ʱʱ��  
	// �����б�  DWORD ReadInterval  //�������ʱ
	// �����б�  DWORD ReadTotalMultiplier //��ʱ��ϵ��
	// �����б�  DWORD ReadTotalconstant  //��ʱ�䳣��
	// �����б�  DWORD WriteTotalMultiplier  // дʱ��ϵ��
	// �����б�  DWORD WriteTotalconstant   //дʱ�䳣��
	// ����ֵ:     bool  �ɹ� true;ʧ�� false
	// ��    ע��  ע�⣺���ܳ�ʱ��ReadTotalTimeoutMultiplier��10��ReadTotalTimeoutConstant
	//					 ���Կ����������ʱ���ܳ�ʱ�������ǲ���صģ�����Է���ͨ�ų����������ø��ֳ�ʱ��
	//					 �������д��ʱ������Ϊ0����ô�Ͳ�ʹ��д��ʱ�����ReadIntervalTimeoutΪ0����ô�Ͳ�ʹ�ö������ʱ
	//                   �����ȴ�һ�����ڣ���֤m_comHandleָ�벻Ϊ�ա�����û�򿪵ȶ��ᵼ�����ó���
	//********************************************************************
	bool SetCommTimeout(DWORD ReadInterval = 0, DWORD ReadTotalMultiplier=0, DWORD ReadTotalconstant=0, DWORD WriteTotalMultiplier=0, DWORD WriteTotalconstant=0);

	//********************************************************************
	// ����ʱ�䣺  2016/05/22  16:17 
	// ��    �ߣ�  GP_Fore
	//********************************************************************
	// ����˵���� ���ô���DCB����  
	// �����б� ������,Ĭ��Ϊ9600
	// �����б� UINT pParity ��żУ��  NOPARITY:0������żУ�飩 ODDPARITY:1 ����  �Ƿ������żУ��,true��ʾ��Ҫ��żУ��,false��ʾ����Ҫ��żУ��  
	// �����б� ����λ�ĸ���,Ĭ��ֵΪ8������λ
	// �����б� UINT pStopBits  ע��0(ONESTOPBIT)������һ��ֹͣλ��2(TWOSTOPBITS):��������ֹͣλ�� ��֧��1.5��ֹͣλ  Ĭ��Ϊ1��ֹͣλ
	// ����ֵ:    �ɹ� true;ʧ�� false
	// ��    ע��        
	//********************************************************************
	bool SetCommDCBParam(UINT pBaudRate = CBR_9600, UINT pParity = NOPARITY, UINT  pByteSize = 8, UINT pStopBits = ONESTOPBIT);

	//********************************************************************
	// ����ʱ�䣺  2016/05/22  16:44 
	// ��    �ߣ�  GP_Fore
	//********************************************************************
	// ����˵����  ������֤���������Ϊtrue,ʹ��Ĭ�ϲ��������˴��ڡ���������ʹ�÷��ͺͽ��ܺ����ˡ�
	// �����б�  TCHAR * pCommName
	// �����б�  UINT pBaudRate
	// �����б�  UINT pParity ��żУ��  NOPARITY:0������żУ�飩 ODDPARITY:1 ���� 
	// �����б�  UINT pByteSize
	// �����б�  UINT pStopBits ע��0(ONESTOPBIT)������һ��ֹͣλ��2(TWOSTOPBITS):��������ֹͣλ�� ��֧��1.5��ֹͣλ  Ĭ��Ϊ1��ֹͣλ
	// ����ֵ:     �ɹ� true;ʧ�� false
	// ��    ע��        
	//********************************************************************
	bool SeriPortInitial(TCHAR* pCommName, UINT pBaudRate = CBR_9600, UINT pParity = NOPARITY, UINT  pByteSize = 8, UINT pStopBits = ONESTOPBIT);
	
	//********************************************************************
	// ����ʱ�䣺  2016/05/22  17:11 
	// ��    �ߣ�  GP_Fore
	//********************************************************************
	// ����˵����  ��ָ���ַ������͵���ǰ����
	// �����б�  TCHAR * pSendData
	// ����ֵ:     bool �ɹ� true;ʧ�� false
	// ��    ע��  pSendData �ַ����м������0x00����ֻʶ��0x00֮ǰ�����ݣ����ʱ��ϣ��ʹ��SendData(byte pSendData[], unsigned int pDataLength)     
	//********************************************************************
    bool SendData(unsigned char* pSendData, unsigned int pDataLength);
	//bool SendData(byte pSendData[], unsigned int pDataLength);



	//********************************************************************
	// ����ʱ�䣺  2016/06/15  14:38 
	// ��    �ߣ�  GP_Fore
	//********************************************************************
	// ����˵����  ��ȡ��ǰ���com�ڻ����������ݳ��ȡ�
	// �����б�  unsigned int& comBufferSize  ����ֵ����ǰ�����������ݳ��ȡ�
	// ����ֵ:     bool �ɹ�true;,ʧ�� false;
	// ��    ע��        
	//********************************************************************

	//********************************************************************
	// ����ʱ�䣺  2016/07/08  10:28 
	// ��    �ߣ�  GP_Fore
	//********************************************************************
	// ����˵����  ��ȡ��ǰ���com�ڻ����������ݳ��ȡ�
	// �����б�  unsigned int& comBufferSize  ����ֵ����ǰ�����������ݳ��ȡ�
	// �����б�  unsigned int pWaitTime ��λΪ������  �ȴ�ʱ�䣬
	// ����ֵ:     bool
	// ��    ע��        
	//********************************************************************
	bool ComBufferDataLength(unsigned int& comBufferSize,unsigned int pWaitTime=0);

	//********************************************************************
	// ����ʱ�䣺  2016/06/15  14:17 
	// ��    �ߣ�  GP_Fore
	//********************************************************************
	// ����˵����  ��������ȴ����������ж��ٽ��ܶ��٣������������ء�
	// �����б�  char * pReceiveData�����ؽ��ܵĴ�������
	// ����ֵ:     bool
	// ��    ע��  pReceiveData �����ں����ڿ����ڴ棬�ⲿʹ������Ҫ��free(pReceiveData)�����ڴ�ռ��ͷ�;    
	//********************************************************************
	bool ReceiveChar(unsigned char &pReceiveChar);
	bool ReceiveByte(byte &pReceiveByte);

	//********************************************************************
	// ����ʱ�䣺  2016/05/25  16:33 
	// ��    �ߣ�  GP_Fore
	//********************************************************************
	// ����˵����  ����ָ�����ȵ��ַ����������ǰ���ڻ����е����ݳ���û�дﵽpDataLengthָ���ĳ��ȣ���ô�˺����ͻ�ȴ���
	//             �ڵ��ô˺���֮ǰ����ʹ��ComBufferDataLength������ȷ�ϴ��ڻ�������ݳ��ȡ���ָ��pDataLength��
	// �����б�  char pReceiveData[]
	// �����б�  unsigned int pDataLength
	// ����ֵ:     bool
	// ��    ע��        
	//********************************************************************
	bool ReceiveData(char pReceiveData[],const unsigned int pDataLength);
	bool ReceiveData(byte pReceiveData[], const unsigned int pDataLength);
	bool ClosePort();
public:

};

#endif