#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>

#include "fonts.h"

#include "oled_interface.h"

volatile char *OLED_cmd = (char *) 0x1000;
volatile char *OLED_data = (char *) 0x1200;

typedef struct menu_item {
    int selection;
    int menu_position;
    char* title;
    int length;
} menu_item;

typedef struct node {
    menu_item* menu;
    struct node * next;
} node;

void push(node * head, char* title, int selection, int length, int menu_position)
{
    node * current = head;

    while (current->next != NULL) {
        current = current->next;
    }

    current->next = (struct node*)malloc(sizeof(struct node));
    current->next->menu = (struct menu_item*)malloc(sizeof(struct menu_item));

    current->next->menu->selection = selection;
    current->next->menu->menu_position = menu_position;
    current->next->menu->length = length;

    current->next->menu->title = malloc(strlen(title)+1);
    strcpy(current->next->menu->title,title);

    current->next->next = NULL;
}

node* Menu_Init(){
    char name[] = {'M','E','N','U'};

    struct node* new_node = (struct node*)malloc(sizeof(struct node));
    new_node->menu = (struct menu_item*)malloc(sizeof(struct menu_item));

    new_node->menu->selection = 1;
    new_node->menu->menu_position = 0;
    new_node->menu->length = 4;

    new_node->menu->title = malloc(strlen(name)+1);
    strcpy(new_node->menu->title,name);

    new_node->next = NULL;

    push(new_node, "CREDITS", 0, 7, 1);
    push(new_node, "EXTRAS", 0, 6, 1);

    return new_node;
}

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

void OLED_setup_column_adress(char* start_adress, char* end_adress)
{
    //Setup colum start and end adress
	*OLED_cmd = 0x21;
	//Column start adress
	*OLED_cmd = start_adress;
	//Column end adress
	*OLED_cmd = end_adress;
}

void OLED_setup_page_adress(char* start_adress, char* end_adress)
{
    //Setup colum start and end adress
	*OLED_cmd = 0x22;
	//Column start adress
	*OLED_cmd = start_adress;
	//Column end adress
	*OLED_cmd = end_adress;
}

void oled_print(char* string, int length)
{
    for (size_t i = 0; i < length; i++) {
        int character = string[i] - 32;
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

void oled_clear()
{
    OLED_setup_column_adress(0x00, 0x7F);
    OLED_setup_page_adress(0x00, 0x07);
    for (size_t i = 0; i < 8; i++) {
        for (size_t i = 0; i < 128; i++) {
            *OLED_data = 0x00;
        }
    }
}

void oled_reset()
{
    OLED_setup_column_adress(0x00, 0x7F);
    OLED_setup_page_adress(0x00, 0x07);
}

void Menu_Print(void* menu)
{
    node* new_node = (node* )menu;
    node * current = new_node;
    oled_clear();
    OLED_setup_column_adress(0x0B, 0x7F);
    char* i = 0;
    while (current != NULL) {
        OLED_setup_page_adress(i, 0x07);
        oled_print(current->menu->title, current->menu->length);
        current = current->next;
        OLED_setup_column_adress(0x0B, 0x7F);
        i = i + 1;
    }
    oled_reset();
}

void print_selection(void* menu)
{
    node* new_node = (node* )menu;
    node * current = new_node;
    OLED_setup_column_adress(0x00, 0x7F);
    char* i = 0;
    while (current != NULL) {
        OLED_setup_page_adress(i, 0x07);
        if (current->menu->selection == 1) {
            oled_print("*", 1);
        }
        current = current->next;
        OLED_setup_column_adress(0x00, 0x7F);
        i = i + 1;
    }
    oled_reset();
}

int where_is_one(void* menu)
{
    node* new_node = (node* )menu;
    node * current = new_node;
    int i = 0;
    while (current != NULL) {
        if (current->menu->selection == 1) {
            return i;
        }
        current = current->next;
        i = i + 1;
    }
    return i;
}

int menu_length(void *menu)
{
    node* new_node = (node* )menu;
    node * current = new_node;
    int i = 0;
    while (current != NULL) {
        current = current->next;
        i = i + 1;
    }
    return i;
}

void set_selection(void* menu, int select)
{
    node* new_node = (node* )menu;
    node * current = new_node;
    int i = 0;
    clear_selection();
    while (current != NULL) {
        if(i == select)
        {
            current->menu->selection = 1;
        } else {
            current->menu->selection = 0;
        }
        current = current->next;
        i = i + 1;
    }
}

void clear_selection()
{
    OLED_setup_column_adress(0x00, 0x0A);
    OLED_setup_page_adress(0x00, 0x7F);
    for (size_t i = 0; i < 300; i++) {
        *OLED_data = 0x00;
    }
}

void clear_menu()
{
    OLED_setup_column_adress(0x0B, 0x0B);
    OLED_setup_page_adress(0x00, 0x7F);
    for (size_t i = 0; i < 3000; i++) {
        *OLED_data = 0x00;
    }
}

void button_pressed(void* menu)
{
    int i = where_is_one(menu);
    oled_clear();
    oled_reset();
    char c = i + '0';
    oled_print(c, 1);
}
