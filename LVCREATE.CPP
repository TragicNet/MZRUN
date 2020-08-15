/*
	Map maker
	load map from txt file and put in lvl file with map data and objects

*/

#include "MZRUN/GLOBALS.H"

class Creator {
private:
	Save data;
	char cmap[MaxY][MaxX];
	char *list;
public:
	coord *pos;
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
	this->pos->x = this->pos->y = 1;	this->end = false;
}

void Creator :: draw() {
	int i = 0, j;	FILE *file;
	file = fopen("./MZRUN/LEVELS/area1.map","r");
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
}

void Creator :: make_map()  {
}

void Creator :: input() {
	/*gotoxy(1,25);	cout<<this->x<<", "<<this->y;
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
		//case 13:	create_object();
		//			break;
		case 27:   this->end = true;
					break;
	}*/
}

bool Creator :: move(int PosX, int PosY) {
}

void Creator :: create_lvl() {
	//Create Map file
	fullmap = new coord(60, 20);
	int i = 0, j;
	Object *temp;

	FILE *file;
	file = fopen("./MZRUN/area1.map","r");
	while(!feof(file)) {
		fscanf(file, "%s", &cmap[i++]);
	}
	fclose(file);
	for(i = 0; i<fullmap->y; i++) {
		for(j = 0; j<fullmap->x; j++) {
			map[i][j].block();
			if(cmap[i][j]=='@') {
				temp = new Object(j, i, RIGHT, '@', "player", GREEN);
				cmap[i][j] = '.';	entityObjects[0] = *temp;
				map[i][j].unblock();
			} else if(cmap[i][j] == '*') {
				temp = new Object(j, i, STOP, '*', "monster", RED, BLACK, 5); //RED
				cmap[i][j] = '.';
				entityObjects[totalEntities++] = *temp;
			} else if(cmap[i][j] == '#') {
				//map[i][j].block();
			} else {
				map[i][j].unblock();
			}
			temp = NULL;	delete temp;
		}
	}
	this->data.Export("./MZRUN/LEVELS/area2.lvl");
	//cout<<"Done";	getch();
}

int main() {
    clrscr();
	Creator creator;
	creator.create_lvl();
	/*creator.draw();
	while(!creator.end) {
		creator.input();
	}*/
	return 0;
}