#ifndef GAME_H
#define GAME_H

// Structure for game data,
struct Game {
	int unlockedModes;
	bool wished;
	char paths[50][50];
	bool pathsCleared[50];
};

// Load game data from file
void Game_loadData(struct Game *this) {
	FILE *file;
	file = fopen("MZRUN/files/data.sav", "rb" );
	if(file == NULL) {
		this->unlockedModes = 0;
		this->wished = false;
	} else {
		fread((char*) this, sizeof(struct Game), 1, file);
	}
	fclose(file);
}

// Save game data to file
void Game_saveData(struct Game *this) {
	FILE *file;
	file = fopen("MZRUN/files/data.sav", "wb" );
	fwrite((char*) this, sizeof(struct Game), 1, file);
	fclose(file);
}

void Game_init(struct Game *this) {
	Object_init(&wall, 0, 0, STOP, '#', "wall", "wall", WHITE, LIGHTGRAY, 0, 0, 1);
	Object_init(&air, 0, 0, STOP, '.', "air", "air", LIGHTGRAY, BLACK, 0, 0, 0);
	Object_init(&blackair, 0, 0, STOP, '.', "blackair", "blackair", BLACK, BLACK, 0, 0, 0);
	Object_init(&outVision, 0, 0, STOP, '.', "out", "out", DARKGRAY, BLACK, 0, 0, 0);
	Object_init(&pit, 0, 0, STOP, '.', "out", "out", BROWN, BLACK, 0, 0, 0);
	coord_init(&board, 9, 4);
	coord_init(&screen, 39, 13);
	Game_loadData(this);
	strcpy(currentMode, "Normal");
}

// Get paths from levels folder
int Game_getpaths(struct Game *this) {
	struct dirent *de;
	int i, cnt=0, s;
	char ext[] = ".LVL";
	DIR *dr = opendir("MZRUN/files/LEVELS");
	if(dr == NULL) return 0;
	while((de = readdir(dr)) != NULL)  {
		s = strlen(de->d_name);
		for(i = s-4; i < s; i++) {
			if(de->d_name[i] != ext[i-(s-4)])	break;
		}
		if(i==s) {
			this->pathsCleared[cnt] = false;
			strcpy(this->paths[cnt], "MZRUN/files/LEVELS/");
			strcat(this->paths[cnt++], de->d_name);
		}
	}
	closedir(dr);
	return cnt;
}

// Select random level from available ones
int Game_generateLevel(struct Game *this) {
	int i, j, gen = 0;
	totStates = 0;
	if(totLevels<=0)	return -1;	// No Levels
	// Generate Random Level
	do {
		gen = random(totLevels);
	} while((gen != random(totLevels)) && this->pathsCleared[gen] == true);
	return gen;
}

// Loop to run all levels
void Game_gameLoop(struct Game *this) {
	struct Engine run;
	int gen;
	do {
		gen = Game_generateLevel(this);
		if(gen == -1)	return;	// No Levels
		levelCnt++;
		Engine_init(&run, this->paths[gen]);
		Engine_HUD();
		while(!Engine_getState(&run)) {
			Engine_draw(&run);
			Engine_input(&run);
			if(wait == true)	continue;
			Engine_handle(&run);
		}
		if(run.cleared)	this->pathsCleared[gen] = true;
		else	return;
	} while(levelCnt != totLevels);
}

void Game_options(struct Game *this);
void Game_optionsMode(struct Game *this);

