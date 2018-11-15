#include <stdio.h>
#include <avr/io.h>

#include "pwm.h"

void pwn_set()
{
  // Set FAST PWN mode and prescaler of 1024
  TCCR1A |= (1 << WGM11) | (1 << COM1A1);
  //TCCR1A &= ~(1 << COM1A0) & ~(1 << WGM10);
  TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS11);
  //TCCR1B &= ~(1 << CS10) & ~(1 << CS12);

  // Top of the Counter
  ICR1 = 40000;
  // Value in wich the output ot the counter change
  OCR1A = 3000;
}

void pwn_set_cycle(int value)
{
  // Set the duty cycle
  OCR1A = value;
}
