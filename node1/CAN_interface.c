#include <stdbool.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "can.h"
#include "MCP2515.h"
#include "CAN_interface.h"

#define test_bit(reg, bit) (reg & (1 << bit))

bool received = false;


typedef struct message {
  uint8_t IDH;
  uint8_t IDL;
  uint8_t data;
  uint8_t length;
} message;

struct message l = {0x01, 0x00, 0x20, 0x01};

void setData(uint8_t data)
{
  l.data = data;
}


void CAN_Init()
{
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
  // Message data
  uint8_t data[4];

  if(received)
  {
    // Get message ID
    data[0] = mcp2515_read(MCP_RXB0SIDH);
    data[1] = mcp2515_read(MCP_RXB0SIDL);

    // Get data length
    data[2] = mcp2515_read(MCP_RXB0DLC);

    data[3] = mcp2515_read(MCP_RXB0D0);

    received = false;
    mcp2515_bit_modify(MCP_CANINTF, 0x01, 0); //write what it does

    return data[3];
  }
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

// INterrupt service routine for CAN bus
ISR(INT1_vect)
{
  //_delay_ms(10000);
  printf("%s\n\r", "INTERRUPT node1!");
  received = true;
}
