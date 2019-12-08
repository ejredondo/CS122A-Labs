/*	Partner(s) Name & E-mail: Kamal
 *	Lab Section: 021
 *	Assignment: Lab 4  Exercise 3 
 *	Exercise Description: [optional - include for your own benefit]
 *	
 *	I acknowledge all content contained herein, excluding template 
 * 	or example code, is my own original work.
 */


#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/portpins.h>
#include <avr/pgmspace.h>

//FreeRTOS include files
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"
enum LEDState {INIT,L0, L1} led_state;

void LEDS_Init(){
	led_state = INIT;
}


unsigned char tmp;
unsigned char cnt = 0x00;
unsigned char tmp_2;
void LEDS_Tick(){
	//Actions
	switch(led_state){
		case INIT:
		tmp = 0x01;
		PORTD = tmp;
		break;
		
		case L0:
		PORTD = tmp;
		tmp = tmp << 1;
		PORTD = tmp;
		break;
		
		case L1:
		tmp = tmp >> 1;
		PORTD = tmp;
		break;
		
	}
	//Transitions
	switch(led_state){
		case INIT:
		led_state = L0;
		break;
		
		case L0:
		if(~PINA & 0x01) {
			led_state = L1;
		}
		else if(cnt < 6) {
			led_state = L0;
			cnt++;
		}
		else {
			led_state = L1;
		}
		break;
		
		case L1:
		if(~PINA & 0x01) {
			led_state = L0;
		}
		else if(cnt > 0) {
			led_state = L1;
			cnt = cnt - 1;;
		}
		else {
			led_state = L0;
		}
		break;
		
	}
}


void LedSecTask() {
	LEDS_Init();
	for(;;) {
		LEDS_Tick();
		vTaskDelay(300);
	}
}


void StartSecPulse(unsigned portBASE_TYPE Priority) {
	xTaskCreate(LedSecTask,(signed portCHAR *)"LedSecTask",configMINIMAL_STACK_SIZE,NULL,Priority,NULL );

}

int main(void) {
	DDRA = 0x00; PORTA=0xFF;
	DDRD = 0xFF; PORTD = 0x00;
	//Start Tasks
	StartSecPulse(1);
	//RunSchedular
	vTaskStartScheduler();
	return 0;
}

