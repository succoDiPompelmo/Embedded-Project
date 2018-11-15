#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#include <util/delay.h>

#define FOSC 16000000// Clock Speed
#define BAUD 9600
#define MYBURR FOSC/16/BAUD-1

#include "uart_interface.h"
#include "CAN_interface.h"
#include "pwm.h"
#include "DAC.h"
#include "Timer_handler.h"
#include "encoder_interface.h"
#include "Score.h"

int main()
{

  cli();

  USART_Init (MYBURR);
  fdevopen(*USART_Transmit, *USART_Receive);

  // We want PB0 as output
  DDRB |= (1 << PB0);
  DDRH |= (1 << PH1);

  DDRB |= (1 << PB5);
  DDRD &= ~(1 << PD4);
  EICRA |= (1 << ISC11);
  EIMSK |= (1 << INT4);

  // Initialise all the modules

  DAC_init();
  CAN_Init();
  Timer_Init();
  Encoder_Init();
  pwn_set();
  PID_Init();
  Score_Init();

  // Bring the DC motor in the initial position
  PORTH |= (1 << PH1);
  DAC_write(0x50);
  _delay_ms(100);

  sei();


  while(1)
  {

    Score_Update();

    _delay_ms(10);

    Encoder_Read();
  }
}
