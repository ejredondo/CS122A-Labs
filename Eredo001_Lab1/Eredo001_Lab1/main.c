/*
 * Eredo001_Lab1.c
 *
 * Created: 10/4/2019 8:49:10 AM
 * Author : EJ
 */ 

#include <avr/io.h>
//#include <Timer.h>
#include <Scheduler.h>

volatile unsigned char TimerFlag = 0;

unsigned long int findGCD(unsigned long int a, unsigned long int b)
{
	unsigned long int c;
	while(1){
		c = a%b;
		if(c==0){return b;}
		a = b;
		b = c;
	}
	return 0;
}

enum States_1{init, led1, led2, led3} state;
unsigned char Three_LED;
	
int SMTick1(int state) {
	switch(state) {
		case init:
		state = led1;
		break;
		
		case led1:
		state = led2;
		break;
		
		case led2:
		state = led3;
		break;
		
		case led3:
		state = led1;
		break;
	}
	
	switch(state) {
		case init:
		Three_LED = 0x00;
		PORTB = Three_LED;
		break;
		
		case led1:
		Three_LED = 0x01;
		PORTB = Three_LED;
		break;
		
		case led2:
		Three_LED = 0x01;
		PORTB = Three_LED;
		break;
		
		case led3:
		Three_LED = 0x04;
		PORTB = Three_LED;
		break;
	}
	return state;
	
}

enum States_2{ledWait, ledOn} state2;
unsigned char led;
	
int SMTick2(int state2) {
	unsigned char pause = ~PINA & 0x01;
	switch(state2) {
		case ledWait:
		if(pause == 0) {
			state2 = ledOn;
			break;
		}
		else {
			state2 = ledWait;
			break;
		}
		
		case ledOn:
		if(pause == 0) {
			state2 = ledWait;
			break;
		}
		else {
			state = ledOn;
			break;
	}
	
	switch(state2) {
		case ledWait:
		led = 0x00;
		PORTB = Three_LED | led;
		break;
		
		case ledOn:
		led = 0x08;
		PORTB = Three_LED | led;
		break;
	}
	
	return state2;
}



int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
    /* Replace with your application code */
	
		// Period for the tasks
		unsigned long int SMTick1_calc = 500;
		unsigned long int SMTick2_calc = 1000;

		//Calculating GCD
		unsigned long int tmpGCD = 1;
		tmpGCD = findGCD(SMTick1_calc, SMTick2_calc);

		//Greatest common divisor for all tasks or smallest time unit for tasks.
		unsigned long int GCD = tmpGCD;

		//Recalculate GCD periods for scheduler
		unsigned long int SMTick1_period = SMTick1_calc/GCD;
		unsigned long int SMTick2_period = SMTick2_calc/GCD;

		//Declare an array of tasks
		static task task1, task2;
		task *tasks[] = { &task1, &task2};
		const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

		// Task 1
		task1.state = -1;//Task initial state.
		task1.period = SMTick1_period;//Task Period.
		task1.elapsedTime = SMTick1_period;//Task current elapsed time.
		task1.TickFct = &SMTick1;//Function pointer for the tick.

		// Task 2
		task2.state = -1;//Task initial state.
		task2.period = SMTick2_period;//Task Period.
		task2.elapsedTime = SMTick2_period;//Task current elapsed time.
		task2.TickFct = &SMTick2;//Function pointer for the tick.

		// Set the timer and turn it on
		TimerSet(GCD);
		TimerOn();

		unsigned short i; // Scheduler for-loop iterator
		while(1) {
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

		// Error: Program should not exit!
		return 0;
 }

