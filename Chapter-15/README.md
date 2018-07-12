# 进程间通信

## 随书代码

### 15.2.c

```shell
$ gcc 15.2.c -lapue
$ ./a.out
hello world
```

`pipe` 创建的管道, 1 为写端, 0 为读端

### 15.2.2.c && 15.3.c

pass

### 15.3.1.c && 15.3.2.c

```shell
$ gcc 15.3.1.c -lapue -o myuclc
$ gcc 15.3.2.c -lapue
$ ./a.out
prompt> a
a
prompt> A
a
prompt> ABC
abc
```