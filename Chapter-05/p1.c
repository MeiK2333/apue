#include <stdio.h>
#include <string.h>
#include "apue.h"

void pr_stdio(const char *, FILE *);
int is_unbuffered(FILE *);
int is_linebuffered(FILE *);
int buffer_size(FILE *);
void my_setbuf(FILE *restrict fp, char *restrict buf);

int main() {
  FILE *fp = fopen("test", "w");
  char buf[BUFSIZ];
  pr_stdio("fp", fp);
  my_setbuf(fp, NULL);
  pr_stdio("fp", fp);
  my_setbuf(fp, buf);
  pr_stdio("fp", fp);
  pr_stdio("stderr", stderr);
  my_setbuf(stderr, buf);
  pr_stdio("stderr", stderr);
  pr_stdio("stdin", stdin);
  my_setbuf(stdin, buf);
  pr_stdio("stdin", stdin);
  pr_stdio("stdout", stdout);
  my_setbuf(stdout, buf);
  pr_stdio("stdout", stdout);
  return 0;
}

void my_setbuf(FILE *restrict fp, char *restrict buf) {
  int len = BUFSIZ;
  /* 如果 buf 为 NULL 或者为错误流的话，将缓冲类型改为无缓冲 */
  if (buf == NULL || fp == stderr) {
    if (setvbuf(fp, NULL, _IONBF, 0) != 0) {
      err_sys("setvbuf _IONBF failure");
    }
    return;
  }
  int file = fileno(fp);
  struct stat st;
  if (fstat(file, &st) == -1) {
    err_sys("fstat failure");
  } else if (S_ISFIFO(st.st_mode) || S_ISSOCK(st.st_mode) ||
             S_ISCHR(st.st_mode) || S_ISBLK(st.st_mode)) {
    /* pipe , socket ,字符设备和块设备采用行缓冲 */
    if (setvbuf(fp, buf, _IOLBF, len) != 0) {
      err_sys("setvbuf _IOLBF failure");
    }
  } else {
    if (setvbuf(fp, buf, _IOFBF, len) != 0) {
      err_sys("setvbuf _IOFBF failure");
    }
  }
}

void pr_stdio(const char *name, FILE *fp) {
  printf("stream = %s, ", name);
  if (is_unbuffered(fp)) {
    printf("unbuffered");
  } else if (is_linebuffered(fp)) {
    printf("line buffered");
  } else {
    printf("fully buffered");
  }
  printf(", buffer size = %d\n", buffer_size(fp));
}

int is_unbuffered(FILE *fp) { return (fp->_flags & _IO_UNBUFFERED); }

int is_linebuffered(FILE *fp) { return (fp->_flags & _IO_LINE_BUF); }

int buffer_size(FILE *fp) { return (fp->_IO_buf_end - fp->_IO_buf_base); }
