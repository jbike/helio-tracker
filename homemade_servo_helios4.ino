/*
Sun tracker aims solar array using Real Time Clock I2C module DS3231. Arduino Pro Mini micro controller.
SN754410 dual H bridge motor driver ic. Elevation motor long shaft 6 volt 10 rpm micro motor with precision 10k,
ball bearing feedback potientiometer. Azimuth motor Tokyo 60 rpm at 12 volts gear micro motor, coupled to 10T/136T
toy helicopter reduction gear, shaft spindle connected to precision 10k feedback potientiometer.
Helios library calculates Azimuth and Elevation. Custom made servos track the Azimuth and Elevation data.
CustomServo library -electro18 (Tanay P.)
pin connection :
motor driver input 1 : pin 5
motor driver input 2 : pin 6
motor driver input2 1 : pin 9 // added second servo -John Mc
motor driver input2 2 : pin 10 //added second servo
Servo feedback pin : analog 0 & analog 2
test pot pin : analog 1 (for calibration)
servo / pot pins to +5 and gnd on arduino
DS3231 library by -Eric Ayars updated & simplified to 24 hr only - John Mc
http://cerebralmeltdown.com/forum/index.php?topic=325.0 SunTracker_Heliostat_Program_V097.zip Gabriel Miller
Compare against online sun tracker:
http://www.sunearthtools.com/dp/tools/pos_sun.php
Faster PWM for quiet operation:
http://playground.arduino.cc/Main/TimerPWMCheatsheet
And modified the line in the wiring.c function in the Arduino program files
hardware\arduino\cores\arduino\wiring.c :
#define MICROSECONDS_PER_TIMER0_OVERFLOW (clockCyclesToMicroseconds(1 * 510))
*/
#include <Wire.h> // for I2C communication with clock module
#include <Helios.h> // calculates sun Azimuth and Elevation
#include <DS3231.h> // I2C clock module library
#include<CustomServo.h> // custom servo library
Helios helios;
DS3231 Clock;
double dAzimuth;
double dElevation;
int angle; // angle by which the servo will rotate
CServo servo1(5,6,0); // initialize the servo with CServo myservo( pin 1 , pin 2 , analog feedback pin ) pin 1,2 may be analogWrite or digitalWrite depending on direction
CServo servo2(9,10,2);
void setup()
{
TCCR0A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM00);
TCCR0B = _BV(CS00); //phase correct 31 kHz, for pwm pins 5&6
// and
TCCR1B = TCCR1B & 0b11111000 | 0x01; //phase correct 31 kHz, for pins 9 and 10
// Start the I2C interface
Wire.begin();
Serial.begin(9600); // setup serial connection
// assumes clock was previously set, see DS3231_set (also part of DS3231 library examples) to set the time
}
void loop()
{
int Year=Clock.getYear();
int Month=Clock.getMonth();
int Date=Clock.getDate();
int DoW=Clock.getDoW();
int Hour=Clock.getHour();
int Minute=Clock.getMinute();
int Second=Clock.getSecond();
Serial.print("UT ");
Serial.print(Hour, DEC);
Serial.print(":");
Serial.print(Minute, DEC);
Serial.print(":");
Serial.print(Second, DEC);
Serial.print(" ");
Serial.print(Month, DEC);
Serial.print("/");
Serial.print(Date, DEC);
Serial.print("/");
Serial.print(Year, DEC);
Serial.print(" Day_of_week:");
Serial.println(DoW, DEC);
helios.calcSunPos(Year,Month,Date,Hour,Minute,Second,-118.2500,34.0500);
dAzimuth=helios.dAzimuth;
dElevation=helios.dElevation;
//delay(5000);
//int sensorValue = analogRead(A1);
//int sensorValue2 = analogRead(A1);
//int potval = analogRead(A1);
int sensorValue = dElevation;
if (sensorValue < 0)
{
sensorValue =90; //park panel overhead at night
}
int sensorValue2 = dAzimuth;
sensorValue2 = sensorValue2 + 36; // set the offset of the azimuth servo
sensorValue2 = map(sensorValue2,360,0,0,165); // set the range of azimuth servo
sensorValue = sensorValue + 12; // set the offset of the elevation servo
sensorValue = map(sensorValue,170,0,0,85); // set the range of elevation servo
servo1.drive(sensorValue); // drive the servo using myservo.drive( angle ) where 0 <= angle <= 160
servo2.drive(sensorValue2);
//Serial.println(potval);
//Serial.println(sensorValue); // print the current angle
Serial.println(dElevation,2);
Serial.println(dAzimuth,2);
// Serial.println(sensorValue2);
}
