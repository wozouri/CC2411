#include "lcd.h"

int lcd_fd;
int *p;
const int N = 800, M = 480;

int lcd_temp[810][490];

int lcd_init()
{
    lcd_fd = open("/dev/fb0", O_RDWR);
    if (lcd_fd < 0)
    {
        perror("open lcd");
        return -1;
    }
    return 0;
}

int mmap_init()
{
    p = mmap(NULL, N * M * 4, PROT_READ | PROT_WRITE, MAP_SHARED, lcd_fd, 0);
    if (p == MAP_FAILED)
    {
        perror("mmap");
        close(lcd_fd);
        return -1;
    }
    return 0;
}

int lcd_draw_point(int x, int y, unsigned int color, void *xcolor, void *msec)
{
    if (x < 0 || x >= N || y < 0 || y >= M)
    {
        printf("error: x:%d y:%d\n", x, y);
        return -1;
    }
    if (color != xcolor)
    {
        if (msec != NULL)
        {
            lcd_temp[x][y] = *(p + y * N + x);
        }

        *(p + y * N + x) = color;
    }
    return 0;
}

int lcd_temp_save(int x, int y, int width, int height)
{

    for (int i = x; i < x + width; i++)
    {
        for (int j = y; j < y + height; j++)
        {
            if (lcd_temp[i][j] == 0)
                continue;
            *(p + j * N + i) = lcd_temp[i][j];
        }
    }
    return 0;
}

void lcd_temp_clear()
{
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            lcd_temp[i][j] = 0;
}

unsigned long file_size_get(const char *pfile_path)
{
    unsigned long filesize = -1;
    struct stat statbuff;

    if (stat(pfile_path, &statbuff) < 0)
    {
        return filesize;
    }
    else
    {
        filesize = statbuff.st_size;
    }

    return filesize;
}
