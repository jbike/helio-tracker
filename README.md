helio-tracker
=============

Dual axis Arduino Heliostat using math for position

video here: http://youtu.be/vtVQ-U-DIPw

This project aims a solar panel at the sun. The structure is foam core board hot glued together. Custom servos consisting of gear motors and a feedback potientiometer track calculated azmuith and elevation of the sun using the Helios library see http://cerebralmeltdown.com/forum/index.php?topic=325.0

The Clock / Calendar library for the DS3231 I2C is http://hacks.ayars.org/2011/04/ds3231-real-time-clock.html I found the Arduino balked at some of the variables, like byte was not accepted while uint8_t was an acceptable substitute in DS3231.cpp Also did not use am/pm variable, century rollover, etc. as UTC 24 hour and calendar was all I needed.

The servos used the custom servo library see http://www.instructables.com/id/Make-your-own-low-cost-servo/?ALLSTEPS

The stock Arduino settings for PWM make an annoying whine for motor control. Therefore, to save your sanity, use the tips for getting 31 KHz phase correct PWM here: http://playground.arduino.cc/Main/TimerPWMCheatsheet

The total current draw was about 81 mA, powered via a 2 cell li-ion battery of 7.4 volts. This was too much draw, and the panel didn't have to be that constantly aligned with the sun. Therefore a power saver circuit was added to power the circuit up for 10 seconds every 5 minutes. The circuit consists of a cmos 555 timer with diode modification for less than 50% duty cycle, and 500 uF capacitor, R1 = 330K, R2 = 2 M. An IRLD110 FET gate connected to the 555's pin 3 switches the power on and off, while a CDS cell connected between Vcc and pin 4 and also pin 4 via 10K to ground allows everyting to be off at night. 
