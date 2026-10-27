#include "campuscompute.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 4
typedef struct { int id, arrival, burst, priority, remaining, completion, first_start; } Job;
static Job base[N] = {{1,0,5,2,5,0,-1},{2,1,3,1,3,0,-1},{3,2,8,3,8,0,-1},{4,3,6,2,6,0,-1}};
static void metrics(Job a[], const char *name, int timeline) {
    double tw=0,tt=0; printf("\nPolicy: %s\n",name);
    if(timeline) puts("Execution timeline (job@start-end):");
    for(int i=0;i<N;i++){ int tat=a[i].completion-a[i].arrival, wait=tat-a[i].burst; tw+=wait;tt+=tat; if(timeline) printf("  J%d: completion=%d, turnaround=%d, waiting=%d\n",a[i].id,a[i].completion,tat,wait); }
    printf("Average turnaround time: %.2f\nAverage waiting time: %.2f\n",tt/N,tw/N);
}
static void fcfs(void){ Job a[N]; memcpy(a,base,sizeof a); int time=0; printf("FCFS timeline: ");
 for(int k=0;k<N;k++){int pick=-1;for(int i=0;i<N;i++)if(a[i].completion==0 && a[i].arrival<=time && (pick<0||a[i].arrival<a[pick].arrival||(a[i].arrival==a[pick].arrival&&a[i].id<a[pick].id)))pick=i;
 if(pick<0){int mn=99999;for(int i=0;i<N;i++)if(!a[i].completion&&a[i].arrival<mn)mn=a[i].arrival;time=mn; k--;continue;} int st=time; if(time<a[pick].arrival)st=a[pick].arrival; time=st+a[pick].burst;a[pick].completion=time; printf("J%d[%d-%d] ",a[pick].id,st,time); }
 puts(""); metrics(a,"FCFS",1); }
static void priority_sched(void){Job a[N];memcpy(a,base,sizeof a);int done=0,time=0;printf("Priority timeline (lower number = higher priority; ties: arrival then job ID): ");while(done<N){int p=-1;for(int i=0;i<N;i++)if(!a[i].completion&&a[i].arrival<=time&&(p<0||a[i].priority<a[p].priority||(a[i].priority==a[p].priority&&(a[i].arrival<a[p].arrival||(a[i].arrival==a[p].arrival&&a[i].id<a[p].id)))))p=i;if(p<0){time++;continue;}int st=time;time+=a[p].burst;a[p].completion=time;done++;printf("J%d(P%d)[%d-%d] ",a[p].id,a[p].priority,st,time);}puts("");metrics(a,"Non-preemptive Priority",1);}
static void rr(int q){Job a[N];memcpy(a,base,sizeof a);int time=0,done=0,queue[1000],head=0,tail=0,added[N]={0};printf("Round Robin timeline (quantum=%d): ",q);while(done<N){for(int i=0;i<N;i++)if(!added[i]&&a[i].arrival<=time){queue[tail++]=i;added[i]=1;}if(head==tail){time++;continue;}int p=queue[head++],run=a[p].remaining<q?a[p].remaining:q;printf("J%d[%d-%d] ",a[p].id,time,time+run);time+=run;a[p].remaining-=run;for(int i=0;i<N;i++)if(!added[i]&&a[i].arrival<=time){queue[tail++]=i;added[i]=1;}if(a[p].remaining>0)queue[tail++]=p;else{a[p].completion=time;done++;}}puts("");metrics(a,"Round Robin",1);}
int run_scheduler(const char *policy,int verbose){(void)verbose;if(!strcmp(policy,"fcfs"))fcfs();else if(!strcmp(policy,"priority"))priority_sched();else{char *e;long q=strtol(policy,&e,10);if(!e||*e||q<1||q>1000){fprintf(stderr,"Invalid Round Robin quantum. Use ./campuscompute rr 2\n");return 2;}rr((int)q);}return 0;}
