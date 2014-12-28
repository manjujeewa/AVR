/*lcd_clrscr();
		double c1 = get_cs1_voltage();
		dtostrf(c1, 4, 2, printbuff);
		lcd_puts(printbuff);
		_delay_ms(30);*/
		
        // increasing brightness
		
        for (brightness = 0; brightness < 255; ++brightness)
        {
            // set the brightness as duty cycle
            OCR0++;
			//increase_duty(0);
			//increase_duty(2);
            // delay so as to make the user "see" the change in brightness
			
			display_Output_voltage();
			lcd_puts("++");
            _delay_ms(30);
        }
  
        // decreasing brightness
        for (brightness = 255; brightness > 0; --brightness)
        {
            // set the brightness as duty cycle
			OCR0--;
            //decrease_duty(0);
			//decrease_duty(2);
            // delay so as to make the user "see" the change in brightness
			
			display_Output_voltage();
			lcd_puts("--");
            _delay_ms(30);
        }
		
		/*
		lcd_clrscr();
		adc_result0 = adc_read(3);
		itoa(adc_result0, int_buffer, 10);
		lcd_puts(int_buffer);
		_delay_ms(30);*/
		 
		/*
		
		lcd_gotoxy(8,0);
		
		double c1 = get_cs1_voltage();
		dtostrf(c1, 4, 2, printbuff);
		lcd_puts(printbuff);
		lcd_gotoxy(0,1);
		
		double d = acs712_getcurrent(c1, 4.93);
		if(d < 0.0)
			d=-d;
		dtostrf(d, 4, 2, printbuff);
		lcd_puts(printbuff);
		
		//lcd_puts("abcdefghijklm");
		_delay_ms(1000);
		lcd_clrscr();
		*/
		/*
		while(1){
		
			lcd_clrscr();
			lcd_puts("p4");
			// PORTD ^= 1  << PORTD4;
			_delay_ms(1000);
			
			lcd_clrscr();
			lcd_puts("p5");
			PORTD ^= 1  << PORTD5;
			_delay_ms(1000);

		}*/
    // repeat this forever