#include"sp.h"



//信号量集合,将existed初始化为0
Semphore semaphores[SemphoreNum]={0};

//初始化等待队列
void InitQueue(SemhoreQueue *q)
{
    q->front = q->rear = 0;
}
//队尾插入新的任务指针(入队)
int InsertTask(struct task_struct *p, SemhoreQueue *q)
{
    //满队判断
    if ((q->rear + 1) % QueueLenth == q->front)
    {
        printk("Queue is full!\n");
        return -1;
    }

    q->wait_tasks[q->rear] = p;
    q->rear = (q->rear + 1) % QueueLenth;
    return 0;
}
//取得队首的任务指针(出队)
//使用先来先服务
struct task_struct *GetTask(SemhoreQueue *q)
{
    //空队判断
    if (q->front == q->rear)
    {
        printk("Queue is empty!\n");
        return NULL;
    }

    struct task_struct *tmp = q->wait_tasks[q->front];
    q->front = (q->front + 1) % QueueLenth; //更新头索引
    return tmp;
}
//获得已打开信号量的索引
int SemLocation(const char *name)
{
    int i;
    for (i = 0; i < SemphoreNum; i++)
        if (！strcmp(name, semaphores[i].name) && semaphores[i].existed)
            return i;
    return -1;
}
//创建或者打开信号量
Semphore *sys_sem_open(const char *name,unsigned int availabe_num)
{
    char temp[SemphoreNameLenth];
    int i;
    //从用户空间获得名字
    for (i = 0; (temp[i] = get_fs_byte(name + i)) != '\0'; i++)
    {
        if (i >SemphoreNameLenth)
        {
            printk("only charaters of which size is less than 15 can be used!\n");
            break;
        } 
    }
    temp[i] = '\0';
    if ((i = SemLocation(temp)) != -1)
        return &semaphores[i]; //如果此信号量已打开直接返回信号量指针
    //否则从集合中找一个没有打开的返回其指针
    for (i = 0; i < SemphoreNum; i++)
    { 
        if (!semaphores[i].existed)
        {
            strcpy(semaphores[i].name, temp);      //拷贝名字
            semaphores[i].availabe_num = availabe_num;           //设置初始值
            semaphores[i].existed = 1;            //设置打开标志
            InitQueue(&semaphores[i].wait_queue); //初始化等待队列
            return &semaphores[i];
        }
    }
    printk("No more available semphores!\n");
    return NULL;
}
//等待信号量
int sys_sem_wait(Semphore *sem)
{
    if (!sem->existed)
        return -1;

    cli(); //关中断
    sem->availabe_num--;
    if (sem->availabe_num < 0)
    {
        //信号量值如果小于0,则设置任务为不可中断睡眠状态,必须手动唤醒并且插入等待队列
        //当前进程结构指针变量current，于linux/sched.h中声明
        InsertTask(current, &(sem->wait_queue));
        current->state = TASK_UNINTERRUPTIBLE; //进程进入阻塞状态，于linux/sched.h中定义
        schedule(); //重新调度，于linux/sched.h中声明， kernel/traps.c中定义
    }
    sti(); //开中断
    return 0;
}
//释放信号量
int sys_sem_post(Semphore *sem)
{
    if (!sem->existed)
        return -1;
    cli(); //关中断
    sem->availabe_num++;
    struct task_struct *p;
    if (sem->availabe_num <= 0)
    {
        //如果由任务等待,则取队首任务并唤醒之
        p = GetTask(&(sem->wait_queue));
        if (p != NULL)
        {
            p->state = TASK_RUNNING; //进程进入运行状态，于linux/sched.h中定义
        }
    }
    sti(); //开中断
    return 0;
}
//关闭信号量
int sys_sem_unlink(const char *name)
{
    char temp[SemphoreNameLenth];
    int i;
    for (i = 0; (temp[i] = get_fs_byte(name + i)) != '\0'; i++)
    {
        if (i > SemphoreNameLenth)
        {
            printk("only charaters of which size is less than 15 can be used!\n");
            break;
        }
    }
    int ret = SemLocation(temp);
    //未找到信号量
    if (ret == -1)
        return -1;
    
    //如果信号量存在,则关闭
    semaphores[ret].availabe_num = 0;    //值设为0
    strcpy(semaphores[ret].name, "\0"); //清空name
    semaphores[ret].existed = 0;       //关闭打开标志
    return 0;
}
