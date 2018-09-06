#include <avr/io.h>
#include "led.h"

void led_init()
{
  DDRA |= (1 << PA2);
  PORTA |= (1 << PA2);
}

void led_turn_on()
{
  PORTA &= ~(1 << PA2);
}

void led_turn_off()
{
  PORTA |= (1 << PA2);
}
