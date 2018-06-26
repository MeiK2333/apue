#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

struct timer_t {
    time_t second;  // 定时器时间
    time_t unix_time_started_at;  // 设置定时器时的 UNIX 时间戳
    time_t unix_time_ended_at;  // 设置的定时器应该触发的 UNIX 时间戳
    struct timer_t * next;  // 指向下一个定时器
};