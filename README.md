# CampusCompute — Operating Systems Level 3 Mini-Project

A C command-line prototype demonstrating CPU scheduling, POSIX threads, bounded producer-consumer logging, POSIX shared-memory IPC, semaphore coordination, Banker's safety algorithm, child-process status collection, and a Linux resource limit.

## Prerequisites

Ubuntu/Linux or WSL Ubuntu, GCC, Make, and POSIX threads/semaphores support.

```bash
sudo apt update
sudo apt install build-essential make
```

## Build and run

Run these commands from the `CampusCompute` project directory:

```bash
make clean
make
./campuscompute help
./campuscompute fcfs
./campuscompute rr 2
./campuscompute priority
./campuscompute compare
./campuscompute logger
./campuscompute ipc
./campuscompute banker safe
./campuscompute banker unsafe
./campuscompute worker normal
./campuscompute worker fail
./campuscompute limit
./campuscompute demo
```

The sample scheduling data is fixed and repeatable for demonstrations. For priority scheduling, lower numeric values mean higher priority; ties are broken by arrival time and then job ID. Round Robin defaults to a time quantum of 2. The logger appends actual producer/consumer events to `logs/campuscompute.log`.

## Cleanup

`make clean` removes the executable and generated log file. The IPC demonstration uses unique names and removes the shared-memory object and semaphore it creates. If an IPC name already exists, the program refuses to delete it because it cannot establish that the object belongs to this run.

## Feature notes / limitations

- `limit` applies `RLIMIT_CPU` to the command process and reports the active soft/hard limits. It deliberately avoids a CPU-burning loop; this demonstrates configuration, not an observed kill event.
- `worker fail` is an intentional exit code of 7, not a real operating-system crash.
- `banker unsafe` uses the same example allocation/maximum matrices with Available set to (0,0,0), making the state unsafe.
- This is a teaching prototype using a repeatable sample job set, not a production scheduler or a real lab job-execution service.

## Suggested evidence

Capture genuine terminal screenshots with your own complete SRN visible in the prompt. Use the course checklist S01–S24 and add one sentence explaining each screenshot. Do not fabricate output or screenshots. The assignment asks for one PDF named with your own SRN and does not ask for a separate report.
