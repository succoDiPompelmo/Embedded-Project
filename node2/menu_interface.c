// The main purpouse of this interface is to
// allow the user to manage the menu and leave all the settings
// to the oled_interface. The main point to remember here is that
// is responsibility of the caller to set the starting position with the
// corresponding function for coloumn and page and this must be done everytime
// you want to print something.

bool treshold_UP   = true;
bool treshold_DOWN = true;
int  Menu_level    = 0;

typedef struct menu_item {
    int selection;
    int menu_position;
    char* title;
    int length;
} menu_item;

typedef struct node {
    menu_item* menu;
    struct node * next;
    // Submenu
    struct node * down;
} node;

int level = 0;

void initialiseNode(node* new_node, char* title, int selection, int length, int menu_position)
{
  new_node->menu = (struct menu_item*)malloc(sizeof(struct menu_item));

  new_node->menu->selection = 1;
  new_node->menu->menu_position = 0;
  new_node->menu->length = 4;

  new_node->menu->title = malloc(strlen(name)+1);
  strcpy(new_node->menu->title, name);

  new_node->next = NULL;
}

void push(node * head, char* title, int selection, int length, int menu_position)
{
    node * current = head;

    while (current->next != NULL) {
        current = current->next;
    }

    current->next = (struct node*)malloc(sizeof(struct node));

    initialiseNode(current->next, title, selection, length, menu_position);

    current->next->menu = (struct menu_item*)malloc(sizeof(struct menu_item));

    current->next->menu->selection = selection;
    current->next->menu->menu_position = menu_position;
    current->next->menu->length = length;

    current->next->menu->title = malloc(strlen(title)+1);
    strcpy(current->next->menu->title, title);

    current->next->next = NULL;
}

node* Menu_Init(){
    char name[] = {'M','E','N','U'};

    struct node* new_node = (struct node*)malloc(sizeof(struct node));

    initialiseNode(new_node, "MENU", selection, length, menu_position);

    new_node->menu = (struct menu_item*)malloc(sizeof(struct menu_item));

    new_node->menu->selection = 1;
    new_node->menu->menu_position = 0;
    new_node->menu->length = 4;

    new_node->menu->title = malloc(strlen(name)+1);
    strcpy(new_node->menu->title, name);

    new_node->next = NULL;

    push(new_node, "CREDITS", 0, 7, 1);
    push(new_node, "EXTRAS", 0, 6, 1);

    return new_node;
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
        oledSendData(0x00)
    }
}

void clear_menu()
{
    OLED_setup_column_adress(0x0B, 0x0B);
    OLED_setup_page_adress(0x00, 0x7F);
    for (size_t i = 0; i < 3000; i++) {
        oledSendData(0x00);
    }
}

void button_pressed(void* menu)
{
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
      //Menu_Print(menu);
  }
}

void change_selection(void* menu)
{
    if(check_Y_Axis_UP() == 1 && treshold_UP)
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
        treshold_UP = false;
    }
    _delay_ms(100);
    if (check_Y_Axis_DOWN() == -1 && !treshold_UP) {
        treshold_UP = true;
    }

  if(check_Y_Axis_DOWN() == 1 && treshold_DOWN)
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
      treshold_DOWN = false;
  }
  _delay_ms(100);
  if (check_Y_Axis_DOWN() == -1 && !treshold_DOWN) {
      treshold_DOWN = true;
  }
}

node* level_selection(void* menu_top)
{
  node* current = (node*) menu_top;
  if (Menu_level == 0) return menu_top;
  else {
    int level = Menu_level;
    node* current = (node*) menu_top;

    while (level > 0)
    {
      // A for loop in this case is better
      int i = where_is_one(menu_top);

      while (i != 0)
      {
        current = current->next;
        i--;
      }

      current = current->down;
    }
    return current;
  }
}
