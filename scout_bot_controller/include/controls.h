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

#define JOYSTICK_MAX_V_ADC 1023

typedef struct joystick_config {
    uint8_t m_joystick_gpio[3];
} joystick_config_t;

void joystick_setup(joystick_config_t *jconfig, uint8_t vert_pin, uint8_t horz_pin, uint8_t sel_pin) {
    jconfig->m_joystick_gpio[0] = vert_pin;
    jconfig->m_joystick_gpio[1] = horz_pin;
    jconfig->m_joystick_gpio[2] = sel_pin;
    
    pinMode(jconfig->m_joystick_gpio[2], INPUT);
}

uint8_t joystick_read_sel(joystick_config_t *jconfig) {
    return digitalRead(jconfig->m_joystick_gpio[2]) != HIGH;
}

float joystick_read_vert(joystick_config_t *jconfig) {
    return (float) analogRead(jconfig->m_joystick_gpio[0]) / JOYSTICK_MAX_V_ADC;
}

float joystick_read_horz(joystick_config_t *jconfig) {
    return (float) analogRead(jconfig->m_joystick_gpio[1]) / JOYSTICK_MAX_V_ADC;
}