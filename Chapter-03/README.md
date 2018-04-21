## 文件 I/O


### 随书代码


#### 3.6.c

```shell
$ gcc 3.6.c -lapue
$ ./a.out
seek OK
$ cat < /etc/passwd | ./a.out
cannot seek
```

如果文件描述符指向的是一个管道、 FIFO 或网络套接字，则 lseek 返回 -1

因为偏移量可能为负值，因此在比较 lseek 的返回值的时候，不应该测试它是否小于 0 ，而是应该测试它是否等于 -1



#### 3.6.2.c

```shell
$ gcc 3.6.2.c -lapue
$ ./a.out
$ ls -l file.hole
-rw-r--r--  1 meik  staff  16394 Apr 21 21:27 file.hole
$ od -c file.hole
0000000    a   b   c   d   e   f   g   h   i   j  \0  \0  \0  \0  \0  \0
0000020   \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0
*
0040000    A   B   C   D   E   F   G   H   I   J
0040012
$ ls -ls file.hole
40 -rw-r--r--  1 meik  staff  16394 Apr 21 21:32 file.hole
```

在我的系统（ MacOS ）下，占用磁盘块的个数与文件大小正相关，并没有因为空洞的存在而有改变，原因不明……


#### 3.9.c

创建了一个大小为 516 581 760 字节的文件，实际测试代码

````C
#define BUFFSIZE 4096
```

```shell
$ gcc 3.9.c -lapue
$ time ./a.out < file.hole > /dev/null

real    0m0.428s
user    0m0.128s
sys     0m0.283s

```

---

````C
#define BUFFSIZE 64
```

```shell
$ gcc 3.9.c -lapue
$ time ./a.out < file.hole > /dev/null

real    0m21.694s
user    0m7.867s
sys     0m11.943s

```

其余的没有测试，从这两个就已经能够明显的看出差别了