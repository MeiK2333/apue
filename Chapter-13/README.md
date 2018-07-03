# 守护进程

## 随书代码

## 课后习题

### 13.1

**从图 13-2 可以推测处, 直接调用 openlog 或第一次调用 syslog 都可以初始化 syslog 设施, 此时一定要打开用于 UNIX 域数据报套接字的特殊设备文件 /dev/log. 如果调用 openlog 前, 用户进程(守护进程)先调用了 chroot ,结果会怎么样?**  
如果进程调用 chroot ,它就不能打开 /dev/log . 解决的办法是, 守护进程在调用 chroot 之前调用选项为 LOG_NDELAY 的 openlog . 它打开特殊设备文件(UNIX 域数据报套接字)并生成一个描述符, 即使调用了 chroot 之后, 该描述符仍然是有效的. 这种场景在诸如 ftpd (文件传输协议守护进程)这样的守护进程中出现, 为了安全起见, 专门调用了 chroot, 但仍需要调用 syslog 来对出错条件记录日志.

### 13.2

**回顾 13.2 节中 ps 输出的示例. 唯一一个不是会话首进程的用户层守护进程是 rsyslogd 进程. 请解释为什么 rsyslogd 守护进程不是会话首进程.**  
待续

### 13.3

**列出你系统中所有有效的守护进程, 并说明它们各自的功能.**  
查看系统进程如下

