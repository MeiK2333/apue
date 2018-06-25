# 文件和目录

## 随书代码

### 4.3.c

```shell
$ ./a.out /etc/passwd /etc /dev/log /dev/tty /var/lib/oprofile/opd_pipe /dev/sr0 /dev/cdrom
/etc/passwd: regular
/etc: symbolic link
/dev/log: lstat error: No such file or directory
/dev/tty: character special
/var/lib/oprofile/opd_pipe: lstat error: No such file or directory
/dev/sr0: lstat error: No such file or directory
/dev/cdrom: lstat error: No such file or directory
```

意思到了就行……我是 MacOS ，好多文件都没有，我也没能找到这些在 MacOS 下对应的文件。

### 4.7.c

MacOS 下没有 /etc/shadow 文件，因此我创建 hello 文件来进行测试，结果如下

```shell
$ ls -l a.out
-rwxr-xr-x  1 meik  staff  13644 Apr 24 20:47 a.out
$ ./a.out a.out
read access OK
open for reading OK
$ ls -l hello
-r--------  1 root  staff  13 Apr 24 20:52 hello
$ ./a.out hello
access error for hello: Permission denied
open error for hello: Permission denied
$ sudo chown root a.out
$ sudo chmod u+s a.out
$ ls -l a.out
-rwsr-xr-x  1 root  staff  13644 Apr 24 20:47 a.out
$ ./a.out hello
access error for hello: Permission denied
open for reading OK
```

尽管 open 函数能打开文件，但通过设置用户 ID 程序可以确定实际用户不能正常读指定的文件。

### 4.8.c

```shell
$ umask
0022
$ ./a.out
$ ls -l foo bar
-rw-------  1 meik  staff  0 Apr 24 21:03 bar
-rw-rw-rw-  1 meik  staff  0 Apr 24 21:03 foo
$ umask
0022
$ echo 'Hello World' > hello
$ ls -l hello
-rw-r--r--  1 meik  staff  12 Apr 24 21:07 hello

$ umask -S
u=rwx,g=rx,o=rx
$ umask 027
$ umask -S
u=rwx,g=rx,o=
$ umask 022
```

创建屏蔽字设置会让进程创建文件时默认屏蔽掉某些权限，可以通过 umask 函数手动更改。

### 4.9.c

```shell
$ ls -l foo bar
-rw-------  1 meik  staff  0 Apr 24 21:15 bar
-rw-rw-rw-  1 meik  staff  0 Apr 24 21:15 foo
$ ./a.out
$ ls -l foo bar
-rw-r--r--  1 meik  staff  0 Apr 24 21:15 bar
-rw-rwSrw-  1 meik  staff  0 Apr 24 21:15 foo

```

### 4.10

粘着位，现今的含义已经与最初不同，如果对一个目录设置了粘着位，只有对该目录具有写权限并且满足下列条件之一，才能删除或重命名该目录下的文件：

- 拥有此文件；
- 拥有此目录；
- 是超级用户。

目录 /tmp 和 /var/tmp 是典型的粘着位候选者。

### 4.15.c

首先创建一个 tempfile 文件。

```shell
$ ls -l template
-rw-r--r--  1 meik  staff  31000 Apr 25 21:30 tempfile
$ df /home
Filesystem    512-blocks Used Available Capacity iused ifree %iused  Mounted on
map auto_home          0    0         0   100%       0     0  100%   /home
$ ./a.out &
[1] 20238
file unlinked
done
```

意思到了就行……

### 4.20.c

```shell
# ls -l 4.3.c 4.7.c
-rw-r--r-- 1 root root 912 Apr 23 13:47 4.3.c
-rw-r--r-- 1 root root 449 Apr 24 12:47 4.7.c
# ls -lu 4.3.c 4.7.c
-rw-r--r-- 1 root root 912 Apr 23 14:28 4.3.c
-rw-r--r-- 1 root root 449 Apr 24 13:30 4.7.c
# date
Sat Apr 28 13:48:41 UTC 2018
# ./a.out 4.3.c 4.7.c
# ls -lu 4.3.c 4.7.c
-rw-r--r-- 1 root root 0 Apr 23 14:28 4.3.c
-rw-r--r-- 1 root root 0 Apr 24 13:30 4.7.c
# ls -l 4.3.c 4.7.c
-rw-r--r-- 1 root root 0 Apr 23 13:47 4.3.c
-rw-r--r-- 1 root root 0 Apr 24 12:47 4.7.c
# ls -lc 4.3.c 4.7.c
-rw-r--r-- 1 root root 0 Apr 28 13:48 4.3.c
-rw-r--r-- 1 root root 0 Apr 28 13:48 4.7.c
# ls -l 4.3.c 4.7.c
-rw-r--r-- 1 root root 0 Apr 23 13:47 4.3.c
-rw-r--r-- 1 root root 0 Apr 24 12:47 4.7.c
```

