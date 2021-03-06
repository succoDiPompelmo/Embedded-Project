#include <stdio.h>
#include <util/delay.h>
#include "sram_interface.h"

void SRAM_test(void)
{
    double T = 500.00;
    volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
    uint16_t ext_ram_size = 0x800;
    uint16_t write_errors = 0;
    uint16_t retrieval_errors = 0;
    printf("Starting SRAM test...\n\r");
    // rand() stores some internal state, so calling this function in a loop will
    // yield different seeds each time (unless srand() is called before this function)
    uint16_t seed = rand();
    // Write phase: Immediately check that the correct value was stored
    srand(seed);
    for (uint16_t i = 0; i < ext_ram_size; i++) {
        uint8_t some_value = rand();
        ext_ram[i] = some_value;
        //_delay_ms(T);
        uint8_t retreived_value = ext_ram[i];
        if (retreived_value != some_value) {
            printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n\r", i, retreived_value, some_value);
            write_errors++;
        }
      }
    printf("SRAM test completed with %4d errors in write phase and %4d errors in retrieval phase\n\n\r", write_errors, retrieval_errors);
    if (write_errors == 0) {
        led_turn_on();
        _delay_ms(T);
        led_turn_off();
    }
}

void test()
{
  double T = 500.00;
  volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM

  uint8_t some_value = rand();
  uint8_t adress = rand();
  ext_ram[adress] = some_value;
  _delay_ms(100.0);

  uint8_t retreived_value = ext_ram[adress];
  printf("%02x\n\r", retreived_value);
  if (retreived_value != some_value) {
      printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n\r", 0, retreived_value, some_value);
  }

}
