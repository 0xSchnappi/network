/*
 * @Author: 0xSchnappi 952768182@qq.com
 * @Date: 2024-09-04 10:09:20
 * @LastEditors: 0xSchnappi 952768182@qq.com
 * @LastEditTime: 2024-09-05 15:03:06
 * @FilePath: /network/tcp-keep-alive-client.c
 * @Description: 心跳包客户端，保持连接客户端
 *
 * 功能核心：使用select创建定时器，定时发送心跳包
 *
 * Copyright (c) 2024 by github.com/0xSchnappi, All Rights Reserved.
 */

#include "common.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {

  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in server_addr = {
      .sin_family = AF_INET,
      .sin_port = htons(SERVER_PORT),
  };

  inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

  int conn_fd = connect(socket_fd, &server_addr, sizeof(server_addr));
  if (conn_fd < 0) {
    perror("Connect");
  }

  char recv_line[MAXLINE + 1];

  /* Socket */
  fd_set readmask;
  fd_set allreads;

  struct timeval tv;
  int heartbeats = 0;

  tv.tv_sec = KEEP_ALIVE_TIME; // 保活时间
  tv.tv_usec = 0;

  struct MessageObject messageObject;

  FD_ZERO(&allreads);
  FD_SET(socket_fd, &allreads);

  for (;;) {
    readmask = allreads;

    /**
     * 套接字的读写操作，设置超时事件，达到超时时间返回0
     * socket_fd + 1:是select遍历readmask的最大值，因为从0开始遍历，所以加1
     */
    int rc = select(socket_fd + 1, &readmask, NULL, NULL, &tv);
    if (rc < 0) {
      perror("Select");
    }

    if (rc == 0) { // select到达KEEP_ALIVE_TIME时间时，就会返回0
      if (++heartbeats > KEEP_ALIVE_PROBETIMES) {
        perror("Connect Dead");
      }
      printf("Sending heartbeat #%d\n", heartbeats);
      messageObject.type = htonl(MSG_PING);
      rc = send(socket_fd, (char *)&messageObject, sizeof(messageObject), 0);
      if (rc < 0) {
        perror("Send");
      }
      tv.tv_sec = KEEP_ALIVE_INTERVAL;
      continue;
    }
    if (FD_ISSET(socket_fd,
                 &readmask)) // 测试文件描述符是否仍然存在于可读集合中
    {
      int n = read(socket_fd, recv_line, MAXLINE);
      if (n < 0) {
        perror("Read");
      } else if (n == 0) {
        perror("Server");
      }
      printf("received heartbeat, make heartbeats to 0\n");
      heartbeats = 0;
      tv.tv_sec = KEEP_ALIVE_TIME;
    }
  }
}