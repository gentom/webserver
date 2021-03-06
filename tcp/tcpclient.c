#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char** argv){
    int sockfd, n;
    struct sockaddr_in sa;
    char sendline[512], buf[512];

    if(argc != 3){
        fprintf(stderr, "invalid arguments. \n");
        return EXIT_FAILURE;
    }

    // ソケットを1つ確保する
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // SOCK_STREAMでTCPを指定

    // 構造体saを用意
    bzero((char *)&sa, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr(argv[1]);
    sa.sin_port = htons(atoi(argv[2]));

    // connect() で目的の相手と接続
    connect(sockfd, (struct sockaddr *)&sa, sizeof(sa));
    printf("Connected\n");

    // あとはsend()とrecv()でファイル入手力と同様に送受信、read()とwrite()でもほぼ同じ
    for(;;){
        fgets(sendline, 512, stdin);
        n = strlen(sendline);
        send(sockfd, sendline, n, 0);
        n = recv(sockfd, buf, sizeof(buf), 0);
        buf[n] = '\0';
        fputs(buf, stdout);
        fflush(stdout);
    }

    return EXIT_SUCCESS;
}