/*
 * @Author: 0xSchnappi 952768182@qq.com
 * @Date: 2024-08-27 09:32:44
 * @LastEditors: 0xSchnappi 952768182@qq.com
 * @LastEditTime: 2024-08-28 11:30:05
 * @FilePath: /network/tcp-server.c
 * @Description: tcp 服务端
 *
 * Copyright (c) 2024 by github.com/0xSchnappi, All Rights Reserved.
 */

#include <errno.h>
#include <netinet/in.h> // sockaddr_in
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h> // bzero函数
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
  struct sockaddr_in reqname;
  int nsize;
  int accept_fd;
  while (true) {

    nsize = sizeof(reqname);
    bzero(&reqname, nsize);
    accept_fd = accept(socket_fd, (struct sockaddr *)&reqname, &nsize);

    if (accept_fd == -1) {
      perror("Accept");
      close(socket_fd);
      return -1;
    }
    read_data(accept_fd);
    close(accept_fd);
  }
  const char *message = "Hello, Client!";
  send(accept, message, strlen(message), 0);
}

/* 从 socketfd 描述字中读取 size 个字节 */
size_t readn(int fd, void *vptr, size_t size) {
  size_t nleft;
  ssize_t nread;
  char *ptr;

  ptr = vptr;
  nleft = size;

  while (nleft > 0) {
    if ((nread = read(fd, ptr, nleft)) < 0) {
      if (errno == EINTR) // 呼叫在传输任何数据之前被信号中断读取
        nread =
            0; /* 这里需要再次调用read。表示是非阻塞I/O的情况下，没有数据可以读，需要继续调用read
                */
      else
        return -1;
    } else if (nread == 0) {
      break; /* EOF(End of File)表示套接字关闭。表述读到对方发出的FIN包 */
    }

    nleft -= nread; // 需要读取的字符减少
    ptr += nread;   // 缓存指针往下移动
  }
  return size - nleft; /* 返回的是实际读取的字节数 */
}

void read_data(int sockfd) {
  ssize_t n;
  char buf[1024] = {0};

  int time = 0;
  for (;;) {
    fprintf(stdout, "Block in read\n");
    if ((n = readn(sockfd, buf, 1024)) == 0) {
      return;
    }
    time++;
    // fprintf(stderr, "%s\n", buf);
    fprintf(stdout, "1K read for %d\n", time);
    usleep(1000);
  }
}