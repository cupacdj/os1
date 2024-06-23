#include "../h/syscall_c.hpp"

size_t ecall(size_t code, ...) {
    size_t volatile ret;
    asm volatile("ecall");
    asm volatile ("mv %[ret], a0" : [ret] "=r"(ret));
    return ret;
}

void* mem_alloc(size_t size) {
    return (void*)ecall(MEM_ALLOC, (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
}

int mem_free(void* ptr) {
    return ecall(MEM_FREE, ptr);
}

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg) {
    void* stack = mem_alloc(DEFAULT_STACK_SIZE);
    return ecall(THREAD_CREATE, handle, start_routine, arg, stack);
}

int thread_exit() {
    return ecall(THREAD_EXIT);
}

void thread_dispatch() {
    ecall(THREAD_DISPATCH);
}

int sem_open(sem_t* handle, unsigned init) {
    return ecall(SEM_OPEN, handle, init);
}

int sem_close(sem_t handle) {
    return ecall(SEM_CLOSE, handle);
}

int sem_wait(sem_t id) {
    return ecall(SEM_WAIT, id);
}

int sem_signal(sem_t id) {
    return ecall(SEM_SIGNAL, id);
}

int sem_trywait(sem_t id) {
    return ecall(SEM_TRYWAIT, id);
}

int sem_timedwait(sem_t id, time_t timeout) {
    return ecall(SEM_TIMEDWAIT, id, timeout);
}

int time_sleep(time_t time) {
    return ecall(TIME_SLEEP, time);
}

char getc() {
    return ecall(GETC);
}

void putc(char c) {
    ecall(PUTC, c);
}