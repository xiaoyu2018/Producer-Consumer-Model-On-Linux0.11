#define __LIBRARY__
#include <unistd.h>
#include <stdio.h>

#define __NR_sem_open 87
#define __NR_sem_info 94

_syscall2(Semphore *, sem_open, const char *, name, unsigned int, availabe_num)
_syscall1(void, sem_info, Semphore*, sem_info)

typedef void Semphore;

int main()
{
    Semphore *sem = sem_open("test", 5);
    sem_info(sem);
}