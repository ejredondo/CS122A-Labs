/*
 * Eredo001_Lab5_Part1.c
 *
 * Created: 10/23/2019 2:09:40 PM
 * Author : Ered5
 */ 

#include <avr/io.h>
#include <bit.h>
#include <timer.h>

void transmit_data(unsigned char data) {
	for(unsigned char i = 0; i < 8; i++) {
		PORTC = 0x08;
		PORTC |= ((data >> i) & 0x01);
		PORTC |= 0x04;
	}
	PORTC |= 0x02;
	PORTC = 0x00;
}

enum State{init, L0, L1, L2} state;
unsigned char tmp = 0x00;
	
void SMSR() {
	switch(state) {
		case init:
		state = L0;
		break;
		
		case L0:
		if(~PINA & 0x01) {
			state = L1;
		}
		else if(~PINA & 0x02) {
			state = L2;
		}
		else {
			state = L0;
		}
		break;
		
		case L1:
		if(~PINA & 0x01) {
			state = L1;
		}
		else {
			state = L0;
		}
		break;
		
		case L2:
		if(~PINA & 0x02) {
			state = L2;
		}
		else {
			state = L0;
		}
		break;
		
		

		
	}
	
	switch(state) {
		case init:
		tmp = 0x0F;
		break;
		
		case L0:
		transmit_data(tmp);
		break;
		
		case L1:
		tmp++;
		transmit_data(tmp);
		break;
		
		case L2:
		tmp--;
		transmit_data(tmp);
		break;
		
	}
}


int main(void)
{
	DDRC = 0xFF;PORTC = 0x00;
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

