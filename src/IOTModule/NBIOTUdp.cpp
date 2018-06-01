
#include "NBIOTUdp.h"

CNBIOTUdp::CNBIOTUdp()
{
	m_localIpAddr = "0.0.0.0";
}

CNBIOTUdp::~CNBIOTUdp()
{

}

int CNBIOTUdp::initNBIOTModule()
{
	int iRet = 0;
	CData atCmd;
	CData atCmdRsp;

	//coap
	atCmd = CData("AT+CFUN=0") + STR_CR;
	if (0 == sendCmdSynWaitRsp(atCmd, atCmdRsp, SERIALPORT_RCV_TIMEOUT))
	{
		/* RSP: 
		OK

		//+CEREG:0, 0000, 0, 7

		*/

	}
	else
	{
		printf("--->CNBIOTUdp::initNBIOTModule() sendCmdSynWaitRsp() Error! AT+CFUN=0\n");
		iRet = -1;
	}

	atCmd = CData("AT+CGMR") + STR_CR;
	if (0 == sendCmdSynWaitRsp(atCmd, atCmdRsp, SERIALPORT_RCV_TIMEOUT))
	{
		/* RSP:
			SECURITY,V100R100C10B657SP5

			PROTOCOL, V100R100C10B657SP5

			APPLICATION, B657R5Lierda1200203

			SEC_UPDATER, V100R100C10B657SP5

			APP_UPDATER, V100R100C10B657SP5

			RADIO, LSD4NBN - LB08000002_02

			OK
		*/

	}
	else
	{
		printf("--->CNBIOTUdp::initNBIOTModule() sendCmdSynWaitRsp() Error! AT+CGMR\n");
		iRet = -1;
	}

	atCmd = CData("AT+NBAND?") + STR_CR;
	if (0 == sendCmdSynWaitRsp(atCmd, atCmdRsp, SERIALPORT_RCV_TIMEOUT))
	{
		/* RSP:
			+NBAND:8

			OK
		*/
	}
	else
	{
		printf("--->CNBIOTUdp::initNBIOTModule() sendCmdSynWaitRsp() Error! AT+NBAND?\n");
		iRet = -1;
	}

	/*
	atCmd = CData("AT+NBAND=8") + STR_CR;
	if (0 == sendCmdSynWaitRsp(atCmd, atCmdRsp, SERIALPORT_RCV_TIMEOUT))
	{
		//	if (0 != m_ATParser.doBandRsp(atCmdRsp))
		//	{//error
		//		printf("--->CNBIOTUdp::initNBIOTModule() doBandRsp() Error! \n");
		//		iRet = -1;
		//	}
	}
	else
	{
		printf("--->CNBIOTUdp::initNBIOTModule() sendCmdSynWaitRsp() Error! AT+NBAND=5\n");
		iRet = -1;
	}
	*/

	atCmd = CData("AT+CGSN=1") + STR_CR;
	if (0 == sendCmdSynWaitRsp(atCmd, atCmdRsp, SERIALPORT_RCV_TIMEOUT))
	{
		/* RSP:
			+CGSN:865353030071341

			OK
		*/
		if (0 != m_ATParser.doIMEIRsp(atCmdRsp,m_IMEI))
		{//error
			printf("--->CNBIOTUdp::reportInfo() doNotifyRsp() Error! \n");
			iRet = -1;
		}
		printf("--->CNBIOTUdp::initNBIOTModule() doIMEIRsp():%s\n", m_IMEI.c_str());
	}
	else
	{
		printf("--->CNBIOTUdp::initNBIOTModule() sendCmdSynWaitRsp() Error! AT+CGSN=1\n");
		iRet = -1;
	}
/*
//	atCmd = CData("AT+NCDP=192.168.0.1") + STR_CR;
//	sendCmdNoWaitRsp(atCmd);
*/


	//set parameter
	RebootNBIOTModule();

	atCmd = CData("AT+CFUN=1") + STR_CR;
	if (0 == sendCmdSynWaitRsp(atCmd, atCmdRsp, SERIALPORT_RCV_TIMEOUT))
	{
		//OK

	}
	else
	{
		printf("--->CNBIOTUdp::initNBIOTModule() sendCmdSynWaitRsp() Error! AT+CFUN=1\n");
		iRet = -1;
	}

	atCmd = CData("AT+CIMI") + STR_CR;
	if (0 == sendCmdSynWaitRsp(atCmd, atCmdRsp, SERIALPORT_RCV_TIMEOUT))
	{
		/* Rsp:
		460041133901461

		OK

		*/
		if (0 != m_ATParser.doIMSIRsp(atCmdRsp,m_IMSI))
		{//error
			printf("--->CNBIOTUdp::initNBIOTModule() doIMSIRsp() Error! \n");
			iRet = -1;
		}
		printf("--->CNBIOTUdp::initNBIOTModule() doIMSIRsp():%s\n", m_IMSI.c_str());
	}
	else
	{
		printf("--->CNBIOTUdp::initNBIOTModule() sendCmdSynWaitRsp() Error! AT+CIMI\n");
		iRet = -1;
	}

	atCmd = CData("AT+CMEE=1") + STR_CR;
	if (0 == sendCmdSynWaitRsp(atCmd, atCmdRsp, SERIALPORT_RCV_TIMEOUT))
	{
		/*Rsp:
		OK

		*/
		//
	}
	else
	{
		printf("--->CNBIOTUdp::initNBIOTModule() sendCmdSynWaitRsp() Error! AT+CMEE=1\n");
		iRet = -1;
	}

//	atCmd = CData("AT+CGDCONT=1,\"IP\",\"ctnet\"") + STR_CR;
//	sendCmdNoWaitRsp(atCmd);

	atCmd = CData("AT+CSCON=1") + STR_CR;
	if (0 == sendCmdSynWaitRsp(atCmd, atCmdRsp, SERIALPORT_RCV_TIMEOUT))
	{
		/*Rsp:
		OK

		*/	//
	}
	else
	{
		printf("--->CNBIOTUdp::initNBIOTModule() sendCmdSynWaitRsp() Error! AT+CSCON=1\n");
		iRet = -1;
	}

	atCmd = CData("AT+CEREG=2") + STR_CR;
	if (0 == sendCmdSynWaitRsp(atCmd, atCmdRsp, SERIALPORT_RCV_TIMEOUT))
	{
		/*Rsp:
		OK

		*/	//
	}
	else
	{
		printf("--->CNBIOTUdp::initNBIOTModule() sendCmdSynWaitRsp() Error! AT+CEREG=2\n");
		iRet = -1;
	}

	atCmd = CData("AT+NNMI=1") + STR_CR;
	if (0 == sendCmdSynWaitRsp(atCmd, atCmdRsp, SERIALPORT_RCV_TIMEOUT))
	{
		//
		/*Rsp:
		OK

		*/
	}
	else
	{
		printf("--->CNBIOTUdp::initNBIOTModule() sendCmdSynWaitRsp() Error! AT+CMEE=1\n");
		iRet = -1;
	}

	atCmd = CData("AT+CGATT=1") + STR_CR;
	if (0 == sendCmdSynWaitRsp(atCmd, atCmdRsp, SERIALPORT_RCV_TIMEOUT))
	{
		/*Rsp:
		OK

		*/		//
	}
	else
	{
		printf("--->CNBIOTUdp::initNBIOTModule() sendCmdSynWaitRsp() Error! AT+CGATT=1\n");
		iRet = -1;
	}

	atCmd = CData("AT+NUESTATS") + STR_CR;
	if (0 == sendCmdSynWaitRsp(atCmd, atCmdRsp, SERIALPORT_RCV_TIMEOUT))
	{
		/* Rsp:
		Signal power:-856
		Total power:-745
		TX power:-32768
		TX time:0
		RX time:950
		Cell ID:220752962
		ECL:255
		SNR:27
		EARFCN:3738
		PCI:150
		RSRQ:-127

		OK

		*/
		//	if (0 != m_ATParser.doStatsRsp(atCmdRsp))
		//	{//error
		//		printf("--->CNBIOTUdp::initNBIOTModule() doStatsRsp() Error! \n");
		//		iRet = -1;
		//	}
	}
	else
	{
		printf("--->CNBIOTUdp::initNBIOTModule() sendCmdSynWaitRsp() Error! AT+NUESTATS\n");
		iRet = -1;
	}

	iRet = getLocalAddr();
	if (iRet == 0)
	{
		m_iRegStatus = NBIOT_REG_SUCCESS;
		printf("--->CNBIOTUdp::initNBIOTModule() NBIOT_REG_SUCCESS\n");
	}
	else
	{
		printf("--->CNBIOTUdp::initNBIOTModule() NBIOT_REG_FAILED\n");

		m_iRegStatus = NBIOT_REG_FAILED;//just for test
	}

	return iRet;
}

