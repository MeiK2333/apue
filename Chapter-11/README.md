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

### 11.5.c

```shell
$ gcc 11.5.c -lapue -lpthread
$ ./a.out
thread 1 returning
thread 2 exiting
thread 1 exit code 1
thread 2 exit code 2
```

### 11.5.2.c

```shell
$ gcc 11.5.2.c -lapue -lpthread
$ ./a.out
thread 1:
  structure at 0x7f1daacbaed0
  foo.a = 1
  foo.b = 2
  foo.c = 3
  foo.d = 4
parent starting second thread
thread 2: ID is 139765396256512
partne:
  structure at 0x7f1daacbaed0
  foo.a = -1425393824
  foo.b = 32541
  foo.c = -1427798446
  foo.d = 32541
```

### 11.5.3.c

```shell
$ gcc 11.5.3.c -lapue -lpthread
$ ./a.out
thread 2 start
thread 2 push complete
thread 1 start
thread 1 push complete
thread 1 exit code 1
cleanup: thread 2 second handler
cleanup: thread 2 first handler
thread 2 exit code 2
```

和书上的结果有所不同，但只有线程 2 的清理函数执行了是正确的

### 11.6.3.c

```shell
$ gcc 11.6.3.c -lapue -lpthread
$ ./a.out
mutex is locked
current time is 03:17:49 PM
the time is now 03:17:59 PM
can't lock mvtex again: Connection timed out
```

### 11.6.8.c

```shell
$ gcc 11.6.8.c -lapue -lpthread -lbsd
$ ./a.out
sort took 3.1410 seconds
```

这个程序用到了 bsd 库，需要添加 `-lbsd` 参数。

手动将线程数量改为 1 ，发现多线程确实有明显的性能提升，但在我的电脑上还达不到 6 倍的差距。

## 课后习题

### 11.1

**修改图 11-4 所示的实例代码，正确的在两个线程之前传递结构**  
见程序 [p1.c](p1.c) ，使用动态内存分配代替了自动变量。

### 11.2

**在图 11-14 所示的实例代码中，需要另外添加什么同步（如果需要的话）可以使得主线程改变与挂起作业关联的线程 ID ？这会对 job_remove 函数产生什么影响？**  
要改变挂起作业的线程 ID ，必须持有写模式下的读写锁，防止 ID 在改变过程中有其他线程在搜索该列表。目前定义该接口的方式存在的问题在于：调用 job_find 找到该作业以及调用 job_remove 从列表中删除该作业这两个时间之间作业 ID 可以改动。这个问题可以通过在 job 结构中嵌入引用计数和互斥量，然后让 job_find 增加引用计数的方法来解决。这样修改 ID 的代码就可以避免对列表中非零引用计数的任何作业进行 ID 改动的情况。

### 11.3

**把图 11-15 中的技术运用到工作线程实例（图 11-1 和图 11-14 ）中实现工作线程函数。不要忘记更新 queue_init 函数对条件变量进行初始化，修改 job_insert 和 job_append 函数给工作线程发信号。会出现什么样的困难？**  
首先，列表是由读写锁保护的，但条件变量需要互斥量对条件进行保护。其次，每个线程等待满足的条件应该是有某个作业进行处理时需要的条件，所以需要创建每线程数据结构来表示这个条件。或者，可以把互斥量和条件变量嵌入到 queue 结构中，但这意味着所有的工作线程将等待相同的条件。如果有很多工作线程存在，当唤醒了许多线程但由没有工作可做时，就可能出现*惊群效应*问题，最后导致 CPU 资源的浪费，并且增加了锁的争夺。

### 11.4

**下面哪个步骤序列是正确的？**  
（1） 对互斥量加锁（pthread_mutex_lock）  
（2） 改变互斥量保护的条件  
（3） 给等待条件的线程发信号（pthread_cond_broadcast）  
（4） 对互斥量解锁（pthread_mutex_unlock）  
或者  
（1） 对互斥量加锁（pthread_mutex_lock）  
（2） 改变互斥量保护的条件  
（3） 对互斥量解锁（pthread_mutex_unlock）  
（4） 给等待条件的线程发信号（pthread_cond_broadcast）  

这根据具体情况而定。总的来说，两种情况都可能是正确的，但每一种方法都有不足之处。在第一种情况下，等待线程会被安排在调用 pthread_cond_broadcast 之后运行。如果程序运行在多处理器上，由于还持有互斥锁（pthread_cond_wait 返回持有的互斥锁），一些线程就会运行而且马上阻塞。在第二章情况下，运行线程可以在第 3 步和第 4 步之间获得互斥锁，然后使条件失效，最后释放互斥锁。接着，当调用 pthread_cond_broadcast 时，条件不再为真，线程无需执行。这就是为什么唤醒线程必须重新检查条件，不能仅仅因为 pthread_cond_wait 返回就假定条件就为真。

### 11.5

**实现屏障需要什么原语？给出 pthread_barrier_wait 函数的一个实现**  
母鸡……