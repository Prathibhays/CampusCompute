#include "campuscompute.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define CAP 5
#define ITEMS 12
typedef struct { int data[CAP], head, tail, count, finished; pthread_mutex_t m; pthread_cond_t not_full, not_empty; FILE *log; } Queue;
static Queue q;
static void log_event(const char *s,int n){time_t now=time(NULL);struct tm tmv;localtime_r(&now,&tmv);char ts[32];strftime(ts,sizeof ts,"%Y-%m-%d %H:%M:%S",&tmv);fprintf(q.log,"%s | %s | item=%d\n",ts,s,n);fflush(q.log);}
static void *producer(void *arg){(void)arg;for(int i=1;i<=ITEMS;i++){pthread_mutex_lock(&q.m);while(q.count==CAP)pthread_cond_wait(&q.not_full,&q.m);q.data[q.tail]=i;q.tail=(q.tail+1)%CAP;q.count++;printf("Producer: queued event %d (queue=%d)\n",i,q.count);log_event("PRODUCED",i);pthread_cond_signal(&q.not_empty);pthread_mutex_unlock(&q.m);}pthread_mutex_lock(&q.m);q.finished=1;pthread_cond_broadcast(&q.not_empty);pthread_mutex_unlock(&q.m);return NULL;}
static void *consumer(void *arg){(void)arg;for(;;){pthread_mutex_lock(&q.m);while(q.count==0&&!q.finished)pthread_cond_wait(&q.not_empty,&q.m);if(q.count==0&&q.finished){pthread_mutex_unlock(&q.m);break;}int x=q.data[q.head];q.head=(q.head+1)%CAP;q.count--;printf("Consumer: processed event %d (queue=%d)\n",x,q.count);log_event("CONSUMED",x);pthread_cond_signal(&q.not_full);pthread_mutex_unlock(&q.m);}return NULL;}
int run_logger(void){memset(&q,0,sizeof q);q.log=fopen("logs/campuscompute.log","a");if(!q.log){perror("logs/campuscompute.log (run from project directory)");return 1;}pthread_mutex_init(&q.m,NULL);pthread_cond_init(&q.not_full,NULL);pthread_cond_init(&q.not_empty,NULL);pthread_t p,c;if(pthread_create(&p,NULL,producer,NULL)||pthread_create(&c,NULL,consumer,NULL)){fprintf(stderr,"Failed to create logger threads\n");fclose(q.log);return 1;}pthread_join(p,NULL);pthread_join(c,NULL);pthread_cond_destroy(&q.not_full);pthread_cond_destroy(&q.not_empty);pthread_mutex_destroy(&q.m);fclose(q.log);puts("Logger threads terminated cleanly. Log appended to logs/campuscompute.log");return 0;}
