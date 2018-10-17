/**
 * @file
 * Sample uart driver for sending adn receving data from
 * to adn ffrom the serial interface
 */

/**
 * @brief Initialize UART driver; necessary to call only once
 */
void SRAM_test(void);

uint8_t SRAM_read(int offset);

void SRAM_write(int offset, uint8_t value);
