#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "image.h"

typedef struct BT_info
{
    double x;
    double y;
    double width;
    double height;
    char *BT_bmp_name; // bt-%d
    char *page_bmp_name;
    void *xcolor;
} BT_info;

extern BT_info BT_album, BT_quit;

extern BT_info BT_videoPlayer,
    BT_vP_last, BT_vP_pause, BT_vP_next;

extern BT_info BT_ledORbee, BT_lOb_led8,
    BT_lOb_led9, BT_lOb_led10, BT_lOb_all, BT_lOb_bee;

extern BT_info BT_game_2048, BT_game_restart,
    BT_game_restart_yes, BT_game_restart_no;

extern BT_info BT_sensor, BT_sensor_handoff;

extern BT_info BT_monitor, BT_monitor_handoff,
    BT_monitor_capture, BT_monitor_show;

extern BT_info BT_ColorAnalyzer, BT_ColorAnalyzer_index[9];
int button_click(BT_info bt, int flag);

#endif