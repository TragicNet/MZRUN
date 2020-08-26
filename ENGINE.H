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
	~Engine() {}
	void drawEngine();
	void inputEngine();
	void handleEngine();
	void HUD();
	void death();
	int getPaths(char paths[][50]);
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
	char path[50] = "./MZRUN/LEVELS/";
	strcat(path, this->paths[gen]);
	this->data.Import(path);
}

void Engine :: HUD() {
	int i, j;
	/*char msg[124];
	//Map Window
	for(i=1; i<=25; i++) {
		//gotoxy(screen.x+boardX,i);	cprintf("%c", border[1]);
		sprintf(msg, "%c", border[1]);
		outtextxy(screen.x + boardX, i, msg);
	}
	for(i=1; i<=screen.x+boardX;i++) {
		//gotoxy(i,screen.y+boardY);	cprintf("%c", border[0]);
		sprintf(msg, "%c", border[0]);
		outtextxy(i, screen.y + boardY, msg);
	}
	//gotoxy(screen.x+boardX,screen.y+boardY);	cprintf("%c", 180);
	sprintf(msg, "%c", 180);
	outtextxy(screen.x+boardX,screen.y+boardY, msg);*/

	//gotoxy(2, 24);	cprintf("Timer: ");
	outtextxy(16 , 461, "Timer: ");
}
int page = 0;
char far *vid_mem= (char far *) 0xB8000000L ;
void drawbox (int sr,int sc,int er,int ec,char attr) {
	int r,c;
	char far *v;
	for(r=sr;r<=er;r++) {
		for(c=sc;c<=ec;c++) {
			v=vid_mem+(r*160)+(c*2);	*v=250 ;	v++;	*v=attr;
		}
	}
}
void Engine :: drawEngine() {
	int i, j;
	setactivepage(page);
	setvisualpage(1 - page);
	cleardevice();
	//outtextxy(16 , 461, "Timer: ");
	//setviewport(boardX, boardY, screen.x * 8, screen.y * 8, 1);
	//clearviewport();
	//drawbox (0,0,16,79,12);

	coord camera = view(player->getPos(), screen, fullmap);
	for(i = 0; i<screen.y; i++) {
		for(j = 0; j<screen.x; j++) {
			//gotoxy(boardX+j, boardY+i);
			px = 1 + j * gr.x;	py = 1 + i * gr.y;
			//Vision
			/*if(player->distance(camera + coord(j, i)) > torchRange) {
				blackair.draw();
				continue;
			}*/
			int obj = posCheck(camera + coord(j, i));
			if(obj!=-1)	entityObjects[obj].draw();
			else if(goal.getPos() == camera + coord(j, i))	goal.draw();
			else if(map[camera.y + i][camera.x + j].isBlocked()) wall.draw();
			else	air.draw();
		}
	}
	//gotoxy(8, 24);	cprintf("%3d", timer/TICKSPEED);
	char msg[124];	sprintf(msg, "%3d", timer/TICKSPEED);
	outtextxy(64, 461, msg);
	
	page = 1 - page;
	delay(5);
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

int Engine :: getPaths(char paths[][50]) {
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