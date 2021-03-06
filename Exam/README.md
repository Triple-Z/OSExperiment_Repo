# 操作系统实验上机考试

NUAA Spring 2018

<!-- TOC -->

- [操作系统实验上机考试](#操作系统实验上机考试)
    - [题目1](#题目1)
    - [题目2](#题目2)
    - [题目3](#题目3)
    - [题目4](#题目4)

<!-- /TOC -->

## 题目1

主进程创建1个子进程
主进程通过管道与子进程连接
子进程的标准输出连接到管道的写端
主进程的标准输入连接到管道的读端

在子进程中调用exec(“echo”, “echo”, “hello world”, NULL)
在父进程中调用read(0, buf, sizeof(buf))，从标准输入中获取子进程发送的字符串，并打印出来

Answer: [problem1.c](problem1.c)

## 题目2

主进程创建2个子进程，主进程通过两个管道分别与两个子进程连接
第一个子进程计算从1加到50的和，并将结果通过管道送给父进程
第二个子进程计算从51加到100的和，并将结果通过管道送给父进程
父进程读取两个子进程的结果，将他们相加，打印出来，结果为5050

Answer: [problem2.c](problem2.c)

## 题目3

主线程创建10个子线程
第0个子线程计算从01加到10的和
第1个子线程计算从11加到20的和
第2个子线程计算从21加到30的和
...
第9个子线程计算从91加到100的和

主线程归并10个子线程的计算结果，最终结果为5050
本题必须使用**线程参数**来完成

Answer: [problem3.c](problem3.c)

## 题目4

主线程创建4个子线程T1、T2、T3、T4，主线程在4个子线程退出后，才退出。

线程T1、T2、T3、T4的运行时代码如下：

```c
#include <unistd.h> // sleep函数声明在该头文件中

void *T1_entry(void *arg)
{
    sleep(2);  // 睡眠2秒，不准删除此条语句，否则答题无效
    puts(“T1”);
}

void *T2_entry(void *arg)
{
    sleep(1);  // 睡眠1秒，不准删除此条语句，否则答题无效
    puts(“T2”);
}

void *T3_entry(void *arg)
{
    sleep(1);  // 睡眠1秒，不准删除此条语句，否则答题无效
    puts(“T3”);
}

void *T4_entry(void *arg)
{
    puts(“T4”);
}
```

使用信号量或者条件变量机制(而不是使用sleep函数)，使得这四个线程满足如下制约关系：

- T1的print语句执行后，T2和T3才可以执行print语句
- T2和T3的print语句执行后，T4才可以执行print语句

教程中关于信号量的实现有错误，已经更正。
程序输出结果为:

```bash
T1
T2
T3
T4
```

或者

```bash
T1
T3
T2
T4
```

Answer: [problem4.c](problem4.c)
