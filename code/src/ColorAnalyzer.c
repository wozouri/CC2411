#include "ColorAnalyzer.h"

// JPG图片颜色识别

void Recognition_color_jpg(char *jpegname)
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

    int h = 0, mx = 0, c_mx = 0;
    int Red = 0, Green = 0, Blue = 0;
    while (cinfo.output_scanline < cinfo.output_height)
    {
        jpeg_read_scanlines(&cinfo, &buffer, 1);

        char *p = buffer;
        for (int w = 0; w < cinfo.output_width; w++)
        {
            unsigned int color;
            unsigned char a, r, g, b;
            c_mx = 0;
            a = cinfo.output_components == 4 ? *p++ : 0xff;
            r = *p++;
            g = *p++;
            b = *p++;
            color = (a << 24) | (r << 16) | (g << 8) | b;
            c_mx = max(c_mx, max(r, g));
            c_mx = max(c_mx, b);
            if (r == c_mx)
                Red++;
            if (g == c_mx)
                Green++;
            if (b == c_mx)
                Blue++;
            mx = max(mx, max(Red, Green));
            mx = max(mx, Blue);
        }
        h++;
    }

    printf("Red:%d,Green:%d,Blue:%d\n", Red, Green, Blue);

    bmp_show(328.5, 210, "../../bmp/ColorAnalyzer/approach.bmp", 0xfffaf8ef, NULL);

    int flag = 0;
    if (Red == mx)
    {
        bmp_show(411, 210, "../../bmp/ColorAnalyzer/Red.bmp", 0xfffaf8ef, NULL);
        flag++;
    }

    if (Green == mx)
    {
        bmp_show(411 + flag * 70, 210, "../../bmp/ColorAnalyzer/Green.bmp", 0xfffaf8ef, NULL);
        flag++;
    }

    if (Blue == mx)
    {
        bmp_show(411 + flag * 70, 210, "../../bmp/ColorAnalyzer/Blue.bmp", 0xfffaf8ef, NULL);
        flag++;
    }

    int Sum = Red + Green + Blue;
    Red = Red * 282 / Sum * 5;
    Green = Green * 282 / Sum * 5;
    Blue = Blue * 282 / Sum * 5;
    for (int w = 0; w < N; w++)
    {
        for (int h = 0; h < 5; h++)
        {
            if (Red)
            {
                lcd_draw_point(w + 259, h + 284, RED, NULL, NULL);
                Red--;
            }
            else if (Green)
            {
                lcd_draw_point(w + 259, h + 284, GREEN, NULL, NULL);
                Green--;
            }
            else if (Blue)
            {
                lcd_draw_point(w + 259, h + 284, BLUE, NULL, NULL);
                Blue--;
            }
        }
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    free(buffer);
    fclose(input_file);
}