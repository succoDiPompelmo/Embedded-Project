#include <stdio.h>
#include <avr/io.h>

void pwn_set()
{
  // Set FAST PWN mode
  TCCR1A |= (1 << WGM11) | (1 << COM1A1);
  //TCCR1A &= ~(1 << COM1A0) & ~(1 << WGM10);
  TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS11);
  //TCCR1B &= ~(1 << CS10) & ~(1 << CS12);

  ICR1 = 40000;

  OCR1A = 4000;
}
