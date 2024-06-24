#include "../h/tcb.hpp"
#include "../h/print.hpp"
#include "../h/riscv.hpp"

#include "../h/MemoryAllocator.hpp"

#include "../h/syscall_c.hpp"

extern void userMain();

void userThreadFn(void* arg)
{
    userMain();
}
int main()
{
    MemoryAllocator::init();
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);

    TCB::running = TCB::createThread(nullptr, nullptr, nullptr);

    thread_t userThread;
    thread_create(&userThread, userThreadFn, nullptr);

    while (1)
    {
        thread_dispatch();
    }
   

    return 0;
}
