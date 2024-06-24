#include "../h/tcb.hpp"
#include "../h/print.hpp"
#include "../h/riscv.hpp"

#include "../h/MemoryAllocator.hpp"

#include "../h/syscall_cpp.hpp"

extern void userMain();

void userThreadFn(void* arg)
{
    userMain();
    ((Semaphore*)arg)->signal();
}
int main()
{
    MemoryAllocator::init();
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);

    TCB::running = TCB::createThread(nullptr, nullptr, nullptr);

    Semaphore *sem = new Semaphore(0);

    // thread_t userThread;
    // thread_create(&userThread, userThreadFn, sem);

    Thread *userThread = new Thread(userThreadFn, sem);

    userThread->start();

    sem->wait();


    return 0;
}
