
#ifndef C_NBIOT_UDP_H
#define C_NBIOT_UDP_H

#include "NBIOT.h"
#include "IOTModuleCommon.h"

typedef struct
{
	int socketId;

	//	CData localIpAddr;
	int   localPort;

//	CData localIpAddrOut;
//	int   localPortOut;

	CData remoteIpAddr;
	int   remotePort;
}T_UdpInfo;

class  CLierdaUDPATParser
{
public:
	CLierdaUDPATParser();
	virtual ~CLierdaUDPATParser();

public:
	int initModule(CData &atCmd);
	int doInitModuleRsp(CData atCmdRsp);

	int unInitModule(CData &atCmd);
	int doUninitModuleRsp(CData atCmdRsp);

	int doModuleRsp(CData atCmdRsp);

	int notify(CData &atCmd, int socketId, CData remoteAddr, int remotePort, CData notifyInfo);
	int doNotifyRsp(CData atCmdRsp);

	int createUdpSocket(CData &atCmd, int udpLocalPort);
	int doCreateUdpSocketRsp(CData atCmdRsp, int &socketId);

	int closeUdpSocket(CData &atCmd, int socketId);
	int doCloseUdpSocketRsp(CData atCmd);
	
	int doNewMsgIndicator(CData atCmd, int& socketId, int& msgLen);
	int doReceiveMsgRsp(CData atCmd, CData& msgInfo);

	int doGetLocalAddrRsp(CData atCmd, CData& msgInfo);

	int doIMSIRsp(CData atCmd, CData& msgInfo);
	int doIMEIRsp(CData atCmd, CData& msgInfo);

	int doEvent(CData atCmd);

private:
	int setSocketId(int socketId, int port);
	int getSocketId(int socketId);

	int convertStr2Hex(CData strInfo, CData &hexInfo);
	int convertHex2Str(CData hexInfo, CData &strInfo);

private:

};


class  CNBIOTUdp : public CNBIOT
{
public:
	CNBIOTUdp();
	virtual ~CNBIOTUdp();

public:
	virtual int initNBIOTModule();//
	virtual int unInitNBIOTModule();//

	virtual int reportInfo(CData reportInfo);//

	virtual void OnInput(char *data, int len);//
	virtual void doDispatch(CData atCmd);//
	int doReceiveMsg(CData atCmd, CData& msgInfo);

	virtual void handle_time(int time = 0);//

	virtual int SetUdpServerInfo(CData serverIp, int serverPort);//

	int RecreateSocket();
	int CloseSockets();

private:
	int reportInfo(int socketId, CData reportInfo);

	int heartBeat();

	int getLocalAddr();

	int CreateUdpSocket();
	int CreateUdpSocket(int localPort, int &socketId);
	int CloseUdpSocket(int socketId);

private:
	int m_iLifeTime;
	int m_iRegTime;

	CLierdaUDPATParser m_ATParser;

private:
	CData m_IMEI;
	CData m_IMSI;

	int m_socketId;//just for test
	CData m_localIpAddr;
	//udp info
	std::map<int, T_UdpInfo> m_UdpInfo; //socketid, T_UdpInfo
};


#endif

