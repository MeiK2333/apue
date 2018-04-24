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