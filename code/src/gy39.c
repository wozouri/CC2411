#include "gy39.h"

int gy39_fd;
int Humi = 0, Temp = 0, Pressure = 0;
int Lux = 0;

int gy39_init(int id)
{
    char buf[110];
    sprintf(buf, "/dev/ttySAC%d", id);
    gy39_fd = init_serial(buf, 9600);
}

int gy39_getHumiTempPressure()
{

    ssize_t rsize = 0;

    unsigned char cmd[3] = {0xa5, 0x82, 0x27};
    unsigned char buf[25] = {0};

    int msec = 1500;

    while (1)
    {
        write(gy39_fd, cmd, 3);
        rsize = read(gy39_fd, buf, 15);
        if (rsize == 15)
        {
            if (buf[2] == 0x45)
            {
                Humi = ((buf[10] << 8) | buf[11]) / 100;
                Temp = ((buf[4] << 8) | buf[5]) / 100;
                Pressure =
                    ((buf[6] << 24) | (buf[7] << 16) | (buf[8] << 8) | buf[9]) /
                    100000;
                printf("Temp = %d, Humi = %d, Pressure = %d\n", Temp, Humi, Pressure);

                lcd_temp_save(80, 60, 46, 20);
                lcd_temp_save(217, 60, 46, 20);
                lcd_temp_save(400, 60, 46, 20);

                bmp_num_show(80, 60, 46, 20, Temp, msec, 1);
                bmp_num_show(217, 60, 46, 20, Humi, msec, 1);
                bmp_num_show(400, 60, 46, 20, Pressure, msec, 1);
            }
            else
            {
                printf("data erro: neither temp or humi\n");
            }
        }
        else if (rsize < 0)
        {
            printf("read error\n");
            break;
        }
        else
        {
            //  printf("HTP read nothing\n");
            msleep(100);
        }
    }

    return 0;
}

int gy39_getLux()
{

    ssize_t rsize = 0;

    unsigned char cmd[3] = {0xa5, 0x83, 0x28}; // 光照强度命令
    unsigned char buf[25] = {0};

    int msec = 150;

    while (1)
    {
        write(gy39_fd, cmd, 3);
        rsize = read(gy39_fd, buf, 15);
        if (rsize == 15)
        {
            if (buf[2] == 0x15)
            {
                Lux = ((buf[4] << 24) | (buf[5] << 16) | (buf[6] << 8) | buf[7]) / 100;
                printf("Lux = %d\n", Lux);

                lcd_temp_save(130, 317, 100, 20);

                bmp_num_show(130, 317, 100, 20, Lux, msec, 1);
            }
            else
            {
                printf("data erro: neither lux\n");
            }
        }
        else if (rsize < 0)
        {
            printf("read error\n");
            break;
        }
        else
        {
            //    printf("Lux read nothing\n");
            msleep(100);
        }
    }

    return 0;
}

int gy39_composite_mode()
{
    int gy39_st = 0;
    pthread_t gy39_t, Alarm_t = 0;
    pthread_create(&gy39_t, NULL, gy39_getHumiTempPressure, NULL);
    while (1)
    {
        gy39_st ^= 1;
        void (*func)(void *);
        if (gy39_st)
        {
            func = gy39_getLux;
        }
        else
        {
            func = gy39_getHumiTempPressure;
        }

        pthread_cancel(gy39_t);
        pthread_join(gy39_t, NULL);
        pthread_create(&gy39_t, NULL, func, NULL);

        if (Temp > 35 || Humi > 70 || Pressure > 110 || Lux > 1000 || SmokeLevel > 50)
        {
            if (!Alarm_t)
            {
                pthread_create(&Alarm_t, NULL, Alarm, NULL);
            }
        }
        else if (Alarm_t)
        {
            pthread_cancel(Alarm_t);
            pthread_join(Alarm_t, NULL);
            Alarm_t = 0;
        }
        msleep(800);
    }

    pthread_cancel(gy39_t);
    pthread_join(gy39_t, NULL);

    if (Alarm_t)
    {
        pthread_cancel(Alarm_t);
        pthread_join(Alarm_t, NULL);
    }
}