# Robo Soccer Bot (RC Controlled)

## Overview

This project is a Robo Soccer Bot controlled using a standard transmitter and receiver system. It is designed for responsive movement and precise control, suitable for robotic soccer applications.

An ESP32 microcontroller reads input signals from the receiver and controls the motors using PWM to achieve smooth and proportional motion.

---

## Features

* Wireless control using RC transmitter and receiver
* Real-time motor response
* Differential drive (tank steering)
* Smooth diagonal movement using proportional scaling
* Failsafe mechanism for signal loss
* Deadzone handling for stable control

---

## Working Principle

The receiver outputs PWM signals corresponding to throttle (forward/backward) and steering (left/right).

The ESP32 reads these signals using `pulseIn()`. The input range of 1000–2000 microseconds is mapped to motor speed values ranging from -255 to +255.

A mixing algorithm is used:

* Left motor speed = Throttle + Steering
* Right motor speed = Throttle - Steering

Proportional scaling ensures that both motors maintain the correct ratio when maximum speed is reached, allowing smooth turning even at high speeds.

PWM signals are generated using ESP32 LEDC channels and sent to the motor driver.

---

## Hardware Used

* ESP32 microcontroller
* RC transmitter and receiver
* Motor driver (BTS7960 or equivalent)
* DC motors
* Battery
* Robot chassis

---

## Pin Configuration

| Function            | Pin |
| ------------------- | --- |
| Steering Input      | 34  |
| Throttle Input      | 35  |
| Left Motor Forward  | 19  |
| Left Motor Reverse  | 21  |
| Right Motor Forward | 22  |
| Right Motor Reverse | 23  |

---

## How to Run

1. Connect all components according to the pin configuration
2. Upload the code (`rc_car_control.ino`) to the ESP32 using Arduino IDE
3. Power the system
4. Turn on the transmitter
5. Control the robot using joystick inputs

---

## Safety Features

* Stops motors when signal is lost for multiple cycles
* Uses cached values to handle temporary signal drops
* Deadzone prevents unintended minor movements

---

## Results

* Smooth forward and reverse motion
* Stable turning and diagonal movement
* Reliable operation under minor signal interruptions


## Future Improvements

* Autonomous mode using sensors
* Vision-based tracking
* Speed feedback using encoders
* Mobile application control


## Submission Note

This repository is submitted for evaluation. All relevant code, control logic, and outputs are included.

