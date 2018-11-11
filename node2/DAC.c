#include <avr/io.h>
#include <util/delay.h>
#include "DAC.h"

#define SLAVE_ADRESS_WRITE 0x50;

void DAC_init()
{
  // Set SDA ans SCL pin as OUTPUT
  DDRD |= (1 << PD0);
  DDRD |= (1 << PD1);
  // Initialise the I2C protocol
  TWI_Master_Initialise();
}

void DAC_write(uint8_t value)
{
  // Variable to store the message to send
  uint8_t i2c_write[3];
  // Variable to store the message size
  unsigned char msgSize = 3;

  // Store the adress in the first position of the array
  i2c_write[0] = SLAVE_ADRESS_WRITE;
  // Store dummy data in the second position of the array
  i2c_write[1] = 0x00;
  // Store the data we want to send to the DAC in the thrid position of the array
  i2c_write[2] = (uint8_t) value;

  // Transmit to the DAC the message defined above 
  TWI_Start_Transceiver_With_Data(i2c_write, msgSize);
}
