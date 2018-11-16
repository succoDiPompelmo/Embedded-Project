#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <avr/pgmspace.h>

#define F_CPU 4915200UL
#include <util/delay.h>

#include "fonts.h"

#include "joystick.h"
#include "oled_interface.h"

// Data and Command adress
volatile char *OLED_cmd = (char *) 0x1000;
volatile char *OLED_data = (char *) 0x1200;

void OLED_Init()
{
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

    //Setup colum start and end adress
	*OLED_cmd = 0x21;
	//Column start adress
	*OLED_cmd = 0x00;
	//Column end adress
	*OLED_cmd = 0x7F;

    //Set up page start and end adress
    *OLED_cmd = 0x22;
    //Page start adress
	*OLED_cmd = 0x00;
	//Page end adress
	*OLED_cmd = 0x07;
}

// Send a single char to the oled
void oledSendData(char c)
{
  *OLED_data = c;
}

// Send all 1 to the oled
void oled_full()
{
	*OLED_data = 0xFF;
}

// Send all 0 to the oled
void oled_empty()
{
	*OLED_data = 0x00;
}

// Print a char character on the oled
void oledPrintChar(char c)
{
	// We need to subtract 32 to align with the indexing used in the fonts file for the ASCII code
  int character = c - 32;
	// Read data from the PROGMEM memory and send it to the oled
  OLED_data[0] = pgm_read_byte(&(font8[character][0]));
  OLED_data[0] = pgm_read_byte(&(font8[character][1]));
  OLED_data[0] = pgm_read_byte(&(font8[character][2]));
  OLED_data[0] = pgm_read_byte(&(font8[character][3]));
  OLED_data[0] = pgm_read_byte(&(font8[character][4]));
  OLED_data[0] = pgm_read_byte(&(font8[character][5]));
  OLED_data[0] = pgm_read_byte(&(font8[character][6]));
  OLED_data[0] = pgm_read_byte(&(font8[character][7]));
}

// Set a personalised starting and ending coloumn adress
void OLED_setup_column_adress(char* start_adress, char* end_adress)
{
    //Setup colum start and end adress
	*OLED_cmd = 0x21;
	//Column start adress
	*OLED_cmd = start_adress;
	//Column end adress
	*OLED_cmd = end_adress;
}

// Set a personalised starting and ending page adress
void OLED_setup_page_adress(char* start_adress, char* end_adress)
{
    //Setup colum start and end adress
	*OLED_cmd = 0x22;
	//Column start adress
	*OLED_cmd = start_adress;
	//Column end adress
	*OLED_cmd = end_adress;
}

// Print a string on the oled
void oled_print(char* string, int length)
{
		// Cycle over the string length to print one character at the time
    for (size_t i = 0; i < length; i++) {
				// We need to subtract 32 to align with the indexing used in the fonts file for the ASCII code
        int character = string[i] - 32;
				// Read data from the PROGMEM memory and send it to the oled
        OLED_data[0] = pgm_read_byte(&(font8[character][0]));
        OLED_data[0] = pgm_read_byte(&(font8[character][1]));
        OLED_data[0] = pgm_read_byte(&(font8[character][2]));
        OLED_data[0] = pgm_read_byte(&(font8[character][3]));
        OLED_data[0] = pgm_read_byte(&(font8[character][4]));
        OLED_data[0] = pgm_read_byte(&(font8[character][5]));
        OLED_data[0] = pgm_read_byte(&(font8[character][6]));
        OLED_data[0] = pgm_read_byte(&(font8[character][7]));
    }
}

// Clear all the oled
void oled_clear()
{
		// Set the intial coloumn and page adress
    OLED_setup_column_adress(0x00, 0x7F);
    OLED_setup_page_adress(0x00, 0x07);
		// Write all 0 to clear the oled
    for (size_t i = 0; i < 8; i++) {
        for (size_t i = 0; i < 128; i++) {
            *OLED_data = 0x00;
        }
    }
}

// Reset coloumn and page adress
void oled_reset()
{
    OLED_setup_column_adress(0x00, 0x7F);
    OLED_setup_page_adress(0x00, 0x07);
}
