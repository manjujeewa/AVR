# include<avr/io.h>
# include<util/delay.h>
# include "lcd.h"

unsigned char ch;

void init_i2c()
{
	TWSR = 0X00; // Setting prescalar bits
	TWBR = 0X47; // Bit rate
	TWCR = (1<<TWEN);
}

void start(){
	// Clear TWI interrupt flag, Put start condition on SDA, Enable TWI
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	// Wait till start condition is transmitted
	while((TWCR &(1<<TWINT))==0); 
}

unsigned char read_i2c(){
	TWCR = (1<<TWINT)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT))); 
	return TWDR;
}

void write_i2c(unsigned char ch){
	TWDR = ch;	// put data in TWDR
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while(!(TWCR&(1<<TWINT)));
}

void stop(){
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	_delay_ms(1);
}

void rtc_write(char dev_addr,char dev_loc,char dev_data){
	start();
	write_i2c(dev_addr);
	write_i2c(dev_loc);
	write_i2c(dev_data);
	stop();
	_delay_ms(10);
}

unsigned char rtc_read(char dev_addr,char dev_loc){
	char ch;

	start();
	write_i2c(dev_addr);
	write_i2c(dev_loc);
	start();
	write_i2c(dev_addr|0x01);
	ch = read_i2c();
	stop();
	
	return ch;
}


unsigned char getDate(){
	unsigned char *date;
	date[1] = rtc_read(0xd0 , 0x04)/16+48;
	date[2] = rtc_read(0xd0 , 0x04)%16+48;
	date[3] = rtc_read(0xd0 , 0x05)/16+48;
	date[4] = rtc_read(0xd0 , 0x05)%16+48;
	date[5] = rtc_read(0xd0 , 0x06)/16+48;
	date[6] = rtc_read(0xd0 , 0x06)%16+48;
	return date;
}

unsigned char getTime(){
	unsigned char *time;
	time[1] = rtc_read(0xd0 , 0x02)/16+48;
	time[2] = rtc_read(0xd0 , 0x02)%16+48;
	time[3] = rtc_read(0xd0 , 0x01)/16+48;
	time[4] = rtc_read(0xd0 , 0x01)%16+48;
	time[5] = rtc_read(0xd0 , 0x00)/16+48;
	time[6] = rtc_read(0xd0 , 0x00)%16+48;
	return time;
}

int main(){
	DDRC =0XFF;
	init_i2c();

	rtc_write(0xd0,0x00,0x27);
	rtc_write(0xd0,0x01,0x27);
	rtc_write(0xd0,0x02,0x08);

	rtc_write(0xd0,0x04,0x27);
	rtc_write(0xd0,0x05,0x09);
	rtc_write(0xd0,0x06,0x13);

	//_delay_ms(10000); 

	/* initialize display, cursor off */
	lcd_init(LCD_DISP_ON);

	while(1){
			lcd_clrscr();
			lcd_puts(getTime());
			_delay_ms(100);
	}

}