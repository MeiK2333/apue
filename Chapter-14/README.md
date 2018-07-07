# 高级 I/O

## 随书代码

### 14.2.c

```shell
$ gcc 14.2.c -lapue
$ ./a.out < /etc/services > temp.file
......(剩下的其他操作)
```

在我的电脑上, /etc/services 文件并没有 50000 大小, 我减小了 buf 的大小, 却发现程序没有按照预期的那样输出大量错误.

### 14.3.c

```shell
$ gcc 14.3.c -lapue
$ ./a.out
parent: got the lock, byte 1
child: got the lock, byte 0
parent: writew_lock error: Resource deadlock avoided
child: got the lock, byte 1
```

### 14.3.2.c

```shell
$ gcc 14.3.2.c -lapue
$ ./a.out temp.lock
read_lock of already-locked region erturns 11
read OK (no mandatory locking), buf = ab
```

我的电脑是 Ubuntu 18.04, 看起来并不支持强制性锁.

### 14.5.3.c && 14.5.3.2.c

```shell
$ ll in.txt
-rw-r--r-- 1 meik meik 2600000 7月   5 19:47 in.txt

$ gcc 14.5.3.c -lapue
$ time ./a.out in.txt out.txt

real 0m0.128s
user 0m0.032s
sys  0m0.004s

$ gcc 14.5.3.2.c -lapue -lrt
$ time ./a.out in.txt out.txt

real 0m0.126s
user 0m0.036s
sys  0m0.016s
```

尝试了 26000 / 260000 / 2600000 数据量, 两个程序均没有明显差距

## 课后习题

### 14.1

**编写一个测试程序说明你所用系统在下列情况下的运行情况: 一个进程在试图对一个文件的某个范围加读写锁的时候阻塞, 之后其他进程又提出了一些相关的加锁请求. 试图加锁的进程会不会因此而饿死?**  
我的电脑是 Ubuntu 18.04, 运行结果答案代码结果如下:

```shell
$ gcc p1.c -lapue
$ ./a.out
child 1: obtained read lock on file
child 2: obtained read lock on file
child 3 about to block in write-lock...
parent: obtained additional read lock while write lock is pending
killing child 1...
killing child 2...
killing child 3...child 1: exit after pausechild 2: exit after pause


child 3: can't write-lock file: Interrupted system call
```

### 14.2

**查看你所用的系统的头文件, 并研究 select 和 4 个 FD_ 宏的实现**  
查看 ```sys/select.h``` 头文件的内容

```C
#define __NFDBITS	(8 * (int) sizeof (__fd_mask))
#define	__FD_ELT(d)	((d) / __NFDBITS)
#define	__FD_MASK(d)	((__fd_mask) (1UL << ((d) % __NFDBITS)))

/* Access macros for `fd_set'.  */
#define	FD_SET(fd, fdsetp)	__FD_SET (fd, fdsetp)
#define	FD_CLR(fd, fdsetp)	__FD_CLR (fd, fdsetp)
#define	FD_ISSET(fd, fdsetp)	__FD_ISSET (fd, fdsetp)
#define	FD_ZERO(fdsetp)		__FD_ZERO (fdsetp)

/* 一些其他的内容 */

#define __FD_SET(d, set) \
  ((void) (__FDS_BITS (set)[__FD_ELT (d)] |= __FD_MASK (d)))
#define __FD_CLR(d, set) \
  ((void) (__FDS_BITS (set)[__FD_ELT (d)] &= ~__FD_MASK (d)))
#define __FD_ISSET(d, set) \
  ((__FDS_BITS (set)[__FD_ELT (d)] & __FD_MASK (d)) != 0)
