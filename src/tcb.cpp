#include "../h/tcb.hpp"
#include "../h/riscv.hpp"

#include "../h/syscall_c.hpp"

TCB *TCB::running = nullptr;


TCB *TCB::createThread(Body body, void* arg, char* stack)
{
    return new TCB(body, arg, stack);
}

void TCB::dispatch()
{
    TCB *old = running;
    if (!old->isFinished() && !old->isBlocked()) { Scheduler::put(old); }
    running = Scheduler::get();

    TCB::contextSwitch(&old->context, &running->context);
}

void TCB::threadWrapper()
{
    Riscv::popSppSpie();
    running->body(running->arg);
    thread_exit();
}
