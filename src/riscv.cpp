//
// Created by marko on 20.4.22..
//

#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../lib/console.h"

#include "../h/print.hpp"

#include "../h/syscall_c.hpp"

#include "../h/MemoryAllocator.hpp"

#include "../h/syscall_cpp.hpp"

#include "../h/scb.hpp"

void Riscv::popSppSpie()
{
    asm volatile("csrc sstatus, %0" : : "r" (SSTATUS_SPP));
    asm volatile("csrw sepc, ra");
    asm volatile("sret");
}

void Riscv::handleSupervisorTrap()
{
    uint64 scause = r_scause();
    uint64 volatile sepc = r_sepc();
    uint64 volatile sstatus = r_sstatus();
    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL)
    {
        // interrupt: no; cause code: environment call from U-mode(8) or S-mode(9)
        sepc += 4;

        size_t syscall_number = get_user_register(a0);

        switch (syscall_number)
        {
        case MEM_ALLOC:
        {
            size_t size = get_user_register(a1);
            size_t result = (size_t)MemoryAllocator::mem_alloc(size);
            set_user_register(a0, result);
            break;
        }
        case MEM_FREE:
        {
            void *ptr = (void *)get_user_register(a1);
            int result = MemoryAllocator::mem_free(ptr);
            set_user_register(a0, result);
            break;
        }
        case THREAD_CREATE:
        {
            thread_t *handle = (thread_t *)get_user_register(a1);
            void (*start_routine)(void *) = (void (*)(void *))get_user_register(a2);
            void *arg = (void *)get_user_register(a3);
            char *stack = (char *)get_user_register(a4);
            TCB* thread = TCB::createThread(start_routine, arg, stack);
            if (thread) { 
                Scheduler::put(thread); 
                *handle = thread;
                set_user_register(a0, 0);
            }
            else {
                *handle = nullptr;
                set_user_register(a0, -1);
            }
            break;
        }   
        case THREAD_EXIT:
        {
            TCB::running->setFinished(true);
            TCB::dispatch();
            break;
        }
        case THREAD_DISPATCH:
        {
            TCB::dispatch();
            break;
        }

        case SEM_OPEN:
        {
            sem_t *handle = (sem_t *)get_user_register(a1);
            size_t init = get_user_register(a2);
            auto sem = SCB::createSemaphore(init);
            if (sem) {
                *handle = sem;
                set_user_register(a0, 0);
            }
            else {
                *handle = nullptr;
                set_user_register(a0, -1);
            }
            break;
        }

        case SEM_CLOSE:
        {
            sem_t handle = (sem_t)get_user_register(a1);
            int ret = handle->closeSemaphore();
            set_user_register(a0, ret);
            break;
        }

        case SEM_WAIT:
        {
            sem_t handle = (sem_t)get_user_register(a1);
            int ret = handle->wait();
            set_user_register(a0, ret);
            break;
        }

        case SEM_SIGNAL:
        {
            sem_t handle = (sem_t)get_user_register(a1);
            int ret = handle->signal();
            set_user_register(a0, ret);
            break;
        }

        case SEM_TRYWAIT:
        {
            sem_t handle = (sem_t)get_user_register(a1);
            int ret = handle->tryWait();
            set_user_register(a0, ret);
            break;
        }

        case GETC:
        {
            char c = __getc();
            set_user_register(a0, c);
            break;
        }
        case PUTC:
        {
            char c = get_user_register(a1);
            __putc(c);
            break;
        }

 
        case TIME_SLEEP:
        case SEM_TIMEDWAIT:
        default:
            break;
        }



    }
    else if (scause == 0x8000000000000001UL)
    {
        // interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)
        mc_sip(SIP_SSIP);
        /*
        //ASYNC
        TCB::timeSliceCounter++;
        if (TCB::timeSliceCounter >= TCB::running->getTimeSlice())
        {
            uint64 volatile sepc = r_sepc();
            uint64 volatile sstatus = r_sstatus();
            TCB::timeSliceCounter = 0;
            TCB::dispatch();
            w_sstatus(sstatus);
            w_sepc(sepc);
        }*/
    }
    else if (scause == 0x8000000000000009UL)
    {
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
        console_handler();
    }
    else
    {
        // unexpected trap cause
        _printString("Unexpected trap cause: ");
        _printInteger(scause, 16);
        _printString(", sepc: ");
        _printInteger(sepc, 16);
        _printString("\n");
        while (1);
    }
    w_sstatus(sstatus);
    w_sepc(sepc);
}