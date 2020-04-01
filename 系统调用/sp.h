/*****信号量机制头文件****/
#include <linux/kernel.h>
#include <asm/segment.h>
#include <asm/system.h>
#include <string.h>
#include <unistd.h>
#define __LIBRARY__

//信号量最大数目
#define SemphoreNum 10
//信号量名字最大长度
#define SemphoreNameLenth 15
//等待队列最大长度
#define QueueLenth 32

//等待队列结构体
typedef struct 
{
    int front;
    int rear;
    //task_sruct在linux/sched.h中定义
    struct task_struct* wait_tasks[QueueLenth];
} SemhoreQueue;

//信号量结构体
typedef struct 
{
    char name[SemphoreNameLenth];
    int existed;
    //可共享的进程个数
    int availabe_num;
    //被阻塞的进程等待队列
    SemhoreQueue wait_queue;
} Semphore;

extern sem_t semtable[SEMTABLE_LEN];

Semphore *sys_sem_open(const char *name, unsigned int availabe_num);
int sys_sem_wait(Semphore *sem);
int sys_sem_post(Semphore *sem);
int sys_sem_unlink(const char *name);