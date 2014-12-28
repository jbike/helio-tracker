/*
  Helios.cpp- 
  Copyright (c) 2011 Hannes Hassler.  All rights reserved.
  
  Hannes Hassler, 22-7-2012 adaption to Arduino 1.0.1, as pointed out by Roman Foltyn

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
  
  This library can be used for calculating the solar position on Arduino.  
  
  The algorithm is an adaption from
  the "PSA" solar positioning algorithm, as documented in:
  
  Blanco-Muriel et al.: Computing the Solar Vector. Solar Energy Vol 70 No 5 pp 431-441.
  http://dx.doi.org/10.1016/S0038-092X(00)00156-0
  
  According to the paper, "The algorithm allows .. the true solar vector 
  to be determined with an accuracy of 0.5
  minutes of arc for the period 1999–2015.
  The original code has been downloaded from
  http://www.psa.es/sdg/sunpos.htm
  
  Adaptions:
  Modified calculation of number of Days since 1.Jan 2000 (dJulianDate-2451545.0)
  Neccessary because of the limited double precision on Arduino
  (double has the same precision as float on the current Arduino (2011))
  It should be used only for dates between 1.1.2000 and 31.12.2100
  (PSA itself has garantueed accuracy only until 2015)
  
*/

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "Helios.h"
#include <math.h>


Helios::Helios(){}


void Helios::calcSunPos(
        int iYear,
	int iMonth,
	int iDay,
	double dHours,
	double dMinutes,
	double dSeconds,
	double dLongitude,
	double dLatitude)
{
	
	
	// Calculate difference in days between the current Julian Day 
	// and JD 2451545.0, which is noon 1 January 2000 Universal Time
	{
		  
	        //double dJulianDate;
		//long int liAux1;
		//long int liAux2;
		// Calculate time of the day in UT decimal hours
		dDecimalHours = dHours + (dMinutes 
			+ dSeconds / 60.0 ) / 60.0;
			
		// Calculate current Julian Day
		/* original calculation; does not work
		   for Arduino, because double precision is the same 
		   same as float.
		liAux1 =(iMonth-14)/12;
		liAux2=(1461*(iYear + 4800 + liAux1))/4 + (367*(iMonth 
			- 2-12*liAux1))/12- (3*((iYear + 4900 
		+ liAux1)/100))/4+iDay-32075;
		dJulianDate=(double)(liAux2)-0.5+dDecimalHours/24.0;
		// Calculate difference between current Julian Day and JD 2451545.0 
		dElapsedJulianDays = dJulianDate-2451545.0;
		*/
		
		long int iYfrom2000=iYear;//expects now as YY (from 2000)-2000;
		long int iA=(14-(iMonth))/12;
		long int iM=(iMonth)+12*iA-3;
		
		long int liAux3=(153*iM+2)/5;
		long int liAux4=365*(iYfrom2000-iA);
		long int liAux5=(iYfrom2000-iA)/4;
		
		
		dElapsedJulianDays=(double)(iDay+liAux3+liAux4+liAux5+59)+
		                    -0.5+dDecimalHours/24.0;
		
	}

	// Calculate ecliptic coordinates (ecliptic longitude and obliquity of the 
	// ecliptic in radians but without limiting the angle to be less than 2*Pi 
	// (i.e., the result may be greater than 2*Pi)
	{
		
	        /*double dMeanLongitude;
		double dMeanAnomaly;
		double dOmega;*/
		
		dOmega=2.1429-0.0010394594*dElapsedJulianDays;
		dMeanLongitude = 4.8950630+ 0.017202791698*dElapsedJulianDays; // Radians
		dMeanAnomaly = 6.2400600+ 0.0172019699*dElapsedJulianDays;
		dEclipticLongitude = dMeanLongitude + 0.03341607*sin( dMeanAnomaly ) 
			+ 0.00034894*sin( 2*dMeanAnomaly )-0.0001134
			-0.0000203*sin(dOmega);
		dEclipticObliquity = 0.4090928 - 6.2140e-9*dElapsedJulianDays
			+0.0000396*cos(dOmega);
	}

	// Calculate celestial coordinates ( right ascension and declination ) in radians 
	// but without limiting the angle to be less than 2*Pi (i.e., the result may be 
	// greater than 2*Pi)
	{
		
		dSin_EclipticLongitude= sin( dEclipticLongitude );
		double dY1 = cos( dEclipticObliquity ) * dSin_EclipticLongitude;
		double dX1 = cos( dEclipticLongitude );
		dRightAscension = atan2( dY1,dX1 );
		if( dRightAscension < 0.0 ) dRightAscension = dRightAscension + twopi;
		dDeclination = asin( sin( dEclipticObliquity )*dSin_EclipticLongitude );
	}

	// Calculate local coordinates ( azimuth and zenith angle ) in degrees
	{
		
		dGreenwichMeanSiderealTime = 6.6974243242 + 
			0.0657098283*dElapsedJulianDays 
			+ dDecimalHours;
						
		dLocalMeanSiderealTime = (dGreenwichMeanSiderealTime*15 
			+ dLongitude)*rad;
		dHourAngle = dLocalMeanSiderealTime - dRightAscension;
		dLatitudeInRadians = dLatitude*rad;
		dCos_Latitude = cos( dLatitudeInRadians );
		dSin_Latitude = sin( dLatitudeInRadians );
		dCos_HourAngle= cos( dHourAngle );
		dZenithAngle = (acos( dCos_Latitude*dCos_HourAngle
		*cos(dDeclination) + sin( dDeclination )*dSin_Latitude));
		double dY = -sin( dHourAngle );
		double dX = tan( dDeclination )*dCos_Latitude - dSin_Latitude*dCos_HourAngle;
		dAzimuth=atan2( dY, dX );
		if ( dAzimuth < 0.0 ) 
			dAzimuth = dAzimuth + twopi;
		dAzimuth = dAzimuth/rad;		
		// Parallax Correction		
		dParallax=(dEarthMeanRadius/dAstronomicalUnit)
			*sin(dZenithAngle);
		dZenithAngle=(dZenithAngle 
			+ dParallax)/rad;
		dElevation=90-dZenithAngle;
	}	
}
