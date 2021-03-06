
#ifndef _IOT_MODULE_COMMON_H_
#define _IOT_MODULE_COMMON_H_

#define STR_CR "\r"
#define STR_LF "\n"
#define STR_CRLF "\r\n"
#define STR_OK "OK"

#define STR_SPLIT ","

#define STR_COLON ":"
#define CHAR_ADD '+'

#define SERIALPORT_RCV_TIMEOUT  15

// 0:onenet; 1:udp;
#define NBIOT_MODE_ONENET 0
#define NBIOT_MODE_UDP 1

#define MAX_PER_REPORT_LEN     512    // per buffer send max length
#define MAX_SERIAL_RCV_BUF_LEN 1024*2   // per buffer receive max length

#define MAX_HEARTBEAT_LIFETIME (60*2) //second
#define MAX_MODULE_REGTIME (60*2) //second

//cmd: 1 read;  2: write;   3:execute;  4:....
typedef int(*OnReceiveCallback)(int cbCmd, const char *pCallbackPara, char *pBuf, int iBufLen);

#define CIS_EVENT_BASE 0x00
#define CIS_EVENT_BOOTSTRAP_START CIS_EVENT_BASE + 1
#define CIS_EVENT_BOOTSTRAP_SUCCESS CIS_EVENT_BASE + 2
#define CIS_EVENT_BOOTSTRAP_FAILED CIS_EVENT_BASE + 3
#define CIS_EVENT_CONNECT_SUCCESS CIS_EVENT_BASE + 4
#define CIS_EVENT_CONNECT_FAILED CIS_EVENT_BASE + 5
#define CIS_EVENT_REG_SUCCESS CIS_EVENT_BASE + 6
#define CIS_EVENT_REG_FAILED CIS_EVENT_BASE + 7
#define CIS_EVENT_REG_TIMEOUT CIS_EVENT_BASE + 8
#define CIS_EVENT_LIFETIME_TIMEOUT CIS_EVENT_BASE + 9
#define CIS_EVENT_STATUS_HALT CIS_EVENT_BASE + 10
#define CIS_EVENT_UPDATE_SUCCESS CIS_EVENT_BASE + 11
#define CIS_EVENT_UPDATE_FAILED CIS_EVENT_BASE + 12
#define CIS_EVENT_UPDATE_TIMEOUT CIS_EVENT_BASE + 13
#define CIS_EVENT_UPDATE_NEED CIS_EVENT_BASE + 14
#define CIS_EVENT_LOGON_SUCCESS CIS_EVENT_BASE + 15//ע���ɹ�
#define CIS_EVENT_RESPONSE_FAILED CIS_EVENT_BASE + 20
#define CIS_EVENT_RESPONSE_SUCCESS CIS_EVENT_BASE + 21
#define CIS_EVENT_NOTIFY_FAILED CIS_EVENT_BASE + 25
#define CIS_EVENT_NOTIFY_SUCCESS CIS_EVENT_BASE + 26

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

