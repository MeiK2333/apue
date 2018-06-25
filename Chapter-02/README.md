# UNIX 标准及实现

## 随书代码

### 2.5.1.c

<limits.h> 中定义的整型值大小

```shell
$ gcc 2.5.1.c
$ ./a.out
CHAR_BIT: 8
CHAR_MAX: 127
CHAR_MIN: -128
SCHAR_MAX: 127
SCHAR_MIN: -128
INT_MAX: 2147483647
INT_MIN: -2147483648
UINT_MAX: -1
SHRT_MAX: 32767
SHRT_MIN: -32768
USHRT_MAX: 65535
LONG_MAX: 9223372036854775807
LONG_MIN: -9223372036854775808
ULONG_MAX: 18446744073709551615
LLONG_MAX: 9223372036854775807
LLONG_MIN: -9223372036854775808
ULLONG_MAX: 18446744073709551615
MB_LEN_MAX: 6

```

### 2.5.4.awk && 2.5.4.c

```shell
$
$ awk -f 2.5.4.awk > conf.c
$ gcc conf.c -lapue

```

[conf.c](conf.c)

### 2.5.5.2.c

```shell
$ gcc 2.5.5.2.c -lapue
$ ./a.out
10240
```

## 习题

### 2.1

**在 2.8 节中提到一些基本系统数据类型可以在多个头文件中定义。例如，在 FreeBSD 8.0 中，size_t 在29 个不同的头文件中都有定义。由于一个程序可能包含这 29 个不同的头文件，但是 ISO C 却不允许对同一个名字进行多次 typedef ，那么如何编写这些头文件呢？**  
使用宏定义保护。

```c
#ifndef _MACHINE_TYPES_H_
#define _MACHINE_TYPES_H_

typedef int _int32_t;
typedef unsigned int _uint32_t;

.........

typedef _uint32_t _size_t;

.........

#enddef

```

### 2.2

**检查系统的头文件，列出实现基本系统数据类型所用到的实际数据类型**  
待续

### 2.3

**改写图2-17中的程序，使其在 ```sysconf``` 为 ```OPEN_MAX``` 限制返回 ```LONG_MAX``` 时，避免进行不必要的处理**  
在 ```sysconf``` 没有能够准确获得 ```OPEN_MAX``` 限制的时候，可以使用 ```getrlimit``` 来获得。

```C
long open_max()
{
    struct rlimit rl;
    if (openmax == 0){
        errno = 0;

    if ((openmax = sysconf(_SC_OPEN_MAX)) < 0 || openmax == LONG_MAX) {
            if ((openmax = getrlimit(RLIMIT_NOFILE, &rl)) < 0) {
                err_sys("can not get file limit\n");
            } else if (openmax == RLIM_INFINITY)
                openmax = OPEN_MAX_GUESS;
            else
                openmax = rl.rlim_max;
        }
    }
    return(openmax);
}

```