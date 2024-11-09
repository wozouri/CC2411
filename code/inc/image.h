#ifndef __BMP_H__
#define __BMP_H__

#include "lcd.h"
#include <jpeglib.h>
#include <png.h>

#define BMP_HEADER_SIZE 54

typedef struct bmpinfo
{
    char *bmpname;
    int size;
    unsigned int width;
    unsigned int height;
    short bpp;
    char *data;
} bmpinfo;

bmpinfo bmp_get_info(char *bmpname);

int bmp_show(int x, int y, char *bmpname, void *xcolor, void *usec);

int bmp_num_show(int x, int y, int width, int height, int num, void *usec, int mode);
// mode 左0 右1

void bmp_show_direction(int x, int y, char *bmpname, int flag, void *xcolor, void *usec);

int jpeg_show(int x, int y, char *jpegname, char *jpeg_data, int jpeg_size, int mode);

int lcd_draw_jpg(unsigned int x, unsigned int y, const char *pjpg_path, char *pjpg_buf, unsigned int jpg_buf_size, unsigned int jpg_half);

int png_show(int x, int y, char *pngname);

#endif