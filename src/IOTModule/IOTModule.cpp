//
#include "IOTModule.h"
#include "./SingletonHolder.h"
CIOTModule &OneNetNBIOT_Module()
{
	return CIOTModule::GetSingleInstance();
} 

int OneNetNBIOT_Init(const char *pComName, int iComSpeed)
{
	return CIOTModule::GetSingleInstance().InitModule(pComName, iComSpeed);
}
int OneNetNBIOT_ReportInfo(char *pReportInfo)
{
	return CIOTModule::GetSingleInstance().ReportInfo(3200, 0, 5750, pReportInfo);
}
int OneNetNBIOT_ReportInfoExt(int objId, int instanceId, int resourceId, void *pReportInfo)
{
	return CIOTModule::GetSingleInstance().ReportInfo(objId, instanceId, resourceId, pReportInfo);
}
int OneNetNBIOT_RegisterOnReceiveCallback(OnReceiveCallback cbFun)
{
	return CIOTModule::GetSingleInstance().SetCbFun(cbFun);
}
int OneNetNBIOT_UnInit()
{
	return CIOTModule::GetSingleInstance().UnInitModule();
}

int OneNetNBIOT_SendAtCmd(const char *pAtCmd, char *pAtCmdRsp, int iAtCmdRspLen)
{
	return CIOTModule::GetSingleInstance().SendAtCmd(pAtCmd, pAtCmdRsp, iAtCmdRspLen);
}

//ok:return 0;   error: return -1
//status: see Query Status list
int OneNetNBIOT_QueryStatus(int *pStatus)
{
	int status = 0;
	int iRet = CIOTModule::GetSingleInstance().QueryStatus(status);
	*pStatus = status;
	return iRet;
}

CIOTModule::CIOTModule(void)
{

}
CIOTModule::~CIOTModule(void)
{

}

int CIOTModule::InitModule(const char *pSerialName, int iComSpeed)
{
	int iRet = m_ConenetNBIOT.init(pSerialName, iComSpeed);
	m_thread.start(*this);
	return iRet;
}
int CIOTModule::UnInitModule()
{
	return m_ConenetNBIOT.unInit();
}
int CIOTModule::ReportInfo(int objId, int instanceId, int resourceId, void *pReportInfo)
{
	return m_ConenetNBIOT.reportInfo(objId, instanceId, resourceId, CData((char*)pReportInfo));
}
int CIOTModule::SetCbFun(OnReceiveCallback cbFun)
{
	return m_ConenetNBIOT.SetCbFun(cbFun);;
}

int CIOTModule::SendAtCmd(const char *pAtCmd, char *pAtCmdRsp, int iAtCmdRspLen)
{
	return m_ConenetNBIOT.SendAtCmd(pAtCmd, pAtCmdRsp, iAtCmdRspLen);
}
int CIOTModule::QueryStatus(int &status)
{
	return m_ConenetNBIOT.QueryStatus(status);
}

CIOTModule& CIOTModule::GetSingleInstance()
{
	static SingletonHolder<CIOTModule> sh;
	return *sh.get();
}

void CIOTModule::run()
{
	while (1)
	{
		m_ConenetNBIOT.handle_time(1);
		Thread::sleep(1000);
	}
	return;
}