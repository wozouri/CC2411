#ifndef __TOOL_H__
#define __TOOL_H__

#include <math.h>
#include <unistd.h>
#include <errno.h>

#define ll long long

enum direction
{
    dir_left,
    dir_up,
    dir_right,
    dir_down
}; // paramotoring

int max(int x, int y);

int min(int x, int y);

ll qmi(ll x, ll y);

void p_x_y_st(int touch_x, int touch_y, char *status);

void start_print(char *name);

void end_print(char *name);

void msleep(int ms);
#endif