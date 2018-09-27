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
#include "sram_interface.h"

void OLED_Init()
{

}

int main()
{
    USART_Init (MYBURR);
    led_init();
   // MCUCR |= (1 << SRE);
   // SFIOR |= (1 << XMM2);
    fdevopen(*USART_Transmit, *USART_Receive);
    double T = 1000.00;

    while(1)
    {
        volatile char *oled = (char *) 0x1200;
        oled[0] = "f";
      

        //_delay_ms(T);
        //JOYSTICK_Output();
        //uint8_t value;
        //value = JOYCON_X_Axis();
        //printf("%02X\n\r", value);
        //SRAM_test();
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
