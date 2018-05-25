/*
 * main.cpp
*/
#include "../IOTModule/common_config_base.h"
#include <iostream>

#include <stdio.h>
#include "../Include/IOTModuleOut.h"
#include "../IOTModule/Data.h"
#include "../IOTModule/Thread.h"

using namespace CORE;

#if !defined(CORE_OS_FAMILY_WINDOWS)
#include <syscall.h>
#include <sys/prctl.h>
#include <unistd.h>
#include <execinfo.h>
#include <time.h> 
#else
#include <windows.h>  
#endif

#define NBIOT_TEST
#ifdef NBIOT_TEST

CData testInfo;
int onReceiveInfo(int cbCmd, const char *pCallbackPara, char *pBuf, int iBufLen)
{
	printf("-----> onReceiveInfo(), cbCmd:%d, pCallbackPara:%s, pBuf:%p, iBufLen:%d\n", cbCmd, pCallbackPara, pBuf, iBufLen);

	if (cbCmd == 1)
	{
		CData strValue = CData("Read response string:") + testInfo; //just for  test
		if (pBuf && iBufLen > strValue.length() + 1)
		{
			memcpy(pBuf, strValue.c_str(), strValue.length());
		}
	}
	else if (cbCmd == 2)
	{
		if (pCallbackPara)
		{
			testInfo = pCallbackPara;
		}
	}

	return 0;
}
CData getTimeString()
{
	CData tmpTime;
#if !defined(CORE_OS_FAMILY_WINDOWS)
	//linux // /dev/ttyS0
	struct tm *start;
	time_t the_time;
	(void)time(&the_time);
	start = gmtime(&the_time);

	tmpTime = CData(start->tm_year) + CData("-") + CData(start->tm_mon + 1) + CData("-") + CData(start->tm_mday) + CData(" ") +
		CData(start->tm_hour) + CData(":") + CData(start->tm_min) + CData(":") + CData(start->tm_sec);

#else
	//windows //COM5,COM7
	SYSTEMTIME start; //windows.h中  
	GetLocalTime(&start);//time.h的tm结构体一样的效果 
	tmpTime = CData(start.wYear) + CData("-") + CData(start.wMonth) + CData("-") + CData(start.wDay) + CData(" ") +
		CData(start.wHour) + CData(":") + CData(start.wMinute) + CData(":") + CData(start.wSecond) + CData(" ") + CData(start.wMilliseconds);
#endif
	return tmpTime;
}

