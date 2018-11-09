#include <avr/interrupt.h>
#include <avr/io.h>

#include "Timer_handler.h"

#define LED PB1

ISR(TIMER1_OVF_vect)
{
  cli();
  PORTB ^= (1 << LED);
  TCNT1 = 63974;

  sei();
}

void Timer_Init()
{
  DDRB |= (1 << LED);

  TCNT1 = 63974; // For 1 sec at 16 Mhz

  TCCR1A = 0x00;
  // Timer mode with 1024 prescaler
  TCCR1B = (1 << CS12);
  // Enable timer2 overflow interrupt
  TIMSK = (1 << TOIE1);
}
