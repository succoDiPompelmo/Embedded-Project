void OLED_Init();

void oled_clear();

void oledSendData(char c);

void oled_reset();

void oled_print(char* string, int length);

void OLED_setup_column_adress(char* start_adress, char* end_adress);

void OLED_setup_page_adress(char* start_adress, char* end_adress);
