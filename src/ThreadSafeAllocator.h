#pragma once

/////////////////////////////////////////////////////////////////////////////////////////////
///////////////// Tiago Costa, 2014
/////////////////////////////////////////////////////////////////////////////////////////////

#include <mutex>
#include "Allocator.h"

class ThreadSafeAllocator : public Allocator
{
public:
    ThreadSafeAllocator(size_t size, void* start);
    ~ThreadSafeAllocator() override;

    void* allocate(size_t size, uint8_t alignment) override;

    void deallocate(void* p) override;

private:
    struct AllocationHeader
    {
        size_t size;
        uint8_t adjustment;
    };

    struct FreeBlock
    {
        size_t size;
        FreeBlock* next;
    };

    std::mutex allocatorLock;

    static_assert(sizeof(AllocationHeader) >= sizeof(FreeBlock), "sizeof(AllocationHeader) < sizeof(FreeBlock)");

    ThreadSafeAllocator(const ThreadSafeAllocator&) = delete;
    ThreadSafeAllocator& operator=(const ThreadSafeAllocator&) = delete;

    FreeBlock* _free_blocks;
};