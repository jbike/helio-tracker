/*
  Helios.h - Library for calculating the solar
  position.
  Copyright (c) 2011 Hannes Hassler.  All right reserved.
  
  adaption 22-7-2012, Arduino 1.0.1 as pointed out by Roman Foltyn

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA  
*/
#ifndef Helios_h
#define Helios_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <math.h>

// Declaration of some constants 
#define pi    3.14159265358979323846
#define twopi (2*pi)
#define rad   (pi/180)
#define dEarthMeanRadius     6371.01	// In km
#define dAstronomicalUnit    149597890	// In km


class Helios
{
  public:
    Helios();        
    void calcSunPos(
        int iYear,
	int iMonth,
	int iDay,
	double dHours,
	double dMinutes,
	double dSeconds,
	double dLongitude,
	double dLatitude);        
	
	double dElapsedJulianDays;
	double dDecimalHours;	
	double dEclipticLongitude;
	double dEclipticObliquity;
	double dRightAscension;
	double dDeclination;	
	double dSin_EclipticLongitude;
	double dMeanLongitude;
	double dMeanAnomaly;
	double dOmega;	
	double dGreenwichMeanSiderealTime;
	double dLocalMeanSiderealTime;
	double dLongitude;
	double dLatitudeInRadians;
	double dHourAngle;
	double dCos_Latitude;
	double dSin_Latitude;
	double dCos_HourAngle;
	double dParallax;
	
	double dAzimuth;
        double dZenithAngle;
        double dElevation;
	
 
   
};




#endif
