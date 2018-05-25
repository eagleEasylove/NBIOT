
#ifndef _C_SERIALPORT_WIN32_H
#define _C_SERIALPORT_WIN32_H

namespace CORE{

class CSerialPort
{
public:
	CSerialPort();
	virtual ~CSerialPort();

public:
	int openCom(const char *pSerialPortName);
	int closeCom();
	int ioCtrl(unsigned int speed, int databits = 8,  int stopbits = 1,  int parity = 0);

	int writeCom(const char* data, Int32 len);
	int readCom(char *pBuf, int ilen);

//	int setObserver(AsynSessionObserver* observer);

private:	
#if 0
	void setParam(int databits = 8, int stopbits = 1, int parity = 0);
	void setSpeed(unsigned int speed);
#endif
private:
	HANDLE m_hHandle;//句柄，用于初始化串口  
	
};

}

#endif

