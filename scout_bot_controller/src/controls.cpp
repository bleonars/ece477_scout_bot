#include "controls.h"

#include <numeric>

void joystick_setup(joystick_config_t *jconfig, uint8_t vert_pin, uint8_t horz_pin, uint8_t sel_pin) {
    std::array<float, JOYSTICK_CALIBRATION_BUF_SIZE> calibration_buffer;

    jconfig->m_joystick_gpio[0] = vert_pin;
    jconfig->m_joystick_gpio[1] = horz_pin;
    jconfig->m_joystick_gpio[2] = sel_pin;
    
    pinMode(jconfig->m_joystick_gpio[2], INPUT);
    
    for (size_t i = 0; i < JOYSTICK_CALIBRATION_BUF_SIZE; ++i)
        calibration_buffer[i] = (float) analogRead(jconfig->m_joystick_gpio[0]);
    
    jconfig->m_calibration_offset = std::accumulate(calibration_buffer.begin(), calibration_buffer.end(), 0) / JOYSTICK_CALIBRATION_BUF_SIZE;
}

uint8_t joystick_read_sel(joystick_config_t *jconfig) {
    return digitalRead(jconfig->m_joystick_gpio[2]) != HIGH;
}

static float joystick_read(joystick_config_t *jconfig, size_t port, float deadzone_ratio) {
    float deadzone_upper_volt = (float) jconfig->m_calibration_offset * (1.f + deadzone_ratio);
    float deadzone_lower_volt = (float) jconfig->m_calibration_offset * (1.f - deadzone_ratio);
    
    float adc_read_volt = (float) analogRead(jconfig->m_joystick_gpio[port]);

    if (adc_read_volt >= deadzone_lower_volt && adc_read_volt <= deadzone_upper_volt)
        return 0.f;

    if (adc_read_volt > deadzone_upper_volt) {
        float input_ratio = (adc_read_volt - deadzone_upper_volt) / (JOYSTICK_MAX_V_ADC - deadzone_upper_volt);

        return (input_ratio * 100.f);
    }
    else {
        float input_ratio = (deadzone_lower_volt - adc_read_volt) / deadzone_lower_volt;
        
        return -(input_ratio * 100.f);
    }
}

float joystick_read_vert(joystick_config_t *jconfig) {
    return joystick_read(jconfig, 0, JOYSTICK_DEADZONE_RATIO);
}

float joystick_read_horz(joystick_config_t *jconfig) {
    return joystick_read(jconfig, 1, JOYSTICK_DEADZONE_RATIO);
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