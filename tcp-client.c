/*
 * @Author: 0xSchnappi 952768182@qq.com
 * @Date: 2024-08-27 09:32:37
 * @LastEditors: 0xSchnappi 952768182@qq.com
 * @LastEditTime: 2024-08-28 11:26:03
 * @FilePath: /network/tcp-client.c
 * @Description: tcp 客户端
 *
 * Copyright (c) 2024 by github.com/0xSchnappi, All Rights Reserved.
 */

#include <arpa/inet.h> // 二进制ip转换
#include <errno.h>
#include <netinet/in.h> // sockaddr_in 结构体
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h> // close函数

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 5000
#define BUFFER_SIZE 1024
void main() {
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd == -1) {

    perror("Socket");
  }

  struct sockaddr_in name = {
      .sin_family = AF_INET,
      .sin_port = htons(SERVER_PORT),
  };

  // 将ip地址转为二进制
  if (inet_pton(AF_INET, SERVER_IP, &name.sin_addr) <= 0) {
    perror("Inet pton");
    close(socket_fd);
  }

  if (connect(socket_fd, (struct sockaddr *)&name, sizeof(name)) == -1) {
    perror("Connection");
    close(socket_fd);
  }

  send_data(stdin, socket_fd);
}

#define MESSAGE_SIZE 10240000
void send_data(FILE *fp, int sockfd) {
  char *query;
  query = malloc(MESSAGE_SIZE + 1);
  for (size_t i = 0; i < MESSAGE_SIZE; i++) {
    query[i] = 'a' + i % 26;
  }
  query[MESSAGE_SIZE] = '\0';

  const char *cp;
  cp = query;
  int remaining = strlen(query);

  while (remaining) {
    int n_written = send(sockfd, cp, remaining, 0);
    fprintf(stdout, "Send into buffer %ld \n", n_written);
    if (n_written <= 0) {
      perror("Send");
      return;
    }
    remaining -= n_written;
    cp += n_written;
  }

  free(query);
  return;
}
