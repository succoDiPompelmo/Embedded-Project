#include <avr/interrupt.h>
#include <avr/io.h>

#include "Timer_handler.h"
#include "joystick.h"
#include "CAN_interface.h"

#define F_CPU 4915200UL
#include <util/delay.h>

#define LED PB1

void * Menu;

ISR(TIMER1_OVF_vect)
{
  cli();
  // Toggle the LED
  PORTB ^= (1 << LED);
  // Restart the count for the timer
  TCNT1 = 64000;
  //Menu_Print(Menu);

  setData(JOYCON_X_Axis());
  setIDH(0x30);
  CAN_Trasmission();

  _delay_ms(5);

  setData(SLIDEBAR_Left());
  setIDH(0x20);
  CAN_Trasmission();

  sei();
}

void Timer_Init(void * menu)
{
  // Set the pin of the LED as OUTPUT
  DDRB |= (1 << LED);
  // Set from wich number the counter must start
  TCNT1 = 64000;

  TCCR1A = 0x00;
  // Timer mode with 256 prescaler
  TCCR1B = (1 << CS12);
  // Enable timer1 overflow interrupt
  TIMSK = (1 << TOIE1);

  Menu = menu;
}
