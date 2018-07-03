# 高级 I/O

## 随书代码

### 14.2.c

```shell
$ gcc 14.2.c -lapue
$ ./a.out < /etc/services > temp.file
......(剩下的其他操作)
```

在我的电脑上, /etc/services 文件并没有 50000 大小, 我减小了 buf 的大小, 却发现程序没有按照预期的那样输出大量错误.