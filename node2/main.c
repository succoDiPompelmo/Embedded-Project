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

  // Initialise usart
  USART_Init (MYBURR);
  fdevopen(*USART_Transmit, *USART_Receive);

  // We want PB0 as output
  DDRB |= (1 << PB0);
  // We want PH1 as output to control the motor direction of motion
  DDRH |= (1 << PH1);
  // Pin for the pwm output
  DDRB |= (1 << PB5);
  // D
  DDRD &= ~(1 << PD4);
  // Enable interrupt 4 on rising edge
  EICRA |= (1 << ISC11);
  EIMSK |= (1 << INT4);

  web_message();

  // Initialise all the modules

  DAC_init();
  CAN_Init();
  Timer_Init();
  //Encoder_Init();
  pwn_set();
  PID_Init();
  Score_Init();

  // Bring the DC motor in the initial position
  PORTH |= (1 << PH1);
  DAC_write(0x55);
  _delay_ms(2000);

  Encoder_Init();

  sei();


  while(1)
  {
    // Update the score
    Score_Update();
    _delay_ms(10);
    // Read the Encoder
    Encoder_Read();
    // Save the slidebar position into a 16 bit variable
    int16_t position = ((get_MSB() << 8) | get_LSB());
    // Send the position of the slidebar to the web server
    position_message(position);
    //check_incoming_message();
  }
}
