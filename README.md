# stm32-motor

STM32 HAL motor driver for PWM-controlled hobby motors with configurable direction control.

## Project Overview

This project provides a lightweight motor control module for STM32 microcontrollers using STM32 HAL.

The driver supports:

- PWM speed control
- Direction control
- Positive and negative PWM logic
- Hardware timer PWM generation
- Runtime channel selection

## Key Features

- STM32 HAL based
- Multiple motor instances
- Runtime enable/disable
- Percentage-based speed control
- Small memory footprint

## Hardware

| Component | Requirement |
|------------|------------|
| MCU | STM32 with TIM peripherals |
| Motor Type | Brushed DC / BLDC Motor |
| Driver | H-Bridge or BLDC motor driver |
| PWM Source | STM32 Timer PWM |

## Software

- Programming Language: C
- IDE: STM32CubeIDE / STM32CubeMX
- Framework: STM32 HAL

## Repository Structure

```text
stm32-dc-motor/
├── Core/
│   ├── Inc/
│   │   └── motor.h
│   └── Src/
│       └── motor.c
├── LICENSE
└── README.md
```

## Initialization

```c
Motor_Init(&htim1);
```

## Set Motor Speed

Forward rotation:

```c
Motor_Set_Pwm(DIR_Pin,
              DIR_GPIO_Port,
              1500,
              0,
              TIM_CHANNEL_1);
```

Reverse rotation:

```c
Motor_Set_Pwm(DIR_Pin,
              DIR_GPIO_Port,
              -1500,
              0,
              TIM_CHANNEL_1);
```

## Notes

PWM Logic Inversion:

Normal PWM:

```c
pwm_inverted = 0;
```

Inverted PWM:

```c
pwm_inverted = 1;
```

## Example Usage

```c
Motor_t left_motor;

Motor_Init(
    &left_motor,
    &htim1,
    TIM_CHANNEL_1,
    MOTOR_DIR_GPIO_Port,
    MOTOR_DIR_Pin,
    MOTOR_EN_GPIO_Port,
    MOTOR_EN_Pin,
    2500U,
    0U
);

Motor_Enable(&left_motor);

Motor_SetPercent(&left_motor, 50.0f);

Motor_SetSpeed(&left_motor, -1250);

Motor_Brake(&left_motor);

Motor_Disable(&left_motor);
```

## Future Improvements

- implement proper motor break feature
- provide example implementation
