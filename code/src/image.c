#include "image.h"

bmpinfo bmp_get_info(char *bmpname)
{
    int fd = open(bmpname, O_RDONLY);
    if (fd < 0)
    {
        printf("open bmp error: %s\n", bmpname);
        exit(-1);
    }

    bmpinfo bmp;
    bmp.bmpname = bmpname;

    lseek(fd, 0x02, SEEK_SET);
    read(fd, &bmp.size, 4);

    lseek(fd, 0x12, SEEK_SET);
    read(fd, &bmp.width, 4);
    bmp.width &= 0x7fffffff;

    lseek(fd, 0x16, SEEK_SET);
    read(fd, &bmp.height, 4);
    bmp.height &= 0x7fffffff;

    bmp.width = min(bmp.width, N);
    bmp.height = min(bmp.height, M);

    lseek(fd, 0x1c, SEEK_SET);
    read(fd, &bmp.bpp, 2);

    bmp.data = (char *)malloc(bmp.size - BMP_HEADER_SIZE);
    lseek(fd, BMP_HEADER_SIZE, SEEK_SET);
    read(fd, bmp.data, bmp.size - BMP_HEADER_SIZE);

    close(fd);

    return bmp;
}

int bmp_show(int x, int y, char *bmpname, void *xcolor, void *msec)
{
    bmpinfo bmp = bmp_get_info(bmpname);
    if (!bmp.data)
    {
        perror("bmp.date error");
        free(bmp.data);
        return -1;
    }

    if (bmp.bpp < 24)
    {
        printf("bmp bpp < 24");
        free(bmp.data);
        return -1;
    }

    int row_size = bmp.width * (bmp.bpp / 8);
    int padding = (row_size % 4) ? (4 - row_size % 4) : 0;

    unsigned int color;
    char *p = bmp.data;

    for (int h = 0; h < bmp.height; h++)
    {
        for (int w = 0; w < bmp.width; w++)
        {
            char b = *p++; // 等价 *(p++)
            char g = *p++;
            char r = *p++;
            char a = bmp.bpp == 32 ? *p++ : 0xff;
            color = a << 24 | r << 16 | g << 8 | b;

            int m = x + w;
            int n = y + bmp.height - 1 - h; // 翻转

            lcd_draw_point(m, n, color, xcolor, msec);
        }
        p += padding;
    }

    free(bmp.data);

    return 0;
}

// 数字默认扣除背景 0xfffaf8ef
// 大小：20px * 20px
// 间隔：13px
// 相互覆盖显示

int num_id[30]; // 800/33 = 24
int num_px = 20;
int num_dis = 13;

