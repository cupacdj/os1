//
// Created by marko on 20.4.22..
//

#ifndef OS1_SCHEDULER_HPP
#define OS1_SCHEDULER_HPP

#include "list.hpp"

class TCB;

class Scheduler
{
private:
    static List<TCB> readyThreadQueue;

public:
    static TCB *get();

    static void put(TCB *ccb);

};

#endif //OS1_SCHEDULER_HPP