比较坑的地方是，在 MacOS 下这个程序是编译不过的，我开了个 docker 来编译运行它……

### 4.22.c

```shell
$ gcc 4.22.c -lapue
$ ./a.out .
regular files  =       9, 90.00 %
directories    =       1, 10.00 %
block special  =       0,  0.00 %
char special   =       0,  0.00 %
FIFOs          =       0,  0.00 %
symbolic links =       0,  0.00 %
sockets        =       0,  0.00 %
$ ./a.out ..
regular files  =    1039, 67.08 %
directories    =     303, 19.56 %
block special  =       0,  0.00 %
char special   =       0,  0.00 %
FIFOs          =       0,  0.00 %
symbolic links =     207, 13.36 %
sockets        =       0,  0.00 %
```

### 4.23.c

```shell
$ gcc 4.23.c -lapue
$ ./a.out
chdir to /tmp succeeded
```

### 4.23.2.c

```shell
$ gcc 4.23.2.c -lapue
$ ./a.out
cwd = /private/tmp
```

在 ubuntu 镜像里面也没找到 `/usr/spool/uucppublic` ……

### 4.24.c

```shell
$ gcc 4.24.c -lapue
$ ./a.out / /home/meik /dev/tty[01]
/: dev = 0/2055
/home/meik: dev = 0/2055
/dev/tty0: dev = 0/6 (character) rdev = 0/1024
/dev/tty1: dev = 0/6 (character) rdev = 0/1025
```

编译时提示没有找到 major 和 minor 的定义，我在网上找到了一段代码以解决这个问题。

```C
#define MINORBITS        20
#define MINORMASK        ((1U << MINORBITS) - 1)
#define MAJOR(dev)        ((unsigned int) ((dev) >> MINORBITS))
#define MINOR(dev)        ((unsigned int) ((dev) & MINORMASK))
#define MKDEV(ma,mi)      (((ma) << MINORBITS) | (mi))
```

## 习题

### 4.1

**用 stat 函数替换图 4-3 程序中的 lstat 函数，如若命令行参数之一是符号链接，会发生什么变化？**  
apue P74: lstat 函数类似于 stat ，但是当命名的文件是一个符号链接时， lstat 返回该符号链接的有关信息，而不是由该符号链接引用的文件的信息。也就是说，如果用 stat 替换 lstat 的话，将无法获取到符号链接的信息，而只能获取到符号链接对应的文件的信息。

### 4.2

**如果文件模式创建屏蔽字是 777 （八进制），结果会怎样？用 shell 的 umask 命令验证该结果**  

```shell
$ umask
0022
$ umask -S
u=rwx,g=rx,o=rx
$ umask 777
$ umask -S
u=,g=,o=
$ echo 'Hello World!' > hello
$ ls -l hello
---------- 1 meik meik 13 5月   2 11:22 hello
$ cat hello
cat: hello: 权限不够
```

会导致新建的文件没有任何权限

### 4.3

**关闭一个你所拥有文件的用户读权限，将导致拒绝你访问自己的文件，对此进行验证。**  

```shell
$ echo 'Hello World!' > hello
$ ls -l hello 
-rw-r--r-- 1 meik meik 13 5月   2 11:28 hello
$ chmod 244 hello 
$ cat hello 
cat: hello: 权限不够
$ ls -l hello 
--w-r--r-- 1 meik meik 13 5月   2 11:28 hello
```

### 4.4

**创建文件 foo 和 bar 后，运行图 4-9 的程序，将发生什么情况？**  

```shell
$ gcc 4.8.c -lapue
$ echo 'Hello World' > foo
$ echo 'Hello World' > bar
$ ./a.out 
$ ls -l foo bar
-rw-r--r-- 1 meik meik 0 5月   2 11:32 bar
-rw-r--r-- 1 meik meik 0 5月   2 11:32 foo
```

文件被重置为新的，但是权限位却没有变化。

### 4.5

**4.12 节中讲到一个普通文件的大小可以为 0，同时我们又知道 st_size 字段是为目录或符号链接定义的，那么目录和符号链接的长度是否可以为 0 ？**  
因为目录中总是包括 . 和 .. ，因此目录的长度不可能为 0 。

符号链接的长度是其指向的文件的文件名长度。

### 4.6

**编写一个类似 cp(1) 的程序，它复制包含空洞的文件，但不将字节 0 写到输出文件中去。**  
如 [p6.c](p6.c) ，遇到空洞时 lseek 跳过，直到非空洞为止。

### 4.7

**在 4.12 节 ls 命令的输出中， core 和 core.copy 的访问权限不同，如果创建两个文件时 umask 没有变，说明为什么会出现这种差别。**  
通过 cat 命令复制文件，使用的是 cat 程序的 umask ，和 shell 的 umask 不同。

### 4.8

