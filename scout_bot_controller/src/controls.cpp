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

void button_setup(button_config_t *bconfig, uint8_t pin_1, uint8_t pin_2, uint8_t pin_3, uint8_t pin_4) {
    bconfig->m_button_gpio[0] = pin_1;
    bconfig->m_button_gpio[1] = pin_2;
    bconfig->m_button_gpio[2] = pin_3;
    bconfig->m_button_gpio[3] = pin_4;
}

uint8_t button_read_sel(button_config_t *bconfig, size_t button_idx){
    return digitalRead(bconfig->m_button_gpio[button_idx]) != HIGH;
}