
#ifndef OS1_SCHEDULER_HPP
#define OS1_SCHEDULER_HPP

#include "list.hpp"

class CCB;

class Scheduler
{
private:
    static List<CCB> readyCoroutineQueue;

public:
    static CCB *get();

    static void put(CCB *ccb);

};

#endif //OS1_SCHEDULER_HPP
