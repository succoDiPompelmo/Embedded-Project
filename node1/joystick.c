#include <util/delay.h>
#include "joystick.h"

#define T_CONVERSION 0.200

uint8_t JOYCON_X_Axis()
{
    // Set ADC adress
    volatile uint8_t * ext_adc = 0x1400;
    // Send the ADC the value of the channel that you want to read
    *ext_adc = 0x0004;
    // Wait the ADC for finish the conversion
    _delay_ms(T_CONVERSION);
    // Read the ADC value
    uint8_t read_value = *ext_adc;

    return read_value;
}

// Retunr 1 if joystic y position is higher than a certain amount
// or -1 if is less than a certain amount
int check_Y_Axis_UP()
{
    volatile uint8_t selection = JOYCON_Y_Axis();
    if (selection > 0xF2) return 1;
    if (selection < 0x88) return -1;
    return 0;
}

// Retunr 1 if joystic y position is lower than a certain amount
// or -1 if is higher than a certain amount
int check_Y_Axis_DOWN()
{
    volatile uint8_t selection = JOYCON_Y_Axis();
    if (selection < 0x08) return 1;
    if (selection > 0x72) return -1;
    return 0;
}

uint8_t JOYCON_Y_Axis()
{
    // Set ADC adress
    volatile uint8_t * ext_adc = 0x1400;
    // Send the ADC the value of the channel that you want to read
    *ext_adc = 0x0005;
    // Wait the ADC for finish the conversion
    _delay_ms(T_CONVERSION);
    // Read the ADC value
    uint8_t read_value = *ext_adc;

    return read_value;
}

uint8_t SLIDEBAR_Right()
{
    // Set ADC adress
    volatile uint8_t * ext_adc = 0x1400;
    // Send the ADC the value of the channel that you want to read
    *ext_adc = 0x0006;
    // Wait the ADC for finish the conversion
    _delay_ms(T_CONVERSION);
    // Read the ADC value
    uint8_t read_value = *ext_adc;

    return read_value;
}

uint8_t SLIDEBAR_Left()
{
    // Set ADC adress
    volatile uint8_t * ext_adc = 0x1400;
    // Send the ADC the value of the channel that you want to read
    *ext_adc = 0x0007;
    // Wait the ADC for finish the conversion
    _delay_ms(T_CONVERSION);
    // Read the ADC value
    uint8_t read_value = *ext_adc;

    return read_value;
}

// Read and print all the Ooutput fomr the joystick togheter
void JOYSTICK_Output()
{
    volatile uint8_t x_value = JOYCON_X_Axis();
    printf("%02X  ", x_value);
    _delay_ms(200);
    volatile uint8_t y_value = JOYCON_Y_Axis();
    printf("%02X  ", y_value);
    _delay_ms(200);
    volatile uint8_t slidebar_r = SLIDEBAR_Right();
    printf("%02X  ", slidebar_r);
    _delay_ms(200);
    volatile uint8_t slidebar_l = SLIDEBAR_Left();
    printf("%02X  \n\r", slidebar_l);
    _delay_ms(200);
}
