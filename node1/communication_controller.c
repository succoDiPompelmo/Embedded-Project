#define START 0x00
#define STOP  0xFF
#define GOAL  0x55

#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <stdbool.h>
#include <avr/interrupt.h>


#include "communication_controller.h"
#include "CAN_interface.h"

void start_message()
{
  setData(0x00);
  setIDH(START);
  CAN_Trasmission();
}

void stop_message()
{
  setData(0x00);
  setIDH(STOP);
  CAN_Trasmission();
}

void goal_message()
{
  setData(0x00);
  setIDH(0X55);
  CAN_Trasmission();
}

// Interrupt service routine for CAN bus
ISR(INT2_vect)
{
  cli();
  printf("%s\n\r", "INTERRUPT BUTTON!");
  sei();
}
