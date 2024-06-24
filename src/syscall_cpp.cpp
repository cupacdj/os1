#include "../h/syscall_cpp.hpp"
#include "../h/syscall_c.hpp"
#include "../h/tcb.hpp"

Thread::Thread(void (*body)(void *), void *arg): myHandle(nullptr), body(body), arg(arg) {}

Thread::Thread(): myHandle(nullptr), body(runWrapper), arg(this) {}

void Thread::runWrapper(void *me) {
    ((Thread *) me)->run();
}

int Thread::start() {
    return thread_create(&myHandle, body, arg);
}

Thread::~Thread() {
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t time) {
    return time_sleep(time);
}

Semaphore::Semaphore(unsigned init) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

int Semaphore::timedWait(time_t time) {
    return sem_timedwait(myHandle, time);
}

int Semaphore::tryWait() {
    return sem_trywait(myHandle);
}

void Console::putc(char c) {
    ::putc(c);
}

char Console::getc() {
    return ::getc();
}

