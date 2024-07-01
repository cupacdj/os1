#ifndef OS1_MEMORYALLOCATOR_HPP
#define OS1_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"

class MemoryAllocator {

public:

    static void init();

    static void* mem_alloc(size_t size);

    static int mem_free(void* ptr);
    
    struct Block
    {
        Block* next;
        Block* prev;
        size_t size;
    };

    static int tryToMerge(Block* block);

private:
    
    static Block* head;


};

#endif //OS1_MEMORYALLOCATOR_HPP