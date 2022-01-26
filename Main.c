#include <unistd.h>
#include <stdio.h>
#include "Bool.h"
#include "Network.h"

void mainLoop(void){

	// 初期処理
	NetworkInit();

	// ループ
	while (TRUE){
		NetworkReceive();
		
		NetworkSend();
	}

	// 終了処理	
	NetworkClose();
}

int main(void){
    //ここで子プロセスを生成し親は終了
    if(daemon(0, 0) == 0) {
        mainLoop();
    } else {
        printf("error\n");
    }
    return 0;
}