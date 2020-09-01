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

int torchRange = 7;	//7.0f
int totLevels;
int levelCnt;
int unlockedModes;
bool returnTitle = false;
bool wait = false;
char cmap[MaxY][MaxX];
char paths[50][50];
bool pathsCleared[50];
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
char border[] = { 196, 179, 218, 191, 192, 217 };
unsigned int TICKSPEED = oTICKSPEED;
unsigned int TIMELIMIT = oTIMELIMIT;

char currentMode[20];
char Modes[][20] = { "Normal", "Normal+", "Hard", "Hard+" };

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
#include "MZRUN/STATE.H"
#include "MZRUN/OBJECT.H"
#include "MZRUN/EVENTS.H"
#include "MZRUN/SAVE.H"
#include "MZRUN/ENGINE.H"
#include "MZRUN/GAME.H"

#endif