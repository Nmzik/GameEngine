#pragma once
#if defined(__APPLE__) || defined(__ANDROID__)
#include <pthread.h>
#else
#include <Windows.h>
#endif
class Mutex
{
#if defined(__APPLE__) || defined(__ANDROID__)
    pthread_mutex_t mutex_lock;
#else
    CRITICAL_SECTION mutex_lock;
#endif
    friend class Condition_variable;

public:
    Mutex()
    {
#if defined(__APPLE__) || defined(__ANDROID__)
        pthread_mutex_init(&mutex_lock, NULL);
#else
        InitializeCriticalSection(&mutex_lock);
#endif
    }

    ~Mutex()
    {
#if defined(__APPLE__) || defined(__ANDROID__)
        pthread_mutex_destroy(&mutex_lock);
#else
        DeleteCriticalSection(&mutex_lock);
#endif
    }

    void lock()
    {
#if defined(__APPLE__) || defined(__ANDROID__)
        pthread_mutex_lock(&mutex_lock);
#else
        EnterCriticalSection(&mutex_lock);
#endif
    }

    void unlock()
    {
#if defined(__APPLE__) || defined(__ANDROID__)
        pthread_mutex_unlock(&mutex_lock);
#else
        LeaveCriticalSection(&mutex_lock);
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
#if defined(__APPLE__) || defined(__ANDROID__)
    pthread_cond_t cv;
#else
    CONDITION_VARIABLE cv;
#endif
public:
    Condition_variable()
    {
#if defined(__APPLE__) || defined(__ANDROID__)
        pthread_cond_init(&cv, NULL);
#else
        InitializeConditionVariable(&cv);
#endif
    }

    ~Condition_variable()
    {
#if defined(__APPLE__) || defined(__ANDROID__)
        pthread_cond_destroy(&cv);
#else
        //
#endif
    }

    void wait(Mutex* lock)
    {
#if defined(__APPLE__) || defined(__ANDROID__)
        pthread_cond_wait(&cv, &lock->mutex_lock);
#else
        SleepConditionVariableCS(&cv, &lock->mutex_lock, INFINITE);
#endif
    }

    void notify()
    {
#if defined(__APPLE__) || defined(__ANDROID__)
        pthread_cond_signal(&cv);
#else
        WakeConditionVariable(&cv);
#endif
    }
};