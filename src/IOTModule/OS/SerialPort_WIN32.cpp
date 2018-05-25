
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
		/** 打开指定的串口 */
		m_hHandle = CreateFile(strSerialPortName,  /** 设备名,COM1,COM2等 */
			GENERIC_READ | GENERIC_WRITE, /** 访问模式,可同时读写 */
			0,                            /** 共享模式,0表示不共享 */
			NULL,                         /** 安全性设置,一般使用NULL */
			OPEN_EXISTING,                /** 该参数表示设备必须存在,否则创建失败 */
			0,
			0);

		/** 如果打开失败，释放资源并返回 */
		if (m_hHandle == INVALID_HANDLE_VALUE)
		{
			return -1;
		}

		// 获取和设置串口参数  
		DCB myDCB;
		GetCommState(m_hHandle, &myDCB);  // 获取串口参数  
		//    printf("baud rate is %d",(int)myDCB.BaudRate);  
		//   fflush(stdout);  
		myDCB.BaudRate = 9600;       // 波特率  
		myDCB.Parity = NOPARITY;   // 校验位  
		myDCB.ByteSize = 8;          // 数据位  
		myDCB.StopBits = ONESTOPBIT; // 停止位  
		bool flag = SetCommState(m_hHandle, &myDCB);  // 设置串口参数 

		//#define JUST_FOR_TEST
#ifdef JUST_FOR_TEST
		GetCommState(m_hHandle, &myDCB);  // 获取串口参数  

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
	/** 如果有串口被打开，关闭它 */  
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
	// 获取和设置串口参数  
	DCB myDCB;  
	GetCommState(m_hHandle, &myDCB);  // 获取串口参数  
	//    printf("baud rate is %d",(int)myDCB.BaudRate);  
	//   fflush(stdout);  
	myDCB.BaudRate = speed;       // 波特率  
	myDCB.Parity  =  NOPARITY;   // 校验位  
	myDCB.ByteSize = databits;          // 数据位  
	myDCB.StopBits = ONESTOPBIT; // 停止位  
	SetCommState(m_hHandle, &myDCB);  // 设置串口参数  
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

	/** 向缓冲区写入指定量的数据 */  
	bResult = WriteFile(m_hHandle, data, len, &BytesToSend, NULL);  
	if (!bResult)  
	{  
	    DWORD dwError = GetLastError();  
	    /** 清空串口缓冲区 */  
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

	/** 从缓冲区读取一个字节的数据 */  
	bResult = ReadFile(m_hHandle, pBuf, ilen, &BytesRead, NULL);  
	if ((!bResult))  
	{  
	    /** 获取错误码,可以根据该错误码查出错误原因 */  
	    DWORD dwError = GetLastError();  

	    /** 清空串口缓冲区 */  
	    PurgeComm(m_hHandle, PURGE_RXCLEAR | PURGE_RXABORT);  

	    return -1;  
	}  

	return BytesRead;
}
}

