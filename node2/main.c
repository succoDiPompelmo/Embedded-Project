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
#include "pwn.h"
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


  PORTH &= ~(1 << PH1);

  DAC_init();

  CAN_Init();

  Timer_Init();

  Encoder_Init();

  pwn_set();

  PID_Init();

  Score_Init();

  // adc init
  // ADC_Init()
  // DDRF &= ~(1 << PINF0);
  //
  // ADMUX &= ~(1 << REFS1);
  // ADMUX |= (1 << REFS0) | (1 << ADLAR);
  //
  // ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
  // ADCSRA |= (1 << ADEN) | (1 << ADSC);

  sei();


  while(1)
  {
    volatile int value;
    volatile int sum;

    //CAN_Trasmission();

    Score_Update();

    // ADCSRA |= (1 << ADEN) | (1 << ADSC);

    _delay_ms(10);

    Encoder_Read();

    // value = ADCH;

    //printf("ADCH :   %d\n\r", value);

    // if (value < 10)
    // {
    //   score_stack--;
    // }
    //
    // if (value > 10 && score_stack <= 20)
    // {
    //   score_stack++;
    // }
    //
    // if (score_stack == 0)
    // {
    //   printf("%s\n\r", "GOAL");
    //   // Move to the communication Controller
    //   cli();
    //   //setIDH(0x55);
    //   //setData(0x00);
    //   //CAN_Trasmission();
    //   sei();
    //   score_stack = 50;
    // }
  }
}
