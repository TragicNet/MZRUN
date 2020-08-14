/*
	Map maker
	load map from txt file and put in lvl file with map data and objects

*/

#include "MZRUN/ENGINE.H"

class Creator {
private:
	Engine *level;
	char cmap[MaxY][MaxX];
	char *list;
public:
	int x, y;
	bool end;
	Creator();
	void draw();
	void input();
	bool move(int PosX, int PosY);
	void create_object(char c);
	void make_map();
	void create_lvl();
}

Creator :: Creator() {
	this->x = this->y = 1;	this->end = false;
	this->level = new Engine();
}

void Creator :: draw() {
	int i = 0, j;	FILE *file;
	file = fopen("./MZRUN/LEVELS/area1.lvl","r");
	while(!feof(file)) {
		fscanf(file, "%s", &cmap[i++]);
	}
	fclose(file);
	textcolor(BLACK);	textbackground(WHITE);
	for(i = 0; i<MaxY; i++) {
		for(j = 0; j<MaxX; j++) {
			gotoxy(1+j, 1+i);
			if(cmap[0][0] == '#')	cprintf("%c", cmap[i][j]);
			else	cprintf(".");
		}
	}
	textcolor(WHITE);	textbackground(BLACK);
	for(i = 0; i<80; i++) {	//MaxX
		gotoxy(1+i, MaxY+1);	printf("-");
	}
	for(i = 0; i<25; i++) {	//MaxY
		gotoxy(MaxX+1, 1+i);	printf("|");
	}
	char *t = { "@#*," };
	list = t;
	gotoxy(47, 1);	printf("@: Player");
	gotoxy(47, 2);	printf("#: Wall");
	gotoxy(47, 3);	printf("*: Monster");
	gotoxy(47, 4);	printf(",: Door Close");	//_ Door Open
	gotoxy(47, 5);	printf("direction: Stop 0, Up 1, Down 2, ");
	gotoxy(47, 6);	printf("           Left 3, Right 4");
}

void Creator :: create_object(char c) {
	Object *temp;
	if(c == '@') {
		temp = new Object(this->y, this->x, STOP, '@', GREEN);
	} else {
		gotoxy("")
		if(c == '*') {
			temp = new Object(this->y, this->x, STOP, '*', RED);
		} else
			return;
	}
	if(c == '@') {
		entityObjects[0].erase('.', BLACK, WHITE);
		entityObjects[0] = *temp;
		entityObjects[0].draw();
	} else {
		for(int i = 1; i < level->totalEntities; i++) {
			if(entityObjects[i].isThere(this->x, this->y))
				break;
		}
		if(i == level->totalEntities) {
			entityObjects[level->totalEntities++] = *temp;
			entityObjects[level->totalEntities-1].draw();
		} else {
			entityObjects[i] = *temp;
			entityObjects[i].draw();
		}
	}
	map[this->y-1][this->x-1].occupy();
	map[this->y-1][this->x-1].unblock();
	temp = NULL;	delete temp;
}

void Creator :: make_map()  {

}

void Creator :: input() {
	gotoxy(1,25);	cout<<this->x<<", "<<this->y;
	gotoxy(this->x, this->y);
	switch(getch()) {
		case 'w':   this->move(+0, -1);
					break;
		case 's':   this->move(+0, +1);
					break;
		case 'a':   this->move(-1, +0);
					break;
		case 'd':   this->move(+1, +0);
					break;
		case '3':
		case '#':	wall.draw();
					map[this->y-1][this->x-1].block();
					break;
		case '.':	air.draw();
					map[this->y-1][this->x-1].unblock();
					for(int i = 1; i < level->totalEntities; i++) {
						if(entityObjects[i].isThere(this->x, this->y)) {
							for(int j = i; j < level->totalEntities; j++) {
								entityObjects[j] = entityObjects[j+1];
							}
							level->totalEntities--;
							break;
						}
					}
					break;
		case '@':	create_object('@');
					break;
		case '*':	create_object('*');
					break;
		/*case 13:	create_object();
					break;*/
		case 27:   this->end = true;
					break;
	}
}

bool Creator :: move(int PosX, int PosY) {
	if(this->x+PosX<=0 || this->x+PosX>MaxX || this->y+PosY<=0 || this->y+PosY>MaxY)
		return false;
	this->x += PosX;	this->y += PosY;
	return true;
}

void Creator :: create_lvl() {
	ofstream file("a1.lvl", ios::out | ios::binary );
	file.write((char*) &this->level, sizeof(Engine));
	file.close();
}

int main() {
    clrscr();
	Creator creator;
	creator.draw();
	while(!creator.end) {
		creator.input();
	}
	return 0;
}