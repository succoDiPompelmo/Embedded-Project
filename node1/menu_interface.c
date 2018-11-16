// The main purpouse of this interface is to
// allow the user to manage the menu and leave all the settings
// to the oled_interface. The main point to remember here is that
// is responsibility of the caller to set the starting position with the
// corresponding function for coloumn and page and this must be done everytime
// you want to print something.
#include <avr/pgmspace.h>
#include <stdbool.h>
#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>

#define F_CPU 4915200UL
#include <util/delay.h>

#include "oled_interface.h"
#include "joystick.h"
#include "communication_controller.h"

// Treshold for the menu selection
bool treshold_UP = true;
bool treshold_DOWN = true;

// Menu item struct
typedef struct menu_item {
    int selection;
    int menu_position;
    char* title;
    int length;
} menu_item;

// Menu hierarchy struct
typedef struct node {
    menu_item* menu;
    struct node * next;
    // Submenu
    struct node * down;
} node;

// Sub-menu
int level = 0;

// Global variable to save the menu
void* MENU;

#include "menu_interface.h"

// Initialise a node
void initialiseNode(node* new_node, char* title, int selection, int length, int menu_position)
{
  // Allocate the memory for one menu item
  new_node->menu = (struct menu_item*)malloc(sizeof(struct menu_item));
  // Initilise the selection variable inside a menu item
  new_node->menu->selection = 1;
  // Initialise the position variable inside a menu item
  new_node->menu->menu_position = 0;
  // Initialise the lenght variable, that stores the length of the title
  new_node->menu->length = 4;
  // Allocate an array of char for the title
  new_node->menu->title = malloc(strlen(title)+1);
  // Copy the title parameter into the title variable into a menu item
  strcpy(new_node->menu->title, title);
  // Set the next node to NULL
  new_node->next = NULL;
}

// Add a node to the linked list 
void push(node * head, char* title, int selection, int length, int menu_position)
{
    // Take the head of the list
    node * current = head;
    // Loop until we reach the end of the list
    while (current->next != NULL) {
        current = current->next;
    }
    // Allocate the memory for one node
    current->next = (struct node*)malloc(sizeof(struct node));
    // Allocate the memory for one menu item
    current->next->menu = (struct menu_item*)malloc(sizeof(struct menu_item));
    // Initilise the selection variable inside a menu item
    current->next->menu->selection = selection;
    // Initialise the position variable inside a menu item
    current->next->menu->menu_position = menu_position;
    // Initialise the lenght variable, that stores the length of the title
    current->next->menu->length = length;
    // Allocate an array of char for the title
    current->next->menu->title = malloc(strlen(title)+1);
    // Copy the title parameter into the title variable into a menu item
    strcpy(current->next->menu->title, title);
    // Set the next node to NULL
    current->next->next = NULL;
}

// Initialise the menu
node* Menu_Init(){
    // Title of the first element of our menu
    char name[] = {'M','E','N','U'};
    // First node of the linked list
    struct node* new_node = (struct node*)malloc(sizeof(struct node));
    //initialiseNode(new_node, "MENU", selection, length, menu_position);

    // Allocate memory for the first menu item
    new_node->menu = (struct menu_item*)malloc(sizeof(struct menu_item));
    // Initilise the selection variable inside a menu item, saying if the current item is selected or not by the user
    new_node->menu->selection = 1;
    // Initialise the position variable inside a menu item
    new_node->menu->menu_position = 0;
    // Initialise the lenght variable, that stores the length of the title
    new_node->menu->length = 4;
    // Allocate an array of char for the title
    new_node->menu->title = malloc(strlen(name)+1);
    // Copy the title parameter into the title variable into a menu item
    strcpy(new_node->menu->title, name);
    // Set the next node to NULL
    new_node->next = NULL;

    // COnstruct the linked list, the menu
    push(new_node, "PLAY", 0, 4, 1);
    push(new_node, "STOP", 0, 4, 1);
    push(new_node, "DIFF UP", 0, 7, 1);
    push(new_node, "DIFF DOWN", 0, 9, 1);

    // Set a global pointer, storing the head of the linked list
    MENU = new_node;
    // Return the head of the list
    return new_node;
}

// Print the menu
void Menu_Print(void* menu)
{
    // Set a copy of the head of the linked list
    node* new_node = (node* )MENU;
    node * current = new_node;
    // Clear the oled
    oled_clear();
    // Set the coloumn initial and final adress
    OLED_setup_column_adress(0x0B, 0x7F);
    // Initialise a counter
    char* i = 0;
    // Loop over the linked list until you reach the end
    while (current != NULL) {
        // Set up the page final and initial adress, ROW
        OLED_setup_page_adress(i, 0x07);
        // Print on the oled the title of the current menu item
        oled_print(current->menu->title, current->menu->length);
        // Move to the next item
        current = current->next;
        // Restart the adress for the coloumn
        OLED_setup_column_adress(0x0B, 0x7F);
        // Increase the counter
        i = i + 1;
    }
    // Reset the page and coloumn adress to the standard one
    oled_reset();
}