int bmp_num_show(int x, int y, int width, int height, int num, void *msec, int mode)
{
    if (width < 20 || height < 20)
    {
        printf("width or height too small\n");
        return -1;
    }

    char num_name[110];
    int cnt = 0;

    if (!num)
    {
        bmp_show(x, y, "../../bmp/number/0.bmp", 0xfffaf8ef, msec);
        return 0;
    }

    while (num)
    {
        num_id[cnt++] = num % 10;
        num /= 10;
    }

    if (num_dis * (cnt - 1) > width)
    {
        printf("width too small\n");
        return -1;
    }

    int l = cnt - 1, r = -1, index = -1; // 0： 居左对齐   1：居右对齐
    if (mode)
    {
        index = 1;
        l = 0;
        r = cnt;
    }

    for (int i = l; i != r; i += index)
    {
        sprintf(num_name, "../../bmp/number/%d.bmp", num_id[i]);
        if (mode)
            bmp_show(x + width - num_px - num_dis * i, y, num_name, 0xfffaf8ef, msec);
        else
            bmp_show(x + num_dis * (cnt - i - 1), y, num_name, 0xfffaf8ef, msec);
    }

    return 0;
}
void bmp_show_direction(int x, int y, char *bmpname, int flag, void *xcolor, void *msec)
{
    bmpinfo bmp = bmp_get_info(bmpname);

    if (!bmp.data)
    {
        printf("bmp data error !\n");
        return;
    }

    int valid_types = abs(bmp.width) * (bmp.bpp / 8);

    int pad_types = (valid_types % 4 == 0) ? 0 : (4 - (valid_types % 4));

    int color;
    char *p = bmp.data;

    // 默认 up
    int h_l = 0, h_r = abs(bmp.height), h_index = 1;
    int w_l = 0, w_r = abs(bmp.width), w_index = 1;

    if (flag == dir_down)
    {
        h_l = abs(bmp.height) - 1;
        h_r = -1;
        h_index = -1;
    }
    else if (flag == dir_left)
    {
        w_l = abs(bmp.width) - 1;
        w_r = -1;
        w_index = -1;
    }

    int by_num = 3;
    if (bmp.bpp == 32)
    {
        by_num = 4;
    }
    else if (bmp.bpp < 24)
    {
        printf("bmp bpp error !\n");
        free(bmp.data);
        return;
    }

    if (flag & 1)
    {
        for (int h = h_l; h != h_r; h += h_index)
        {
            for (int w = w_l; w != w_r; w += w_index)
            {
                char b = *(p + (h * abs(bmp.width) + w) * by_num);
                char g = *(p + (h * abs(bmp.width) + w) * by_num + 1);
                char r = *(p + (h * abs(bmp.width) + w) * by_num + 2);
                char a = (bmp.bpp == 24)
                             ? 0
                             : *(p + (h * abs(bmp.width) + w) * by_num + 3);
                color = a << 24 | r << 16 | g << 8 | b;
                int m = x + (bmp.width > 0 ? w : (bmp.width - 1 - w));
                int n = y + (bmp.height < 0 ? h : (bmp.height - 1 - h));

                lcd_draw_point(m, n, color, xcolor, msec);
            }
        }
    }
    else
    {
        for (int w = w_l; w != w_r; w += w_index)
        {
            for (int h = h_l; h != h_r; h += h_index)
            {
                char b = *(p + (h * abs(bmp.width) + w) * by_num);
                char g = *(p + (h * abs(bmp.width) + w) * by_num + 1);
                char r = *(p + (h * abs(bmp.width) + w) * by_num + 2);
                char a = (bmp.bpp == 24)
                             ? 0
                             : *(p + (h * abs(bmp.width) + w) * by_num + 3);
                color = a << 24 | r << 16 | g << 8 | b;
                int m = x + (bmp.width > 0 ? w : (bmp.width - 1 - w));
                int n = y + (bmp.height < 0 ? h : (bmp.height - 1 - h));

                lcd_draw_point(m, n, color, xcolor, msec);
            }
        }
    }

    free(bmp.data);
}

// mode  1： 原尺寸  2：1/2尺寸
int jpeg_show(int x, int y, char *jpegname, char *jpeg_data, int jpeg_size, int mode)
{
    FILE *input_file;
    if (jpegname != NULL)
    {
        input_file = fopen(jpegname, "rb");
        if (!input_file)
        {
            printf("open %s failed\n", jpegname);
            return -1;
        }
    }
    else
    {
        input_file = fmemopen(jpeg_data, jpeg_size, "rb");
        if (!input_file)
        {
            printf("open %s failed\n", jpegname);
            return -1;
        }
    }
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, input_file);

    if (jpeg_read_header(&cinfo, TRUE) != JPEG_HEADER_OK)
    {
        fprintf(stderr, "JPEG header error\n");
        fclose(input_file);
        return 1;
    }

    jpeg_start_decompress(&cinfo);

    unsigned char *buffer = (unsigned char *)malloc(cinfo.output_width * cinfo.output_components);

    int h = 0;
    while (cinfo.output_scanline < cinfo.output_height)
    {
        for (int i = 1; i <= mode; i++)
            jpeg_read_scanlines(&cinfo, &buffer, 1);

        char *p = buffer;
        for (int w = 0; w < cinfo.output_width / mode; w++)
        {
            unsigned int color;
            unsigned char a, r, g, b;
            a = cinfo.output_components == 4 ? *p++ : 0xff;
            r = *p++;
            g = *p++;
            b = *p++;
            color = (a << 24) | (r << 16) | (g << 8) | b;
            if (x + w < N && y + h < M)
                lcd_draw_point(x + w, y + h, color, NULL, NULL);
            p += (mode - 1) * (cinfo.output_components == 4 ? 4 : 3);
        }
        h++;
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    free(buffer);
    fclose(input_file);

    return 0;
}

static char g_color_buf[FB_SIZE] = {0};
static int g_fb_fd;
static int *g_pfb_memory;

