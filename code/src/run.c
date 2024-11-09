#include "run.h"

int Main()
{
    start_print("Main");

    bmp_show(0, 0, "../../bmp/Main/Main_1.bmp", NULL, NULL);

    while (1)
    {
        int flag = event_xy();
        if (flag == 1)
        {
            if (check_range(BT_album))
            {
                // album
                App_album();
            }
            else if (check_range(BT_videoPlayer))
            {
                // videoPlayer
                App_videoPlayer();
            }
            else if (check_range(BT_ledORbee))
            {
                // ledORbee
                App_ledORbee();
            }
            else if (check_range(BT_game_2048))
            {
                // game_2048
                App_game_2048();
            }
            else if (check_range(BT_sensor))
            {
                // sensor
                App_sensor();
            }

            int slide_direction = check_slide();
            if (slide_direction == 0)
            {
                Main_2();
            }
        }
        p_x_y_st(touch_x, touch_y, "Main");

        bmp_show(0, 0, "../../bmp/Main/Main_1.bmp", NULL, NULL);
    }

    end_print("Main");
}

int Main_2()
{
    start_print("Main_2");
    bmp_show_direction(0, 0, "../../bmp/Main/Main_2.bmp", 0, NULL, NULL);

    while (1)
    {
        int flag = event_xy();

        if (flag == 1)
        {
            if (check_range(BT_monitor))
            {
                App_monitor();
            }
            if (check_range(BT_ColorAnalyzer))
            {
                App_ColorAnalyzer();
            }
        }

        int slide_direction = check_slide();
        if (slide_direction == 2)
        {
            bmp_show_direction(0, 0, "../../bmp/Main/Main_1.bmp", 2, NULL, NULL);
            return;
        }

        bmp_show(0, 0, "../../bmp/Main/Main_2.bmp", NULL, NULL);
    }
    end_print("Main_2");
    return 0;
}