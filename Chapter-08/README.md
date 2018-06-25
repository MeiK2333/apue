# 进程控制

## 随书代码

### 8.3.c

```shell
$ gcc 8.3.c -lapue
$ ./a.out 
a write to stdout
before fork
pid = 7764, glob = 7, var = 89
pid = 7763, glob = 6, var = 88
$ ./a.out > temp.out
$ cat temp.out 
a write to stdout
before fork
pid = 8076, glob = 7, var = 89
before fork
pid = 8075, glob = 6, var = 88
```

子进程拥有父进程的数据的副本，但是子进程对数据的修改不会影响到父进程。子进程与父进程共享正文段。

当输出到文件时，因为 stdout 是全缓冲的，而在输出的时候没有输出末尾的 null ，因此 "a write to stdout\n" 留在了缓冲区中，然后被子进程拷贝了一份。在子进程结束之前，流冲洗，导致这个字符串又被输出了一遍。而当输出到控制台时，因为是行缓冲的，在遇到换行符时输出了，因此没有这样的问题。

### 8.4.c

```shell
$ gcc 8.4.c -lapue
$ ./a.out
before vfork
pid = 22413, glob = 7, car = 89
```

vfork 与 fork 不同的是： vfork 不会将父进程的地址空间完全复制过来，在子进程调用 exec 或者 exit 之前，它在父进程的空间中运行。并且 vfork 会保证子进程在父进程之前运行，直到子进程触发了 exec 或者 exit 。

从这个程序中可以明显的看出来，在子进程中做的修改操作在父进程中依然有效。

### 8.6.c

```shell
$ gcc 8.6.c -lapue
$ ./a.out
normal termination, exit status = 7
abnormal termination, signal number = 6  (core file generated)
abnormal termination, signal number = 8  (core file generated)
```

### 8.6.2.c

```shell
$ ./a.out
$ second child, parent pid = 3266
```

在 Ubuntu 的图形界面下执行这个程序，结果不为 1 ，查询 3266 这个程序，发现是 ```3266 ? Ss 0:00 /lib/systemd/systemd --user```。

按 ```ctrl + alt + F3``` 切换到终端登录，再次执行这个程序。

```shell
$ ./a.out
$ second child, parent pid = 1
```

得到了预想的结果。

### 8.9.c

```shell
$ gcc 8.9.c -lapue
$ ./a.out
output from poaurtepnutt
 from child
$ ./a.out
output from paroeunttp
ut from child
$ ./a.out
output from parent
output from child
$ ./a.out 
output from parenotu
tput from child
$ ./a.out
output from parent
output from child
$ ./a.out
output from parent
output from child
$ ./a.out
output from parent
output from child
$ ./a.out
output from pareonutt
put from child
```

### 8.9.2.c

```shell
$ gcc 8.9.2.c -lapue
$ ./a.out
output from parent
output from child
$ ./a.out
output from parent
output from child
$ ./a.out 
output from parent
output from child
```

### 8.9.3.c

```shell
$ gcc 8.9.3.c -lapue
$ ./a.out
output from child
output from parent
$ ./a.out
output from child
output from parent
$ ./a.out
output from child
output from parent
```

通过种方式，能确保父进程和子进程之间的先后关系。

### 8.10.c

```shell
$ gcc 8.10.c -lapue
$ ./a.out
argv[0]: echoall
argv[1]: myarg1
argv[2]: MY ARG2
USER=unknown
PATH=/tmp
$ argv[0]: echoall
argv[1]: only 1 arg
(中间省略若干行)
OLDPWD=/home/meik/apue
_=./a.out
```

我对书上的代码进行了部分改动，使其能够正常运行。

shell 提示符在第二个 exec 打印 argv[0] 之前，因为父进程不等待子进程结束。

### 8.12.sh

```shell
$ chmod 754 8.12.sh
$ ./8.12.sh
hello world ./8.12.sh
```

我写了另一个文件来验证解释器文件。

### 8.16.c

```shell
$ sudo docker run -it --rm -v /home/meik/apue:/apue --cpuset-cpus="1" ubuntu

(docker)$ ./a.out 20
NZERO = 20
current nice value in parent is 20
current nice value in child is 20, adjusting by 20
now child nice value is 39
parent count = 530972868
child count = 7997025

(docker)$ ./a.out
NZERO = 20
current nice value in parent is 20
current nice value in child is 20, adjusting by 0
now child nice value is 20
parent count = 269416766
child count = 268210473

(docker)$ ./a.out 20
NZERO = 20
current nice value in parent is 20
current nice value in child is 20, adjusting by 20
now child nice value is 39
parent count = 530972868
child count = 7997025
```

