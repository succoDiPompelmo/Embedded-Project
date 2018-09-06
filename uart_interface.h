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
void USTART_Trasmit(unsigned char data);

/**
 * @brief Receive a character from the serial interface
 */
unsigned char USTART_Receive( void );

/**
 * @brief
 */
void USTART_Flush( void );

/**
 * @brief
 */
unsigned char USART_ReadUCSRC( void );
