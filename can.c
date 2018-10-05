#include "can.h"

uint8_t mcp2515_read(uint8_t address)
{
  uint8_t result;
  PORTB &=~(1<<CAN_CS;) //Select CAN-controller

  SPI_write(MCP_READ);  //Send read instruction
  SPI_write(address); //Send addressing
  result=SPI_read()

  PORTB|=(1<<CAN_CS);   //Deselect CAN-controller

  return result;
}

uint8_t mcp2515_init()
{
  uint8_t value;

  SPI_init();   //Initialize SPI
  mcp2515_reset();  //Send reset-command

  //Self-test
  mcp2515_read(MCP_CANSTAT, &value);
  if((value & MODE_MASK) != MODE_CONFIG) {
    printf("MCP2515 in NOT in configuration mode after reset! \n");
    return1;
  }
  // More initialization

  return 0;
}