// Start screen
bool Game_start(struct Game *this) {
	int i, j, k = 0, x = 35, y = 12, width, key;
	unsigned int t = TIMELIMIT * TICKSPEED;
	bool selected = false;
	char banner[][38] = {
		"       _____ _____ _____ _____       ",
		"      |     |  _  |__   |   __|      ",
		"      | | | |     |   __|   __|      ",
		"      |_|_|_|__|__|_____|_____|      ",
		" _____ _____ _____ _____ _____ _____ ",
		"| __  |  |  |   | |   | |   __| __  |",
		"|    -|  |  | | | | | | |   __|    -|",
		"|__|__|_____|_|___|_|___|_____|__|__|"
	};
	levelCnt = 0;
	totLevels = Game_getpaths(this);
	clrscr();
	
	drawBorder();
	textcolor(YELLOW);
	gotoxy(37,12);	cprintf(" Start ");
	gotoxy(37,13);	cprintf("Options");
	gotoxy(37,14);	cprintf(" Exit  ");
	textcolor(GREEN);
	for(i = 11; i <= 15; i++) {	// Vertical
		gotoxy(33, i);	cprintf("%c", border[1]);	gotoxy(47, i);	cprintf("%c", border[1]);
	}
	for(i = 33; i <= 47; i++) {	// Horizontal
		gotoxy(i, 11);	cprintf("%c", border[0]);	gotoxy(i, 15);	cprintf("%c", border[0]);
	}
	gotoxy(33, 11);	cprintf("%c", border[2]);	gotoxy(47, 11);	cprintf("%c", border[3]);
	gotoxy(33, 15);	cprintf("%c", border[4]);	gotoxy(47, 15);	cprintf("%c", border[5]);
	textcolor(CYAN);	gotoxy(5, 24);
	cprintf("w: UP       ");	cprintf("a: LEFT     ");	cprintf("s: DOWN     ");	cprintf("d: RIGHT    ");
	cprintf("h: HELP     ");	cprintf("rtn: ENTER    ");
	textcolor(YELLOW);	gotoxy(x,y);	cprintf("<");	gotoxy(x+10,y);	cprintf(">");
	if(startAnimation)
	for(i = 0; i < 8; i++) {
		textcolor(8 + i);
		gotoxy(22, i + 2);
		for(j=0; j<37; j++) {
			if(animationOn)	delay(10);
			if(kbhit())	break;
			cprintf("%c", banner[i][j]);
		}
		if(j!=37)	break;
	}
	if(startAnimation)	startAnimation = false;
	textcolor(YELLOW);
	while(1) {
		gotoxy(x,y);	cprintf("<");	gotoxy(x+10,y);	cprintf(">");
		while(1) {
			if(t/1000 < 13) {
				t = TIMELIMIT * TICKSPEED;	k++;
			}
			for(i = 0; i < 8; i++) {
				if(k>=8)	k = 0;
				textcolor(8 - k + i);
				gotoxy(22, i + 2);	cprintf("%s", banner[i]);
			}
			t--;
			if(kbhit())	break;
		}
		key = getch();
		textcolor(YELLOW);
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
	case 1:	Game_options(this);		break;
	case 2: return false;
	}
	// Unlock New Mode and Finish
	for(i = 0; i<totLevels; i++) {
		if(this->pathsCleared[i] == false)	break;
	}
	if(i == totLevels && levelCnt == totLevels) {
		if(strcmp(currentMode, Modes[this->unlockedModes]) == 0) {
			x = 29, y = 12;
			textcolor(LIGHTGREEN);
			if(this->unlockedModes < totModes-1) {	// Unlock New Mode
				this->unlockedModes++;
				width = 18 + strlen(Modes[this->unlockedModes]);
				gotoxy(x, y++);	cprintf("%c", border[2]);	for(i = 0; i<width-2; i++)	cprintf("%c", border[0]);	cprintf("%c", border[3]);
				gotoxy(x, y++);	cprintf("%c UNLOCKED %s MODE %c", border[1], Modes[this->unlockedModes], border[1]);
				gotoxy(x, y++);	cprintf("%c", border[4]);	for(i = 0; i<width-2; i++)	cprintf("%c", border[0]);	cprintf("%c", border[5]);
				getch();
			} else if(!this->wished){	// Wish Once for Clearing all levels
				width = 19;
				gotoxy(x, y++);	cprintf("%c", border[2]);	for(i = 0; i<width-2; i++)	cprintf("%c", border[0]);	cprintf("%c", border[3]);
				gotoxy(x, y++);	cprintf("%c CONGRATULATIONS %c", border[1], border[1]);
				gotoxy(x, y++);	cprintf("%c", border[4]);	for(i = 0; i<width-2; i++)	cprintf("%c", border[0]);	cprintf("%c", border[5]);
				getch();
				this->wished = true;
			}
		}
		Game_saveData(this);
	}
	return true;
}

