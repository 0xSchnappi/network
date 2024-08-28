/*
 * @Author: 0xSchnappi 952768182@qq.com
 * @Date: 2024-08-28 11:40:29
 * @LastEditors: 0xSchnappi 952768182@qq.com
 * @LastEditTime: 2024-08-28 17:08:46
 * @FilePath: /network/udp-client.c
 * @Description:UDP 客户端
 *
 * Copyright (c) 2024 by github.com/0xSchnappi, All Rights Reserved.
 */

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 5000
#define MAXLINE 4096

int main() {
  int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

  struct sockaddr_in server = {
      .sin_family = AF_INET,
      .sin_port = htons(SERVER_PORT),

  };

  inet_pton(AF_INET, SERVER_IP, &server.sin_addr);

  struct sockaddr *reply_addr;
  reply_addr = malloc(sizeof(struct sockaddr_in));
  char send_line[MAXLINE + 1], recv_line[MAXLINE + 20];
  socklen_t len = 0;
  int n;

  while (fgets(send_line, MAXLINE, stdin) != NULL) {
    int i = strlen(send_line);
    send_line[MAXLINE] = 0;

    printf("now sending %s\n", send_line);
    size_t rt = sendto(socket_fd, send_line, strlen(send_line), 0,
                       (struct sockaddr *)&server, sizeof(struct sockaddr_in));
    if (rt < 0) {
      perror("Send");
    }
    printf("send bytes:%zu \n", rt);

    len = 0;
    n = recvfrom(socket_fd, recv_line, MAXLINE, 0, reply_addr, &len);
    if (n < 0) {
      perror("Recvfrom");
    }
    recv_line[n] = 0;
    fputs(recv_line, stdout);
    fputc('\n', stdout);
  }

  return 0;
}