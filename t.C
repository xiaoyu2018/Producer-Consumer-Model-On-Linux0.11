#define __LIBRARY__
#include<unistd.h>
#include<stdio.h>
#define __NR_shmget 91
#define __NR_shmat 92
#define __NR_shmdt 93
#define __NR_shm_info 95

_syscall2(int, shmget, unsigned int, key, size_t, size)
_syscall2(int, shmdt, unsigned int, key, unsigned long, startaddr)
_syscall1(void *, shmat, int, shmid)
_syscall1(void, shm_info, int, key)
#define KEY 100
int main()
{
	int ID;
	
	ID=shmget(100,4096);
	add=shmat(ID);
	printf(">>>>>>>After GET and ATTACH\n");
	shm_info(KEY);
	shmdt(KEY);
	printf(">>>>>>>After Delete\n");
	shm_info(KEY);
}
