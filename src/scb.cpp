#include "../h/scb.hpp"

SCB* SCB::createSemaphore(size_t init) {
    return new SCB(init);
}

int SCB::closeSemaphore(){
    if(isClosed()) return -1;

    setClosed(true);
    if (blockedThreads.peekLast() != nullptr) {
        for(int i = 0; i < cntBlck; i++){
            TCB* thread = blockedThreads.removeLast();
            thread->setBlocked(false);
            Scheduler::put(thread);
        }
    }
    cntBlck = 0;
    return 0;
}

int SCB::wait(){
    if(isClosed()) return -1;

    if (value <= 0) {
        TCB::running->setBlocked(true);
        blockedThreads.addLast(TCB::running);
        cntBlck++;
        TCB::dispatch();
    } else {
        value--;
    }
    return isClosed() ? -1 : 0;
}

int SCB::signal(){
    if(isClosed()) return -1;

    if (cntBlck > 0) {
        TCB* thread = blockedThreads.removeLast();
        thread->setBlocked(false);
        Scheduler::put(thread);
        cntBlck--;
    } else {
        value++;
    }
    return 0;
}

int SCB::tryWait(){
    if(isClosed()) return -1;

    if (value <= 0) {
        return 1;
    } else {
        value--;
        return 0;
    }
}
