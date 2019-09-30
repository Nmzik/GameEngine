#pragma once
#include <pthread.h>

class Thread {
#ifdef __APPLE__
    pthread_t thread_id;
#endif
public:
    Thread()
    {
        
    }
    
    Thread(void *(*start_routine) (void *), void *arg)
    {
        pthread_create(&thread_id, NULL, start_routine, arg);
    }
    
    ~Thread()
    {
        
    }
    
    void join()
    {
        pthread_join(thread_id, NULL);
    }
};

