#include "controls.h"

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
    return ((float) analogRead(jconfig->m_joystick_gpio[0]) / JOYSTICK_MAX_V_ADC);
}

float joystick_read_horz(joystick_config_t *jconfig) {
    return ((float) analogRead(jconfig->m_joystick_gpio[1]) / JOYSTICK_MAX_V_ADC);
}

uint8_t button_read_sel(button_config_t *bconfig){
    return digitalRead(bconfig->m_button_gpio) != HIGH;
}

void