#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char** argv){
    int sockfd, len, s, n, i;
    struct sockaddr_in sa, ca;
    char buf[512];
    char *c = buf;

    if(argc!=2){
        fprintf(stderr, "invalid arguments\n");
        return EXIT_FAILURE;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
    sa.sin_port = htons(atoi(argv[1]));

    if(bind(sockfd, (struct sockaddr *)&sa, sizeof(sa)) < 0){
        perror("bind");
        return EXIT_FAILURE;
    }

    listen(sockfd, 5);

    len = sizeof(ca);
    s = accept(sockfd, (struct sockaddr *)&ca, (socklen_t *)&len);
    printf("Connected\n");

    for(;;){
        n = recv(s, buf, sizeof(buf), 0);
        buf[n] = '\0';
        fputs(buf, stdout);
        fflush(stdout);
        c = buf;
        while(*c){
            *c = (char)tolower((int) *c); //文字変換
            c++;
        }
        send(s, buf, n, 0);
    }
    return EXIT_SUCCESS;
}