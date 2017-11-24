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
        sa: 送信用構造体
    */
    int sockfd, n;
    struct sockaddr_in sa;
    char sendline[BUFSIZE]; // 送信用の文字列格納配列
    char username[BUFSIZE];
    char sdata[BUFSIZE];

    // ./sender 150.89.212.255 50003
    if(argc != 3){
        fprintf(stderr, "Invalid parameters :( \n");
        return -1;
    }

    //### 通信相手先の送信用情報を構造体saに格納する ###//
    bzero((char *)&sa, sizeof(sa)); // 構造体saの中をゼロで埋める
    sa.sin_family = AF_INET; // TCPやUDPを使うという指定
    sa.sin_addr.s_addr = inet_addr(argv[1]); // 相手IPアドレス
    sa.sin_port = htons(atoi(argv[2])); // 相手ポート番号
    // htons: host to network short , ホストバイトオーダーをネットワークバイトオーダーに変換

    //### ソケットを一つ確保する ###/
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); // UDP通信用のソケット獲得
    printf("User: ");
    scanf("%s",username);
    strcat(username, ":");
    //### キーボードから読み込んだ文字列を送信する。また受信したデータを画面に表示する。
    // 無限ループ
    for(;;){
        strcpy(sdata, username);
        fgets(sendline, BUFSIZE, stdin); //キーボードから1行読み込み
        strcat(sdata, sendline);
        n = strlen(sdata); // 文字列の長さを変数nに代入
        setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (char *)&n, sizeof(n));
        sendto(sockfd, sdata, n, 0, (struct sockaddr *)&sa, sizeof(sa)); //ソケットsockfdに配列sendlineの内容を書き出す
    }

    return 0;
}