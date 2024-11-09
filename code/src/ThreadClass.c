#include "ThreadClass.h"

void ThreadClass_Init(ThreadClass *tc, void (*func)(void *), void *arg, int st)
{
    tc->func = func;
    tc->arg = arg;
    tc->should_exit = st;
}

void ThreadClass_Start(ThreadClass *tc)
{
    pthread_create(&(tc->thread_id), NULL, tc->func, tc->arg);
}

void ThreadClass_RequestExit(ThreadClass *tc)
{
    tc->should_exit = 1;
}