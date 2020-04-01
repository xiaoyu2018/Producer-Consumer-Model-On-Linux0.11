#include"shm.h"

//共享内存集合
static SharedMemory shms[SharedMemorySize] = {0};

int sys_shmget(unsigned int key, size_t size)
{
    int i;
    unsigned long *page;  //接收空闲页面地址
    if (size > PAGE_SIZE) //页面大小，于linux/mm.h定义
        return -1;
    //若对应key的共享内存已存在，则直接返回集合索引
    for (i = 0; i < SharedMemorySize; i++)
    {
        if (shms[i].key == key && shms[i].value)
        {
            //AddMemMap(shms[i].page);
            return i;
        }
            
    }
    page = get_free_page();//申请请一个空闲页，于mm/memory.c定义
    if (!page)             //申请失败
        return -1;
    
    //按顺序将集合内第一个未使用的位置交给key
    for (i = 0; i < SharedMemorySize; i++)
    {
        if (shms[i].key == 0)
        {
            shms[i].page = page;
            shms[i].key = key;
            shms[i].size = size;
            return i;
        }
    }
    //共享内存集合满
    return -1;
}

void *sys_shmat(int shmid)
{
    int i;
    unsigned long tail;

    if (shmid == -1 || SharedMemorySize <= shmid || 
    shms[shmid].page == 0 || shms[shmid].key == 0)
        return NULL;

    tail = current->start_code + current->brk; //当前进程的总长度（字节），于linux/sched.h定义
                                               //即映射到当前程序的末尾
    //把物理页映射到指定线性地址，于mm/memory.c定义
    if (put_page(shms[shmid].page, tail) == 0)
        return (void *)-1;

    //增加引用次数
    shms[shmid].value++;

    //返回共享内存的逻辑地址，在用户态中段基地址和代码起始地址相同，即start_code=0
    return (void *)(current->brk);
}

int sys_shmdt(unsigned int key, unsigned long startaddr)
{
    int i;
    for (i = 0; i < SharedMemorySize && shms[i].key != key; i++)
        ;
    if (i == SharedMemorySize)
        return -1;
    startaddr += current->start_code;
    UnMap(startaddr);
    if (!(--shms[i].value))
    {
        free_page(shms[i].page);
        shms[i].key = 0;
        shms[i].size = 0;
        shms[i].page = 0;
    }

    return 0;
}