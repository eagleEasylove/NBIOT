
#ifndef _IOT_MODULE_OUT_H_
#define _IOT_MODULE_OUT_H_

#include "dllexport.h"

//cmd: 0:unkown; 1 read;  2: write;   3:execute;  4:....
typedef int(*OnReceiveCallback)(int cbCmd, const char *pCallbackPara, char *pBuf, int iBufLen);

// 0:onenet; 1:udp;
#define NBIOT_MODE_ONENET 0
#define NBIOT_MODE_UDP 1

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


//Query Status list
#define NBIOT_BOOTSTRAP_START 1
#define NBIOT_BOOTSTRAP_SUCCESS 2
#define NBIOT_BOOTSTRAP_FAILED 3
#define NBIOT_CONNECT_SUCCESS 4
#define NBIOT_CONNECT_FAILED 5
#define NBIOT_REG_SUCCESS 6
#define NBIOT_REG_FAILED 7
#define NBIOT_REG_TIMEOUT 8
#define NBIOT_REG_REGING 9

#ifndef NBIOT_PREPARE_SUCCESS
#define NBIOT_COM_UNINIT      -1
#define NBIOT_COM_OPEN_SUCCESS 0

#define NBIOT_OBSERVE_SUCCESS  96
#define NBIOT_OBSERVE_FAILED   97
#define NBIOT_DISCOVER_SUCCESS 98
#define NBIOT_DISCOVER_FAILED  99

#define NBIOT_PREPARE_SUCCESS 100
#endif 



#endif 

