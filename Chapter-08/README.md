## 进程控制


### 随书代码


#### 8.3.c
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


#### 8.4.c
```shell
$ gcc 8.4.c -lapue
$ ./a.out 
before vfork
pid = 22413, glob = 7, car = 89
```
vfork 与 fork 不同的是： vfork 不会将父进程的地址空间完全复制过来，在子进程调用 exec 或者 exit 之前，它在父进程的空间中运行。并且 vfork 会保证子进程在父进程之前运行，直到子进程触发了 exec 或者 exit 。

从这个程序中可以明显的看出来，在子进程中做的修改操作在父进程中依然有效。


#### 8.6.c
```shell
$ gcc 8.6.c -lapue
$ ./a.out 
normal termination, exit status = 7
abnormal termination, signal number = 6  (core file generated)
abnormal termination, signal number = 8  (core file generated)
```


#### 8.6.2.c
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


#### 8.9.c
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


#### 8.9.2.c
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


#### 8.9.3.c
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


#### 8.10.c
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


#### 8.12.sh
```shell
$ chmod 754 8.12.sh
$ ./8.12.sh
hello world ./8.12.sh
```

我写了另一个文件来验证解释器文件。


#### 8.16.c
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