
#include "NBIOTOnenet.h"

#define VERSION_LIERDA_180426A 1
#ifdef VERSION_LIERDA_180426A
#define VERSION_LIERDA  1
#else
#define VERSION_LIERDA  0
#endif

COnenetNBIOT::COnenetNBIOT()
{
	m_iLifeTime = 0;
	m_iRegTime = 0;
}

COnenetNBIOT::~COnenetNBIOT()
{

}

int COnenetNBIOT::initNBIOTModule()
{
	int iRet = -1;
	CData atCmd;
	CData atCmdRsp;
	m_ATParser.ver(atCmd);
	if (0 == sendCmdSynWaitRsp(atCmd, atCmdRsp, SERIALPORT_RCV_TIMEOUT))
	{
		CData version;
		if (0 != m_ATParser.doVerRsp(atCmdRsp, version))
		{//error
			printf("--->COnenetNBIOT::initNBIOTModule() doVerRsp() Error! \n");
			iRet = -1;
			return iRet;
		}
		printf("--->COnenetNBIOT::initNBIOTModule() doVerRsp(): %s\n", version.c_str());
	}
	else
	{
		printf("--->COnenetNBIOT::initNBIOTModule() sendCmdSynWaitRsp() Error! \n");
		iRet = -1;
		return iRet;
	}

	m_ATParser.initModule(atCmd);
	if (0 == sendCmdSynWaitRsp(atCmd, atCmdRsp, SERIALPORT_RCV_TIMEOUT))
	{
		if(0 != m_ATParser.doInitModuleRsp(atCmdRsp))
		{//error
			printf("--->COnenetNBIOT::initNBIOTModule() doInitModuleRsp() Error! \n");
			iRet = -1;
			return iRet;
		}
	}
	else
	{
		printf("--->COnenetNBIOT::initNBIOTModule() sendCmdSynWaitRsp() Error! \n");
	}

	m_ATParser.addObj(atCmd);
	if (0 == sendCmdSynWaitRsp(atCmd, atCmdRsp, SERIALPORT_RCV_TIMEOUT))
	{
		if(0 != m_ATParser.doAddObjRsp(atCmdRsp))
		{//error
			printf("--->COnenetNBIOT::initNBIOTModule() doAddObjRsp() Error! \n");
			iRet = -1;
			return iRet;
		}
	}
	else
	{
		printf("--->COnenetNBIOT::initNBIOTModule() sendCmdSynWaitRsp() Error! \n");
		iRet = -1;
		return iRet;
	}

	m_ATParser.openModule(atCmd);
	if (0 == sendCmdSynWaitRsp(atCmd, atCmdRsp, SERIALPORT_RCV_TIMEOUT))
	{
		if(0 != m_ATParser.doOpenModuleRsp(atCmdRsp))
		{//error
			printf("--->COnenetNBIOT::initNBIOTModule() doOpenModuleRsp() Error! \n");
			iRet = -1;
			return iRet;
		}
	}
	else
	{
		printf("--->COnenetNBIOT::initNBIOTModule() sendCmdSynWaitRsp() Error! \n");
		iRet = -1;
		return iRet;
	}

	return 0; 
}

int COnenetNBIOT::unInitNBIOTModule()
{
	int iRet = -1;
	CData atCmd;
	CData atCmdRsp;
	m_ATParser.closeModule(atCmd);
	if (0 == sendCmdSynWaitRsp(atCmd, atCmdRsp, SERIALPORT_RCV_TIMEOUT))
	{
		if (0 != m_ATParser.doCloseModuleRsp(atCmdRsp))
		{//error
			printf("--->COnenetNBIOT::unInitNBIOTModule() doCloseModuleRsp() Error! \n");
			iRet = -1;
			return iRet;
		}
	}
	else
	{
		printf("--->COnenetNBIOT::unInitNBIOTModule() sendCmdSynWaitRsp() Error! \n");
		iRet = -1;
		return iRet;
	}

	m_ATParser.delObj(atCmd);
	if (0 == sendCmdSynWaitRsp(atCmd, atCmdRsp, SERIALPORT_RCV_TIMEOUT))
	{
		if (0 != m_ATParser.doDelObjRsp(atCmdRsp))
		{//error
			printf("--->COnenetNBIOT::initNBIOTModule() doDelObjRsp() Error! \n");
			iRet = -1;
			return iRet;
		}
	}
	else
	{
		printf("--->COnenetNBIOT::initNBIOTModule() sendCmdSynWaitRsp() Error! \n");
		iRet = -1;
		return iRet;
	}

	m_ATParser.unInitModule(atCmd);
	if (0 == sendCmdSynWaitRsp(atCmd, atCmdRsp, SERIALPORT_RCV_TIMEOUT))
	{
		if (0 != m_ATParser.doUninitModuleRsp(atCmdRsp))
		{//error
			printf("--->COnenetNBIOT::unInitNBIOTModule() doUninitModuleRsp() Error! \n");
			iRet = -1;
			return iRet;
		}
	}
	else
	{
		printf("--->COnenetNBIOT::unInitNBIOTModule() sendCmdSynWaitRsp() Error! \n");
		iRet = -1;
		return iRet;
	}
	return 0;
}

int COnenetNBIOT::reportInfo(CData strReportInfo)
{
	return reportInfo(3200, 0, 5750, strReportInfo);
}

int COnenetNBIOT::reportInfo(int objId, int instanceId, int resourceId, CData reportInfo)
{
	if (m_iRegStatus != NBIOT_PREPARE_SUCCESS)
	{
		printf("--->COnenetNBIOT::reportInfo() ERROR, m_iRegStatus:%d \n", m_iRegStatus);
		return -1;
	}

	int iRet = 0;
	CData atCmd;
	CData atCmdRsp = "0";
	T_LWM2MResourceModule resModule;
	resModule.objectId = objId;
	resModule.instanceId = instanceId;
	resModule.resourceId = resourceId;

	int cnts = reportInfo.length() / MAX_PER_REPORT_LEN + ( (reportInfo.length() % MAX_PER_REPORT_LEN == 0) ? 0 : 1 ) ;
	int cur = 1;
	while (cur <= cnts)
	{
		int startPos = (cur - 1)*MAX_PER_REPORT_LEN;
		int endPos = (cur == cnts) ? reportInfo.length() : (cur*MAX_PER_REPORT_LEN);

		int notifyIndex = 0;
		int notifyFlag = 0;
		if (cnts > 1)
		{
			notifyIndex = cnts - cur;

			if (cur == 1)
			{
				notifyFlag = 1;
			}
			else if (cur != cnts)
			{
				notifyFlag = 2;
			}
			else
			{
				notifyFlag = 0;
			}
		}
		printf("--->COnenetNBIOT::reportInfo() reportInfo() cnts:%d, cur:%d, startPos:%d, endPos:%d, index:%d, flag:%d \n", cnts, cur, startPos, endPos, notifyIndex, notifyFlag);

		m_ATParser.notify(atCmd, resModule, reportInfo.substring(startPos, endPos), notifyIndex, notifyFlag);
		if (0 == sendCmdSynWaitRsp(atCmd, atCmdRsp, SERIALPORT_RCV_TIMEOUT))
		{
			if (0 != m_ATParser.doNotifyRsp(atCmdRsp))
			{//error
				printf("--->COnenetNBIOT::reportInfo() doNotifyRsp() Error! \n");
				iRet = -1;
			}
		}
		else
		{
			printf("--->COnenetNBIOT::reportInfo() sendCmdSynWaitRsp() Error! \n");
			iRet = -1;
		}
		cur++;
	}

	return iRet;
}