**在运行图 4-16 的程序时，使用了 df(1) 命令来检查空闲的磁盘空间，为什么不使用 du(1) 命令？**  
du 命令查看的是磁盘占用空间的大小，创建的新文件并没有写入内容，所以占用磁盘空间大小为 0 。  
df 命令查看的是磁盘最大可用空间，即以使用数据块为单位进行统计，尽管新文件没有写入内容，但是占据了一定数量的数据块。

### 4.9

**图 4-20 中显示 unlink 函数会修改文件状态更改时间，这是怎样发生的？**  
如果删除的链接不是该文件的最后一个链接，则不会删除该文件，状态被更新。

如果被删除的链接是最后一个链接，则该文件被物理删除。

i 节点中存储了文件的引用次数，修改文件引用会修改 i 节点，从而修改文件状态更改时间。

### 4.10

**4.22 节中，系统对可打开文件数的限制对 myftw 函数会产生什么影响？**  
myftw 函数中使用 opendir 来打开文件夹，可打开文件数的限制限制了可以便利的文件夹的个数与深度。

### 4.11

**在 4.22 节中的 myftw 从不改变其目录，对这种处理方法进行改动：每次遇到一个目录就用其调用 chdir ，这样每次调用 lstat 时就可以使用文件名而非路径名，处理完所有的目录项后执行 chdir("..") ，比较这种版本的程序和书中程序的运行时间。**  

```shell
gcc p11.c -lapue
$ time ./a.out ~
can't read directory dconf: Permission denied
regular files   =  259331, 92.75 %
directories     =   16500,  5.90 %
block special   =       0,  0.00 %
char special    =       0,  0.00 %
FIFOs           =       1,  0.00 %
symbolic links  =    3763,  1.35 %
sockets         =       0,  0.00 %
test4_11 costs : 0.361990 second

real    0m0.493s
user    0m0.040s
sys     0m0.323s
$ gcc 4.22.c -lapue
$ time ./a.out ~
can't read directory /home/meik/.cache/dconf/: Permission denied
regular files  =  259331, 92.75 %
directories    =   16500,  5.90 %
block special  =       0,  0.00 %
char special   =       0,  0.00 %
FIFOs          =       1,  0.00 %
symbolic links =    3763,  1.35 %
sockets        =       0,  0.00 %

real    0m0.420s
user    0m0.040s
sys     0m0.378s
```

通过 dconf 可以看出两者读取方式的区别，但是时间没有较大差异……感觉可能是我哪里弄错了

### 4.12

**每个进程都有一个根目录用于解析绝对路径名，可以通过 chroot 函数改变根目录。在手册中查阅此函数。说明这个函数什么时候有用。**  
chroot函数被因特网文件传输协议（Internet File Transfer Protocol, FTP）程序用于辅助安全性。系统中没有账户的用户（也称匿名FTP）放在一个单独的目录下，利用chroot讲此目录当做新的根目录，就可以阻止用户访问此目录以外的文件。
chroot也用于另一台机器上构造一个文件系统层次结构的副本，然后修改此副本，不会更改原来的文件系统。这可用于测试新软件包的安装。
chroot只能由超级用户执行，一旦更改了一个进程的根，该进程及后代进程就再也不能恢复至原先的根。

### 4.13

**如何只设置两个时间值中的一个来使用 utimes 函数？**  
首先使用 stat 来获取当前时间值，然后修改将要改变的值，重新赋值。

### 4.14

**有些版本的 finger(1) 命令输出 “New mail received ...”和“unread since ...”，其中 ... 表示相应的日期和时间。程序是如何决定这些日期和时间的。**  
finger(1)对邮箱调用stat函数，最近一次的修改时间是上一次接收邮件的时间，最近一次访问时间是上一次读邮件的时间。

### 4.16

**UNIX 系统对目录树的深度有限制吗？编写一个程序循环，在每次循环中，创建目录，并将该目录更改为工作目录。确保叶节点的绝对路径名的长度大于系统的 PATH_MAX 限制。可以调用 getcwd 得到目录的路径名吗？标准 UNIX 系统工具是如何处理长路径名的？对目录可以使用 tar 或 cpio 命令归档吗？**  
程序 [p16.c](p16.c) ，创建文件夹失败。不能用 getcwd 获得路径名。

标准答案：我写的这个程序是有问题的，可以参考 apue.3e 中的代码。

### 4.17

**3.16 节中描述了 /dev/fd 特征。如果每个用户都可以访问这些文件，则其访问权限必须为 rw-rw-rw- 。有些程序创建输出文件时，先删除该文件以确保该文件名不存在，忽略返回值。**  

```C
unlink(path);
if ((fd = creat(path, FILE_MODE)) < 0)
    err_sys("...");
```

**如果 path 是 /dev/fd/1 ，会出现什么情况。**  
程序 [p17.c](p17.c) ，因为 /dev 目录关闭了其他用户的写权限，因此 unlink 会失败。