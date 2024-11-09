#include "mq2.h"

int mq2_fd;
int SmokeLevel = 0;
void mq2_init(int id)
{
    char buf[110];
    sprintf(buf, "/dev/ttySAC%d", id);
    mq2_fd = init_serial(buf, 9600);
}

int mq2_getSmokeLevel()
{
    ssize_t rsize = 0;
    unsigned char cmd[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
    unsigned char buf[25] = {0};

    int msec = 1500;

    while (1)
    {
        write(mq2_fd, cmd, 9);
        rsize = read(mq2_fd, buf, 25);
        if (rsize == 9)
        {
            {
                SmokeLevel = (buf[2] << 8) | buf[3];
                printf("SmokeLevel: %d\n", SmokeLevel);

                lcd_temp_save(400, 195, 76, 20);

                bmp_num_show(400, 195, 76, 20, SmokeLevel, msec, 1);
            }
        }
        else if (rsize < 0)
        {
            printf("read error\n");
            break;
        }
        else
        {
            // printf("smokeLevel read nothing\n");
            msleep(500);
        }
    }
    return 0;
}
