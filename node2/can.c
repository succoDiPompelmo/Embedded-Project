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

#define SS PB7

void mcp2515_reset()
{
  // Select CAN-controller
  PORTB &= ~(1<<PB7);
  // Send the reset command to the MCP2515
  SPI_MasterTransmit(MCP_RESET);
  //Deselect CAN-controller
  PORTB |= (1<<PB7);
}

void mcp2515_bit_modify(uint8_t address, uint8_t mask, uint8_t data)
{
  // Select CAN-controller
  PORTB &= ~(1<<PB7);
  // Send the bit modify command to the MCP2515
  SPI_MasterTransmit(MCP_BITMOD);
  // Send the adress of the register to the CAN-Controller
  SPI_MasterTransmit(address);
  // Send the mask to the CAN-Controller
  SPI_MasterTransmit(mask);
  // Send the data to write in the regoster to the CAN-Controller
  SPI_MasterTransmit(data);
  // Deselect CAN-controller
  PORTB |= (1<<PB7);
}


 void mcp2515_request_to_send()
 {
   // Select CAN-controller
   PORTB &= ~(1<<PB7);
   // Send command to initiate the trasmission for the trasmission buffer TXB0
   SPI_MasterTransmit(MCP_RTS_TX0);
   // Deselect CAN-controller
   PORTB |= (1<<PB7);
 }


uint8_t mcp2515_read_status(uint8_t type)
{
  uint8_t data;
  // Select CAN-controller
  PORTB &= ~(1<<PB7);
  // Send command to read the status bits
  SPI_MasterTransmit(MCP_READ_STATUS);
  // Read the status bits and save it
  data=SPI_read();
  // Deselect CAN-controller
  PORTB |= (1<<PB7);

  return data;
}



void mcp2515_write_register( uint8_t address, uint8_t data )
{
  // Select CAN-controller
	PORTB &= ~(1<<PB7);
  // Send write command to the Can-Controller
  SPI_MasterTransmit(MCP_WRITE);
  // Send the adress of the register to write
  SPI_MasterTransmit(address);
  // Send the data to write
  SPI_MasterTransmit(data);
  // Deselect CAN-controller
  PORTB |= (1<<PB7);
}



uint8_t mcp2515_read(uint8_t address)
{
  uint8_t result;
  // Select CAN-controller
  PORTB &= ~(1<<PB7);
  // Send read command to the Can-Controller
  SPI_MasterTransmit(MCP_READ);
  // Send the adress of the register to read
  SPI_MasterTransmit(address);
  // Read and save the register
  result=SPI_read();
  // Deselect CAN-controller
  PORTB |= (1<<PB7);

  return result;
}

uint8_t mcp2515_init()
{
  // Set SS as OUTPUT
  DDRB |= (1 << PB7);
  uint8_t value;

  // Initialize SPI
  SPI_MasterInit();
  // Send reset-command
  mcp2515_reset();

  _delay_ms(100.0);

  printf("work! \n\r");
  value = mcp2515_read(MCP_CANSTAT);
  if((value & MODE_MASK) != MODE_CONFIG) {
    printf("MCP2515 in NOT in configuration mode after reset! \n\r");
    return 1;
  }
  // More initialization

  return 0;
}
