# UNIX 基础知识

## 随书代码

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
hello world from process ID 851
$ ./a.out
hello world from process ID 854
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
$ kill 90612
[刚刚的程序]
% Terminated: 15
```

## 习题

### 1.1

**在系统上验证，除根目录外，目录 . 和目录 .. 是不同的**  
通过编写代码 [p1.c](p1.c) ，验证 . 和目录 .. 不同

```shell
$ ls -ldi /etc/. /etc/..
20971521 drwxr-xr-x 148 root root 12288 5月   2 08:08 /etc/.
       2 drwxr-xr-x  25 root root  4096 4月  26 15:45 /etc/..
$ ls -ldi /. /..
2 drwxr-xr-x 25 root root 4096 4月  26 15:45 /.
2 drwxr-xr-x 25 root root 4096 4月  26 15:45 /..
```

- -d 仅打印目录信息
- -i 打印文件或目录的 i 节点编号

可见，对除了根目录外的目录， . 和 .. 是不同的。

### 1.2

**分析 [1.6.2.c](1.6.2.c) 的输出，说明进程 ID 为 852 和 853 的进程发生了什么情况**  
Unix 系统下，创建一个新的进程时，内核顺序分配 PID ，第一次运行程序和第二次运行程序之间的时间里，系统里创建了两个新的进程

### 1.3

**在 [1.7.c](1.7.c) 中， perror 的参数是用 ISO C 的属性 const 定义的，而 strerror 的整形参数没有用此属性定义，为什么？**  
下面是两个函数的定义：

```C
#include <string.h>
char *strerror(int errnum);

#include <stdio.h>
void perror(const char *msg);
```

```strerror``` 传递的参数为 ```int``` 类型，将会在对应的函数内复制一份，因此不会修改原来参数的值

```perror``` 传递的参数为 ```const char *``` 类型，传递的是对应的地址，如果不使用 ```const``` 限定的话，那么如果在函数内修改了值，函数外的变量值也会变化

### 1.4

**若日历时间存放在带符号的 32 位整型数中，那么到哪一年它将溢出？可以用什么办法扩展溢出浮点数？采用的策略是否与现有的应用相兼容？**  
32 位整型数的最大值为 2147483647 ，大概为 68.09626 年。通过查询可知，最大可表示到 2038-01-19 11:14:07

使用 IEEE754 编码扩展（double），与现有的应用不一定兼容。

### 1.5

**若进程时间存放在带符号的 32 位整型数中，而且每秒为 100 时钟滴答，那么经过多少天后该时间值将会溢出？**  
[p5.c](p5.c)

```shell
$ gcc p5.c
$ ./a.out
248.551348
```