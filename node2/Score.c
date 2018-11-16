#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#include <util/delay.h>

#include "CAN_interface.h"
#include "DAC.h"
#include "Score.h"
#include "uart_interface.h"

int score_stack = 20;

uint8_t score = 0;

#define LED PL0

void Score_Init()
{
  // Init the ADC
  ADC_Init();
  // Send the initial socre to the web server
  score_message(score);
  // Initialise the goal led
  DDRL |= (1 << LED);
  PORTL |= (1 << LED);
}

void Score_Update()
{

  // Read hte value converted by the ADC
  uint8_t value = ADC_Read();
  // If the value from the ADC is less than 10 we can decrease the counter for the GOAL
  if (value < 10)
  {
    score_stack--;
  }
  // If the ball is not more on the bottom of the box increase the counter
  if (value > 10 && score_stack <= 20)
  {
    score_stack++;
  }
  // If the ball stayed at the bottom of the box for 50 cycle it's GOAL
  if (score_stack == 0)
  {
    // Disable interrupt
    cli();
    // Set goal IDH
    setIDH(0x55);
    // Set dummy data
    setData(0x00);
    // Transmit the message
    CAN_Trasmission();
    // Toggle the led
    PORTL ^= (1 << LED);
    // Wait
    _delay_ms(100);
    // Increase the score
    score++;
    // Send the score message to the web server
    score_message(score);
    // Toggle the led
    PORTL ^= (1 << LED);
    // Reanable the interrupt
    sei();
    // Restart the count
    score_stack = 50;
  }
}
