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

private:
    SCB(size_t init) : value(init), closed(false), cntBlck(0) {}

    int cntBlck;
    bool closed;
    size_t value;
    List<TCB> blockedThreads;

};