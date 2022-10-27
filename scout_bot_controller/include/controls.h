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

typedef struct joystick_config {
    uint8_t m_joystick_gpio[3];
} joystick_config_t;

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
 * @return float 0 - 1 value, 0.44-0.47 for no input, 1.0 for fully up, 0.0 for fully down
 */
float joystick_read_vert(joystick_config_t *jconfig);

/**
 * @brief read analog joystick horizontal axis
 * 
 * @param jconfig joystick config
 * @return float 0 - 1 value, 0.44-0.47 for no input, 1.0 for fully left, 0.0 for fully right
 */
float joystick_read_horz(joystick_config_t *jconfig);