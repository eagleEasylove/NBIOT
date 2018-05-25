
#ifndef _IOT_MODULE_OUT_H_
#define _IOT_MODULE_OUT_H_

#include "dllexport.h"

//cmd: 1 read;  2: write;   3:execute;  4:....
typedef int(*OnReceiveCallback)(int cbCmd, const char *pCallbackPara, char *pBuf, int iBufLen);

extern "C" DLLEXPORT_API int OneNetNBIOT_Init(const char *pComName, int iComSpeed);
extern "C" DLLEXPORT_API int OneNetNBIOT_RegisterOnReceiveCallback(OnReceiveCallback cbFun);
extern "C" DLLEXPORT_API int OneNetNBIOT_ReportInfo(char *pReportInfo);
extern "C" DLLEXPORT_API int OneNetNBIOT_UnInit();

//Query Status list
#define NBIOT_BOOTSTRAP_START 1
#define NBIOT_BOOTSTRAP_SUCCESS 2
#define NBIOT_BOOTSTRAP_FAILED 3
#define NBIOT_CONNECT_SUCCESS 4
#define NBIOT_CONNECT_FAILED 5
#define NBIOT_REG_SUCCESS 6
#define NBIOT_REG_FAILED 7
#define NBIOT_REG_TIMEOUT 8

#ifndef NBIOT_PREPARE_SUCCESS
#define NBIOT_COM_UNINIT      -1
#define NBIOT_COM_OPEN_SUCCESS 0

#define NBIOT_OBSERVE_SUCCESS  96
#define NBIOT_OBSERVE_FAILED   97
#define NBIOT_DISCOVER_SUCCESS 98
#define NBIOT_DISCOVER_FAILED  99

#define NBIOT_PREPARE_SUCCESS 100
#endif 

//ok:return 0;
//status: see Query Status list
extern "C" DLLEXPORT_API int OneNetNBIOT_QueryStatus(int *pStatus);

//just for test
extern "C" DLLEXPORT_API int OneNetNBIOT_ReportInfoExt(int objId, int instanceId, int resourceId, void *pReportInfo);
extern "C" DLLEXPORT_API int OneNetNBIOT_SendAtCmd(const char *pAtCmd, char *pAtCmdRsp, int iAtCmdRspLen);

#endif 

