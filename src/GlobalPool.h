#pragma once
#include <stack>

template <typename Type>
class BasicPool
{
    //allocate a chunk of memory as big as Type needs:
    void* allocate()
    {
        void* place;
        if (!free.empty())
        {
            place = static_cast<void*>(free.top());
            free.pop();
        }
        else
        {
            place = operator new(sizeof(Type));
        }
        return place;
    }

    //mark some memory as available (no longer used):
    void deallocate(void* o)
    {
        free.push(static_cast<Type*>(o));
    }

public:
    BasicPool() {}

    //delete the copy constructor; we can't copy it:
    BasicPool(const BasicPool&) = delete;

    //move constructor; we can move it:
    BasicPool(BasicPool&& other) = delete;

    void preAllocate(int num)
    {
        for (size_t i = 0; i < num; i++)
        {
            Type* place = (Type*)operator new(sizeof(Type));
            free.push(place);
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
        free.push(o);
    }

    //delete all of the available memory chunks:
    ~BasicPool()
    {
        while (!free.empty())
        {
            ::operator delete(free.top());
            free.pop();
        }
    }

private:
    //stack to hold pointers to free chunks:
    std::stack<Type*> free;
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