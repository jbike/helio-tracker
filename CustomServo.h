#include"Arduino.h"
#include"CustomServo.h"

CServo::CServo(int pin1, int pin2, int feedbkpin)
{

	pinMode(pin1,OUTPUT);
	pinMode(pin2,OUTPUT);
	_feedbkpin = feedbkpin;
	_pin1 = pin1;
	_pin2 = pin2;
	
}

void CServo::drive(int ang)
{
int _feedbk = analogRead(_feedbkpin);
_feedbk=map(_feedbk,0,1020,0,160);
ang=constrain(ang,0,160);
int _diff = _feedbk-ang;
if(_diff>0)
_diff=40*_diff;     // factor controls sensitivity
if(_diff<0)
_diff=-40*_diff;    // factor controls sensitivity
_diff= constrain(_diff,0,250);   
 
if(_diff>0)
{
  if(ang>_feedbk)
 {
digitalWrite(_pin1,0);
  analogWrite(_pin2,_diff); 
}
  if(ang<_feedbk)
  {
  analogWrite(_pin1,_diff);  // was _diff
  digitalWrite(_pin2,0);
}

}
if(_diff<1)  //was 1
{
  digitalWrite(_pin1,0);
  digitalWrite(_pin2,0);
}
}
