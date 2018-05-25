
#include <stdio.h>      /*标准输入输出定义*/
#include <stdlib.h>     /*标准函数库定义*/
#include <unistd.h>     /*Unix 标准函数定义*/

#include <sys/types.h> 
#include <sys/stat.h>  
#include <fcntl.h>      /*文件控制定义*/
#include <termios.h>    /*PPSIX 终端控制定义*/
#include <errno.h>      /*错误号定义*/

#include <pthread.h>
#include <string.h>

#include <sys/time.h>


#include "SerialPort_POSIX.h"
#include "Thread_POSIX.h"

namespace CORE {

CSerialPort::CSerialPort()
{
	m_iHandle = -1;
}

CSerialPort::~CSerialPort()
{

}

int CSerialPort::openCom(const char *pSerialPortName)
{
	if(m_iHandle == -1)
	{
		m_iHandle = open(pSerialPortName, O_RDWR | O_NOCTTY);
		if(m_iHandle<0)
		{
	//		_LogError<<"SerialPort error"<<_LogErrorEnd;
			return -1;
		}
		setSpeed(9600);
		setParam();

	}
	return 0;
}

int CSerialPort::closeCom()
{
	if(m_iHandle  != -1)
	{
		int ret = close(m_iHandle);
		if(ret<0)
		{
	//		_LogError<<"SerialPort Close error"<<_LogErrorEnd;
			return -1;
		}
		m_iHandle = -1;
	}
	return 0;
}

int CSerialPort::ioCtrl(unsigned int speed, int databits,  int stopbits,  int parity)
{
	if(m_iHandle==-1)
	{
		return -1 ;
	}
	setSpeed(speed);
	setParam(databits, stopbits, parity);
	return 0;
}

int CSerialPort::writeCom(const char* data, Int32 len)
{
	if(m_iHandle==-1)
	{
		return -1 ;
	}
	int rt_size = 0;
	rt_size = write(m_iHandle,data,len);
	if(rt_size <= 0)
	{
	//	_LogError << "sessionOutput write error:" << errno << " " << strerror(errno) << _LogErrorEnd;
		return -1;
	}
	return rt_size;
}

int CSerialPort::readCom(char *pBuf, int ilen )
{
	if(m_iHandle != -1)
	{
		memset(pBuf,0x00,ilen);
		int len = read(m_iHandle, pBuf, ilen);
		if(len>=0)
		{
			return len;
		}
	}

	return -1;
}

void CSerialPort::setSpeed(unsigned int speed)
{
	if(m_iHandle==-1)
	{
		return;
	}
	int fd;
	fd=m_iHandle;
	
	int speed_arr[] = {B50, B75, B110, B150, B300, B600, B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400};
	unsigned int name_arr[]  = {50, 75, 110, 150, 300, 600, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400 };

	
	int   i;
	//int   status;
	struct termios option;
	tcgetattr(fd,&option);
//	cfmakeraw(&option);

	option.c_iflag &=~(INLCR|IGNCR|ICRNL);
	option.c_oflag &=~(ONLCR|OCRNL);
	
	for ( i= 0;  i < sizeof(speed_arr) /sizeof(int);  i++) 
	{
		if(speed == name_arr[i]) 
		{
			cfsetispeed(&option, speed_arr[i]); 
			cfsetospeed(&option, speed_arr[i]);  
			
			tcflush(m_iHandle, TCIFLUSH);
			tcsetattr(fd,TCSANOW,&option);
		} 
	}
}


void CSerialPort::setParam(int databits, int stopbits, int parity)
{
	if(m_iHandle==-1)
	{
		return;
	}
	
	int fd;
	fd=m_iHandle;
	struct termios options;
	if ( tcgetattr( fd,&options) != 0) 
	{
		return;
	}

	options.c_cflag &= ~CSIZE;
	switch (databits) /*设置数据位数*/
	{
		case 7:
		{
			options.c_cflag |= CS7;
			break;
		}
		case 8:
		{
			options.c_cflag |= CS8;
			break;
		}
		default:
		{
			options.c_cflag |= CS8;
		}
	}

	switch (parity)
	{  
		case 0:
		{
			options.c_cflag &= ~PARENB;   /* Clear parity enable */
			options.c_iflag &= ~INPCK;     /* Enable parity checking */
			break; 
		}
		case 1:  
		{
			options.c_cflag |= (PARODD | PARENB); /* 设置为奇效验*/ 
			options.c_iflag |= INPCK;             /* Disnable parity checking */
			break; 
		}
		case 2: 
		{
			options.c_cflag |= PARENB;     /* Enable parity */   
			options.c_cflag &= ~PARODD;   /* 转换为偶效验*/    
			options.c_iflag |= INPCK;       /* Disnable parity checking */
			break;
		}
		default:  
		{
			options.c_cflag &= ~PARENB;   /* Clear parity enable */
			options.c_iflag &= ~INPCK;     /* Enable parity checking */
		}

	}
	
	/* 设置停止位*/ 
	switch (stopbits)
	{  
		case 1:
		{
			options.c_cflag &= ~CSTOPB; 
			break; 
		}
		case 2:
		{
			options.c_cflag |= CSTOPB; 
			break;
		}
		default:
		{
			options.c_cflag &= ~CSTOPB; 
		}
	}

	options.c_cflag &= ~CRTSCTS;
	options.c_cc[VTIME] = 1; 
	options.c_cc[VMIN] = 0;

	options.c_lflag = 0;
	options.c_oflag = 0;

	options.c_iflag &= ~(IXON | IXOFF | IXANY);	

	options.c_iflag &=~(INLCR|IGNCR|ICRNL);
	options.c_oflag &=~(ONLCR|OCRNL);

	tcflush(m_iHandle, TCIFLUSH);
	tcsetattr(m_iHandle,TCSANOW,&options);
}


}

