#include "../h/MemoryAllocator.hpp"
#include "../lib/hw.h"

MemoryAllocator::Block* MemoryAllocator::head = nullptr;

void MemoryAllocator::init() {
        head = (Block*)HEAP_START_ADDR;
        head->next = nullptr;
        head->prev = nullptr;
        head->size = (((size_t)HEAP_END_ADDR - (size_t)HEAP_START_ADDR) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;  // number of blocks
}

void* MemoryAllocator::mem_alloc(size_t size) {
    if(size <= 0) return nullptr;

    size_t numOfBlocksBytes = size*MEM_BLOCK_SIZE + MEM_BLOCK_SIZE;
    size_t numOfBlocks = size + 1;

    Block* curr = head;
    if(curr == nullptr) {
        return nullptr;
    }

    while(curr != nullptr ){
        if(curr->size == numOfBlocks){
            if(curr->prev == nullptr){
                head = curr->next;
            } else {
                curr->prev->next = curr->next;
            }
            if(curr->next != nullptr){
                curr->next->prev = curr->prev;
            }
            curr->next = nullptr;
            curr->prev = nullptr;
            return (void*)((char*)curr + MEM_BLOCK_SIZE);
        }
        else if(curr->size > numOfBlocks){
            Block* newFreeBlock = (Block*)((char*)curr + numOfBlocksBytes);
            newFreeBlock->size = curr->size - numOfBlocks;
            newFreeBlock->next = curr->next;
            newFreeBlock->prev = curr->prev;
            if(curr->next != nullptr){
                curr->next->prev = newFreeBlock;
            }
            curr->size = numOfBlocks;

            if(curr->prev == nullptr){
                head = newFreeBlock;
            } else {
                curr->prev->next = newFreeBlock;
            }

            curr->next = nullptr;
            curr->prev = nullptr;

            return (void*)((char*)curr + MEM_BLOCK_SIZE);
        }
        curr = curr->next;
    }
    return nullptr;
}
// logika je preuzeta iz 2. zadatka sa 2. kolokvijuma iz septembra 2015 sa predmeta OS1
int MemoryAllocator::tryToMerge(MemoryAllocator::Block* block){
    if(block == nullptr) return 0;
    if(block->next && (char*)block + block->size * MEM_BLOCK_SIZE == (char*)block->next){
        block->size += block->next->size;
        block->next = block->next->next;
        if(block->next){
            block->next->prev = block;
        }
        return 1;
    } else 
        return 0; 
}

int MemoryAllocator::mem_free(void* ptr) {
    if(ptr == nullptr) return -2;
    
    Block* freeBlk = (Block*)((char*)ptr - MEM_BLOCK_SIZE);
    Block* curr = head, *prev = nullptr;
    while(curr != nullptr){
        if(curr > freeBlk){
            break;
        }
        prev = curr;
        curr = curr->next;
    }
    if(prev == nullptr){
        head = freeBlk;
    } else {
        prev->next = freeBlk;
    }

    freeBlk->prev = prev;
    freeBlk->next = curr;
    if(curr != nullptr){
        curr->prev = freeBlk;
    }

    tryToMerge(freeBlk);
    tryToMerge(freeBlk->prev);
    
    return 0;
}   