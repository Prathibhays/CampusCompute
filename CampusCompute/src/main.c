#include "campuscompute.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_help(void) {
    puts("CampusCompute - OS mini-project\n"
         "Usage: ./campuscompute COMMAND [OPTIONS]\n\n"
         "Commands:\n"
         "  help                 Show this help\n"
         "  fcfs                 Run First-Come, First-Served scheduling\n"
         "  rr [QUANTUM]         Run Round Robin (default quantum: 2)\n"
         "  priority             Run non-preemptive priority scheduling\n"
         "  compare              Compare all scheduling policies on same jobs\n"
         "  logger               Run bounded producer-consumer activity logger\n"
         "  ipc                  Demonstrate POSIX shared memory + named semaphore\n"
         "  banker safe          Run Banker's algorithm on a safe example\n"
         "  banker unsafe        Run Banker's algorithm on an unsafe example\n"
         "  worker normal        Run a child process that exits normally\n"
         "  worker fail          Simulate a controlled worker failure\n"
         "  limit                Demonstrate a process CPU-time resource limit\n"
         "  demo                 Run a short integrated demonstration\n\n"
         "Scheduling demo jobs are repeatable examples. Lower priority number means higher priority.\n");
}
int main(int argc, char **argv) {
    if (argc < 2 || strcmp(argv[1], "help") == 0 || strcmp(argv[1], "--help") == 0) { print_help(); return argc < 2 ? 1 : 0; }
    if (!strcmp(argv[1], "fcfs")) return run_scheduler("fcfs", 1);
    if (!strcmp(argv[1], "rr")) {
        if (argc > 2) { char *end = NULL; long q = strtol(argv[2], &end, 10); if (!end || *end || q < 1 || q > 1000) { fprintf(stderr,"Error: quantum must be an integer from 1 to 1000.\n"); return 2; } }
        return run_scheduler(argc > 2 ? argv[2] : "2", 1); /* numeric argument is interpreted as RR quantum */
    }
    if (!strcmp(argv[1], "priority")) return run_scheduler("priority", 1);
    if (!strcmp(argv[1], "compare")) { run_scheduler("fcfs",0); run_scheduler("2",0); run_scheduler("priority",0); puts("Comparison complete. All policies used the same sample jobs."); return 0; }
    if (!strcmp(argv[1], "logger")) return run_logger();
    if (!strcmp(argv[1], "ipc")) return run_ipc();
    if (!strcmp(argv[1], "banker") && argc == 3) { if (!strcmp(argv[2],"safe")) return run_banker(1); if (!strcmp(argv[2],"unsafe")) return run_banker(0); }
    if (!strcmp(argv[1], "worker") && argc == 3) { if (!strcmp(argv[2],"normal")) return run_worker(0); if (!strcmp(argv[2],"fail")) return run_worker(1); }
    if (!strcmp(argv[1], "limit")) return run_resource_limit();
    if (!strcmp(argv[1], "demo")) return run_demo();
    fprintf(stderr,"Unknown command or malformed arguments: %s\nRun './campuscompute help' for usage.\n", argv[1]); return 2;
}
