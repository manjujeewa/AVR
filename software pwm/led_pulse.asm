; Copyright (c) 2008, Mats Mattsson
;
; Permission to use, copy, modify, and/or distribute this software for any
; purpose with or without fee is hereby granted, provided that the above
; copyright notice and this permission notice appear in all copies.
;
; THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
; WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
; MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
; ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
; WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
; ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
; OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

.device ATtiny26

.equ PORTA = $1B
.equ DDRA = $1A
.equ TCCR0 = $33
.equ TCNT0 = $32
.equ TIMSK = $39
.equ SREG = $3F
.equ SP = $3D


.def pwm_counter = r16
.def pinoutput = r17
.def temp0 = r20
.def delay = r21
.def temp2 = r22
.def arg0 = r24
.def arg1 = r25
.def arg2 = r26

.def led_intensity_0 = r0
.def led_intensity_1 = r1
.def led_intensity_2 = r2
.def led_intensity_3 = r3
.def led_intensity_4 = r4
.def led_intensity_5 = r5
.def led_intensity_6 = r6
.def led_intensity_7 = r7
.def led_counter_0 = r8
.def led_counter_1 = r9
.def led_counter_2 = r10
.def led_counter_3 = r11
.def led_counter_4 = r12
.def led_counter_5 = r13
.def led_counter_6 = r14
.def led_counter_7 = r15







RJMP main
RETI ; RJMP EXT_INT0
RETI ; RJMP PIN_CHANGE
RETI ; rjmp TIM1_CMP1A ; Timer1 compare match 1A 
RETI ; rjmp TIM1_CMP1B ; Timer1 compare match 1B 
RETI ; rjmp TIM1_OVF ; Timer1 overflow handler 
RJMP TIMER0_OVERFLOW
RETI ; rjmp USI_STRT ; USI Start handler 
RETI ; rjmp USI_OVF ; USI Overflow handler 
RETI ; rjmp EE_RDY ; EEPROM Ready handler 
RETI ; rjmp ANA_COMP ; Analog Comparator handler 
RETI ; rjmp ADC ; ADC Conversion Handler 



main:
    ; initialize things
    LDI temp0, $FF
	OUT DDRA, temp0
    LDI pwm_counter, $00
    LDI temp0, $01
    OUT TCCR0, temp0 ; start timer0
    LDI temp0, $02
    OUT TIMSK, temp0 ; enable timer0 overflow interrupt
	LDI temp0, $7F
	OUT SP, temp0; init stack pointer
	LDI temp0, $80
	OUT SREG, temp0

main_loop:

.macro update_led
	LDI arg0, @0
	MOV arg1, @1
	MOV arg2, @2
	RCALL update_intensity
	MOV @1, arg1
	MOV @2, arg2
.endm
update_led 3, led_counter_0, led_intensity_0
update_led 5, led_counter_1, led_intensity_1
update_led 7, led_counter_2, led_intensity_2
update_led 11, led_counter_3, led_intensity_3
update_led 13, led_counter_4, led_intensity_4
update_led 17, led_counter_5, led_intensity_5
update_led 19, led_counter_6, led_intensity_6
update_led 23, led_counter_7, led_intensity_7

	LDI delay, $FF
delay_loop:
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	DEC delay
	BRNE delay_loop

	RJMP main_loop


update_intensity:

	; arg0 = period
	; arg1 = counter
	; arg2 = intensity
	;
	; 8 cycle macro
.macro change_intensity_macro
.endm

	INC arg1
	CP arg1, arg0
	BRCC ui_do_update
	NOP ; 1
	NOP ; 2
	NOP ; 3
	NOP ; 4
	NOP ; 5
	NOP ; 6
	NOP ; 7
	NOP ; 8
	RJMP ui_end
ui_do_update:
	CLR arg1
		CPI arg2, $80 ; carry = @0 < $80
	    BRCC cim_decrease
		INC arg2
		CPI arg2, $80
		BRCS cim_increase_1
		LDI arg2, $FE
	cim_increase_1:
		RJMP cim_end

	cim_decrease: ; $0 >= $80
		DEC arg2
		CPI arg2, $81
		BRCC cim_decrease_1
		LDI arg2, $00
	cim_decrease_1:
		NOP ; make branches equal length, so decreasing is no faster than increasing
	cim_end:
	change_intensity_macro arg2

ui_end:
	RET



TIMER0_OVERFLOW:
	IN temp0, SREG ; save Status register, as interrupt may be between CP and branch instruction
	PUSH temp0
	.macro generate_pinout_for_led
	    LSL  @0
	    ROR  temp0
	    CP   pwm_counter, @0
	    ROL  pinoutput
	    ROL  temp0
	    ROR  @0
	.endm
    generate_pinout_for_led led_intensity_0
    generate_pinout_for_led led_intensity_1
    generate_pinout_for_led led_intensity_2
    generate_pinout_for_led led_intensity_3
    generate_pinout_for_led led_intensity_4
    generate_pinout_for_led led_intensity_5
    generate_pinout_for_led led_intensity_6
    generate_pinout_for_led led_intensity_7
    out PORTA, pinoutput
	INC pwm_counter
	INC pwm_counter
	; increase timer a bit, for higher pwm frequency
	IN temp0, TCNT0
	LDI temp2, 164
	ADD temp0, temp2
	OUT TCNT0, temp0
	; restore Status Register
	POP temp0
	OUT SREG, temp0
	RETI

