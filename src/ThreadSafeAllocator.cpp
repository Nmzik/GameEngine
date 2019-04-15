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
    std::lock_guard<std::mutex> lock(allocatorLock);
    return FreeListAllocator::allocate(size, alignment);
}

void ThreadSafeAllocator::deallocate(void* p)
{
    std::lock_guard<std::mutex> lock(allocatorLock);
    FreeListAllocator::deallocate(p);
}