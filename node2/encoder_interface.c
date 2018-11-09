#include <avr/interrupt.h>
#include <avr/io.h>

#define F_CPU 16000000UL
#include <util/delay.h>

#include "encoder_interface.h"
#include "DAC.h"

#define OE  PH5
#define SEL PH3
#define RST PH6

volatile uint8_t MSB_data;
volatile uint8_t LSB_data;

void Encoder_Init()
{
  // Set this pin as output
  DDRH |= (1 << OE) | (1 << SEL) | (1 << RST);
  // We want all the PIN K to be input
  DDRK = 0;

  PORTH &= ~(1 << RST);
  _delay_ms(20);
  PORTH |= (1 << RST);

}

uint8_t get_MSB()
{
  return MSB_data;
}

uint8_t get_LSB()
{
  return LSB_data;
}

uint8_t Encoder_Read()
{
  // Set OE low to enable output of encoder
  PORTH &= ~(1 << OE);
  // Set SEL low to get high byte
  PORTH &= ~(1 << SEL);
  // Wait 20 microseconds
  _delay_ms(20);
  // Read MSB
  MSB_data = PINK;
  //Set SEL high to get low byte
  PORTH |= (1 << SEL);
  // Wait 20 microseconds
  _delay_ms(20);
  // Read LSB
  LSB_data = PINK;
  // Set OE high to disable output of encoder
  PORTH |= (1 << OE);
}
