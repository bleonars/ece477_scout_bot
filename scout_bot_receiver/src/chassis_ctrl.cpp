#include "chassis_ctrl.h"
#include "payload.h"

void ChassisControl::init_chassis() {
    m_motor_cfg[0].m_mcpwm_out_gpios[0] = 5; 
    m_motor_cfg[0].m_mcpwm_out_gpios[1] = 19; 
    m_motor_cfg[0].m_mcpwm_unit         = MCPWM_UNIT_0;
    m_motor_cfg[0].m_mcpwm_timer        = MCPWM_TIMER_0;
    bdc_motor_setup(&m_motor_cfg[0]);

    m_motor_cfg[1].m_mcpwm_out_gpios[0] = 19; 
    m_motor_cfg[1].m_mcpwm_out_gpios[1] = 21; 
    m_motor_cfg[1].m_mcpwm_unit         = MCPWM_UNIT_0;
    m_motor_cfg[1].m_mcpwm_timer        = MCPWM_TIMER_0;
    bdc_motor_setup(&m_motor_cfg[1]);
   
    m_motor_cfg[2].m_mcpwm_out_gpios[0] = 22; 
    m_motor_cfg[2].m_mcpwm_out_gpios[1] = 23; 
    m_motor_cfg[2].m_mcpwm_unit         = MCPWM_UNIT_1;
    m_motor_cfg[2].m_mcpwm_timer        = MCPWM_TIMER_1;
    bdc_motor_setup(&m_motor_cfg[2]);
    
    m_motor_cfg[3].m_mcpwm_out_gpios[0] = 26; 
    m_motor_cfg[3].m_mcpwm_out_gpios[1] = 27; 
    m_motor_cfg[3].m_mcpwm_unit         = MCPWM_UNIT_1;
    m_motor_cfg[3].m_mcpwm_timer        = MCPWM_TIMER_1;
    bdc_motor_setup(&m_motor_cfg[3]);

    setup_rangefinder();
}

void ChassisControl::start_left_motors() {
    bdc_motor_drive(&m_motor_cfg[0]);
    bdc_motor_drive(&m_motor_cfg[1]);
}

void ChassisControl::start_right_motors() {
    bdc_motor_drive(&m_motor_cfg[2]);
    bdc_motor_drive(&m_motor_cfg[3]);
}

void ChassisControl::stop_left_motors() {
    bdc_motor_stop(&m_motor_cfg[0]);
    bdc_motor_stop(&m_motor_cfg[1]);
}

void ChassisControl::stop_right_motors() {
    bdc_motor_stop(&m_motor_cfg[2]);
    bdc_motor_stop(&m_motor_cfg[3]);
}

void ChassisControl::set_left_motors(float duty_cycle) {
    bdc_motor_set_speed(&m_motor_cfg[0], duty_cycle);
    bdc_motor_set_speed(&m_motor_cfg[1], duty_cycle);
}

void ChassisControl::set_right_motors(float duty_cycle) {
    bdc_motor_set_speed(&m_motor_cfg[2], duty_cycle);
    bdc_motor_set_speed(&m_motor_cfg[3], duty_cycle);
}

void ChassisControl::setup_rangefinder(){
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    // Timer 2 for every microsecond, 12.5 * 80
    rngFTimer = timer(2, 80, true);
    timerAttachInterrupt(rngFTimer, get_range, true);
    // Currently runs every 100000 * 1 us = 0.1 seconds
    timerAlarmWrite(rngFTimer, 100000, true);
    timerAlarmEnable(rngFTimer);
}

float ChassisControl::get_range() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    float duration = pulseIn(ECHO_PIN, HIGH);
    float distance = (duration * SOUND_SPEED / 2);
    
    return distance;
}

int ChassisControl::distanceSetting() {
    int setting; // 0 for close, 1 to medium distance, 2 for free range
    float length = get_range() * CM_TO_IN;

    if (length <= 6){
        setting = 0;
    }
    else if (10 > length > 6) {
        setting = 1;
    }
    else {
        setting = 2;
    }

    return setting;
}

float ChassisControl::get_jstick(ScoutBot_Server::RFManager_Service *receiver_service, jstick_select_e_t jstick) {
    switch (jstick) {
    case JOYSTICK_RIGHT_X:
        return Utils::payload_to_f32(receiver_service->get_characteristic(BLEUUID(RIGHT_JS_X_CHARACTERISTIC_UUID))->getData());
    case JOYSTICK_RIGHT_Y:
        return Utils::payload_to_f32(receiver_service->get_characteristic(BLEUUID(RIGHT_JS_Y_CHARACTERISTIC_UUID))->getData());
    case JOYSTICK_LEFT_X:
        return Utils::payload_to_f32(receiver_service->get_characteristic(BLEUUID(LEFT_JS_X_CHARACTERISTIC_UUID))->getData());
    case JOYSTICK_LEFT_Y:
        return Utils::payload_to_f32(receiver_service->get_characteristic(BLEUUID(LEFT_JS_Y_CHARACTERISTIC_UUID))->getData());
    default:
        return 50.f;
    };
}

bool ChassisControl::is_range_enabled(ScoutBot_Server::RFManager_Service *receiver_service) {
    return Utils::payload_to_u8(receiver_service->get_characteristic(BLEUUID(RANGE_ENABLE_CHARACTERISTIC_UUID))->getData()) == 1;
}

drive_mode_e ChassisControl::get_drive_mode(ScoutBot_Server::RFManager_Service *receiver_service) {
    uint32_t ret = Utils::payload_to_u8(receiver_service->get_characteristic(BLEUUID(DRIVE_MODE_CHARACTERISTIC_UUID))->getData());
    return ret ? DRIVE_MODE_TANK : DRIVE_MODE_ARCADE;
}
