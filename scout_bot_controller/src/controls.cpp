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

static float joystick_read(joystick_config_t *jconfig, size_t port) {
    static float deadzone_max = (float) jconfig->m_calibration_offset * (1.f + JOYSTICK_DEADZONE_RATIO);
    static float deadzone_min = (float) jconfig->m_calibration_offset * (1.f - JOYSTICK_DEADZONE_RATIO);
    
    float read = (float) analogRead(jconfig->m_joystick_gpio[0]);

    if (read >= deadzone_min && read <= deadzone_max) {
        return 0.f;
    }
    else if (read > deadzone_max) {
        static float upper_range = (JOYSTICK_MAX_V_ADC - jconfig->m_calibration_offset);

        float ratio = (read - jconfig->m_calibration_offset) / upper_range;

        return (ratio * 100.f);
    }
    else {
        static float lower_range = jconfig->m_calibration_offset;

        float ratio = (jconfig->m_calibration_offset - read) / lower_range;
        
        return -1.f * (ratio * 100.f);
    }
}

float joystick_read_vert(joystick_config_t *jconfig) {
    return joystick_read(jconfig, 0);
}

float joystick_read_horz(joystick_config_t *jconfig) {
    return joystick_read(jconfig, 1);
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