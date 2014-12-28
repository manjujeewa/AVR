#define F_CPU 16000000UL 
#define USART_BAUDRATE 9600   // Define baud rate
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)


#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "UART_routines.h"
#include "RTC_routines.h"
#include "i2c_routines.h"
#include "SPI_routines.h"
#include "SD_routines.h"
#include "SYSTEM_functions.h"
#include "FAT32.h"

int main(void){
	_delay_ms(1000);
	unsigned char error, i;
	port_init();
   USART_Init(BAUD_PRESCALE);  // Initialise USART
   twi_init();
   spi_init();
   
   
   for(;;){    // Repeat indefinitely
   
   transmitString_F (PSTR("\n\r\n\r****************************************************"));
  transmitString_F (PSTR("\n\r         Weather Station Data Storage Testing "));
  transmitString_F (PSTR("\n\r****************************************************\n\r"));
             
  cardType = 0;
	
  for (i=0; i<10; i++){
  	error = SD_init();
  	if(!error) break;
  }
  

  if(error){
  	if(error == 1) transmitString_F(PSTR("SD card not detected.."));
  	if(error == 2) transmitString_F(PSTR("Card Initialization failed.."));
  
  	blinkRedLED();
  }

switch (cardType)
  {
  	case 1:transmitString_F(PSTR("Standard Capacity Card (Ver 1.x) Detected!"));
  		   break;
  	case 2:transmitString_F(PSTR("High Capacity Card Detected!"));
  		   break;
  	case 3:transmitString_F(PSTR("Standard Capacity Card (Ver 2.x) Detected!"));
  		   break;
  	default:transmitString_F(PSTR("Unknown SD Card Detected!"));
  		   break; 
  }
  
  error = getBootSectorData (); //read boot sector and keep necessary data in global variables
  if(error) 
  {
   	transmitString_F (PSTR("\n\rFAT32 not found!"));  //FAT32 incompatible drive
   	blinkRedLED();
  }
  
	
	TX_NEWLINE;
  	findFiles(GET_LIST,0);
	while(1);
             
		         // delay just to stop Hyperterminal screen cluttering up    
   }
}

int recordData (void){

	unsigned char option, error, i, j, data, channel;
	
	error = RTC_getDate();
		if(error) blinkRedLED();
	
	j=0;
	
	for(i=0; i<8; i++){
		fileName[i] = date[j++];
		if(j==2 || j==5) j++;	//excluding the '/' character from date in the fileName
	}
	
	fileName[8] = '.';
	fileName[9] = 'C';
	fileName[10] = 'S';
	fileName[11] = 'V';
	
	error = RTC_getTime();
    	if(error) blinkRedLED();
		
		
	for(i=0; i<10; i++) dataString[i] = date[i];
	dataString[i++] = ' ';

	for(j=0;j<8; j++)  dataString[i++] = time[j];
	dataString[i++] = ' ';
	
	
	humidity = getHumidity();
	temperature = getTemperature();
	luminance = getLuminance();
	pressure = getPressure();
	w_direction = getW_direction();
	w_speed = getW_speed();
	
	
	for(j=0;j<7; j++)  dataString[i++] = temperature[j];

}


























