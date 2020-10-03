#ifndef ENGINE_H
#define ENGINE_H

struct Engine {
	struct Level levelData;
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

	Level_Import(&this->levelData, paths[gen]);
	
	addState(states, "fullvision1",  Engine_tick(this));
	addState(states, "immunity1",  Engine_tick(this));
	//addState(states, "invisible1",  Engine_tick(this));
	//addState(states, "hinder1",  Engine_tick(this));
	/*addState(states, "paralyze1",  Engine_tick(this));
	addState(states, "stop1",  Engine_tick(this));*/
	//printState(&states[0]);	
}

void Engine_HUD() {
	int i, j;
	clrscr();
	//Outer Window
	drawBorder();
	textcolor(GREEN);
	//Map Window
	for(i = board.y; i < board.y + screen.y; i++) {	//Vertical
		gotoxy(board.x-1, i);	cprintf("%c", border[1]);	gotoxy(board.x+screen.x, i);	cprintf("%c", border[1]);
	}
	for(i = board.x; i < board.x + screen.x; i++) {	//Horizontal
		gotoxy(i, board.y-1);	cprintf("%c", border[0]);	gotoxy(i, board.y+screen.y);	cprintf("%c", border[0]);
	}
	gotoxy(board.x-1, board.y-1);	cprintf("%c", border[2]);	gotoxy(board.x+screen.x, board.y-1);	cprintf("%c", border[3]);
	gotoxy(board.x-1, board.y+screen.y);	cprintf("%c", border[4]);	gotoxy(board.x+screen.x, board.y+screen.y);	cprintf("%c", border[5]);
	gotoxy(board.x+screen.x-7, board.y-1);	textcolor(GREEN); cprintf("[ "); textcolor(MAGENTA); cprintf("MAP"); textcolor(GREEN); cprintf(" ]");
	
	//Bottom
	gotoxy(1, 23);	cprintf("%c", 195);
	for(i=2; i<80; i++) {
		gotoxy(i, 23);	cprintf("%c", border[0]);
	}
	gotoxy(80, 23);	cprintf("%c", 180);
}

void Engine_draw(struct Engine *this) {
	int i, j, objPos, colorVal, tempColor, tempBgColor;
	bool fullvision = false, notVisible;
	struct coord tmp, sumtmp;
	struct Tile *tmpTile;
	struct coord camera = view(this->player->pos, screen, fullmap);
	struct Object *entity;
	tempColor = this->player->color;
	tempBgColor = this->player->bgcolor;
	for(i = 0; i<totStates; i++) {
		if(strcmp(states[i].name, "VIS") == 0) {
			fullvision = true;	break;
		}
	}
	for(i = 0; i<screen.y; i++) {
		for(j = 0; j<screen.x; j++) {
			coord_init(&tmp, j, i);
			tmpTile = &map[camera.y + i][camera.x + j];
			sumtmp = coord_add(camera, tmp);
			gotoxy(board.x+j, board.y+i);
			objPos = posCheck(coord_add(camera, tmp), "", true);
			if( objPos >= 0 ) {
				entity = &entityObjects[objPos];
			}
			notVisible = Object_torchRadius(this->player->pos, sumtmp) > (torchRange * torchRange) && fullvision == false;
			
			//Vision
			if(notVisible) {
				Object_draw(&outVision);
			} else if(!Tile_isOpen(tmpTile)) {
				if(Tile_isBlocked(tmpTile) && !notVisible)
					Object_draw(&wall);
				else
					Object_draw(&outVision);
			} else if(objPos>=0) {
				if( coord_cmp(entity->pos, this->player->pos) ) {
					if(this->gameOver && strcmp(entity->type, "player") == 0) {
						Object_draw(&blackair);	continue;
					} else if( entity->weight < this->player->weight ) {
						colorVal = (entity->bgcolor == BLACK) ? tempColor : WHITE;
						Object_setColor(this->player, colorVal, entity->bgcolor);
						objPos = 0;
					}
				}
				Object_draw(entity);
				Object_setColor(this->player, tempColor, tempBgColor);
			} else {
				Object_draw(&blackair);
			}
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
	for(i = 0; i<totStates; i++) {
		textcolor(GREEN);
		gotoxy(2, 4 + i);	cprintf("%s", states[i].name);
		if(states[i].duration <= 3)	textcolor(RED + BLINK);
		cprintf("%3d", states[i].duration);
	}
	textcolor(WHITE);
}

void script_death(struct Engine *this) {
	int i, j, cx = 40, cy = 13;
	int height = 3, width = 12;
	this->gameOver = true;
	halt(225);
	Engine_draw(this);
	cx-=width/2;	cy-=height/2;
	textcolor(LIGHTRED);
	gotoxy(cx, cy++);	cprintf("%c", border[2]);	for(i = 0; i<width-2; i++)	cprintf("%c", border[0]);	cprintf("%c", border[3]);
	gotoxy(cx, cy++);	cprintf("%c GAMEOVER %c", border[1], border[1]);
	gotoxy(cx, cy++);	cprintf("%c", border[4]);	for(i = 0; i<width-2; i++)	cprintf("%c", border[0]);	cprintf("%c", border[5]);

	sound(100);	halt(100);	nosound();
	halt(15);	sound(100);	halt(100);	nosound();
	getch();
}

void script_fall(struct Engine *this) {
	Engine_draw(this);
	this->gameOver = true;
	//Halt for visible player Death
	noise(30, 50);
	Engine_draw(this);	halt(50);
	noise(25, 75);
	script_death(this);
}

void script_kill() {
	int i;
	for(i=100; i<200; i+=50) {
		noise(i, i-50);
	}
}

void script_eaten(struct Engine *this) {
	int i;
	Engine_draw(this);
	this->gameOver = true;
	for(i=150; i>50; i-=10) {
		noise(i, i-50);
	}
	script_death(this);
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

	noise(100, 100);	halt(5);
	noise(250, 100);	halt(5);
	noise(150, 100);	halt(5);
	noise(300, 100);	halt(5);
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
		case 27:   this->gameOver = true;	
			break;
		}
		if(wait==true)	wait = false;
	}
}

