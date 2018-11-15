#include <stdio.h>
#include <avr/io.h>

#define F_CPU 16000000UL
#include <util/delay.h>

#include "ADC.h"

void ADC_Init()
{
  // Set as input a pin on the PORTF to read the IR signal
  DDRF &= ~(1 << PINF0);
  // Set the voltage reference to AVCC and left adjust the result
  ADMUX &= ~(1 << REFS1);
  ADMUX |=  (1 << REFS0) | (1 << ADLAR);
  // Set prescaler of 128
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
  // Enable the ADC and start first conversion
  ADCSRA |= (1 << ADEN)  | (1 << ADSC);
}

uint8_t ADC_Read()
{
  // Enable the ADC and start the conversion
  ADCSRA |= (1 << ADEN) | (1 << ADSC);
  // Delay to wait for the conversion
  _delay_ms(10);
  // Read the result of the conversion
  return ADCH;
}
