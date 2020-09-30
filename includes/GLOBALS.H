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
#define MaxX 90
#define MaxY 30
#define MaxSpeed 20
#define oTICKSPEED 100//30
#define oTIMELIMIT 100//200
#define totModes 4

int torchRange = 7;
int totLevels;
int levelCnt;
int wished = false;
bool returnTitle = false;
bool wait = false;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
char border[] = { 196, 179, 218, 191, 192, 217 };
unsigned int TICKSPEED = oTICKSPEED;
unsigned int TIMELIMIT = oTIMELIMIT;

char paths[50][50];
bool pathsCleared[50];

char currentMode[10];
char Modes[totModes][10] = { "Normal", "Normal+", "Hard", "Hard+" };

void halt(int x) {   
    delay(x);
	while(1)
	{	if(kbhit())	getch();
		else	break;
	}
}

int distance(int xdiff, int ydiff) {
	int val = abs(xdiff * xdiff) + abs(ydiff * ydiff);
	val = (unsigned)((val<0) ? -val : val);
	if(val < 0) {
		clrscr();	printf("xdiff: %d\nydiff: %d\nval: ", xdiff, ydiff, val);	getch();	exit(0);
	}
	return (val <= 0) ? 0 : sqrt(val);
}

#include "MZRUN/INCLUDES/COORD.H"
#include "MZRUN/INCLUDES/TILE.H"
#include "MZRUN/INCLUDES/STATE.H"
#include "MZRUN/INCLUDES/OBJECT.H"
#include "MZRUN/INCLUDES/HELP.H"
#include "MZRUN/INCLUDES/LEVEL.H"
#include "MZRUN/INCLUDES/ENGINE.H"
#include "MZRUN/INCLUDES/GAME.H"

#endif