#pragma once
#include <pthread.h>

class mutex {
#ifdef __APPLE__
    pthread_mutex_t mutex_lock;
#endif
    friend class condition_variable;
public:
    mutex()
    {
        pthread_mutex_init(&mutex_lock, NULL);
    }
    
    ~mutex()
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

class lock_guard
{
    mutex* mutex_lock;
public:
    lock_guard(mutex* lock)
    : mutex_lock(lock)
    {
        mutex_lock->lock();
    }
    
    ~lock_guard()
    {
        mutex_lock->unlock();
    }
};

class condition_variable
{
    pthread_cond_t cv;
public:
    condition_variable()
    {
        pthread_cond_init(&cv, NULL);
    }
    
    ~condition_variable()
    {
        pthread_cond_destroy(&cv);
    }
    
    void wait(mutex* lock)
    {
        pthread_cond_wait(&cv, &lock->mutex_lock);
    }
    
    void notify()
    {
        pthread_cond_signal(&cv);
    }
};

