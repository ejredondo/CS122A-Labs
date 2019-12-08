/*
 * Eredo001_Lab7_Part2_M2.c
 *
 * Created: 11/6/2019 3:00:20 PM
 * Author : Ered5
 */ 

#include <avr/io.h>
#include <Timer.h>
#include <lcd.h>

unsigned char receivedData = 0;
void SPI_SlaveInit(void)
{
	/* Set MISO output, all others input */
	DDRB = 0x40; PORTB = 0xBF;
	/* Enable SPI */
	SPCR |= (1<<SPE)|(1<<SPIE);
	sei();
}

ISR(SPI_STC_vect)
{
	///* Wait for reception complete */
	//while(!(SPSR & (1<<SPIF)))
	//;
	///* Return Data Register */
	//return SPDR;
	receivedData = SPDR;
}

unsigned char RecData[10];
enum Servant1_State{init, receive, display} state;

void ServantSM() {
	switch(state) {
		case init:
		state = receive;
		break;
		
		case receive:
		if(receivedData == 0x20 || receivedData == 0x40) {
			state = display;
		}
		else {
			state = receive;
		}
		break;
		
		case display:
		state = receive;
		break;
	}
	
	switch(state) {
		case init:
		break;
		
		case receive:
		sprintf(RecData, "%d", receivedData);
		break;
		
		case display:
		LCD_ClearScreen();
		LCD_DisplayString(1, RecData);
		break;
		
	}
}


int main(void)
{
	DDRA = 0xFF; PORTA = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	
	LCD_init();
	SPI_SlaveInit();
	TimerOn();
	TimerSet(100);
	/* Replace with your application code */
	while (1)
	{
		ServantSM();
		while(!TimerFlag);
		TimerFlag = 0;
	}
}

