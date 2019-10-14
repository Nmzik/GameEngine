#pragma once
#ifdef __APPLE__
#include <pthread.h>
#else
#include <Windows.h>
#endif

class Thread {
#ifdef __APPLE__
    pthread_t thread_id;
#else
    HANDLE thread_id;
#endif
public:
    Thread()
    {
        
    }
    
    Thread(void *(*start_routine) (void *), void *arg)
    {
#ifdef __APPLE__
        pthread_create(&thread_id, NULL, start_routine, arg);
#else
        thread_id = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)start_routine, arg, 0, 0);
#endif
    }
    
    ~Thread()
    {
#ifndef __APPLE__
        CloseHandle(thread_id);
#endif
    }
    
    void join()
    {
#ifdef __APPLE__
        pthread_join(thread_id, NULL);
#else
        WaitForSingleObject(thread_id, INFINITE);
#endif
    }
};

