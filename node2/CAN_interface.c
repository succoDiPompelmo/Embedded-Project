#include <stdbool.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "can.h"
#include "MCP2515.h"
#include "CAN_interface.h"
#include "DAC.h"

#define test_bit(reg, bit) (reg & (1 << bit))

volatile bool received = false;


typedef struct message {
  uint8_t IDH;
  uint8_t IDL;
  uint8_t data;
  uint8_t length;
} message;

struct message l = {0x01, 0x00, 0x14, 0x01};

struct message lr = {0x01, 0x00, 0x14, 0x01};

void setData(uint8_t data)
{
  l.data = data;
}

int data_GLOBAL;

int get_DATA_GLOBAL()
{
  return data_GLOBAL;
}

int get_IDH()
{
  return lr.IDH;
}


void CAN_Init()
{
  mcp2515_init();

  // RX0 - Turn masks/filter off, rollover disabled
  mcp2515_bit_modify(MCP_RXB0CTRL, 0b01100100, 0xFF);

    // Enable interrupt when message is received (RX0IE = 1)
  mcp2515_bit_modify(MCP_CANINTE, 0b00000001, 1);
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

    data_GLOBAL = mcp2515_read(MCP_RXB0D0);

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
  CAN_Receive();
  if (get_IDH() == 0x30)
  {
    volatile int value;
    value = get_DATA_GLOBAL();
    sum = 2000 + value*7.8;
    pwn_set_cycle(sum);
  }
  if (get_IDH() == 0x20)
  {
    if (get_DATA_GLOBAL() < 0x80)
    {
      PORTH &= ~(1 << PH1);
    }
    if (get_DATA_GLOBAL() > 0x80)
    {
      PORTH |= (1 << PH1);
    }
    if (get_DATA_GLOBAL() > 0x80) DAC_write(get_DATA_GLOBAL() - 0x80);
    else DAC_write(0x80 - get_DATA_GLOBAL());
  }
}

// Interrupt service routine for CAN bus
ISR(INT4_vect)
{
  cli();
  //printf("%s\n\r", "INTERRUPT node2!");
  received = true;
  //mcp2515_bit_modify(MCP_CANINTF, 0x01, 0);
  manage_message();
  sei();
}
