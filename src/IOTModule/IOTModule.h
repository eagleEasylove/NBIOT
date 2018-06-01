

#ifndef _IOT_MODULE_H_
#define _IOT_MODULE_H_

#include "../Include/dllexport.h"
#include "NBIOT.h"
#include "IOTModuleCommon.h"

extern "C" DLLEXPORT_API int NBIOT_Init(const char *pComName, int iComSpeed, int iNbiotMode);
extern "C" DLLEXPORT_API int NBIOT_RegisterOnReceiveCallback(OnReceiveCallback cbFun);

//only used in udp mode
extern "C" DLLEXPORT_API int NBIOT_SetUdpServerInfo(const char *pServerIp, int iServerPort);

extern "C" DLLEXPORT_API int NBIOT_ReportInfo(const char *pReportInfo);
extern "C" DLLEXPORT_API int NBIOT_UnInit();
//ok:return 0;
//status: see Query Status list
extern "C" DLLEXPORT_API int NBIOT_QueryStatus(int *pStatus);

//just for test
extern "C" DLLEXPORT_API int NBIOT_ReportInfoExt(int objId, int instanceId, int resourceId, const char *pReportInfo);
extern "C" DLLEXPORT_API int NBIOT_ReportInfoUdp(int socketId, const char *pReportInfo);
extern "C" DLLEXPORT_API int NBIOT_SendAtCmd(const char *pAtCmd, char *pAtCmdRsp, int iAtCmdRspLen);

class CIOTModule : public Runnable
{
public:
	CIOTModule();
	virtual ~CIOTModule();
	static CIOTModule& GetSingleInstance();

	virtual int InitModule(const char *pSerialName = "COM5", int iComSpeed=9600, int mode = 1); //mode: 0,onenet; 1,udp; 
	virtual int UnInitModule();

	int ReportInfo(const char *pReportInfo);
	int ReportInfoExt(int objId, int instanceId, int resourceId, const char *pReportInfo);
	int ReportInfoUdp(int socketId, const char *pReportInfo);

	int SetUdpServerInfo(CData serverIp, int serverPort);

	int SetCbFun(OnReceiveCallback cbFun);
	int SendAtCmd(const char *pAtCmd, char *pAtCmdRsp, int iAtCmdRspLen);
	int QueryStatus(int &status);

	virtual void run();

private:
	CNBIOT *m_pNBIOT;

	Thread m_thread;
//	FastMutex m_mutex;
//	Event     m_event;
};

extern "C" DLLEXPORT_API CIOTModule &NBIOT_Module();



#endif 

