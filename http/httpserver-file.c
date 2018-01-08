#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> //fork

#define DEFAULT_SERV_TCP_PORT 8080

int main(int argc, char** argv) {
	
	int sockfd, len, s, n, port, i;
	struct sockaddr_in ca;
	char buf[1024], loadpath[1024], method[10], path[256], version[256], *file;
	char scode[] = "HTTP1.0 200 OK\n";
	char eoh[] = "\n";
  char notfound[] = "HTTP1.1 404 Not Found\n";
  char notfound_body[] = "<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\n \
                          <html><head><title>404 NotFound</title</head>\
                          <body><h1>Not Found</h1><p>ファイルは見つかりませんでした。</p></body></html>";
	FILE *fp;

	switch(argc) {
					case 1:
									port = DEFAULT_SERV_TCP_PORT;
									break;
					case 2:
									port = atoi(argv[1]);
									break;
					default:
									fprintf(stderr, "invalid arguments.\n");
									return EXIT_FAILURE;
	}	


	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	ca.sin_family = AF_INET;
	ca.sin_addr.s_addr = htonl(INADDR_ANY);
	ca.sin_port = htons(port);

	if (bind(sockfd, (struct sockaddr *)&ca, sizeof(ca)) == -1) {
		perror("bind");
		return EXIT_FAILURE;
	}
  
  // 待ち合わせを行う
	if (listen(sockfd, 5) == -1) {
		perror("listen");
		return EXIT_FAILURE;
	}

	len = sizeof(ca);

  // 相手と接続する
	s = accept(sockfd, (struct sockaddr *)&ca, (socklen_t *) &len);

  // アドレス情報を表示する
	printf("%s:%d \n", inet_ntoa(ca.sin_addr), ntohs(ca.sin_port));

  // リクエストを受信する
	n = recv(s, buf, 1024, 0);

  sscanf(buf, "%s %s %s", method, path, version);

  //
  if (strstr(method, "GET")) {
    len = strlen(path);
    if (path[len - 1] == '/')  
      strcat(path, "index.html");
    file = path;
    if (file[0] == '/') file++;
    // 読み込みを行う
	  if((fp = fopen(file, "r")) != NULL) {
      buf[n] = 0;
      printf("%s, len=%d", buf, (int)strlen(buf));
      send(s, scode, strlen(scode), 0);
      send(s, eoh, strlen(eoh), 0);
      while ((n = fread(buf, 1, 1024, fp)) > 0) {
        send(s, buf, n, 0);
      }
  
	    fclose(fp);
    // 存在しない場合には404を返す
    } else {
      send(s, notfound, strlen(notfound), 0);  
      send(s, eoh, strlen(eoh), 0);
      send(s, notfound_body, strlen(notfound_body), 0);  
    }
  }

	close(s);
	return EXIT_SUCCESS;

}





/* vim: set sw=2 ts=2 expandtab nowrap : */

