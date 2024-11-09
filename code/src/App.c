#include "App.h"

void App_album()
{
    start_print("App_album");
    bmp_show(0, 0, BT_album.page_bmp_name, NULL, NULL);

    int cnt = 0, album_num = 5;
    char bmp_name[110];

    while (1)
    {
        int flag = event_xy();
        char *status;
        status = "NULL";
        if (flag)
        {
            if (flag == 1)
            {
                int slide_direction = check_slide();

                if (slide_direction >= 2) // 2 right 3 down
                {
                    cnt = (cnt - 1 + album_num) % album_num;
                    status = "last image";
                }
                else if (slide_direction >= 0) // 0 left 1 up
                {
                    cnt = (cnt + 1) % album_num;
                    status = "next image";
                }
                if (status != "NULL")
                {
                    sprintf(bmp_name, "../../bmp/album/%d.bmp", cnt);
                    bmp_show_direction(0, 0, bmp_name, slide_direction, NULL, NULL);
                    bmp_num_show(0, 0, 40, 20, cnt, NULL, 0);
                }
            }

            if (check_range(BT_quit))
            {
                button_click(BT_quit, flag);
                if (flag == 1)
                {
                    break;
                }
            }
        }
        p_x_y_st(touch_x, touch_y, status);
    }

    end_print("App_album");
}

void App_videoPlayer()
{
    start_print("App_videoPlayer");
    video_mode = -1;

    bmp_show(0, 0, BT_videoPlayer.page_bmp_name, NULL, NULL);
    while (1)
    {
        int flag = event_xy();
        char *status;
        status = "NULL";
        if (flag)
        {
            if (flag == 1)
            {
                if (check_range(BT_vP_last))
                {
                    video_change(-1);
                    status = "last video";
                }
                else if (check_range(BT_vP_pause))
                {
                    video_pause();
                    status = "pause";
                }
                else if (check_range(BT_vP_next))
                {
                    video_change(1);
                    status = "next video";
                }
            }

            if (check_range(BT_quit))
            {
                button_click(BT_quit, flag);
                if (flag == 1)
                {
                    video_stop();
                    break;
                }
            }
        }
        p_x_y_st(touch_x, touch_y, status);
    }
    end_print("videoPlayer");
}

int d8, d9, d10, all, bee = 1;
void App_ledORbee()
{

    start_print("ledORbee");

    bmp_show(0, 0, BT_ledORbee.page_bmp_name, NULL, NULL);

    char led[2];

    while (1)
    {
        int flag = event_xy();
        char *status;
        status = "NULL";
        if (flag)
        {
            if (flag == 1)
            {
                if (check_range(BT_lOb_led8))
                {
                    trun_led(led_fd, 8, &d8);
                    status = "led8";
                }
                else if (check_range(BT_lOb_led9))
                {
                    trun_led(led_fd, 9, &d9);
                    status = "led9";
                }
                else if (check_range(BT_lOb_led10))
                {
                    trun_led(led_fd, 10, &d10);
                    status = "led10";
                }
                else if (check_range(BT_lOb_all))
                {
                    all ^= 1;
                    led[0] = all;
                    for (int i = 8; i <= 10; i++)
                    {
                        led[1] = i;
                        write(led_fd, led, 2);
                    }
                    d8 = d9 = d10 = all;
                    button_click(BT_lOb_all, all + 1);
                    status = all ? "led is on" : "led is off";
                }
                else if (check_range(BT_lOb_bee))
                {
                    bee ^= 1;
                    write(bee_fd, &bee, 1);
                    button_click(BT_lOb_bee, bee + 1);
                    status = bee ? "bee is on" : "bee is off";
                }
            }

            if (check_range(BT_quit))
            {
                button_click(BT_quit, flag);
                if (flag == 1)
                {
                    break;
                }
            }
        }
        p_x_y_st(touch_x, touch_y, status);
    }

    end_print("App_ledORbee");
}

char *directon[4] = {"left", "up", "right", "down"};
void App_game_2048()
{

    start_print("App_game_2048");
    if (!game_2048_get_score())
        game_2048_init();
    else
        game_2048_show();

    while (1)
    {
        int flag = event_xy();
        int game_st = 0;
        char *status;
        status = "NULL";
        if (flag)
        {
            if (flag == 1)
            {
                int slide_direction = check_slide();
                if (slide_direction == -1)
                    status = "NULL";
                else
                {
                    game_st = game_2048_update(slide_direction);
                    status = directon[slide_direction];
                }

                if (check_range(BT_game_restart))
                {
                    if (!game_2048_get_score())
                    {
                        game_2048_init();
                        status = "New Game";
                    }
                    else
                    {
                        bmp_show(0, 0, BT_game_restart.page_bmp_name, 0xfffaf8ef, NULL);
                        while (1)
                        {
                            int r_flag = event_xy();

                            if (check_range(BT_game_restart_yes))
                            {
                                game_2048_init();
                                status = "New Game";
                                break;
                            }
                            if (check_range(BT_game_restart_no))
                            {
                                game_2048_show();
                                status = "Continue Game";
                                break;
                            }
                        }
                    }
                }

                if (game_st)
                {
                    if (game_st == -1) // fail
                    {
                        bmp_show(0, 0, "../../bmp/game_2048/2048_fail.bmp", 0xfffaf8ef, NULL);
                        while (1)
                        {
                            int flag1 = event_xy();
                            if (flag1 == 1)
                            {
                                if (touch_x >= 640 * 1.3 && touch_x <= (640 + 118.9) * 1.3 && touch_y >= 45 * 1.25 && touch_y <= (45 + 54.5) * 1.25)
                                {
                                    game_2048_init();
                                    status = "New Game";
                                    break;
                                }
                            }
                        }
                    }
                    else if (game_st == 1) // win
                    {
                        bmp_show(0, 0, "../../bmp/game_2048/2048_win.bmp", 0xfffaf8ef, NULL);
                        sleep(2);
                        game_2048_init();
                    }
                }
            }
        }
        if (check_range(BT_quit))
        {
            button_click(BT_quit, flag);
            if (flag == 1)
                break;
        }

        p_x_y_st(touch_x, touch_y, status);
    }
    end_print("game_2048");
}

