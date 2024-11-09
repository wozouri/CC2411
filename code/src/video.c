#include "video.h"

int video_sum = 3;
int video_id = 0;
int video_mode = -1; // 0:stop 1:play -1:start
char cmd[110];

void video_play()
{
    if (video_mode != -1)
        system("killall -9 mplayer &");
    video_mode = 1;
    sprintf(cmd,
            "mplayer -slave -quiet -geometry 0:0 -zoom -x 800 -y 480 "
            "../../video/%d.mp4 &",
            video_id);
    system(cmd);
}

void video_change(int cnt)
{
    video_id = (video_id + cnt + video_sum) % video_sum;
    if (cnt > 0)
    {
        bmp_show(672, 375, "../../bmp/videoPlayer/next.bmp", 0xfffaf8ef, NULL);
    }
    else
    {
        bmp_show(48, 375, "../../bmp/videoPlayer/last.bmp", 0xfffaf8ef, NULL);
    }

    video_play();
}

void video_pause()
{
    bmp_show(360, 375, "../../bmp/videoPlayer/pause.bmp", 0xfffaf8ef, NULL);

    if (video_mode == -1)
    {
        video_play();
    }
    else if (video_mode == 1)
    {
        system("killall -STOP mplayer &");
        video_mode ^= 1;
    }
    else if (video_mode == 0)
    {
        system("killall -CONT mplayer &");
        video_mode ^= 1;
    }
}

void video_stop() { system("killall -9 mplayer &"); }