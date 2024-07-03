#ifndef OS1_TCB_HPP
#define OS1_TCB_HPP

#include "../lib/hw.h"
#include "scheduler.hpp"

// Thread Control Block
class TCB {

public:
    ~TCB() { delete[] stack; }

    bool isFinished() const { return finished; }

    void setFinished(bool value) { finished = value; }

    bool isBlocked() const { return blocked; }

    void setBlocked(bool value) { blocked = value; }

    using Body = void (*)(void*);

    static TCB *createThread(Body body, void* arg, char* stack);

    static TCB *running;

private:
    TCB(Body body, void* arg, char* stack) :
            body(body),
            arg(arg),
            stack(stack),
            context({(size_t) &threadWrapper,
                     stack != nullptr ? (size_t) &stack[DEFAULT_STACK_SIZE] : 0
                    }),
            finished(false),
            blocked(false)
    {}

    struct Context
    {
        size_t ra;
        size_t sp;
    };

    
    Body body;
    void* arg;
    char *stack;
    Context context;
    bool finished;
    bool blocked;
    size_t id;

    friend class Riscv;
    friend class SCB;

    static void threadWrapper();

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static void dispatch();

    friend bool operator<(const TCB& t1, const TCB& t2) { return t1.id < t2.id; }

};

#endif //OS1_TCB_HPP
