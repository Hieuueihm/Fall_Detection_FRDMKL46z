#include "led.h"
#include "switch.h"
#include "systick.h"
#include "uart.h"
#include "clock.h"
#include "lcd.h"
#include "i2c.h"
#include "accel.h"
#include <stdio.h>
#include "timer.h"

#define abs(x, y) (x > y ? x - y : y - x)

#define CLEAR_INTR_PORTC_PORTD (1 << 24)
#define SWITCH_1_PIN 3
#define SWITCH_2_PIN 12
#define INT1_MMA_PIN 5

#define STATE_4 4
#define STATE_1 1
#define STATE_2 2
#define STATE_3 3

// 1s -> 1
#define TIME_OUT 5
#define EPSILON 0.3

#define MAIN_STATUS_STAND_BY 0
#define MAIN_STATUS_ACTIVE 1

#define SENSITIVITY 0.0625 // Sensitivity for �8g range

volatile uint32_t count = 0;

float convert_to_g(int8_t raw_value, float sensitivity);
void read_accel_data(uint32_t delay);

uint8_t main_status = MAIN_STATUS_STAND_BY;
uint8_t state = STATE_1;
uint32_t prev_time = 0;

char buffer[50];
float initX_f = 0, initY_f = -1, initZ_f = 0;

int main(void)
{

  mcg_clock_config();
  systick_init();
  switch_init();
  uart_init();

  led_init(RED_LED);
  led_init(GREEN_LED);

  lcd_init();
  i2c_init();

  accel_init();

  pit_timer_init(1, STATE_ACTIVE);
  pit_timer_init(0.5, STATE_FALL);

  while (1)
  {

    // if (state == STATE_3)
    // {

    //   read_accel_data(100);
    // }
    if (state != STATE_1 && state != STATE_4)
    {
      if (abs(count, prev_time) >= TIME_OUT)
      {
        accel_config(FREE_FALL, 6, 3);
        state = STATE_1;
      }
    }
  }
}

void PORTC_PORTD_IRQHandler()
{
  // switch handler
  if ((PTC->PDIR & (1 << SWITCH_1_PIN)) == 0)
  {
    main_status = main_status == MAIN_STATUS_STAND_BY ? MAIN_STATUS_ACTIVE : MAIN_STATUS_STAND_BY;
    if (main_status == MAIN_STATUS_ACTIVE)
    {
      accel_config(FREE_FALL, 6, 3);
      state = STATE_1;
      led_on(GREEN_LED);
      lcd_display_digit(NUM_0);
    }
    else if (main_status == MAIN_STATUS_STAND_BY)
    {
      lcd_clear();
      led_off(RED_LED);
      led_off(GREEN_LED);
    }
    PORTC->PCR[SWITCH_1_PIN] |= CLEAR_INTR_PORTC_PORTD;
  }

  if ((PTC->PDIR & (1 << SWITCH_2_PIN)) == 0)
  {
    if (main_status == MAIN_STATUS_STAND_BY)
    {
    }
    else if (main_status == MAIN_STATUS_ACTIVE)
    {
      accel_config(FREE_FALL, 6, 3);
      state = STATE_1;
      lcd_display_digit(NUM_0);
      led_off(RED_LED);
    }

    PORTC->PCR[SWITCH_2_PIN] |= CLEAR_INTR_PORTC_PORTD;
  }

  // mmq intr handler
  if ((PTC->PDIR & (1 << INT1_MMA_PIN)) == 0)
  {
    if (main_status == MAIN_STATUS_ACTIVE)
    {
      prev_time = count;
      switch (state)
      {
      case STATE_1:
        state = STATE_2;
        // uart_send_msg("state1");
        accel_config(MOTION_DETECTION, 16, 10);
        break;
      case STATE_2:
        // uart_send_msg("state2");
        state = STATE_3;
        accel_config(FREE_FALL, 17, 5);
        break;
      case STATE_3:
        // uart_send_msg("state3");
        int8_t x = 0, y = 0, z = 0;
        x = i2c_read_single_byte(ACCEL_DEVICE_ADDRESS, 0x01);
        y = i2c_read_single_byte(ACCEL_DEVICE_ADDRESS, 0x03);
        z = i2c_read_single_byte(ACCEL_DEVICE_ADDRESS, 0x05);

        float currentX_f;
        float currentY_f;
        float currentZ_f;
        currentX_f = convert_to_g(x, SENSITIVITY);
        currentY_f = convert_to_g(y, SENSITIVITY);
        currentZ_f = convert_to_g(z, SENSITIVITY);

        // xem 2 c�i c� su sai lech hay khong

        // 0 -1 0
        // 0 0 -1
        // 0 0 1
        // 1 0 0
        // -1 0 0

        if (abs(currentY_f, initY_f) >= EPSILON && (abs(currentX_f, initY_f) >= EPSILON || abs(currentZ_f, initZ_f) >= EPSILON))
        {

          led_on(RED_LED);
          lcd_display_digit(NUM_1);
          state = STATE_4; // TRANG THAI DA NGA
        }

        else
        {
          accel_config(FREE_FALL, 6, 3);
          state = STATE_1;
        }

        break;
      }
    }
    PORTC->PCR[INT1_MMA_PIN] |= CLEAR_INTR_PORTC_PORTD;
  }

  // clear interrupt
}

// Function to convert the raw value to g
float convert_to_g(int8_t raw_value, float sensitivity)
{
  return raw_value * sensitivity;
}

void PIT_IRQHandler()
{
  if (PIT->CHANNEL[0].TFLG == 1)
  {
    count++;
    if (main_status == MAIN_STATUS_ACTIVE)
    {
      led_toggle(GREEN_LED);
    }
    else if (main_status == MAIN_STATUS_STAND_BY)
    {
    }
    PIT->CHANNEL[0].TFLG |= 1; // Clear timer interrupt flag
  }
  if (PIT->CHANNEL[1].TFLG == 1)
  {
    if (state == STATE_4 && main_status == MAIN_STATUS_ACTIVE)
    {
      led_toggle(RED_LED);
    }
    PIT->CHANNEL[1].TFLG |= 1; // Clear timer interrupt flag
  }
}

#ifdef HIEU_DEBUG
void read_accel_data(uint32_t delay)
{
  int x, y, z;
  x = i2c_read_single_byte(ACCEL_DEVICE_ADDRESS, 0x01);
  y = i2c_read_single_byte(ACCEL_DEVICE_ADDRESS, 0x03);
  z = i2c_read_single_byte(ACCEL_DEVICE_ADDRESS, 0x05);

  // Convert the raw data to g
  float sensitivity = 0.0625; // Sensitivity for �8g range
  float x_g = convert_to_g(x, sensitivity);
  float y_g = convert_to_g(y, sensitivity);
  float z_g = convert_to_g(z, sensitivity);
  // x = convert_twos_complement(x);
  // y = convert_twos_complement(y);
  // z = convert_twos_complement(z);

  snprintf(buffer, sizeof(buffer), "[%.2f, %.2f, %.2f] - ", x_g, y_g, z_g);

  uart_send_msg(buffer);

  delay_ms(delay);
}
#endif