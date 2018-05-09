## 标准 I/O 库


### 随书代码


#### 5.8.c && 5.8.2.c

```shell
$ gcc 5.8.2.c -lapue
$ ./a.out 
Hello World!
Hello World!
^C
$ gcc 5.8.c -lapue
$ ./a.out 
Hello World!
Hello World!
```

书中指出：标准 I/O 库与直接调用 read 和 write 函数相比并不慢很多。