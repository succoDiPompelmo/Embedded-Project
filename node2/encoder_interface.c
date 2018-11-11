#include <avr/interrupt.h>
#include <avr/io.h>

#define F_CPU 16000000UL
#include <util/delay.h>

#include "encoder_interface.h"
#include "DAC.h"

#define OE  PH5
#define SEL PH3
#define RST PH6

// Variable to store the 16 bit value returned from the ENCODER
volatile uint8_t MSB_data;
volatile uint8_t LSB_data;

void Encoder_Init()
{
  // Set OE, SEL and RST pin as OUTPUT
  DDRH |= (1 << OE) | (1 << SEL) | (1 << RST);
  // We want all the PIN K to be input
  // On PORTK we will read the value returned from the encoder
  DDRK = 0;

  // Toggle the reset PIN to Reset the ENCODER
  PORTH &= ~(1 << RST);
  _delay_ms(20);
  PORTH |= (1 << RST);

}

// Get the most significant bit returned from the encoder
uint8_t get_MSB()
{
  return MSB_data;
}
// Get the least significant bit returned fomr the encoder
uint8_t get_LSB()
{
  return LSB_data;
}

// Read the value returned from the encoder 
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
