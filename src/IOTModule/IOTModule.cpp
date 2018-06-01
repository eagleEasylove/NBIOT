//
#include "IOTModule.h"
#include "./SingletonHolder.h"
#include "IOTModuleCommon.h"
#include "NBIOTOnenet.h"
#include "NBIOTUdp.h"

CIOTModule &NBIOT_Module()
{
	return CIOTModule::GetSingleInstance();
} 

int NBIOT_Init(const char *pComName, int iComSpeed, int iNbiotMode)
{
	return CIOTModule::GetSingleInstance().InitModule(pComName, iComSpeed, NBIOT_MODE_UDP);
}

int NBIOT_ReportInfo(const char *pReportInfo)
{
	return CIOTModule::GetSingleInstance().ReportInfo(pReportInfo);
}

int NBIOT_SetUdpServerInfo(const char* serverIp, int serverPort)
{
	return CIOTModule::GetSingleInstance().SetUdpServerInfo(serverIp, serverPort);
}

int NBIOT_ReportInfoExt(int objId, int instanceId, int resourceId, const char * pReportInfo)
{
	return CIOTModule::GetSingleInstance().ReportInfoExt(objId, instanceId, resourceId, pReportInfo);
}

int NBIOT_ReportInfoUdp(int socketId, const char *pReportInfo)
{
	return CIOTModule::GetSingleInstance().ReportInfoUdp(socketId, pReportInfo);
}

int NBIOT_RegisterOnReceiveCallback(OnReceiveCallback cbFun)
{
	return CIOTModule::GetSingleInstance().SetCbFun(cbFun);
}

int NBIOT_UnInit()
{
	return CIOTModule::GetSingleInstance().UnInitModule();
}

int NBIOT_SendAtCmd(const char *pAtCmd, char *pAtCmdRsp, int iAtCmdRspLen)
{
	return CIOTModule::GetSingleInstance().SendAtCmd(pAtCmd, pAtCmdRsp, iAtCmdRspLen);
}

//ok:return 0;   error: return -1
//status: see Query Status list
int NBIOT_QueryStatus(int *pStatus)
{
	int status = 0;
	int iRet = CIOTModule::GetSingleInstance().QueryStatus(status);
	*pStatus = status;
	return iRet;
}

CIOTModule::CIOTModule(void)
{
	m_pNBIOT = NULL;
}
CIOTModule::~CIOTModule(void)
{
	if (m_pNBIOT)
	{
		delete m_pNBIOT;
	}
	m_pNBIOT = NULL;
}

int CIOTModule::InitModule(const char *pSerialName, int iComSpeed, int mode)
{//not thread safe
	if (!m_pNBIOT)
	{
		if (mode == NBIOT_MODE_UDP)
		{//1:udp mode
			m_pNBIOT = new CNBIOTUdp;
		}
		else
		{//0:onenet mode
			m_pNBIOT = new COnenetNBIOT;
		}
	}
	
	int iRet = m_pNBIOT->init(pSerialName, iComSpeed);
	m_thread.start(*this);
	return iRet;
}
int CIOTModule::UnInitModule()
{
	return m_pNBIOT->unInit();
}

int CIOTModule::ReportInfo(const char *pReportInfo)
{
	return m_pNBIOT->reportInfo(pReportInfo);
}

int CIOTModule::ReportInfoExt(int objId, int instanceId, int resourceId, const char *pReportInfo)
{
	return m_pNBIOT->reportInfoExt(objId, instanceId, resourceId, pReportInfo);
}
int CIOTModule::ReportInfoUdp(int socketId, const char *pReportInfo)
{
	return m_pNBIOT->reportInfoUdp(socketId, pReportInfo);
}
int CIOTModule::SetCbFun(OnReceiveCallback cbFun)
{
	return m_pNBIOT->SetCbFun(cbFun);;
}

int CIOTModule::SetUdpServerInfo(CData serverIp, int serverPort)
{
	return m_pNBIOT->SetUdpServerInfo(serverIp, serverPort);
}

int CIOTModule::SendAtCmd(const char *pAtCmd, char *pAtCmdRsp, int iAtCmdRspLen)
{
	return m_pNBIOT->SendAtCmd(pAtCmd, pAtCmdRsp, iAtCmdRspLen);
}
int CIOTModule::QueryStatus(int &status)
{
	return m_pNBIOT->QueryStatus(status);
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
		m_pNBIOT->handle_time(1);
		Thread::sleep(1000);
	}
	return;
}
