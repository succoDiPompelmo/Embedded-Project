// MAIN PROGRAM

#include <avr/io.h>

#define F_CPU 4915200UL
#include <util/delay.h>

#define FOSC 1843200// Clock Speed
#define BAUD 9600
#define MYBURR FOSC/16/BAUD-1

void USART_Init( unsigned int ubrr );
void USTART_Trasmit(unsigned char data);
unsigned char USTART_Receive( void );


int main()
{
    USART_Init (MYBURR);
    double T = 500.00;
    while(1)
    {
        USTART_Trasmit('z');
        _delay_ms(T);
    }
}

void USART_Init( unsigned int ubrr )
{
    // Set baud rate
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)ubrr;
    //Enable receiver and trasmitter
    UCSR0B = (1<<URSEL0) | (1<<TXEN0);
    //Set frame format: 8data, 2stop bit
    UCSR0C = (1<<URSEL0) | (1<<USBS0) | (3<<UCSZ00);
}

void USTART_Trasmit(unsigned char data)
{
    // Wait for empty trasmission buffer
    while( !(UCSR0A & (1 << UDRE0)) );

    // Put data into buffer, sends the data
    UDR0 = data;
}

unsigned char USTART_Receive( void )
{
    // Wait for data to be received
    while( !(UCSR0A & (1 << RXC0)) );

    // Get and return received data from buffer
    return UDR0;
}

/*
void USTART_Flush( void )
{
        unsigned char dummy;
        while (UCSRA & (1<<RXC) ) dummy = UDR;
}

unsigned char USART_ReadUCSRC( void )
{
    unsigned char ucsrc;
    // Read ucsrc
    ucsrc = UBRRH;
    ucsrc = UCSRC;
    return ucsrc;
}
*/
