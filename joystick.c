#include <util/delay.h>
#include "joystick.h"

#define T_CONVERSION 0.800

uint8_t JOYCON_X_Axis()
{
    volatile uint8_t * ext_adc = 0x1400;
    *ext_adc = 0x0004;
    _delay_ms(T_CONVERSION);
    uint8_t read_value = *ext_adc;
    return read_value;
    //printf("X value : %02X  ", read_value);
}

int check_Y_Axis()
{
    volatile uint8_t selection = JOYCON_Y_Axis();
    if (selection > 0xF2) return 1;
    if (selection < 0x88) return -1;
    return 0;
}

uint8_t JOYCON_Y_Axis()
{
    volatile uint8_t * ext_adc = 0x1400;
    *ext_adc = 0x0005;
    _delay_ms(T_CONVERSION);
    uint8_t read_value = *ext_adc;
    return read_value;
    //printf("Y value : %02X  ", read_value);
}

uint8_t SLIDEBAR_Right()
{
    volatile uint8_t * ext_adc = 0x1400;
    *ext_adc = 0x0006;
    _delay_ms(T_CONVERSION);
    uint8_t read_value = *ext_adc;
    return read_value;
    //printf("Slidebar rigth value : %02X  ", read_value);
}

uint8_t SLIDEBAR_Left()
{
    volatile uint8_t * ext_adc = 0x1400;
    *ext_adc = 0x0007;
    _delay_ms(T_CONVERSION);
    uint8_t read_value = *ext_adc;
    return read_value;
    //printf("Slidebar left value%02X\n\r", read_value);
}

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
    //printf("%02X  ", x_value);
    //printf("%02X  ",y_value);
    //printf("%02X  ",slidebar_r);
}
