#include <stdbool.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

#include "can.h"
#include "MCP2515.h"
#include "CAN_interface.h"
#include "DAC.h"
#include "PID.h"

#define test_bit(reg, bit) (reg & (1 << bit))

volatile bool received = false;
// Boolean variable that activate the ping pong
volatile bool start = false;

// Struct that define the content of a message
typedef struct message {
  uint8_t IDH;
  uint8_t IDL;
  uint8_t data;
  uint8_t length;
} message;

// Global variable that contain/save the content of a message to transmit
struct message l = {0x01, 0x00, 0x14, 0x01};
// Global variable that contain/save the content of a message to receive
struct message lr = {0x01, 0x00, 0x14, 0x01};

// Set the data in the message to transmit
void setData(uint8_t data)
{
  l.data = data;
}

// Set the high adress in the message to transmit
void setIDH(uint8_t data)
{
  l.IDH = data;
}

// Global variable to save the data
int data_GLOBAL;

// Return the global variable
int get_DATA_GLOBAL()
{
  return data_GLOBAL;
}

// Return the high adress of the message received
int get_IDH()
{
  return lr.IDH;
}


void CAN_Init()
{
  // Init Can-Controller
  mcp2515_init();

  // RX0 - Turn masks/filter off, rollover disabled
  mcp2515_bit_modify(MCP_RXB0CTRL, 0b01100100, 0xFF);

    // Enable interrupt when message is received (RX0IE = 1)
  mcp2515_bit_modify(MCP_CANINTE, 0b00000001, 1);
  // Delay
  _delay_ms(1);
  // Enable loop-back mode
  mcp2515_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_NORMAL);


}

void CAN_Trasmission()
{
  if (CAN_Trasmission_Complete())
  {
    // Set message ID
    mcp2515_write_register(MCP_TXB0SIDH, l.IDH);
    mcp2515_write_register(MCP_TXB0SIDL, l.IDL);
    // Set data length
    mcp2515_write_register(MCP_TXB0DLC, l.length);
    // Set data bytes
    mcp2515_write_register(MCP_TXB0D0, l.data);
    // Send the command to transmit the content in the transmission buffer tothe Can-Controller
    mcp2515_request_to_send();
  }
}

uint8_t CAN_Receive()
{
  if(received)
  {

    volatile uint8_t data;
    // Get message ID
    lr.IDH = mcp2515_read(MCP_RXB0SIDH);
    data = mcp2515_read(MCP_RXB0SIDL);
    // Get data length
    data = mcp2515_read(MCP_RXB0DLC);
    // Get data
    data_GLOBAL = mcp2515_read(MCP_RXB0D0);
    // Reset the interrupt flag bit
    mcp2515_bit_modify(MCP_CANINTF, 0x01, 0);
    received = false;

    return data;
  }
  return 0;
}

int CAN_Trasmission_Complete()
{
  // Check if the trasmission of buffer TX0 is finished (TXREQ = 0)
  if (test_bit(mcp2515_read(MCP_TXB0CTRL), 3))
  {
    return 0;
  }
  else {
    return 1;
  }
}

volatile int sum;

void manage_message()
{
  // Read the message received
  CAN_Receive();
  // If the player has pressed the start button
  if (start)
  {
    // Check if we received a message that contains the Joystick position
    if (get_IDH() == 0x30)
    {
      volatile int value;
      // Read the joystick position and save it in a variable
      value = get_DATA_GLOBAL();
      // Computation to transform the joystick position into a square wave
      sum = 2000 + value*7.8;
      // Set the square wave
      pwn_set_cycle(sum);
    }
    // Check if we received a message that contains the Slider position
    if (get_IDH() == 0x20)
    {
      /*
      if (get_DATA_GLOBAL() < 0x80)
      {
        PORTH &= ~(1 << PH1);
      }
      if (get_DATA_GLOBAL() > 0x80)
      {
        PORTH |= (1 << PH1);
      }
      */
      //printf("%d\n", get_DATA_GLOBAL());
      //if (get_DATA_GLOBAL() > 0x80) DAC_write(get_DATA_GLOBAL() - 0x80);
      //else DAC_write(0x80 - get_DATA_GLOBAL());

      // Use a PID to compute the value to send to the DAC to control the DC motor
      PID_update(get_DATA_GLOBAL());
    }
  }
  // If we received the message to start the Game
  if (get_IDH() == 0x00)
  {
    printf("%s\n\r", "START THE GAME");
    start = true;
  }
  // If we received the message to stop the Game
  if (get_IDH() == 0xFF)
  {
    printf("%s\n\r", "STOP THE GAME");
    start = false;
  }
}

// Interrupt service routine for CAN bus
ISR(INT4_vect)
{
  cli();
  received = true;
  //mcp2515_bit_modify(MCP_CANINTF, 0x01, 0);
  manage_message();
  sei();
}
