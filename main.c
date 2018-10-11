// MAIN PROGRAM

#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <stdbool.h>

#define F_CPU 4915200UL
#include <util/delay.h>

#define FOSC F_CPU // Clock Speed
#define BAUD 9600
#define MYBURR FOSC/16/BAUD-1

#include "uart_interface.h"
#include "led.h"
#include "joystick.h"
#include "sram_interface.h"
#include "oled_interface.h"
#include "SPI.h"
#include "can.h"
#include "MCP2515.h"
//#include "fonts.h"

//volatile char *oled_data = (char *) 0x1200;

int main()
{
    USART_Init (MYBURR);
    fdevopen(*USART_Transmit, *USART_Receive);
    //led_init();

    //Enable the external memory interface/4 bits address
    MCUCR |= (1 << SRE);
    SFIOR |= (1 << XMM2);

    DDRD &= ~(1 << PD2);

    void* menu = Menu_Init();

    OLED_Init();
    Menu_Print(menu);



    /* Set MOSI and SCK output, all others input */
    //DDRB = (1<<PB5)|(1<<PB7);
    /* Enable SPI, Master, set clock rate fck/16 */
    //SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
    SPI_MasterInit();
    mcp2515_init();


    double T = 10.00;

    while(1)
    {

        _delay_ms(T);
        print_selection(menu);

        //printf(" CIAO-1 ");

        //JOYSTICK_Output();
        change_selection(menu);

        //SPI_MasterTransmit(0b01010101);

        //mcp2515_init();

        //button_pressed(menu);
        //oled_clear();
        printf(" CIAO-1 %x \n\r",mcp2515_read(MCP_CANSTAT));
        //mcp2515_read(0b00001110);
        //_delay_ms(T);
        //oled_data[0] = 0xFF;
        //OLED_cmd = 0x41;
        //_delay_ms(T);

        //_delay_ms(T);
        //JOYSTICK_Output();



        //OLED_cmd[0] = 0xF0;
        //uint8_t value;
        //value = JOYCON_X_Axis();
        //printf("%02X\n\r", value);
        //SRAM_test();
        //SRAM_test();
        //volatile char *ext_ram = (char *) 0x1800;
        //ext_ram[0] = "c";
        //_delay_ms(T);
        //volatile char *ext_oled = (char *) 0x1000;
        //ext_oled[0] = "d";
        //_delay_ms(T);
        //volatile char *ext_adc = (char *) 0x1400;
        //ext_adc[0] = "f";
        //_delay_ms(T);

        // SRAM_test();
    }
}
