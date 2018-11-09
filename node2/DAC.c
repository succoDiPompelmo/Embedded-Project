#include <avr/io.h>
#include <util/delay.h>
#include "DAC.h"

#define SLAVE_ADRESS_WRITE 0x50;

void DAC_init()
{
  DDRD |= (1 << PD0);
  DDRD |= (1 << PD1);

  TWI_Master_Initialise();
}

void DAC_write(uint8_t value)
{
  //if (value < 0) value = value * -1;

  uint8_t i2c_write[3];
  unsigned char msgSize = 3;

  i2c_write[0] = SLAVE_ADRESS_WRITE;
  i2c_write[1] = 0x00;
  i2c_write[2] = (uint8_t) value;

  TWI_Start_Transceiver_With_Data(i2c_write, msgSize);
}
