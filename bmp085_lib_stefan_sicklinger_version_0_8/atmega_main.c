//#################################################################################
//#################################################################################
//#################################################################################
/*	Library to use BMP085 with ATMEL Atmega family
	
	Copyright (C) 2010 Stefan Sicklinger

	For support check out http://www.sicklinger.com
    
	This library is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

/*  Tested with STK-600 Atmega32 @ 8MHz */

/* Error codes:
   100 complete time out
   10  TWI wrong status
   0   no error
*/
	
//#################################################################################
//#################################################################################
//#################################################################################
#include "atmega_bmp085_lib.h"
#include "uart.h"
//#########################################
//#########################################
//#########################################
// S T A R T  M A I N  P R O G R A M 
int main(void)
{	

//-----------------------------------------
// Allocate memory
//-----------------------------------------	
	int32_t temperature = 0;
	int32_t pressure = 0;
	int16_t BMP085_calibration_int16_t[8];
	int16_t BMP085_calibration_uint16_t[3];
	uint8_t error_code=0;
//-----------------------------------------
// Initialize Stuff
//-----------------------------------------
	uart_init();				//Initialize UART0
	_delay_ms(11);
	i2cSetBitrate(1000);		//Initialize TWI 1000kHz	
	BMP085_Calibration(BMP085_calibration_int16_t, BMP085_calibration_uint16_t,&error_code);////Initialize BMP085
//-----------------------------------------
// Do forever
//-----------------------------------------
		
		printf("error code global init: %d \n",error_code);
    while(1)
    {
		
		printf("------------------------\n");
		printf("------------------------\n");
/*		printf("Pressure raw %ld \n",bmp085ReadPressure(&error_code));
		printf("error code global: %d \n",error_code);
		printf("------------------------\n");
		printf("------------------------\n");
		printf("Temperature raw %ld \n",bmp085ReadTemp(&error_code));
		printf("error code global: %d \n",error_code);
		printf("------------------------\n");*/
		printf("------------------------\n");
		bmp085Convert(BMP085_calibration_int16_t, BMP085_calibration_uint16_t,&temperature, &pressure,&error_code);
		printf("Temperature: %ld (in 0.1 deg C)\n", temperature);
		printf("Pressure: %ld Pa\n\n", pressure);	
		printf("Altitude: %ld dm\n", bmp085CalcAltitude(pressure));		
		printf("error code global: %d \n",error_code);
		printf("------------------------\n");
		printf("------------------------\n");
		_delay_ms(10);		
    }


    return 0;
}
// E N D  M A I N  P R O G R A M 
//#########################################
//#########################################
//#########################################



