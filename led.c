#include <avr/io.h>
#include "led.h"

void led_init()
{
  DDRB |= (1 << PB0);
  PORTB |= (1 << PB0);
}

void led_turn_on()
{
  PORTB &= ~(1 << PB0);
}

void led_turn_off()
{
  PORTB |= (1 << PB0);
}
