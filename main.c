// MAIN PROGRAM

#include <stdio.h>
#include <avr/io.h>

#define F_CPU 4915200UL
#include <util/delay.h>

#define FOSC F_CPU// Clock Speed
#define BAUD 9600
#define MYBURR FOSC/16/BAUD-1

#include "uart_interface.h"
#include "led.h"

int main()
{
    USART_Init (MYBURR);
    led_init();
    fdevopen(*USART_Transmit, *USART_Receive);
    double T = 1000.00;  
    while(1)
    {
        //USART_Transmit('z');
        //_delay_ms(T);
	//printf("Test it");
        USART_Transmit(USART_Receive());
        printf("Test it");
    }
}