int CNBIOTUdp::getLocalAddr()
{
	//	Thread::sleep(5*1000); //just for test
	int iRet = 0;
	CData atCmd;
	CData atCmdRsp;

	atCmd = CData("AT+CGPADDR") + STR_CR;
	if (0 == sendCmdSynWaitRsp(atCmd, atCmdRsp, SERIALPORT_RCV_TIMEOUT))
	{
		/* Rsp:
		+CGPADDR:0,100.126.193.105

		OK

		*/
		if (0 != m_ATParser.doGetLocalAddrRsp(atCmdRsp, m_localIpAddr))
		{//error
			printf("--->CNBIOTUdp::getLocalAddr() doGetLocalAddrRsp() Error! \n");
			iRet = -1;
		}
	}
	else
	{
		printf("--->CNBIOTUdp::getLocalAddr() sendCmdSynWaitRsp() Error! AT+CGPADDR\n");
		iRet = -1;
	}

	return iRet;
}

int CNBIOTUdp::unInitNBIOTModule()
{
	CloseSockets();

	return 0;
}

int CNBIOTUdp::reportInfo(CData strReportInfo)
{
	return reportInfo(0, strReportInfo);
}

int CNBIOTUdp::reportInfo(int socketId, CData reportInfo)
{
	if (m_iRegStatus != NBIOT_PREPARE_SUCCESS)
	{
		printf("--->CNBIOTUdp::reportInfo() ERROR, m_iRegStatus:%d \n", m_iRegStatus);
		return -1;
	}
	int iRet = 0;
	CData atCmd;
	CData atCmdRsp;

	CData remoteAddr = m_UdpInfo[m_socketId].remoteIpAddr; //"210.21.202.36";
	int   remotePort = m_UdpInfo[m_socketId].remotePort;// 18080;

	m_ATParser.notify(atCmd, socketId, remoteAddr, remotePort, reportInfo);
	if (0 == sendCmdSynWaitRsp(atCmd, atCmdRsp, SERIALPORT_RCV_TIMEOUT))
	{
		if (0 != m_ATParser.doNotifyRsp(atCmdRsp))
		{//error
			printf("--->CNBIOTUdp::reportInfo() doNotifyRsp() Error! \n");
			iRet = -1;
		}
	}
	else
	{
		printf("--->CNBIOTUdp::reportInfo() sendCmdSynWaitRsp() Error! \n");
		iRet = -1;
	}

	return 0;
}

