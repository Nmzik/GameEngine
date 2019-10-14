#pragma once
#include <vector>
#include <cassert>

template <typename Type>
class BasicPool
{
    //allocate a chunk of memory as big as Type needs:
    void* allocate()
    {
        assert(!free.empty());
        void* place = static_cast<void*>(free.back());
        free.pop_back();
        return place;
    }

public:
    BasicPool() {}

    //delete the copy constructor; we can't copy it:
    BasicPool(const BasicPool&) = delete;

    //move constructor; we can move it:
    BasicPool(BasicPool&& other) = delete;

    void preAllocate(int num)
    {
        memPool = new uint8_t[num * sizeof(Type)];

        for (size_t i = 0; i < num; i++)
        {
            free.push_back(memPool + (i * sizeof(Type)));
        }
    }

    //#### create an instance of Type:
    template <typename... Args>
    Type* create(Args&&... args)
    {
        Type* place = (Type*)(allocate());
        new (place) Type(std::forward<Args>(args)...);
        return place;
    }

    //#### remove an instance of Type (add memory to the pool):
    void remove(Type* o)
    {
        o->~Type();
        free.push_back((uint8_t*)o);
    }

    //delete all of the available memory chunks:
    ~BasicPool()
    {
        delete[] memPool;
    }

private:
    uint8_t* memPool;
    //stack to hold pointers to free chunks:
    std::vector<uint8_t*> free;
};

class Resource;
class YdrLoader;
class YddLoader;
class YftLoader;
class YbnLoader;
class YtdLoader;
class YmapLoader;

class CPed;
class CVehicle;

class GlobalPool
{
    static GlobalPool pool;

public:
    BasicPool<Resource> resourcesPool;
    BasicPool<YdrLoader> ydrPool;
    BasicPool<YddLoader> yddPool;
    BasicPool<YftLoader> yftPool;
    BasicPool<YbnLoader> ybnPool;
    BasicPool<YtdLoader> ytdPool;
    BasicPool<YmapLoader> ymapPool;
    //
    BasicPool<CPed> CPedPool;
    BasicPool<CVehicle> CVehiclePool;

    static GlobalPool* GetInstance()
    {
        return &pool;
    }
};
