#ifndef ENGINE_H
#define ENGINE_H

void script_death(struct Engine *this);
void script_fall(struct Engine *this);
void script_kill();
void script_eaten(struct Engine *this);
void script_cleared(struct Engine *this);

struct Engine {
	struct Level levelData;
	bool gameOver;
	bool cleared;
	unsigned int timer;
	//struct Tile map[MaxY][MaxX];
	struct Object *player;
};

int Engine_tick(struct Engine *this)		{	return this->timer/TICKSPEED;	}
bool Engine_getState(struct Engine *this)	{	return this->gameOver;			}
bool Engine_isCleared(struct Engine *this)	{	return this->cleared;			}

void Engine_init(struct Engine *this, char *path) {
	int i, j;
	for(i = 0; i < fullmap.y; i++) {
		for(j = 0; j < fullmap.x; j++) {
			Tile_init(&this->levelData.map[i][j], false, false, false, true);
		}
	}
	this->cleared = false;	this->gameOver = false;	this->timer = TICKSPEED * TIMELIMIT;
	this->player = &entityObjects[0];
	wait = true;
	Level_Import(&this->levelData, path);
	do {
		i = random(4);
	} while(i != random(4));
	coord_cpy(&this->player->pos, this->levelData.player_pos[i]);

	addState(states, "fullvision1",  Engine_tick(this));
	addState(states, "immunity1",  Engine_tick(this));
	
	//addState(states, "invisible1",  Engine_tick(this));
	//addState(states, "hinder1",  Engine_tick(this));
	//addState(states, "paralyze1",  Engine_tick(this));
	//addState(states, "stop1",  Engine_tick(this));
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
	struct coord tmp;
	struct Tile *thisTile;
	struct coord camera = display(this->player->pos, screen, fullmap);
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
			thisTile = &this->levelData.map[camera.y + i][camera.x + j];
			gotoxy(board.x+j, board.y+i);
			objPos = posCheck(coord_add(camera, tmp), "", true);
			if( objPos >= 0 ) {
				entity = &entityObjects[objPos];
			}
			notVisible = Tile_view(this->player->pos, coord_add(camera, tmp)) >= (torchRange * torchRange) && fullvision == false;
			
			//Vision
			if(notVisible) {
				Object_draw(&outVision);
			} else if(objPos>=0 && !Tile_isBlocked(thisTile) && !(objPos == 0 && this->gameOver)) {
				if( coord_cmp(entity->pos, this->player->pos) ) {
					if( entity->weight < this->player->weight ) {
						colorVal = (entity->bgcolor == BLACK) ? tempColor : WHITE;
						Object_setColor(this->player, colorVal, entity->bgcolor);
						Object_draw(this->player);
						Object_setColor(this->player, tempColor, tempBgColor);
					}
				}
				Object_draw(entity);
				Object_setColor(this->player, tempColor, tempBgColor);
			} else if(!Tile_isOpen(thisTile)) {
				Object_draw(&pit);
			} else if(Tile_isBlocked(thisTile)) {
				Object_draw(&wall);
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

void Engine_input(struct Engine *this) {
	int i;
	bool paralyzed = false, hindered = false;
	for(i = 0; i<totStates; i++) {
		if(strcmp(states[i].name, "PAR") == 0)	paralyzed = true;
		else if(strcmp(states[i].name, "HND") == 0) if(random(100)%2 == 0)	hindered = true;
	}
	if(kbhit()) {
		switch(getch()) {
		case 'w':   if(!paralyzed && !hindered)	Object_move(this->player, +0, -1, this->levelData.map);
			break;
		case 's':   if(!paralyzed && !hindered)	Object_move(this->player, +0, +1, this->levelData.map);
			break;
		case 'a':   if(!paralyzed && !hindered)	Object_move(this->player, -1, +0, this->levelData.map);
			break;
		case 'd':   if(!paralyzed && !hindered)	Object_move(this->player, +1, +0, this->levelData.map);
			break;
		case 'h':	help();	Engine_HUD();
		case 'p':	wait = true;	return;
		case 27:   this->gameOver = true;	
			break;
		}
		if(wait==true)	wait = false;
	}
}

void Engine_script(struct Engine *engine, int loc, struct Tile map[][90]) {
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
					script_kill();	removeObject(entityObjects, loc, map);
				}
				return;
			} else if(strcmp(states[i].name, "BRK") == 0) {
				if(!immortal) {
					script_kill();	removeObject(entityObjects, loc, map);
				}
				removeState(states, i);	gotoxy(2, 4 + totStates);	cprintf("      ");	return;
			}
		}
		script_eaten(engine);
	} else if(strcmp(temp->type, "goal") == 0) {
		script_cleared(engine);
	} else if(addState(states, temp->name, Engine_tick(engine))) {
		removeObject(entityObjects, loc, map);
	}
}

void Engine_handle(struct Engine *this) {
	int i;
	//gotoxy(1, 25);	cprintf("%5u",this->timer);
	i = posCheck(this->player->pos, "", true);
	if(i > 0) {
		Engine_script(this, i, this->levelData.map);
		if(this->gameOver == true)	return;
	}
	if( Engine_tick(this) < 1 ) {
		script_death(this);	return;
	} else if(!Tile_isOpen(&this->levelData.map[this->player->pos.y][this->player->pos.x])) {
		script_fall(this);	return;
	} 
	for(i = 1; i<totalEntities; i++) {
		if(entityObjects[i].speed != 0)
			Object_route(&entityObjects[i], this->timer, this->levelData.map);
	}
	//Update States
	for(i = 0; i<totStates; i++) {
		if(states[i].duration <= 0) {
			removeState(states, i);
			gotoxy(2, 4 + totStates);	cprintf("      ");
		} else if(states[i].last!=Engine_tick(this)) {
			states[i].duration--;	states[i].last = Engine_tick(this);
		}
	}
	this->timer--;
}

#endif