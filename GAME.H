#ifndef GAME_H
#define GAME_H

void temp() {
}

struct saveData {
	int unlockedModes;
};

void loadData() {
	FILE *file;
	struct saveData d;
	file = fopen("./MZRUN/data.sav", "rb" );
	if(file == NULL) {
		unlockedModes = 0;
	} else {
		fread((char*) &d, sizeof(struct saveData), 1, file);
		unlockedModes = d.unlockedModes;
	}
	fclose(file);
}
void saveData() {
	FILE *file;
	struct saveData d;
	d.unlockedModes = unlockedModes;
	file = fopen("./MZRUN/data.sav", "wb" );
	fwrite((char*) &d, sizeof(struct saveData), 1, file);
	fclose(file);
}

void init() {
	temp();
	srand((unsigned)time(0));
	Object_init(&wall, 0, 0, STOP, '#', "wall", WHITE, LIGHTGRAY, 0, 0, 1);
	Object_init(&air, 0, 0, STOP, '.', "air", LIGHTGRAY, BLACK, 0, 0, 0);
	Object_init(&blackair, 0, 0, STOP, '.', "air", BLACK, BLACK, 0, 0, 0);
	coord_init(&board, 2, 4);
	coord_init(&screen, 39, 13);
	levelCnt = 0;
	totLevels = 0;
	unlockedModes = 0;
	loadData();
	strcpy(currentMode, "Normal");
}

void initGame() {
	int i, j;
	for(i = 0; i < MaxY; i++) {
		for(j = 0; j < MaxX; j++) {
			Tile_init(&map[i][j], false, false, false);
		}
	}
	totLevels = getPaths(paths);
	levelCnt = 0;
}

int generateLevel() {
	int gen = 0, t = totLevels;
	gen = rand() % t--;
	while(pathsCleared[gen] == true && t > 0) {
		gen = rand() % t--;
	}
	levelCnt++;
	return gen;
}

void GameLoop() {
	struct Engine game;
	int gen;
	do {
		gen = generateLevel();
		Engine_init(&game, gen);
		Engine_HUD(&game);
		while(!Engine_getState(&game)) {
			Engine_draw(&game);
			Engine_input(&game);
			Engine_handle(&game);
		}
		if(game.cleared == true)	pathsCleared[gen] = true;
		else	return;
	} while(levelCnt != totLevels);
}

void Options() {
	int i, x = 35, y = 11, key;
	bool pressed = false;
	clrscr();
	gotoxy(37,9);
	textcolor(YELLOW);	cprintf("OPTIONS");
	textcolor(WHITE);
	if(strcmp(currentMode, "Normal") == 0) {
		textcolor(WHITE);	y+=0;
	} else textcolor(LIGHTGRAY);
	gotoxy(37, 11);	cprintf("Normal  ");
	if(strcmp(currentMode, "Normal+") == 0) {
		textcolor(WHITE);	y+=1;
	} else ((unlockedModes > 0) ? textcolor(LIGHTGRAY) : textcolor(DARKGRAY));
	gotoxy(37, 12);	cprintf("Normal+ ");
	if(strcmp(currentMode, "Hard") == 0) {
		textcolor(WHITE);	y+=2;
	} else ((unlockedModes > 1) ? textcolor(LIGHTGRAY) : textcolor(DARKGRAY));
	gotoxy(37, 13);	cprintf(" Hard   ");
	if(strcmp(currentMode, "Hard+") == 0) {
		textcolor(WHITE);	y+=3;
	} else ((unlockedModes > 2) ? textcolor(LIGHTGRAY) : textcolor(DARKGRAY));
	gotoxy(37, 14);	cprintf(" Hard+  ");
	textcolor(GREEN);
	for(i = 0; i < 4; i++) {	//Vertical
		gotoxy(33, i + 11);	cprintf("%c", border[1]);	gotoxy(47, i + 11);	cprintf("%c", border[1]);
	}
	for(i = 0; i < 15; i++) {	//Horizontal
		gotoxy(33 + i, 10);	cprintf("%c", border[0]);	gotoxy(33 + i, 15);	cprintf("%c", border[0]);
	}
	gotoxy(33, 10);	cprintf("%c", border[2]);	gotoxy(47, 10);	cprintf("%c", border[3]);
	gotoxy(33, 15);	cprintf("%c", border[4]);	gotoxy(47, 15);	cprintf("%c", border[5]);
	while(1) {
		gotoxy(x,y);	cprintf("<");	gotoxy(x+10,y);	cprintf(">");
		key = getch();
		gotoxy(x,y);	cprintf(" ");	gotoxy(x+10,y);	cprintf(" ");
		switch(key) {
		case 'w':	if(y!=11) y--;	break;
		case 's':	if(y!=11+unlockedModes) y++;	break;
		case 13:	pressed = true;	break;
		case 'x':	return;
		case 27:	exit(0);
		}
		if(pressed)	break;
	}
	switch(y-11) {
	case 0:	strcpy(currentMode, "Normal");	break;
	case 1:	strcpy(currentMode, "Normal+");	break;
	case 2:	strcpy(currentMode, "Hard");	break;
	case 3:	strcpy(currentMode, "Hard+");	break;
	}
	Options();
}

