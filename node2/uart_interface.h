/**
 * @file
 * Sample uart driver for sending adn receving data from
 * to adn ffrom the serial interface
 */

/**
 * @brief Initialize UART driver; necessary to call only once
 */
void USART_Init( unsigned int ubrr );

/**
 * @brief Transmit a single character through the serial interface
 */
void USART_Transmit(char data);

/**
 * @brief Receive a character from the serial interface
 */
unsigned char USART_Receive( void );

/**
 * @brief
 */
void USART_Flush( void );

void web_message();

void score_message(uint8_t value);

void position_message(int16_t value);

void check_incoming_message();
