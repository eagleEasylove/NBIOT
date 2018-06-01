
#ifndef C_NBIOT_H
#define C_NBIOT_H
#include "../Include/IOTModuleOut.h"
#include "./AsynSessionObserver.h"
#include "./SerialPortSession.h"
#include "./Data.h"
#include "./Mutex.h"
#include "./Event.h"
#include "IOTModuleCommon.h"

#include <list>
#include <map>
#include <vector>
using namespace CORE;
using namespace std;

class CNBIOT : public CAsynSessionObserver, public Runnable
{
public:
	CNBIOT();
	virtual ~CNBIOT();

public:
	int init(const char *pSerialName, int iComSpeed);
	virtual int initNBIOTModule() = 0;//

	int unInit();
	virtual int unInitNBIOTModule() = 0;//

	virtual int reportInfo(CData reportInfo) = 0;//
	virtual int reportInfoExt(int objId, int instanceId, int resourceId, CData strReportInfo)
	{
		return -1;
	};//
	virtual int reportInfoUdp(int scoketId, CData strReportInfo)
	{
		return -1;
	};//

	virtual int SetUdpServerInfo(CData serverIp, int serverPort)
	{
		return -1;
	};//

	int SetCbFun(OnReceiveCallback cbFun);

//	virtual void OnInput(char *data, int len) = 0; //CAsynSessionObserver

	virtual void run(); 
	virtual void doDispatch(CData atCmd) = 0;//

//	virtual void handle_time(int time = 0) = 0;    //CAsynSessionObserver

	int SendAtCmd(const char *pAtCmd, char *pAtCmdRsp, int iAtCmdRspLen);
	int QueryStatus(int &status);

protected:
	int initSerialPort(CData serialPortName, int speed);

	int RebootNBIOTModule();

protected:
	int sendCmdNoWaitRsp(CData atCmd);
	int sendCmdSynWaitRsp(CData atCmd, CData &atCmdRsp, int timeout);

private:
	bool waitTimeout(int ms);

protected:
	int m_iRegStatus; //-1: error; 
	int m_iNeedReset;

	CSerialPortSession m_serialPort;

	OnReceiveCallback m_CbFun;
	char *m_pRspBuf;
	int m_iRspBufLen;

	std::list<CData> m_atCmdList;


	Thread m_thread;
	Event  m_event;

	FastMutex m_syncWaitMutex;

	FastMutex m_listMutex;
	Event     m_listEvent;
};

#endif

