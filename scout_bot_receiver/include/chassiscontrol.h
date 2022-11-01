#ifndef __CHASSISCONTROL_H__
#define __CHASSISCONTROL_H__

#include <cstdint>
#include <limits.h>

// ----------------------------------------------------------------
// This header contains the class which provides low level control
// for all the components on the reciever (robot), as well as definitions
// for different enums.
// ---------------------------------------------------------------- 
typedef enum jstick_select_e {
    JOYSTICK_RIGHT_X, ///< Right X Axis Joystrick
    JOYSTICK_RIGHT_Y,///< Right Y Axis Joystrick
    JOYSTICK_LEFT_Y,///< Left Y Axis Joystrick
    JOYSTICK_INVALID = INT_MAX
} jstick_select_e_t;


typedef enum btn_select_e {
    BTN_TOGGLE, ///< button for toggling the drive mode
    BTN_INVALID = INT_MAX
} btn_select_e_t;

class ChassisControl {
    private:
        bdc_motor_config_t motor_1_cfg; // left front
        bdc_motor_config_t motor_2_cfg; // left back

        bdc_motor_config_t motor_3_cfg; // right front
        bdc_motor_config_t motor_4_cfg; // right back

    public:
        void init_chassis();
        void set_left_motors(float duty_cycle);
        void set_right_motors(float duty_cycle);

        void start_motors();
        void stop_motors();

        std::uint32_t get_jstick(jstick_select_e_t jstick);
        std::uint32_t get_range();
        
        std::uint32_t toggle_drive_mode();
        bool arcade_enable = true;
};


#endif /* __CHASSISCONTROL_H_ */