int lcd_draw_jpg(unsigned int x, unsigned int y, const char *pjpg_path, char *pjpg_buf, unsigned int jpg_buf_size, unsigned int jpg_half)
{

    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    char *pcolor_buf = g_color_buf;
    char *pjpg;

    unsigned int i = 0;
    unsigned int color = 0;
    unsigned int count = 0;

    unsigned int x_s = x;
    unsigned int x_e;
    unsigned int y_e;

    int jpg_fd;
    unsigned int jpg_size;

    unsigned int jpg_width;
    unsigned int jpg_height;

    if (pjpg_path != NULL)
    {
        jpg_fd = open(pjpg_path, O_RDWR);

        if (jpg_fd == -1)
        {
            printf("open %s error\n", pjpg_path);

            return -1;
        }

        jpg_size = file_size_get(pjpg_path);

        pjpg = malloc(jpg_size);

        read(jpg_fd, pjpg, jpg_size);
    }
    else
    {
        jpg_size = jpg_buf_size;

        pjpg = pjpg_buf;
    }

    cinfo.err = jpeg_std_error(&jerr);

    jpeg_create_decompress(&cinfo);

    jpeg_mem_src(&cinfo, pjpg, jpg_size);

    jpeg_read_header(&cinfo, TRUE);

    jpeg_start_decompress(&cinfo);

    if (jpg_half)
    {
        x_e = x_s + (cinfo.output_width / 2);
        y_e = y + (cinfo.output_height / 2);

        while (cinfo.output_scanline < cinfo.output_height)
        {
            pcolor_buf = g_color_buf;

            jpeg_read_scanlines(&cinfo, (JSAMPARRAY)&pcolor_buf, 1);

            jpeg_read_scanlines(&cinfo, (JSAMPARRAY)&pcolor_buf, 1);

            for (i = 0; i < (cinfo.output_width / 2); i++)
            {
                color = *(pcolor_buf + 2);
                color = color | *(pcolor_buf + 1) << 8;
                color = color | *(pcolor_buf) << 16;

                lcd_draw_point(x, y, color, NULL, NULL);

                pcolor_buf += 6;

                x++;
            }

            y++;

            x = x_s;
        }
    }
    else
    {
        x_e = x_s + cinfo.output_width;
        y_e = y + cinfo.output_height;

        while (cinfo.output_scanline < cinfo.output_height)
        {
            pcolor_buf = g_color_buf;

            jpeg_read_scanlines(&cinfo, (JSAMPARRAY)&pcolor_buf, 1);

            for (i = 0; i < cinfo.output_width; i++)
            {
                color = *(pcolor_buf + 2);
                color = color | *(pcolor_buf + 1) << 8;
                color = color | *(pcolor_buf) << 16;

                lcd_draw_point(x, y, color, NULL, NULL);

                pcolor_buf += 3;

                x++;
            }

            y++;

            x = x_s;
        }
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    if (pjpg_path != NULL)
    {
        close(jpg_fd);

        free(pjpg);
    }

    return 0;
}

int png_show(int x, int y, char *pngname)
{
    FILE *input_file = fopen(pngname, "rb");
    if (!input_file)
    {
        perror("fopen");
        return -1;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
        fclose(input_file);
        return -1;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(input_file);
        return -1;
    }

    if (setjmp(png_jmpbuf(png_ptr)))
    {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(input_file);
        return -1;
    }

    png_init_io(png_ptr, input_file);
    png_read_info(png_ptr, info_ptr);

    int width = png_get_image_width(png_ptr, info_ptr);
    int height = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    // 转换 PNG 图像到 RGB 格式
    if (color_type == PNG_COLOR_TYPE_PALETTE)
    {
        png_set_palette_to_rgb(png_ptr);
    }
    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    {
        png_set_gray_to_rgb(png_ptr);
    }
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
    {
        png_set_tRNS_to_alpha(png_ptr);
    }
    if (bit_depth == 16)
    {
        png_set_scale_16(png_ptr);
    }

    png_read_update_info(png_ptr, info_ptr);

    png_bytep *row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * height);
    for (int i = 0; i < height; i++)
    {
        row_pointers[i] = (png_byte *)malloc(png_get_rowbytes(png_ptr, info_ptr));
    }

    png_read_image(png_ptr, row_pointers);

    for (int h = 0; h < height; h++)
    {
        png_byte *row = row_pointers[h];
        for (int w = 0; w < width; w++)
        {
            png_byte *ptr = &(row[w * 4]);
            // unsigned int color = (ptr[0] << 24) | (ptr[1] << 16) | (ptr[2] << 8) | ptr[3];
            unsigned int color = (ptr[3] << 24) | (ptr[0] << 16) | (ptr[1] << 8) | ptr[2];
            if (x + w < N && y + h < M)
                lcd_draw_point(x + w, y + h, color, NULL, NULL);
        }
    }

    for (int i = 0; i < height; i++)
    {
        free(row_pointers[i]);
    }
    free(row_pointers);

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(input_file);

    return 0;
}