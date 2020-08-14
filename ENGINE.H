#ifndef ENGINE_H
#define ENGINE_H

//#include "MZRUN/MONSTER.H"

class Engine {
private:
	bool gameOver;
	unsigned int timer;
	Object *player;

public:
	//int totalEntities;
	Engine();
	~Engine() {}
	void drawEngine();
	void inputEngine();
	void handleEngine();
	void HUD();
	inline bool getState() {	return gameOver;	};
};

Engine :: Engine() {
	int i = 0, j;
	gameOver = false;	timer = TICKSPEED * TIMELIMIT;
	Object *temp;
	FILE *file;
	file = fopen("./MZRUN/LEVELS/area1.lvl","r");
	while(!feof(file)) {
		fscanf(file, "%s", &cmap[i++]);
	}
	fclose(file);
	for(i = 0; i<MaxY; i++) {
		for(j = 0; j<MaxX; j++) {
			if(cmap[i][j]=='@') {
				temp = new Object(j, i, RIGHT, '@', "player", GREEN);
				cmap[i][j] = '.';	entityObjects[0] = *temp;
				player = &entityObjects[0];
			} else if(cmap[i][j] == '*') {
				temp = new Object(j, i, STOP, '*', "monster", RED); //RED
				cmap[i][j] = '.';
				entityObjects[totalEntities++] = *temp;
			} else if(cmap[i][j] == '#') {
				map[i][j].block();
			}
			temp = NULL;	delete temp;
		}
	}
}

void temp() {
	int i, j;
	gotoxy(37,10);
	for(i=1; i<=10;i++) {
		if(i==1)	cprintf("%c", border[2]);
		else if(i==10)	cprintf("%c", border[3]);
		else	cprintf("%c", border[0]);
	}
	for(j=1; j<4; j++) {
		gotoxy(37,10+j);
		for(i=1; i<=10;i++) {
			if(i==1 || i==10)	cprintf("%c", border[1]);
			else	cprintf("%c", ' ');
		}
	}
	gotoxy(37,14);
	for(i=1; i<=10;i++) {
		if(i==1)	cprintf("%c", border[4]);
		else if(i==10)	cprintf("%c", border[5]);
		else	cprintf("%c", border[0]);
	}
	getch();
	exit(0);
}

void Engine :: HUD() {
	int i, j;
	{	//Map Window
		/*for(i=1; i<=25; i++) {
			gotoxy(60+boardX,i);	printf("%c", 179);
		}
		for(i=1; i<=60+boardX;i++) {
			gotoxy(i,20+boardY);	printf("%c", 196);
		}
		gotoxy(60+boardX,20+boardY);	printf("%c", 180);
		*/
		for(i=1; i<=25; i++) {
			gotoxy(screen.x+boardX,i);	printf("%c", 179);
		}
		for(i=1; i<=screen.x+boardX;i++) {
			gotoxy(i,screen.y+boardY);	printf("%c", 196);
		}
		gotoxy(screen.x+boardX,screen.y+boardY);	printf("%c", 180);
	}

	gotoxy(3, 24);	printf("Timer: ");
}

void Engine :: drawEngine() {
	int i, j;
	coord camera = view(player->getPos(), screen, fullmap);
	for(i = 0; i<screen.y; i++) {
		for(j = 0; j<screen.x; j++) {
			gotoxy(boardX+j, boardY+i);
			//Vision
			/*if(player->distance(camera + coord(j, i)) > range) {
				//if(map[i][j].isBlocked()) wall.draw();
				//else	blackair.draw();
				blackair.draw();
				continue;
			}*/
			int obj = posCheck(camera + coord(j, i));
			if(obj!=-1)	entityObjects[obj].draw();
			else if(map[camera.y + i][camera.x + j].isBlocked()) wall.draw();
			else	air.draw();
		}
	}
	gotoxy(10, 24);	/*printf("Timer: ");*/
	if(timer/TICKSPEED<10)	printf("00");
	else if(timer/TICKSPEED<100)	printf("0");
	printf("%d", timer/TICKSPEED);
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
	static unsigned int tick = 0;
	int i;
	/*if(posCheck(player->getPos(), "monster")) {
		drawEngine();
		player->death();
		gameOver = true;	return;
	}*/
	if(tick != timer/TICKSPEED) {
		for(i = 1; i<totalEntities; i++) {
			entityObjects[i].route();
		}
	}
	tick = timer/TICKSPEED;
	if( tick < 1 )
		gameOver = true;
	timer--;
}

#endif