void App_sensor() // 获取 温湿度加大气压 或者 光照强度
{
    start_print("sensor");
    gy39_init(1);
    mq2_init(2);

    bmp_show(0, 0, BT_sensor.page_bmp_name, NULL, NULL);

    pthread_t gy39_cm_t, mq2_t;
    pthread_create(&gy39_cm_t, NULL, gy39_composite_mode, NULL);
    pthread_create(&mq2_t, NULL, mq2_getSmokeLevel, NULL);
    while (1)
    {
        int flag = event_xy();

        if (check_range(BT_quit))
        {
            button_click(BT_quit, flag);
            if (flag == 1)
            {
                pthread_cancel(gy39_cm_t);
                pthread_cancel(mq2_t);
                break;
            }
        }
        p_x_y_st(touch_x, touch_y, "sensor");
    }

    pthread_join(gy39_cm_t, NULL);
    pthread_join(mq2_t, NULL);

    close(gy39_fd);
    close(mq2_fd);
    end_print("sensor");
}

void App_monitor()
{
    start_print("App_monitor");

    bmp_show(0, 0, BT_monitor.page_bmp_name, NULL, NULL);
    pthread_t monitor_t = 0;

    int mon_st = 0;
    while (1)
    {
        int flag = event_xy();
        if (flag == 1)
        {
            if (check_range(BT_monitor_handoff))
            {
                mon_st ^= 1;
                button_click(BT_monitor_handoff, mon_st + 1);
                if (mon_st)
                {
                    pthread_create(&monitor_t, NULL, real_time_video, NULL);
                    video_show_flag = 1;
                }
                else
                {
                    video_show_flag = 0;
                }
            }
            if (check_range(BT_monitor_capture))
            {
                take_photo_flag = 1;
            }
            if (check_range(BT_monitor_show))
            {
                printf("********   Show photo : %d !   ********\n", photo_s_num);
                if (photo_s_num == 1)
                    jpeg_show(320, 0, "photo-0.jpg", NULL, NULL, 2);
                else if (photo_s_num == 2)
                    jpeg_show(0, 240, "photo-1.jpg", NULL, NULL, 2);
                else if (photo_s_num == 0)
                    jpeg_show(320, 240, "photo-2.jpg", NULL, NULL, 2);
                photo_s_num = (photo_s_num + 1) % 3;
                lcd_temp_save(753, 423, 46, 20);
                bmp_num_show(753, 423, 46, 20, photo_s_num, 1, 0);
            }
        }

        if (check_range(BT_quit))
        {
            button_click(BT_quit, flag);
            if (flag == 1)
            {
                if (monitor_t)
                    video_show_flag = 0;
                break;
            }
        }
        p_x_y_st(touch_x, touch_y, "monitor");
    }

    if (monitor_t)
        pthread_join(monitor_t, NULL);
    end_print("App_monitor");
}

void App_ColorAnalyzer()
{
    start_print("App_ColorAnalyzer");

    char jpegname[110];

    bmp_show(0, 0, BT_ColorAnalyzer.page_bmp_name, NULL, NULL);
    for (int i = 0; i < 9; i++)
    {
        sprintf(jpegname, "../../jpg/ColorAnalyzer/%d.jpg", i);
        BT_ColorAnalyzer_index[i].x = i % 3 * 251 + 49;
        BT_ColorAnalyzer_index[i].y = i / 3 * 130 + 49;
        BT_ColorAnalyzer_index[i].width = 200;
        BT_ColorAnalyzer_index[i].height = 120;
        BT_ColorAnalyzer_index[i].xcolor = NULL;
        BT_ColorAnalyzer_index[i].BT_bmp_name = strdup(jpegname);   // 使用strdup为每个文件名分配内存
        BT_ColorAnalyzer_index[i].page_bmp_name = strdup(jpegname); // 同上 有点坑
        jpeg_show(BT_ColorAnalyzer_index[i].x, BT_ColorAnalyzer_index[i].y,
                  BT_ColorAnalyzer_index[i].BT_bmp_name, NULL, NULL, 4);
    }
    while (1)
    {
        int flag = event_xy();
        if (flag == 1)
        {
            for (int i = 0; i < 9; i++)
            {
                if (check_range(BT_ColorAnalyzer_index[i]))
                {
                    jpeg_show(0, 0, BT_ColorAnalyzer_index[i].page_bmp_name, NULL, NULL, 1);
                    Recognition_color_jpg(BT_ColorAnalyzer_index[i].page_bmp_name);
                    msleep(2000);
                    bmp_show(0, 0, BT_ColorAnalyzer.page_bmp_name, NULL, NULL);
                    for (int j = 0; j < 9; j++)
                    {
                        jpeg_show(BT_ColorAnalyzer_index[j].x, BT_ColorAnalyzer_index[j].y,
                                  BT_ColorAnalyzer_index[j].BT_bmp_name, NULL, NULL, 4);
                    }
                }
            }
        }

        if (check_range(BT_quit))
        {
            button_click(BT_quit, flag);
            if (flag == 1)
            {
                break;
            }
        }
        p_x_y_st(touch_x, touch_y, "ColorAnalyzer");
    }

    end_print("App_ColorAnalyzer");
}
