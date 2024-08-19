#ifndef SCB_HPP
#define SCB_HPP

#include "../h/list.hpp"
#include "../h/tcb.hpp"

// Semaphore Control Block
class SCB {

public:
    static SCB* createSemaphore(size_t init);
    
    bool isClosed() const { return closed; }

    void setClosed(bool value) { closed = value; }

    int signal();
    
    int wait();

    int closeSemaphore();

    int tryWait();

private:
    SCB(size_t init) : value(init), closed(false), cntBlck(0) {}

    size_t value;
    bool closed;
    int cntBlck;
    List<TCB> blockedThreads;

};

#endif //SCB_HPP