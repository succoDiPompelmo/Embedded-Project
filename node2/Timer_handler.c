#include <avr/interrupt.h>
#include <avr/io.h>

#include "Timer_handler.h"

#define LED PG1

ISR(TIMER3_OVF_vect)
{
  cli();
  PORTG ^= (1 << LED);
  TCNT3 = 63974;
  sei();
}

void Timer_Init()
{
  DDRG |= (1 << LED);

  TCNT3 = 63974; // For 1 sec at 16 Mhz

  TCCR3A = 0x00;
  // Timer mode with 1024 prescaler
  TCCR3B = (1 << CS30) | (1 << CS32);
  // Enable timer2 overflow interrupt
  TIMSK3 = (1 << TOIE3);
}
