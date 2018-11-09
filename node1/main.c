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
#include "Timer_handler.h"

volatile char *OLED_DATA = (char *) 0x1200;
volatile char *OLED_CMD = (char *) 0x1000;

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
    DDRD |= (1 << PD3);
    PORTD |= (1 << PD3);
    cli();
    MCUCR |= (1<<ISC11);
    GICR |= (1 << INT1);



    void* menu = Menu_Init();

    OLED_Init();
    Menu_Print(menu);
    CAN_Init();

    Timer_Init();

    sei();

    while(1)
    {

        uint8_t value;

        print_selection(menu);

        setData(JOYCON_X_Axis());
        setIDH(0x30);
        CAN_Trasmission();

        setData(SLIDEBAR_Left());
        setIDH(0x20);
        CAN_Trasmission();
        printf("%d\n", SLIDEBAR_Left());

        _delay_ms(100);

        change_selection(menu);

        button_pressed(menu);
        PORTD |= (1 << PD3);
    }
}