int main(int argc, char **argv)
{
	cout << "---> main start <---";

	int i = 0;
	while (1)
	{
		cout<<"\n---> please input cmd(open,close,report,at,status):";
		char tmp[100] = { 0 };
		cin >> tmp;
		CData cmd = tmp;

		if (cmd == "open")
		{
#if !defined(CORE_OS_FAMILY_WINDOWS)
//linux // /dev/ttyS0
			CData param = "/dev/ttyUSB1";
#else
//windows //COM5,COM7
			memset(tmp, 0x00, sizeof(tmp));
			cout << "\n---> please input open param:";
			cin >> tmp;
			CData param = tmp;	
			param = CData("COM") + param;
#endif
			if (0 == OneNetNBIOT_Init(param.c_str(), 9600))
			{
				OneNetNBIOT_RegisterOnReceiveCallback(onReceiveInfo);
			}
		}
		else if (cmd == "close")
		{
			OneNetNBIOT_UnInit();
		}
		else if (cmd == "report")
		{
			while (1)
			{
				memset(tmp, 0x00, sizeof(tmp));
				cout << "---> please input report param(0, 1, resourceId):";
				cin >> tmp;
				CData param = tmp;
				if (param == "0")
				{
					CData reportInfo = "000--JUST FOR TEST";//just for test
					OneNetNBIOT_ReportInfoExt(3200, 0, 5750, (void *)(reportInfo.c_str()));
				}
				else if (param == "0-1")
				{
					CData reportInfo = "111--JUST FOR TEST";//just for test
					OneNetNBIOT_ReportInfoExt(3200, 0, 5750, (void *)(reportInfo.c_str()));
				}
				else if (param == "0-2")
				{
					CData reportInfo = "{\"FSUID\":\"44030143800001\",\"Site\":[{\"DevID\":\"0600001\",\"Meter\":[{\"ID\":\"115001\",\"Data\":\"0.0\"},{\"ID\":\"112001\",\"Data\":\"0.0\"},{\"ID\":\"125001\",\"Data\":\"33.6\"},{\"ID\":\"003001\",\"Data\":\"0\"},{\"ID\":\"002001\",\"Data\":\"0\"}]},{\"DevID\":\"0700001\",\"Meter\":[{\"ID\":\"102001\",\"Data\":\"53.2\"}]},{\"DevID\":\"4400001\",\"Meter\":[{\"ID\":\"105001\",\"Data\":\"54.2\"},{\"ID\":\"101001\",\"Data\":\"226.0\"},{\"ID\":\"005001\",\"Data\":\"0\"},{\"ID\":\"006001\",\"Data\":\"0\"},{\"ID\":\"004001\",\"Data\":\"0\"}]},{\"DevID\":\"1700001\",\"Meter\":[{\"ID\":\"005001\",\"Data\":\"1\"}]}]}";//just for test
					OneNetNBIOT_ReportInfoExt(3200, 0, 5750, (void *)(reportInfo.c_str()));
				}
				else if (param == "0-3")
				{
					CData reportInfo = "{\"FSUID\":\"44030143800001\",\"Site\":[{\"DevID\":\"0600001\",\"Meter\":[{\"ID\":\"115001\",\"Data\":\"1.0\"},{\"ID\":\"112001\",\"Data\":\"1.0\"},{\"ID\":\"125001\",\"Data\":\"22.6\"},{\"ID\":\"003001\",\"Data\":\"1\"},{\"ID\":\"002001\",\"Data\":\"1\"}]},{\"DevID\":\"0700001\",\"Meter\":[{\"ID\":\"102001\",\"Data\":\"43.2\"}]},{\"DevID\":\"4400001\",\"Meter\":[{\"ID\":\"105001\",\"Data\":\"44.2\"},{\"ID\":\"101001\",\"Data\":\"116.0\"},{\"ID\":\"005001\",\"Data\":\"1\"},{\"ID\":\"006001\",\"Data\":\"1\"},{\"ID\":\"004001\",\"Data\":\"1\"}]},{\"DevID\":\"1700001\",\"Meter\":[{\"ID\":\"005001\",\"Data\":\"0\"}]}]}";//just for test
					OneNetNBIOT_ReportInfoExt(3200, 0, 5750, (void *)(reportInfo.c_str()));
				}
				else if (param == "1")
				{
					CData reportInfo = "000--JUST FOR TEST";//just for test
					OneNetNBIOT_ReportInfoExt(3200, 0, 5751, (void *)(reportInfo.c_str()));
				}
				else if (param == "1-1")
				{
					CData reportInfo = "111--JUST FOR TEST";//just for test
					OneNetNBIOT_ReportInfoExt(3200, 0, 5751, (void *)(reportInfo.c_str()));
				}
				else if (param == "1-2")
				{
					CData reportInfo = "{\"FSUID\":\"44030143800002\",\"Site\":[{\"DevID\":\"0600001\",\"Meter\":[{\"ID\":\"115001\",\"Data\":\"0.0\"},{\"ID\":\"112001\",\"Data\":\"0.0\"},{\"ID\":\"125001\",\"Data\":\"33.6\"},{\"ID\":\"003001\",\"Data\":\"0\"},{\"ID\":\"002001\",\"Data\":\"0\"}]},{\"DevID\":\"0700001\",\"Meter\":[{\"ID\":\"102001\",\"Data\":\"53.2\"}]},{\"DevID\":\"4400001\",\"Meter\":[{\"ID\":\"105001\",\"Data\":\"54.2\"},{\"ID\":\"101001\",\"Data\":\"226.0\"},{\"ID\":\"005001\",\"Data\":\"0\"},{\"ID\":\"006001\",\"Data\":\"0\"},{\"ID\":\"004001\",\"Data\":\"0\"}]},{\"DevID\":\"1700001\",\"Meter\":[{\"ID\":\"005001\",\"Data\":\"1\"}]}]}";//just for test
					OneNetNBIOT_ReportInfoExt(3200, 0, 5751, (void *)(reportInfo.c_str()));
				}
				else if (param == "1-3")
				{
					CData reportInfo = "{\"FSUID\":\"44030143800002\",\"Site\":[{\"DevID\":\"0600001\",\"Meter\":[{\"ID\":\"115001\",\"Data\":\"1.0\"},{\"ID\":\"112001\",\"Data\":\"1.0\"},{\"ID\":\"125001\",\"Data\":\"22.6\"},{\"ID\":\"003001\",\"Data\":\"1\"},{\"ID\":\"002001\",\"Data\":\"1\"}]},{\"DevID\":\"0700001\",\"Meter\":[{\"ID\":\"102001\",\"Data\":\"43.2\"}]},{\"DevID\":\"4400001\",\"Meter\":[{\"ID\":\"105001\",\"Data\":\"44.2\"},{\"ID\":\"101001\",\"Data\":\"116.0\"},{\"ID\":\"005001\",\"Data\":\"1\"},{\"ID\":\"006001\",\"Data\":\"1\"},{\"ID\":\"004001\",\"Data\":\"1\"}]},{\"DevID\":\"1700001\",\"Meter\":[{\"ID\":\"005001\",\"Data\":\"0\"}]}]}";//just for test
					OneNetNBIOT_ReportInfoExt(3200, 0, 5751, (void *)(reportInfo.c_str()));
				}
				else if (param == "999")
				{
					memset(tmp, 0x00, sizeof(tmp));
					cout << "---> please input report Cnts:";
					cin >> tmp;
					CData reportCnt = tmp;

					int cnt = 0;
					while(cnt++ < reportCnt.convertInt())
					{
						CData tmpTime = getTimeString();
						CData reportInfo1 = "{\"FSUID\":\"44030143800001\",\"Site\":[{\"DevID\":\"0600001\",\"Meter\":[{\"ID\":\"115001\",\"Data\":\"0.0\"},{\"ID\":\"112001\",\"Data\":\"0.0\"},{\"ID\":\"125001\",\"Data\":\"33.6\"},{\"ID\":\"003001\",\"Data\":\"0\"},{\"ID\":\"002001\",\"Data\":\"0\"}]},{\"DevID\":\"0700001\",\"Meter\":[{\"ID\":\"102001\",\"Data\":\"53.2\"}]},{\"DevID\":\"4400001\",\"Meter\":[{\"ID\":\"105001\",\"Data\":\"54.2\"},{\"ID\":\"004001\",\"Data\":\"0\"}]},{\"DevID\":\"1700001\",\"Meter\":[{\"ID\":\"005001\",\"Data\":\"1\"}]}]}";//just for test
						CData reportInfo = CData(cnt) + CData(" ") +tmpTime + reportInfo1;
						OneNetNBIOT_ReportInfoExt(3200, 0, 5750, (void *)(reportInfo.c_str()));

						Thread::sleep(100);
					}
				}
				else if (param == "000" || param == "111" || param == "222" || param == "333")
				{//test upload 
					memset(tmp, 0x00, sizeof(tmp));
					cout << "---> please input report len param:";
					cin >> tmp;
					CData paramCur = tmp;
					int len = paramCur.convertInt();
					CData reportInfo;
					int cur = 0;
					while (cur<len)
					{
						if (cur%100 < 10)
						{
							reportInfo += "0";
						}
						reportInfo += CData(cur%100);

						cur++;
					}
					if (param == "000")
					{
						OneNetNBIOT_ReportInfoExt(3200, 0, 5750, (void *)(reportInfo.c_str()));
					}
					if (param == "111")
					{
						OneNetNBIOT_ReportInfoExt(3200, 0, 5751, (void *)(reportInfo.c_str()));
					}
					if (param == "222")
					{
						OneNetNBIOT_ReportInfoExt(3200, 0, 30001, (void *)(reportInfo.c_str()));
					}
				}
				else if (param == "99")
				{
					memset(tmp, 0x00, sizeof(tmp));
					cout << "---> please input report info:";
					cin >> tmp;
					CData reportInfo = tmp;
					OneNetNBIOT_ReportInfoExt(3200, 0, 5750, (void *)(reportInfo.c_str()));
				}
				else
				{
					CData resourceId = param;
					memset(tmp, 0x00, sizeof(tmp));
					cout << "---> please input report info:";
					cin >> tmp;
					CData reportInfo = tmp;
					OneNetNBIOT_ReportInfoExt(3200, 0, resourceId.convertInt(), (void *)(reportInfo.c_str()));

					break;
				}
			}
		}
		else if (cmd == "at")
		{
			memset(tmp, 0x00, sizeof(tmp));
			cout << "---> please input at Cmd:";
			cin >> tmp;
			CData param = tmp;
			char tmpRspBuf[1024];
			memset(tmpRspBuf, 0x00, sizeof(tmpRspBuf));
			OneNetNBIOT_SendAtCmd(param.c_str(), tmpRspBuf, sizeof(tmpRspBuf));
			printf("---> main() OneNetNBIOT_SendAtCmd() Rsp:%s \n", tmpRspBuf);
		}
		else if (cmd == "status")
		{
			int status = 0;
			OneNetNBIOT_QueryStatus(&status);
			printf("---> main() OneNetNBIOT_QueryStatus():%d \n", status);
		}
		else
		{
			printf("---> main() unknow cmd\n");
		}

		Thread::sleep(1*1000);
	}

	return 0;
}


#endif