// Option screen
void Game_options(struct Game *this) {
	int i, x = 0, y = 0, key;
	int opt[2][3] = {
		{ 1, 0 },
		{ 1, 1 }
	};
	int colors[2] = { RED, GREEN };
	bool pressed = false;
	
	clrscr();
	drawBorder();
	gotoxy(34, 1); textcolor(GREEN);	cprintf("[ "); textcolor(MAGENTA); cprintf("OPTIONS"); textcolor(GREEN); cprintf(" ]");

	while(1) {
		pressed = false;

		textcolor(YELLOW);	gotoxy(5, 3);		cprintf("Mode");
		textcolor(WHITE);	gotoxy(20, 3);	for(i=0; i<8; i++)	cprintf(" ");
		for(i = 0; i<totModes; i++)
			if(strcmp(currentMode, Modes[i]) == 0)	break;
			gotoxy(5+15 + 3 - strlen(Modes[i])/2, 3);	cprintf("%s", currentMode);	gotoxy(20, 3);
		textcolor(YELLOW);	gotoxy(5, 5);		cprintf("Sound");
		textcolor(colors[soundOn]);	gotoxy(5+15, 5);	cprintf("%s", (soundOn) ? " ON" : "OFF");
		textcolor(YELLOW);	gotoxy(40, 5);	cprintf("Animation");
		textcolor(colors[animationOn]);	gotoxy(40+15, 5);	cprintf("%s", (animationOn) ? " ON" : "OFF");
		textcolor(YELLOW);
		
		while(1) {
			gotoxy(3+x*35,3+y*2);	cprintf(">");
			key = getch();
			gotoxy(3+x*35,3+y*2);	cprintf(" ");
			switch(key) {
			case 'w':	if(y>0 && opt[y-1][x]==1) y--;	break;
			case 's':	if(y<1 && opt[y+1][x]==1) y++;	break;
			case 'a':	if(x>0 && opt[y][x-1]==1) x--;	break;
			case 'd':	if(x<1 && opt[y][x+1]==1) x++;	break;
			case 13:	pressed = true;	break;
			case 27:	return;
			}
			if(pressed)	break;
		}
		if(x == 0 && y == 0)
			Game_optionsMode(this);
		else if(x == 0 && y == 1) {
			soundOn = 1 - soundOn;
		} else if(x == 1 && y == 1) {
			animationOn = 1 - animationOn;
		}
	}
}

// Mode selection
void Game_optionsMode(struct Game *this) {
	int i, x = 0, key, spaces = 0;
	bool pressed = false;
	
	textcolor(WHITE);
	// Print current mode
	for(i = 0; i<totModes; i++)
		if(strcmp(currentMode, Modes[i]) == 0)	x=i;

	while(1) {
		gotoxy(18, 3);	cprintf("<");	gotoxy(28, 3);	cprintf(">");
		spaces = 3 - strlen(Modes[x])/2;	gotoxy(20, 3);	for(i=0; i<8; i++)	cprintf(" ");
		gotoxy(20+spaces, 3);	cprintf("%s", Modes[x]);
		key = getch();
		gotoxy(18, 3);	cprintf(" ");	gotoxy(28, 3);	cprintf(" ");
		switch(key) {
		case 'a':	if(x>0) x--;	break;
		case 'd':	if(x<this->unlockedModes) x++;	break;
		case 13:	pressed = true;	break;
		case 27:	return;
		}
		if(pressed)	break;
	}
	// Set Current Mode
	strcpy(currentMode, Modes[x]);
}

#endif