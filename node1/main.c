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
#include "joystick.h"
#include "sram_interface.h"
#include "CAN_interface.h"
#include "Timer_handler.h"

int main()
{
    USART_Init (MYBURR);
    fdevopen(*USART_Transmit, *USART_Receive);

    //Enable the external memory interface/4 bits address
    MCUCR |= (1 << SRE);
    SFIOR |= (1 << XMM2);


    // Game Board Joystic Button PIN
    DDRE &= ~(1 << PD2);

    cli();

    // INT2 will be used for the button sense
    // Enable INT1 and INT2
    MCUCR |= (1 << ISC11);
    //EMCUCR |= (1 << ISC2);
    GICR |= (1 << INT1);
    //GICR |= (1 << INT2);


    // Initialisation
    void* menu = Menu_Init();

    OLED_Init();

    oled_clear();

    // Stupid initial animation
    for (int i = 1; i < 1024; i++)
    {
      oled_full();
      _delay_ms(1);
    }
    for (int i = 1; i < 1024; i++)
    {
      oled_empty();
      _delay_ms(1);
    }

    Menu_Print(menu);

    CAN_Init();

    Timer_Init(menu);

    sei();

    while(1)
    {
        // Print the menu
        // Menu_Print(menu);
        print_selection(menu);

        // MOVE THIS INSIDE THE TIMER

        // Send Joycon Position to the second node
        setData(JOYCON_X_Axis());
        setIDH(0x30);
        //CAN_Trasmission();

        // This delay must be here to prevent the stall of the second node
        // otherwise we sent too many message to the second node and the interrupt
        // routine deadlock
        _delay_ms(40);

        // Send Slidebar Position to the second node
        setData(SLIDEBAR_Left());
        setIDH(0x20);
        //CAN_Trasmission();

        // This delay must be here to prevent the stall of the second node
        // otherwise we sent too many message to the second node and the interrupt
        // routine deadlock
        _delay_ms(40);

        // Change the menu selection
        change_selection(menu);

        // Check if the button is pressed
        button_pressed(menu);
    }
}
