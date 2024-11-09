#include "touch.h"

int touch_x = 0;
int touch_y = 0;
int touch_xx = 0;
int touch_yy = 0;
int touch_fd;
int slide_direction = 70; // px

struct input_event ts;
int touch_init()
{
    touch_fd = open("/dev/input/event0", O_RDWR);
    if (touch_fd == -1)
    {
        perror("open touch failed");
        return -1;
    }
    return 0;
}

int event_xy()
{
    while (read(touch_fd, &ts, sizeof(ts)) == sizeof(ts))
    {
        if (ts.type == EV_ABS)
        {
            if (ts.code == ABS_X)
            {
                touch_x = ts.value;
            }
            else if (ts.code == ABS_Y)
            {
                touch_y = ts.value;
            }
        }
        else if (ts.type == EV_KEY && ts.code == BTN_TOUCH && ts.value == 1)
        {
            // 触摸屏被按下
            touch_xx = touch_x;
            touch_yy = touch_y;
            return 2;
        }
        else if (ts.type == EV_KEY && ts.code == BTN_TOUCH && ts.value == 0)
        {
            // 触摸屏被释放
            return 1;
        }
    }
    return 0; // 读取失败或其他未知事件
}

int check_range(BT_info app)
{
    return touch_x >= app.x * 1.3 && touch_x <= (app.x + app.width) * 1.3 &&
           touch_y >= app.y * 1.25 && touch_y <= (app.y + app.height) * 1.25;
}

int check_slide()
{
    if (touch_x < touch_xx && abs(touch_x - touch_xx) > slide_direction) // left
    {
        return dir_left;
    }
    else if (touch_y < touch_yy && abs(touch_y - touch_yy) > slide_direction) // up
    {
        return dir_up;
    }
    else if (touch_x > touch_xx && abs(touch_x - touch_xx) > slide_direction) // right
    {
        return dir_right;
    }
    else if (touch_y > touch_yy && abs(touch_y - touch_yy) > slide_direction) // down
    {
        return dir_down;
    }
    return -1;
}