void COnenetNBIOT::OnInput(char *data, int len)
{
	//parse AT cmd or rsp
	if (data && (len > 0))
	{
		CData atInfo = CData(data,len);
		//printf("--->COnenetNBIOT::OnInput():len:%d, data:%s\n", len, data);

		if (atInfo.find("REBOOT_CAUSE_") != CDATA_NPOS && (atInfo.find("Lierda") != CDATA_NPOS))
		{//module error 
			printf("--->COnenetNBIOT::OnInput() Module Error, regStatus:%d\n", m_iRegStatus);
			if (atInfo.find("REBOOT_CAUSE_APPLICATION_AT") == CDATA_NPOS)
			{//not atcmd reboot //module error
				m_iNeedReset = 1;//need
				printf("--->COnenetNBIOT::OnInput() need resetModule \n");
			}
			else
			{//REBOOT_CAUSE_APPLICATION_AT
				printf("--->COnenetNBIOT::OnInput() atcmd resetModule \n");
			}
		}
		else
		{//AT  cmd or rsp
			if (atInfo.find("+MIPL") != CDATA_NPOS && atInfo.find("+MIPLCREATE") == CDATA_NPOS  && atInfo.find("+MIPLVER") == CDATA_NPOS)
			{// AT cmd,  not "+MIPLCREATE"
				int startPos = atInfo.find("+MIPL");
				int endPos = atInfo.find(STR_CRLF, startPos) + strlen(STR_CRLF);

				do
				{
					CData atInfoCmd = atInfo.substring(startPos, endPos);

					{
						FastMutex::ScopedLock lock(m_listMutex);
						m_atCmdList.push_back(atInfoCmd);
					}

					startPos = atInfo.find("+MIPL", endPos);
					if (startPos != CDATA_NPOS)
					{
						endPos = atInfo.find(STR_CRLF, startPos) + strlen(STR_CRLF);
						printf("--->COnenetNBIOT::OnInput() atInfo has more than one atcmd\n");
					}

				} while ((startPos != CDATA_NPOS) && (endPos != CDATA_NPOS));

				m_listEvent.set();
			}

			if ((atInfo.find("+CIS ERROR") != CDATA_NPOS || atInfo.find("OK") != CDATA_NPOS))
			{//AT rsp 
				m_iRspBufLen = atInfo.length() > MAX_SERIAL_RCV_BUF_LEN ? MAX_SERIAL_RCV_BUF_LEN : atInfo.length();
				memcpy(m_pRspBuf, atInfo.c_str(), m_iRspBufLen);

				//	Thread::sleep(1000);//just for test
				m_event.set();
			}
		}
	}
	return;
}


