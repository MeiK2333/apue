# 高级进程间通信

## 随书代码

### 17-3 && 17-4

```shell
$ gcc 17-3.c -lapue -lpthread -o pollmsg
$ gcc 17-4.c -lapue -o sendmsg
$ ./pollmsg &
[2] 56775
$ queue ID 0 is 65537
queue ID 1 is 65536
queue ID 2 is 65538

$ ./sendmsg 0x123 "hello world"
queue id 65537, message hello world
$ ./sendmsg 0x124 "just a test"
queue id 65536, message just a test
$ ./sendmsg 0x125 "bye"
queue id 65538, message bye
```

### 17-5

```shell
$ gcc 17-5.c -lapue
$ ./a.out
UNIX domain socket bound
$ ls -l foo.socket
srwxr-xr-x  1 meik  staff  0  7 19 14:26 foo.socket
$ ./a.out
bind failed: Address already in use
$ rm foo.socket
$ ./a.out
UNIX domain socket bound
```