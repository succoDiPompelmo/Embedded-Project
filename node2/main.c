#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define FOSC 16000000// Clock Speed
#define BAUD 9600
#define MYBURR FOSC/16/BAUD-1

#define F_CPU 16000000UL
#include <util/delay.h>

#include "uart_interface.h"
#include "CAN_interface.h"
#include "pwn.h"

int main()
{

  USART_Init (MYBURR);
  fdevopen(*USART_Transmit, *USART_Receive);

  // We want PB0 as output
  DDRB |= (1 << PB0);

  DDRB |= (1 << PB5);

  DDRD &= ~(1 << PD1);
  cli();
  EICRA |= (1 << ISC11);
  EIMSK |= (1 << INT1);

  sei();

  CAN_Init();

  pwn_set();

  while(1)
  {
    volatile uint8_t value;

    //CAN_Trasmission();

    _delay_ms(1000.0);

    value = CAN_Receive();

    printf("VALUE : %02x\n\r", value);

    //setData(0x97);
  }
}
