#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>

#define F_CPU 4915200UL
#include <util/delay.h>

#include <avr/pgmspace.h>
#include <stdbool.h>

#include "MCP2515.h"
#include "SPI.h"
#include "can.h"

// TODO : Move the selection/deselection of the CAN into the SPI interaface

#define SS PB0

void mcp2515_reset()
{
  PORTB &= ~(1<<PB0); //Select CAN-controller
  SPI_MasterTransmit(MCP_RESET);

  PORTB |= (1<<PB0);   //Deselect CAN-controller
}

void mcp2515_bit_modify(uint8_t adress, uint8_t mask, uint8_t data)
{
  PORTB &= ~(1<<PB0); //Select CAN-controller
  SPI_MasterTransmit(MCP_BITMOD);
  SPI_MasterTransmit(adress);
  SPI_MasterTransmit(mask);
  SPI_MasterTransmit(data);
  PORTB |= (1<<PB0);   //Deselect CAN-controller
}


 void mcp2515_request_to_send()
 {
   PORTB &= ~(1<<PB0); //Select CAN-controller
   SPI_MasterTransmit(MCP_RTS_TX0);
   PORTB |= (1<<PB0);   //Deselect CAN-controller
 }


uint8_t mcp2515_read_status(uint8_t type)
{
  uint8_t data;
  PORTB &= ~(1<<PB0); //Select CAN-controller
  SPI_MasterTransmit(MCP_READ_STATUS);
  data=SPI_read();
  PORTB |= (1<<PB0);   //Deselect CAN-controller

  return data;
}



void mcp2515_write_register( uint8_t adress, uint8_t data )
{
	PORTB &= ~(1<<PB0); //Select CAN-controller

  SPI_MasterTransmit(MCP_WRITE);  //Send write instruction
  SPI_MasterTransmit(adress); //Send addressing
  SPI_MasterTransmit(data); //Send data

  PORTB |= (1<<PB0);   //Deselect CAN-controller
}



uint8_t mcp2515_read(uint8_t address)
{
  uint8_t result;
  PORTB &= ~(1<<PB0); //Select CAN-controller

  SPI_MasterTransmit(MCP_READ);  //Send read instruction
  SPI_MasterTransmit(address); //Send addressing
  result=SPI_read();

  PORTB |= (1<<PB0);   //Deselect CAN-controller

  return result;
}

uint8_t mcp2515_init()
{
  DDRB |= (1 << PB0);
  uint8_t value;

  SPI_MasterInit();   //Initialize SPI
  mcp2515_reset();    //Send reset-command

  _delay_ms(100.0);

  printf("work! \n\r");
  //Self-test
  value = mcp2515_read(MCP_CANSTAT);
  printf("%02x\n", value);
  if((value & MODE_MASK) != MODE_CONFIG) {
    printf("MCP2515 in NOT in configuration mode after reset! \n\r");
    return 1;
  }
  // More initialization

  return 0;
}