void COnenetNBIOT::doDispatch(CData atCmd)
{
	CData atCmdName = atCmd.substring(atCmd.find_first_not_of(STR_CRLF), atCmd.find(":"));
	CData atCmdRsp, atCmdRspRsp;
	T_LWM2MResourceModule resModule;

	if( atCmdName == "+MIPLOBSERVE")
	{
		//input
		m_ATParser.doObserver(atCmd, resModule);
		//rsp
		m_ATParser.observerRsp(atCmdRsp, resModule);
		//send
	//	sendCmdNoWaitRsp(atCmdRsp);//just for test
		if (0 == sendCmdSynWaitRsp(atCmdRsp, atCmdRspRsp, SERIALPORT_RCV_TIMEOUT))
		{
			//rsp rsp
			if (0 != m_ATParser.doObserverRsp(atCmdRspRsp))
			{//error
				printf("--->COnenetNBIOT::doDispatch() +MIPLOBSERVER Rsp Error! \n");
				m_iRegStatus = NBIOT_OBSERVE_FAILED;
			}
			else
			{
				m_iRegStatus = NBIOT_OBSERVE_SUCCESS;
			}
		}
		else
		{
			m_iRegStatus = NBIOT_OBSERVE_FAILED;
			printf("--->COnenetNBIOT::doDispatch() sendCmdSynWaitRsp() Error! \n");
		}
	}

	if( atCmdName == "+MIPLDISCOVER")
	{
		m_ATParser.doDiscover(atCmd, resModule);
		m_ATParser.discoverRsp(atCmdRsp, resModule);

	//	sendCmdNoWaitRsp(atCmdRsp);//just for test
		if (0 == sendCmdSynWaitRsp(atCmdRsp, atCmdRspRsp, SERIALPORT_RCV_TIMEOUT))
		{
			//rsp rsp
			if (0 != m_ATParser.doDiscoverRsp(atCmdRspRsp))
			{//error
				printf("--->COnenetNBIOT::doDispatch() +MIPLDISCOVER Rsp Error! \n");
				m_iRegStatus = NBIOT_DISCOVER_FAILED;
			}
			else
			{
				m_iRegStatus = NBIOT_DISCOVER_SUCCESS;
			}
		}
		else
		{
			m_iRegStatus = NBIOT_DISCOVER_FAILED;
			printf("--->COnenetNBIOT::doDispatch() sendCmdSynWaitRsp() Error! \n");
		}
	}
	if (atCmdName == "+MIPLREAD")
	{
		char buffer[512] = { 0 };//just for test
		int result = -1;
		m_ATParser.doRead(atCmd, resModule);
		if (m_CbFun)
		{
			printf("--->COnenetNBIOT::doDispatch() MIPLREAD() callback start!\n");
			result = m_CbFun(1, NULL, buffer, 512);
			printf("--->COnenetNBIOT::doDispatch() MIPLREAD() callback end!\n");
			if (0 == result)
			{//1  2.05 Content 读取操作正确完成
				result = 1;//ok
			}
			else
			{//		11 4.00 Bad Request
				//		12 4.01 Unauthorized
				//		13 4.04 Not Found
				//		14 4.05 Method Not Allowed
				//		15 4.06 Not Acceptable
				result = 11;//4.00 Bad Request
			}
		}
		CData cmdInfo = buffer;
		m_ATParser.readRsp(atCmdRsp, resModule, cmdInfo);
		//	sendCmdNoWaitRsp(atCmdRsp);//just for test
		if (0 == sendCmdSynWaitRsp(atCmdRsp, atCmdRspRsp, SERIALPORT_RCV_TIMEOUT))
		{
			//rsp rsp
			if (0 != m_ATParser.doReadRsp(atCmdRspRsp))
			{//error
				printf("--->COnenetNBIOT::doDispatch() +MIPLREAD Rsp Error! \n");
			}
		}
		else
		{
			printf("--->COnenetNBIOT::doDispatch() sendCmdSynWaitRsp() Error! \n");
		}
	}
	if (atCmdName == "+MIPLWRITE")
	{
		CData cmdInfo;
		int result = -1;
		m_ATParser.doWrite(atCmd, resModule, cmdInfo);
		if (m_CbFun)
		{
			printf("--->COnenetNBIOT::doDispatch() MIPLWRITE() callback start!\n");
			result = m_CbFun(2, cmdInfo.c_str(),NULL,0);
			printf("--->COnenetNBIOT::doDispatch() MIPLWRITE() callback end!\n");
			if (0 == result)
			{//2 2.04 Changed 写操作正确完成
				result = 2;//ok
			}
			else
			{//		11 4.00 Bad Request
			//		12 4.01 Unauthorized
			//		13 4.04 Not Found
			//		14 4.05 Method Not Allowed
				result = 11;//4.00 Bad Request
			}
		}
	//	m_ATParser.writeRsp(atCmdRsp, resModule);
		m_ATParser.writeRsp(atCmdRsp, resModule, result);
		//	sendCmdNoWaitRsp(atCmdRsp);//just for test
		if (0 == sendCmdSynWaitRsp(atCmdRsp, atCmdRspRsp, SERIALPORT_RCV_TIMEOUT))
		{
			//rsp rsp
			if (0 != m_ATParser.doWriteRsp(atCmdRspRsp))
			{//error
				printf("--->COnenetNBIOT::doDispatch() +MIPLWRITE Rsp Error! \n");
			}
		}
		else
		{
			printf("--->COnenetNBIOT::doDispatch() sendCmdSynWaitRsp() Error! \n");
		}
	}
	if (atCmdName == "+MIPLPARAMETER")
	{
		m_ATParser.doParameter(atCmd, resModule);
		m_ATParser.parameterRsp(atCmdRsp, resModule);
		//	sendCmdNoWaitRsp(atCmdRsp);//just for test
		if (0 == sendCmdSynWaitRsp(atCmdRsp, atCmdRspRsp, SERIALPORT_RCV_TIMEOUT))
		{
			//rsp rsp
			if (0 != m_ATParser.doParameterRsp(atCmdRspRsp))
			{//error
				printf("--->COnenetNBIOT::doDispatch() +MIPLPARAMETER Rsp Error! \n");
			}
		}
		else
		{
			printf("--->COnenetNBIOT::doDispatch() sendCmdSynWaitRsp() Error! \n");
		}
	}
	if (atCmdName == "+MIPLEXECUTE")
	{
		m_ATParser.doExecute(atCmd, resModule);
		m_ATParser.executeRsp(atCmdRsp, resModule);
		//	sendCmdNoWaitRsp(atCmdRsp);//just for test
		if (0 == sendCmdSynWaitRsp(atCmdRsp, atCmdRspRsp, SERIALPORT_RCV_TIMEOUT))
		{
			//rsp rsp
			if (0 != m_ATParser.doExecuteRsp(atCmdRspRsp))
			{//error
				printf("--->COnenetNBIOT::doDispatch() +MIPLEXECUTE Rsp Error! \n");
			}
		}
		else
		{
			printf("--->COnenetNBIOT::doDispatch() sendCmdSynWaitRsp() Error! \n");
		}
	}
	if( atCmdName == "+MIPLEVENT")
	{
		int evtid = m_ATParser.doEvent(atCmd);
		printf("--->COnenetNBIOT::doDispatch() doEvent() m_iRegStatus:%d \n", m_iRegStatus);

		if (evtid < CIS_EVENT_REG_TIMEOUT)
		{
			m_iRegStatus = evtid;
		}

		switch (evtid)
		{
			case CIS_EVENT_LOGON_SUCCESS:
			{
				m_iRegStatus = NBIOT_COM_OPEN_SUCCESS;
				break;
			}
			case CIS_EVENT_STATUS_HALT:
			case CIS_EVENT_LIFETIME_TIMEOUT:
			case CIS_EVENT_UPDATE_SUCCESS:
			case CIS_EVENT_UPDATE_FAILED:
			case CIS_EVENT_UPDATE_TIMEOUT:
			case CIS_EVENT_UPDATE_NEED:
			case CIS_EVENT_RESPONSE_FAILED:
			case CIS_EVENT_RESPONSE_SUCCESS:
			case CIS_EVENT_NOTIFY_FAILED:
			case CIS_EVENT_NOTIFY_SUCCESS:
			default:
				break;
		}
	}

	if(m_CbFun)
	{//callback fun,  do something...

	
	}
}

void COnenetNBIOT::handle_time(int time)
{
	if (m_iRegStatus != NBIOT_PREPARE_SUCCESS)
	{
		printf("--->COnenetNBIOT::handle_time() m_iRegStatus:%d \n", m_iRegStatus);

		if (m_iRegStatus == NBIOT_COM_UNINIT)
		{
			//	printf("--->COnenetNBIOT::heartBeat() m_iComStatus:%d, m_iRegStatus:%d \n", m_iComStatus, m_iRegStatus);
			return;
		}
		else if (m_iRegStatus == NBIOT_DISCOVER_SUCCESS)
		{
			m_iRegStatus = NBIOT_PREPARE_SUCCESS;
			printf("--->COnenetNBIOT::handle_time() m_iRegStatus:%d \n", m_iRegStatus);
		}
	}

	//heart beat
	if ((m_iRegStatus == NBIOT_PREPARE_SUCCESS) && (m_iLifeTime++ >= MAX_HEARTBEAT_LIFETIME / 2))
	{
		//heart beat
		int iRet = heartBeat();
		if (iRet == 0)
		{//ok
			m_iLifeTime = 0;
		}
		else
		{//error
			m_iLifeTime = MAX_HEARTBEAT_LIFETIME / 3;
		}
	}

	if (m_iRegStatus != NBIOT_PREPARE_SUCCESS)
	{//init or error
		if (m_iRegTime++ >= MAX_MODULE_REGTIME)
		{
			m_iNeedReset = 1;
			m_iRegTime = 0;
		}
	}
	else
	{//ok
		m_iRegTime = 0;
	}

	if (1 == m_iNeedReset)
	{
		printf("--->COnenetNBIOT::handle_time() RebootNBIOTModule() m_iRegStatus:%d \n", m_iRegStatus);

		unInitNBIOTModule();
		m_iRegStatus = NBIOT_COM_OPEN_SUCCESS;
		m_iLifeTime = 0;
		m_iRegTime = 0;

		RebootNBIOTModule();

		int iRet = initNBIOTModule();
		if (iRet == -1)
		{//error
			printf("--->COnenetNBIOT::handle_time() initNBIOTModule() Error! \n");
		}

		m_iNeedReset = 0;
	}

	return;
}

int COnenetNBIOT::heartBeat()
{
	int iRet = 0;

	CData atCmd;
	CData atCmdRsp;
	m_ATParser.update(atCmd);
	if (0 == sendCmdSynWaitRsp(atCmd, atCmdRsp, SERIALPORT_RCV_TIMEOUT))
	{
		CData version;
		if (0 != m_ATParser.doUpdateRsp(atCmdRsp))
		{//error
			printf("--->COnenetNBIOT::heartBeat() doUpdateRsp() Error! \n");
			iRet = -1;
		}
	}
	else
	{
		printf("--->COnenetNBIOT::heartBeat() sendCmdSynWaitRsp() Error! \n");
		iRet = -1;
	}

	return iRet;
}

CLierdaOnenetATParser::CLierdaOnenetATParser()
{
	m_uiNBModRef = 0;
}

