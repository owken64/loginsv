#pragma once
#include <arpa/inet.h> // struct sockaddr_in, struct sockaddr, inet_ntoa()
#include <sys/select.h>
#include "Config.h"
#include "Bool.h"

// As Server
extern int sockForProxy;
extern struct sockaddr_in sockAddrForProxy;

// To Client
extern int sockProxy;
extern struct sockaddr_in sockAddrProxy;

// To Server
extern int sockDbsv;
extern struct sockaddr_in sockAddrDbsv;
extern int sockWorldsv;
extern struct sockaddr_in sockAddrWorldsv;
extern int sockLogsv;
extern struct sockaddr_in sockAddrLogsv;

// select()用
fd_set readfds;

// Message
extern char messageRecvProxy[MSG_LENGTH];
extern BOOL flgMsgRecvProxy;
extern char messageRecvDbsv[MSG_LENGTH];
extern BOOL flgMsgRecvDbsv;
extern char messageRecvWorldsv[MSG_LENGTH];
extern BOOL flgMsgRecvWorldsv;
extern char messageRecvLogsv[MSG_LENGTH];
extern BOOL flgMsgRecvLogsv;

extern char messageSendProxy[MSG_LENGTH];
extern BOOL flgMsgSendProxy;
extern char messageSendDbsv[MSG_LENGTH];
extern BOOL flgMsgSendDbsv;
extern char messageSendWorldsv[MSG_LENGTH];
extern BOOL flgMsgSendWorldsv;
extern char messageSendLogsv[MSG_LENGTH];
extern BOOL flgMsgSendLogsv;
