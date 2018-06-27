<!-- TOC -->

- [OSExperiment Exam](#osexperiment-exam)
- [OSExperiment Repo](#osexperiment-repo)
    - [文件读写编程题目](#文件读写编程题目)
        - [myecho.c](#myechoc)
        - [mycat.c](#mycatc)
        - [mycp.c](#mycpc)
    - [多进程题目](#多进程题目)
        - [mysys.c: 实现函数mysys，用于执行一个系统命令，要求如下](#mysysc-实现函数mysys用于执行一个系统命令要求如下)
        - [sh1.c: 实现shell程序，要求具备如下功能](#sh1c-实现shell程序要求具备如下功能)
        - [sh2.c: 实现shell程序，要求在第1版的基础上，添加如下功能](#sh2c-实现shell程序要求在第1版的基础上添加如下功能)
        - [sh3.c: 实现shell程序，要求在第2版的基础上，添加如下功能](#sh3c-实现shell程序要求在第2版的基础上添加如下功能)
    - [多线程题目](#多线程题目)
        - [pi1.c: 使用2个线程根据莱布尼兹级数计算PI](#pi1c-使用2个线程根据莱布尼兹级数计算pi)
        - [pi2.c: 使用N个线程根据莱布尼兹级数计算PI](#pi2c-使用n个线程根据莱布尼兹级数计算pi)
        - [sort.c: 多线程排序](#sortc-多线程排序)
        - [pc1.c: 使用条件变量解决生产者、计算者、消费者问题](#pc1c-使用条件变量解决生产者计算者消费者问题)
        - [pc2.c: 使用信号量解决生产者、计算者、消费者问题](#pc2c-使用信号量解决生产者计算者消费者问题)
        - [ring.c: 创建N个线程，它们构成一个环](#ringc-创建n个线程它们构成一个环)
    - [Copyright](#copyright)

<!-- /TOC -->

# OSExperiment Exam

操作系统上机考试 NUAA 2018 Spring

[题目及解答](Exam/README.md)

# OSExperiment Repo

操作系统实验 NUAA


## 文件读写编程题目

### myecho.c

[Source C File](FileRW/myecho.c)

- myecho.c的功能与系统echo程序相同
- 接受命令行参数，并将参数打印出来，例子如下：

```bash
$ ./myecho x
x
$ ./myecho a b c
a b c
```

### mycat.c

[Source C File](FileRW/mycat.c)

- mycat.c的功能与系统cat程序相同
- mycat将指定的文件内容输出到屏幕，例子如下：
- 要求使用系统调用open/read/write/close实现

```bash
$ cat /etc/passwd
root:x:0:0:root:/root:/bin/bash
daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
bin:x:2:2:bin:/bin:/usr/sbin/nologin
...
$ ./mycat /etc/passwd
root:x:0:0:root:/root:/bin/bash
daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
bin:x:2:2:bin:/bin:/usr/sbin/nologin
...
```

### mycp.c

[Source C File](FileRW/mycp.c)

- mycp.c的功能与系统cp程序相同
- 将源文件复制到目标文件，例子如下：
- 要求使用系统调用open/read/write/close实现

```bash
$ cat /etc/passwd
root:x:0:0:root:/root:/bin/bash
daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
bin:x:2:2:bin:/bin:/usr/sbin/nologin
...
$ ./mycp /etc/passwd passwd.bak
$ cat passwd.bak
root:x:0:0:root:/root:/bin/bash
daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
bin:x:2:2:bin:/bin:/usr/sbin/nologin
...
```

## 多进程题目

### mysys.c: 实现函数mysys，用于执行一个系统命令，要求如下

[Source C File](MultiProcess/mysys.c)

- mysys的功能与系统函数system相同，要求用进程管理相关系统调用自己实现一遍
- 使用fork/exec/wait系统调用实现mysys
- 不能通过调用系统函数system实现mysys

测试程序

```c
#include <stdio.h>

int main()
{
    printf("--------------------------------------------------\n");
    system("echo HELLO WORLD");
    printf("--------------------------------------------------\n");
    system("ls /");
    printf("--------------------------------------------------\n");
    return 0;
}
```

测试程序的输出结果

```bash
--------------------------------------------------
HELLO WORLD
--------------------------------------------------
bin    core  home	     lib	 mnt   root  snap  tmp	vmlinuz
boot   dev   initrd.img      lost+found  opt   run   srv   usr	vmlinuz.old
cdrom  etc   initrd.img.old  media	 proc  sbin  sys   var
--------------------------------------------------
```

### sh1.c: 实现shell程序，要求具备如下功能

[Source C File](MultiProcess/sh1.c)

支持命令参数

```bash
$ echo arg1 arg2 arg3
$ ls /bin /usr/bin /home
```

实现内置命令cd、pwd、exit

```bash
$ cd /bin
$ pwd
/bin
```

### sh2.c: 实现shell程序，要求在第1版的基础上，添加如下功能

[Source C File](MultiProcess/sh2.c)

实现文件重定向

```bash
$ echo hello >log
$ cat log
hello
```

### sh3.c: 实现shell程序，要求在第2版的基础上，添加如下功能

实现管道

```bash
$ cat /etc/passwd | wc -l
```

实现管道和文件重定向

```bash
$ cat input.txt
3
2
1
3
2
1
$ cat <input.txt | sort | uniq | cat >output.txt
$ cat output.txt
1
2
3
```

## 多线程题目

### pi1.c: 使用2个线程根据莱布尼兹级数计算PI

[Source C File](MultiThread/pi1.c)

- 莱布尼兹级数公式: 1 - 1/3 + 1/5 - 1/7 + 1/9 - ... = PI/4
- 主线程创建1个辅助线程
- 主线程计算级数的前半部分
- 辅助线程计算级数的后半部分
- 主线程等待辅助线程运行結束后,将前半部分和后半部分相加

### pi2.c: 使用N个线程根据莱布尼兹级数计算PI

[Source C File](MultiThread/pi2.c)

- 与上一题类似，但本题更加通用化，能适应N个核心，需要使用线程参数来实现
- 主线程创建N个辅助线程
- 每个辅助线程计算一部分任务，并将结果返回
- 主线程等待N个辅助线程运行结束，将所有辅助线程的结果累加

### sort.c: 多线程排序

[Source C File](MultiThread/sort.c)

- 主线程创建一个辅助线程
- 主线程使用选择排序算法对数组的前半部分排序
- 辅助线程使用选择排序算法对数组的后半部分排序
- 主线程等待辅助线程运行結束后,使用归并排序算法归并数组的前半部分和后半部分

### pc1.c: 使用条件变量解决生产者、计算者、消费者问题

[Source C File](MultiThread/pc1.c)

- 系统中有3个线程：生产者、计算者、消费者
- 系统中有2个容量为4的缓冲区：buffer1、buffer2
- 生产者生产'a'、'b'、'c'、‘d'、'e'、'f'、'g'、'h'八个字符，放入到buffer1
- 计算者从buffer1取出字符，将小写字符转换为大写字符，放入到buffer2
- 消费者从buffer2取出字符，将其打印到屏幕上

### pc2.c: 使用信号量解决生产者、计算者、消费者问题

[Source C File](MultiThread/pc2.c)

- 功能和前面的实验相同，使用信号量解决

### ring.c: 创建N个线程，它们构成一个环

[Source C File](MultiThread/ring.c)

- 创建N个线程：T1、T2、T3、… TN
- T1向T2发送整数1
- T2收到后将整数加1
- T2向T3发送整数2
- T3收到后将整数加1
- T3向T4发送整数3
- …
- TN收到后将整数加1
- TN向T1发送整数N

## Copyright

Copyright &copy; 2018 [TripleZ](https://github.com/Triple-Z)
