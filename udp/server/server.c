// UDP-Server
// server port

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 512

int main(int argc, char **argv){
    int sockfd; //ソケット
    int port; //ポート番号
    int clientLength; //クライアントアドレスのバイトサイズ
    struct sockaddr_in serverAddr; //サーバーアドレス
    struct sockaddr_in clientAddr; //クライアントアドレス
    struct hostent *hostp; //クライアントホスト情報
    char msg[BUFSIZE]; //メッセージ・buffer
    char *hostaddrp; //ドット付き十進数で表されたホストアドレス
    int optValue; //setsockoptのフラグ値

    /* 
    * コマンドライン引数のチェック 
    */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }
    port = atoi(argv[1]);


}