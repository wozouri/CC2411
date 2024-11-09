#ifndef __ledORbee_H__
#define __ledORbee_H__

#include "lcd.h"
#include "ThreadClass.h"

extern int led_fd;
extern int bee_fd;

int ledORbee_init();
void trun_led(int led_fd, int num, int *status);
void Alarm();

#endif