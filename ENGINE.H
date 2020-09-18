#ifndef ENGINE_H
#define ENGINE_H

struct Engine {
	struct Save data;
	bool gameOver;
	bool cleared;
	unsigned int timer;
	struct Object *player;
};

int Engine_tick(struct Engine *this)		{	return this->timer/TICKSPEED;	}
bool Engine_getState(struct Engine *this)	{	return this->gameOver;			}
bool Engine_isCleared(struct Engine *this)	{	return this->cleared;			}

void Engine_init(struct Engine *this, int gen) {
	this->cleared = false;	this->gameOver = false;	this->timer = TICKSPEED * TIMELIMIT;
	this->player = &entityObjects[0];
	
	Save_Import(&this->data, paths[gen]);
	
	State_add(states, "fullvision1",  Engine_tick(this));
	State_add(states, "immunity1",  Engine_tick(this));
	/*State_add(states, "invisible1",  Engine_tick(this));
	State_add(states, "hinder1",  Engine_tick(this));
	State_add(states, "paralyze1",  Engine_tick(this));
	State_add(states, "stop1",  Engine_tick(this));*/
}

void Engine_HUD() {
	int i, j;
	clrscr();
	textcolor(GREEN);
	//Outer Window
	for(i=1; i<=24; i++) {	//Vertical
		gotoxy(1, i);	cprintf("%c", border[1]);	gotoxy(80, i);	cprintf("%c", border[1]);
	}
	gotoxy(1, 24);	cprintf("%c", border[4]);	gotoxy(80, 24);	cprintf("%c", border[5]);
	gotoxy(80, 1);	insline();
	gotoxy(1, 1);	cprintf("%c", border[2]);	gotoxy(80, 1);	cprintf("%c", border[3]);
	for(i=2; i<80; i++) {	//Horizontal
		gotoxy(i, 1);	cprintf("%c", border[0]);	gotoxy(i, 25);	cprintf("%c", border[0]);
	}

	//Map Window
	for(i = board.y; i < board.y + screen.y; i++) {	//Vertical
		gotoxy(board.x-1, i);	cprintf("%c", border[1]);	gotoxy(board.x+screen.x, i);	cprintf("%c", border[1]);
	}
	for(i = board.x; i < board.x + screen.x; i++) {	//Horizontal
		gotoxy(i, board.y-1);	cprintf("%c", border[0]);	gotoxy(i, board.y+screen.y);	cprintf("%c", border[0]);
	}
	gotoxy(board.x-1, board.y-1);	cprintf("%c", border[2]);	gotoxy(board.x+screen.x, board.y-1);	cprintf("%c", border[3]);
	gotoxy(board.x-1, board.y+screen.y);	cprintf("%c", border[4]);	gotoxy(board.x+screen.x, board.y+screen.y);	cprintf("%c", border[5]);
	gotoxy(board.x+screen.x-3, board.y-1);	cprintf("Map");
	//gotoxy(board.x-1, board.y-1);	cprintf("%c", 195);	gotoxy(board.x-1, board.y+screen.y);	cprintf("%c", 195);
	//Top
	/*gotoxy(1, 3);	cprintf("%c", 195);
	for(i=2; i<80; i++) {
		gotoxy(i, 3);	cprintf("%c", border[0]);
	}
	gotoxy(80, 3);	cprintf("%c", 180);*/
	//Bottom
	gotoxy(1, 23);	cprintf("%c", 195);
	for(i=2; i<80; i++) {
		gotoxy(i, 23);	cprintf("%c", border[0]);
	}
	gotoxy(80, 23);	cprintf("%c", 180);
}

void Engine_draw(struct Engine *this) {
	int i, j, k, obj;
	bool fullvision = false;
	struct coord tmp, sumtmp;
	struct coord camera = view(this->player->pos, screen, fullmap);
	for(i = 0; i<totStates; i++) {
		if(strcmp(states[i].name, "VIS") == 0) {
			fullvision = true;	break;
		}
	}
	for(i = 0; i<screen.y; i++) {
		for(j = 0; j<screen.x; j++) {
			coord_init(&tmp, j, i);
			sumtmp = coord_add(camera, tmp);
			gotoxy(board.x+j, board.y+i);
			obj = posCheck(coord_add(camera, tmp), "", true);
			//Vision
			if(Object_torchRadius(this->player->pos, sumtmp) > torchRange && fullvision == false) {
				Object_draw(&blackair);
			} else if(obj>=0)	{
				if( coord_cmp(entityObjects[obj].pos, this->player->pos) && entityObjects[obj].weight < this->player->weight) {
					Object_setColor(this->player, WHITE, entityObjects[obj].bgcolor);	obj = 0;
				}
				Object_draw(&entityObjects[obj]);
			} else if(Tile_isBlocked(&map[camera.y + i][camera.x + j])) Object_draw(&wall);
			else	Object_draw(&air);
		}
	}
	gotoxy(3, 2);	textcolor(YELLOW);	cprintf("Level:");	textcolor(WHITE);	cprintf("%2d", levelCnt);
	gotoxy(13, 2);	textcolor(YELLOW);	cprintf("Mode:");	textcolor(WHITE);	cprintf("%s", currentMode);
	gotoxy(3, 24);	textcolor(YELLOW);	
	if(wait == false) {
		cprintf("Timer:");	textcolor(WHITE);	cprintf("%3d ", Engine_tick(this));
	} else cprintf("<<PAUSED>>");
	textcolor(CYAN);
	gotoxy(9, 19);	cprintf("  w: UP       ");	gotoxy(9, 20);	cprintf("  a: LEFT     ");
	gotoxy(9, 21);	cprintf("  s: DOWN     ");	gotoxy(9, 22);	cprintf("  d: RIGHT    ");
	gotoxy(23, 19);	cprintf("  h: HELP     ");	gotoxy(23, 20);	cprintf("  p: PAUSE    ");
	gotoxy(23, 21);	cprintf("esc: EXIT     ");	gotoxy(23, 22);	cprintf("rtn: ENTER    ");
	textcolor(YELLOW);
	for(i = 0; i<totStates; i++) {
		gotoxy(2, 4 + i);	cprintf("%s ", states[i].name);
		cprintf("%2d", states[i].duration);
	}
	textcolor(WHITE);
}

