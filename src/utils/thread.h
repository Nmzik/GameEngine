#pragma once
#include <pthread.h>

class thread {
#ifdef __APPLE__
    pthread_t thread_id;
#endif
public:
    thread()
    : thread_id(0)
    {
        
    }
    
    thread(void *(*start_routine) (void *), void *arg)
    {
        pthread_create(&thread_id, NULL, start_routine, arg);
    }
    
    ~thread()
    {
        
    }
    
    void join()
    {
        pthread_join(thread_id, NULL);
    }
};

