#ifndef __LCD__H__
#define __LCD__H__

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "tool.h"

#define LCD_WIDTH 800
#define LCD_HEIGHT 480
#define FB_SIZE (LCD_WIDTH * LCD_HEIGHT * 4)

#define RED 0x00ff0000
#define GREEN 0x0000ff00
#define BLUE 0x000000ff

extern const int N;
extern const int M;

extern int lcd_fd;
extern int *p;

int lcd_init();
int mmap_init();
int lcd_draw_point(int x, int y, unsigned int color, void *xcolor, void *flag);
int lcd_temp_save(int x, int y, int width, int height);
void lcd_temp_clear();
unsigned long file_size_get(const char *pfile_path);

#endif