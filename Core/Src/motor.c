#include "motor.h"

/* =========================================================
 * PRIVATE FUNCTIONS
 * ========================================================= */

static uint16_t Motor_ClampAbs(Motor_t *motor,
                               int32_t value)
{
    if (value < 0)
    {
        value = -value;
    }

    if ((uint32_t)value > motor->pwm_max)
    {
        value = motor->pwm_max;
    }

    return (uint16_t)value;
}

static uint8_t Motor_ChannelEnabled(Motor_t *motor)
{
    switch (motor->pwm_channel)
    {
        case TIM_CHANNEL_1:
            return (motor->htim->Instance->CCER &
                    TIM_CCER_CC1E) != 0U;

        case TIM_CHANNEL_2:
            return (motor->htim->Instance->CCER &
                    TIM_CCER_CC2E) != 0U;

        case TIM_CHANNEL_3:
            return (motor->htim->Instance->CCER &
                    TIM_CCER_CC3E) != 0U;

        case TIM_CHANNEL_4:
            return (motor->htim->Instance->CCER &
                    TIM_CCER_CC4E) != 0U;

        default:
            return 0U;
    }
}

/* =========================================================
 * INITIALIZATION
 * ========================================================= */

void Motor_Init(Motor_t *motor,
                TIM_HandleTypeDef *htim,
                uint32_t pwm_channel,
                GPIO_TypeDef *dir_port,
                uint16_t dir_pin,
                GPIO_TypeDef *enable_port,
                uint16_t enable_pin,
                uint16_t pwm_max,
                uint8_t pwm_inverted)
{
    if ((motor == NULL) ||
        (htim == NULL) ||
        (dir_port == NULL) ||
        (enable_port == NULL) ||
        (pwm_max == 0U))
    {
        return;
    }

    motor->htim = htim;
    motor->pwm_channel = pwm_channel;

    motor->dir_port = dir_port;
    motor->dir_pin = dir_pin;

    motor->enable_port = enable_port;
    motor->enable_pin = enable_pin;

    motor->pwm_max = pwm_max;
    motor->pwm_inverted = pwm_inverted ? 1U : 0U;

    Motor_Disable(motor);
    Motor_Coast(motor);
}

/* =========================================================
 * ENABLE / DISABLE
 * ========================================================= */

void Motor_Enable(Motor_t *motor)
{
    if (motor == NULL)
    {
        return;
    }

    HAL_GPIO_WritePin(motor->enable_port,
                      motor->enable_pin,
                      GPIO_PIN_SET);
}

void Motor_Disable(Motor_t *motor)
{
    if (motor == NULL)
    {
        return;
    }

    HAL_GPIO_WritePin(motor->enable_port,
                      motor->enable_pin,
                      GPIO_PIN_RESET);
}

/* =========================================================
 * SPEED CONTROL
 * ========================================================= */

void Motor_SetSpeed(Motor_t *motor,
                    int16_t speed)
{
    uint16_t pwm;

    if ((motor == NULL) ||
        (motor->htim == NULL))
    {
        return;
    }

    if (speed < -(int16_t)motor->pwm_max)
    {
        speed = -(int16_t)motor->pwm_max;
    }

    if (speed > (int16_t)motor->pwm_max)
    {
        speed = (int16_t)motor->pwm_max;
    }

    if (speed < 0)
    {
        HAL_GPIO_WritePin(motor->dir_port,
                          motor->dir_pin,
                          GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(motor->dir_port,
                          motor->dir_pin,
                          GPIO_PIN_RESET);
    }

    pwm = Motor_ClampAbs(motor, speed);

    if (motor->pwm_inverted)
    {
        pwm = motor->pwm_max - pwm;
    }

    if (!Motor_ChannelEnabled(motor))
    {
        return;
    }

    __HAL_TIM_SET_COMPARE(motor->htim,
                          motor->pwm_channel,
                          pwm);
}

void Motor_SetPercent(Motor_t *motor,
                      float percent)
{
    int32_t pwm;

    if (motor == NULL)
    {
        return;
    }

    if (percent > 100.0f)
    {
        percent = 100.0f;
    }

    if (percent < -100.0f)
    {
        percent = -100.0f;
    }

    pwm = (int32_t)((percent / 100.0f) *
                    (float)motor->pwm_max);

    Motor_SetSpeed(motor,
                   (int16_t)pwm);
}

/* =========================================================
 * STOP / BRAKE
 * ========================================================= */

void Motor_Coast(Motor_t *motor)
{
    if ((motor == NULL) ||
        (motor->htim == NULL))
    {
        return;
    }

    __HAL_TIM_SET_COMPARE(motor->htim,
                          motor->pwm_channel,
                          0U);
}

void Motor_Brake(Motor_t *motor)
{
    /*
     * Generic implementation.
     *
     * For most drivers this can be replaced with a
     * dedicated braking strategy.
     */
    Motor_Coast(motor);
}
