#include "main.h"
#include <stdint.h>

/* PCB S8 */
#define JOYSTICK_L_VERT 34
#define JOYSTICK_L_HORZ 35
#define JOYSTICK_L_SEL 18

/* PCB S9 */
#define JOYSTICK_R_VERT 32
#define JOYSTICK_R_HORZ 33
#define JOYSTICK_R_SEL 19

#define JOYSTICK_MAX_V_ADC 4095
#define JOYSTICK_CALIBRATION_BUF_SIZE 10
#define JOYSTICK_DEADZONE_RATIO 0.05f

/* PCB S3 */
#define BUTTON_1_SEL 5

/* PCB S5 */
#define BUTTON_2_SEL 22

/* PCB S6 */
#define BUTTON_3_SEL 23

/* PCB S7 */
#define BUTTON_4_SEL 21

typedef struct joystick_config {
    uint8_t  m_joystick_gpio[3];
    float    m_calibration_offset;
} joystick_config_t;

typedef struct button_config {
    uint8_t m_button_gpio[4];
} button_config_t;

/**
 * @brief setup a joystick based on input pins
 * 
 * @param jconfig joystick config
 * @param vert_pin vertical analog adc pin (y-axis)
 * @param horz_pin horizontal analog adc pin (x-axis)
 * @param sel_pin selection digital pin (press-down)
 */
void joystick_setup(joystick_config_t *jconfig, uint8_t vert_pin, uint8_t horz_pin, uint8_t sel_pin);

/**
 * @brief read digigal joystick selection
 * 
 * @param jconfig joystick config
 * @return uint8_t 0 if not pressed, 1 if pressed
 */
uint8_t joystick_read_sel(joystick_config_t *jconfig);

/**
 * @brief read analog joystick vertical axis
 * 
 * @param jconfig joystick config
 * @return float duty cycle 0% to 100%, negative for backwards
 */
float joystick_read_vert(joystick_config_t *jconfig);

/**
 * @brief read analog joystick horizontal axis
 * 
 * @param jconfig joystick config
 * @return float duty cycle 0% to 100%, negative for backwards
 */
float joystick_read_horz(joystick_config_t *jconfig);



/**
* @brief setup the four controller buttons based on input 
* 
* @param bconfig button config
* @param pin_1 button 1 pin
* @param pin_2 button 2 pin
* @param pin_3 button 3 pin
* @param pin_4 button 4 pin
*/
void button_setup(button_config_t *bconfig, uint8_t pin_1, uint8_t pin_2, uint8_t pin_3, uint8_t pin_4);

/**
 * @brief read button by idx
 * 
 * @param bconfig button config
 * @param button_idx index of button to read
 * @return uint8_t if not pressed, 1 if pressed
*/
uint8_t button_read_sel(button_config_t *bconfig, size_t button_idx);