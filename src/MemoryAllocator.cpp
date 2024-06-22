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
    if(size <= 0) return nullptr;

    size_t numOfBlocks = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE + 1;
    size_t numOfBlocksBytes = numOfBlocks * MEM_BLOCK_SIZE;

    Block* curr = head;
    if(curr == nullptr) {
        return nullptr;
    }

    while(curr != nullptr ){
        if(curr->size >= numOfBlocksBytes){
            Block* newFreeBlock = (Block*)((char*)curr + numOfBlocksBytes);
            newFreeBlock->size = curr->size - numOfBlocksBytes;
            newFreeBlock->next = curr->next;
            newFreeBlock->prev = curr;
            if(curr->next != nullptr){
                curr->next->prev = newFreeBlock;
            }
            curr->next = newFreeBlock;
            curr->size = numOfBlocksBytes;

            if(curr->prev == nullptr){
                head = curr->next;
            } else {
                curr->prev->next = curr->next;
            }
            if(curr->next != nullptr){
                curr->next->prev = curr->prev;
            }

            return (void*)((char*)curr + MEM_BLOCK_SIZE);
        }
        curr = curr->next;
    }
    return nullptr;
}

int MemoryAllocator::mem_free(void* ptr) {
    if(ptr == nullptr) return -1;

}   