

#ifndef _IOT_MODULE_H_
#define _IOT_MODULE_H_

#include "../Include/dllexport.h"

#include "OnenetNBIOT.h"

//dll export 
extern "C" DLLEXPORT_API int OneNetNBIOT_Init(const char *pComName, int iComSpeed);
extern "C" DLLEXPORT_API int OneNetNBIOT_RegisterOnReceiveCallback(OnReceiveCallback cbFun);
extern "C" DLLEXPORT_API int OneNetNBIOT_ReportInfo(char *pReportInfo);
extern "C" DLLEXPORT_API int OneNetNBIOT_UnInit();

//ok:return 0;
//status: see Query Status list
extern "C" DLLEXPORT_API int OneNetNBIOT_QueryStatus(int *pStatus);

extern "C" DLLEXPORT_API int OneNetNBIOT_ReportInfoExt(int objId, int instanceId, int resourceId, void *pReportInfo);
extern "C" DLLEXPORT_API int OneNetNBIOT_SendAtCmd(const char *pAtCmd, char *pAtCmdRsp, int iAtCmdRspLen);

class CIOTModule : public Runnable
{
public:
	CIOTModule();
	virtual ~CIOTModule();
	static CIOTModule& GetSingleInstance();

	virtual int InitModule(const char *pSerialName = "COM5", int iComSpeed=9600);
	virtual int UnInitModule();

	int ReportInfo(int objId, int instanceId, int resourceId, void *pReportInfo);
	int SetCbFun(OnReceiveCallback cbFun);
	int SendAtCmd(const char *pAtCmd, char *pAtCmdRsp, int iAtCmdRspLen);
	int QueryStatus(int &status);

	virtual void run();

private:
	COnenetNBIOT m_ConenetNBIOT;

	Thread m_thread;
//	FastMutex m_mutex;
//	Event     m_event;
};

extern "C" DLLEXPORT_API CIOTModule &OneNetNBIOT_Module();



#endif 

