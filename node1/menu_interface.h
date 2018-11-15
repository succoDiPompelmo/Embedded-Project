void initialiseNode(node* new_node, char* title, int selection, int length, int menu_position);

void push(node * head, char* title, int selection, int length, int menu_position);

node* Menu_Init();

void Menu_Print(void* menu);

void print_selection(void* menu);

int where_is_one(void* menu);

int menu_length(void *menu);

void clear_selection();

void clear_menu();
