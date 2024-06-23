#ifndef SYSCALL_C_HPP
#define SYSCALL_C_HPP

#include "../lib/hw.h"

enum syscalls {
    MEM_ALLOC       = 0x01,
    MEM_FREE        = 0x02,
    THREAD_CREATE   = 0x11,
    THREAD_EXIT     = 0x12,
    THREAD_DISPATCH = 0x13,
    SEM_OPEN        = 0x21,
    SEM_CLOSE       = 0x22,
    SEM_WAIT        = 0x23,
    SEM_SIGNAL      = 0x24,
    SEM_TRYWAIT     = 0x25,
    SEM_TIMEDWAIT   = 0x26,
    TIME_SLEEP      = 0x31,
    GETC            = 0x41,
    PUTC            = 0x42,
    GOTO_USER       = 0x50,
    GOTO_SYSTEM     = 0x51,
};

class TCB;
typedef TCB *thread_t;

class _sem;
typedef _sem* sem_t;

typedef unsigned long time_t;

const int EOF = -1;


void* mem_alloc(size_t size);

int mem_free(void*);

int thread_create(thread_t* handle, void(*start_routine)(void*),void* arg);

int thread_exit();

void thread_dispatch();


int sem_open(sem_t* handle,unsigned init);

int sem_close(sem_t handle);

int sem_wait(sem_t id);

int sem_signal (sem_t id);

int sem_trywait(sem_t id);

int sem_timedwait(sem_t id, time_t timeout);

int time_sleep (time_t);

char getc ();

void putc (char);

#endif //SYSCALL_C_HPP