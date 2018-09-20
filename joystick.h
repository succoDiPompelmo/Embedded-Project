/**
 * @file
 * Sample joystick driver for reading the data from the ADC about the
 * different joystick output
 */

 /**
  * @brief Initialize UART driver; necessary to call only once
  */
uint8_t JOYCON_X_Axis();

/**
 * @brief Initialize UART driver; necessary to call only once
 */
uint8_t JOYCON_Y_Axis();

/**
 * @brief Initialize UART driver; necessary to call only once
 */
uint8_t SLIDEBAR_Right();

/**
 * @brief Initialize UART driver; necessary to call only once
 */
uint8_t SLIDEBAR_Left();


void JOYSTICK_Output();