#include "tool.h"

int max(int x, int y)
{
    return x > y ? x : y;
}

int min(int x, int y)
{
    return x < y ? x : y;
}

ll qmi(ll x, ll y)
{
    ll ans = 1;
    while (y)
    {
        if (y & 1)
            ans = ans * x;
        x = x * x;
        y = y >> 1;
    }
    return ans;
}

void msleep(int ms)
{
    if (usleep(ms * 1000) == -1)
    {
        perror("usleep failed");
    }
}

void p_x_y_st(int touch_x, int touch_y, char *status)
{
    printf("touch_x = %d, touch_y = %d, status = %s\n", touch_x, touch_y, status);
}

void start_print(char *name)
{
    printf("\n*************** start : %s ***************\n\n", name);
}

void end_print(char *name)
{
    printf("\n*************** end : %s ***************\n\n", name);
}