#include "Parameter.h"

#include <arpa/inet.h> // struct sockaddr_in, struct sockaddr, inet_ntoa()
#include <sys/select.h>
#include "Config.h"
#include "Bool.h"

// As Server
int sockForProxy;
struct sockaddr_in sockAddrForProxy;

// To Client
int sockProxy;
struct sockaddr_in sockAddrProxy;

// To Server
int sockDbsv;
struct sockaddr_in sockAddrDbsv;
int sockWorldsv;
struct sockaddr_in sockAddrWorldsv;
int sockLogsv;
struct sockaddr_in sockAddrLogsv;

// select()用
fd_set readfds;

// Message
char messageRecvProxy[MSG_LENGTH];
BOOL flgMsgRecvProxy;
char messageRecvDbsv[MSG_LENGTH];
BOOL flgMsgRecvDbsv;
char messageRecvWorldsv[MSG_LENGTH];
BOOL flgMsgRecvWorldsv;
char messageRecvLogsv[MSG_LENGTH];
BOOL flgMsgRecvLogsv;

char messageSendProxy[MSG_LENGTH];
BOOL flgMsgSendProxy;
char messageSendDbsv[MSG_LENGTH];
BOOL flgMsgSendDbsv;
char messageSendWorldsv[MSG_LENGTH];
BOOL flgMsgSendWorldsv;
char messageSendLogsv[MSG_LENGTH];
BOOL flgMsgSendLogsv;