// Print the slection symbol in the current position
void print_selection(void* menu)
{
    // Set a copy of the head of the linked list
    node* new_node = (node* )menu;
    node * current = new_node;
    // Set the coloumn initial and final adress
    OLED_setup_column_adress(0x00, 0x7F);
    // Initialise a counter
    char* i = 0;
    // Loop over the linked list until you reach the end
    while (current != NULL) {
        // Set up the page final and initial adress, ROW
        OLED_setup_page_adress(i, 0x07);
        // If the current menu item is selected
        if (current->menu->selection == 1) {
            // Print the symbol to mark a selection
            oled_print("*", 1);
        }
        // Move to the next node
        current = current->next;
        // Restart the adress for the coloumn
        OLED_setup_column_adress(0x00, 0x7F);
        // Increase the counter
        i = i + 1;
    }
    // Reset the page and coloumn adress to the standard one
    oled_reset();
}

// Find the index of the currently selected item
int where_is_one(void* menu)
{
    // Set a copy of the head of the linked list
    node* new_node = (node* )menu;
    node * current = new_node;
    // Variable to store the index of the selected item
    int i = 0;
    // Loop over the linked list until you reach the end
    while (current != NULL) {
        // If the current menu item is selected
        if (current->menu->selection == 1) {
            // Return its index
            return i;
        }
        // Move to the next element
        current = current->next;
        // Increase the counter
        i = i + 1;
    }
    // Return the index of the selected element
    return i;
}

// Compute the current length of the menu
int menu_length(void *menu)
{
    // Set a copy of the head of the linked list
    node* new_node = (node* )menu;
    node * current = new_node;
    // Variable to store the index of the selected item
    int i = 0;
    // Loop over the linked list until you reach the end
    while (current != NULL) {
        // Move to the next element
        current = current->next;
        // Increase the counter
        i = i + 1;
    }
    // Return the index of the selected element
    return i;
}

void clear_selection()
{
    // Set the coloumn and page initial and end adress for the selection portion of the oled screen
    OLED_setup_column_adress(0x00, 0x0A);
    OLED_setup_page_adress(0x00, 0x7F);
    // Clear this portion by writing zero to it
    for (size_t i = 0; i < 300; i++) {
        oledSendData(0x00);
    }
}

// Set to one the menu item currently selected
void set_selection(void* menu, int select)
{
    // Set a copy of the head of the linked list
    node* new_node = (node* )menu;
    node * current = new_node;
    // Variable to store the index of the selected item
    int i = 0;
    // Clear the selection part of the oled
    clear_selection();
    // Loop over the linked list until you reach the end
    while (current != NULL) {
        // If you are on the selected item
        if(i == select)
        {
            // Set its selection to 1
            current->menu->selection = 1;
        } else {
           // Otherwise set its selection to 0
            current->menu->selection = 0;
        }
        // Move to the next element
        current = current->next;
        // Increase the counter
        i = i + 1;
    }
}

// Clear the menu portion of the screen
void clear_menu()
{
    // Set the intial coloumn and page adress for the menu portion of the oled display
    OLED_setup_column_adress(0x0B, 0x0B);
    OLED_setup_page_adress(0x00, 0x7F);
    // Clear it by writing to the oled memory zero
    for (size_t i = 0; i < 3000; i++) {
        oledSendData(0x00);
    }
}

// What happen if a press the joystic button
int button_pressed(void* menu)
{
  int i = -1;
  // Check if button is pressed
  if(!(PIND & (1 << PD2)))
  {
      // Find where is the selection
      i = where_is_one(menu);
      // Clear the oled
      oled_clear();
      // Reset the page and coloumn adress
      oled_reset();
      // Cast a char number into a string
      char str[2] = "\0";
      char c = i + '0';
      str[0] = c;
      printf("%c\n\r", c);
      // Print the number on the oled
      oled_print(str, 1);
      // Delay
      _delay_ms(1);
      // Print the menu
      Menu_Print(menu);
  }
  // Base on the selector position send a particular message
  if (i == 1) start_message();
  if (i == 2) stop_message();
  if (i == 3) change_difficulty(0);
  if (i == 4) change_difficulty(1);
  return i;
}

// If we move the joystick up or down we change the selection
void change_selection(void* menu)
{
    // Check if the joystick is up
    if(check_Y_Axis_UP() == 1 && treshold_UP)
    {
        // Index of menu item currently selected
        int i = where_is_one(menu);
        // Manage the boundaries of the linked list
        if(i - 1 >= 0)
        {
            // Save the index of the new selected item
            i = i - 1;
        }
        else
        {
            // Save the index of the new selected item
            i = menu_length(menu) - 1;
        }
        // Change the selection
        set_selection(menu, i);
        // Set treshold up to false so we don't move the slection more than once each time we move up the joystick
        treshold_UP = false;
    }
    // Check if the joystick is not up
    if (check_Y_Axis_UP() == -1 && !treshold_UP) {
        // Reset the threshold to let the user move the selection again
        treshold_UP = true;
    }

   if(check_Y_Axis_DOWN() == 1 && treshold_DOWN)
   {
      // Index of menu item currently selected
      int i = where_is_one(menu);
      // Manage the boundaries of the linked list
      if(i - 1 >= 0)
      {
          // Save the index of the new selected item
          i = i - 1;
      }
      else
      {
          // Save the index of the new selected item
          i = menu_length(menu) - 1;
      }
      // Change the selection
      set_selection(menu, i);
      // Set treshold down to false so we don't move the selection more than once each time we move down the joystick
      treshold_DOWN = false;
   }
   // Check if the joystick is not down
   if (check_Y_Axis_DOWN() == -1 && !treshold_DOWN) {
      // Reset the threshold to let the user move the selection again
      treshold_DOWN = true;
   }
}
