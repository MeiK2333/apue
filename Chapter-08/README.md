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