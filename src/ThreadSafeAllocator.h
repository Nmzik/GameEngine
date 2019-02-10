#pragma once

/////////////////////////////////////////////////////////////////////////////////////////////
///////////////// Tiago Costa, 2014
/////////////////////////////////////////////////////////////////////////////////////////////

#include "Allocator.h"
#include <mutex>

class ThreadSafeAllocator : public Allocator
{
	public:
	ThreadSafeAllocator(size_t size, void* start);
	~ThreadSafeAllocator();

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

	ThreadSafeAllocator(const ThreadSafeAllocator&);
	ThreadSafeAllocator& operator=(const ThreadSafeAllocator&);

	FreeBlock* _free_blocks;
};