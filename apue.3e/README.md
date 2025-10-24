## 安装
### Ubuntu

```shell
sudo apt-get install libbsd-dev
cd apue.3e
make
......

sudo cp ./include/apue.h /usr/include/
sudo cp ./lib/libapue.a /usr/local/lib/
```

Ubuntu直接make编译会有如下几种报错：

1. dylib问题，报错主要内容如下：

   ```plaintext
   gcc -shared -Wl,-dylib -o libapue_db.so.1 -L../lib -lapue -lc db.o
   /usr/bin/ld: Error: unable to disambiguate: -dylib (did you mean --dylib ?)
   collect2: error: ld returned 1 exit status
   make[1]: *** [Makefile:32：libapue_db.so.1] 错误 1
   ```

   经过定位，问题出现在：`db/Makefile`中，
   
   ```plaintext
   ifeq "$(PLATFORM)" "solaris"
     LDCMD=$(LD) -64 -G -Bdynamic -R/lib/64:/usr/ucblib/sparcv9 -o libapue_db.so.1 -L/lib/64 -L/usr/ucblib/sparcv9 -L$(ROOT)/lib -lapue db.o
     EXTRALD=-m64 -R.
   else
     LDCMD=$(CC) -shared -Wl,-dylib -o libapue_db.so.1 -L$(ROOT)/lib -lapue -lc db.o
   endif
   ```

   将`LDCMD=$(CC) -shared -Wl,-dylib -o libapue_db.so.1 -L$(ROOT)/lib -lapue -lc db.o`中的`-Wl,-dylib`删除即可。

2. 设备号的major，minor问题，显示`undefined reference to 'major', undefined reference to 'minor'`，主要报错如下：

   ```plaintext
   /usr/bin/ld: /tmp/ccAOKQie.o: in function `main':
   devrdev.c:(.text+0xcb): undefined reference to `minor'
   /usr/bin/ld: devrdev.c:(.text+0xe1): undefined reference to `major'
   /usr/bin/ld: devrdev.c:(.text+0x131): undefined reference to `minor'
   /usr/bin/ld: devrdev.c:(.text+0x147): undefined reference to `major'
   collect2: error: ld returned 1 exit status
   make[1]: *** [Makefile:18：devrdev] 错误 1
   ```

   在`include/apue.h`中添加头文件`#include <sys/sysmacros.h>`即可解决。

3. FILE包含字段命名不匹配问题，主要报错如下：

   ```plaintext
   make[1]: 进入目录“/home/zhuang/Projects/apue/apue.3e/stdio”
   gcc -ansi -I../include -Wall -DLINUX -D_GNU_SOURCE  buf.c -o buf  -L../lib -lapue 
   buf.c: In function ‘is_unbuffered’:
   buf.c:90:15: error: ‘FILE’ has no member named ‘__pad’; did you mean ‘__pad5’?
      90 | #define _flag __pad[4]
         |               ^~~~~
   buf.c:98:20: note: in expansion of macro ‘_flag’
      98 |         return(fp->_flag & _IONBF);
         |                    ^~~~~
   buf.c: In function ‘is_linebuffered’:
   buf.c:90:15: error: ‘FILE’ has no member named ‘__pad’; did you mean ‘__pad5’?
      90 | #define _flag __pad[4]
         |               ^~~~~
   buf.c:104:20: note: in expansion of macro ‘_flag’
     104 |         return(fp->_flag & _IOLBF);
         |                    ^~~~~
   buf.c: In function ‘buffer_size’:
   buf.c:92:15: error: ‘FILE’ has no member named ‘__pad’; did you mean ‘__pad5’?
      92 | #define _base __pad[2]
         |               ^~~~~
   buf.c:111:20: note: in expansion of macro ‘_base’
     111 |         return(fp->_base - fp->_ptr);
         |                    ^~~~~
   buf.c:91:14: error: ‘FILE’ has no member named ‘__pad’; did you mean ‘__pad5’?
      91 | #define _ptr __pad[1]
         |              ^~~~~
   ```

   问题出现在`stdio/buf.c`中，解决方法时修改代码最后的三个函数中的fp指向的参数名称，原来代码如下：

   ```c
   int is_unbuffered(FILE *fp) { return (fp->_flag & _IONBF); }
   
   int is_linebuffered(FILE *fp) { return (fp->_flag & _IOLBF); }
   
   int buffer_size(FILE *fp) {
   #ifdef _LP64
     return (fp->_base - fp->_ptr);
   #else
     return (BUFSIZ); /* just a guess */
   #endif
   ```

   将其修改为：

   ```c
   int is_unbuffered(FILE *fp) { return(fp->_flags & _IONBF); }
   
   int is_linebuffered(FILE *fp) { return(fp->_flags & _IOLBF); }
   
   int
   buffer_size(FILE *fp)
   {
   #ifdef _LP64
       return(fp->_IO_buf_end - fp->_IO_buf_base);
   #else
       return(BUFSIZ);    /* just a guess */
   #endif
   }
   ```

经过这3次修改之后再执行：

```c
make clean
make
......

sudo cp ./include/apue.h /usr/include/
sudo cp ./lib/libapue.a /usr/local/lib/
```

如此便可无错编译出`libapue.a`文件了。

### MacOS

```shell
cd apue.3e
make
......

sudo cp ./include/apue.h /usr/local/include/
sudo cp ./lib/libapue.a /usr/local/lib/
```

## 编译

```shell
gcc filename.c -lapue
```