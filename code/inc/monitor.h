#ifndef __MONITOR_H__
#define __MONITOR_H__

#include "api_v4l2.h"
#include "image.h"
#include "touch.h"
#include "yuyv.h"
#include "tool.h"

extern int take_photo_flag, video_show_flag, photo_c_num, photo_s_num;

void *real_time_video();

#endif