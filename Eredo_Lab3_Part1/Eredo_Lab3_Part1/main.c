/*
 * Eredo_Lab3_Part1.c
 *
 * Created: 10/10/2019 11:27:18 PM
 * Author : Ered5
 */ 

#include <avr/io.h>
#include <lcd.h>
#include <keypad.h>
#include <scheduler.h>
#include <timer.h>
#include <bit.h>
#include "avr/iom1284.h"

void SPI_MasterInit(void)
{
	/* Set MOSI and SCK output, all others input */
	DDRB = (1<<DDB4)|(1<<DDB5)|(1<<DDB7);
	/* Enable SPI, Master, set clock rate fck/16 */
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
	sei();
}

void SPI_MasterTransmit(char cData)
{
	/* Start transmission */
	SPDR = cData;
	SetBit(PORTB, 4, 0);
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF))){
		;
	}
	unsigned char tmp = SPDR;
	SetBit(PORTB, 4, 1);
}

enum States{init, wait, Transmit_2};

unsigned char PtrnSelect;
unsigned char SpdSelect;
unsigned char RecievedData;
char ptrn[10];
unsigned char foo;
unsigned char input;
unsigned long int SMTick1_calc;

int SMPattern(int state) {
	input = GetKeypadKey();
	switch(state) {
	case init:
	state = wait;
	break;
	
	case wait:
		switch(input) {
			case 'A':
			PtrnSelect = 0x10;
			LCD_Cursor(6);
			LCD_WriteData('1');
			break;
			
			case 'B':
			PtrnSelect = 0x20;
			LCD_Cursor(6);
			LCD_WriteData('2');
			break;
			
			case 'C':
			PtrnSelect = 0x30;
			LCD_Cursor(6);
			LCD_WriteData('3');
			break;
			
			case 'D':
			PtrnSelect = 0x40;
			LCD_Cursor(6);
			LCD_WriteData('4');
			break;
	}
	state = wait;
	break;
	
	
	}
	
	return state;
}

enum State_2{init_2, wait_2};
	
int SMSpd(int state) {
	input = GetKeypadKey();
	switch(state) {
	case init_2:
	state = wait_2;
	break;
	
	case wait_2:
	switch(input) {
		case '1':
		SpdSelect = 0x01;
		LCD_Cursor(14);
		LCD_WriteData('1');
		break;
		
		case '2':
		SpdSelect = 0x02;
		LCD_Cursor(14);
		LCD_WriteData('2');
		break;
		
		case '3':
		SpdSelect = 0x03;
		LCD_Cursor(14);
		LCD_WriteData('3');
		break;
		
		case '4':
		SpdSelect = 0x04;
		LCD_Cursor(14);
		LCD_WriteData('4');
		break;
		
		case '5':
		SpdSelect = 0x05;
		LCD_Cursor(14);
		LCD_WriteData('5');
		break;
		
		case '6':
		SpdSelect = 0x06;
		LCD_Cursor(14);
		LCD_WriteData('6');
		break;
	}
	state = wait_2;
	break;
	
	}
	return state;
}

enum States_3{init_3, Transmit_1};

int SMaster(int state) {
	switch(state) {
		case init_3:
		state = Transmit_1;
		break;
		
		case Transmit_1:
		RecievedData = PtrnSelect | SpdSelect;
		if(PtrnSelect != 0x00 || SpdSelect != 0x00) {
// 			LCD_ClearScreen();
// 			foo = RecievedData;
// 			sprintf(ptrn,"%d",foo );
// 			LCD_DisplayString(1, ptrn);
			SPI_MasterTransmit( RecievedData);
		}
		else {
			break;
		}
		state = init_3;
		break;
	}
	return state;
}



int main(void)
{
	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xF0; PORTD = 0x0F;
	LCD_init();
	unsigned char Patt[] = {'P','t','r','n', ':'};
	unsigned char sped[] = {'S', 'p', 'd', ':'};
	unsigned char uC[] = {'u', 'C', ':', '1'};
	unsigned char i;
		
	for(i = 0; i < 5; i++) {
		LCD_Cursor(i + 1);
		LCD_WriteData(Patt[i]);
	}
	
	for(i = 0; i < 4; i++) {
		LCD_Cursor(i + 10);
		LCD_WriteData(sped[i]);
	}
	
	for(i = 0; i < 4; i++) {
		LCD_Cursor(i + 17);
		LCD_WriteData(uC[i]);
	}
	
	
		SMTick1_calc = 100;
		unsigned long int SMTick2_calc = 50;
 		unsigned long int SMTick3_calc = 50;
// 		unsigned long int SMTick4_calc = 150;
// 		unsigned long int SMTick5_calc = 10;

		//Calculating GCD
		unsigned long int tmpGCD = 10;

		//Greatest common divisor for all tasks or smallest time unit for tasks.
		unsigned long int GCD = tmpGCD;

		//Recalculate GCD periods for scheduler
		unsigned long int SMTick1_period = SMTick1_calc/GCD;
		unsigned long int SMTick2_period = SMTick2_calc/GCD;
 		unsigned long int SMTick3_period = SMTick3_calc/GCD;
// 		unsigned long int SMTick4_period = SMTick4_calc/GCD;
// 		unsigned long int SMTick5_period = SMTick5_calc/GCD;

		//Declare an array of tasks
		static task task1, task2, task3;//, task4, task5;
		task *tasks[] = { &task1, &task2, &task3};//, &task4, &task5 };
		const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
		
		task1.state = init;//Task initial state.
		task1.period = SMTick1_period;//Task Period.
		task1.elapsedTime = SMTick1_period; // Task current elasped time.
		task1.TickFct = &SMaster; // Function pointer for the tick.
		
		task2.state = init_2;//Task initial state.
		task2.period = SMTick2_period;//Task Period.
		task2.elapsedTime = SMTick2_period; // Task current elasped time.
		task2.TickFct = &SMPattern; // Function pointer for the tick.
		
		
		task3.state = init_3;//Task initial state.
		task3.period = SMTick3_period;//Task Period.
		task3.elapsedTime = SMTick3_period; // Task current elasped time.
		task3.TickFct = &SMSpd; // Function pointer for the tick.
		
		SPI_MasterInit();
		TimerSet(GCD);
		TimerOn();
// 		
// 		task4.state = 0;//Task initial state.
// 		task4.period = SMTick4_period;//Task Period.
// 		task4.elapsedTime = SMTick4_period; // Task current elasped time.
// 		task4.TickFct = &SMTick4; // Function pointer for the tick.
// 		
// 		task5.state = 0;//Task initial state.
// 		task5.period = SMTick5_period;//Task Period.
// 		task5.elapsedTime = SMTick5_period; // Task current elasped time.
// 		task5.TickFct = &SMTick5; // Function pointer for the tick.
	
    /* Replace with your application code */
    while (1) 
    {
		// Scheduler code
		for ( i = 0; i < numTasks; i++ ) {
			// Task is ready to tick
			if ( tasks[i]->elapsedTime == tasks[i]->period ) {
				// Setting next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				// Reset the elapsed time for next tick.
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		while(!TimerFlag);
		TimerFlag = 0;
    }
}

