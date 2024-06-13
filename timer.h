#ifndef TIMER_H
#define TIMER_H
#include "mkl46z4.h"

#define STATE_ACTIVE 0
#define STATE_FALL 1

/* The application starts with Core clock = 41.94MHz, BusClock = 13.98MHz */
void pit_timer_init(float expected_time, uint8_t STATE);

#define BUSCLOCK_HZ (13.98 * 1000000)
#define BUSCLOCK_PERIOD_SEC (1 / BUSCLOCK_HZ)

#endif