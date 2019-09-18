#include "ThreadSafeAllocator.h"

ThreadSafeAllocator::ThreadSafeAllocator(size_t size)
    : FreeListAllocator(size)
{
}

ThreadSafeAllocator::~ThreadSafeAllocator()
{
}

void* ThreadSafeAllocator::allocate(size_t size, uint8_t alignment)
{
    lock_guard lock(&allocatorLock);
    return FreeListAllocator::allocate(size, alignment);
}

void ThreadSafeAllocator::deallocate(void* p)
{
    lock_guard lock(&allocatorLock);
    FreeListAllocator::deallocate(p);
}
