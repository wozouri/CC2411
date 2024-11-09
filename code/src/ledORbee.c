#include "ledORbee.h"

int led_fd;
int bee_fd;
int ledORbee_init()
{
    led_fd = open("/dev/led_drv", O_RDWR);
    if (led_fd < 0)
    {
        printf("open failed\n");
        return -1;
    }

    bee_fd = open("/dev/pwm", O_RDWR);
    if (bee_fd < 0)
    {
        printf("open failed\n");
        return -1;
    }
}

void trun_led(int led_fd, int num, int *status)
{
    char led[2] = {0, num};
    if (*status)
    {
        write(led_fd, led, 2);
        *status ^= 1;
    }
    else
    {
        led[0] = 1;
        write(led_fd, led, 2);
        *status ^= 1;
    }
}

int d[3];
void Alarm()
{
    int bee = 1;
    while (1)
    {
        for (int i = 8; i <= 10; i++)
        {
            bee ^= 1;
            trun_led(led_fd, i, &d[i - 8]);
            write(bee_fd, &bee, 1);
            msleep(100);
            trun_led(led_fd, i, &d[i - 8]);
        }
    }
}