void Object_script(struct Engine *engine, int loc) {
	int i;
	bool immortal = false;
	struct Object *temp = &entityObjects[loc];
	if(strcmp(temp->type, "monster") == 0) {
		//turn immortal on for certain monsters
		if(strcmp(temp->name, "vampire") == 0 || strcmp(temp->name, "ghost") == 0 /*|| strcmp(temp->name, "reaper") == 0*/) {
			immortal = true;
		}
		for(i = 0; i<totStates; i++) {
			if(strcmp(states[i].name, "IMM") == 0) {
				if(!immortal) {
					script_kill();	removeObject(entityObjects, loc);
				}
				return;
			} else if(strcmp(states[i].name, "BRK") == 0) {
				if(!immortal) {
					script_kill();	removeObject(entityObjects, loc);
				}
				removeState(states, i);	gotoxy(2, 4 + totStates);	cprintf("      ");	return;
			}
		}
		script_eaten(engine);
	} else if(strcmp(temp->type, "goal") == 0) {
		script_cleared(engine);
	} else if(addState(states, temp->name, Engine_tick(engine))) {
		removeObject(entityObjects, loc);
	}
}

void Engine_handle(struct Engine *this) {
	int i;
	//gotoxy(1, 25);	cprintf("%5u",this->timer);
	i = posCheck(this->player->pos, "", true);
	if(i > 0) {
		Object_script(this, i);
		if(this->gameOver == true)	return;
	}
	if( Engine_tick(this) < 1 ) {
		script_death(this);	return;
	} else if(!Tile_isOpen(&map[this->player->pos.y][this->player->pos.x])) {
		script_fall(this);	return;
	} 
	for(i = 1; i<totalEntities; i++) {
		if(entityObjects[i].speed != 0)
			Object_route(&entityObjects[i], this->timer);
	}
	//Update States
	for(i = 0; i<totStates; i++) {
		if(states[i].duration <= 0) {
			removeState(states, i);
			gotoxy(2, 4 + totStates);	cprintf("      ");
		}
		else if(states[i].last!=Engine_tick(this)) {
			states[i].duration--;	states[i].last = Engine_tick(this);
		}
	}
	this->timer--;
}

#endif