#pragma once
#if defined(__APPLE__) || defined(__ANDROID__)
#include <pthread.h>
#else
#include <Windows.h>
#endif

class Thread
{
#if defined(__APPLE__) || defined(__ANDROID__)
    pthread_t thread_id;
#else
    HANDLE thread_id;
#endif
public:
    Thread()
    {
    }

    Thread(void* (*start_routine)(void*), void* arg)
    {
#if defined(__APPLE__) || defined(__ANDROID__)
        pthread_create(&thread_id, NULL, start_routine, arg);
#else
        thread_id = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)start_routine, arg, 0, 0);
#endif
    }

    ~Thread()
    {
#if defined(__APPLE__) || defined(__ANDROID__)

#else
        CloseHandle(thread_id);
#endif
    }

    void join()
    {
#if defined(__APPLE__) || defined(__ANDROID__)
        pthread_join(thread_id, NULL);
#else
        WaitForSingleObject(thread_id, INFINITE);
#endif
    }
};