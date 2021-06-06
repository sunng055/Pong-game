/*	Author: lab
 *  Partner(s) Name: Sunny Ng
 *	Lab Section:
 *	Assignment: Final Project Pong Game
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Video:
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include "../header/timer.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct task {
        int state;
        unsigned long period;
        unsigned long elapsedTime;
        int (*TickFct)(int);
} task;

unsigned char tempA = 0x00;

unsigned char midCol = 0xF7; //middle for pad1
unsigned char tempMid1 = 0x00;
unsigned char tempC = 0x00; //col for pad1
unsigned char tempD = 0x00; //row for pad1
static unsigned char col = 0x80; //pad1
static unsigned char row = 0xE3;//pad1
enum P1_States {wait, up, down};
int P1_Tick(int state) {
//static unsigned char col = 0x80; 
//static unsigned char row = 0xC3;

tempA = ~PINA & 0x03;
switch(state) {
        case wait:
        if (tempA == 0x01) {
        state = up;
        }
        else if (tempA == 0x02) {
        state = down;
        }
        else {
        state = wait;
        }
        break;
        case up:
        state = wait;
        break;
        case down:
        state = wait;
        break;
        default:
        state = wait;
        break;
}
switch(state) {
        case wait:
        break;
        case down:
        if (row != 0x1F) {
        row = (row << 1) + 0x01;
	midCol = ((midCol << 1) + 0x01);
        }
        else if (row == 0x1F) {
        row = 0x1F;
        }
        break;
        case up:
        if (row != 0xF8) {
        row = (row >> 1) + 0x80;
	midCol = (midCol >> 1) + 0x80;
        }
        else if (row == 0xF8) {
        row = 0xF8;
        }
        break;
        default:
        break;
}
tempC = col;
tempD = row;
tempMid1 = midCol;
return state;
}
static unsigned char tempC1 = 0x00;
//tempC1 and tempD1 are the ball
unsigned char tempMid2 = 0x00; //row for mid pad2
unsigned char midCol2 = 0xEF;
unsigned char tempAiC = 0x00;;
//unsigned char tempAiR = tempMid1;
static unsigned char tempC3 = 0x00; //col for pad2
static unsigned char tempD3 = 0x00; //row for pad2
static unsigned char col3 = 0x01; //pad2
static unsigned char row3 = 0xC7; //pad2
//unsigned char rando = rand() % 2;
enum P2_States {wait2, up2, down2};
int P2_Tick(int state) {
//tempA = ~PINA; was for player control P2
tempAiC = tempC1;
unsigned char tempAiR = tempMid1;
unsigned char rando = rand() % 2;

switch(state) {
        case wait2:
	if ((tempAiC < 3) && (tempAiR > row3) && rando == 1) {
	state = up2;
        }
        else if ((tempAiC < 3) && (tempAiR < row3) && rando == 1) {
	state = down2;
        }
        else {
        state = wait2;
        }
        break;
        case up2:
        state = wait2;
        break;
        case down2:
        state = wait2;
        break;
        default:
        state = wait2;
        break;
}
switch(state) {
        case wait2:
        break;
        case down2:
        if (row3 != 0x1F) {
        row3 = (row3 << 1) + 0x01;
	midCol2 = (midCol2 << 1) + 0x01;
        }
        else if (row3 == 0x1F) {
        row3 = 0x1F;
        }
        break;
        case up2:
        if (row3 != 0xF8) {
        row3 = (row3 >> 1) + 0x80;
	midCol2 = (midCol2 >> 1) + 0x80;
        }
        else if (row3 == 0xF8) {
        row3 = 0xF8;
        }
        break;
        default:
        break;
}
tempC3 = col3;
tempD3 = row3;
tempMid2 = midCol2;
return state;
}

//unsigned char flagTop = 0x00; //to tell which direction ball is coming from
unsigned char startArr[3] = {0xEF, 0xF7, 0xFB}; //starting for P1
unsigned char indStart = 0x00; //starting for P1
unsigned char startArr2[3] = {0xEF, 0xF7, 0xFB}; //starting for P2
unsigned char indStart2 = 0x00; //starting for P2
unsigned char scoreP1 = 0x00; //flag for scoring a point to P1
unsigned char scoreP2 = 0x00; //flag for scoring a point to P2
//static unsigned char tempC1 = 0x00; //ball col //redeclared on top lol
static unsigned char tempD1 = 0x00; //ball row
enum Ball_States {startBallP1, startBallP2,  move, back, move1, back1, move2, back2, topwall, topwall2, botwall, botwall2, endP1, endP2, straight, straight2, winner};
int Ball_Tick(int state) {
static unsigned char col1 = 0x40; //ball col
static unsigned char row1 = 0xEF; //ball row
unsigned char end1 = tempC; //col 1 (paddle2)
unsigned char end2 = tempC3;//col 8 (paddle1)
unsigned char tmpEND1 = row1 | tempD3;//checks if ball hits paddle2 (row)
unsigned char tmpEND2 = row1 | tempD; //checks if ball hits paddle1 (row)
unsigned char tmpMID = row1 | ~tempMid1; //checks if ball hits mid of pad1
unsigned char tmpMID2 = row1 | ~tempMid2; //checks if ball hits mid of pad2
switch(state) {
	case startBallP1:
	state = move;
	break;

	case startBallP2:
	state = back;
	break;

	case move:
	if ((col1 == end2) && (tmpEND1 == 0xFF)) {
	state = endP1;
	}
	else if (col1 == end2) {
	state = back;
	}
	else if (row1 == 0xFE) {
	state = topwall;
	}
	else {
	state = move;
	}
	break;

	case back:
	if ((col1 == end1) && (tmpEND2 == 0xFF)) {
	state = endP2;
	}
	else if ((col1 == end1) && (tmpMID == 0xFF)) {
	state = straight;
	}
	else if (col1 == end1) {
	state = move;
	}
	else if (row1 == 0x7F) {
	state = botwall;
	}
	else {
	state = back;
	}
	break;

	case straight://not actually straight it goes diagonal lol
	//state = winner;
	if ((col1 == end2) && (tmpEND1 == 0xFF)) {
        state = endP1;
        }
        else if (col1 == end2) {
        state = back;
        }
	else if (row1 == 0xFE) {
	state = topwall;
	}
	else if (col1 != end2){
	state = straight;
	}
	break;

	case straight2:
	//state = winner;
	if ((col1 == end1) && (tmpEND2 == 0xFF)) {
        state = endP2;
        }
        else if (col1 == end1) {
        state = move;
        }
	else if (row1 == 0x7F) {
	state = botwall;
	}
        else if (col1 != end1){
        state = straight2;
        }
	break;

	case topwall:
	state = move1;
	break;

	case topwall2:
	state = back2;
	break;

	case move1:
	if ((col1 != end2) && (row1 == 0x7F)) {
        state = botwall2;
        }
	else if (col1 != end2) {
        state = move1;
	}
	else if ((col1 == end2) && (tmpEND1 == 0xFF)) {
	state = endP1;
	}
	else if ((col1 == end2) && (tmpMID2 == 0xFF)) {
        state = straight2;
        }
	else if (col1 == end2) {
	state = back1;
	}
	else {
	state = move1;
	}
	break;

	case move2:
	state = move;
	break;

	case back1:
	if ((col1 != end1) && (row1 == 0xFE)) {
        state = topwall2;
        }
	else if (col1 != end1) {
        state = back1;
        }
        else if ((col1 == end1) && (tmpEND2 == 0xFF)) {
        state = endP2;
        }
	else if ((col1 == end1) && (tmpMID == 0xFF)) {
        state = straight;
        }
        else if (col1 == end1) {
        state = move1;
        }
	break;

	case back2:
        state = back;
	break;

	case botwall:
	state = back1;
	break;

	case botwall2:
	state = move2;
	break;

	case endP1:
	if ((scoreP1 < 3)) {
	state = startBallP1;
	}
	else {
	state = winner;
	}
	break;

	case endP2:
	if ((scoreP2 < 3)) {
        state = startBallP2;
        }
        else {
        state = winner;
        }
	break;

	case winner:
	state = winner;
	break;

	default:
	state = startBallP1;
	break;
}

switch(state) {

	case startBallP1:
	col1 = 0x40;
	if(indStart >= 2) {
	row1 = startArr[indStart];
	indStart = 0;
	}
	else {
	row1 = startArr[indStart];
	indStart = indStart + 1;
	}
	break;

	case startBallP2:
	col1 = 0x02;
        if(indStart2 >= 2) {
        row1 = startArr2[indStart2];
        indStart2 = 0;
        }
        else {
        row1 = startArr2[indStart2];
        indStart2 = indStart2 + 1;
        }
        break;

	case move:
	row1 = ((row1 >> 1) + 0x80);
	col1 = (col1 >> 1);
	break;

	case move1:
	row1 = ((row1 << 1) + 0x01);
	col1 = (col1 >> 1);
	break;
	
	case move2:
	break;

	case back:
	col1 = (col1 << 1);
	row1 = ((row1 << 1) + 0x01);
	break;

	case back1:
	col1 = (col1 << 1);
	row1 = ((row1 >> 1) + 0x80);
	break;

	case back2:
	break;

	case topwall:
	break;
	
	case topwall2:
	break;

	case straight:
	col1 = (col1 >> 1);
	row1 = ((row1 >> 1) + 0x80);
	break;

	case straight2:
	col1 = (col1 << 1);
	row1 = ((row1 << 1) + 0x01);
	break;

	case botwall:
	break;

	case botwall2:
	break;

	case endP1:
	scoreP1 = scoreP1 + 1;
	col1 = 0x0F;
	row1 = 0x00;
	break;

	case endP2:
	scoreP2 = scoreP2 + 1;
	col1 = 0xF0;
	row1 = 0x00;
	break;

	case winner:
        col1 = 0xFF;
        row1 = 0x00;	
	break;

	default:
	break;
}
tempC1 = col1;
tempD1 = row1;
return state;
}

unsigned char ind = 0;
enum Display_States {start, show};
int Display_Tick(int state) {
unsigned char Carr[3] = {tempC1, tempC3, tempC};
unsigned char Rarr[3] = {tempD1, tempD3, tempD};
//unsigned char ind = 0;
switch(state) {
	case start:
	state = show;
	break;
	case show:
	state = show;
	break;
	default:
	state = start;
	break;
}

switch(state) {
	case show:
	if(ind >= 2) {
	PORTC = Carr[ind];
	PORTD = Rarr[ind];
	ind = 0;
	}
	else {
	PORTC = Carr[ind];
	PORTD = Rarr[ind];
	ind = ind + 1;
	}
	break;
	default:
	break;

}
//PORTC = Carr[ind];
//PORTD = Rarr[ind];
return state;
}


unsigned char tempB1 = 0x00; //led display score for P1
unsigned char tempB2 = 0x00;
enum Score_States {polling};
int Score_Tick(int state) {
unsigned char sc1 = scoreP1;
unsigned char sc2 = scoreP2;

switch(state) {
	case polling:
	state = polling;
	//tempB1 = sc1;
        //tempB2 = sc2;
	break;
	default:
	state = polling;
	break;
}

switch(state) {
	case polling:
	tempB1 = sc1;
	tempB2 = sc2 << 2;
	break;
	default:
	break;
}
PORTB = tempB1 | tempB2;
return state;
}

int main(void) {
    /* Insert DDR and PORT initializations */
        DDRA = 0x00;
        DDRC = 0xFF;
        DDRD = 0xFF;
	DDRB = 0xFF;
        PORTA = 0xFF;
	PORTB = 0xFF;
        PORTC = 0x00;
        PORTD = 0x00;
    /* Insert your solution below */

	static task task1, task2, task3, task4, task5;
        task *tasks[] = {&task1, &task2, &task3, &task4, &task5};
        const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

        const char start = -1;

    task1.state = start;
    task1.period = 100;
    task1.elapsedTime = task1.period;
    task1.TickFct = &P1_Tick;

    task3.state = start;
    task3.period = 100;
    task3.elapsedTime = task3.period;
    task3.TickFct = &P2_Tick;

    task2.state = start;
    task2.period = 150;
    task2.elapsedTime = task2.period;
    task2.TickFct = &Ball_Tick;

    task4.state = start;
    task4.period = 1;
    task4.elapsedTime = task4.period;
    task4.TickFct = &Display_Tick;

    task5.state = start;
    task5.period = 1;
    task5.elapsedTime = task5.period;
    task5.TickFct = &Score_Tick;

    TimerSet(1);
    TimerOn();

    unsigned short i;
    while (1) {
        for(i = 0; i < numTasks; i++) {
        if( tasks[i]->elapsedTime == tasks[i]->period ) {
                tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
                tasks[i]->elapsedTime = 0;
        }
        tasks[i]->elapsedTime += 1;
	//PORTC = tempC;
	//PORTD = tempD;
    }//end of for loop
	if ( (tempD1 | ~tempMid2) == 0xFF && (tempC1 == 0x80)) {
		task2.period = 300;
	}
	else if ((tempC1 == 0x01) && (tempD1 | ~tempMid2) == 0xFF) {
		task2.period = 300;
	}
	else if(tempC1 == 0x01 || tempC1 == 0x80) {
		task2.period = 30;
	}
	else {
		task2.period = 150;
	}
    while(!TimerFlag);
    TimerFlag = 0;
    }
    return 1;
}

