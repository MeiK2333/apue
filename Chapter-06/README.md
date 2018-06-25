# 系统数据文件和信息

## 随书代码

### 6.10.c

```shell
$ gcc 6.10.c
$ ./a.out
buffer length 16 is too small
time and date: 07:34:53 PM, Mon May 14, 2018
```

strptime 函数是 strftime 的反过来的版本，把字符串时间转换为分解时间。

strftime 函数的说明符列见书本 P.154

strptime 函数的说明符列见书本 P.156

## 课后习题

### 6.1

**如果系统使用阴影文件，那么如何取得加密口令？**  
在 linux 中，有一组函数可用于访问阴影口令文件。

```C
#include <shadow.h>

struct spwd *getspnam(const char *name);

struct spwd *getspent(void);

void setspent(void);

void endspent(void);
```

### 6.2

**假设你有超级用户权限，并且系统使用了阴影口令，重新考虑上一道习题。**  
超级用户可以直接读取 /etc/shadow 文件

### 6.3

**编写一程序，它调用 uname 并且输出 utsname 结构中的所有字段，将该输出与 uname 命令的输出结果进行比较。**  

```shell
$ uname -a
Linux MeiK-PC 4.15.0-20-generic #21-Ubuntu SMP Tue Apr 24 06:16:15 UTC 2018 x86_64 x86_64 x86_64 GNU/Linux
$ ./a.out 
sysname  = Linux
nodename = MeiK-PC
release  = 4.15.0-20-generic
version  = #21-Ubuntu SMP Tue Apr 24 06:16:15 UTC 2018
machine  = x86_64
```

### 6.4

**计算可由 time_t 数据类型表示的最近时间。如果超出了这一时间将会如何？**  
根据系统不同 time_t 可能是 long int 或者 int 。如果是 int 存储的话，那么到 2038 年将达到上限，之后将会溢出，时间变成负数。

### 6.5

**编写一程序，获取当前时间，并使用 strftime 将输出结果转换为类似于 date 命令的默认输出。将环境变量 TZ 设置为不同值，观察输出结果。**  

```shell
$ ./a.out 
Mon May 14 19:48:53 CST 2018
$ TZ=US/Mountain ./a.out 
Mon May 14 05:48:58 MDT 2018
$ TZ=Japan ./a.out 
Mon May 14 20:49:07 JST 2018
$ TZ=China ./a.out 
Mon May 14 11:49:11 China 2018
```