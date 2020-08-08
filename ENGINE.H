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
	inline bool getState() {	return gameOver;	};
};

Engine :: Engine() {
	int i = 0, j;
	gameOver = false;	timer = TICKSPEED*60;
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
				temp = new Object(i, j, RIGHT, '@', "player", GREEN);
				cmap[i][j] = '.';	entityObjects[0] = *temp;
				player = &entityObjects[0];
			} else if(cmap[i][j] == '*') {
				temp = new Object(i, j, STOP, '*', "monster", RED);
				cmap[i][j] = '.';
				entityObjects[totalEntities++] = *temp;
			} else if(cmap[i][j] == '#') {
				map[i][j].block();
			}
			temp = NULL;	delete temp;
		}
	}
}

void Engine :: drawEngine() {
	for(int i = 0; i<MaxY; i++) {
		for(int j = 0; j<MaxX; j++) {
			gotoxy(boardX+j, boardY+i);
			/*if(player->distance(i, j) > range && ( j!=0 && i!=0 && j!=MaxX-1 && i!=MaxY-1 ) ) {
				//if(map[i][j].isBlocked()) wall.draw();
				//else	blackair.draw();
				blackair.draw();
				continue;
			}*/
			int obj = posCheck(j, i);
			if(obj!=-1)	entityObjects[obj].draw();
			else if(map[i][j].isBlocked()) wall.draw();
			else	air.draw();
		}
	}
	gotoxy(1, 25);	printf("Timer: ");
	if(timer/TICKSPEED<10)	printf("0");
	printf("%d", timer/TICKSPEED);
}

void Engine :: inputEngine() {
	if(kbhit()) {
		switch(getch()) {
			case 'w':   player->moveObject(+0, -1);	playerMoved = true;
						break;
			case 's':   player->moveObject(+0, +1);	playerMoved = true;
						break;
			case 'a':   player->moveObject(-1, +0);	playerMoved = true;
						break;
			case 'd':   player->moveObject(+1, +0);	playerMoved = true;
						break;
			case 'x':   gameOver = true;
						break;
		}
	}
}

void Engine :: handleEngine() {
	static unsigned int tick = 0;
	int i;
	
	if(posCheck(player->getX(), player->getY(), "monster")) {
		drawEngine();
		player->death();
		gameOver = true;	return;
	}
	if(tick != timer/TICKSPEED) {
	//if(playerMoved == true) {
		playerMoved = false;
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