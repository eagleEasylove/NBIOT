
#include <windows.h>  
#include <stdio.h>  

#include "SerialPort_WIN32.h"

namespace CORE {

CSerialPort::CSerialPort()
{
    m_hHandle = INVALID_HANDLE_VALUE;  
}

CSerialPort::~CSerialPort()
{

}

int CSerialPort::openCom(const char* strSerialPortName)
{
	if (m_hHandle == INVALID_HANDLE_VALUE)
	{
		/** ��ָ���Ĵ��� */
		m_hHandle = CreateFile(strSerialPortName,  /** �豸��,COM1,COM2�� */
			GENERIC_READ | GENERIC_WRITE, /** ����ģʽ,��ͬʱ��д */
			0,                            /** ����ģʽ,0��ʾ������ */
			NULL,                         /** ��ȫ������,һ��ʹ��NULL */
			OPEN_EXISTING,                /** �ò�����ʾ�豸�������,���򴴽�ʧ�� */
			0,
			0);

		/** �����ʧ�ܣ��ͷ���Դ������ */
		if (m_hHandle == INVALID_HANDLE_VALUE)
		{
			return -1;
		}

		// ��ȡ�����ô��ڲ���  
		DCB myDCB;
		GetCommState(m_hHandle, &myDCB);  // ��ȡ���ڲ���  
		//    printf("baud rate is %d",(int)myDCB.BaudRate);  
		//   fflush(stdout);  
		myDCB.BaudRate = 9600;       // ������  
		myDCB.Parity = NOPARITY;   // У��λ  
		myDCB.ByteSize = 8;          // ����λ  
		myDCB.StopBits = ONESTOPBIT; // ֹͣλ  
		bool flag = SetCommState(m_hHandle, &myDCB);  // ���ô��ڲ��� 

		//#define JUST_FOR_TEST
#ifdef JUST_FOR_TEST
		GetCommState(m_hHandle, &myDCB);  // ��ȡ���ڲ���  

		Thread::sleep(1000);
		//	CData tmp = "AT+MIPLVER?\r\n";
		CData tmp = "AT+CGSN=1\r\n";
		//	CData tmp = "AT+CIMI=1\r\n";
		write(tmp.c_str(), tmp.length());

		char buf[100];
		read(buf, 100);
#endif
	}

	return 0;
}

int CSerialPort::closeCom()
{
	/** ����д��ڱ��򿪣��ر��� */  
	if (m_hHandle != INVALID_HANDLE_VALUE)  
	{  
		CloseHandle(m_hHandle);  
		m_hHandle = INVALID_HANDLE_VALUE;  
	}  
	return 0;
}


int CSerialPort::ioCtrl(unsigned int speed, int databits,  int stopbits,  int parity)
{
	if (m_hHandle == INVALID_HANDLE_VALUE)
	{
		return -1 ;
	}
	// ��ȡ�����ô��ڲ���  
	DCB myDCB;  
	GetCommState(m_hHandle, &myDCB);  // ��ȡ���ڲ���  
	//    printf("baud rate is %d",(int)myDCB.BaudRate);  
	//   fflush(stdout);  
	myDCB.BaudRate = speed;       // ������  
	myDCB.Parity  =  NOPARITY;   // У��λ  
	myDCB.ByteSize = databits;          // ����λ  
	myDCB.StopBits = ONESTOPBIT; // ֹͣλ  
	SetCommState(m_hHandle, &myDCB);  // ���ô��ڲ���  
	return 0;
}

int CSerialPort::writeCom(const char* data, Int32 len)
{
	BOOL   bResult = TRUE;  
	DWORD  BytesToSend = 0;  
	if (m_hHandle == INVALID_HANDLE_VALUE)
	{  
	    return -1;  
	}  

	/** �򻺳���д��ָ���������� */  
	bResult = WriteFile(m_hHandle, data, len, &BytesToSend, NULL);  
	if (!bResult)  
	{  
	    DWORD dwError = GetLastError();  
	    /** ��մ��ڻ����� */  
	    PurgeComm(m_hHandle, PURGE_RXCLEAR | PURGE_RXABORT);  
	    return -1;  
	}  
	return BytesToSend;
}

int CSerialPort::readCom(char *pBuf, int ilen)
{
	BOOL  bResult = TRUE;  
	DWORD BytesRead = 0;  
	if (m_hHandle == INVALID_HANDLE_VALUE)  
	{  
	    return -1;  
	}  

	/** �ӻ�������ȡһ���ֽڵ����� */  
	bResult = ReadFile(m_hHandle, pBuf, ilen, &BytesRead, NULL);  
	if ((!bResult))  
	{  
	    /** ��ȡ������,���Ը��ݸô�����������ԭ�� */  
	    DWORD dwError = GetLastError();  

	    /** ��մ��ڻ����� */  
	    PurgeComm(m_hHandle, PURGE_RXCLEAR | PURGE_RXABORT);  

	    return -1;  
	}  

	return BytesRead;
}
}

