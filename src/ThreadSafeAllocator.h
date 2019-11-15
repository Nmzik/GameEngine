#pragma once

/////////////////////////////////////////////////////////////////////////////////////////////
///////////////// Tiago Costa, 2014
/////////////////////////////////////////////////////////////////////////////////////////////

#include "FreeListAllocator.h"

#include "utils/mutex.h"

class ThreadSafeAllocator : public FreeListAllocator
{
public:
    ThreadSafeAllocator(size_t size);
    ~ThreadSafeAllocator() override;

    void* allocate(size_t size, uint8_t alignment) override;

    void deallocate(void* p) override;

private:
    Mutex allocatorLock;

    ThreadSafeAllocator(const ThreadSafeAllocator&) = delete;
    ThreadSafeAllocator& operator=(const ThreadSafeAllocator&) = delete;
};