void script_death(struct Engine *this) {
	int i, j, cx = 40, cy = 13;
	int height = 3, width = 12;
	Engine_draw(this);
	cx-=width/2;	cy-=height/2;
	this->gameOver = true;
	textcolor(LIGHTRED);
	gotoxy(cx, cy++);	cprintf("%c", border[2]);	for(i = 0; i<width-2; i++)	cprintf("%c", border[0]);	cprintf("%c", border[3]);
	gotoxy(cx, cy++);	cprintf("%c GAMEOVER %c", border[1], border[1]);
	gotoxy(cx, cy++);	cprintf("%c", border[4]);	for(i = 0; i<width-2; i++)	cprintf("%c", border[0]);	cprintf("%c", border[5]);

	sound(120);	halt(100);	nosound();
	halt(15);
	sound(120);	halt(100);	nosound();
	getch();
}

void script_cleared(struct Engine *this) {
	int i, j, cx = 40, cy = 13;
	int height = 3, width = 11;
	Engine_draw(this);
	cx-=width/2;	cy-=height/2;
	this->gameOver = true;
	this->cleared = true;
	textcolor(LIGHTGREEN);
	gotoxy(cx, cy++);	cprintf("%c", border[2]);	for(i = 0; i<width-2; i++)	cprintf("%c", border[0]);	cprintf("%c", border[3]);
	gotoxy(cx, cy++);	cprintf("%c CLEARED %c", border[1], border[1]);
	gotoxy(cx, cy++);	cprintf("%c", border[4]);	for(i = 0; i<width-2; i++)	cprintf("%c", border[0]);	cprintf("%c", border[5]);

	sound(100);	halt(100);	nosound();
	halt(5);
	sound(250);	halt(100);	nosound();
	halt(5);
	sound(150);	halt(100);	nosound();
	halt(5);
	sound(300);	halt(100);	nosound();
	getch();
}

void Engine_input(struct Engine *this) {
	int i;
	bool paralyzed = false, hindered = false;
	for(i = 0; i<totStates; i++) {
		if(strcmp(states[i].name, "PAR") == 0)	paralyzed = true;
		else if(strcmp(states[i].name, "HND") == 0) if(random(10)%2 == 0)	hindered = true;
	}
	if(kbhit()) {
		switch(getch()) {
		case 'w':   if(!paralyzed && !hindered)	Object_move(this->player, +0, -1);
			break;
		case 's':   if(!paralyzed && !hindered)	Object_move(this->player, +0, +1);
			break;
		case 'a':   if(!paralyzed && !hindered)	Object_move(this->player, -1, +0);
			break;
		case 'd':   if(!paralyzed && !hindered)	Object_move(this->player, +1, +0);
			break;
		case 'h':	help();	Engine_HUD();
		case 'p':	wait = true;	return;
		case 'x':	exit(0);
		case 27:   this->gameOver = true;	break;
		}
		if(wait==true)	wait = false;
	}
}

void Object_script(struct Engine *engine, int loc) {
	int i;
	if(strcmp(entityObjects[loc].type, "monster") == 0) {
		for(i = 0; i<totStates; i++) {
			if(strcmp(states[i].name, "IMM") == 0) {
				Object_remove(entityObjects, loc);	return;
			} else if(strcmp(states[i].name, "BRK") == 0) {
				Object_remove(entityObjects, loc);	State_remove(states, i);
				gotoxy(2, 4 + totStates);	cprintf("      ");	return;
			}
		}
		script_death(engine);
	} else if(strcmp(entityObjects[loc].type, "goal") == 0) {
		script_cleared(engine);
	} else if(State_add(states, entityObjects[loc].name, Engine_tick(engine))) {
		Object_remove(entityObjects, loc);
	}
}

void Engine_handle(struct Engine *this) {
	int i;
	//gotoxy(1, 25);	cprintf("%5u",timer);
	i = posCheck(this->player->pos, "", true);
	if(i > 0) {
		Object_script(this, i);
		if(this->gameOver == true)	return;
	}
	if( Engine_tick(this) < 1 ) {
		script_death(this);	return;
	}
	for(i = 1; i<totalEntities; i++) {
		if(entityObjects[i].speed != 0)
			Object_route(&entityObjects[i], this->timer);
	}
	//Update States
	for(i = 0; i<totStates; i++) {
		if(states[i].duration <= 0) {
			State_remove(states, i);
			gotoxy(2, 4 + totStates);	cprintf("      ");
		}
		else if(states[i].last!=Engine_tick(this)) {
			states[i].duration--;	states[i].last = Engine_tick(this);
		}
	}
	this->timer--;
}

int getPaths(char paths[][50]) {
	struct dirent *de;
	int i, cnt=0, s;
	char ext[] = ".LVL";
	DIR *dr = opendir("./MZRUN/LEVELS");
	if(dr == NULL) return 0;
	while((de = readdir(dr)) != NULL)  {
		s = strlen(de->d_name);
		/*for(i=s-4; i>0; i--) {
			if(de->d_name[i] != ext[s-4-i])	continue;
			else {
				pathsCleared[cnt] = false;
				strcpy(paths[cnt], "./MZRUN/LEVELS/");
				strcat(paths[cnt++], de->d_name);
			}
		}*/
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

#endif