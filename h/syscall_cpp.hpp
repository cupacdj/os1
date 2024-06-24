#ifndef _syscall_cpp
#define _syscall_cpp

#include "../lib/hw.h"
#include "syscall_c.hpp"

class Thread {
public:
    Thread(void (*body)(void *), void *arg);

    virtual ~Thread();

    int start();

    static void dispatch();
    static int sleep(time_t);

protected:
    Thread();
    virtual void run() {}

    static void runWrapper(void *me);

private:
    thread_t myHandle;

    void (*body)(void *); void *arg;
};


class Semaphore {
public:

    explicit Semaphore(unsigned init);

    ~Semaphore();

    int wait();

    int signal();

    int timedWait(time_t);

    int tryWait();


protected:
    sem_t myHandle;

};


class PeriodicThread : public Thread {
public:
    void terminate() {period = 0;};

protected:
    explicit PeriodicThread(time_t period): period(period) {};

    virtual void periodicActivation() {}

    void run() override final {};

private:
    time_t period;
};

class Console {
public:
    static char getc();

    static void putc(char);
};

#endif