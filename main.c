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
//#include "fonts.h

volatile char *oled_data = (char *) 0x1200;
bool treshold = true;

void change_selection(void* menu)
{
    printf("%d\n", check_Y_Axis());
    if(check_Y_Axis() == 1 && treshold)
    {
        int i = where_is_one(menu);
        if(i - 1 >= 0)
        {
            i = i - 1;
        }
        else
        {
            i = menu_length(menu) - 1;
        }
        set_selection(menu, i);
        treshold = false;
    }
    _delay_ms(100);
    if (check_Y_Axis() == -1 && !treshold) {
        treshold = true;
    }
}

int main()
{
    USART_Init (MYBURR);
    //led_init();

    //Enable the external memory interface/4 bits address
    MCUCR |= (1 << SRE);
    SFIOR |= (1 << XMM2);

    DDRD &= ~(1 << PD2);

    OLED_Init();

    void* menu = Menu_Init();

    Menu_Print(menu);

    fdevopen(*USART_Transmit, *USART_Receive);
    double T = 100.00;

    while(1)
    {
        _delay_ms(T);

        print_selection(menu);

        //JOYSTICK_Output();
        change_selection(menu);

        // Check if button is pressed
        if(!(PIND & (1 << PD2)))
        {
            int i = where_is_one(menu);
            oled_clear();
            oled_reset();
            char str[2] = "\0";
            char c = i + '0';
            str[0] = c;
            printf("%c\n", c);
            oled_print(str, 1);
            _delay_ms(1000.0);
            Menu_Print(menu);
        }
        //Menu_Print(menu);
        //oled_clear();
        //printf("CIAO");
        //_delay_ms(T);
        //oled_data[0] = 0xFF;
        //*OLED_cmd = 0x41;
        //_delay_ms(T);

        //_delay_ms(T);
        //JOYSTICK_Output();

        //OLED_Init();

        //OLED_cmd[0] = 0xF0;
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
