// Define IDH that we are going to use for the communication between node
#define START 0x00
#define STOP  0xFF
#define GOAL  0x55
#define DOWN  0x72
#define UP    0x73

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
// Send the message to stop the game
void stop_message()
{
  // Dummy data
  setData(0x00);
  // Set high adress
  setIDH(STOP);
  // Transmit the initialise message
  CAN_Trasmission();
}
// Send the goal message
void goal_message()
{
  // Dummy data
  setData(0x00);
  // Set high adress
  setIDH(GOAL);
  // Transmit the initialise message
  CAN_Trasmission();
}
// Send message to change the difficulty
void change_difficulty(int i)
{
  // Dummy data
  setData(0x00);
  // Set high adress
  if (i == 0)
  {
    setIDH(UP);
  }
  else
  {
    setIDH(DOWN);
  }
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
