#include "campuscompute.h"
#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#define SHM_NAME "/campuscompute_shm_demo"
#define SEM_NAME "/campuscompute_sem_demo"
typedef struct { int ready; char message[128]; } Shared;
int run_ipc(void){int fd=shm_open(SHM_NAME,O_CREAT|O_EXCL|O_RDWR,0600);if(fd<0){if(errno==EEXIST){fprintf(stderr,"IPC name already exists; not deleting an object this run did not create. Run cleanup instructions only after verifying ownership.\n");return 1;}perror("shm_open");return 1;}sem_t *sem=SEM_FAILED;int rc=1; if(ftruncate(fd,sizeof(Shared))<0){perror("ftruncate");goto cleanup;}Shared *s=mmap(NULL,sizeof *s,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);if(s==MAP_FAILED){perror("mmap");goto cleanup;}s->ready=0;strcpy(s->message,"empty");sem=sem_open(SEM_NAME,O_CREAT|O_EXCL,0600,0);if(sem==SEM_FAILED){perror("sem_open");munmap(s,sizeof *s);goto cleanup;}pid_t pid=fork();if(pid<0){perror("fork");munmap(s,sizeof *s);goto cleanup;}if(pid==0){strcpy(s->message,"Hello from child process through POSIX shared memory");s->ready=1;sem_post(sem);munmap(s,sizeof *s);close(fd);_exit(0);}if(sem_wait(sem)<0)perror("sem_wait");else printf("Parent received shared-memory message: %s\n",s->message);int st;waitpid(pid,&st,0);printf("Child status: %s\n",WIFEXITED(st)&&WEXITSTATUS(st)==0?"normal completion":"failure");munmap(s,sizeof *s);rc=0;
cleanup: if(sem!=SEM_FAILED){sem_close(sem);if(sem_unlink(SEM_NAME)<0)perror("sem_unlink");}close(fd);if(shm_unlink(SHM_NAME)<0)perror("shm_unlink");puts("IPC objects created by this run cleaned up.");return rc;}
