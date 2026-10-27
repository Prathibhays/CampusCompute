#include "campuscompute.h"
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
int run_worker(int fail){pid_t p=fork();if(p<0){perror("fork");return 1;}if(p==0){printf("Child worker PID=%ld started.\n",(long)getpid());fflush(stdout);if(fail){fprintf(stderr,"Child worker: controlled simulated failure (exit code 7).\n");_exit(7);}_exit(0);}int st;if(waitpid(p,&st,0)<0){perror("waitpid");return 1;}printf("Parent collected PID=%ld: ",(long)p);if(WIFEXITED(st)){int code=WEXITSTATUS(st);printf("exited normally with code %d\n",code);if(code!=0)puts("Detection: worker reported a non-zero exit; this is a simulated failure, not an OS crash.");return fail?(code==7?0:1):(code==0?0:1);}if(WIFSIGNALED(st)){printf("terminated by signal %d\n",WTERMSIG(st));return 1;}puts("status could not be classified");return 1;}
