#include <avr/interrupt.h>
#include <avr/io.h>

#include "Timer_handler.h"

#define LED PG1

ISR(TIMER3_OVF_vect)
{
  cli();
  // Toggle the LED
  PORTG ^= (1 << LED);
  // Restart the count for the timer
  TCNT3 = 63974;
  sei();
}

void Timer_Init()
{
  // Set the pin of the LED as OUTPUT
  DDRG |= (1 << LED);
  // Set from wich number the counter must start
  TCNT3 = 63974; // For 1 sec at 16 Mhz

  TCCR3A = 0x00;
  // Timer mode with 1024 prescaler
  TCCR3B = (1 << CS30) | (1 << CS32);
  // Enable timer2 overflow interrupt
  TIMSK3 = (1 << TOIE3);
}
