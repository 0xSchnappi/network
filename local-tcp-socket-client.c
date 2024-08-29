/*
 * @Author: 0xSchnappi 952768182@qq.com
 * @Date: 2024-08-28 17:19:34
 * @LastEditors: 0xSchnappi 952768182@qq.com
 * @LastEditTime: 2024-08-29 14:30:38
 * @FilePath: /network/local-tcp-socket-client.c
 * @Description: 本地字节流套接字客户端
 *
 * Copyright (c) 2024 by github.com/0xSchnappi, All Rights Reserved.
 */

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/test-local-socket.sock"
#define MAX_LINE 1024

int main() {

  int sock_fd = 0;
  if ((sock_fd = socket(AF_LOCAL, SOCK_STREAM, 0)) == -1) {
    perror("Socket");
    return -1;
  }

  struct sockaddr_un servaddr, cliaddr = {0};
  socklen_t servlen = 0;

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sun_family = AF_LOCAL;
  strcpy((char *)&servaddr.sun_path, SOCKET_PATH);

  if (connect(sock_fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
    perror("Connect");
    close(sock_fd);
    return -1;
  }

  char send[MAX_LINE + 1], recv[MAX_LINE + 21];

  memset(send, 0, MAX_LINE + 1);
  snprintf(send, MAX_LINE, "han tong");
  write(sock_fd, send, strlen(send));

  memset(recv, 0, MAX_LINE + 21);
  ssize_t n_readed = read(sock_fd, recv, MAX_LINE + 20);
  printf("%s\n", recv);

  close(sock_fd);
  return 0;
}
