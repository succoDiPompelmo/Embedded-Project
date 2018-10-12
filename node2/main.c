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

int main()
{

  USART_Init (MYBURR);
  fdevopen(*USART_Transmit, *USART_Receive);

  EIMSK |= (1 << INT0);

  sei();

  printf("%s\n", "QUI");

  CAN_Init();

  while(1)
  {
    uint8_t value;

    printf("%s\n", "QUI");

    CAN_Trasmission();

    _delay_ms(10.0);

    value = CAN_Receive();

    printf("%02x\n\r", value);

    setData(0x97);
  }
}
