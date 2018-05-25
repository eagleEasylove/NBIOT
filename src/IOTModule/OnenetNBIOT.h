
#ifndef C_ONENET_NBIOT_H
#define C_ONENET_NBIOT_H


#include "./AsynSessionObserver.h"
#include "./SerialPortSession.h"
#include "./Data.h"
#include "./Mutex.h"
#include "./Event.h"

#include <list>
#include <map>
#include <vector>
using namespace CORE;
using namespace std;

//cmd: 1 read;  2: write;   3:execute;  4:....
typedef int(*OnReceiveCallback)(int cbCmd, const char *pCallbackPara, char *pBuf, int iBufLen);

#define MAX_OBJECT_ID_NUM   10
#define MAX_INSTANCE_ID_NUM 10
#define MAX_RESOURE_ID_NUM  10

#define CIS_EVENT_BASE 0x00
#define CIS_EVENT_BOOTSTRAP_START CIS_EVENT_BASE + 1
#define CIS_EVENT_BOOTSTRAP_SUCCESS CIS_EVENT_BASE + 2
#define CIS_EVENT_BOOTSTRAP_FAILED CIS_EVENT_BASE + 3
#define CIS_EVENT_CONNECT_SUCCESS CIS_EVENT_BASE + 4
#define CIS_EVENT_CONNECT_FAILED CIS_EVENT_BASE + 5
#define CIS_EVENT_REG_SUCCESS CIS_EVENT_BASE + 6
#define CIS_EVENT_REG_FAILED CIS_EVENT_BASE + 7
#define CIS_EVENT_REG_TIMEOUT CIS_EVENT_BASE + 8
#define CIS_EVENT_LIFETIME_TIMEOUT CIS_EVENT_BASE + 9
#define CIS_EVENT_STATUS_HALT CIS_EVENT_BASE + 10
#define CIS_EVENT_UPDATE_SUCCESS CIS_EVENT_BASE + 11
#define CIS_EVENT_UPDATE_FAILED CIS_EVENT_BASE + 12
#define CIS_EVENT_UPDATE_TIMEOUT CIS_EVENT_BASE + 13
#define CIS_EVENT_UPDATE_NEED CIS_EVENT_BASE + 14
#define CIS_EVENT_LOGON_SUCCESS CIS_EVENT_BASE + 15//×¢Ïú³É¹¦
#define CIS_EVENT_RESPONSE_FAILED CIS_EVENT_BASE + 20
#define CIS_EVENT_RESPONSE_SUCCESS CIS_EVENT_BASE + 21
#define CIS_EVENT_NOTIFY_FAILED CIS_EVENT_BASE + 25
#define CIS_EVENT_NOTIFY_SUCCESS CIS_EVENT_BASE + 26

#ifndef NBIOT_PREPARE_SUCCESS
#define NBIOT_COM_UNINIT      -1
#define NBIOT_COM_OPEN_SUCCESS 0

#define NBIOT_OBSERVE_SUCCESS  96
#define NBIOT_OBSERVE_FAILED   97
#define NBIOT_DISCOVER_SUCCESS 98
#define NBIOT_DISCOVER_FAILED  99

#define NBIOT_PREPARE_SUCCESS 100
#endif 

typedef struct
{
	int objectId;
	int instanceId;
	int resourceId;
	int msgId;
}T_LWM2MResourceModule;

class  CLierdaOnenetATParser
{
public:

	CLierdaOnenetATParser();

	virtual ~CLierdaOnenetATParser();

public:
	int initModule(CData &atCmd);
	int doInitModuleRsp(CData atCmdRsp);
	int unInitModule(CData &atCmd);
	int doUninitModuleRsp(CData atCmdRsp);
	int doModuleRsp(CData atCmdRsp);

	int openModule(CData &atCmd);
	int doOpenModuleRsp(CData atCmdRsp);
	int closeModule(CData &atCmd);
	int doCloseModuleRsp(CData atCmdRsp);

	int addObj(CData &atCmd);
	int doAddObjRsp(CData atCmdRsp);
	int delObj(CData &atCmd);
	int doDelObjRsp(CData atCmdRsp);

	int notify(CData &atCmd, T_LWM2MResourceModule resModule, CData notifyInfo, int notifyIndex=0, int notifyFlag=0);
	int doNotifyRsp(CData atCmdRsp);

