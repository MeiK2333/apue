# 信号

## 随书代码

### 10.3.c

```shell
$ gcc 10.3.c -lapue
$ ./a.out &
[1] 18096
$ kill -USR1 18096
received SIGUSR1
$ kill -USR2 18096
received SIGUSR2
$ kill 18096
```