#include "campuscompute.h"
#include <stdio.h>
int run_demo(void){puts("=== CampusCompute integrated demonstration ===");puts("[1/5] FCFS scheduling");run_scheduler("fcfs",1);puts("\n[2/5] Banker's safe-state analysis");run_banker(1);puts("\n[3/5] Normal worker lifecycle");run_worker(0);puts("\n[4/5] Controlled simulated worker failure");run_worker(1);puts("\n[5/5] Resource limit configuration");run_resource_limit();puts("\nIntegrated demonstration complete. Run 'logger' and 'ipc' separately for concurrency and IPC evidence.");return 0;}