CLierdaOnenetATParser::~CLierdaOnenetATParser()
{

}

#if VERSION_LIERDA //VERSION_LIERDA_180426A
int CLierdaOnenetATParser::initModule(CData &atCmd)
{
	atCmd = "AT+MIPLCREATE";
	atCmd += STR_CRLF;
	return 0;
}
int CLierdaOnenetATParser::doInitModuleRsp(CData atCmdRsp)
{
	//send: AT+MIPLCREATE=<totalsize>,<config>,<currentsize>,<flag><CR>          
	//rsp: <CR><LF>OK<CR><LF>  /  <CR><LF>+MIPLCREATE:<ref><CR><LF>OK<CR><LF>    /   <CR><LF>+CIS ERROR:<errid><CR><LF>
	int iErrNo = 100;
	CData rspCmd = atCmdRsp.substring(3, atCmdRsp.find(":"));
	if (rspCmd == "CIS ERROR")
	{//error
		iErrNo = 100;
		iErrNo = atCmdRsp.substring(atCmdRsp.find(":") + 1, atCmdRsp.find(STR_CRLF, 2)).convertInt();
	}
	else  if (rspCmd == "MIPLCREATE")
	{//ok   get ref
		m_uiNBModRef = atCmdRsp.substring(atCmdRsp.find(":") + 1, atCmdRsp.find(STR_CRLF, 2)).convertInt();
		iErrNo = 0;
	}
	else
	{//ok
		iErrNo = 0;
	}

	return iErrNo;
}
#else  //VERSION_LIERDA_180412
int CLierdaOnenetATParser::initModule(CData &atCmd)
{
	//send: AT+MIPLCREATE=<totalsize>,<config>,<currentsize>,<flag><CR>          
	//rsp: <CR><LF>OK<CR><LF>  /  <CR><LF><ref><CR><LF>OK <CR><LF>    /   <CR><LF>+CIS ERROR:<errid><CR><LF>
	atCmd = "AT+MIPLCREATE=51,130033F10003F200250400110005434D494F54000130000130000D3138332E3233302E34302E3339000130F30008E200C80000,0,51,0";
//	atCmd = "AT+MIPLCREATE=51,130033F10003F200250400110005434D494F54000130000130000D3138332E3233302E34302E3339000130F30008D104000000,0,51,0";
//	atCmd = "AT+MIPLCREATE=48,130030F10003F200220400110005434D494F5400000000000D3138332E3233302E34302E33390000F300080000000000,0,48,0";
	atCmd += STR_CRLF;
	return 0;
}
int CLierdaOnenetATParser::doInitModuleRsp(CData atCmdRsp)
{
	//send: AT+MIPLCREATE=<totalsize>,<config>,<currentsize>,<flag><CR>          
	//rsp: <CR><LF>OK<CR><LF>  /  <CR><LF><ref><CR><LF>OK<CR><LF> / <CR><LF>+CIS ERROR:<errid><CR><LF>
	int iErrNo = 100;
	int flag = atCmdRsp.find(":");
	if (flag != CDATA_NPOS)
	{//error
		iErrNo = 100;
		iErrNo = atCmdRsp.substring(atCmdRsp.find(":") + 1, atCmdRsp.find(STR_CRLF, 2)).convertInt();
	}
	else  if (atCmdRsp.length() > 5)
	{//ok

		m_uiNBModRef = atCmdRsp.substring(2, atCmdRsp.find(STR_CRLF, 2)).convertInt();
		iErrNo = 0;
	}
	else
	{//ok
		iErrNo = 0;
	}
	
	return iErrNo;
}
#endif 

int CLierdaOnenetATParser::unInitModule(CData &atCmd)
{
	//send: AT+MIPLDELETE=<ref><CR>      
	//rsp: <CR><LF>OK<CR><LF>    /   <CR><LF>+CIS ERROR:<errid><CR><LF>
	atCmd = "AT+MIPLDELETE=";
	atCmd += CData(m_uiNBModRef);
	atCmd += STR_CR;

	return 0;
}

int CLierdaOnenetATParser::doUninitModuleRsp(CData atCmdRsp)
{
	return doModuleRsp(atCmdRsp);
}

int CLierdaOnenetATParser::doModuleRsp(CData atCmdRsp)
{
	//rsp: <CR><LF>OK<CR><LF>    /   <CR><LF>+CIS ERROR:<errid><CR><LF>
	int iErrNo = 100;
	if (atCmdRsp.length() <= 2)
	{
		printf("--->CLierdaOnenetATParser::doModuleRsp() Error!");
		return iErrNo;
	}
	if(atCmdRsp.getChar(2) == CHAR_ADD)
	{//error
		iErrNo = 100;
		iErrNo = atCmdRsp.substring(atCmdRsp.find(":") + 1, atCmdRsp.find(STR_CRLF, 2)).convertInt();
	}
	else if(atCmdRsp.substr(2,2) == "OK")
	{//ok //just for test
		iErrNo = 0;
	}

	return iErrNo;
}
int CLierdaOnenetATParser::openModule(CData &atCmd)
{
	//send: AT+MIPLOPEN=<ref>,<lifetime>[,<timeout>]<CR>   
	//rsp: <CR><LF>OK<CR><LF>    /   <CR><LF>+CIS ERROR:<errid><CR><LF>
	atCmd = "AT+MIPLOPEN=";
	atCmd += CData(m_uiNBModRef);
	atCmd += STR_SPLIT;
	atCmd += CData(MAX_HEARTBEAT_LIFETIME);//"3600";	
	atCmd += STR_SPLIT;
	atCmd += "30";
	atCmd += STR_CR;
	return 0;
}

int CLierdaOnenetATParser::doOpenModuleRsp(CData atCmdRsp)
{
	//send: AT+MIPLOPEN=<ref>,<lifetime>[,<timeout>]<CR>   
	//rsp: <CR><LF>OK<CR><LF>    /   <CR><LF>+CIS ERROR:<errid><CR><LF>
	return doModuleRsp(atCmdRsp);
}

int CLierdaOnenetATParser::closeModule(CData &atCmd)
{
	//send: AT+MIPLCLOSE=<ref><CR> 
	//rsp: <CR><LF>OK<CR><LF>    /   <CR><LF>+CIS ERROR:<errid><CR><LF>
	atCmd = "AT+MIPLCLOSE=";
	atCmd += CData(m_uiNBModRef);
	atCmd += STR_CR;
	return 0;
}

int CLierdaOnenetATParser::doCloseModuleRsp(CData atCmdRsp)
{
	//send: AT+MIPLCLOSE=<ref><CR> 
	//rsp: <CR><LF>OK<CR><LF>    /   <CR><LF>+CIS ERROR:<errid><CR><LF>
	return doModuleRsp(atCmdRsp);
}

