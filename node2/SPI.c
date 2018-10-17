#include <avr/io.h>

#include "SPI.h"

#define MOSI PB2;
#define SS   PB0;
#define MISO PB3;
#define SCK  PB1;

void SPI_MasterInit()
{
  /* Set MOSI, SCK and !SS output, all others input */
  DDRB = (1 << MOSI)|(1 << SCK)|(1 << SS);

  /* Enable SPI, Master, set clock rate fck/16 */
  SPCR = (1 << SPE)|(1 << MSTR)|(1 << SPR0);

  /* Set SS high, deselect CAN-controller */
  PORTB |= (1 << SS);
}

void SPI_MasterTransmit(char cData)
{
  /* Start transmission */
  SPDR = cData;
  /* Wait for transmission complete */
  while(!(SPSR & (1 << SPIF)));

  // When we finish to transmit the SPIF register goes to 1 and we need to reset it
  // if we want to transmit again.This is done
  // first by reading the SPSR register with SPIF set, loop above, and then accesing the SPI data
  // data register, this line.
  cData = SPDR;
}

void SPI_SlaveInit()
{
  /* Set MISO output, all others input */
  DDRB = (1 << MISO);
  /* Enable SPI */
  SPCR = (1 << SPE);
}

uint8_t SPI_read()
{
  //Send dummy data to read from the slave
  SPI_MasterTransmit(0);

  /* Wait for data to be received */
  //while(!(SPSR & (1<<SPIF)));

  return SPDR;

}

char SPI_SlaveReceive()
{
  /* Wait for reception complete */
  while(!(SPSR & (1 << SPIF)));
  /* Return data register */
  return SPDR;
}
