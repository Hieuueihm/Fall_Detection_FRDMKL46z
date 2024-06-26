#ifndef ACCEL_H
#define ACCEL_H

#include "MKL46Z4.h"
#include "i2c.h"
#include "systick.h"
#include "uart.h"

#define ACCEL_DEVICE_ADDRESS 0x1D
#define ACCEL_WHO_AM_I 0x1A

#define STATUS_REG 0x00
#define OUT_X_MSB_REG 0x01
#define OUT_X_LSB_REG 0x02
#define OUT_Y_MSB_REG 0x03
#define OUT_Y_LSB_REG 0x04
#define OUT_Z_MSB_REG 0x05
#define OUT_Z_LSB_REG 0x06

#define F_SETUP_REG 0x09
#define TRIG_CFG_REG 0x0A
#define SYSMOD_REG 0x0B
#define INT_SOURCE_REG 0x0C
#define WHO_AM_I_REG 0x0D
#define XYZ_DATA_CFG_REG 0x0E
#define HP_FILTER_CUTOFF_REG 0x0F

#define PL_STATUS_REG 0x10
#define PL_CFG_REG 0x11
#define PL_COUNT_REG 0x12
#define PL_BF_ZCOMP_REG 0x13
#define P_L_THS_REG 0x14

#define FF_MT_CFG_REG 0x15
#define FF_MT_SRC_REG 0x16
#define FF_MT_THS_REG 0x17
#define FF_MT_COUNT_REG 0x18

#define TRANSIENT_CFG_REG 0x1D
#define TRANSIENT_SRC_REG 0x1E
#define TRANSIENT_THS_REG 0x1F
#define TRANSIENT_COUNT_REG 0x20

#define PULSE_CFG_REG 0x21
#define PULSE_SRC_REG 0x22
#define PULSE_THSX_REG 0x23
#define PULSE_THSY_REG 0x24
#define PULSE_THSZ_REG 0x25
#define PULSE_TMLT_REG 0x26
#define PULSE_LTCY_REG 0x27
#define PULSE_WIND_REG 0x28

#define ASLP_COUNT_REG 0x29

#define CTRL_REG1 0x2A
#define CTRL_REG2 0x2B
#define CTRL_REG3 0x2C
#define CTRL_REG4 0x2D
#define CTRL_REG5 0x2E

#define OFF_X_REG 0x2F
#define OFF_Y_REG 0x30
#define OFF_Z_REG 0x31

#define FREE_FALL 0
#define MOTION_DETECTION 1

void accel_init(void);

void accel_config(uint8_t MODE, uint8_t THRESHOLD, uint8_t DEBOUNCE);

#endif