void CNBIOTUdp::OnInput(char *data, int len)
{
	//parse AT cmd or rsp
	if (data && (len > 0))
	{
		CData atInfo = CData(data, len);
		//printf("--->CNBIOTUdp::OnInput():len:%d, data:%s\n", len, data);

		if (atInfo.find("REBOOT_CAUSE_") != CDATA_NPOS)
		{//module error 
			printf("--->CNBIOTUdp::OnInput() Module Error, regStatus:%d\n", m_iRegStatus);
			if (atInfo.find("REBOOT_CAUSE_APPLICATION_AT") == CDATA_NPOS)
			{//not atcmd reboot //module error
				m_iNeedReset = 1;//need
				printf("--->CNBIOTUdp::OnInput() need resetModule \n");
			}
			else
			{//REBOOT_CAUSE_APPLICATION_AT
				printf("--->CNBIOTUdp::OnInput() atcmd resetModule \n");
			}
		}
		else
		{//AT  cmd or rsp
			if (atInfo.find("+") != CDATA_NPOS && atInfo.find(":") != CDATA_NPOS)
			{// AT cmd
				int startPos = atInfo.find("+");
				int endPos = atInfo.find(STR_CRLF, startPos) + strlen(STR_CRLF);

				do
				{
					CData atInfoCmd = atInfo.substring(startPos, endPos);

					{
						FastMutex::ScopedLock lock(m_listMutex);
						m_atCmdList.push_back(atInfoCmd);
					}

					startPos = atInfo.find("+", endPos);
					if (startPos != CDATA_NPOS)
					{
						endPos = atInfo.find(STR_CRLF, startPos) + strlen(STR_CRLF);
						printf("--->CNBIOTUdp::OnInput() atInfo has more than one atcmd\n");
					}
				} while ((startPos != CDATA_NPOS) && (endPos != CDATA_NPOS));

				m_listEvent.set();
			}

			if ((atInfo.find("ERROR") != CDATA_NPOS || atInfo.find("OK") != CDATA_NPOS))
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

void CNBIOTUdp::doDispatch(CData atCmd)
{
	CData atCmdName = atCmd.substring(atCmd.find_first_not_of(STR_CRLF), atCmd.find(STR_COLON));

	if (atCmdName == "+NSONMI")
	{
		CData msgInfo;
		int iRet = doReceiveMsg(atCmd, msgInfo);
		if (iRet != 0)
		{
			printf("--->CNBIOTUdp::doDispatch() doReceiveMsg() Error! iRet:%d \n", iRet);
		}

		if (m_CbFun)
		{//callback fun,  do something...

			int result = m_CbFun(0, msgInfo.c_str(), NULL, 0);
		}
	}
	if (atCmdName == "+CSCON")
	{
//
	}
	if (atCmdName == "+CEREG")
	{
//
	}

	return;
}

int CNBIOTUdp::doReceiveMsg(CData atCmd, CData& msgInfo)
{
	int socketId = -1;
	int msgLen = -1;
	int iRet = m_ATParser.doNewMsgIndicator(atCmd, socketId, msgLen);

	if (msgLen > 0)
	{
		//send: +NSORF=<socket>,<req_length>
		//rsp:  <socket>,<ip_addr>,<port>,<length>, <data>, <remaining_length>
		//	    +CME ERROR : <err>
		//e.g.  AT + NSORF = 1, 10
		//		1, 192.168.5.1, 1024, 2, ABAB, 0
		//		OK
		CData atCmdRsp = "AT+NSORF=";
		atCmdRsp += CData(socketId);
		atCmdRsp += STR_SPLIT;
		atCmdRsp += CData(msgLen + 1);
		atCmdRsp += STR_CRLF;

		CData atCmdRspRsp;

		if (0 == sendCmdSynWaitRsp(atCmdRsp, atCmdRspRsp, SERIALPORT_RCV_TIMEOUT))
		{
			if (0 != m_ATParser.doReceiveMsgRsp(atCmdRspRsp, msgInfo))
			{//error
				printf("--->CNBIOTUdp::doReceiveMsg() doReceiveMsgRsp() Error! \n");
				iRet = -1;
			}
		}
		else
		{
			printf("--->CNBIOTUdp::doReceiveMsg() sendCmdSynWaitRsp() Error! \n");
			iRet = -1;
		}
	}
	
	return 0;
}

void CNBIOTUdp::handle_time(int time)
{
	if (m_iRegStatus != NBIOT_PREPARE_SUCCESS)
	{
		printf("--->CNBIOTUdp::handle_time() m_iRegStatus:%d \n", m_iRegStatus);

		if (m_iRegStatus == NBIOT_COM_UNINIT)
		{
			//	printf("--->CNBIOTUdp::heartBeat() m_iComStatus:%d, m_iRegStatus:%d \n", m_iComStatus, m_iRegStatus);
			return;
		}
		else if (m_iRegStatus == NBIOT_REG_FAILED || m_localIpAddr == "0.0.0.0")
		{//registering

			int iRet = 0;
			iRet = getLocalAddr();
			if (iRet == 0)
			{
				m_iRegStatus = NBIOT_REG_SUCCESS;
				printf("--->CNBIOTUdp::handle_time() NBIOT_REG_SUCCESS\n");
			}
			else
			{
				printf("--->CNBIOTUdp::handle_time() NBIOT_REG_FAILED\n");
				m_iRegStatus = NBIOT_REG_FAILED;//just for test
			}
		}
		else if (m_iRegStatus == NBIOT_REG_SUCCESS && m_UdpInfo.size() > 0)
		{
			printf("--->CNBIOTUdp::handle_time() NBIOT_PREPARE_SUCCESS\n");
			m_iRegStatus = NBIOT_PREPARE_SUCCESS;
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
		printf("--->CNBIOTUdp::handle_time() RebootNBIOTModule() m_iRegStatus:%d \n", m_iRegStatus);

		unInitNBIOTModule();
		m_iRegStatus = NBIOT_COM_OPEN_SUCCESS;
		m_iLifeTime = 0;
		m_iRegTime = 0;

		RebootNBIOTModule();

		int iRet = initNBIOTModule();
		if (iRet == -1)
		{//error
			printf("--->CNBIOTUdp::handle_time()  initNBIOTModule() Error! \n");
		}

		//recreate udp socket
		RecreateSocket();

		m_iNeedReset = 0;
	}

	return;
}

int CNBIOTUdp::heartBeat()
{
	int iRet = 0;

	if (reportInfo("heartbeat") == 0)
	{
		printf("--->CNBIOTUdp::heartBeat() heartbeat!\n");
	}
	else
	{
		printf("--->CNBIOTUdp::heartBeat() sendCmdSynWaitRsp() Error! \n");
		iRet = -1;
	}

	return iRet;
}

int CNBIOTUdp::SetUdpServerInfo(CData serverIp, int serverPort)
{
	if (serverPort < 0 || serverIp.length() < 7)
	{
		return -1;
	}

	int udpLocalPort = serverPort;//just for test
	int socketId = -1;

	int iRet = CreateUdpSocket(udpLocalPort, socketId);

	if (iRet >= 0)
	{
		m_socketId = socketId;//just for test

		T_UdpInfo tmpUdpInfo;
		tmpUdpInfo.socketId = socketId;
		tmpUdpInfo.localPort = udpLocalPort;

		tmpUdpInfo.remoteIpAddr = serverIp;
		tmpUdpInfo.remotePort = serverPort;

		m_UdpInfo[socketId] = tmpUdpInfo;

		if (m_iRegStatus == NBIOT_REG_SUCCESS)
		{
			m_iRegStatus = NBIOT_PREPARE_SUCCESS;
		}
		else
		{
			printf("--->CNBIOTUdp::SetUdpServerInfo() NBIOT_PREPARE_SUCCESS Error! \n");
		}
	}

	return 0;
}

int CNBIOTUdp::CreateUdpSocket(int localPort, int &socketId)
{
	int iRet = 0;

	CData atCmd;
	CData atCmdRsp;
//	int socketId = -1;

	m_ATParser.createUdpSocket(atCmd, localPort);
	if (0 == sendCmdSynWaitRsp(atCmd, atCmdRsp, SERIALPORT_RCV_TIMEOUT))
	{
		if (0 != m_ATParser.doCreateUdpSocketRsp(atCmdRsp, socketId))
		{//error
			printf("--->CNBIOTUdp::CreateUdpSocket() doCreateUdpSocketRsp() Error! \n");
			iRet = -1;
			return iRet;
		}
	}
	else
	{
		printf("--->CNBIOTUdp::CreateUdpSocket() sendCmdSynWaitRsp() Error! \n");
		iRet = -1;
		return iRet;
	}

	return iRet;
}

int CNBIOTUdp::RecreateSocket()
{
	std::map<int, T_UdpInfo>::iterator iter;
	iter = m_UdpInfo.begin();
	while (iter != m_UdpInfo.end())
	{
		int socketId = iter->first;
		int tmpSocketId = -1;

		T_UdpInfo tmpUdpInfo = iter->second;
		int localPort = tmpUdpInfo.localPort;

		CreateUdpSocket(localPort, tmpSocketId);

		if (tmpSocketId != -1 && tmpSocketId != socketId)
		{
			//del
			//add
		}

		iter++;
	}
	return 0;
}

int CNBIOTUdp::CloseSockets()
{
	std::map<int, T_UdpInfo>::iterator iter;
	iter = m_UdpInfo.begin();
	while (iter != m_UdpInfo.end()) 
	{
		int socketId = iter->first;
		T_UdpInfo tmpUdpInfo = iter->second;

		CloseUdpSocket(socketId);

		iter++;
	}
	return 0;
}

int CNBIOTUdp::CloseUdpSocket(int socketId)
{
	int iRet = 0;

	CData atCmd;
	CData atCmdRsp;
//	int socketId = -1;

	m_ATParser.closeUdpSocket(atCmd, socketId);
	if (0 == sendCmdSynWaitRsp(atCmd, atCmdRsp, SERIALPORT_RCV_TIMEOUT))
	{
		if (0 != m_ATParser.doCloseUdpSocketRsp(atCmdRsp))
		{//error
			printf("--->CNBIOTUdp::CloseUdpSocket() doCreateUdpSocketRsp() Error! \n");
			iRet = -1;
			return iRet;
		}
	}
	else
	{
		printf("--->CNBIOTUdp::CloseUdpSocket() sendCmdSynWaitRsp() Error! \n");
		iRet = -1;
		return iRet;
	}

	return iRet;
}


/*
int CNBIOTUdp::setSocketId(int socketId, CData serverIp, int serverPort)
{
	if (socketId < 0 || serverPort < 0)
	{
		return -1;
	}

	return 0;
}

int CNBIOTUdp::getSocketId(CData serverIp, int serverPort)
{
	if (serverPort < 0)
	{
		return -1;
	}

	return socketId;
}

int CNBIOTUdp::getServerInfo(int socketId, CData &serverIp, int &serverPort)
{
	if (socketId < 0)
	{
		return -1;
	}

	return 0;
}

int CNBIOTUdp::isSocketCreated(CData serverIp, int serverPort)
{
	if (serverPort < 0)
	{
		return -1;
	}

	return 0;
}
*/

///CLierdaUDPATParser
CLierdaUDPATParser::CLierdaUDPATParser()
{

}

CLierdaUDPATParser::~CLierdaUDPATParser()
{

}

int CLierdaUDPATParser::initModule(CData &atCmd)
{

	return 0;
}
int CLierdaUDPATParser::doInitModuleRsp(CData atCmdRsp)
{
	int iErrNo = 0;

	return iErrNo;
}

int CLierdaUDPATParser::unInitModule(CData &atCmd)
{

	return 0;
}

int CLierdaUDPATParser::doUninitModuleRsp(CData atCmdRsp)
{
	return doModuleRsp(atCmdRsp);
}

int CLierdaUDPATParser::doModuleRsp(CData atCmdRsp)
{
	int iErrNo = 0;

	return iErrNo;
}

int CLierdaUDPATParser::notify(CData &atCmd, int socketId, CData remoteAddr, int remotePort, CData notifyInfo)
{//udp mode
	//send: AT+NSOST=socket,remote_addr, remote_port, length, data
	//rsp: socket, length
	//     +CME ERROR : <err>

	//eg. AT+NSOST=1,192.158.5.1,1024,2,AB3
	//	  0
	//	  1, 2
	//	  OK
	atCmd = "AT+NSOST=";
	atCmd += CData(socketId);

	atCmd += STR_SPLIT;
	atCmd += remoteAddr;

	atCmd += STR_SPLIT;
	atCmd += CData(remotePort);


	atCmd += STR_SPLIT;
	atCmd += CData(notifyInfo.length());
	
//	CData strValue = "\"ZNV NBIOT--JUST FOR TEST\"";
	CData strValue;
	convertStr2Hex(notifyInfo, strValue);

	atCmd += STR_SPLIT;
	atCmd += strValue;

	atCmd += STR_CR;
	return 0;
}

int CLierdaUDPATParser::doNotifyRsp(CData atCmdRsp)
{
	//udp mode
	//send: AT+NSOST=socket,remote_addr, remote_port, length, data
	//rsp: socket, length
	//     +CME ERROR : <err>

	//eg. [13:40:10.979]发→◇AT+NSOST=0,210.21.202.36,18080,2,FF8E
	//                     □
	//	[13:40 : 11.173]收←◆
	//	0, 74
	//
	//	OK
	int iRet = 0;
	if (atCmdRsp.find("OK") == CDATA_NPOS)
	{
		CData atName = "ERROR";
		int iSplitPosStart = atCmdRsp.find(atName) + atName.length();
		int iErrNo = atCmdRsp.substring(atCmdRsp.find(":") + 1, atCmdRsp.find(STR_CRLF, iSplitPosStart)).convertInt();
		printf("--->CLierdaUDPATParser::doNotifyRsp() Error, iErrNo:!", iErrNo);

		return -1;
	}
	
	return 0;
}

int CLierdaUDPATParser::createUdpSocket(CData &atCmd, int udpLocalPort)
{
	//send: AT+NSOCR=<type>,<protocol>, <listen port>[, <receivecontrol>]
	//rsp: <socket>
	//     +CME ERROR: <err>
	/*
		[11:20 : 36.510]发→◇AT + NSOCR = DGRAM, 17, 18081, 1
		□
		[11:20 : 36.541]收←◆
		0

		OK

		[11:20 : 37.423]发→◇AT + NSOCR = DGRAM, 17, 18081, 1
		□
		[11:20 : 37.455]收←◆
		ERROR
	*/

	atCmd = "AT+NSOCR=DGRAM,17";

	atCmd += STR_SPLIT;
	atCmd += CData(udpLocalPort);

	atCmd += STR_SPLIT;
	atCmd += "1";
	
	atCmd += STR_CR;

	return 0;
}

int CLierdaUDPATParser::doCreateUdpSocketRsp(CData atCmdRsp, int &socketId)
{
	int iErrNo = 100;

	if (atCmdRsp.find("OK") == CDATA_NPOS)
	{//error
		printf("--->CLierdaUDPATParser::doCreateUdpSocketRsp() Error!");
	}
	else
	{//ok
		iErrNo = 0;
		int iPosStart = atCmdRsp.find_first_not_of(STR_CRLF) + 1;
		int iPosEnd   = atCmdRsp.find_first_of(STR_CRLF, iPosStart);
		socketId = atCmdRsp.substring(iPosStart, iPosEnd).convertInt();
	}

	return iErrNo;
}

int CLierdaUDPATParser::closeUdpSocket(CData &atCmd, int socketId)
{
	//send: AT+NSOCL= socket
	//rsp:  +CME ERROR: <err>
	/*
	AT+NSOCL=1
	OK
	*/

	atCmd = "AT+NSOCL=";

	atCmd += CData(socketId);

	atCmd += STR_CR;

	return 0;
}

int CLierdaUDPATParser::doCloseUdpSocketRsp(CData atCmd)
{
	//send: AT+NSOCL= socket
	//rsp:  +CME ERROR: <err>
	/*
	AT+NSOCL=1
	OK
	*/

	int iErrNo = 100;

	if (atCmd.find("OK") == CDATA_NPOS)
	{//error
		printf("--->CLierdaUDPATParser::doCloseUdpSocketRsp() Error!\n");
	}
	else
	{//ok
		printf("--->CLierdaUDPATParser::doCloseUdpSocketRsp() ok!\n");
	}

	return iErrNo;
}

int CLierdaUDPATParser::doNewMsgIndicator(CData atCmd, int& socketId, int& msgLen)
{
	//e.g.  +NSONMI:1, 10
	int iErrNo = 0;
	CData atName = "+NSONMI:";

	int iPosStart = atCmd.find(atName) + atName.length();
	int iPosEnd = atCmd.find(STR_SPLIT, iPosStart);
	socketId = atCmd.substring(iPosStart, iPosEnd).convertInt();

	iPosStart = iPosEnd + 1;
	iPosEnd = atCmd.find(STR_CRLF, iPosStart);
	msgLen = atCmd.substring(iPosStart, iPosEnd).convertInt();

	return 0;
}

int CLierdaUDPATParser::doReceiveMsgRsp(CData atCmd, CData& msgInfo)
{
	//send: +NSORF=<socket>,<req_length>
	//rsp:  <socket>,<ip_addr>,<port>,<length>, <data>, <remaining_length>
	//	    +CME ERROR : <err>
	//e.g.  AT + NSORF = 1, 10
	//		1, 192.168.5.1, 1024, 2, ABAB, 0
	//		OK

	int iErrNo = 0;

	int iPosStart = atCmd.find_first_not_of(STR_CRLF);
	int iPosEnd = atCmd.find(STR_SPLIT);
	int socketId = atCmd.substring(iPosStart, iPosEnd).convertInt();

	iPosStart = iPosEnd + 1;
	iPosEnd = atCmd.find(STR_SPLIT, iPosStart);
	CData remoteIpAddr = atCmd.substring(iPosStart, iPosEnd);

	iPosStart = iPosEnd + 1;
	iPosEnd = atCmd.find(STR_SPLIT, iPosStart);
	int port = atCmd.substring(iPosStart, iPosEnd).convertInt();

	iPosStart = iPosEnd + 1;
	iPosEnd = atCmd.find(STR_SPLIT, iPosStart);
	int msglen = atCmd.substring(iPosStart, iPosEnd).convertInt();

	CData hexMsgInfo;
	int remainLen = 0;
	if (msglen != 0)
	{
		iPosStart = iPosEnd + 1;
		iPosEnd = atCmd.find(STR_SPLIT, iPosStart);
		hexMsgInfo = atCmd.substring(iPosStart, iPosEnd);

		convertHex2Str(hexMsgInfo, msgInfo);
		printf("--->CLierdaUDPATParser::doReceiveMsgRsp() convertHex2Str():%s", msgInfo.c_str());

		iPosStart = iPosEnd + 1;
		iPosEnd = atCmd.find(STR_CRLF, iPosStart);
		remainLen = atCmd.substring(iPosStart, iPosEnd).convertInt();
	}
	return 0;
}

int CLierdaUDPATParser::doGetLocalAddrRsp(CData atCmd, CData& msgInfo)
{
	/*
	+CGPADDR:0, 100.126.193.105

	OK

	*/
	// OR
	/*
	+CGPADDR:0

	OK
	
	*/

	int iErrNo = 0;


	if (atCmd.find(STR_OK) != CDATA_NPOS)
	{
		CData atName = "+CGPADDR:";
		int iPosStart = atCmd.find(atName) + atName.length();
		int iPosEnd = atCmd.find(STR_SPLIT, iPosStart);
		if (iPosEnd != CDATA_NPOS)
		{
			int cid = atCmd.substring(iPosStart, iPosEnd).convertInt();

			iPosStart = iPosEnd + 1;
			iPosEnd = atCmd.find(STR_CRLF, iPosStart);
			msgInfo = atCmd.substring(iPosStart, iPosEnd);//m_localIpAddr
		}
		else
		{
			iErrNo = -1;
		}
	}
	else
	{
		iErrNo = -1;
	}

	return iErrNo;
}
int CLierdaUDPATParser::doIMEIRsp(CData atCmd, CData& msgInfo)
{
	/* RSP:
	+CGSN:865353030071341

	OK
	*/
	int iErrNo = 0;

	if (atCmd.find(STR_OK) != CDATA_NPOS)
	{
		CData atName = "+CGSN:";
		int iPosStart = atCmd.find(atName) + atName.length();
		int iPosEnd = atCmd.find(STR_CRLF, iPosStart);
		msgInfo = atCmd.substring(iPosStart, iPosEnd);//IMEI
	}
	else
	{
		iErrNo = -1;
	}

	return iErrNo;
}

int CLierdaUDPATParser::doIMSIRsp(CData atCmd, CData& msgInfo)
{//AT+CIMI
	/* Rsp:
	460041133901461

	OK

	*/
	int iErrNo = 0;

	if (atCmd.find(STR_OK) != CDATA_NPOS)
	{
		int iPosStart = atCmd.find(STR_CRLF) +2;
		int iPosEnd = atCmd.find(STR_CRLF, iPosStart);
		msgInfo = atCmd.substring(iPosStart, iPosEnd);//IMSI
	}
	else
	{
		iErrNo = -1;
	}

	return iErrNo;
}


int CLierdaUDPATParser::doEvent(CData atCmd)
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

static char ASCII_CHAR[] = { 
	'\0', ' ', ' ', ' ', ' ', ' ', '\a', '\b', '\t', '\n', '\v', '\f', '\r', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', //0~31
	' ', '!', '\"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?', //32~63
	'@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '^', '_', //64~95
	'`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~', ' ', //96~127
};

int CLierdaUDPATParser::convertStr2Hex(CData strInfo, CData &hexInfo)
{
//	hexInfo = strInfo;//just for test

	unsigned char *pSrcInfo = (unsigned char *)strInfo.c_str();
	int strLen = strInfo.length();
	int iCur = 0;

	while (iCur < strLen)
	{
		unsigned char HiHalf = pSrcInfo[iCur] >> 4;
		unsigned char LoHalf = pSrcInfo[iCur] & 0x0F;
		hexInfo.setChar(2*iCur, (HiHalf <= 9) ? (HiHalf + '0') : (HiHalf + 55));
		hexInfo.setChar(2*iCur+1, (LoHalf <= 9) ? (LoHalf + '0') : (LoHalf + 55));
		iCur++;
	}

	return 0;
}

int CLierdaUDPATParser::convertHex2Str(CData hexInfo, CData &strInfo)
{
//	strInfo = hexInfo;//just for test
	
	int hexLen = hexInfo.length();
	int iCur = 0;
//	unsigned char ucStrInfo[MAX_SERIAL_RCV_BUF_LEN];
//	memset(ucStrInfo, 0x00, sizeof(ucStrInfo));

	unsigned char * pucHexInfo = (unsigned char *)hexInfo.uppercase().c_str();
//	unsigned char * pucStrInfo = (unsigned char *)strInfo.c_str();

	while (iCur < hexLen)
	{

		int HiHalf = ((pucHexInfo[iCur] - '0') <= 9) ? (pucHexInfo[iCur] - '0') : (pucHexInfo[iCur] - 'A' + 10);
		int LoHalf = ((pucHexInfo[iCur + 1] - '0') <= 9) ? (pucHexInfo[iCur + 1] - '0') : (pucHexInfo[iCur + 1] - 'A' + 10);

//		if (HiHalf * 16 + LoHalf < 31)
//		{//just for test
//			int ASCII_CHARNUM = HiHalf * 16 + LoHalf;
//			printf("--->CLierdaUDPATParser::convertHex2Str() ASCII_CHARNUM:%d\n", ASCII_CHARNUM);
//		}

		strInfo.setChar(iCur / 2, ASCII_CHAR[HiHalf * 16 + LoHalf]);
		iCur += 2;
	}
	return 0;
}
