#include <poll.h>
#include <sys/select.h>
#include "apue.h"

void sleep_us(unsigned int nusecs) {
  struct timeval tval;

  tval.tv_sec = nusecs / 1000000;
  tval.tv_usec = nusecs % 1000000;
  select(0, NULL, NULL, NULL, &tval);
}

void sleep_us_poll(unsigned int nusecs) {
  struct pollfd dummy;
  int timeout;

  if ((timeout = nusecs / 1000) <= 0) {
    timeout = 1;
  }
  poll(&dummy, 0, timeout);
}