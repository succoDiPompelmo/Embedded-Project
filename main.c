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
  //Enable the external memory interface/4 bits address
	MCUCR |= (1<<SRE);
	SFIOR |= (1<<XMM2);

  //Address for the OLED commands
  volatile char *OLED_cmd = (char *) 0x1000;

	//Setup the OLED display

	//display off
	*OLED_cmd = 0xAE;
	//segment remap
	*OLED_cmd = 0xA1;

	//common pads hardware: alternative
	*OLED_cmd = 0xDA;
	*OLED_cmd = 0x12;

	//common output scan direction:com63~com0
	*OLED_cmd = 0xC8;

	//multiplex ration mode:63
	*OLED_cmd = 0xA8;
	*OLED_cmd = 0x3F;

	//display divide ratio/osc. freq. mode
	*OLED_cmd = 0xD5;
	*OLED_cmd = 0x80;

	//contrast control
	*OLED_cmd = 0x81;
	*OLED_cmd = 0x50;

	//set pre-charge period
	*OLED_cmd = 0xD9;
	*OLED_cmd = 0x21;

	//set Memory Addressing Mode
	*OLED_cmd = 0x20;
	*OLED_cmd = 0x00;

	//VCOM deselect level mode
	*OLED_cmd = 0xDB;
	*OLED_cmd = 0x30;

	//master configuration
	*OLED_cmd = 0xAD;
	*OLED_cmd = 0x00;

	//out follows RAM content
	*OLED_cmd = 0xA4;

	//set normal display
	*OLED_cmd = 0xA6;
	//display on
	*OLED_cmd = 0xAF;

  //Set page start address
	//*OLED_cmd = 0xB0;
	//Set lower column start address
	//*OLED_cmd = 0x00;
	//Set higher column start address
	//*OLED_cmd = 0x10;
}

int main()
{
    USART_Init (MYBURR);
    led_init();
    MCUCR |= (1 << SRE);
    SFIOR |= (1 << XMM2);
    fdevopen(*USART_Transmit, *USART_Receive);
    double T = 1000.00;

    while(1)
    {
        //volatile char *oled = (char *) 0x1200;
        //oled[0] = "f";


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
