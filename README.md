# stm32-hill-hold-assist-system

## Project Description

This project implements an embedded automotive hill-hold assist prototype using the STM32F103C8T6 (Blue Pill) microcontroller, ADXL345 accelerometer, and L298N motor driver to prevent rollback on inclined surfaces.

The system continuously reads accelerometer data over I²C to calculate slope angle in real time using trigonometric tilt estimation. When the detected incline exceeds **25°**, the controller activates hill-hold mode.

Brake engagement logic is based on simulated driver pedal inputs using potentiometers:

- **Clutch < 10% and Acceleration < 10%**
  → Brake automatically applied
  → Motor stopped
  → Vehicle held stationary on slope

Brake release logic:

- If hill-hold is active and
- **Clutch > 15% OR Acceleration > 15%**
  → Brake released
  → Motor drive enabled
  → Vehicle allowed to move forward

For flat-road operation:

- No pedal input → system remains idle
- Pedal input detected → motor allowed to move normally

A servo motor simulates the braking mechanism, while the DC motor controlled through the L298N represents drivetrain movement. ADC inputs on STM32 are used for real-time throttle and clutch sensing, creating a closed-loop control model.

### Engineering Concepts Demonstrated

- STM32 embedded firmware development
- I²C sensor interfacing (ADXL345)
- ADC-based real-time input acquisition
- PWM / actuator control
- Threshold-based automotive control logic
- Servo brake actuation
- Motor driver interfacing (L298N)
- Real-time decision making
- Closed-loop embedded control systems

### Applications

- Automotive hill-start assist systems
- Driver assistance prototypes
- Embedded vehicle control systems
- Mechatronic braking automation
