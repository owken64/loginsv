#include "Network.h"

#include <stdio.h> //printf(), fprintf(), perror()
#include <sys/socket.h> //socket(), bind(), accept(), listen()
#include <arpa/inet.h> // struct sockaddr_in, struct sockaddr, inet_ntoa()
#include <string.h> //memset()
#include <unistd.h> //close()
#include <sys/select.h>

#include "Config.h"
#include "Parameter.h"
#include "Bool.h"

#define QUEUELIMIT 5

int ConnectProxy(); 
int ConnectDbsv(); 
int ConnectWorldsv();
int ConnectLogsv();
void CloseProxy();
void CloseDbsv();
void CloseWorldsv();
void CloseLogsv();

int NetworkInit() {
	// 初期化
	FD_ZERO(&readfds);
	flgMsgRecvProxy = FALSE;
	flgMsgRecvDbsv = FALSE;
	flgMsgRecvWorldsv = FALSE;
	flgMsgRecvLogsv = FALSE;
	flgMsgSendProxy = FALSE;
	flgMsgSendDbsv = FALSE;
	flgMsgSendWorldsv = FALSE;
	flgMsgSendLogsv = FALSE;
	
	if (ConnectDbsv() == NETWORK_CONNECT_FAILURE){
		return NETWORK_CONNECT_FAILURE;
	}
	
	if (ConnectWorldsv() == NETWORK_CONNECT_FAILURE) {
		return NETWORK_CONNECT_FAILURE;
	}
	
	if (ConnectProxy() == NETWORK_CONNECT_FAILURE) {
		return NETWORK_CONNECT_FAILURE;
	}
	
	if (ConnectLogsv() == NETWORK_CONNECT_FAILURE) {
		return NETWORK_CONNECT_FAILURE;
	}
	return NETWORK_CONNECT_SUCCESS;
}

void NetworkClose(){
	CloseProxy();
	CloseDbsv();
	CloseWorldsv();
	CloseLogsv();
}

int NetworkReceive(){
	fd_set fds;
    unsigned int clitLen; // client internet socket address length

	// fd setの初期化
	memcpy(&fds, &readfds, sizeof(fd_set));

	// fdsに設定されたソケットが読み込み可能になるまで待ちます
	select(0, &fds, NULL, NULL, NULL);

	// listenしているサーバで受信した場合
	if (FD_ISSET(sockForProxy, &fds)){
    	clitLen = sizeof(sockAddrProxy);
    	// Accept
    	if ((sockProxy = accept(sockForProxy, (struct sockaddr *) &sockAddrProxy, &clitLen)) < 0) {
    	    perror("accept() failed.");
    	    return NETWORK_RECEIVE_FAILURE;
    	}
    	FD_SET(sockProxy, &readfds);
    }

	// sockProxyに読み込み可能データがある場合
	if (FD_ISSET(sockProxy, &fds)) {
		// sockProxyからデータを受信します
		memset(messageRecvProxy, 0, sizeof(messageRecvProxy));
		recv(sockProxy, messageRecvProxy, sizeof(messageRecvProxy), 0);
		flgMsgRecvProxy = TRUE;
	}

	// sockDbsvに読み込み可能データがある場合
	if (FD_ISSET(sockDbsv, &fds)) {
		// sockDbsvからデータを受信します
		memset(messageRecvDbsv, 0, sizeof(messageRecvDbsv));
		recv(sockDbsv, messageRecvDbsv, sizeof(messageRecvDbsv), 0);
		flgMsgRecvDbsv = TRUE;
	}

	// sockWorldsvに読み込み可能データがある場合
	if (FD_ISSET(sockWorldsv, &fds)) {
		// sockWorldsvからデータを受信します
		memset(messageRecvWorldsv, 0, sizeof(messageRecvWorldsv));
		recv(sockWorldsv, messageRecvWorldsv, sizeof(messageRecvWorldsv), 0);
		flgMsgRecvWorldsv = TRUE;
	}

	// sockLogsvに読み込み可能データがある場合
	if (FD_ISSET(sockLogsv, &fds)) {
		// sockLogsvからデータを受信します
		memset(messageRecvLogsv, 0, sizeof(messageRecvLogsv));
		recv(sockLogsv, messageRecvLogsv, sizeof(messageRecvLogsv), 0);
		flgMsgRecvLogsv = TRUE;
	}

    return NETWORK_RECEIVE_SUCCESS;
}

