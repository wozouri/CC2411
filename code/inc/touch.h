#ifndef __TOUCH_H__
#define __TOUCH_H__

#include "button.h"
#include "lcd.h"
#include "linux/input.h"

extern int touch_x, touch_y, touch_xx, touch_yy;
extern int touch_fd;
extern int slide_direction;
extern struct input_event ts;

int touch_init();

int event_xy(); // 2:press 1:release 未返回（一直循环）:move

int check_range(BT_info app);

int check_slide();

#endif