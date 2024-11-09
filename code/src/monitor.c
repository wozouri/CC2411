#include "monitor.h"

int monitor_init = 0;
int photo_c_num = 0;
int photo_s_num = 0;
int take_photo_flag = 0;
int video_show_flag = 0;

struct jpg_data video_buf;

void *real_time_video()
{
    start_print("real_time_video");

    // 初始化摄像头设备
    if (!monitor_init)
        linux_v4l2_yuyv_init("/dev/video7");
    monitor_init = 1;

    // 启动摄像头
    linux_v4l2_start_yuyv_capturing();

    while (1)
    {

        // 获取摄像头数据
        linux_v4l2_get_yuyv_data(&video_buf);

        // 显示摄像头图像
        // lcd_draw_jpg(0, 0, NULL, video_buf.jpg_data, video_buf.jpg_size, 1);

        jpeg_show(0, 0, NULL, video_buf.jpg_data, video_buf.jpg_size, 2);
        if (take_photo_flag == 1) // 进行抓拍
        {
            printf("********   Capture photo : %d !   ********\n", photo_c_num);
            // 创建一个新的文件
            char photo_name[110];
            sprintf(photo_name, "photo-%d.jpg", photo_c_num);
            lcd_temp_save(753, 351, 46, 20);
            bmp_num_show(753, 351, 46, 20, photo_c_num, 1, 0);
            lcd_temp_save(753, 423, 46, 20);
            bmp_num_show(753, 423, 46, 20, photo_s_num, 1, 0);
            photo_c_num = (photo_c_num + 1) % 3;
            int fd = open(photo_name, O_RDWR | O_CREAT, 0777);
            if (fd == -1)
            {
                perror("拍照失败");
                continue;
            }
            // 将图片数据写入文件中
            write(fd, video_buf.jpg_data, video_buf.jpg_size);
            // 关闭文件保存
            close(fd);
            // 清空抓拍的标志位
            take_photo_flag = 0;
        }

        if (video_show_flag == 0)
            break;
    }

    // 停止摄像头
    linux_v4l2_yuyv_quit();
    end_print("real_time_video");
}