bool Start() {
	int i, x = 35, y = 12, width, key;
	bool pressed = false;
	initGame();
	clrscr();
    /*int t = CYAN;
    textcolor(t); lowvideo();   cprintf("Hello");
    textcolor(t); highvideo();  cprintf("Hello");
    textcolor(t); normvideo();  cprintf("Hello");*/
	gotoxy(35,9);
	textcolor(YELLOW);	cprintf("MAZE RUNNER");
	textcolor(WHITE);
	gotoxy(37,12);	cprintf(" Start ");
	gotoxy(37,13);	cprintf("Options");
	gotoxy(37,14);	cprintf(" Exit  ");
	textcolor(GREEN);
	for(i = 11; i <= 15; i++) {	//Vertical
		gotoxy(33, i);	cprintf("%c", border[1]);	gotoxy(47, i);	cprintf("%c", border[1]);
	}
	for(i = 33; i <= 47; i++) {	//Horizontal
		gotoxy(i, 11);	cprintf("%c", border[0]);	gotoxy(i, 15);	cprintf("%c", border[0]);
	}
	gotoxy(33, 11);	cprintf("%c", border[2]);	gotoxy(47, 11);	cprintf("%c", border[3]);
	gotoxy(33, 15);	cprintf("%c", border[4]);	gotoxy(47, 15);	cprintf("%c", border[5]);
	while(1) {
		gotoxy(x,y);	cprintf("<");	gotoxy(x+10,y);	cprintf(">");
		key = getch();
		gotoxy(x,y);	cprintf(" ");	gotoxy(x+10,y);	cprintf(" ");
		switch(key) {
		case 'w':	y = (y==12) ? 12 : y-1;	break;
		case 's':	y = (y==14) ? 14 : y+1;	break;
		case 13:	pressed = true;	break;
		case 27:	return false;
		}
		if(pressed)	break;
	}
	switch(y-12) {
	case 0:	GameLoop();	break;
	case 1:	Options();	break;
	case 2: return false;
	}
	if(levelCnt == totLevels) {
		if(unlockedModes<3 && strcmp(currentMode, Modes[unlockedModes]) == 0) {
			unlockedModes++;
			x = 29, y = 12, width = 18+strlen(Modes[unlockedModes]);
			textcolor(GREEN);	highvideo();
			gotoxy(x, y++);	cprintf("%c", border[2]);	for(i = 0; i<width-2; i++)	cprintf("%c", border[0]);	cprintf("%c", border[3]);
			gotoxy(x, y++);	cprintf("%c UNLOCKED %s MODE %c", border[1], Modes[unlockedModes], border[1]);
			gotoxy(x, y++);	cprintf("%c", border[4]);	for(i = 0; i<width-2; i++)	cprintf("%c", border[0]);	cprintf("%c", border[5]);
			getch();
			normvideo();
		}
		saveData();
		return true;
	}
	return true;
}

#endif