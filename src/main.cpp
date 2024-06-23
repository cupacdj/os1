//
// Created by marko on 20.4.22..
//

#include "../h/tcb.hpp"
#include "../h/workers.hpp"
#include "../h/print.hpp"
#include "../h/riscv.hpp"

#include "../h/MemoryAllocator.hpp"

#include "../h/syscall_c.hpp"

int main()
{
    MemoryAllocator::init();
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);

    mem_alloc(500);

    TCB *threads[5];

    threads[0] = TCB::createThread(nullptr);
    TCB::running = threads[0];

    threads[1] = TCB::createThread(workerBodyA);
    printString("ThreadA created\n");
    threads[2] = TCB::createThread(workerBodyB);
    printString("ThreadB created\n");
    threads[3] = TCB::createThread(workerBodyC);
    printString("ThreadC created\n");
    threads[4] = TCB::createThread(workerBodyD);
    printString("ThreadD created\n");

    while (!(threads[1]->isFinished() &&
             threads[2]->isFinished() &&
             threads[3]->isFinished() &&
             threads[4]->isFinished()))
    {
        thread_dispatch();
    }

    for (auto &thread: threads)
    {
        delete thread;
    }
    printString("Finished\n");

    return 0;
}
