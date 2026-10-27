#ifndef CAMPUSCOMPUTE_H
#define CAMPUSCOMPUTE_H
#include <stddef.h>
void print_help(void);
int run_scheduler(const char *policy, int verbose);
int run_logger(void);
int run_ipc(void);
int run_banker(int safe_case);
int run_worker(int simulate_failure);
int run_resource_limit(void);
int run_demo(void);
#endif
