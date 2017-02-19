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