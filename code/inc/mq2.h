#ifndef __MQ_2_H__
#define __MQ_2_H__

#include "uart.h"
#include "image.h"

extern int mq2_fd;
extern int SmokeLevel;

void mq2_init(int id);
int mq2_getSmokeLevel();

#endif