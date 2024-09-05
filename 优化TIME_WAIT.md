[toc]

# TIME_WAIT

## TIME_WAIT 的危害

端口资源的占用，一个TCP连接至少消耗一个本地端口。

一般可以开启的端口为`32768~60999`，也可以通过`net.ipv4.ip_local_port_range`指定。

如果TIME_WAIT状态过多，会导致无法创建新连接。

## 优化 TIME_WAIT

### 修改系统端口范围

- 查看系统端口范围

  ```shell
  sysctl net.ipv4.ip_local_port_range
  ```

  

- 临时修改端口范围

  ```shell
  sudo sysctl -w net.ipv4.ip_local_port_range="20000 60000"
  ```

  > 重启后会失效

- 永久修改端口范围

  在`/etc/sysctl.conf`文件中添加如下行

  ```shell
  net.ipv4.ip_local_port_range = 20000 60000
  ```

  

### net.ipv4.tcp_max_tw_buckets

`net.ipv4.tcp_max_tw_buckets` 是一个用于控制TCP TIME-WAIT套接字的内核参数。TIME-WAIT是一个TCP状态，当一个TCP连接被关闭时，套接字将进入TIME-WAIT状态，并且在一段时间内保持打开状态。在此期间，该套接字将无法被重新使用。

在Linux内核中，TIME-WAIT套接字被存储在一个特殊的数据结构中，称为TIME-WAIT套接字哈希表。该哈希表用于存储所有处于TIME-WAIT状态的套接字。`net.ipv4.tcp_max_tw_buckets` 决定了该哈希表的最大大小。当哈希表达到最大大小时，新的TIME-WAIT套接字将被拒绝，从而保护系统免受TCP SYN洪水攻击。



### TCP_TIMEWAIT_LEN

调低内核`TCP_TIMEWAIT_LEN`字段，重新编译系统。



### SO_LINGER 的设置

```c
int setsockopt (int __fd, int __level, int __optname,
		       const void *__optval, socklen_t __optlen)
    
    
struct linger so_linger;
so_linger.l_onoff = 1;
so_linger.l_linger = 0;

setsockopt(socket_fd, SOL_SOCKET, SO_LINGER, &so_linger, sizeof(so_linger));
  
```

- `l_onoff`为0，就是关闭本选项。`l_linger`值被忽略，这对应默认设置，`close`或`shutdown`立即返回。如果在套接字缓冲区中有数据残留，系统会将试着把这些数据发送出去。
- `l_onoff`为非0，且`l_linger`值也为0。调用`close`后，会立即发送一个RST标志给对端，该**TCP**连接将跳过四次握手，也就跳过了**TIME_WAIT**状态，直接关闭。排队的数据不会被发送，被动关闭方也不知道对端已经彻底断开。只有当被动关闭方正阻塞在`recv()`调用上时，接收到RST时，会立刻得到一个`connet reset by peer`的异常。(不提倡)
- `l_onoff`为非0，且`l_linger`的值也非0，那么调用`close`后，调用`close`的线程就将阻塞，直到数据被发送出去，或者设置的`l_linger`计时时间到。



### net.ipv4.tcp_tw_reuse: 更安全的设置

如果从协议角度理解是安全可控的，可以复用处于**TIME_WAIT**的套接字为新的连接所用。

从协议角度安全可控，主要两点：

1. 只适用于连接发起方。(C/S模型中的客户端)
2. 对应的**TIME_WAIT**状态的连接创建时间超过1秒才可以被复用

**注意：**使用这个选项的前提是打开对TCP时间戳的支持，即`net.ipv4.tcp_timestamps=1`
