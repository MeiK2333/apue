# 标准 I/O 库

## 随书代码

### 5.8.c && 5.8.2.c

```shell
$ gcc 5.8.2.c -lapue
$ ./a.out 
Hello World!
Hello World!
^C
$ gcc 5.8.c -lapue
$ ./a.out 
Hello World!
Hello World!
```

书中指出：标准 I/O 库与直接调用 read 和 write 函数相比并不慢很多。

### 5.12.c

```shell
$ gcc 5.12.c -lapue
$ ./a.out
enter any character

one line to standard error
stream = stdin, line buffered, buffer size = 1024
stream = stdout, line buffered, buffer size = 1024
stream = stderr, unbuffered, buffer size = 1
stream = /etc/passwd, fully buffered, buffer size = 4096
$ ./a.out < /etc/group > std.out 2> std.err
$ cat std.err
one line to standard error
$ cat std.out
enter any character
stream = stdin, fully buffered, buffer size = 4096
stream = stdout, fully buffered, buffer size = 4096
stream = stderr, unbuffered, buffer size = 1
stream = /etc/passwd, fully buffered, buffer size = 4096
```

可见，当标准输入与输出在连接终端时是行缓冲的，缓冲区的大小是 1024 字节。也就是说每输出一行，或者要输出的字符达到了 1024 字节的时候，就会调用一次 write 将数据写入。而在重新定向流到普通文件时，标准输入和标准输出就变成了全缓冲的了，其缓冲区长度为 stat 中得到的 st_blksize 的值。而标准错误流 stderr 不管输出到哪里都是无缓冲的。

### 5.13.c

```shell
$ gcc 5.13.c -lapue
$ ./a.out
trying to create first temp file...
temp name = /tmp/dirhgwSdH
file exists
try to create second temp file...
段错误 (核心已转储)
```

mkstemp 函数会修改对应的字符串，第一种声明方法的数组是在栈上定义的，而第二种声明方法只有指针在栈上，字符串在可执行文件的只读段。因此第二种方式会导致段错误。

### 5.14.c

```shell
$ ./a.out
initial buffer contents:
before flush:
after fflush: hello, world
len of string in buf = 12
after fseek: bbbbbbbbbbbbhello, world
len of string in buf = 24
after fclose: hello, worldcccccccccccccccccccccccccccccccccc
len of string in buf = 46
```

lseek 之前，输出位置在第一个 hello, world 的后面，虽然已经被 memset 为了 'b' ，但是位置还在。

fclose, fflush, fseek, fseeko, fsetpos 等会在当前位置写入一个 null 字节。

## 习题

### 5.1

**用 setvbuf 实现 setbuf 。**  
setbuf 对 pipe , socket , 字符设备和块设备，采用行缓冲；对 stderr 采用无缓冲；对其他类型的流采用全缓冲。

为了带缓冲的进行 I/O ， setbuf 的 buf 参数必须指向一个长度为 BUFSIZ 的缓冲区，如果 buf 被设置为 NULL ，则关闭缓冲。

```shell
$ gcc p1.c -lapue
$ ./a.out
stream = fp, fully buffered, buffer size = 0
stream = fp, unbuffered, buffer size = 1
stream = fp, fully buffered, buffer size = 8192
stream = stderr, unbuffered, buffer size = 0
stream = stderr, unbuffered, buffer size = 1
stream = stdin, fully buffered, buffer size = 0
stream = stdin, line buffered, buffer size = 8192
stream = stdout, line buffered, buffer size = 1024
stream = stdout, line buffered, buffer size = 8192
```

### 5.2

**图 5-5 中的程序利用每次一行 I/O （fgets 和 fputs 函数）复制文件。若将程序中的 MAXLINE 改为 4 ，当复制的行超过该最大值时会出现什么情况？请对此进行解释**  
fgets 函数读入数据，直到行结束或缓冲区满（当然会留出一个字节存放终止 null 字节）。同样， fputs 只负责将缓冲区的内容输出直到遇到一个 null 字节，而不考虑缓冲区中是否包含换行符。所以，如果将 MAXLINE 设得很小，这两个函数仍然会正常工作；只不过在缓冲区较大时，函数被执行的次数要多于 MAXLINE 值设置得较大的时候。如果这些函数删除或添加换行符（如 gets 和 puts 函数的操作），则必需保证对于最长的行，缓冲区也足够大。

### 5.3

**printf 返回 0 值表示什么？**  
printf 函数的返回值是输出的字符的长度，因此返回 0 值代表着输出了空白。

```C
printf("");
```

### 5.4

**下面的代码在一些机器上运行正确，而在另外一些机器运行时出错，解释原因所在**  

```C
#include <stdio.h>

int main() {
    char c;
    while ((c = getchar()) != EOF) {
        putchar(c);
    }
}
```

这是一个比较常见的错误。 getc 以及 getchar 的返回值是 int 类型，而不是 char 类型。由于 EOF 经常定义为 -1 ，那么如果系统使用的是有符号的字符类型，程序可以正常工作。但如果使用的是无符号字符类型，那么返回的 EOF 被保存到字符 c 后将不再是 -1 ，所以，程序会进入死循环。

### 5.5

**对标准 I/O 流如何使用 fsync 函数（见 3.13 节）？**  
fsync 函数只对由文件描述符 fd 指定的一个文件起作用，并且等待写磁盘操作结束才返回。 fsync 可用于数据库这样的应用程序，这种应用程序需要确保修改过的块立即写到磁盘上。

对标准 I/O 流首先调用 fflush ，然后调用 fsync 。 fsync 的参数由 fileno 函数获得。如果不调用 fflush ，所有的数据仍然在内存缓冲区中，此时调用 fsync 将没有任何效果。

### 5.6

**在图 1-7 和图 1-10 程序中，打印的提示信息没有包含换行符，程序也没有调用 fflush 函数，请解释输出提示信息的原因是什么。**  
在标准输入输出流连接终端的时候，这些流是行缓冲的，调用 fgets 的时候标准输出流将自动冲洗。

### 5.7

**基于 BSD 的系统提供了 funopen 的函数调用使我们可以拦截读、写、定位以及关闭一个流的调用。使用这个函数为 FreeBSD 和 MacOS X 实现 fmemopen。**  
不会……标程跑不起来……我怕是已经废了……