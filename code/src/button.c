#include "button.h"

// x y width height BT_bmp_name
// extern

// page : Main
BT_info BT_album = {48, 352, 80, 80, "", "../../bmp/album/App-album.bmp", NULL};
BT_info BT_videoPlayer = {
    204, 352, 80, 80, "", "../../bmp/videoPlayer/App-videoPlayer.bmp", NULL};
BT_info BT_ledORbee = {360, 352, 80,
                       80, "", "../../bmp/ledORbee/App-ledORbee.bmp", NULL};
BT_info BT_game_2048 = {516, 352, 80,
                        80, "", "../../bmp/game_2048/2048_Main.bmp", NULL};
BT_info BT_sensor = {672, 352, 80, 80, "", "../../bmp/sensor/App_sensor.bmp", NULL};

// page : Main_2
BT_info BT_monitor = {48, 352, 80, 80, "", "../../bmp/monitor/App_monitor.bmp", NULL};
BT_info BT_ColorAnalyzer = {
    204, 352, 80, 80, "", "../../bmp/ColorAnalyzer/App_ColorAnalyzer.bmp", NULL};

// page : all ()
BT_info BT_quit = {720, 0, 80, 40, "../../bmp/Main/quit-%d.bmp", "", 0xfffaf8ef};

// page : videoPlayer
BT_info BT_vP_last = {48, 375, 80, 80, "", "../../bmp/videoPlayer/last.bmp", NULL};
BT_info BT_vP_pause = {360, 375, 80, 80, "", "../../bmp/videoPlayer/pause.bmp", NULL};
BT_info BT_vP_next = {672, 375, 80, 80, "", "../../bmp/videoPlayer/next.bmp", NULL};

// page : ledORbee
BT_info BT_lOb_led8 = {48, 48, 80, 80, "", "", NULL};
BT_info BT_lOb_led9 = {256, 48, 80, 80, "", "", NULL};
BT_info BT_lOb_led10 = {464, 48, 80, 80, "", "", NULL};
BT_info BT_lOb_all = {672, 48, 80, 80, "../../bmp/ledORbee/all-%d.bmp", "", NULL};
BT_info BT_lOb_bee = {360, 352, 80, 80, "../../bmp/ledORbee/bee-%d.bmp", "", NULL};

// page : game_2048
BT_info BT_game_restart = {640, 45, 118.9, 54.5, "", "../../bmp/game_2048/2048_restart_check.bmp"};
BT_info BT_game_restart_yes = {200, 300, 100, 60, "", "", NULL};
BT_info BT_game_restart_no = {500, 300, 100, 60, "", "", NULL};

// page : sensor
BT_info BT_sensor_handoff = {550, 392, 80, 40, "../../bmp/sensor/handoff-%d.bmp", "", NULL};

// page : monitor
BT_info BT_monitor_handoff = {673.4, 215.9, 100, 55, "../../bmp/monitor/handoff-%d.bmp", "", 0xfffaf8ef};
BT_info BT_monitor_capture = {673.4, 333.9, 100, 55, "", "", NULL};
BT_info BT_monitor_show = {673.4, 405.9, 100, 55, "", "", NULL};

// page : ColorAnalyzer
BT_info BT_ColorAnalyzer_index[9];

// flag = 2 为按下，flag = 1 为弹起

int button_click(BT_info bt, int flag)
{
    char Bmp_name[110];
    if (flag == 2)
    {
        sprintf(Bmp_name, bt.BT_bmp_name, 2);
        bmp_show(bt.x, bt.y, Bmp_name, bt.xcolor, NULL);
    }
    else if (flag == 1)
    {
        sprintf(Bmp_name, bt.BT_bmp_name, 1);
        bmp_show(bt.x, bt.y, Bmp_name, bt.xcolor, NULL);
    }

    return 0;
}