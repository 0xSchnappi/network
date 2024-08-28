/*
 * @Author: 0xSchnappi 952768182@qq.com
 * @Date: 2024-08-27 09:32:44
 * @LastEditors: 0xSchnappi 952768182@qq.com
 * @LastEditTime: 2024-08-28 10:17:04
 * @FilePath: /network/tcp-server.c
 * @Description: tcp 服务端
 *
 * Copyright (c) 2024 by github.com/0xSchnappi, All Rights Reserved.
 */

#include <netinet/in.h> // sockaddr_in
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_CONNECTION_QUEUE 5
#define LISTEN_PORT 5000

void read_data(int sockfd);

void main() {

  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd == -1) {
    perror("Socket");
    return -1;
  }

  struct sockaddr_in name = {
      .sin_addr = htons(INADDR_ANY),
      .sin_port = htons(LISTEN_PORT),
      .sin_family = AF_INET,
  };
  if (bind(socket_fd, (struct sockaddr *)&name, sizeof(name))) {
    perror("Bind");
    close(socket_fd);
    return -1;
  }

  if (listen(socket_fd, MAX_CONNECTION_QUEUE)) {
    perror("Listen");
    close(socket_fd);
    return -1;
  }

  /* 循环处理用户请求 */
  struct sockaddr_in reqname = {0};
  int nsize;
  nsize = sizeof(reqname);
  int accept_fd = accept(socket_fd, (struct sockaddr *)&reqname, &nsize);

  if (accept_fd == -1) {
    perror("Accept");
    close(socket_fd);
    return -1;
  }
}
