#ifndef __CHASSIS_CTRL_H__
#define __CHASSIS_CTRL_H__

#include <cstdint>
#include <climits>
#include <array>

#include "bdc_motor.h"
#include "rfmgr_srv.h"

// Rangefinder Values
#define SOUND_SPEED 0.034 // Speed of Sound in cm/uS
#define SIZE_EQUALIZER_IN 0.5
#define CM_TO_IN 0.393701
#define ECHO_PIN 33;
#define TRIG_PIN 32;

// ----------------------------------------------------------------
// This header contains the class which provides low level control
// for all the components on the reciever (robot), as well as definitions
// for different enums.
// ---------------------------------------------------------------- 
typedef enum jstick_select_e {
    JOYSTICK_RIGHT_X, ///< Right X Axis Joystrick
    JOYSTICK_RIGHT_Y, ///< Right Y Axis Joystrick
    JOYSTICK_LEFT_X,  ///< Left X Axis Joystrick
    JOYSTICK_LEFT_Y,  ///< Left Y Axis Joystrick
    JOYSTICK_INVALID = INT_MAX
} jstick_select_e_t;

typedef enum btn_select_e {
    BTN_DRIVE_MODE, ///< button for toggling the drive mode
    BTN_RANGE_EN,   ///< button for toggling the range finder
    BTN_INVALID = INT_MAX
} btn_select_e_t;

typedef enum drive_mode_e {
    DRIVE_MODE_ARCADE, ///< active drive mode arcade
    DRIVE_MODE_TANK    ///< active drive mode tank
} drive_mode_e_t;

class ChassisControl {
private:
    /*          FRONT        */
    /* motor 1  ---- motor 4 */
    /* motor 2  ---- motor 3 */
    /*          BACK         */
    std::array<bdc_motor_config_t, 4> m_motor_cfg;

    hw_timer_t * rngFTimer;

public:
    void init_chassis();

    void set_left_motors(float duty_cycle);
    void set_right_motors(float duty_cycle);

    void start_left_motors();
    void start_right_motors();
    void stop_left_motors();
    void stop_right_motors();

    float get_jstick(ScoutBot_Server::RFManager_Service *receiver_service, jstick_select_e_t jstick);

    bool is_range_enabled(ScoutBot_Server::RFManager_Service *receiver_service);
    drive_mode_e get_drive_mode(ScoutBot_Server::RFManager_Service *receiver_service);

    void setup_rangefinder();
    float get_range();
    int distanceSetting();
};

extern ChassisControl g_chassis_ctrl;

#endif /* __CHASSIS_CTRL_H_ */
