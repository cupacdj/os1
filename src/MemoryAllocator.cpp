#include "MemoryAllocator.hpp"
#include "../lib/hw.h"

MemoryAllocator::Block* MemoryAllocator::head = nullptr;

MemoryAllocator* MemoryAllocator::getInstance() {
    if(instance == nullptr) {
        head = (Block*)HEAP_START_ADDR;
        head->next = nullptr;
        head->prev = nullptr;
        head->size = (size_t)HEAP_END_ADDR - (size_t)HEAP_START_ADDR;
        instance = (MemoryAllocator*)MemoryAllocator::mem_alloc((sizeof(MemoryAllocator)+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE);
    }
    return instance;
}

void* MemoryAllocator::mem_alloc(size_t size) {
    
}