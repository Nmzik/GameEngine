#pragma once

/////////////////////////////////////////////////////////////////////////////////////////////
///////////////// Tiago Costa, 2014
/////////////////////////////////////////////////////////////////////////////////////////////

#include "Allocator.h"

class FreeListAllocator : public Allocator
{
public:
    FreeListAllocator(size_t size);
    ~FreeListAllocator() override;

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

    static_assert(sizeof(AllocationHeader) >= sizeof(FreeBlock), "sizeof(AllocationHeader) < sizeof(FreeBlock)");

    FreeListAllocator(const FreeListAllocator&) = delete;
    FreeListAllocator& operator=(const FreeListAllocator&) = delete;

    FreeBlock* _free_blocks;
};
