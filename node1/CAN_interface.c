#include <stdbool.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#define F_CPU 4915200UL
#include <util/delay.h>

#include "can.h"
#include "MCP2515.h"
#include "CAN_interface.h"
#include "oled_interface.h"

#define test_bit(reg, bit) (reg & (1 << bit))

// Struct that define the content of a message
typedef struct message {
  uint8_t IDH;
  uint8_t IDL;
  uint8_t data;
  uint8_t length;
} message;

// Global variable that contain save the content of a message
struct message l = {0x01, 0x00, 0x20, 0x01};

struct message lr = {0x01, 0x00, 0x20, 0x01};

// Set the data field in the message variable
void setData(uint8_t data)
{
  l.data = data;
}

// Set the IDH field in the message variable
void setIDH(uint8_t data)
{
  l.IDH = data;
}


// Init the can
void CAN_Init()
{
  // Init the the CAN-Cotroller
  mcp2515_init();

  // RX0 - Turn masks/filter off, rollover disabled
  mcp2515_bit_modify(MCP_RXB0CTRL, 0b01100100, 0xFF);

  // Enable loop-back mode
  mcp2515_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_NORMAL);

  // Enable interrupt when message is received (RX0IE = 1)
  mcp2515_bit_modify(MCP_CANINTE, 0x01, 1);
}

void CAN_Trasmission()
{
  // Check if the previous transmission has finished
  if (CAN_Trasmission_Complete())
  {
    // Set message ID
    mcp2515_write_register(MCP_TXB0SIDH, l.IDH);
    mcp2515_write_register(MCP_TXB0SIDL, l.IDL);

    // Set data length
    mcp2515_write_register(MCP_TXB0DLC, l.length);

    // Set data bytes
    mcp2515_write_register(MCP_TXB0D0, l.data);

    mcp2515_request_to_send();
  }
}

uint8_t CAN_Receive()
{
  // Get message ID
  lr.IDH = mcp2515_read(MCP_RXB0SIDH);
  l.IDL = mcp2515_read(MCP_RXB0SIDL);

  // Get data length
  l.length = mcp2515_read(MCP_RXB0DLC);

  // Get the data
  l.data = mcp2515_read(MCP_RXB0D0);

  // Reset the interrupt flag bit
  mcp2515_bit_modify(MCP_CANINTF, 0x01, 0);

  // GOAL
  if (lr.IDH == 0x55)
  {
     oled_clear();
     //printf("%d\n\r", lr.IDH);
     oled_print("GOAL", 4);
     _delay_ms(500);
     lr.data = 0x00;
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

// Interrupt service routine for CAN bus
ISR(INT1_vect)
{
  cli();
  // Handle the reception of a message
  CAN_Receive();
  sei();
}
