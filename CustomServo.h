#ifndef CustomServo_h
#define CustomServo_h
#include "Arduino.h"

class CServo
{
public :
	CServo(int pin1 , int pin2 , int feedbkpin);
	
	void drive(int ang);
private :
	int _feedbkpin;
	int _pin1;
	int _pin2;
	
	
};

#endif
