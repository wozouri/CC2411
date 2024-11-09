#ifndef __VIDEO_H__
#define __VIDEO_H__

#include "image.h"

extern int video_mode;

void video_play();
void video_change(int cnt);
void video_pause();
void video_stop();

#endif