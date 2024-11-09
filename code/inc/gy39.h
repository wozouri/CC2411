#ifndef __GY39_H
#define __GY39_H

#include <pthread.h>
#include "image.h"
#include "uart.h"
#include "ledORbee.h"
#include "mq2.h"

extern int gy39_fd;
extern int Humi, Temp, Pressure, Lux;

int gy39_init(int id);
int gy39_getHumiTempPressure();
int gy39_getLux();
int gy39_composite_mode();

#endif