
#ifndef C_SERIALPORT_SERSSION_H
#define C_SERIALPORT_SERSSION_H

#include "AsynSessionObserver.h"
#include "Runnable.h"
#include "Types.h"
#include "Thread.h"
#include "IOTModuleCommon.h"

#if defined(CORE_OS_FAMILY_WINDOWS)
#include "OS/SerialPort_WIN32.h"
#else
#include "OS/SerialPort_POSIX.h"
#endif

using CORE::Runnable;
using CORE::Thread;
using CORE::CSerialPort;

//#define MAX_SERIAL_RCV_BUF_LEN 1024

class  CSerialPortSession: public Runnable
{
public:
	CSerialPortSession();

	virtual ~CSerialPortSession();

public:

	virtual int sessionOutput(const char* data, int len);

	virtual void setObserver(CAsynSessionObserver* observer);

	void setSessionParam(unsigned int speed, int databits = 8,  int stopbits = 1,  int parity = 0);

	virtual void run();

	int init(CData serialPortName, int speed);

	int unInit();

private:
	Thread m_thread;
	CSerialPort m_serialPort;

	CAsynSessionObserver *m_observer;
};


#endif

