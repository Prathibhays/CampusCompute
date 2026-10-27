#include "campuscompute.h"
#include <stdio.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int run_resource_limit(void) {
    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return 1; }
    if (pid == 0) {
        struct rlimit lim = {1, 2};
        if (setrlimit(RLIMIT_CPU, &lim) < 0) { perror("child setrlimit"); _exit(2); }
        struct rlimit actual;
        if (getrlimit(RLIMIT_CPU, &actual) < 0) { perror("child getrlimit"); _exit(3); }
        printf("Child PID=%ld: RLIMIT_CPU soft=%ld sec, hard=%ld sec\n", (long)getpid(), (long)actual.rlim_cur, (long)actual.rlim_max);
        fflush(stdout);
        /* Deliberate, bounded-by-the-OS CPU-limit test in a child process only. */
        volatile unsigned long counter = 0;
        for (;;) counter++;
    }
    int status;
    if (waitpid(pid, &status, 0) < 0) { perror("waitpid"); return 1; }
    if (WIFSIGNALED(status)) {
        int sig = WTERMSIG(status);
        printf("Parent detected child termination by signal %d (%s).\n", sig, sig == SIGXCPU ? "SIGXCPU: CPU soft limit exceeded" : "other signal");
        puts("Resource-limit effect observed in an isolated child; the parent process remains running.");
        return sig == SIGXCPU ? 0 : 1;
    }
    if (WIFEXITED(status)) {
        printf("Child exited with status %d before the CPU limit signal was observed.\n", WEXITSTATUS(status));
        return WEXITSTATUS(status) == 0 ? 0 : 1;
    }
    puts("Child status could not be classified.");
    return 1;
}
