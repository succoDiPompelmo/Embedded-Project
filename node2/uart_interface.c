#include <avr/io.h>
#include "uart_interface.h"

void USART_Init( unsigned int ubrr )
{
    // Set baud rate
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)ubrr;
    //Enable receiver and trasmitter
    UCSR0B = (1<<RXEN0) | (1<<TXEN0);
    //Set frame format: 8data, 2stop bit
    UCSR0C = (1<<USBS0) | (3<<UCSZ00);
}

void USART_Transmit(char data)
{
    // Wait for empty trasmission buffer
    while( !(UCSR0A & (1 << UDRE0)) );

    // Put data into buffer, sends the data
    UDR0 = data;

    return 0;
}

void web_message()
{
  USART_Transmit(0x02);
  USART_Transmit(0x00);
}

unsigned char USART_Receive( void )
{
    // Wait for data to be received
    while( !(UCSR0A & (1 << RXC0)) );

    // Get and return received data from buffer
    return UDR0;
}

void USART_Flush( void )
{
        unsigned char dummy;
        while (UCSR0A & (1<<RXC0) ) dummy = UDR0;
}