在多核的机器上，因为两个进程无需共享 CPU ，因此其实看不出来差距。我启动了一个只有一个 CPU 核心的 docker ，成功的体现出差异。

### 8.17.c

```shell
$ gcc 8.17.c -lapue
$ ./a.out "sleep 5" "date" "man bash >/dev/null"

command: sleep 5
  real:     5.10
  user:     0.00
  sys:      0.00
  child user:     0.00
  child sys:      0.00
normal termination, exit status = 0

command: date
2018年 05月 27日 星期日 16:52:15 CST
  real:     0.00
  user:     0.00
  sys:      0.00
  child user:     0.00
  child sys:      0.00
normal termination, exit status = 0

command: man bash >/dev/null
  real:     0.58
  user:     0.00
  sys:      0.00
  child user:     0.21
  child sys:      0.07
normal termination, exit status = 0
```

## 课后习题

### 8.1

**在图 8-3 程序中，如果用 exit 调用代替 _exit 调用，那么可能会使标准输出关闭，使 printf 返回 -1 。修改该程序以验证在那你所使用的系统上是否会产生此种效果。如果并非如此，你怎样处理才能得到类似结果呢？**  
如果直接将 _exit 替换为 exit ，则不会有任何变化。其原因就和书上写的一样 —— exit 不再自找麻烦的去关闭所有的文件描述符。

而如果使用 fclose 关闭了 stdout 文件描述符，则不会输出任何东西。

### 8.2

**回忆图 7-6 中典型的存储空间布局。由于对应与每个函数调用的栈帧通常存储在栈中，并且由于调用 vfork 后，子进程运行在父进程的地址空间中，如果不是在 main 函数中而是在另一个函数中调用 vfork ，此后子进程又从该函数返回，将会发生什么？请编写一段测试程序对此进行验证，并且画图说明发生了什么。**  
如程序 p2 ，程序会运行出错。

子程序从 f1 返回，然后执行 f2 ，此时 f2 的栈帧覆盖了之前 f1 的栈帧。子进程运行结束后，父进程继续从 f1 返回，但此时 f1 的栈帧已经被修改了。

### 8.3

**重写 8-6 中的程序，把 wait 换成 waitid ，不调用 pr_exit ，而从 siginfo 结构中确定等价的信息。**  
如程序 p3 ，反正就这个意思……

### 8.4.c

**当用 ```$ ./a.out``` 执行图 8-13 中的程序一次时，其输出是正确的。但是若将该程序按下列方式执行多次，则其输出不正确。**  

```shell
$ ./a.out ; ./a.out ; ./a.out
output from parent
ooutput from parent
ouotuptut from child
put from parant
output from child
utput from child
```

**原因是什么？怎样才能更正此类错误？如果使子进程首先输出，还会发生此问题吗？**  
在图 8-13 中，我们先让父进程输出，但是当父进程输出完毕子进程要输出时，要让父进程终止。是父进程先终止还是子进程先执行输出，要依赖于内核对两个进程的调度（另一个竞争条件）。在父进程终止后， shell 会开始执行下一个程序，它也行会干扰子进程的输出。为了避免这种情况，要在子进程完成后才终止父进程。

修改后的程序 p4 可以解决这个问题。

### 8.5

**在图 8-20 所示的程序中，调用 execl ，指定 pathname 为解释器文件。如果将其改为调用 execlp ，指定 testinterp 的 filename ，并且如果目录 /home/sar/bin 是路径前缀，则运行该程序时， argv[2] 的打印输出是什么？**  
对 argv[2] 打印的是相同的值（/home/sar/bin/testinterp）。原因是 execlp 在结束时调用了 execve ，并且与直接调用 execl 的路径名相同。

### 8.6

**编写一段程序创建一个僵死进程，然后调用 system 执行 ps 命令以验证该进程是僵死进程。**  

```shell
$ gcc p6.c -lapue
$ ./a.out
  PID  PPID S TT       COMMAND
 7406  7399 S pts/0    bash
29343  7406 S pts/0    ./a.out
29344 29343 Z pts/0    [a.out] <defunct>
29345 29343 S pts/0    sh -c ps -o pid,ppid,state,tty,command
29346 29345 R pts/0    ps -o pid,ppid,state,tty,command
```

### 8.7

**8.10 节中提及 POSIX.1 要求在 exec 时关闭目录流。按下列方法对此进行验证：对根目录调用 opendir ，查看在你系统上实现的 DIR 结构，然后打印执行时关闭标志。接着打开同一目录读并打印执行时关闭标志。**  