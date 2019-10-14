#pragma once
#ifdef __APPLE__
#include <pthread.h>
#else
#include <Windows.h>
#endif
class Mutex {
#ifdef __APPLE__
    pthread_mutex_t mutex_lock;
#else
    CRITICAL_SECTION mutex_lock;
#endif
    friend class Condition_variable;
public:
    Mutex()
    {
#ifdef __APPLE__
        pthread_mutex_init(&mutex_lock, NULL);
#else
		InitializeCriticalSection(&mutex_lock);
#endif
    }
    
    ~Mutex()
    {
#ifdef __APPLE__
        pthread_mutex_destroy(&mutex_lock);
#else
        EnterCriticalSection(&mutex_lock);
#endif
    }
    
    void lock()
    {
#ifdef __APPLE__
        pthread_mutex_lock(&mutex_lock);
#else
        LeaveCriticalSection(&mutex_lock);
#endif
    }
    
    void unlock()
    {
#ifdef __APPLE__
        pthread_mutex_unlock(&mutex_lock);
#else
        DeleteCriticalSection(&mutex_lock);
#endif
    }
};

class Lock_guard
{
    Mutex* mutex_lock;
public:
    Lock_guard(Mutex* lock)
    : mutex_lock(lock)
    {
        mutex_lock->lock();
    }
    
    ~Lock_guard()
    {
        mutex_lock->unlock();
    }
};

class Condition_variable
{
#ifdef __APPLE__
    pthread_cond_t cv;
#else
    CONDITION_VARIABLE cv;
#endif
public:
    Condition_variable()
    {
#ifdef __APPLE__
        pthread_cond_init(&cv, NULL);
#else
        InitializeConditionVariable(&cv);
#endif
    }
    
    ~Condition_variable()
    {
#ifdef __APPLE__
        pthread_cond_destroy(&cv);
#else
        //
#endif
    }
    
    void wait(Mutex* lock)
    {
#ifdef __APPLE__
        pthread_cond_wait(&cv, &lock->mutex_lock);
#else
        SleepConditionVariableCS(&cv, &lock->mutex_lock, INFINITE);
#endif
    }
    
    void notify()
    {
#ifdef __APPLE__
        pthread_cond_signal(&cv);
#else
        WakeConditionVariable(&cv);
#endif
    }
};

