#include "main.h"

#include "driver/mcpwm.h"
#include "soc/mcpwm_reg.h"
#include "soc/mcpwm_struct.h"
#include "soc/mcpwm_periph.h"

#define VEX393_PWM_BASE_FREQ 200 /* VEX393 Motor Default Freq Hz */

static mcpwm_dev_t *mcpwm0_reg = &MCPWM0; /* MCPWM0 Unit Register */
static mcpwm_dev_t *mcpwm1_reg = &MCPWM1; /* MCPWM0 Unit Register */

typedef struct bdc_motor_config {
    mcpwm_unit_t  m_mcpwm_unit;         /* MCPWM Unit  */
    mcpwm_timer_t m_mcpwm_timer;        /* MCPWM Timer */
    uint8_t       m_mcpwm_out_gpios[2]; /* PWM GPIO Output A & B */
} bdc_motor_config_t;

/**
 * @brief setup bdc motor according to config, needs MCPWM Unit 0-1, MCPWM Timer 0-3, 2 GPIO for Output A & B
 * https://docs.espressif.com/projects/esp-idf/en/v4.4.2/esp32/api-reference/peripherals/mcpwm.html#configure
 * 
 * @param bdc_config bdc motor config
 */
void bdc_motor_setup(bdc_motor_config_t *bdc_config);

/**
 * @brief start bdc motor
 * https://docs.espressif.com/projects/esp-idf/en/v4.4.2/esp32/api-reference/peripherals/mcpwm.html#operate
 * @param bdc_config bdc motor config
 */
void bdc_motor_drive(bdc_motor_config_t *bdc_config);

/**
 * @brief stop bdc motor
 * https://docs.espressif.com/projects/esp-idf/en/v4.4.2/esp32/api-reference/peripherals/mcpwm.html#operate
 * @param bdc_config bdc motor config
 */
void bdc_motor_stop(bdc_motor_config_t *bdc_config);

/**
 * @brief set bdc motor speed by pwm duty cycle, positive duty indicates forward, negative duty indicates backwards
 * https://docs.espressif.com/projects/esp-idf/en/v4.4.2/esp32/api-reference/peripherals/mcpwm.html#operate
 *
 * @param bdc_config bdc motor config
 * @param duty_cycle_percentage percentage duty cycle, 60.3% -> duty_cycle_percentage = 60.3f
 */
void bdc_motor_set_speed(bdc_motor_config_t *bdc_config, float duty_cycle_percentage);