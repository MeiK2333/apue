# 网络 IPC: 套接字

## 随书代码

### 16.3.1.c

```shell
$ gcc 16.3.1.c
$ ./a.out
67305985
ch0 = 1
ch1 = 2
ch2 = 3
ch3 = 4
```

我的系统为 macOS, 为小端字节序.

### 16.3.3.c

```shell
$ gcc 16.3.3.c -lapue
$ ./a.out baidu.com 80
flags 0 family inet type datagram protocol UDP
        host baidu.com address 123.125.115.110 port 80
flags 0 family inet type stream protocol TCP
        host baidu.com address 123.125.115.110 port 80
flags 0 family inet type datagram protocol UDP
        host baidu.com address 220.181.57.216 port 80
flags 0 family inet type stream protocol TCP
        host baidu.com address 220.181.57.216 port 80
$ ./a.out meik2333.com 80
flags 0 family inet type datagram protocol UDP
        host meik2333.com address 45.116.13.102 port 80
flags 0 family inet type stream protocol TCP
        host meik2333.com address 45.116.13.102 port 80
```