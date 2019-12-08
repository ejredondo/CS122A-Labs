/*	Partner(s) Name & E-mail: Kamal
 *	Lab Section: 021
 *	Assignment: Lab 4  Exercise 2 
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
enum LEDState {INIT,L0} led_state;
enum LEDState_1 {INIT_1,L1} led_state_1;
enum LEDState_2 {INIT_2,L2} led_state_2;

void LEDS_Init(){
	led_state = INIT;
}

void LEDS_Init_1(){
	led_state_1 = INIT_1;
}

void LEDS_Init_2(){
	led_state_2 = INIT_2;
}

unsigned char tmp;
unsigned char tmp_1;
unsigned char tmp_2;
void LEDS_Tick(){
	//Actions
	switch(led_state){
		case INIT:
		tmp = 0x00;
		PORTD = tmp | tmp_1 | tmp_2;
		break;
		case L0:
		tmp = 0x01;
		PORTD = tmp | tmp_1 | tmp_2;
		break;
		default:
		tmp = 0x00;
		PORTD = tmp |tmp_1 | tmp_2;
		break;
	}
	//Transitions
	switch(led_state){
		case INIT:
		led_state = L0;
		break;
		case L0:
		led_state = INIT;
		break;
		default:
		led_state = INIT;
		break;
			}
}

void LEDS_Tick_1(){
	//Actions
	switch(led_state_1){
		case INIT_1:
		tmp_1 = 0x00;
		PORTD = tmp_1 | tmp| tmp_2;
		break;
		case L1:
		tmp_1 = 0x04;
		PORTD = tmp_1 | tmp| tmp_2;
		break;
		default:
		tmp_1 = 0x00;
		PORTD = tmp_1 | tmp| tmp_2;
		break;
	}
	//Transitions
	switch(led_state_1){
		case INIT_1:
		led_state_1 = L1;
		break;
		case L1:
		led_state_1 = INIT_1;
		break;
		default:
		led_state_1 = INIT_1;
		break;
		
	}
}

void LEDS_Tick_2(){
	//Actions
	switch(led_state_2){
		case INIT_2:
		tmp_2 = 0x00;
		PORTD = tmp_2 | tmp_1 | tmp;
		break;
		case L2:
		tmp_2 = 0x10;
		PORTD = tmp_2 | tmp_1 | tmp;
		break;
		default:
		tmp_2 = 0x00;
		PORTD = tmp_2 | tmp_1 | tmp;
		break;
	}
	//Transitions
	switch(led_state_2){
		case INIT_2:
		led_state_2 = L2;
		break;
		case L2:
		led_state_2 = INIT_2;
		break;
		default:
		led_state_2 = INIT_2;
		break;
		
	}
}



void LedSecTask() {
	LEDS_Init();
	for(;;) {
		LEDS_Tick();
		vTaskDelay(500);
	}
}
void LEDSecTask_1() {
	LEDS_Init_1();
	for(;;) {
		LEDS_Tick_1();
		vTaskDelay(1000);
	}
}
void LEDSecTask_2() {
	LEDS_Init_2();
	for(;;) {
		LEDS_Tick_2();
		vTaskDelay(2500);
	}
}

void StartSecPulse(unsigned portBASE_TYPE Priority) {
	xTaskCreate(LedSecTask,(signed portCHAR *)"LedSecTask",configMINIMAL_STACK_SIZE,NULL,Priority,NULL );
	
	xTaskCreate(LEDSecTask_1,(signed portCHAR *)"LedSecTask_1",configMINIMAL_STACK_SIZE,NULL,Priority,NULL );
	
	xTaskCreate(LEDSecTask_2, (signed portCHAR *)"LedSecTask_2",configMINIMAL_STACK_SIZE,NULL,Priority,NULL );
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

