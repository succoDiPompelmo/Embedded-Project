// MAIN PROGRAM

#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>

#define F_CPU 4915200UL
#include <util/delay.h>

#define FOSC F_CPU // Clock Speed
#define BAUD 9600
#define MYBURR FOSC/16/BAUD-1

#include "uart_interface.h"
#include "led.h"
#include "joystick.h"

void SRAM_test(void);

int main()
{
    USART_Init (MYBURR);
    led_init();
    MCUCR |= (1 << SRE);
    SFIOR |= (1 << XMM2);
    fdevopen(*USART_Transmit, *USART_Receive);
    double T = 200.00;

    while(1)
    {

        JOYSTICK_Output();
        /*SRAM_test();
        volatile char *ext_ram = (char *) 0x1800;
        ext_ram[0] = "c";
        _delay_ms(T);
        volatile char *ext_oled = (char *) 0x1000;
        ext_oled[0] = "d";
        _delay_ms(T);
        volatile char *ext_adc = (char *) 0x1400;
        ext_adc[0] = "f";
        _delay_ms(T);*/

        // SRAM_test();
    }
}

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
    // Retrieval phase: Check that no values were changed during or after the write phase
    srand(seed);    // reset the PRNG to the state it had before the write phase
    for (uint16_t i = 0; i < ext_ram_size; i++) {
        uint8_t some_value = rand();
        uint8_t retreived_value = ext_ram[i];
        if (retreived_value != some_value) {
            printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
            retrieval_errors++;
        }
}
    printf("SRAM test completed with %4d errors in write phase and %4d errors in retrieval phase\n\n\r", write_errors, retrieval_errors);
    if (write_errors == 0) {
        led_turn_on();
        _delay_ms(T);
        led_turn_off();
    }
}
