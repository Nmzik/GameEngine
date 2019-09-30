#pragma once
#include <pthread.h>

class Mutex {
#ifdef __APPLE__
    pthread_mutex_t mutex_lock;
#endif
    friend class Condition_variable;
public:
    Mutex()
    {
        pthread_mutex_init(&mutex_lock, NULL);
    }
    
    ~Mutex()
    {
        pthread_mutex_destroy(&mutex_lock);
    }
    
    void lock()
    {
        pthread_mutex_lock(&mutex_lock);
    }
    
    void unlock()
    {
        pthread_mutex_unlock(&mutex_lock);
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
    pthread_cond_t cv;
public:
    Condition_variable()
    {
        pthread_cond_init(&cv, NULL);
    }
    
    ~Condition_variable()
    {
        pthread_cond_destroy(&cv);
    }
    
    void wait(Mutex* lock)
    {
        pthread_cond_wait(&cv, &lock->mutex_lock);
    }
    
    void notify()
    {
        pthread_cond_signal(&cv);
    }
};

