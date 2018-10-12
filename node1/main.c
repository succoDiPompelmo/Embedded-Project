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

    GICR |= (1 << INT1);

    sei();

    void* menu = Menu_Init();

    OLED_Init();
    Menu_Print(menu);
    CAN_Init();

    double T = 10.00;

    while(1)
    {

        uint8_t value;

        _delay_ms(T);
        print_selection(menu);

        //printf(" CIAO-1 \n\r");
        CAN_Trasmission();

        _delay_ms(1000.0);

        value = CAN_Receive();

        printf("%02x\n\r", value);

        setData(0x97);

        //JOYSTICK_Output();
        change_selection(menu);

        //SPI_MasterTransmit(0b01010101);

        //mcp2515_init();

        button_pressed(menu);
        //oled_clear();

        //printf(" CIAO-1 %x \n\r",mcp2515_read(MCP_CANSTAT));


        //_delay_ms(T);
        //JOYSTICK_Output();
    }
}
