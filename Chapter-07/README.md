# 进程环境

## 随书代码

### 7.3.c

```shell
$ gcc 7.3.c -lapue
$ ./a.out
main is done
first exit handler
first exit handler
second exit handler
```

exit 调用 atexit 注册的函数的顺序与它们登记时候的顺序相反，同一个函数如果登记多次，那么也会被调用多次。只有自愿的由 exit 结束的进程才会触发这些登记的函数。

### 7.4.c

```shell
$ gcc 7.4.c -o echoarg
$ ./echoarg arg1 TEST foo
argv[0]: ./echoarg
argv[1]: arg1
argv[2]: TEST
argv[3]: foo
```

### 7.7.c

```shell
$ gcc hello1.c -static
$ ls -l a.out
-rwxr-xr-x 1 meik meik 844696 5月  15 13:36 a.out
$ size a.out
   text	   data	    bss	    dec	    hex	filename
 743281	  20876	   5984	 770141	  bc05d	a.out
$ gcc hello1.c
$ ls -l a.out
-rwxr-xr-x 1 meik meik 8296 5月  15 13:36 a.out
$ size a.out
   text	   data	    bss	    dec	    hex	filename
   1515	    600	      8	   2123	    84b	a.out
```

使用共享库编译的程序，可执行文件的正文和数据段的长度都显著减少。

### 7.10.c

```shell
$ gcc 7.10.c -lapue
$ ./a.out
in f1():
globval = 95, autoval = 96, regival = 97, volaval = 98, statval = 99
after longjmp:
globval = 95, autoval = 96, regival = 3, volaval = 98, statval = 99
$ gcc 7.10.c -lapue -O
$ ./a.out
in f1():
globval = 95, autoval = 96, regival = 97, volaval = 98, statval = 99
after longjmp:
globval = 95, autoval = 2, regival = 3, volaval = 98, statval = 99
```

### 7.11.c

```shell
$ gcc 7.11.c -lapue
$ ./a.out
RLIMIT_AS            (infinite) (infinite)
RLIMIT_CORE                   0 (infinite)
RLIMIT_CPU           (infinite) (infinite)
RLIMIT_DATA          (infinite) (infinite)
RLIMIT_FSIZE         (infinite) (infinite)
RLIMIT_MEMLOCK         16777216   16777216
RLIMIT_MSGQUEUE          819200     819200
RLIMIT_NICE                   0          0
RLIMIT_NPROC              30556      30556
RLIMIT_RSS           (infinite) (infinite)
RLIMIT_SIGPENDING         30556      30556
RLIMIT_STACK            8388608 (infinite)
```

## 课后习题

### 7.1

**在 Intel x86 系统上，使用 Linux ，如果执行一个输出 “hello, world” 的程序但不调用 exit 或 return ，则程序的返回代码为 13 （用 shell 检查），解释其原因。**  

```shell
$ gcc p1.c 
$ ./a.out 
hello, world
$ echo $?
13
```

如果 main 函数返回值为 int 的话，那么无论如何返回的都是 0 ；如果 main 函数返回值为 void 的话，那么将会返回 printf 的返回值，也就是输出的字符个数 13 。

### 7.2

**图 7-3 中的 printf 函数的结果何时才被真正输出。**  
当程序处于交互运行方式时，标准输出通常处于行缓冲方式，所以当输出换行符时，上次的结果才被真正输出。如果标准输出被定向到一个文件，而标准输出处于全缓冲方式，则当标准 I/O 清理操作执行时，结果才真正被输出。

### 7.3

**是否有方法不适用（a）参数传递、（b）全局变量这两种方法，将 main 中的参数 argc 和argv 传递给它所调用的其他函数？**  
没有

### 7.4

**在有些 UNIX 系统实现中执行程序时访问不到其数据段的 0 单元，这是一种有意的安排，为什么？**  
当 C 程序解引用一个空指针出错时，执行该程序的进程将终止。可以利用这种方式终止进程。

### 7.5

**用 C 语言的 typedef 为终止处理程序定义了一个新的数据类型 Exitfunc ，使用该类型修改 atexit 的原型。**  
atexit 的原型：

```C
#include <stdlib.h>
int atexit(void (*func)(void));
```

定义如下：

```C
typedef void Exitfunc(void);
int atexit(Exitfunc *func);
```

### 7.6

**如果用 calloc 分配一个 long 型的数组，数组的初始值是否为 0 ？如果用 calloc 分配一个指针数组，数组的初始值是否为空指针？**  
ISO C 说明：

- calloc，为指定数量指定长度的对象分配存储空间。该空间中的每一位（bit）都初始化为 0 。

因此数组的初始值为 0，但是 ISO C 不保证 0 值与空指针的值相同。

### 7.7

**在 7.6 节结尾处 size 命令的输出结果中，为什么没有给出堆和栈的大小？**  
堆和栈的大小根据程序运行时的条件不同，所占用的空间也是不同的。

只有通过 exec 函数执行一个程序时，才会分配堆和栈。

### 7.8

**为什么 7.7 节中两个文件的大小（879443 和 8378）不等于它们各自文本和数据大小的和？**  
可执行文件 a.out 包含了用于调试 core 文件的符号表信息。用 strip 命令可以删除这些信息，对两个 a.out 文件执行这条命令，它们的大小减为 798760 和 6200 字节。

```shell
$ gcc hello1.c -static
$ ls -l a.out
-rwxr-xr-x 1 meik meik 844696 5月  16 22:09 a.out
$ size a.out
   text	   data	    bss	    dec	    hex	filename
 743281	  20876	   5984	 770141	  bc05d	a.out
$ strip a.out
$ ls -l a.out
-rwxr-xr-x 1 meik meik 774384 5月  16 22:10 a.out
$ size a.out
   text	   data	    bss	    dec	    hex	filename
 743281	  20876	   5984	 770141	  bc05d	a.out
```

### 7.9

**为什么 7.7 节中对于一个简单的程序，使用共享库以后其可执行文件的大小变化如此巨大？**  
因为输入输出等操作的标准 I/O 库占用了大部分大小。

### 7.10

**在 7.10 节中我们已经说明为什么不能将一个指针返回给一个自动变量，下面的程序是否正确？**  

```C
int f1(int val) {
  int num = 0;
  int *ptr = &num;
  if (val == 0) {
    int val;
    val = 5;
    ptr = &val;
  }
  return (*ptr + 1);
}
```

不正确，if 语句里面的 val 是一个自动变量，当离开 if 的作用域时，这个变量就不存在了。