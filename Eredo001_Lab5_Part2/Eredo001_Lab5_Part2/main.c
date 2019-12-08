/*
 * Eredo001_Lab5_Part2.c
 *
 * Created: 10/23/2019 4:01:42 PM
 * Author : Ered5
 */ 

#include <avr/io.h>
#include <bit.h>
#include <timer.h>
#define button (~PINA & 0x03)
#define button1 (~PINA & 0x0C)
const unsigned char next = (PINA1);
const unsigned char prev = (PINA2);
const unsigned char Off = (PINA1) | (PINA2);

const unsigned char next1 = (PINA3);
const unsigned char prev1 = (PINA4);
const unsigned char Off1 = (PINA3) | (PINA4);

void transmit_data(unsigned char data) {
	for(unsigned char i = 0; i < 8; i++) {
		PORTC = 0x08;
		PORTC |= ((data >> i) & 0x01);
		PORTC |= 0x04;
	}
	PORTC |= 0x02;
	PORTC = 0x00;
}

unsigned char tmp;
unsigned char Patt_3[] = {0x33, 0xCC};
unsigned char Patt_1[] = {0x55, 0xAA};
unsigned char Patt_2[] = {0x66, 0x18};
enum State{ init, L1, L2, L3, L4, L5, L6, off} state;


void SMSR() {
	switch(state) {
		case init:
		state = L1;
		break;
		
		case L1:
		if(button == next || button1 == next1) {
			state = L2;
		}
		else if(button == prev || button1 == prev1) {
			state = L3;
		}
		else if(button == Off || button1 == Off) {
			state = off;
		}
		else {
			state = L4;
		}
		break;
		
		case L2:
		if(button == next || button1 == next1) {
			state = L3;
		}
		else if(button == prev || button1 == prev1) {
			state = L1;
		}
		else if(button == Off || button1 == Off) {
			state = off;
		}
		else {
			state = L5;
		}
		break;
		
		case L3:
		if(button == next) {
			state = L1;
		}
		else if(button == prev) {
			state = L2;
		}
		else if(button == Off) {
			state = off;
		}
		else {
			state = L6;
		}
		break;
		
		case L4:
		if(button == next) {
			state = L2;
		}
		else if(button == prev) {
			state = L3;
		}
		else if(button == Off) {
			state = off;
		}
		else {
			state = L1;
		}
		break;
		
		case L5:
		if(button == next) {
			state = L3;
		}
		else if(button == prev) {
			state = L1;
		}
		else if(button == Off) {
			state = off;
		}
		else {
			state = L2;
		}
		break;
		
		case L6:
		if(button == next) {
			state = L1;
		}
		else if(button == prev) {
			state = L2;
		}
		else if(button == Off) {
			state = off;
		}
		else {
			state = L3;
		}
		break;
		
		case off:
		if(button == Off) {
			state = L1;
		}
		else {
			state = off;
		}
		break;
		
		default:
		state = L1;
		break;
	}
	
	switch(state) {
		case init:
		break;
				
		case L1:
		transmit_data(Patt_1[0]);
		break;
		
		case L2:
		transmit_data(Patt_2[0]);
		break;
		
		case L3:
		transmit_data(Patt_3[0]);
		break;
		
		case L4:
		transmit_data(Patt_1[1]);
		break;
		
		case L5:
		transmit_data(Patt_2[1]);
		break;
		
		case L6:
		transmit_data(Patt_3[1]);
		break;
		
		case off:
		tmp = 0x00;
		transmit_data(tmp);
	}
}

int main(void)
{
	DDRC = 0xFF; PORTC = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
    /* Replace with your application code */
	
	TimerOn();
	TimerSet(300);
    while (1) 
    {
		SMSR();
		while(!TimerFlag);
		TimerFlag = 0;
    }
}

