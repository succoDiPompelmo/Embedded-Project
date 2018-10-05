#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <stdbool.h>

#include "MCP2515.h"
#include "SPI.h"
#include "can.h"

void mcp2515_reset()
{
  PORTB &= ~(1<<PB4); //Select CAN-controller
  SPI_MasterTransmit(MCP_RESET);

  PORTB |= (1<<PB4);   //Deselect CAN-controller
}

uint8_t mcp2515_read(uint8_t address)
{
  uint8_t result;
  PORTB &= ~(1<<PB4); //Select CAN-controller

  SPI_MasterTransmit(MCP_READ);  //Send read instruction 
  SPI_MasterTransmit(address); //Send addressing
  result=SPI_read();

  PORTB |= (1<<PB4);   //Deselect CAN-controller

  return result;
}

uint8_t mcp2515_init()
{
  DDRB |= (1 << PB4);
  uint8_t value;

  SPI_MasterInit();   //Initialize SPI
  mcp2515_reset();  //Send reset-command
  printf("work! \n");
  //Self-test
  value = mcp2515_read(MCP_CANSTAT);
  if((value & MODE_MASK) != MODE_CONFIG) {
    printf("MCP2515 in NOT in configuration mode after reset! \n");
    return 1;
  }
  // More initialization

  return 0;
}
