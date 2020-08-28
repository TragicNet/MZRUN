#ifndef ENGINE_H
#define ENGINE_H

struct Engine {
	struct Save data;
	char paths[50][50];
	bool gameOver;
	bool cleared;
	unsigned int timer;
	struct Object *player;
};

int Engine_tick(struct Engine *this)		{	return this->timer/TICKSPEED;	}
bool Engine_getState(struct Engine *this)	{	return this->gameOver;			}
bool Engine_isCleared(struct Engine *this)	{	return this->cleared;			}

void Engine_init(struct Engine *this) {
	int gen;
	char path[50] = "./MZRUN/LEVELS/";
	srand((unsigned)time(0));
	gen = rand() % Engine_getPaths(this->paths);
	this->cleared = false;	this->gameOver = false;	this->timer = TICKSPEED * TIMELIMIT;
	this->player = &entityObjects[0];

	strcat(path, this->paths[gen]);
	Save_Import(&this->data, path);
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
	
	//Bottom
	for(i=2; i<80; i++) {
		gotoxy(i, 23);	cprintf("%c", border[0]);
	}
	gotoxy(1, 23);	cprintf("%c", 195);
	gotoxy(80, 23);	cprintf("%c", 180);
}

void Engine_draw(struct Engine *this) {
	int i, j, obj;
	struct coord tmp;
	struct coord camera = view(this->player->pos, screen, fullmap);
	for(i = 0; i<screen.y; i++) {
		for(j = 0; j<screen.x; j++) {
			coord_init(&tmp, j, i);
			gotoxy(board.x+j, board.y+i);
			//Vision
			if(Object_torchRadius(this->player, coord_add(camera, tmp)) > torchRange) {
				Object_draw(&blackair);
				continue;
			}
			obj = posCheck(coord_add(camera, tmp), "", true);
			if(obj>=0)	{
				if( coord_cmp(entityObjects[obj].pos, this->player->pos) && entityObjects[obj].weight < this->player->weight) {
					Object_setColor(this->player, WHITE, entityObjects[obj].bgcolor);	obj = 0;
				}
				Object_draw(&entityObjects[obj]);
			} else if(Tile_isBlocked(&map[camera.y + i][camera.x + j])) Object_draw(&wall);
			else	Object_draw(&air);
		}
	}
	gotoxy(3, 24);
	textcolor(YELLOW);	cprintf("Timer:");	textcolor(WHITE);
	cprintf("%3d", this->timer/TICKSPEED);
}

void script_death(struct Engine *this) {
	int i, j, cx = 40, cy = 13;
	int height = 3, width = 12;
	Engine_draw(this);
	cx-=width/2;	cy-=height/2;
	this->gameOver = true;
	textcolor(GREEN);	highvideo();
	gotoxy(cx, cy++);	cprintf("%c", border[2]);	for(i = 0; i<width-2; i++)	cprintf("%c", border[0]);	cprintf("%c", border[3]);
	gotoxy(cx, cy++);	cprintf("%c GAMEOVER %c", border[1], border[1]);
	gotoxy(cx, cy++);	cprintf("%c", border[4]);	for(i = 0; i<width-2; i++)	cprintf("%c", border[0]);	cprintf("%c", border[5]);

	sound(120);	halt(100);	nosound();
	halt(50);
	sound(120);	halt(100);	nosound();
	normvideo();
	getch();
}

void script_cleared(struct Engine *this) {
	int i, j, cx = 40, cy = 13;
	int height = 3, width = 12;
	Engine_draw(this);
	cx-=width/2;	cy-=height/2;
	this->gameOver = true;
	textcolor(GREEN);	highvideo();
	gotoxy(cx, cy++);	cprintf("%c", border[2]);	for(i = 0; i<width-2; i++)	cprintf("%c", border[0]);	cprintf("%c", border[3]);
	gotoxy(cx, cy++);	cprintf("%c GAMEOVER %c", border[1], border[1]);
	gotoxy(cx, cy++);	cprintf("%c", border[4]);	for(i = 0; i<width-2; i++)	cprintf("%c", border[0]);	cprintf("%c", border[5]);

	sound(120);	halt(100);	nosound();
	halt(50);
	sound(120);	halt(100);	nosound();
	normvideo();
	getch();
}

void Engine_input(struct Engine *this) {
	if(kbhit()) {
		switch(getch()) {
		case 'w':   Object_move(this->player, +0, -1);	break;
		case 's':   Object_move(this->player, +0, +1);	break;
		case 'a':   Object_move(this->player, -1, +0);	break;
		case 'd':   Object_move(this->player, +1, +0);	break;
		case 'x':   this->gameOver = true;			break;
		}
	}
}

void Object_script(struct Engine *engine, struct Object *this) {
	overlapping = true;
	if(strcmp(this->type, "monster")) {
		script_death(engine);
	} else if(strcmp(this->type, "goal")) {
		script_cleared(engine);
	}
	overlapping = false;
}

void Engine_handle(struct Engine *this) {
	int i;
	//gotoxy(1, 25);	cprintf("%5u",timer);
	/*i = posCheck(this->player->pos, "goal", false);
	gotoxy(1, 1);	printf("%d: %d, %d : %s", i, entityObjects[i].pos.x, entityObjects[i].pos.y, entityObjects[i].type);*/
	i = posCheck(this->player->pos, "", true);
	if(i > 0) {
		Object_script(this, &entityObjects[i]);
		if(this->gameOver == true)	return;
	}
	for(i = 1; i<totalEntities; i++) {
		if(entityObjects[i].speed != 0)
			Object_route(&entityObjects[i], this->timer);
	}
	if( Engine_tick(this) < 1 )
		this->gameOver = true;
	this->timer--;
}

int Engine_getPaths(char paths[][50]) {
	struct dirent *de;
	int i, cnt=0, s;
	char ext[] = ".LVL";
	DIR *dr = opendir("./MZRUN/LEVELS");
	if(dr == NULL) return 0;
	while((de = readdir(dr)) != NULL)  {
		s = strlen(de->d_name);
		for(i=s-4; i>0; i--) {
			if(de->d_name[i] != ext[s-4-i])	continue;
			else strcpy(paths[cnt++], de->d_name);
		}
	}
	closedir(dr);
	return cnt;
}

#endif