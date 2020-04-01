#define __LIBRARY__
#include <unistd.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm.h>

#define SharedMemorySize 64

typedef struct
{
    unsigned int key;
    unsigned int size;
    unsigned long page;
    int value;
} SharedMemory;



int sys_shmget(unsigned int key, size_t size);
void *sys_shmat(int shmid);