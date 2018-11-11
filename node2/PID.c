#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#include <util/delay.h>

volatile int16_t e;
volatile uint8_t last_e;
volatile int16_t pwm;

float Kp = 0.1;
float Kd = 0.0;
float Ki = 0.0;

#include "PID.h"
#include "encoder_interface.h"
#include "DAC.h"

void PID_Init()
{
  Kp = 0.05;
}

void PID_update(uint8_t target_8)
{
  // Get the current position
  int16_t current = ((get_MSB() << 8) | get_LSB());
  // Cast the target from an 8 bit value to an 16 bit value
  int16_t target = (int16_t) target_8;
  // Multiply to extend the range of the target on all the 16 bit values
  target = target * 35;
  // Compute the error
  e = target - current;
  // Calculate the control variable
  pwm = (0.05 * e);
  //+ (Ki * e) + (Kd * derivative);

  // Limit the control variable
  if (pwm > 255) pwm = 255;
  else if(pwm < - 255) pwm = -255;

  // if the control variable is positive, run the motor clockwise
  if (pwm > 0) PORTH &= ~(1 << PH1);
  // If the control variable is negative, run the motor counter clockwise
  if (pwm < 0) PORTH |= (1 << PH1);

  // If the control variable is negative, multiply for -1 to set it positive
  if (pwm < 0) pwm = pwm * -1;

  // DAC accept an 8 bits value so we need to cast the control variable
  uint8_t pwm_8 = (uint8_t) pwm;

  printf("PWM_8 %d\n\r", pwm_8);

  // Sedn the value of the control variable to the DAC
  DAC_write(pwm);

  //last_e = e;

}
