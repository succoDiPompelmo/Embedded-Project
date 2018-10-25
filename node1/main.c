// MAIN PROGRAM

#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <stdbool.h>
#include <avr/interrupt.h>


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
#include "CAN_interface.h"

volatile char *OLED_DATA = (char *) 0x1200;

int main()
{
    _delay_ms(100);
    USART_Init (MYBURR);
    fdevopen(*USART_Transmit, *USART_Receive);
    //led_init();

    //Enable the external memory interface/4 bits address
    MCUCR |= (1 << SRE);
    SFIOR |= (1 << XMM2);

    DDRD &= ~(1 << PD2);
    cli();
    MCUCR |= (1<<ISC11);
    GICR |= (1 << INT1);



    //void* menu = Menu_Init();

    OLED_Init();
    //Menu_Print(menu);
    CAN_Init();

    double T = 10.00;

    sei();

    while(1)
    {

        uint8_t value;

        _delay_ms(100.0);
        //print_selection(menu);

        CAN_Trasmission();

        *OLED_DATA = 0xFF;

        //value = CAN_Receive();
        //printf("%02x\n\r ", value);

        setData(JOYCON_X_Axis());

        //JOYSTICK_Output();
        //change_selection(menu);

        //test();

        //SRAM_test();

        //SPI_MasterTransmit(0b01010101);

        //mcp2515_init();

        //button_pressed(menu);
        //oled_clear();

        //_delay_ms(T);
        //JOYSTICK_Output();
    }
}