```shell
$ sudo ps -axj
 PPID   PID  PGID   SID TTY      TPGID STAT   UID   TIME COMMAND
    0     1     1     1 ?           -1 Ss       0   0:14 /sbin/init splash
    0     2     0     0 ?           -1 S        0   0:00 [kthreadd]
    2     4     0     0 ?           -1 I<       0   0:00 [kworker/0:0H]
    2     6     0     0 ?           -1 I<       0   0:00 [mm_percpu_wq]
    2     7     0     0 ?           -1 S        0   0:03 [ksoftirqd/0]
    2     8     0     0 ?           -1 I        0   4:14 [rcu_sched]
    2     9     0     0 ?           -1 I        0   0:00 [rcu_bh]
    2    10     0     0 ?           -1 S        0   0:00 [migration/0]
    2    11     0     0 ?           -1 S        0   0:00 [watchdog/0]
    2    12     0     0 ?           -1 S        0   0:00 [cpuhp/0]
    2    13     0     0 ?           -1 S        0   0:00 [cpuhp/1]
    2    14     0     0 ?           -1 S        0   0:00 [watchdog/1]
    2    15     0     0 ?           -1 S        0   0:00 [migration/1]
    2    16     0     0 ?           -1 S        0   0:07 [ksoftirqd/1]
    2    18     0     0 ?           -1 I<       0   0:00 [kworker/1:0H]
    2    19     0     0 ?           -1 S        0   0:00 [cpuhp/2]
    2    20     0     0 ?           -1 S        0   0:00 [watchdog/2]
    2    21     0     0 ?           -1 S        0   0:00 [migration/2]
    2    22     0     0 ?           -1 S        0   0:03 [ksoftirqd/2]
    2    24     0     0 ?           -1 I<       0   0:00 [kworker/2:0H]
    2    25     0     0 ?           -1 S        0   0:00 [cpuhp/3]
    2    26     0     0 ?           -1 S        0   0:00 [watchdog/3]
    2    27     0     0 ?           -1 S        0   0:00 [migration/3]
    2    28     0     0 ?           -1 S        0   0:04 [ksoftirqd/3]
    2    30     0     0 ?           -1 I<       0   0:00 [kworker/3:0H]
    2    31     0     0 ?           -1 S        0   0:00 [kdevtmpfs]
    2    32     0     0 ?           -1 I<       0   0:00 [netns]
    2    33     0     0 ?           -1 S        0   0:00 [rcu_tasks_kthre]
    2    34     0     0 ?           -1 S        0   0:00 [kauditd]
    2    38     0     0 ?           -1 S        0   0:00 [khungtaskd]
    2    39     0     0 ?           -1 S        0   0:00 [oom_reaper]
    2    40     0     0 ?           -1 I<       0   0:00 [writeback]
    2    41     0     0 ?           -1 S        0   0:00 [kcompactd0]
    2    42     0     0 ?           -1 SN       0   0:00 [ksmd]
    2    43     0     0 ?           -1 SN       0   0:01 [khugepaged]
    2    44     0     0 ?           -1 I<       0   0:00 [crypto]
    2    45     0     0 ?           -1 I<       0   0:00 [kintegrityd]
    2    46     0     0 ?           -1 I<       0   0:00 [kblockd]
    2    48     0     0 ?           -1 I<       0   0:00 [ata_sff]
    2    51     0     0 ?           -1 I<       0   0:00 [md]
    2    52     0     0 ?           -1 I<       0   0:00 [edac-poller]
    2    53     0     0 ?           -1 I<       0   0:00 [devfreq_wq]
    2    54     0     0 ?           -1 I<       0   0:00 [watchdogd]
    2    57     0     0 ?           -1 S        0   0:54 [kswapd0]
    2    58     0     0 ?           -1 S        0   0:00 [ecryptfs-kthrea]
    2   100     0     0 ?           -1 I<       0   0:00 [kthrotld]
    2   101     0     0 ?           -1 I<       0   0:00 [acpi_thermal_pm]
    2   106     0     0 ?           -1 I<       0   0:00 [ipv6_addrconf]
    2   115     0     0 ?           -1 I<       0   0:00 [kstrp]
    2   136     0     0 ?           -1 I<       0   0:00 [charger_manager]
    2   185     0     0 ?           -1 S        0   0:00 [irq/30-DLL0655:]
    2   186     0     0 ?           -1 S        0   0:00 [scsi_eh_0]
    2   187     0     0 ?           -1 I<       0   0:00 [scsi_tmf_0]
    2   188     0     0 ?           -1 S        0   0:00 [scsi_eh_1]
    2   191     0     0 ?           -1 I<       0   0:00 [scsi_tmf_1]
    2   192     0     0 ?           -1 S        0   0:00 [scsi_eh_2]
    2   193     0     0 ?           -1 I<       0   0:00 [scsi_tmf_2]
    2   194     0     0 ?           -1 S        0   0:00 [scsi_eh_3]
    2   195     0     0 ?           -1 I<       0   0:00 [scsi_tmf_3]
    2   199     0     0 ?           -1 I<       0   0:00 [kworker/u9:0]
    2   200     0     0 ?           -1 S        0   0:00 [i915/signal:0]
    2   201     0     0 ?           -1 S        0   0:00 [i915/signal:1]
    2   202     0     0 ?           -1 S        0   0:00 [i915/signal:2]
    2   203     0     0 ?           -1 S        0   0:00 [i915/signal:4]
    2   204     0     0 ?           -1 I<       0   0:00 [rmi4-poller]
    2   206     0     0 ?           -1 S        0   0:00 [irq/47-i2c_hid]
    2   207     0     0 ?           -1 S        0   0:00 [nvidia-modeset]
    2   210     0     0 ?           -1 I<       0   0:04 [kworker/0:1H]
    2   244     0     0 ?           -1 I<       0   0:01 [kworker/1:1H]
    2   245     0     0 ?           -1 D        0   0:12 [jbd2/sda8-8]
    2   246     0     0 ?           -1 I<       0   0:00 [ext4-rsv-conver]
    2   248     0     0 ?           -1 I<       0   0:02 [kworker/3:1H]
    2   257     0     0 ?           -1 I<       0   0:03 [kworker/2:1H]
    2   331     0     0 ?           -1 S        0   0:00 [UVM global queu]
    2   332     0     0 ?           -1 S        0   0:00 [UVM Tools Event]
    2   369     0     0 ?           -1 I<       0   0:00 [cfg80211]
    2   390     0     0 ?           -1 S        0   0:01 [wl_event_handle]
    2   422     0     0 ?           -1 I<       0   0:00 [kmemstick]
    2   434     0     0 ?           -1 I        0   0:36 [rtsx_usb_ms_1]
    2  3315     0     0 ?           -1 S<       0   0:00 [krfcommd]
    2 17248     0     0 ?           -1 I        0   0:00 [kworker/u8:1]
    2 18652     0     0 ?           -1 I        0   0:00 [kworker/0:1]
    2 19808     0     0 ?           -1 I        0   0:00 [kworker/u8:2]
    2 20939     0     0 ?           -1 I        0   0:00 [kworker/2:0]
    2 20947     0     0 ?           -1 I        0   0:00 [kworker/u8:0]
    2 21672     0     0 ?           -1 I        0   0:00 [kworker/3:0]
    2 22436     0     0 ?           -1 S        0   0:00 [irq/50-mei_me]
    2 22437     0     0 ?           -1 S        0   0:00 [nvidia]
    2 22463     0     0 ?           -1 I<       0   0:00 [kworker/u9:1]
    2 22644     0     0 ?           -1 I        0   0:00 [kworker/1:2]
    2 23282     0     0 ?           -1 I        0   0:00 [kworker/0:2]
    2 23310     0     0 ?           -1 I        0   0:00 [kworker/3:1]
    2 23668     0     0 ?           -1 I        0   0:00 [kworker/1:0]
    2 30568     0     0 ?           -1 I        0   0:02 [kworker/1:1]
    2 32363     0     0 ?           -1 I        0   0:00 [kworker/2:2]
```

### 13.4

**编写一段程序调用图 13-1 中的 daemonize 函数. 调用该函数后, 它已成为守护进程, 再调用 getlogin (见 8.15 节)查看该进程是否有登录名. 将结果打印到一个文件中.**  
pass