int CLierdaOnenetATParser::addObj(CData &atCmd)
{
	//send: AT+MIPLADDOBJ=<ref>,<objectid>,<instancecount>,<instancebitmap>,<attributecount>,<actioncount><CR> 
	//rsp: <CR><LF>OK<CR><LF>    /   <CR><LF>+CIS ERROR:<errid><CR><LF>
	atCmd = "AT+MIPLADDOBJ=";
	atCmd += CData(m_uiNBModRef);

	atCmd += STR_SPLIT;
	atCmd += "3200";	

	atCmd += STR_SPLIT;
	atCmd += "1";	

	atCmd += STR_SPLIT;
	atCmd += "1";	

	atCmd += STR_SPLIT;
	atCmd += "3";	

	atCmd += STR_SPLIT;
	atCmd += "0";	

	atCmd += STR_CR;
	return 0;
}

int CLierdaOnenetATParser::doAddObjRsp(CData atCmdRsp)
{
	return doModuleRsp(atCmdRsp);
}

int CLierdaOnenetATParser::delObj(CData &atCmd)
{
	//send:AT+MIPLDELOBJ=<ref>,<objectid><CR> 
	//rsp: <CR><LF>OK<CR><LF>    /   <CR><LF>+CIS ERROR:<errid><CR><LF>
	atCmd = "AT+MIPLDELOBJ=";
	atCmd += CData(m_uiNBModRef);

	atCmd += STR_SPLIT;
	atCmd += "3200";	

	atCmd += STR_CR;
	return 0;
}

int CLierdaOnenetATParser::doDelObjRsp(CData atCmdRsp)
{
	return doModuleRsp(atCmdRsp);
}


int CLierdaOnenetATParser::notify(CData &atCmd, T_LWM2MResourceModule resModule, CData notifyInfo, int notifyIndex, int notifyFlag)
{
	//send: AT+MIPLNOTIFY=<ref>,<msgid>,<objectid>,<instanceid>,<resourceid>,<valuetype>,<len>,<value>,<index>,<flag>[,<ackid>]<CR>
	//rsp: <CR><LF>OK<CR><LF>    /   <CR><LF>+CIS ERROR:<errid><CR><LF>
	atCmd = "AT+MIPLNOTIFY=";
	atCmd += CData(m_uiNBModRef);

	atCmd += STR_SPLIT;
//	atCmd += CData(m_uiMsgId);	
	int msgId = -1;
	bool observeFlag = 0;
	getMsgId(msgId, observeFlag, resModule);
	atCmd += CData(msgId);


	atCmd += STR_SPLIT;
	atCmd += CData(resModule.objectId);
	
	//instanceid
	atCmd += STR_SPLIT;
	atCmd += CData(resModule.instanceId);
	
	atCmd += STR_SPLIT;
	atCmd += CData(resModule.resourceId);
	
	//<valuetype>：发送的数据类型
	//1： string 字符串型	2： opaque 不透明类型	3： integer 整型， <length>为该整型占用的字节数	
	//4： float 浮点型， <length>为该 float 占用的字节数, 当前可选 4Byte 和 8Byte	5： bool 布尔型， 0 表示 false, 1 代表 true
	int valuetype = 1; //string
	atCmd += STR_SPLIT;
	atCmd += CData(valuetype);

//	CData strValue = "\"ZNV NBIOT--JUST FOR TEST\"";
	CData strValue;
	strValue =CData('\"') + notifyInfo + CData('\"') ;
	if (valuetype == 1)
	{
		atCmd += STR_SPLIT;
		atCmd += CData(strValue.length() - 2);
	}
	else
	{
		atCmd += STR_SPLIT;
		atCmd += strValue.length();
	}

	atCmd += STR_SPLIT;
	atCmd += strValue;

	atCmd += STR_SPLIT;
	atCmd += CData(notifyIndex);

	atCmd += STR_SPLIT;
	atCmd += CData(notifyFlag);
	
	atCmd += STR_SPLIT;
	atCmd += "1";

	atCmd += STR_CR;
	return 0;
}

int CLierdaOnenetATParser::doNotifyRsp(CData atCmdRsp)
{
	return doModuleRsp(atCmdRsp);
}

int CLierdaOnenetATParser::update(CData &atCmd, int hasParam)
{
	//send: AT+MIPLUPDATE=<ref>,<lifetime>,<withObjectFlag><CR>
	//rsp: <CR><LF>OK<CR><LF>    /   <CR><LF>+CIS ERROR:<errid><CR><LF>
	atCmd = "AT+MIPLUPDATE=";
	atCmd += CData(m_uiNBModRef);

	if (hasParam == 0)
	{
		atCmd += STR_SPLIT;
		atCmd += "0";

		atCmd += STR_SPLIT;
		atCmd += "0";
	}
	else
	{
		atCmd += STR_SPLIT;
		atCmd += CData(MAX_HEARTBEAT_LIFETIME);

		atCmd += STR_SPLIT;
		atCmd += "1";
	}

	
	atCmd += STR_CR;
	return 0;
}

int CLierdaOnenetATParser::doUpdateRsp(CData atCmdRsp)
{
	return doModuleRsp(atCmdRsp);
}

int CLierdaOnenetATParser::ver(CData &atCmd)
{
	//send: AT+MIPLVER?<CR>
	//rsp: <CR><LF>version<CR><LF>OK<CR><LF>： 基础通信套件的版本号， 如“10.01”
	//       <CR><LF>+CIS ERROR:<errid><CR><LF>： 返回错误。
	atCmd = "AT+MIPLVER?";
	atCmd += STR_CR;
//	atCmd += STR_CRLF;
	return 0;
}

#if VERSION_LIERDA
int CLierdaOnenetATParser::doVerRsp(CData atCmdRsp,  CData &ver)
{
	//send: AT+MIPLVER?<CR>
	//rsp: <CR><LF>+MIPLVER:version<CR><LF>	OK<CR><LF>： 基础通信套件的版本号， 如“2.1”	
	//     <CR><LF>+CIS ERROR:<errid><CR><LF>： 返回错误。
	int iErrNo = 100;
	CData rspCmd = atCmdRsp.substring(3, atCmdRsp.find(":"));
	if (rspCmd == "CIS ERROR")
	{//error
		iErrNo = 100;
		iErrNo = atCmdRsp.substring(atCmdRsp.find(":") + 1, atCmdRsp.find(STR_CRLF, 2)).convertInt();
	}
	else  if (rspCmd == "MIPLVER")
	{//ok

		ver = atCmdRsp.substring(atCmdRsp.find(":") + 1, atCmdRsp.find(STR_CRLF, 2));
		iErrNo = 0;
	}

	return iErrNo;
}
#else
int CLierdaOnenetATParser::doVerRsp(CData atCmdRsp, CData &ver)
{
	//send: AT+MIPLVER?<CR>
	//rsp: <CR><LF>version<CR><LF>OK<CR><LF>： 基础通信套件的版本号， 如“10.01”	//	<CR><LF>+CIS ERROR : <errid><CR><LF>： 返回错误
	int iErrNo = 100;
	int flag = atCmdRsp.find(":");
	if (flag !=  CDATA_NPOS)
	{//error

		iErrNo = 100;
		iErrNo = atCmdRsp.substring(atCmdRsp.find(":") + 1, atCmdRsp.find(STR_CRLF, 2)).convertInt();
	}
	else
	{//ok

		ver = atCmdRsp.substring(2, atCmdRsp.find(STR_CRLF, 2));
		iErrNo = 0;
	}

	return iErrNo;
}
#endif

