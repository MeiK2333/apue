# 线程控制

## 随书代码

### 12.8.c

```shell
$ gcc 12.8.c -lapue -lpthread
$ ./a.out
^C
interrupt
^C
interrupt
^C
interrupt
^C
interrupt
^\
```

### 12.9.c

```shell
$ gcc 12.9.c -lapue -lpthread
$ ./a.out
thread started...
parent about to fork...
preparing locks...
parent unlocking locks...
parent returned from fork
child unlocking locks...
child returned from fork
```