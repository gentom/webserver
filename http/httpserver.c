#include <stdio.h> //printf(), fprintf(), perror()
#include <sys/socket.h> //socket(), bind(), accept(), listen()
#include <arpa/inet.h> // struct sockaddr_in, struct sockaddr, inet_ntoa()
#include <stdlib.h> //atoi(), exit(), EXIT_FAILURE, EXIT_SUCCESS
#include <string.h> //memset()
#include <unistd.h> //close()


#define DEFAULT_SERV_TCP_PORT 8888 // default port: 8888

int main(int argc, char** argv){
    int sockfd, len, s, n, port;
    struct sockaddr_in ca;
    char buf[1024];
    char scode[] = "HTTP/1.0 200 OK\n";
    char eoh[] = "\n";
    FILE *fp;

    switch(argc){
    case 1:
        port = DEFAULT_SERV_TCP_PORT;
        break;
    case 2:
        port = atoi(argv[1]);
        break;
    default:
        fprintf(stderr, "invalid arguments. \n");
        return EXIT_FAILURE;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    ca.sin_family = AF_INET;
    ca.sin_addr.s_addr = htonl(INADDR_ANY);
    ca.sin_port = htons(port);

    // if fail on binding
    if(bind(sockfd, (struct sockaddr *) &ca, sizeof(ca)) == -1 ){
        perror("bind");
        return EXIT_FAILURE;
    }

    listen(sockfd, 5);

    len = sizeof(ca);
    s = accept(sockfd, (struct sockaddr *) &ca, (socklen_t *) &len);


    n = recv(s, buf, sizeof(buf), 0);
    buf[n] = 0;


    send(s, scode, strlen(scode), 0);
    send(s, eoh, strlen(eoh), 0);

    fp = fopen("index.html", "r");
    while ((n = fread(buf, 1, 1024, fp)) > 0){
        send(s, buf, n, 0);
    }
    fclose(fp);
    close(s);
    return EXIT_SUCCESS;

}
