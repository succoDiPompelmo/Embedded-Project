// Define IDH that we are going to use for the communication between node
#define START    0x00
#define STOP     0xFF
#define GOAL     0x55
#define JOYSTICK 0X30
#define SLIDER   0X20
#define DOWN     0x72
#define UP       0x73

#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <stdbool.h>
#include <avr/interrupt.h>


#include "communication_controller.h"
#include "CAN_interface.h"
#include "PID.h"
#include "pwm.h"

// Boolean variable that activate the ping pong
volatile bool start = false;
volatile int sum;

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

void incoming_communication()
{
  // Read the message received
  CAN_Receive();
  // If the player has pressed the start button
  if (start)
  {
    // Check if we received a message that contains the Joystick position
    if (get_IDH() == JOYSTICK)
    {
      volatile int value;
      // Read the joystick position and save it in a variable
      value = getData();
      // Computation to transform the joystick position into a square wave
      sum = 2000 + value*7.8;
      // Set the square wave
      pwn_set_cycle(sum);
    }
    // Check if we received a message that contains the Slider position
    if (get_IDH() == SLIDER)
    {
      // Use a PID to compute the value to send to the DAC to control the DC motor
      //printf("%d\n\r", getData());
      PID_update(getData());
    }
  }
  // If we received the message to start the Game
  if (get_IDH() == START)
  {
    printf("%s\n\r", "START THE GAME");
    start = true;
  }
  // If we received the message to stop the Game
  if (get_IDH() == STOP)
  {
    printf("%s\n\r", "STOP THE GAME");
    start = false;
  }
  // MEssage to raise the difficulty
  if (get_IDH() == UP)
  {
    printf("%s\n", "DIFFICULTY UP");
    raise_up_difficulty();
  }
  // Message to lower the difficulty
  if (get_IDH() == DOWN)
  {
    printf("%s\n", "DIFFICULTY UP");
    raise_down_difficulty();
  }
}

// Interrupt service routine for CAN bus
ISR(INT2_vect)
{
  cli();
  printf("%s\n\r", "INTERRUPT BUTTON!");
  sei();
}

// Interrupt service routine for CAN bus incoming communication
ISR(INT4_vect)
{
  cli();
  // Manage all the incoming comunication
  incoming_communication();
  sei();
}
