// MAIN PROGRAM

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
    double T = 500.00;
    while(1)
    {
        USTART_Trasmit('z');
        _delay_ms(T);
    }
}
