/*
 * @Author: 0xSchnappi 952768182@qq.com
 * @Date: 2024-09-04 10:23:12
 * @LastEditors: 0xSchnappi 952768182@qq.com
 * @LastEditTime: 2024-09-04 16:59:11
 * @FilePath: /network/common.h
 * @Description: 定义服务的配置
 *
 * Copyright (c) 2024 by github.com/0xSchnappi, All Rights Reserved.
 */

/* 服务器IP和端口 */
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 5000

/* 定义读写数据的缓冲区大小 */
#define MAXLINE 4096

/* 连接socket数量 */
#define MAX_CONNECTION_QUEUE 5

/* 心跳包配置 */
#define KEEP_ALIVE_TIME 10
#define KEEP_ALIVE_INTERVAL 3
#define KEEP_ALIVE_PROBETIMES 3

#include <stdint.h>
/* 心跳包 */
struct MessageObject {
  uint32_t type;
  char data[1024];
};

#define MSG_PING 1
#define MSG_PONG 2
#define MSG_TYPE1 11
#define MSG_TYPE2 21
