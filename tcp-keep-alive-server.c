/*
 * @Author: 0xSchnappi 952768182@qq.com
 * @Date: 2024-09-04 10:09:38
 * @LastEditors: 0xSchnappi 952768182@qq.com
 * @LastEditTime: 2024-09-04 16:48:27
 * @FilePath: /network/tcp-keep-alive-server.c
 * @Description: 心跳服务端，保持连接服务端
 *
 * Copyright (c) 2024 by github.com/0xSchnappi, All Rights Reserved.
 */

#include "common.h"
#include <arpa/inet.h>
#include <error.h>
#include <netinet/in.h>
#include <stdio.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>

static int sleep_time = 1;

int main() {

  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in server_addr, client_addr;

  bzero(&server_addr, sizeof(server_addr));

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(SERVER_PORT);
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(socket_fd, &server_addr, sizeof(server_addr)) == -1) {
    perror("Bind");
  }

  if (listen(socket_fd, MAX_CONNECTION_QUEUE) == -1) {
    perror("Listen");
  }

  int conn_fd = -1;
  bzero(&client_addr, sizeof(client_addr));
  socklen_t addrlen = sizeof(client_addr);
  if ((conn_fd = accept(socket_fd, (struct sockaddr *)&client_addr,
                        &addrlen)) == -1) {
    perror("Accept");
  }

  struct MessageObject message;
  static int count = 0;

  for (;;) {
    int n = read(conn_fd, (char *)&message, sizeof(message));
    if (n < 0) {
      perror("Read");
    } else if (n == 0) {
      error(1, 0, "Client closed\n");
    }

    printf("received %d bytes\n", n);
    count++;

    uint32_t type = ntohl(message.type);

    switch (type) {
    case MSG_TYPE1:
      printf("process MSG_TYPE1 \n");
      break;

    case MSG_TYPE2:
      printf("Process MSG_TYPE2 \n");
      break;

    case MSG_PING:
      struct MessageObject pong_message;
      pong_message.type = MSG_PONG;
      sleep(sleep_time);
      ssize_t rc =
          send(conn_fd, (char *)&pong_message, sizeof(pong_message), 0);
      if (rc < 0) {
        perror("Send");
      }
      break;

    default:
      error(1, 0, "unknown message type (%d)\n", ntohl(message.type));
      break;
    }
  }
}