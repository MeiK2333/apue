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