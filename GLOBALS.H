#ifndef GLOBALS_H
#define GLOBALS_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <bios.h>
#include <string.h>
#include <dirent.h>
#include <time.h>

#define bool int
#define true 1
#define false 0
#define MaxX 60
#define MaxY 20
#define MaxSpeed 9
#define oTICKSPEED 100//30
#define oTIMELIMIT 100//200

float torchRange = 7.0f;	//7.0f
char cmap[MaxY][MaxX];
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
char border[] = { 196, 179, 218, 191, 192, 217 };
unsigned int TICKSPEED = oTICKSPEED;
unsigned int TIMELIMIT = oTIMELIMIT;
bool overlapping = false;

void halt(int x) {   
    delay(x);
	while(1)
	{	if(kbhit())	getch();
		//if(!kbhit())	break;
		else	break;
	}
}

#include "MZRUN/COORD.H"
#include "MZRUN/TILE.H"
#include "MZRUN/EVENTS.H"
#include "MZRUN/OBJECT.H"
#include "MZRUN/SAVE.H"
#include "MZRUN/ENGINE.H"
#include "MZRUN/GAME.H"

#endif