#ifndef ACCEL_H
#define ACCEL_H


#include "MKL46Z4.h"
#include "i2c.h"
#include "systick.h"
#include "uart.h"

void accel_init();
void accel_start();
void accel_acq_cal();

#endif