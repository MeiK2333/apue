#include <aio.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include "apue.h"

#define BSZ 4096
#define NBUF 8

enum rwop { UNUSED = 0, READ_PENDING = 1, WRITE_PENDING = 2 };

struct buf {
  enum rwop op;
  int last;
  struct aiocb aiocb;
  unsigned char data[BSZ];
};

struct buf bufs[NBUF];

unsigned char translate(unsigned char c) {
  if (isalpha(c)) {
    if (c >= 'n') {
      c -= 13;
    } else if (c >= 'a') {
      c += 13;
    } else if (c >= 'N') {
      c -= 13;
    } else {
      c += 13;
    }
  }
  return (c);
}

int main(int argc, char *argv[]) {
  int ifd, ofd, i, j, n, err, numop;
  struct stat sbuf;
  const struct aiocb *aiolist[NBUF];
  off_t off = 0;

  if (argc != 3) {
    err_quit("usage: rot13 infile outfile");
  }
  /* 打开输入文件和输出文件 */
  if ((ifd = open(argv[1], O_RDONLY)) < 0) {
    err_sys("can't open %s", argv[1]);
  }
  if ((ofd = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, FILE_MODE)) < 0) {
    err_sys("can't open %s", argv[2]);
  }
  /* 获取文件信息(主要是文件大小) */
  if (fstat(ifd, &sbuf) < 0) {
    err_sys("fstat failed");
  }

  for (i = 0; i < NBUF; i++) {
    bufs[i].op = UNUSED;
    bufs[i].aiocb.aio_buf = bufs[i].data;
    /**
     * sigev_notify:
     * - SIGEV_NONE: 异步 I/O 完成后不通知进程
     * - SIGEV_SIGNAL: 异步 I/O 完成后, 产生信号(由 sigev_signo 指定)
     * - SIGEV_THREAD: 异步 I/O 完成后, 调用指定的函数(由 sigev_notify_function 指定, 开启新的线程启动)
     * */
    bufs[i].aiocb.aio_sigevent.sigev_notify = SIGEV_NONE;
    aiolist[i] = NULL;
  }

  numop = 0;
  for (;;) {
    /* 循环设置 NBUF 个 异步 I/O 事件 */
    for (i = 0; i < NBUF; i++) {
      switch (bufs[i].op) {
        /* 若当前状态为 UNUSED */
        case UNUSED:
          /* 若没有全部读取完 */
          if (off < sbuf.st_size) {
            /* 修改状态为 READ_PENDING */
            bufs[i].op = READ_PENDING;
            /* set fildes and offset */
            bufs[i].aiocb.aio_fildes = ifd;
            bufs[i].aiocb.aio_offset = off;
            off += BSZ;
            /* 判断是否为最后一组 */
            if (off >= sbuf.st_size) {
              bufs[i].last = 1;
            }
            bufs[i].aiocb.aio_nbytes = BSZ;
            if (aio_read(&bufs[i].aiocb) < 0) {
              err_sys("aio_read failed");
            }
            aiolist[i] = &bufs[i].aiocb;
            numop++;
          }
          break;
        case READ_PENDING:
          /* 异步事件仍在继续 */
          if ((err = aio_error(&bufs[i].aiocb)) == EINPROGRESS) {
            continue;
          }
          /* 调用失败 */
          if (err != 0) {
            if (err == -1) {
              err_sys("aio_error failed");
            } else {
              err_exit(err, "read failed");
            }
          }

          /* 获取异步 I/O 事件的结果 */
          if ((n = aio_return(&bufs[i].aiocb)) < 0) {
            err_sys("aio_return failed");
          }
          /* 若不是最后一组却没有读取到应有的数据 */
          if (n != BSZ && !bufs[i].last) {
            err_quit("short read (%d/%d)", n, BSZ);
          }
          /* 字符变化 */
          for (j = 0; j < n; j++) {
            bufs[i].data[j] = translate(bufs[i].data[j]);
          }
          /* 设置异步写事件 */
          bufs[i].op = WRITE_PENDING;
          bufs[i].aiocb.aio_fildes = ofd;
          bufs[i].aiocb.aio_nbytes = n;
          if (aio_write(&bufs[i].aiocb) < 0) {
            err_sys("aio_write failed");
          }
          break;

        case WRITE_PENDING:
          /* 基本逻辑同上 */
          if ((err = aio_error(&bufs[i].aiocb)) == EINPROGRESS) {
            continue;
          }
          if (err != 0) {
            if (err == -1) {
              err_sys("aio_error failed");
            } else {
              err_exit(err, "write failed");
            }
          }
          if ((n = aio_return(&bufs[i].aiocb)) < 0) {
            err_sys("aio_return failed");
          }
          if (n != bufs[i].aiocb.aio_nbytes) {
            err_quit("short write (%d/%d)", n, BSZ);
          }
          aiolist[i] = NULL;
          bufs[i].op = UNUSED;
          numop--;
          break;
      }
    }
    if (numop == 0) {
      if (off >= sbuf.st_size) {
        break;
      }
    } else {
      /**
       * 如果 off 已经到达文件末尾
       * 而此时还有未完成的异步事件
       * 则阻塞进程, 直到所有的异步事件完成
       * */
      if (aio_suspend(aiolist, NBUF, NULL) < 0) {
        err_sys("aio_suspend failed");
      }
    }
  }
  bufs[0].aiocb.aio_fildes = ofd;
  /* 强制所有的异步事件不等待而写入持久化的存储中 */
  if (aio_fsync(O_SYNC, &bufs[0].aiocb) < 0) {
    err_sys("aio_fsync failed");
  }
  exit(0);
}