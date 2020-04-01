#define __NR_sem_open 87
#define __NR_sem_wait 88
#define __NR_sem_post 89
#define __NR_sem_unlink 90
#define __NR_shmget 91
#define __NR_shmat 92
#define __NR_shmdt 93

//nr_system_calls = 94

extern int sys_sem_open();
extern int sys_sem_wait();
extern int sys_sem_post();
extern int sys_sem_unlink();
extern int sys_shmget();
extern int sys_shmat();
extern int sys_shmdt();

/*sys_sem_open(),       //87
sys_sem_wait(),   //88
sys_sem_post(),   //89
sys_sem_unlink(), //90
sys_shmget(),     //91
sys_shmat()       //92*/

/*int sys_shmget(unsigned int key, size_t size);
void *sys_shmat(int shmid);
Semphore *sys_sem_open(const char *name, unsigned int availabe_num);
int sys_sem_wait(Semphore *sem);
int sys_sem_post(Semphore *sem);
int sys_sem_unlink(const char *name);
int sys_shmdt(unsigned int key, unsigned long startaddr)
*/