	int update(CData &atCmd, int hasParam = 0);
	int doUpdateRsp(CData atCmdRsp);

	int ver(CData &atCmd);
	int doVerRsp(CData atCmdRsp, CData &ver);

	int doObserver(CData atCmd, T_LWM2MResourceModule &resModule);
	int observerRsp(CData &atCmd, T_LWM2MResourceModule resModule);
	int doObserverRsp(CData atCmdRsp);

	int doDiscover(CData atCmd, T_LWM2MResourceModule &resModule);
	int discoverRsp(CData &atCmd, T_LWM2MResourceModule resModule);
	int doDiscoverRsp(CData atCmdRsp);

	int doRead(CData atCmd, T_LWM2MResourceModule &resModule);
	int readRsp(CData &atCmd, T_LWM2MResourceModule resModule, CData cmdInfo);
	int doReadRsp(CData atCmdRsp);

	int doWrite(CData atCmd, T_LWM2MResourceModule &resModule, CData &writeInfo);
	int writeRsp(CData &atCmd, T_LWM2MResourceModule resModule, int result);
	int doWriteRsp(CData atCmdRsp);

	int doParameter(CData atCmd, T_LWM2MResourceModule &resModule);
	int parameterRsp(CData &atCmd, T_LWM2MResourceModule resModule);
	int doParameterRsp(CData atCmdRsp);

	int doExecute(CData atCmd, T_LWM2MResourceModule &resModule);
	int executeRsp(CData &atCmd, T_LWM2MResourceModule resModule);
	int doExecuteRsp(CData atCmdRsp);

	int doEvent(CData atCmd);
private:
	int setMsgId(int msgId, bool flag, T_LWM2MResourceModule resModule);
	int getMsgId(int &msgId, bool &flag, T_LWM2MResourceModule resModule);

private:
	unsigned int m_uiNBModRef;
//	unsigned int m_uiMsgId;

	typedef struct
	{
		bool observeFlag;
		int resourceId;
		int msgId;
	}T_ResourceMsgId;
	typedef struct
	{
		bool isAll;
		bool observeFlag;
		int  instanceId;
		int  msgId;
		std::map<int, T_ResourceMsgId> resourceMsgId;
	}T_instanceMsgId;
	typedef struct 
	{
		bool isAll;
		bool observeFlag;
		int  objectId;
		int  msgId;
		T_instanceMsgId instanceMsgId[MAX_INSTANCE_ID_NUM];
	}T_ObjectMsgId;

	std::map<int, T_ObjectMsgId> m_msgidMap;
};

class  COnenetNBIOT : public CAsynSessionObserver, public Runnable
{
public:
	COnenetNBIOT();
	virtual ~COnenetNBIOT();

public:
	int init(const char *pSerialName, int iComSpeed);
	int unInit();

	int initNBIOTModule();
	int unInitNBIOTModule();

	virtual void OnInput(char *data, int len);
	int reportInfo(int objId, int instanceId, int resourceId, CData reportInfo);
	int SetCbFun(OnReceiveCallback cbFun);

	virtual void run();
	virtual void handle_time(int time = 0);
	int heartBeat();

	int RebootNBIOTModule();

	int SendAtCmd(const char *pAtCmd, char *pAtCmdRsp, int iAtCmdRspLen);
	int QueryStatus(int &status);

private:
	int m_iLifeTime;
	int m_iRegTime;


private:
	int initSerialPort(CData serialPortName, int speed);
	void doDispatch(CData atCmd);
	bool waitTimeout(int ms );

	int sendCmdNoWaitRsp(CData atCmd);
	int sendCmdSynWaitRsp(CData atCmd, CData &atCmdRsp, int timeout);

private:

	int m_iRegStatus; //-1: error; 6:success 
	int m_iNeedReset;

	CSerialPortSession m_serialPort;
	CLierdaOnenetATParser m_OnenetATParser;

	OnReceiveCallback m_CbFun;
	char *m_pRspBuf;
	int m_iRspBufLen;

	std::list<CData> m_atCmdList;

	Thread m_thread;
	FastMutex m_mutex;
	Event     m_event;

	Event     m_listEvent;
};

#endif

