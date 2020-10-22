Arduino Pro Micro (ATMEL32u4) based controller for flight simulators
that uses two linear pots (throttle and mixture) and a rotary encoder
(trim).

This sketch heavily leverages the following libraries to do the heavy
lifting:

* https://github.com/MHeironimus/ArduinoJoystickLibrary
* https://github.com/brianlow/Rotary


# Throttle and Mixture

Throttle is mapped to the Joystick Z-axis. Mixture is mapped to the
X-rot axis. (These are the same as the CH Products flight yoke.)

These elements use linear potentiometers connected to pins A1 and A2
on the Pro Micro. The values are read each time through the loop()
via the built-in ADC. Since the ADC and the Joystick axis both use the
same range (0-1023), we can simply pass the value out as-is.

# Elevator Trim

The trim control is implemented with a 30-step per rotation mechanical rotary encoder connected to digital pins 2 & 3.

For each step of the encoder, the joystick axis value is incremented or decremented by a configurable value, to allow you to configure how fast the trim moves in the sim.