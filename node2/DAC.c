#include <avr/io.h>
#include <util/delay.h>
//#include "DAC.h"

#define SLAVE_ADRESS_WRITE 0x50;

void DAC_init()
{
  // DDRB |= (1 << PB7);
  // DDRD |= (1 << PD1);
  // PORTD ^= (1 << PD0);
  //
  // while (1)
  // {
  //   PORTB ^= (1 << PB0);
  //   PORTD ^= (1 << PD1);
  //   _delay_ms(300);
  // }

  TWI_Master_Initialise();
  DDRD |= (1 << PD0);
  DDRD |= (1 << PD1);
}

void DAC_write(uint8_t value)
{
  unsigned char i2c_write[3];
  unsigned char msgSize = 3;

  i2c_write[0] = SLAVE_ADRESS_WRITE;
  i2c_write[1] = 0x00;
  i2c_write[2] = (uint8_t) value;

  _delay_ms(50);

  TWI_Start_Transceiver_With_Data(i2c_write, msgSize);
}
