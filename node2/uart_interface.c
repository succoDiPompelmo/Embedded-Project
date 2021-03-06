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
  // STX, Start of Text
  USART_Transmit(0x02);
  // LEN[0], most significant byte of a 16 bit unsigned integer
  USART_Transmit(0x00);
  // LEN[1], least significant byte of the same 16 bit unsigned integer
  USART_Transmit(0x05);
  // Payload
  USART_Transmit(0x10);
  USART_Transmit(0x00);
  USART_Transmit(0x00);
  USART_Transmit(0x02);
  USART_Transmit(0x33);
  // ETX, End of Text
  USART_Transmit(0x03);
}

void score_message(uint8_t value)
{
  // STX, Start of Text
  USART_Transmit(0x02);
  // LEN[0], most significant byte of a 16 bit unsigned integer
  USART_Transmit(0x00);
  // LEN[1], least significant byte of the same 16 bit unsigned integer
  USART_Transmit(0x05);
  // Payload
  USART_Transmit(0x10);
  USART_Transmit(0x00);
  USART_Transmit(0x00);
  USART_Transmit(0x00);
  USART_Transmit(value);
  // ETX, End of Text
  USART_Transmit(0x03);
}

void position_message(int16_t value)
{
  uint8_t low;
  uint8_t high;
  // Prepare the numerical value
  // First scale on a range of 1000
  value = value / 8;
  // Spli the 16 bit value into two 8 bit value
  low = value & 0xFF;
  high = value >> 8;
  // STX, Start of Text
  USART_Transmit(0x02);
  // LEN[0], most significant byte of a 16 bit unsigned integer
  USART_Transmit(0x00);
  // LEN[1], least significant byte of the same 16 bit unsigned integer
  USART_Transmit(0x03);
  // Payload
  USART_Transmit(0x12);
  USART_Transmit(high);
  USART_Transmit(low);
  // ETX, End of Text
  USART_Transmit(0x03);
}

unsigned char USART_Receive( void )
{
    // Wait for data to be received
    while( !(UCSR0A & (1 << RXC0)) );

    // Get and return received data from buffer
    return UDR0;
}

void check_incoming_message()
{
  // Check incoming trasmission
  if (!(UCSR0A & (1 << RXC0))) return;
  // If there are incoming trasmission as test set the score
  score_message(0x22);
}

void USART_Flush( void )
{
        unsigned char dummy;
        while (UCSR0A & (1<<RXC0) ) dummy = UDR0;
}
