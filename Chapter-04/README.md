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