#ifndef GLOBALS_H
#define GLOBALS_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <bios.h>
#include <iostream.h>
#include <fstream.h>
#include <string.h>

#define bool int
#define true 1
#define false 0
/*#define fullmap.y 20 //10
#define fullmap.x 60 //20*/
#define MaxSpeed 9
#define boardX 1
#define boardY 1
#define TICKSPEED 100//30
#define TIMELIMIT 100//200

float torchRange = 7.0f;
char cmap[20][60];
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
char border[] = { 196, 179, 218, 191, 192, 217 };

void halt(int x = 1) {   
    delay(x);
	while(1)
	{	if(kbhit())	getch();
		if(!kbhit())	break;
	}
}

#include "MZRUN/COORD.H"
#include "MZRUN/TILE.H"
#include "MZRUN/EVENTS.H"
#include "MZRUN/OBJECT.H"
#include "MZRUN/ENGINE.H"

#endif