# stm32-hill-hold-assist-system
Embedded automotive hill-hold assist prototype using STM32F103C8T6, MPU6050, and L298N motor driver. Detects slope via I²C accelerometer data, engages motor-driven braking automatically, and releases only when throttle (potentiometer) acceleration exceeds threshold using PWM control and ADC feedback.
