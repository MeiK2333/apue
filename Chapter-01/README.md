## UNIX 基础知识

### 1.4.3.c
列出指定目录下的所有文件
```shell
$ gcc 1.4.3.c -lapue
$ ./a.out .
.
..
a.out
apue.h
apue_error.h
1.4.3.c

```

### 1.5.3.c & 1.5.4.c
读取标准输入并写入标准输出
```shell
$ gcc 1.5.3.c -lapue
$ ./a.out
> 1
1
> 2
2
> Hello World!
Hello World!
> ^D
```

### 1.6.2.c
打印当前进程的PID
```shell
$ gcc 1.6.2.c -lapue
$ ./a.out
hello world from process ID 80157
$ ./a.out
hello world from process ID 80158
```
标准保证 pid_t 可以保存在一个长整型中，因此为了保证可移植性，将其强制转换为长整型保存和输出。

### 1.6.3.c
从标准输入读命令并执行
```shell
$ gcc 1.6.3.c -lapue
$ ./a.out
% date
Fri Apr 20 12:11:10 CST 2018
% who
meik     console  Apr 18 09:31
meik     ttys002  Apr 18 13:35
meik     ttys005  Apr 20 11:44
% pwd
/apue/Chapter-01
% ls
1.4.3.c         1.5.3.c       1.5.4.c         1.6.2.c         1.6.3.c         README.md       a.out           apue.h          apue_error.h
% ^D
```

### 1.7.c
测试 strerror 和 perror
```shell
$ gcc 1.7.c -lapue
$ ./a.out
EACCES: Permission denied
./a.out: No such file or directory
```
```strerror``` 将 ```errnum``` 映射为一个错误消息字符串，并返回此字符串

```perror``` 基于 ```errno``` 的当前值，在标准错误（stderr）上产生一条出错消息

### 1.8.c
打印用户 ID 和组 ID
```shell
$ gcc 1.8.c -lapue
$ ./a.out
uid = 501, gid = 20
```

### 1.9.c
监听信号测试
```shell
$ gcc 1.9.c -lapue
$ ./a.out
pid: 90612
% ^Cinterrupt
^C% interrupt
[另开一个控制台]
kill 90612
[刚刚的程序]
% Terminated: 15
```