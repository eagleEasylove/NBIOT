
#ifndef C_ONENET_NBIOT_H
#define C_ONENET_NBIOT_H

#include "NBIOT.h"
#include "IOTModuleCommon.h"

#define MAX_OBJECT_ID_NUM   10
#define MAX_INSTANCE_ID_NUM 10
#define MAX_RESOURE_ID_NUM  10

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

class  COnenetNBIOT : public CNBIOT
{
public:
	COnenetNBIOT();
	virtual ~COnenetNBIOT();

public:
	virtual int initNBIOTModule();//
	virtual int unInitNBIOTModule();//

	virtual int reportInfo(CData strReportInfo);//

	virtual void OnInput(char *data, int len);//
	virtual void doDispatch(CData atCmd);//

	virtual void handle_time(int time = 0);//

private:
	int reportInfo(int objId, int instanceId, int resourceId, CData reportInfo);

	int heartBeat();

private:
	int m_iLifeTime;
	int m_iRegTime;

private:
	CLierdaOnenetATParser m_ATParser;
};

#endif

