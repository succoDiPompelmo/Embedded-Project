// Define IDH that we are going to use for the communication between node
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

// Send the message to start the GAME
void start_message()
{
  // Dummy data
  setData(0x00);
  // Set high adress
  setIDH(START);
  // Transmit the initialise message
  CAN_Trasmission();
}

void stop_message()
{
  // Dummy data
  setData(0x00);
  // Set high adress
  setIDH(STOP);
  // Transmit the initialise message
  CAN_Trasmission();
}

void goal_message()
{
  // Dummy data
  setData(0x00);
  // Set high adress
  setIDH(GOAL);
  // Transmit the initialise message
  CAN_Trasmission();
}

// Interrupt service routine for CAN bus
ISR(INT2_vect)
{
  cli();

  printf("%s\n\r", "INTERRUPT BUTTON!");

  sei();
}
