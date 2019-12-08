/*
 * Eredo001_Lab6_Part1.c
 *
 * Created: 10/25/2019 1:58:50 PM
 * Author : Ered5
 */ 

#include <avr/io.h>
#include <timer.h>
#include <scheduler.h>
#include <incDecSM.h>
#include <lcd.h>

enum Poll_States{Start_1, Poll}Poll_state;
unsigned char Cnt[10];
unsigned char TimeCount;
static task task3;
static task task1;

	
int Poll_task(int Poll_state) {
	switch(Poll_state) {
		case Start_1:
		Poll_state = Poll;
		break;
		
		case Poll:
		Poll_state = Start_1;
		break;	
	}
	
	switch(Poll_state) {
		case Start:
		++TimeCount;
		break;
		
		case Poll:
		if(~PINA & 0x01) {
			TimeCount = 0;
			task3.active = 0x01;
			task1.active = 0x01;
			DDRD = 0xFF;PORTD = 0xFF; 
		}
		if(TimeCount ==  10) {
			task3.active = 0x00;
			task1.active = 0x00;
			DDRD = 0x00;PORTD = 0x00; 
		}
		break;
		//LCD_Cursor(1);
		//LCD_WriteData(InitialCount);
	}
	return Poll_state;
}

enum Timer_State{Start_2, Timer}Timer_states;
	
int Timer_task(int Timer_states) {
	switch(Timer_states) {
		case Start_2:
		Timer_states = Timer;
		break;
		
		case Timer:
		Timer_states = Start_2;
		break;
	}
	
	switch(Timer_states) {
		case Start_2:
		break;
		
		case Timer:
		sprintf(Cnt,"%d", count);
		LCD_DisplayString(1, Cnt);
		break;
	}
	return Timer_states;
}


int main(void)
{
	DDRA = 0x00;PORTA = 0xFF; 
	DDRD = 0xFF;PORTD = 0xFF; 
	DDRB = 0xFF;PORTB = 0x00;
	DDRC = 0xFF;PORTC = 0x00;
	
	
	unsigned long int SMTick1_calc = 100;
	unsigned long int SMTick2_calc = 50;
	unsigned long int SMTick3_calc = 50;

	//Calculating GCD
	unsigned long int tmpGCD = 10;

	//Greatest common divisor for all tasks or smallest time unit for tasks.
	unsigned long int GCD = tmpGCD;

	//Recalculate GCD periods for scheduler
	unsigned long int SMTick1_period = SMTick1_calc/GCD;
	unsigned long int SMTick2_period = SMTick2_calc/GCD;
	unsigned long int SMTick3_period = SMTick3_calc/GCD;

	//Declare an array of tasks
	static task task2;
	task *tasks[] = {&task1, &task2, &task3};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	
	task1.state = Start;//Task initial state.
	task1.active = 0x01;
	task1.period = SMTick1_period;//Task Period.
	task1.elapsedTime = SMTick1_period; // Task current elasped time.
	task1.TickFct = &IncDec_tick; // Function pointer for the tick.
	
	task2.state = Start_1;//Task initial state.
	task2.active = 0x01;
	task2.period = SMTick2_period;//Task Period.
	task2.elapsedTime = SMTick2_period; // Task current elasped time.
	task2.TickFct = &Poll_task; // Function pointer for the tick.
	
	task3.state = Start_2;//Task initial state.
	task3.active = 0x01;
	task3.period = SMTick3_period;//Task Period.
	task3.elapsedTime = SMTick3_period; // Task current elasped time.
	task3.TickFct = &Timer_task; // Function pointer for the tick.
	
	unsigned char i;
	LCD_init();
	TimerOn();
	TimerSet(GCD);
	
    /* Replace with your application code */
    while (1) 
    {
		// Scheduler code
		for ( i = 0; i < numTasks; i++ ) {
			if(tasks[i]->active == 0x01) {
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
}

