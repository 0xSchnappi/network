/*
 * @Author: 0xSchnappi 952768182@qq.com
 * @Date: 2024-08-28 17:19:47
 * @LastEditors: 0xSchnappi 952768182@qq.com
 * @LastEditTime: 2024-08-29 14:30:44
 * @FilePath: /network/local-tcp-socket-server.c
 * @Description: 本地字节流套接字服务端示例
 *
 * Copyright (c) 2024 by github.com/0xSchnappi, All Rights Reserved.
 */

#include <stdbool.h> // bool
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h> // close

#define SOCK_PATH "/tmp/test-local-socket.sock"
#define MAX_CONNECTION_QUEUE 5
#define MAX_LINE 1024

int main() {
  int socket_fd = socket(AF_LOCAL, SOCK_STREAM, 0);
  if (socket_fd == -1) {
    perror("Socket");
    return -1;
  }

  struct sockaddr_un cliaddr, servaddr;
  unlink(SOCK_PATH);
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sun_family = AF_LOCAL;
  strcpy((char *)&servaddr.sun_path, SOCK_PATH);

  if (bind(socket_fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
    perror("Bind");
    close(socket_fd);
    return -1;
  }

  if (listen(socket_fd, MAX_CONNECTION_QUEUE) == -1) {
    perror("Listen");
    close(socket_fd);
    return -1;
  }

  int conn_fd = 0;
  socklen_t cil_len = 0;
  char recv[MAX_LINE + 1], send[MAX_LINE + 20];
  bzero(&cliaddr, sizeof(cliaddr));
  while (true) {
    if ((conn_fd = accept(socket_fd, (struct sockaddr *)&cliaddr, &cil_len)) ==
        -1) {
      perror("Accept");
      close(socket_fd);
      return -1;
    }

    memset(recv, 0, MAX_LINE + 1);
    int n_read = read(conn_fd, recv, MAX_LINE);
    printf("recv %d bytes char.\n", n_read);

    memset(send, 0, MAX_LINE + 20);
    snprintf(send, MAX_LINE + 19, "Hi, %s.\n", recv);
    write(conn_fd, send, strlen(send));
  }

  printf("Server: No Problem!");
  close(socket_fd);
  close(conn_fd);
}