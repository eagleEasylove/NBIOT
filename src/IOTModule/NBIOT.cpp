
#include "NBIOT.h"

CNBIOT::CNBIOT()
{
	m_iRegStatus = NBIOT_COM_UNINIT;
	m_iNeedReset = 0;

	m_CbFun = 0;

	m_pRspBuf = new char[MAX_SERIAL_RCV_BUF_LEN];
	m_iRspBufLen = 0;
}

CNBIOT::~CNBIOT()
{
	if (m_pRspBuf)
	{
		delete[] m_pRspBuf;
		m_pRspBuf = NULL;
	}
}

int CNBIOT::init(const char *pSerialName, int iComSpeed)
{
	//just for test,
	int iRet = initSerialPort(pSerialName, iComSpeed);
	if (iRet == -1)
	{//error
		printf("--->COnenetNBIOT::init() initSerialPort() Error! \n");
		m_iRegStatus = NBIOT_COM_UNINIT;//com error
	}
	else
	{
		m_iRegStatus = NBIOT_COM_OPEN_SUCCESS;//com ok

		iRet = initNBIOTModule();
		if (iRet == -1)
		{//error
			printf("--->COnenetNBIOT::init() initNBIOTModule() Error! \n");
			m_iNeedReset = 1;
		}
	}

	m_thread.start(*this);

	return iRet;
}
/*
int CNBIOT::initNBIOTModule()
{
	return -1;
}*/
int CNBIOT::unInit()
{
	unInitNBIOTModule();
	m_iRegStatus = NBIOT_COM_OPEN_SUCCESS;

	m_serialPort.unInit();
	m_iRegStatus = NBIOT_COM_UNINIT;

	return 0;
}

/*
int CNBIOT::unInitNBIOTModule()
{
	return -1;
}*/

/*
int CNBIOT::reportInfo(CData reportInfo)
{
	int iRet = 0;

	return iRet;
}
*/

int CNBIOT::SetCbFun(OnReceiveCallback cbFun)
{
	m_CbFun = cbFun;
	return 0;
}

/*
void CNBIOT::OnInput(char *data, int len)
{
	//parse AT cmd or rsp
	if (data && (len > 0))
	{

	}
	return;
}
*/

void CNBIOT::run()
{
	while (1)
	{
		while (!m_atCmdList.empty())
		{
			CData atInfo;
			{
				FastMutex::ScopedLock lock(m_listMutex);
				atInfo = *m_atCmdList.begin();
				m_atCmdList.pop_front();
			}
			//	printf("--->CNBIOT::run():m_atCmdList size:%d, len:%d, data:%s\n", m_atCmdList.size(), atInfo.length(), atInfo.c_str());
			doDispatch(atInfo);
		}

		if (m_atCmdList.empty())
		{
			m_listEvent.tryWait(1 * 1000);
		}
	}
	return;
}

/*
void CNBIOT::doDispatch(CData atCmd)
{

}
*/ 

/*
void CNBIOT::handle_time(int time)
{

}
*/

int CNBIOT::SendAtCmd(const char *pAtCmd, char *pAtCmdRsp, int iAtCmdRspLen)
{
	int iRet = -1;
	CData atCmdRsp;
	CData atCmd = CData(pAtCmd) + STR_CRLF;
	if (0 == sendCmdSynWaitRsp(atCmd, atCmdRsp, SERIALPORT_RCV_TIMEOUT))
	{
		if (pAtCmdRsp && iAtCmdRspLen > 0)
		{
			memcpy(pAtCmdRsp, atCmdRsp.c_str(), atCmdRsp.length() < iAtCmdRspLen ? atCmdRsp.length() : iAtCmdRspLen - 1);
		}
		iRet = 0;
	}
	else
	{
		printf("--->CNBIOT::reportInfo() sendCmdSynWaitRsp() Error! \n");
		iRet = -1;
	}
	return iRet;
}

int CNBIOT::QueryStatus(int &status)
{
	int iRet = -1;

	if (m_iRegStatus == NBIOT_PREPARE_SUCCESS)
	{
		iRet = 0;
	}
	status = m_iRegStatus;

	return iRet;
}

int CNBIOT::initSerialPort(CData serialPortName, int speed)
{
	int iRet = m_serialPort.init(serialPortName, speed);
	m_serialPort.setObserver(this);
	return iRet;
}


int CNBIOT::RebootNBIOTModule()
{//REBOOT_CAUSE_APPLICATION_AT
	int iRet = 0;

	CData atCmd = CData("AT+NRB") + STR_CR;
	sendCmdNoWaitRsp(atCmd);

	Thread::sleep(10 * 1000);//just for test

	return iRet;
}

int CNBIOT::sendCmdNoWaitRsp(CData atCmd)
{
	return m_serialPort.sessionOutput(atCmd.c_str(), atCmd.length());
}

int CNBIOT::sendCmdSynWaitRsp(CData atCmd, CData &atCmdRsp, int timeout)
{
	FastMutex::ScopedLock lock(m_syncWaitMutex);

	Int32 ret = m_serialPort.sessionOutput(atCmd.c_str(), atCmd.length());
	bool bFlag = waitTimeout(timeout*1000);
	if(true == bFlag)
	{
		ret = -1;
		return ret;
	}
	else
	{
		if (m_iRspBufLen > 0)
		{
			atCmdRsp = CData(m_pRspBuf, m_iRspBufLen);
		}
		else
		{
			ret = -1;
			return ret;
		}
		m_pRspBuf[0] = '\0';
		m_iRspBufLen = 0;
	}

	return 0;
}

bool CNBIOT::waitTimeout( Int32 ms )
{
	m_event.reset();
	return !m_event.tryWait(ms);
}

