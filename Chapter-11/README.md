# 线程

## 随书代码

### 11.4.c

```shell
$ gcc 11.4.c -lapue -lpthread
$ ./a.out
main thread: pid 21744 tid 140060521072448 (0x7f62619b0740)
new thread:  pid 21744 tid 140060512610048 (0x7f626119e700)
```

注意： Linux 下，使用 pthread 库需要链接静态库 libpthread.a ，因此编译选项需要添加 `-lpthread`