#include <avr/io.h>

#include "SPI.h"

void SPI_MasterInit()
{
  /* Set MOSI and SCK output, all others input */
  DDRB = (1<<DDB5)|(1<<DDB7)|(1<<DDB4);

  /* Enable SPI, Master, set clock rate fck/16 */
  SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
  PORTB |= (1<<PB4);
}

void SPI_MasterTransmit(char cData)
{
  //PORTB &= ~ (1<< PB4);
  /* Start transmission */
  SPDR = cData;
  /* Wait for transmission complete */
  while(!(SPSR & (1<<SPIF)));

  cData = SPDR;
  //PORTB |= (1<<PB4);
}

void SPI_SlaveInit()
{
  /* Set MISO output, all others input */
  DDRB = (1<<PB6);
  /* Enable SPI */
  SPCR = (1<<SPE);
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
  while(!(SPSR & (1<<SPIF)));
  /* Return data register */
  return SPDR;
}
