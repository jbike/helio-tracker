/*
 * DS3231.h
 *
 * Arduino Library for the DS3231 Real-Time Clock chip
 *
 * (c) Eric Ayars
 * 4/1/11
 * released into the public domain. If you use this, please let me know
 * (just out of pure curiosity!) by sending me an email:
 * eric@ayars.org
 *
 */

#ifndef DS3231_h
#define DS3231_h

#include <WProgram.h>
#include <Wire.h>

class DS3231 {
	public:
			
		//Constructor
		DS3231();

		// Time-retrieval functions
	
		// the get*() functions retrieve current values of the registers.
		// If you only need one element, use that one for simplicity; but 
		// if you need the whole passel then use getTime() to avoid
		// the chance of rollover between reads of the different components.
		void getTime(uint8_t& year, uint8_t& month, uint8_t& date, uint8_t& DoW, uint8_t& hour, uint8_t& minute, uint8_t& second); 
		uint8_t getSecond(); 
		uint8_t getMinute(); 
		uint8_t getHour();   //uint8_t getHour(bool& h12, bool& PM);
			// In addition to returning the hour register, this function
			// returns the values of the 12/24-hour flag and the AM/PM flag.
		uint8_t getDoW(); 
		uint8_t getDate(); 
		uint8_t getMonth();   //uint8_t getMonth(bool& Century);
			// Also sets the flag indicating century roll-over.
		uint8_t getYear(); 
			// Last 2 digits only

		// Time-setting functions
		// Note that none of these check for sensibility: You can set the
		// date to July 42nd and strange things will probably result.
		
		void setSecond(uint8_t Second); 
			// In addition to setting the seconds, this clears the 
			// "Oscillator Stop Flag".
		void setMinute(uint8_t Minute); 
			// Sets the minute
		void setHour(uint8_t Hour); 
			// Sets the hour
		void setDoW(uint8_t DoW); 
			// Sets the Day of the Week (1-7);
		void setDate(uint8_t Date); 
			// Sets the Date of the Month
		void setMonth(uint8_t Month); 
			// Sets the Month of the year
		void setYear(uint8_t Year); 
			// Last two digits of the year
		void setClockMode(bool h12); 
			// Set 12/24h mode. True is 12-h, false is 24-hour.

		// Temperature function

		float getTemperature(); 

		// Alarm functions
		
		void getA1Time(uint8_t& A1Day, uint8_t& A1Hour, uint8_t& A1Minute, uint8_t& A1Second, uint8_t& AlarmBits, bool& A1Dy, bool& A1h12, bool& A1PM); 
/* Retrieves everything you could want to know about alarm
 * one. 
 * A1Dy true makes the alarm go on A1Day = Day of Week,
 * A1Dy false makes the alarm go on A1Day = Date of month.
 *
 * uint8_t AlarmBits sets the behavior of the alarms:
 *	Dy	A1M4	A1M3	A1M2	A1M1	Rate
 *	X	1		1		1		1		Once per second
 *	X	1		1		1		0		Alarm when seconds match
 *	X	1		1		0		0		Alarm when min, sec match
 *	X	1		0		0		0		Alarm when hour, min, sec match
 *	0	0		0		0		0		Alarm when date, h, m, s match
 *	1	0		0		0		0		Alarm when DoW, h, m, s match
 *
 *	Dy	A2M4	A2M3	A2M2	Rate
 *	X	1		1		1		Once per minute (at seconds = 00)
 *	X	1		1		0		Alarm when minutes match
 *	X	1		0		0		Alarm when hours and minutes match
 *	0	0		0		0		Alarm when date, hour, min match
 *	1	0		0		0		Alarm when DoW, hour, min match
 */
		void getA2Time(uint8_t& A2Day, uint8_t& A2Hour, uint8_t& A2Minute, uint8_t& AlarmBits, bool& A2Dy, bool& A2h12, bool& A2PM); 
			// Same as getA1Time();, but A2 only goes on seconds == 00.
		void setA1Time(uint8_t A1Day, uint8_t A1Hour, uint8_t A1Minute, uint8_t A1Second, uint8_t AlarmBits, bool A1Dy, bool A1h12, bool A1PM); 
			// Set the details for Alarm 1
		void setA2Time(uint8_t A2Day, uint8_t A2Hour, uint8_t A2Minute, uint8_t AlarmBits, bool A2Dy, bool A2h12, bool A2PM); 
			// Set the details for Alarm 2
		void turnOnAlarm(uint8_t Alarm); 
			// Enables alarm 1 or 2 and the external interrupt pin.
			// If Alarm != 1, it assumes Alarm == 2.
		void turnOffAlarm(uint8_t Alarm); 
			// Disables alarm 1 or 2 (default is 2 if Alarm != 1);
			// and leaves the interrupt pin alone.
		bool checkAlarmEnabled(uint8_t Alarm); 
			// Returns T/F to indicate whether the requested alarm is
			// enabled. Defaults to 2 if Alarm != 1.
		bool checkIfAlarm(uint8_t Alarm); 
			// Checks whether the indicated alarm (1 or 2, 2 default);
			// has been activated.

		// Oscillator functions

		void enableOscillator(bool TF, bool battery, uint8_t frequency); 
			// turns oscillator on or off. True is on, false is off.
			// if battery is true, turns on even for battery-only operation,
			// otherwise turns off if Vcc is off.
			// frequency must be 0, 1, 2, or 3.
			// 0 = 1 Hz
			// 1 = 1.024 kHz
			// 2 = 4.096 kHz
			// 3 = 8.192 kHz (Default if frequency uint8_t is out of range);
		void enable32kHz(bool TF); 
			// Turns the 32kHz output pin on (true); or off (false).
		bool oscillatorCheck();;
			// Checks the status of the OSF (Oscillator Stop Flag);.
			// If this returns false, then the clock is probably not
			// giving you the correct time.
			// The OSF is cleared by function setSecond();.

	private:

		uint8_t decToBcd(uint8_t val); 
			// Convert normal decimal numbers to binary coded decimal
		uint8_t bcdToDec(uint8_t val); 
			// Convert binary coded decimal to normal decimal numbers
		uint8_t readControluint8_t(bool which); 
			// Read selected control uint8_t: (0); reads 0x0e, (1) reads 0x0f
		void writeControluint8_t(uint8_t control, bool which); 
			// Write the selected control uint8_t. 
			// which == false -> 0x0e, true->0x0f.

};

#endif
