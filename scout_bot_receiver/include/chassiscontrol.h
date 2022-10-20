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

class chassisControl {
    public:
        std::uint32_t set_left_motors(uint32_t voltage);
        std::uint32_t set_right_motors(uint32_t voltage);

        std::uint32_t get_jstick(jstick_select_e_t jstick);
        std::uint32_t get_range();
        
        std::uint32_t toggle_drive_mode();
        bool arcade_enable = true;
};


#endif /* __CHASSISCONTROL_H_ */
