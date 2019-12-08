/*
 * Eredo001_Lab7_Part1_M2.c
 *
 * Created: 10/31/2019 8:54:13 PM
 * Author : Ered5
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <timer.h>
unsigned short lightval;

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	//ADEN: setting this bit enables analog-to-digital conversion.
	//ADSC: setting this bit starts the first conversion.
	//ADATE: setting this bit enables auto-triggering. Since we are
	//in Free Running Mode, a new conversion will trigger whenever
	//the previous conversion completes.
}
void SPI_SlaveInit(void)
{
	/* Set MISO output, all others input */
	DDRB = 0x40; PORTB = 0xBF;
	/* Enable SPI */
	SPCR |= (1<<SPE)|(1<<SPIE);
	sei();
}

void SPI_Transmit(char cData)
{
	/* Start transmission */
	SPDR = cData;

	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
	unsigned char tmp = SPDR;

}

unsigned char receivedData = 0;
ISR(SPI_STC_vect)
{
	///* Wait for reception complete */
	//while(!(SPSR & (1<<SPIF)))
	//;
	///* Return Data Register */
	//return SPDR;
	receivedData = SPDR;
}

enum State{L0, L1} state;

void SPI_1() {
	switch(state) {
		case L0:
		state = L1;
		break;
		
		case L1:
		state = L0;
		break;		
		
	}
	switch(state) {
		case L0:
		break;
		
		case L1:
		lightval = ADC;
		SPDR = lightval;
		break;
		
	}
}

int main(void)
{
	DDRA = 0x00;PORTA = 0xFF; //input
	DDRB = 0xFF;PORTB = 0x00; //output
	
	
	
	ADC_init();
	SPI_SlaveInit();
	TimerOn();
	TimerSet(1000);
	
	while (1)
	{
		SPI_1();
		while(!TimerFlag);
		TimerFlag = 0;
	}
}

