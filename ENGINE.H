#ifndef ENGINE_H
#define ENGINE_H

class Engine {
private:
	Save data;
	char paths[50][50];
	bool gameOver;
	bool cleared;
	unsigned int timer;
	Object *player;
	inline int tick() {	return timer/TICKSPEED;	}

public:
	Engine();
	~Engine() {
		free(player);	//free(fullmap);
	}
	void drawEngine();
	void inputEngine();
	void handleEngine();
	//void HUD();
	void death();
	int getPaths(char paths[50][50]);
	inline bool getState() {	return this->gameOver;	};
	inline bool isCleared() {	return this->cleared;	}
	inline unsigned int getTimer()	{	return this->timer;	}
	inline Object* getPlayer()	{	return this->player;	}
};

Engine :: Engine() {
	srand((unsigned)time(0));
	cleared = false;	gameOver = false;	timer = TICKSPEED * TIMELIMIT;
	player = &entityObjects[0];

	int gen = rand() % getPaths(this->paths);
	this->data.Import(strcat("./MZRUN/LEVELS/", paths[gen]));
}

void HUD() {
	int i, j;
	//Map Window
	for(i=1; i<=25; i++) {
		gotoxy(screen.x+boardX,i);	printf("%c", border[1]);
	}
	for(i=1; i<=screen.x+boardX;i++) {
		gotoxy(i,screen.y+boardY);	printf("%c", border[0]);
	}
	gotoxy(screen.x+boardX,screen.y+boardY);	printf("%c", 180);

	gotoxy(2, 24);	printf("Timer: ");
}

void Engine :: drawEngine() {
	int i, j;
	coord camera = view(player->getPos(), screen, fullmap);
	for(i = 0; i<screen.y; i++) {
		for(j = 0; j<screen.x; j++) {
			gotoxy(boardX+j, boardY+i);
			//Vision
			if(player->distance(camera + coord(j, i)) > torchRange) {
				blackair.draw();
				continue;
			}
			int obj = posCheck(camera + coord(j, i));
			if(obj!=-1)	entityObjects[obj].draw();
			else if(goal.getPos() == camera + coord(j, i))	goal.draw();
			else if(map[camera.y + i][camera.x + j].isBlocked()) wall.draw();
			else	air.draw();
		}
	}
	gotoxy(8, 24);
	printf("%3d", timer/TICKSPEED);
}

void Engine :: inputEngine() {
	if(kbhit()) {
		switch(getch()) {
		case 'w':   player->moveObject(+0, -1);	break;
		case 's':   player->moveObject(+0, +1);	break;
		case 'a':   player->moveObject(-1, +0);	break;
		case 'd':   player->moveObject(+1, +0);	break;
		case 'x':   gameOver = true;			break;
		}
	}
}

void Engine :: handleEngine() {
	int i;
	//gotoxy(1, 25);	cprintf("%5u",timer);
	if(posCheck(player->getPos(), "monster")) {		//Die
		drawEngine();	this->death();	return;
	} else if(player->getPos() == goal.getPos()) {	//Win
		player->setColor(WHITE, goal.getBgColor());
		drawEngine();	this->death();
		cleared = true;	return;
	}
	for(i = 1; i<totalEntities; i++) {
		if(entityObjects[i].getSpeed() != 0)
			entityObjects[i].route(timer);
	}
	if( this->tick() < 1 )
		gameOver = true;
	timer--;
}

int Engine :: getPaths(char paths[50][50]) {
	struct dirent *de;
	DIR *dr = opendir("./MZRUN/LEVELS");
	if(dr == NULL) return 0;
	int cnt=0, s;
	char ext[] = ".LVL";
	while((de = readdir(dr)) != NULL)  {
		s = strlen(de->d_name);
		for(int i=s-4; i>0; i--)
			if(de->d_name[i] != ext[s-4-i])	continue;
			else {
				strcpy(paths[cnt++], de->d_name);
			}
	}
	closedir(dr);
	return cnt;
}

void Engine :: death() {
	int i, j, cx = 40, cy = 13;
	int height = 3, width = 12;
	cx-=width/2;	cy-=height/2;
	this->gameOver = true;
	gotoxy(cx, cy++);	cprintf("%c", border[2]);	for(i = 0; i<width-2; i++)	cprintf("%c", border[0]);	cprintf("%c", border[3]);
	gotoxy(cx, cy++);	cprintf("%c You Died %c", border[1], border[1]);
	gotoxy(cx, cy++);	cprintf("%c", border[4]);	for(i = 0; i<width-2; i++)	cprintf("%c", border[0]);	cprintf("%c", border[5]);

	sound(120);	halt(100);	nosound();
	halt(50);
	sound(120);	halt(100);	nosound();
	getch();
}

#endif