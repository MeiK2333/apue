## 文件和目录


### 随书代码


#### 4.3.c

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


#### 4.7.c

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


#### 4.8.c

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


#### 4.9.c

```shell
$ ls -l foo bar
-rw-------  1 meik  staff  0 Apr 24 21:15 bar
-rw-rw-rw-  1 meik  staff  0 Apr 24 21:15 foo
$ ./a.out
$ ls -l foo bar
-rw-r--r--  1 meik  staff  0 Apr 24 21:15 bar
-rw-rwSrw-  1 meik  staff  0 Apr 24 21:15 foo

```


#### 4.10

粘着位，现今的含义已经与最初不同，如果对一个目录设置了粘着位，只有对该目录具有写权限并且满足下列条件之一，才能删除或重命名该目录下的文件：
- 拥有此文件；
- 拥有此目录；
- 是超级用户。

目录 /tmp 和 /var/tmp 是典型的粘着位候选者。


#### 4.15.c

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


#### 4.20.c

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


#### 4.22.c

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


#### 4.23.c

```shell
$ gcc 4.23.c -lapue
$ ./a.out
chdir to /tmp succeeded
```


#### 4.23.2.c

```shell
$ gcc 4.23.2.c -lapue
$ ./a.out
cwd = /private/tmp
```

在 ubuntu 镜像里面也没找到 `/usr/spool/uucppublic` ……


#### 4.24.c

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


### 习题

#### 4.1
**用 stat 函数替换图 4-3 程序中的 lstat 函数，如若命令行参数之一是符号链接，会发生什么变化？**

apue P74: lstat 函数类似于 stat ，但是当命名的文件是一个符号链接时， lstat 返回该符号链接的有关信息，而不是由该符号链接引用的文件的信息。也就是说，如果用 stat 替换 lstat 的话，将无法获取到符号链接的信息，而只能获取到符号链接对应的文件的信息。


#### 4.2
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


#### 4.3
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


#### 4.4
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


#### 4.5
**4.12 节中讲到一个普通文件的大小可以为 0，同时我们又知道 st_size 字段是为目录或符号链接定义的，那么目录和符号链接的长度是否可以为 0 ？**