int CLierdaOnenetATParser::doRead(CData atCmd, T_LWM2MResourceModule &resModule)
{
	//read: +MIPLREAD:<ref>,<msgid>,<objectid>,<instanceid>,<resourceid>,<CR>
	//rsp: NULL

	int iErrNo = 0;
	CData atName = "+MIPLREAD:";
	int iSplitPosStart = atCmd.find(atName) + atName.length();
	int iSplitPosEnd = atCmd.find(STR_SPLIT,0);
	int iNBModRef = atCmd.substring(iSplitPosStart, iSplitPosEnd).convertInt();

	iSplitPosStart = iSplitPosEnd+1;
	iSplitPosEnd = atCmd.find(STR_SPLIT,iSplitPosStart);
	resModule.msgId = atCmd.substring(iSplitPosStart, iSplitPosEnd).convertInt();

	iSplitPosStart = iSplitPosEnd+1;
	iSplitPosEnd = atCmd.find(STR_SPLIT,iSplitPosStart);
	resModule.objectId = atCmd.substring(iSplitPosStart, iSplitPosEnd).convertInt();

	iSplitPosStart = iSplitPosEnd+1;
	iSplitPosEnd = atCmd.find(STR_SPLIT,iSplitPosStart);
	resModule.instanceId = atCmd.substring(iSplitPosStart, iSplitPosEnd).convertInt();
	
	iSplitPosStart = iSplitPosEnd+1;
	iSplitPosEnd = atCmd.find(STR_CR,iSplitPosStart);
	resModule.resourceId = atCmd.substring(iSplitPosStart, iSplitPosEnd).convertInt();

	return iErrNo;
}

int CLierdaOnenetATParser::readRsp(CData &atCmd, T_LWM2MResourceModule resModule, CData cmdInfo)
{
	int resNum = 1;// (resModule.resourceId != -1) ? 1 : 3;
	int resCnt = 1;

	//send: AT+MIPLREADRSP=<ref>,<msgid>,<result>[,<objectid>,<instanceid>,<resourceid>,<valuetype>,<len>,<value>,<index>,<flag>]<CR>
	//rsp: <CR><LF>OK<CR><LF>:<CR><LF>+CIS ERROR:<errid><CR><LF>： 返回错误
	atCmd = "AT+MIPLREADRSP=";
	atCmd += CData(m_uiNBModRef);

	atCmd += STR_SPLIT;
	atCmd += CData(resModule.msgId);

	atCmd += STR_SPLIT;
	atCmd += "1";

	atCmd += STR_SPLIT;
	atCmd += CData(resModule.objectId);
	
	atCmd += STR_SPLIT;
	atCmd += CData(resModule.instanceId);
	
	atCmd += STR_SPLIT;
	atCmd += CData(resModule.resourceId);
	
	//<valuetype>：发送的数据类型
	//1： string 字符串型	2： opaque 不透明类型	3： integer 整型， <length>为该整型占用的字节数	
	//4： float 浮点型， <length>为该 float 占用的字节数, 当前可选 4Byte 和 8Byte	5： bool 布尔型， 0 表示 false, 1 代表 true
	int valuetype = 1; //string
	atCmd += STR_SPLIT;
	atCmd += CData(valuetype);
	
//	CData strValue = "\"Read response string\""; //just for  test
	CData strValue = CData('\"') + cmdInfo + CData('\"');
	if (valuetype == 1)
	{
		atCmd += STR_SPLIT;
		atCmd += CData(strValue.length() - 2);
	}
	else
	{
		atCmd += STR_SPLIT;
		atCmd += strValue.length();
	}
	
	atCmd += STR_SPLIT;
	atCmd += strValue;

	int index = resNum - resCnt;
	atCmd += STR_SPLIT;
	atCmd += CData(index);

	int flag = (resCnt == 1) ? 1 : 2;
	if (resCnt == resNum)
	{
		flag = 0;
	}
	resCnt++;
	atCmd += STR_SPLIT;
	atCmd += CData(flag);

	while (resCnt <= resNum)
	{
		//5751
		atCmd += STR_SPLIT;
		atCmd += CData(resModule.objectId);

		atCmd += STR_SPLIT;
		atCmd += CData(resModule.instanceId);

		atCmd += STR_SPLIT;
		atCmd += CData(5751);//just for test

		//<valuetype>：发送的数据类型
		//1： string 字符串型	2： opaque 不透明类型	3： integer 整型， <length>为该整型占用的字节数	
		//4： float 浮点型， <length>为该 float 占用的字节数, 当前可选 4Byte 和 8Byte	5： bool 布尔型， 0 表示 false, 1 代表 true
		valuetype = 1; //string
		atCmd += STR_SPLIT;
		atCmd += CData(valuetype);

		strValue = "\"string\""; //just for  test

		atCmd += STR_SPLIT;
		atCmd += CData(strValue.length() - 2);

		atCmd += STR_SPLIT;
		atCmd += strValue;

		index = resNum - resCnt;
		atCmd += STR_SPLIT;
		atCmd += CData(index);

		flag = (resCnt == 1) ? 1 : 2;
		if (resCnt == resNum)
		{
			flag = 0;
		}
		resCnt++;
		atCmd += STR_SPLIT;
		atCmd += CData(flag);
	}

	atCmd += STR_CR;	
	return 0;
}

int CLierdaOnenetATParser::doReadRsp(CData atCmdRsp)
{
	return doModuleRsp(atCmdRsp);
}


