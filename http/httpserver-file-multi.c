#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> //fork
#include <sys/wait.h>
#include <fcntl.h>

#define DEFAULT_SERV_TCP_PORT 8080



int main(int argc, char** argv) {
	
  // setsocketoptで必要
  int y = 1;

  // forkで必要
  int pid, status;

  // ソケット作成で必要
	int sockfd, len, s, port, n;

  // アドレス情報
	struct sockaddr_in ca;

  // ファイル情報の取得
  int fp;

  // 取得したリクエストを格納する
	char buf[1024];

  // １行目のリクエストを分割
  char method[10], path[256], version[256], *file;

  // リクエストに対する応答に必要なテンプレート
	char scode[] = "HTTP1.0 200 OK\nContent-type: text/html\n";
	char eoh[] = "\n";
  char notfound[] = "HTTP1.1 404 Not Found\n";
  char notfound_body[] = "<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\n \
                          <html><head><title>404 NotFound</title</head>\
                          <body><h1>Not Found</h1><p>ファイルは見つかりませんでした。</p></body></html>";

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



  // サーバの初期化を行ってバインドする

	ca.sin_family = AF_INET;
	ca.sin_addr.s_addr = htonl(INADDR_ANY);
	ca.sin_port = htons(port);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&y, sizeof(y)) < 0) {
    perror("setsockopt");
    return EXIT_FAILURE;
  }

	if (bind(sockfd, (struct sockaddr *)&ca, sizeof(ca)) == -1) {
		perror("bind");
		return EXIT_FAILURE;
	}



  // listenする
	if (listen(sockfd, 5) < 0) {
		perror("listen");
		return EXIT_FAILURE;
	}



  // forkする
  // 親なら、無限ループ
  // 子なら、処理して終わり




	len = sizeof(ca);
  while((s = accept(sockfd, (struct sockaddr *)&ca, (socklen_t *) &len)) >= 0) {
    if ((pid = fork()) < 0) {
      perror("fork");
      return EXIT_FAILURE;
    } else if (pid == 0) {
      
      // アドレス情報を表示する
  	  printf("%s:%d \n", inet_ntoa(ca.sin_addr), ntohs(ca.sin_port));

      // リクエストを受信する
      n = recv(s, buf, 1024, 0);

      sscanf(buf, "%s %s %s", method, path, version);

      printf("path: %s\n", path);

      //
      if (strstr(method, "GET")) {
        len = strlen(path);
        if (path[len - 1] == '/') strcat(path, "index.html");
        file = path;
        if (file[0] == '/') file++;
        // 読み込みを行う
        if((fp = open(file, O_RDONLY)) != -1) {
          buf[n] = 0;
          printf("%s, len=%d", buf, (int)strlen(buf));
          send(s, scode, strlen(scode), 0);
          send(s, eoh, strlen(eoh), 0);
          //while ((n = read(fp, buf, 1024)) != -1) {
          //  printf("%d", n);
          //  if (n < 1024) buf[n++] = '\0'; 
          //  send(s, buf, n, 0);
          //}
          while ((n = read(fp, buf, 1024)) != -1) {
            send(s, buf, n, 0);
          }
  	      close(fp);
        // 存在しない場合には404を返す
        } else {
          send(s, notfound, strlen(notfound), 0);  
          send(s, eoh, strlen(eoh), 0);
          send(s, notfound_body, strlen(notfound_body), 0);  
        }

      }/* method check */
      close(s);
      break;
    }/* else if (child) */
    close(s);
  }/* while */

  return EXIT_SUCCESS;
}





/* vim: set sw=2 ts=2 expandtab nowrap : */

