#pragma once

/////////////////////////////////////////////////////////////////////////////////////////////
///////////////// Tiago Costa, 2015
/////////////////////////////////////////////////////////////////////////////////////////////

#include <new>
#include <functional>

static const uint8_t DEFAULT_ALIGNMENT = 8;

class Allocator
{
public:
	Allocator(size_t size);
	virtual ~Allocator();

	virtual void* allocate(size_t size, uint8_t alignment = DEFAULT_ALIGNMENT) = 0;

	virtual void deallocate(void* p) = 0;

	//virtual void check(void* p) = 0;

	size_t getSize() const;
	size_t getUsedMemory() const;
	size_t getNumAllocations() const;

protected:
	Allocator(const Allocator&);
	Allocator& operator=(Allocator&);

	size_t        _size;

	size_t        _used_memory;
	size_t        _num_allocations;
};