#include "FreeListAllocator.h"

FreeListAllocator::FreeListAllocator(size_t size)
    : Allocator(size)
{
    _free_blocks = ((FreeBlock*)getMemoryPointer());

    _free_blocks->size = size - sizeof(FreeBlock);
    _free_blocks->next = nullptr;
}

FreeListAllocator::~FreeListAllocator()
{
}

void* FreeListAllocator::allocate(size_t size, uint8_t alignment)
{
    FreeBlock* prev_free_block = nullptr;
    FreeBlock* free_block = _free_blocks;

    FreeBlock* best_fit_prev = nullptr;
    FreeBlock* best_fit = nullptr;
    uint8_t best_fit_adjustment = 0;
    size_t best_fit_total_size = 0;

    //	Find best fit
    while (free_block != nullptr)
    {
        //	Calculate adjustment needed to keep object correctly aligned
        uint8_t adjustment = pointer_math::alignForwardAdjustmentWithHeader<AllocationHeader>(free_block, alignment);

        size_t total_size = size + adjustment;

        //	If its an exact match use this free block
        if (free_block->size == total_size)
        {
            best_fit_prev = prev_free_block;
            best_fit = free_block;
            best_fit_adjustment = adjustment;
            best_fit_total_size = total_size;

            break;
        }

        //	If its a better fit switch
        if (free_block->size > total_size && (best_fit == nullptr || free_block->size < best_fit->size))
        {
            best_fit_prev = prev_free_block;
            best_fit = free_block;
            best_fit_adjustment = adjustment;
            best_fit_total_size = total_size;
        }

        prev_free_block = free_block;
        free_block = free_block->next;
    }

    if (best_fit == nullptr)
        return nullptr;

    //	If allocations in the remaining memory will be impossible
    if (best_fit->size - best_fit_total_size <= sizeof(AllocationHeader))
    {
        //	Increase allocation size instead of creating a new FreeBlock
        best_fit_total_size = best_fit->size;

        if (best_fit_prev != nullptr)
            best_fit_prev->next = best_fit->next;
        else
            _free_blocks = best_fit->next;
    }
    else
    {
        //	Else create a new FreeBlock containing remaining memory
        FreeBlock* new_block = (FreeBlock*)(pointer_math::add(best_fit, best_fit_total_size));
        new_block->size = best_fit->size - best_fit_total_size;
        new_block->next = best_fit->next;

        if (best_fit_prev != nullptr)
            best_fit_prev->next = new_block;
        else
            _free_blocks = new_block;
    }

    uptr aligned_address = (uptr)best_fit + best_fit_adjustment;

    AllocationHeader* header = (AllocationHeader*)(aligned_address - sizeof(AllocationHeader));
    header->size = best_fit_total_size;
    header->adjustment = best_fit_adjustment;

    _used_memory += best_fit_total_size;
    _num_allocations++;

    //-----------------------------------------------------------

    return (void*)aligned_address;
}

void FreeListAllocator::deallocate(void* p)
{
    AllocationHeader* header = (AllocationHeader*)pointer_math::subtract(p, sizeof(AllocationHeader));

    uptr block_start = reinterpret_cast<uptr>(p) - header->adjustment;
    size_t block_size = header->size;
    uptr block_end = block_start + block_size;

    FreeBlock* prev_free_block = nullptr;
    FreeBlock* free_block = _free_blocks;

    while (free_block != nullptr)
    {
        if ((uptr)free_block >= block_end)
            break;

        prev_free_block = free_block;
        free_block = free_block->next;
    }

    if (prev_free_block == nullptr)
    {
        prev_free_block = (FreeBlock*)block_start;
        prev_free_block->size = block_size;
        prev_free_block->next = _free_blocks;

        //	free_block   = _free_blocks;
        _free_blocks = prev_free_block;
    }
    else if ((uptr)prev_free_block + prev_free_block->size == block_start)
    {
        prev_free_block->size += block_size;
    }
    else
    {
        FreeBlock* temp = (FreeBlock*)block_start;
        temp->size = block_size;
        temp->next = prev_free_block->next;

        prev_free_block->next = temp;

        prev_free_block = temp;
    }
    /*
	if(free_block != nullptr && (uptr)free_block == block_end)
	{
	 prev_free_block->size += free_block->size;
	 prev_free_block->next = free_block->next;
	}
	*/

    if ((uptr)prev_free_block + prev_free_block->size == (uptr)prev_free_block->next)
    {
        prev_free_block->size += prev_free_block->next->size;
        prev_free_block->next = prev_free_block->next->next;
    }

    _num_allocations--;
    _used_memory -= block_size;

    //--------------------------------------------------
}