int CLierdaOnenetATParser::doWrite(CData atCmd, T_LWM2MResourceModule &resModule, CData &cmdInfo)
{
	//read: +MIPLWRITE:<ref>,<msgid>,<objectid>,<instanceid>,<resourceid>, <valuetype>, <len>, <value>, <flag> ，<index><CR>
	//rsp: NULL

	int iErrNo = 0;
	CData atName = "+MIPLWRITE:";
	int iSplitPosStart = atCmd.find(atName) + atName.length();
	int iSplitPosEnd = atCmd.find(STR_SPLIT, 0);
	int iNBModRef = atCmd.substring(iSplitPosStart, iSplitPosEnd).convertInt();

	iSplitPosStart = iSplitPosEnd + 1;
	iSplitPosEnd = atCmd.find(STR_SPLIT, iSplitPosStart);
	resModule.msgId = atCmd.substring(iSplitPosStart, iSplitPosEnd).convertInt();

	iSplitPosStart = iSplitPosEnd + 1;
	iSplitPosEnd = atCmd.find(STR_SPLIT, iSplitPosStart);
	resModule.objectId = atCmd.substring(iSplitPosStart, iSplitPosEnd).convertInt();

	iSplitPosStart = iSplitPosEnd + 1;
	iSplitPosEnd = atCmd.find(STR_SPLIT, iSplitPosStart);
	resModule.instanceId = atCmd.substring(iSplitPosStart, iSplitPosEnd).convertInt();

	iSplitPosStart = iSplitPosEnd + 1;
	iSplitPosEnd = atCmd.find(STR_SPLIT, iSplitPosStart);
	resModule.resourceId = atCmd.substring(iSplitPosStart, iSplitPosEnd).convertInt();

	iSplitPosStart = iSplitPosEnd + 1;
	iSplitPosEnd = atCmd.find(STR_SPLIT, iSplitPosStart);
	int valuetype = atCmd.substring(iSplitPosStart, iSplitPosEnd).convertInt();

	iSplitPosStart = iSplitPosEnd + 1;
	iSplitPosEnd = atCmd.find(STR_SPLIT, iSplitPosStart);
	int len = atCmd.substring(iSplitPosStart, iSplitPosEnd).convertInt();

	iSplitPosStart = iSplitPosEnd + 1;
	iSplitPosEnd = atCmd.find(STR_SPLIT, iSplitPosStart);
	cmdInfo = atCmd.substring(iSplitPosStart, iSplitPosEnd);

	iSplitPosStart = iSplitPosEnd + 1;
	iSplitPosEnd = atCmd.find(STR_SPLIT, iSplitPosStart);
	int flag = atCmd.substring(iSplitPosStart, iSplitPosEnd).convertInt();

	iSplitPosStart = iSplitPosEnd + 1;
	iSplitPosEnd = atCmd.find(STR_CR, iSplitPosStart);
	int index = atCmd.substring(iSplitPosStart, iSplitPosEnd).convertInt();

	return iErrNo;
}
int CLierdaOnenetATParser::writeRsp(CData &atCmd, T_LWM2MResourceModule resModule, int result)
{
	//send: AT+MIPLWRITERSP=<ref>,<msgid>, <result><CR>
	//rsp: <CR><LF>OK<CR><LF>:<CR><LF>+CIS ERROR:<errid><CR><LF>： 返回错误
	atCmd = "AT+MIPLWRITERSP=";
	atCmd += CData(m_uiNBModRef);

	atCmd += STR_SPLIT;
	atCmd += CData(resModule.msgId);

	atCmd += STR_SPLIT;
	atCmd += CData(result);

	atCmd += STR_CR;

	return 0;
}
int CLierdaOnenetATParser::doWriteRsp(CData atCmdRsp)
{
	return doModuleRsp(atCmdRsp);
}

int CLierdaOnenetATParser::doParameter(CData atCmd, T_LWM2MResourceModule &resModule)
{
	return 0;
}
int CLierdaOnenetATParser::parameterRsp(CData &atCmd, T_LWM2MResourceModule resModule)
{
	return 0;
}
int CLierdaOnenetATParser::doParameterRsp(CData atCmdRsp)
{
	return doModuleRsp(atCmdRsp);
}

int CLierdaOnenetATParser::doExecute(CData atCmd, T_LWM2MResourceModule &resModule)
{
	return 0;
}
int CLierdaOnenetATParser::executeRsp(CData &atCmd, T_LWM2MResourceModule resModule)
{
	return 0;

}
int CLierdaOnenetATParser::doExecuteRsp(CData atCmdRsp)
{
	return doModuleRsp(atCmdRsp);
}

int CLierdaOnenetATParser::doObserver(CData atCmd, T_LWM2MResourceModule &resModule)
{
	//read: +MIPLOBSERVE: <ref>,<msgid>,<flag>,<objectid>,<instanceid>[,<resourceid>]<CR>
	//rsp: NULL
	int iErrNo = 0;
	CData atName = "+MIPLOBSERVE:";
	int iSplitPosStart = atCmd.find(atName) + atName.length();
	int iSplitPosEnd = atCmd.find(STR_SPLIT,0);
	int iNBModRef = atCmd.substring(iSplitPosStart, iSplitPosEnd).convertInt();

	iSplitPosStart = iSplitPosEnd+1;
	iSplitPosEnd = atCmd.find(STR_SPLIT,iSplitPosStart);
//	m_uiMsgId = atCmd.substring(iSplitPosStart, iSplitPosEnd).convertInt();
	int msgId = atCmd.substring(iSplitPosStart, iSplitPosEnd).convertInt();

	iSplitPosStart = iSplitPosEnd+1;
	iSplitPosEnd = atCmd.find(STR_SPLIT,iSplitPosStart);
	int flag = atCmd.substring(iSplitPosStart, iSplitPosEnd).convertInt();

	iSplitPosStart = iSplitPosEnd+1;
	iSplitPosEnd = atCmd.find(STR_SPLIT,iSplitPosStart);
	int objId = atCmd.substring(iSplitPosStart, iSplitPosEnd).convertInt();

	iSplitPosStart = iSplitPosEnd+1;
	iSplitPosEnd = atCmd.find(STR_SPLIT,iSplitPosStart);
	int instanceId = 0;
	int resourceId = 0;
	//+MIPLOBSERVE: <ref>,<msgid>,<flag>,<objectid>,<instanceid>[,<resourceid>]<CR>
	if (iSplitPosEnd == CDATA_NPOS)
	{//+MIPLOBSERVE: <ref>,<msgid>,<flag>,<objectid>,<instanceid><CR>
		iSplitPosEnd = atCmd.find(STR_CR, iSplitPosStart);
		instanceId = atCmd.substring(iSplitPosStart, iSplitPosEnd).convertInt();
		resourceId = -1;
	}
	else
	{//+MIPLOBSERVE: <ref>,<msgid>,<flag>,<objectid>,<instanceid>,<resourceid><CR>
		instanceId = atCmd.substring(iSplitPosStart, iSplitPosEnd).convertInt();

		iSplitPosStart = iSplitPosEnd + 1;
		iSplitPosEnd = atCmd.find(STR_CR, iSplitPosStart);
		resourceId = atCmd.substring(iSplitPosStart, iSplitPosEnd).convertInt();
	}
	
	resModule.objectId = objId;
	resModule.instanceId = instanceId;
	resModule.resourceId = resourceId;

	//ADD to Observer List
	//.....
	setMsgId(msgId, flag, resModule);

	return iErrNo;
}

int CLierdaOnenetATParser::observerRsp(CData &atCmd, T_LWM2MResourceModule resModule)
{
	//send: AT+MIPLOBSERVERSP=<ref>,<msgid>,<result><CR>
	//rsp: <CR><LF>OK<CR><LF>:<CR><LF>+CIS ERROR:<errid><CR><LF>： 返回错误
	atCmd = "AT+MIPLOBSERVERSP=";
	atCmd += CData(m_uiNBModRef);

	atCmd += STR_SPLIT;
//	atCmd += CData(m_uiMsgId);
	int msgId = -1;
	bool flag = 0;
	getMsgId(msgId, flag, resModule);
	atCmd += CData(msgId);

	atCmd += STR_SPLIT;
	atCmd += "1";	

	atCmd += STR_CR;	
	return 0;
}

int CLierdaOnenetATParser::doObserverRsp(CData atCmdRsp)
{
	return doModuleRsp(atCmdRsp);
}

