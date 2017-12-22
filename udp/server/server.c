#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>

int main(int argc, char** argv){
    int sockfd, n;
    int cl; /* client length */
    struct sockaddr_in ca; /* client address */
    char buf[512]; /* 文字列受信用 */
    char *c = buf;

    if(argc!=2){
        fprintf(stderr, "invalid arguments\n");
        return EXIT_FAILURE;
    }

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    ca.sin_family = AF_INET;
    ca.sin_addr.s_addr = htonl(INADDR_ANY);
    ca.sin_port = htons(atoi(argv[1]));

    /* ソケットにバインド */
    if(bind(sockfd, (struct sockaddr *)&ca, sizeof(ca)) < 0){
        perror("failed to bind socket");
        return EXIT_FAILURE;
    }

    for(;;){
        cl = sizeof(ca); /* クライアントの長さ(サイズ)を取得 */
        n = recvfrom(sockfd, buf, 512, 0, (struct sockaddr *)&ca, (socklen_t *)&cl);
        buf[n] = '\0';
        c = buf;
        while(*c){
            *c = (char)toupper((int)*c); /* 受信した文字を大文字に直す */
            c++;
        }
        sendto(sockfd, buf, n, 0, (struct sockaddr *)&ca, cl); /* クライアントへ送信 */
        fputs(buf, stdout); /* 受信した文字列を全て書き出し */
        fflush(stdout);
    }
    return EXIT_SUCCESS;
}
