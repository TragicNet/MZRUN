#ifndef GAME_H
#define GAME_H

struct Game {
	int unlockedModes;
};

void Game_loadData(struct Game *this) {
	FILE *file;
	file = fopen("./MZRUN/data.sav", "rb" );
	if(file == NULL) {
		this->unlockedModes = 0;
	} else {
		fread((char*) this, sizeof(struct Game), 1, file);
	}
	fclose(file);
}
void Game_saveData(struct Game *this) {
	FILE *file;
	file = fopen("./MZRUN/data.sav", "wb" );
	fwrite((char*) this, sizeof(struct Game), 1, file);
	fclose(file);
}

void Game_init(struct Game *this) {
	srand((unsigned)time(0));
	Object_init(&wall, 0, 0, STOP, '#', "wall", "wall", WHITE, LIGHTGRAY, 0, 0, 1);
	Object_init(&air, 0, 0, STOP, '.', "air", "air", LIGHTGRAY, BLACK, 0, 0, 0);
	Object_init(&blackair, 0, 0, STOP, '.', "air", "air", BLACK, BLACK, 0, 0, 0);
	coord_init(&board, 9, 4);
	coord_init(&screen, 39, 13);
	levelCnt = 0;
	totLevels = Game_getPaths(paths);
	this->unlockedModes = 0;
	Game_loadData(this);
	strcpy(currentMode, "Normal");
}

int Game_getPaths(char paths[][50]) {
	struct dirent *de;
	int i, cnt=0, s;
	char ext[] = ".LVL";
	DIR *dr = opendir("./MZRUN/LEVELS");
	if(dr == NULL) return 0;
	while((de = readdir(dr)) != NULL)  {
		s = strlen(de->d_name);
		for(i = s-4; i < s; i++) {
			if(de->d_name[i] != ext[i-(s-4)])	break;
		}
		if(i==s) {
			pathsCleared[cnt] = false;
			strcpy(paths[cnt], "./MZRUN/LEVELS/");
			strcat(paths[cnt++], de->d_name);
		}
	}
	closedir(dr);
	return cnt;
}

int Game_generateLevel() {
	int i, j, gen = 0;
	totStates = 0;
	if(totLevels<=0)	return -1;	//No Levels
	//Generate Random Level
	do {
		gen = rand() % totLevels;
	} while(pathsCleared[gen] == true);
	//Reset Map
	for(i = 0; i < MaxY; i++) {
		for(j = 0; j < MaxX; j++) {
			Tile_init(&map[i][j], false, false, false);
		}
	}
	return gen;
}

void Game_gameLoop(struct Game *this) {
	struct Engine run;
	int gen;
	levelCnt = 0;
	wait = true;
	do {
		gen = Game_generateLevel(this);
		if(gen == -1)	return;	//No Levels
		levelCnt++;
		Engine_init(&run, gen);
		Engine_HUD();
		while(!Engine_getState(&run)) {
			Engine_draw(&run);
			Engine_input(&run);
			if(wait == true)	continue;
			Engine_handle(&run);
		}
		if(run.cleared == true)	pathsCleared[gen] = true;
		else	return;
	} while(levelCnt != totLevels);
}

void Game_Options(struct Game *this) {
	int i, x = 35, y = 11, key, spaces = 0;
	bool pressed = false;
	clrscr();
	gotoxy(37,9);
	textcolor(YELLOW);	cprintf("OPTIONS");
	textcolor(WHITE);
	//Print Modes based on unlocked, or current
	for(i = 0; i<sizeof(Modes)/sizeof(Modes[0]); i++) {
		if(strcmp(currentMode, Modes[i]) == 0) {
			textcolor(WHITE);	y+=i;
		} else ((this->unlockedModes >= i) ? textcolor(LIGHTGRAY) : textcolor(DARKGRAY));
		spaces = 3 - strlen(Modes[i])/2;
		gotoxy(37+spaces, 11+i);	cprintf("%s", Modes[i]);
	}
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
		case 's':	if(y!=11+this->unlockedModes) y++;	break;
		case 13:	pressed = true;	break;
		case 27:	return;
		}
		if(pressed)	break;
	}
	switch(y-11) {
	case 0:	strcpy(currentMode, "Normal");	break;
	case 1:	strcpy(currentMode, "Normal+");	break;
	case 2:	strcpy(currentMode, "Hard");	break;
	case 3:	strcpy(currentMode, "Hard+");	break;
	}
}

bool Game_Start(struct Game *this) {
	int i, x = 35, y = 12, width, key;
	bool selected = false;
	clrscr();
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
	textcolor(CYAN);	gotoxy(5, 24);
	cprintf("w: UP       ");	cprintf("a: LEFT     ");	cprintf("s: DOWN     ");	cprintf("d: RIGHT    ");
	cprintf("h: HELP     ");	cprintf("rtn: ENTER    ");
	textcolor(YELLOW);
	while(1) {
		gotoxy(x,y);	cprintf("<");	gotoxy(x+10,y);	cprintf(">");
		key = getch();
		gotoxy(x,y);	cprintf(" ");	gotoxy(x+10,y);	cprintf(" ");
		switch(key) {
		case 'w':	y = (y==12) ? 12 : y-1;	break;
		case 's':	y = (y==14) ? 14 : y+1;	break;
		case 'h':	help();	return true;
		case 13:	selected = true;	break;
		}
		if(selected)	break;
	}
	switch(y-12) {
	case 0:	Game_gameLoop(this);	break;
	case 1:	Game_Options(this);		break;
	case 2: return false;
	}
	//Unlock New Mode and Finish
	for(i = 0; i<totLevels; i++) {
		if(pathsCleared[i] == false)	break;
	}
	if(i == totLevels && levelCnt == totLevels) {
		if(this->unlockedModes<3 && strcmp(currentMode, Modes[this->unlockedModes]) == 0) {
			this->unlockedModes++;
			x = 29, y = 12, width = 18 + strlen(Modes[this->unlockedModes]);
			textcolor(LIGHTGREEN);
			gotoxy(x, y++);	cprintf("%c", border[2]);	for(i = 0; i<width-2; i++)	cprintf("%c", border[0]);	cprintf("%c", border[3]);
			gotoxy(x, y++);	cprintf("%c UNLOCKED %s MODE %c", border[1], Modes[this->unlockedModes], border[1]);
			gotoxy(x, y++);	cprintf("%c", border[4]);	for(i = 0; i<width-2; i++)	cprintf("%c", border[0]);	cprintf("%c", border[5]);
			getch();
		}
		Game_saveData(this);
	}
	return true;
}

#endif