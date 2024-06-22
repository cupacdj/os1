#ifndef OS1_MEMORYALLOCATOR_HPP
#define OS1_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"

// this class needs to be Singleton 
class MemoryAllocator {

public:

    MemoryAllocator(const MemoryAllocator&) = delete;
    MemoryAllocator& operator=(const MemoryAllocator&) = delete;

    static MemoryAllocator* getInstance();

    static void* mem_alloc(size_t size);

    static int mem_free(void* ptr);

    
    struct Block
    {
        Block* next;
        Block* prev;
        size_t size;
    };

private:
    
    static Block* head;
    static MemoryAllocator* instance;
    MemoryAllocator(){};


};

#endif //OS1_MEMORYALLOCATOR_HPP