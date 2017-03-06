# rit_first_robot

A rewrite of 2016's Imagine RIT robot code.

## Driving the Robot

The robot has a [DRV8836](http://www.ti.com/lit/ds/symlink/drv8836.pdf) motor driver. Due to the lack of pins on the Arduino Pro Mini (I assume...), the MODE pin (11) is wired to VCC (through a resistor), forcing the driver to always be in PHASE/ENABLE mode. The rest of the pin numbering is then:

Motor Driver (pin #) | Arduino Pin
-------------------- | -----------
A_ENABLE (pin 9) | 5
A_PHASE (pin 10) | 3
B_ENABLE (pin 7) | 6
B_PHASE (pin 8) | 4

x_ENABLE pins turn the motor on (when HIGH) and off (when LOW). In order to vary the speed of the motor, PWM can be utilized. The x_PHASE pins are used to change direction, so logic HIGH would drive the motor in reverse, while logic LOW would drive the motor forward.

## Servos

Servo header were built into the board to the northwest of the Arduino. The control of the headers are wired to A0, A1, A2, and A3 (also called pins 14, 15, 16, and 17), and +5v was wired to the 5v main bus. Only problem is that the ground bus was not correctly connected to ground, so there must be a manual connection from the ground bus to the ground pin to the south of the headers. Some boards have this done already, these ones should be used for servo control. Additionally, because RadioHead is used for wireless, SoftwareServo must be used for the servos.

## Wireless

There is an NRF24 header to the east of the Arduino and it is wired to the correct pins to the Arduino in the board. However, the NRF24 runs on 3.3v, while the Arduino Pro Mini runs on 5v. A voltage divider is built into the board to provide 3.3v, but it does not provide enough current to send data, only enough to receive. In order to get around this, a voltage divider was attached to the robot on the 5v bus and then connected to the NRF24.

### Wireless transfer standards

From the field controller, an unsigned byte array of length 6 is sent to the robot. It's contents are:

| 1 | 2 | 3 | 4 | 5 | 6 |
|---|---|---|---|---|---|
left side drive (0-255, 127 center) | right side drive (0-255, 127 center) | (0-1) | (0-1) | (0-1) | (0-1)
