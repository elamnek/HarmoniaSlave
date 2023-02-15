# Harmonia Slave
This code is run on the slave microcontroller (Arduino Leonardo):

https://www.jaycar.com.au/duinotech-leonardo-tiny-atmega32u4-development-board/p/XC4431

The Leonardo connects to the Arduino Mega and is used for reading sensor data including:

a hall effect rpm sensor:
(more information to come) 

an internal pressure sensor:
https://www.jaycar.com.au/barometric-pressure-sensor-module/p/XC3702

a pressure sensor connected to the air-bag:
https://core-electronics.com.au/adafruit-mprls-ported-pressure-sensor-breakout-0-to-25-psi.html

and a power sensor:
https://core-electronics.com.au/gravity-i2c-digital-wattmeter.html

The slave microcontroller collects sensor data from the above sensors which are all located in the aft pressure hull section, and provides them to them main micorcontroller at 0.5Hz via serial (as a comma separated text string)

Please refer to Wiki for more detailed information:
https://github.com/elamnek/Harmonia/wiki

Note: the Harmonia submarine is part of a University of Adelaide research project being carried out between mid 2022 and mid 2023 - this repository will be continually updated over the next few months with the latest code as it is developed. Final release will be published in mid 2023.
