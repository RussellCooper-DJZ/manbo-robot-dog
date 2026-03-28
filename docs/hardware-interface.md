# Hardware Interface Specification

## Overview

This document describes the hardware interfaces used in the Manbo Robot Dog project.

## MCU: STM32F407VGT6

| Parameter | Value |
|-----------|-------|
| Core | ARM Cortex-M4F |
| Clock | 168 MHz |
| Flash | 1 MB |
| RAM | 192 KB |
| FPU | Single-precision |

## Servo Interface

The robot uses 12 PWM-controlled servo motors (3 per leg).

### PWM Configuration

```c
// Timer configuration for servo control
#define SERVO_TIMER         TIM4
#define SERVO_PRESCALER     167     // 168MHz / 168 = 1MHz
#define SERVO_PERIOD        19999   // 20ms period (50Hz)
#define SERVO_MIN_PULSE_US  500     // 0 degrees
#define SERVO_MID_PULSE_US  1500    // 90 degrees
#define SERVO_MAX_PULSE_US  2500    // 180 degrees
```

### Servo Mapping

| Servo ID | Leg | Joint | GPIO Pin | Timer Channel |
|----------|-----|-------|----------|---------------|
| 0 | Front-Left | Hip | PD12 | TIM4_CH1 |
| 1 | Front-Left | Thigh | PD13 | TIM4_CH2 |
| 2 | Front-Left | Calf | PD14 | TIM4_CH3 |
| 3 | Front-Right | Hip | PD15 | TIM4_CH4 |
| 4 | Front-Right | Thigh | PB6 | TIM4_CH1 |
| 5 | Front-Right | Calf | PB7 | TIM4_CH2 |

## IMU: MPU-6050

- Interface: I2C (400 kHz Fast Mode)
- I2C Address: 0x68 (AD0 = GND)
- Update Rate: 1 kHz
- Gyroscope Range: +/-500 deg/s
- Accelerometer Range: +/-4g

### I2C Pin Assignment

| Signal | GPIO |
|--------|------|
| SCL | PB8 |
| SDA | PB9 |

## Power Distribution

| Rail | Voltage | Max Current | Source |
|------|---------|-------------|--------|
| Logic | 3.3V | 500mA | LDO from 5V |
| Servo | 6.0V | 5A | DC-DC Buck |
| MCU | 3.3V | 200mA | LDO |
| Sensors | 3.3V | 100mA | LDO |
