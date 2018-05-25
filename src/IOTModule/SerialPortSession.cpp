
#include "SerialPortSession.h"

#if defined(CORE_OS_FAMILY_WINDOWS)
#include "OS/SerialPort_WIN32.cpp"
#else
#include "OS/SerialPort_POSIX.cpp"
#endif


CSerialPortSession::CSerialPortSession()
{
	m_observer = NULL;
}

CSerialPortSession::~CSerialPortSession()
{

}

int CSerialPortSession::init(CData strPortName, int portSpeed)
{
	int  iRet = -1;
	if(strPortName != "")
	{
		iRet = m_serialPort.openCom(strPortName.c_str());
		printf("--->CSerialPortSession::init() 00 : %s, iRet:%d\n", strPortName.c_str(), iRet);

		if(iRet != -1)
		{
			iRet  = m_serialPort.ioCtrl(portSpeed);
			
			m_thread.start(*this);
		}
	}
	
	return iRet;
}

int CSerialPortSession::unInit()
{
	int  iRet = m_serialPort.closeCom();

	printf("--->CSerialPortSession::close()\n");

	return iRet;
}


int CSerialPortSession::sessionOutput(const char* data, int len)
{
	int iWsize = 0;
//	printf("--->CSerialPortSession::sessionOutput:write start len:%d,write:%d, %s\n", len, iWsize, data);
	iWsize = m_serialPort.writeCom(data, len);
//	printf("--->CSerialPortSession::sessionOutput:write end len:%d£¬write:%d,,%s\n", len, iWsize, data);
//	printf("--->write datalen:%d£¬writeSize:%d \n%s\n", len, iWsize, data);
	printf("--->write datalen:%d %s\n", len, data);
	return iWsize;
}

void CSerialPortSession::setObserver(CAsynSessionObserver* observer)
{
	m_observer=observer;
}

void CSerialPortSession::setSessionParam(unsigned int speed, int databits ,  int stopbits ,  int parity)
{
	m_serialPort.ioCtrl(speed, databits ,  stopbits ,  parity);
}

void CSerialPortSession::run()
{
	printf("--->CSerialPortSession::run()  \n");
	char *pBuf = new char[MAX_SERIAL_RCV_BUF_LEN];
	while(1)
	{
		memset(pBuf, 0x00, MAX_SERIAL_RCV_BUF_LEN);
//	printf("--->CSerialPortSession::run() 000 \n");
		int len = m_serialPort.readCom(pBuf, MAX_SERIAL_RCV_BUF_LEN);
//	printf("--->CSerialPortSession::run() 111 len:%d \n",  len);
		if(len>0)
		{
			int tmpLen = 0;
			do
			{
				Thread::sleep(100);
			//	printf("--->CSerialPortSession::run() 222 tmpLen:%d,len:%d \n", tmpLen, len);

				tmpLen = m_serialPort.readCom(pBuf + len, MAX_SERIAL_RCV_BUF_LEN-len);
			//	printf("--->CSerialPortSession::run() 333 tmpLen:%d,len:%d \n", tmpLen, len);
			//	printf("--->CSerialPortSession::run() tmpLen:%d,len:%d \n", tmpLen, len);
				if (tmpLen > 0)
				{
					len += tmpLen;
				}
			} while (tmpLen > 0);
			
			if (m_observer)
			{
				printf("--->read datalen:%d %s\n", len, pBuf);
				m_observer->OnInput(pBuf, len);
			}
		}
		else
		{
			Thread::sleep(100);
		}
	}
}