int CLierdaOnenetATParser::doEvent(CData atCmd)
{
	//read: +MIPLEVENT:<ref>,<evtid>[,<extend>,<ackid>]<CR>
	//rsp: NULL

	int iErrNo = 0;
	CData atName = "+MIPLEVENT:";
	int iSplitPosStart = atCmd.find(atName) + atName.length();
	int iSplitPosEnd = atCmd.find(STR_SPLIT);
	int iNBModRef = atCmd.substring(iSplitPosStart, iSplitPosEnd).convertInt();


	iSplitPosStart = iSplitPosEnd+1;
	int evtid = 0;
	//+MIPLEVENT:<ref>,<evtid><CR>
	iSplitPosEnd = atCmd.find(STR_SPLIT, iSplitPosStart);
	if (iSplitPosEnd == CDATA_NPOS)
	{
		iSplitPosEnd = atCmd.find(STR_CR, iSplitPosStart);
		evtid = atCmd.substring(iSplitPosStart, iSplitPosEnd).convertInt();
	}
	else
	{//+MIPLEVENT:<ref>,<evtid>[,<extend>,<ackid>]<CR>
		evtid = atCmd.substring(iSplitPosStart, iSplitPosEnd).convertInt();

		iSplitPosEnd = atCmd.find(STR_SPLIT, iSplitPosStart);
//		extend = atCmd.substring(iSplitPosStart, iSplitPosEnd).convertInt();
		iSplitPosEnd = atCmd.find(STR_CR, iSplitPosStart);
//		ackid = atCmd.substring(iSplitPosStart, iSplitPosEnd).convertInt();
	}
	
	return evtid;
}


int CLierdaOnenetATParser::doDiscover(CData atCmd, T_LWM2MResourceModule &resModule)
{
	//read:+MIPLDISCOVER=<ref>,<msgid>, <objecteid><CR>
	//rsp: NULL

	int iErrNo = 0;
	CData atName = "+MIPLDISCOVER:";
	int iSplitPosStart = atCmd.find(atName) + atName.length();
	int iSplitPosEnd = atCmd.find(STR_SPLIT,0);
	int iNBModRef = atCmd.substring(iSplitPosStart, iSplitPosEnd).convertInt();

	iSplitPosStart = iSplitPosEnd+1;
	iSplitPosEnd = atCmd.find(STR_SPLIT,iSplitPosStart);
	resModule.msgId = atCmd.substring(iSplitPosStart, iSplitPosEnd).convertInt();

	iSplitPosStart = iSplitPosEnd+1;
	iSplitPosEnd = atCmd.find(STR_CR,iSplitPosStart);
	resModule.objectId = atCmd.substring(iSplitPosStart, iSplitPosEnd).convertInt();

	return iErrNo;
}
int CLierdaOnenetATParser::discoverRsp(CData &atCmd, T_LWM2MResourceModule resModule)
{
	//CData valueString = "\"5750;5751;30001;\""; //just for test
	CData valueString = "\"5750;5751;30001;\""; //just for test	
	//CData valueString = "\"5750;5751;30001;30002;30003;30004;30005;30006;\""; //just for test
	//	valueString = "\"5750;5505\""; //just for test
	//send: AT+MIPLDISCOVERRSP=<ref>,<msgid>,<result>,<length>,<valuestring><CR>
	//rsp: <CR><LF>OK<CR><LF>:<CR><LF>+CIS ERROR:<errid><CR><LF>： 返回错误
	atCmd = "AT+MIPLDISCOVERRSP=";
	atCmd += CData(m_uiNBModRef);

	atCmd += STR_SPLIT;
	atCmd += CData(resModule.msgId);

	atCmd += STR_SPLIT;
	atCmd += "1";	

	atCmd += STR_SPLIT;
	atCmd += CData(valueString.length()-2);

	atCmd += STR_SPLIT;
	atCmd += valueString;

	atCmd += STR_CR;	
	return 0;
}

int CLierdaOnenetATParser::doDiscoverRsp(CData atCmdRsp)
{
	return doModuleRsp(atCmdRsp);
}


int CLierdaOnenetATParser::setMsgId(int msgId, bool flag, T_LWM2MResourceModule resModule)
{
	if (resModule.objectId <= 0 || resModule.instanceId < -1 || resModule.resourceId < -1)
	{
		return -1;
	}
	if (resModule.instanceId == -1)
	{
		m_msgidMap[resModule.objectId].isAll = 1;
		m_msgidMap[resModule.objectId].observeFlag = flag;
		m_msgidMap[resModule.objectId].msgId = msgId;
		m_msgidMap[resModule.objectId].objectId = resModule.objectId;
	}
	else if (resModule.resourceId == -1)
	{
		m_msgidMap[resModule.objectId].isAll = 0;
		m_msgidMap[resModule.objectId].observeFlag = flag;
		m_msgidMap[resModule.objectId].msgId = msgId;
		m_msgidMap[resModule.objectId].objectId = resModule.objectId;

		m_msgidMap[resModule.objectId].instanceMsgId[resModule.instanceId].isAll = 1;
		m_msgidMap[resModule.objectId].instanceMsgId[resModule.instanceId].observeFlag = flag;
		m_msgidMap[resModule.objectId].instanceMsgId[resModule.instanceId].msgId = msgId;
		m_msgidMap[resModule.objectId].instanceMsgId[resModule.instanceId].instanceId = resModule.instanceId;
	}
	else
	{
		m_msgidMap[resModule.objectId].isAll = 0;
		m_msgidMap[resModule.objectId].observeFlag = flag;
		m_msgidMap[resModule.objectId].msgId = msgId;
		m_msgidMap[resModule.objectId].objectId = resModule.objectId;

		m_msgidMap[resModule.objectId].instanceMsgId[resModule.instanceId].isAll = 0;
		m_msgidMap[resModule.objectId].instanceMsgId[resModule.instanceId].observeFlag = flag;
		m_msgidMap[resModule.objectId].instanceMsgId[resModule.instanceId].msgId = msgId;
		m_msgidMap[resModule.objectId].instanceMsgId[resModule.instanceId].instanceId = resModule.instanceId;

		m_msgidMap[resModule.objectId].instanceMsgId[resModule.instanceId].resourceMsgId[resModule.resourceId].observeFlag = flag;
		m_msgidMap[resModule.objectId].instanceMsgId[resModule.instanceId].resourceMsgId[resModule.resourceId].msgId = msgId;
		m_msgidMap[resModule.objectId].instanceMsgId[resModule.instanceId].resourceMsgId[resModule.resourceId].resourceId = resModule.resourceId;
	}
	return 0;
}

int CLierdaOnenetATParser::getMsgId(int &msgId, bool &flag, T_LWM2MResourceModule resModule)
{
	if (resModule.objectId <= 0 || resModule.instanceId < -1 || resModule.resourceId < -1)
	{
		return -1;
	}
	msgId = 0;
	flag  = 0;
	if (m_msgidMap[resModule.objectId].isAll)
	{
		msgId = m_msgidMap[resModule.objectId].msgId;
		flag = m_msgidMap[resModule.objectId].observeFlag;
	}
	else if (m_msgidMap[resModule.objectId].instanceMsgId[resModule.instanceId].isAll)
	{
		msgId = m_msgidMap[resModule.objectId].instanceMsgId[resModule.instanceId].msgId;
		flag = m_msgidMap[resModule.objectId].instanceMsgId[resModule.instanceId].observeFlag;
	}
	else
	{
		msgId = m_msgidMap[resModule.objectId].instanceMsgId[resModule.instanceId].resourceMsgId[resModule.resourceId].msgId;
		flag = m_msgidMap[resModule.objectId].instanceMsgId[resModule.instanceId].resourceMsgId[resModule.resourceId].observeFlag;
	}
	
	return msgId;
}
