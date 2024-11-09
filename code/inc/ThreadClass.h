#ifndef __THREAD_CLASS_H
#define __THREAD_CLASS_H

#include <pthread.h>

typedef struct ThreadClass
{
    pthread_t thread_id;
    void (*func)(void *);
    void *arg;
    int should_exit;
} ThreadClass;

void ThreadClass_Init(ThreadClass *tc, void (*func)(void *), void *arg, int st);
void ThreadClass_Start(ThreadClass *tc);
void ThreadClass_RequestExit(ThreadClass *tc);

#endif