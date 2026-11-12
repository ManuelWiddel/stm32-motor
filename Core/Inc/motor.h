#ifndef MOTOR_H
#define MOTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#include <stdint.h>

/* =========================================================
 * MOTOR DATA STRUCTURE
 * ========================================================= */

typedef struct
{
    TIM_HandleTypeDef *htim;
    uint32_t pwm_channel;

    GPIO_TypeDef *dir_port;
    uint16_t dir_pin;

    GPIO_TypeDef *enable_port;
    uint16_t enable_pin;

    uint16_t pwm_max;
    uint8_t pwm_inverted;

} Motor_t;

/* =========================================================
 * INITIALIZATION
 * ========================================================= */

/**
 * @brief Initialize a motor instance.
 *
 * @param motor Motor instance.
 * @param htim PWM timer.
 * @param pwm_channel Timer channel.
 * @param dir_port Direction GPIO port.
 * @param dir_pin Direction GPIO pin.
 * @param enable_port Enable GPIO port.
 * @param enable_pin Enable GPIO pin.
 * @param pwm_max Maximum timer compare value.
 * @param pwm_inverted 0 = normal PWM, 1 = inverted PWM.
 */
void Motor_Init(Motor_t *motor,
                TIM_HandleTypeDef *htim,
                uint32_t pwm_channel,
                GPIO_TypeDef *dir_port,
                uint16_t dir_pin,
                GPIO_TypeDef *enable_port,
                uint16_t enable_pin,
                uint16_t pwm_max,
                uint8_t pwm_inverted);

/* =========================================================
 * MOTOR CONTROL
 * ========================================================= */

/**
 * @brief Enable motor driver.
 */
void Motor_Enable(Motor_t *motor);

/**
 * @brief Disable motor driver.
 */
void Motor_Disable(Motor_t *motor);

/**
 * @brief Set motor speed using timer counts.
 *
 * Range:
 *
 *     -pwm_max ... +pwm_max
 *
 * Negative values reverse direction.
 */
void Motor_SetSpeed(Motor_t *motor,
                    int16_t speed);

/**
 * @brief Set motor speed in percent.
 *
 * Range:
 *
 *     -100.0f ... +100.0f
 */
void Motor_SetPercent(Motor_t *motor,
                      float percent);

/**
 * @brief Stop motor output.
 */
void Motor_Coast(Motor_t *motor);

/**
 * @brief Active brake.
 *
 * Default implementation equals coast.
 * Can be customized for a specific H-bridge.
 */
void Motor_Brake(Motor_t *motor);

#ifdef __cplusplus
}
#endif

#endif /* MOTOR_H */
