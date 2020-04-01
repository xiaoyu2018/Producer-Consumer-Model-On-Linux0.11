#define __LIBRARY__
#include<unistd.h>
#include<stdio.h>

#define __NR_sem_open 87
#define __NR_sem_wait 88
#define __NR_sem_post 89
#define __NR_sem_unlink 90
#define __NR_shmget 91
#define __NR_shmat 92
typedef void Semphore;

_syscall2(int, shmget, unsigned int, key, size_t, size)
_syscall1(void *, shmat, int, shmid)
_syscall2(Semphore *, sem_open, const char *, name, unsigned int, availabe_num)
_syscall1(int, sem_wait, Semphore *, sem)
_syscall1(int, sem_post, Semphore *, sem)
_syscall1(int, sem_unlink, const char *, name)

#define BufferSize 20
#define ConsumerNum 5
#define ProductionNum 50
int main()
{
    int i, j;
    Semphore *mutex, *full, *empty;
    int id = shmget(111, 4096), index = 0, finished = 0;
    char *buffer = (char *)shmat(id);
    mutex = (Semphore*)sem_open("mutex", 1);
    full = (Semphore*)sem_open("full", 0);
    empty = (Semphore*)sem_open("empty", BufferSize);
    for (i = 0; i < ConsumerNum && (j = fork()) != -1; i++)
    {
        if (j == 0)
        {
            while (finished < ProductionNum)
            {
                sem_wait(full);
                sem_wait(mutex);
                printf("Consumer:%d is Consuming No.%d production\n", i, finished + 1);
                sleep(5);
                buffer[(index++) % BufferSize] = index;
                finished++;
                printf("%c is Consumecd\n", buffer[(index + BufferSize-1) % BufferSize]);
                sem_post(mutex);
                sem_post(empty);
            }
        }
    }
    while (1)
    {
        if (finished >= ProductionNum)
        {
            sem_unlink("mutex");
            sem_unlink("empty");
            sem_unlink("full");
            break;
        }
    }
}
