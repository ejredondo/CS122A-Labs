/*
 * Eredo001_Lab5_Part3.c
 *
 * Created: 10/30/2019 12:51:29 AM
 * Author : Ered5
 */ 

#include <avr/io.h>
#include <bit.h>
#include <timer.h>
#define button (~PINA & 0x03)
#define button1 (~PINB & 0x03)
const unsigned char next1 = (PB1);
const unsigned char prev1 = (PB2);
const unsigned char Off1 = (PB1) | (PB2);
const unsigned char next = (PA1);
const unsigned char prev = (PA2);
const unsigned char Off = (PA1) | (PA2);

void transmit_data(unsigned char x) {
// 		for(unsigned char i = 0; i < 15; i++) {
// 			PORTC = 0x01 & 0x0F;
// 			PORTC |= ((data >> i) & 0x01);
// 			PORTC |= 0x04 & 0x0F;
// 		}
// 		PORTC |= 0x02 & 0x0F;
// 		PORTC = 0x00 & 0x0F;
for(int i = 15; i >= 0; i--)
{
	PORTC = 0x88;
	PORTC |= ((x >> i) & 0x0001);   // for first 4
	PORTC |= 0x44;
	PORTC |= 0x22;
}
PORTC |= 0x22;
PORTC = 0x00;
}

unsigned char y;
unsigned char sreg;
unsigned char sreg1;
unsigned char Patt_3[] = {0x33, 0xCC};
unsigned char Patt_1[] = {0x55, 0xAA};
unsigned char Patt_2[] = {0x66, 0x18};
enum State{ init, L1, L2} state;


// void SMSR() {
// 	switch(state) {
// 		case init:
// 		state = L1;
// 		break;
// 		
// 		case L1:
// 		state = L2;
// 		break;
// 		
// 		case L2:
// 		state = L1;
// 		break;
// 	}
// 	
// 	switch(state) {
// 		case init:
// 		tmp = 0x01;
// 		break;
// 		
// 		case L1:
// 		transmit_data(tmp);
// 		break;
// 		
// 		case L2:
// 		tmp = (1<<tmp);
// 		break;
// 	}
// }

enum States {start, s1, s2} State;
void LEDS_Tick()
{
	switch(State)
	{
		case start:
		transmit_data(y);
		State = s1;
		break;
		
		case s1:
		transmit_data(y);
		y = y << 1;
		if(y == 0x8000)
		{
			State = s2;
		}
		else
		{
			State  = s1;
		}
		break;
		
		case s2:
		transmit_data(y);
		y = y >> 1;
		if(y == 0x0001)
		{
			State = s1;
		}
		else
		{
			State = s2;
		}
		break;
	}
}

int main(void)
{
	DDRC = 0xFF; PORTC = 0x00;
	DDRB = 0x00; PORTB = 0xFF;
	DDRA = 0x00; PORTA = 0xFF;
	/* Replace with your application code */
	
	TimerOn();
	TimerSet(300);
	while (1)
	{
		LEDS_Tick();
		while(!TimerFlag);
		TimerFlag = 0;
	}
}

