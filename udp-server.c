/*
 * @Author: 0xSchnappi 952768182@qq.com
 * @Date: 2024-08-28 11:40:43
 * @LastEditors: 0xSchnappi 952768182@qq.com
 * @LastEditTime: 2024-08-28 17:08:52
 * @FilePath: /network/udp-server.c
 * @Description:udp服务端
 *
 * Copyright (c) 2024 by github.com/0xSchnappi, All Rights Reserved.
 */

#include <arpa/inet.h> // ip地址转换
#include <netinet/in.h>
#include <signal.h> // signal
#include <stdio.h>
#include <stdlib.h>  // exit
#include <string.h>  // strlen
#include <strings.h> // bezro
#include <sys/socket.h>
#include <unistd.h> // close

#define SERVER_PORT 5000
#define MAXLINE 4096

static int count;
static void recvfrom_int(int signo);

int main() {

  int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (socket_fd == -1) {
    perror("Socket");
    return -1;
  }

  struct sockaddr_in server = {
      .sin_port = htons(5000),
      .sin_family = AF_INET,
      .sin_addr = htons(INADDR_ANY),
  };

  //   if (inet_pton(AF_INET, SERVER_IP, &name.sin_addr) <= 0)
  //   {
  //     perror ("Inet pton");
  //     close (socket_fd);
  //     return -1;
  //   }

  if (bind(socket_fd, (struct sockaddr *)&server, sizeof(server)) == -1) {
    perror("Bind");
    close(socket_fd);
    return -1;
  }

  signal(
      SIGINT,
      recvfrom_int); // 创建信号处理函数，以便响应"Ctrl+c"退出时，打印出收到的报文总数

  struct sockaddr_in client;
  bzero(&client, sizeof(client));
  char message[MAXLINE + 1];
  count = 0;
  socklen_t client_len = sizeof(client);
  for (;;) {
    int n = recvfrom(socket_fd, message, MAXLINE, 0, (struct sockaddr *)&client,
                     &client_len);
    message[n] = 0;
    printf("received %d bytes: %s\n", n, message);

    char send_line[MAXLINE + 20] = {0};
    snprintf(send_line, MAXLINE + 20, "Hi, %s\n", message);

    sendto(socket_fd, send_line, strlen(send_line), 0,
           (struct sockaddr *)&client, client_len);

    count++;
  }
}

static void recvfrom_int(int signo) {
  printf("\nreceived %d datagrams\n", count);
  exit(0);
}