```

大多数系统将数据类型 fd_set 定义为一个只包含一个成员的结构, 该成员为一个长整形数组. 数组中的每一位(bit)对应于一个描述符. 4 个 FD_ 宏通过开, 关或测试指定的位对这个数组进行操作.

将之定义为一个包含数组的结构而不仅仅是一个数组的原因是: 通过 C 语言的赋值语句, 可以使 fd_set 类型的变量互相赋值.

### 14.3

**系统头文件通常对 fd_set 数据类型可以处理的最大描述符数有一个内置的限制, 假设需要将描述符数添加到 2048, 该如何实现?**  
见 apue p752

### 14.4

**比较处理信号量集的函数(见 10.11 节)和处理 fd_set 描述符集的函数, 并比较这两类函数在你系统上的实现**  
下面列出了功能相似的函数

|  |  |
| - | - |
| FD_ZERO | sigemptyset |
| FD_SET | sigaddset |
| FD_CLR | sigdelset |
| FD_ISSET | sigismember |

没有与 sigfillset 相对应的 FD_XXX 函数. 对于信号量集来说, 指向信号量集的指针总是第一个参数, 信号编号是第二个参数. 对于描述符来说, 描述符编号是第一个参数, 指向描述符集的指针是第二个参数.

### 14.5

**用 select 或 poll 实现一个与 sleep 类似的函数 sleep_us, 不同之处是要等待指定的若干微秒. 比较这个函数和 BSD 中的 usleep 函数**  
见 [p5.c](p5.c)

### 14.6

**是否可以利用建议性记录锁来实现图 10-24 中的函数 TELL_WAIT, TELL_PARENT, TELL_CHILD, WAIT_PARENT 和 WAIT_CHILD? 如果可以, 编写这些函数并测试其功能**  
不行. 我们可以使 TELL_WAIT 创建一个临时文件, 其中 1 个字节用作父进程的锁, 另外 1 个字节用作子进程的锁. WAIT_CHILD 使得父进程等待获取子进程字节上的锁, TELL_PARENT 使得子进程释放子进程字节上的锁. 但是问题在于, 调用 fork 会释放所有子进程中的锁, 使得子进程开始运行的时候不具有任何它自己的锁.

### 14.7

**用非阻塞写来确定管道的容量. 将其值与第 2 章的 PIPE_BUF 值进行比较**  
见 [p7.c](p7.c)

```shell
$ gcc p7.c -lapue
$ ./a.out
write ret -1, pipe capacity = 65536
```

### 14.8

**重写图 14-21 中的程序来制作一个过滤器: 从标准输入中读并向标准输出写, 但是要使用异步 I/O 接口. 为了使之能正常工作, 你都需要修改些什么? 记住, 无论你的标准输出被连接到终端, 管道还是一个普通文件, 都应该得到相同的结果**  
首先需要修改输入输出, 使其从标准输入读取, 写入标准输出

修改结束条件, 从到达文件末尾改为接收到 ```EOF```

每次输出完成后冲洗输出流, 保证相同的结果

```shell
$ echo 'Hello World' > test.txt
$ ./a.out
Hello World
Uryyb Jbeyq
$ ./a.out < test.txt
Uryyb Jbeyq
$ ./a.out < test.txt > test.out
$ cat test.out
Uryyb Jbeyq
```

### 14.9

**回忆图 14-23, 在你的系统上找到一个损益平衡点, 从此点开始, 使用 writev 将快于你自己使用单个 write 复制数据**  
待续

### 14.10

**运行图 14-27 中的程序复制一个文件, 检查输入文件的上一次访问时间是否更新了?**  
图 14-27 中的程序是否更新输入文件的上一次访问时间依赖于操作系统以及文件所属的文件类型的类型. 在所有的 4 中平台中, 当文件具有给定操作系统默认的文件系统类型, 上一次访问时间就会更新.

### 14.11

**在图 14-27 的程序中, 在调用 mmap 后调用 close 关闭输入文件, 以验证关闭描述符不会是内存映射 I/O 失效**  
见 [p11.c](p11.c), close 之后, 文件复制依旧成功了

```shell
$ gcc p11.c -lapue
$ echo 'Hello World' > hello.txt
$ ll hello.txt
-rw-r--r-- 1 meik meik 12 7月   7 16:54 hello.txt
$ ./a.out hello.txt hello2.txt
$ ll hello2.txt
-rw-r--r-- 1 meik meik 12 7月   7 16:54 hello2.txt
```
