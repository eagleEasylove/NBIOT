
#ifndef _SERIALPORT_POSIX_H
#define _SERIALPORT_POSIX_H

#include "../AsynSessionObserver.h"
namespace CORE{

class CORE_API CSerialPort
{
public:
	CSerialPort();
	virtual ~CSerialPort();

public:
	int openCom(const char *strSerialPortName);
	int closeCom();
	int ioCtrl(unsigned int speed, int databits = 8,  int stopbits = 1,  int parity = 0);

	int writeCom(const char* data, Int32 len);
	int readCom(char *pBuf, int ilen );
	int setObserver(CAsynSessionObserver* observer);

private:	
	void setParam(int databits = 8, int stopbits = 1, int parity = 0);
	void setSpeed(unsigned int speed);

private:
	int m_iHandle;
	
};

}

#endif

