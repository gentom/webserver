#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 512

int main(int argc, char** argv){
    /*
        ca: 受信用構造体
    */
    int sockfd, n;
    struct sockaddr_in ca;
    char recvline[BUFSIZE+1]; // 受信用の文字列格納配列

    if(argc != 3){
        fprintf(stderr, "Invalid parameters :( \n");
        return -1;
    }

    //### ソケットを一つ確保する ###/
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); // UDP通信用のソケット獲得

    //### 受信用の情報を構造体caに格納する ###//
    bzero((char *)&ca, sizeof(ca)); 
    ca.sin_family = AF_INET;
    ca.sin_addr.s_addr = htonl(INADDR_ANY); 
    // 自分自身のどのIPアドレスにパケットが到着しても自分宛てとして受け取る
    ca.sin_port = htons(0);
    // 自分側のポート番号はOSによって適当に付与してもらう ここをゼロ以外の値に設定すると自分側のポート番号を陽に指定できる。

    //### 受信用のポートをソケットにバインドする。これにより当該ポートでの受信が開始される
    bind(sockfd, (struct sockaddr *)&ca, sizeof(ca));

    //### キーボードから読み込んだ文字列を送信する。また受信したデータを画面に表示する。
    // 無限ループ
    for(;;){
        n = recvfrom( // ソケットからデータ読み出し。最大512文字
            sockfd,
            recvline,
            BUFSIZE, 0,
            (struct sockaddr *)NULL, (socklen_t *)NULL
        );
        if(n != 0){
            recvline[n] = '\0'; //受信データには文字列の終端記号が含まれていないので書き足す。
            fputs(recvline, stdout); //受信文字列を画面に出力
            fflush(stdout); // 強制的に画面に書き出し
        }
        
    }

    return 0;
}