int NetworkSend(){

	if(flgMsgSendProxy){
	    flgMsgSendProxy = FALSE;
		if (send(sockProxy, messageSendProxy, sizeof(messageSendProxy),0) < 0){
			perror("send");
			return NETWORK_SEND_FAILURE;
		}
	}

	if(flgMsgSendDbsv){
	    flgMsgSendDbsv = FALSE;
		if (send(sockDbsv, messageSendDbsv, sizeof(messageSendDbsv),0) < 0){
			perror("send");
			return NETWORK_SEND_FAILURE;
		}
	}

	if(flgMsgSendWorldsv){
	    flgMsgSendWorldsv = FALSE;
		if (send(sockWorldsv, messageSendWorldsv, sizeof(messageSendWorldsv),0) < 0){
			perror("send");
			return NETWORK_SEND_FAILURE;
		}
	}

	if(flgMsgSendLogsv){
	    flgMsgSendLogsv = FALSE;
		if (send(sockLogsv, messageSendLogsv, sizeof(messageSendLogsv),0) < 0){
			perror("send");
			return NETWORK_SEND_FAILURE;
		}
	}


	return NETWORK_SEND_SUCCESS;
}

int ConnectProxy(){

    if ((sockForProxy = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0 ){
        perror("socket() failed.");
        return NETWORK_CONNECT_FAILURE;
    }

    memset(&sockAddrForProxy, 0, sizeof(sockAddrForProxy));
    sockAddrForProxy.sin_family      = AF_INET;
    sockAddrForProxy.sin_addr.s_addr = inet_addr(IP_PROXY);
    sockAddrForProxy.sin_port        = htons(PORT_PROXY);

    if (bind(sockForProxy, (struct sockaddr *) &sockAddrForProxy, sizeof(sockAddrForProxy) ) < 0 ) {
        perror("bind() failed.");
        return NETWORK_CONNECT_FAILURE;
    }

    if (listen(sockForProxy, QUEUELIMIT) < 0) {
        perror("listen() failed.");
		return NETWORK_CONNECT_FAILURE;
    }
	
	FD_SET(sockForProxy, &readfds);

	return 0;
}

int ConnectDbsv(){
	// IPv4 TCP のソケットを作成する
    if((sockDbsv = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return NETWORK_CONNECT_FAILURE;
    }
 
    // 送信先アドレスとポート番号を設定する
    sockAddrDbsv.sin_family = AF_INET;
    sockAddrDbsv.sin_port = htons(PORT_DBSV);
    sockAddrDbsv.sin_addr.s_addr = inet_addr(IP_DBSV);
 
    // サーバ接続（TCP の場合は、接続を確立する必要がある）
    connect(sockDbsv, (struct sockaddr *)&sockAddrDbsv, sizeof(struct sockaddr_in));

	FD_SET(sockDbsv, &readfds);

 	return NETWORK_CONNECT_SUCCESS;
}

int ConnectWorldsv(){
	// IPv4 TCP のソケットを作成する
    if((sockWorldsv = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return NETWORK_CONNECT_FAILURE;
    }
 
    // 送信先アドレスとポート番号を設定する
    sockAddrWorldsv.sin_family = AF_INET;
    sockAddrWorldsv.sin_port = htons(PORT_WORLDSV);
    sockAddrWorldsv.sin_addr.s_addr = inet_addr(IP_WORLDSV);
 
    // サーバ接続（TCP の場合は、接続を確立する必要がある）
    connect(sockWorldsv, (struct sockaddr *)&sockAddrWorldsv, sizeof(struct sockaddr_in));

	FD_SET(sockWorldsv, &readfds);
 
 	return NETWORK_CONNECT_SUCCESS;
}

int ConnectLogsv(){
	// IPv4 TCP のソケットを作成する
    if((sockLogsv = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        return NETWORK_CONNECT_FAILURE;
    }
 
    // 送信先アドレスとポート番号を設定する
    sockAddrLogsv.sin_family = AF_INET;
    sockAddrLogsv.sin_port = htons(PORT_LOGSV);
    sockAddrLogsv.sin_addr.s_addr = inet_addr(IP_LOGSV);
 
    // サーバ接続（TCP の場合は、接続を確立する必要がある）
    connect(sockLogsv, (struct sockaddr *)&sockAddrLogsv, sizeof(struct sockaddr_in));

	FD_SET(sockLogsv, &readfds);
 
 	return NETWORK_CONNECT_SUCCESS;
}

void CloseProxy(){
	FD_CLR(sockProxy, &readfds);
	FD_CLR(sockForProxy, &readfds);
	close(sockProxy);
	close(sockForProxy);
}

void CloseDbsv(){
	FD_CLR(sockDbsv, &readfds);
	close(sockDbsv);
}

void CloseWorldsv(){
	FD_CLR(sockWorldsv, &readfds);
	close(sockWorldsv);
}

void CloseLogsv(){
	FD_CLR(sockLogsv, &readfds);
	close(sockLogsv);
}

