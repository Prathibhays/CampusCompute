CC := gcc
CFLAGS := -std=c11 -D_POSIX_C_SOURCE=200809L -Wall -Wextra -Wpedantic -O2 -Iinclude
LDLIBS := -pthread -lrt
SRC := src/main.c src/scheduler.c src/logger.c src/ipc.c src/banker.c src/worker.c src/resource_limit.c src/demo.c
TARGET := campuscompute
.PHONY: all clean test
all: $(TARGET)
$(TARGET): $(SRC) include/campuscompute.h
	$(CC) $(CFLAGS) $(SRC) -o $@ $(LDLIBS)
clean:
	rm -f $(TARGET) logs/campuscompute.log
	rm -f *.o src/*.o

test: $(TARGET)
	./campuscompute help
	./campuscompute fcfs
	./campuscompute rr 2
	./campuscompute priority
	./campuscompute banker safe
	./campuscompute banker unsafe
	./campuscompute worker normal
	./campuscompute worker fail
