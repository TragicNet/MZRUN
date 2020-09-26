/*
	Map maker
	load map from txt file and put in lvl file with map data and objects

*/

#include "MZRUN/GLOBALS.H"

char cmap[MaxY][MaxX];

struct Creator {
	struct Level data;
	char cmap[MaxY][MaxX];
	char list[50];
	struct coord pos;
	bool end;
	/*
	void draw();
	void input();
	bool move(int PosX, int PosY);
	void create_object(char c);
	void make_map();
	void create_lvl();*/
};

void Creator_init(struct Creator *this) {
	this->pos.x = this->pos.y = 1;	this->end = false;
}

void Creator_draw(struct Creator *this) {
}

void Creator_create_object(struct Creator *this, char c) {
}

void Creator_make_map(struct Creator *this)  {
}

void Creator_input(struct Creator *this) {
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

bool Creator_move(struct Creator *this, int PosX, int PosY) {
	return true;
}

void Creator_create_lvl(struct Creator *this) {
	int i = 0, j;
	struct Object *temp;
	struct tmp;
	FILE *file;
	//Create Map file
	file = fopen("MZRUN/map.txt","r");
	while(!feof(file)) {
		fscanf(file, "%s", &cmap[i++]);
	}
	coord_init(&fullmap, strlen(cmap[0]), i);
	fclose(file);
	for(i = 0; i<fullmap.y; i++) {
		for(j = 0; j<fullmap.x; j++) {
			Tile_init(&map[i][j], true, false, false, true);
			//coord_init(&tmp, j, i);
			if(cmap[i][j]=='@') {
				temp = (struct Object*) malloc(sizeof(struct Object));
				Object_init(temp, j, i, RIGHT, '@', "player", "player", GREEN, BLACK, 0, 0, 1);
				cmap[i][j] = '.';	entityObjects[0] = *temp;	free(temp);
				Tile_unblock(&map[i][j]);
			} else if(cmap[i][j] == '*') {
				temp = (struct Object*) malloc(sizeof(struct Object));
				Object_init(temp, j, i, STOP, '*', "monster", "normal", RED, BLACK, 5, 4, 1); //RED
				cmap[i][j] = '.';	entityObjects[totalEntities++] = *temp;	free(temp);
				Tile_unblock(&map[i][j]);	map[i][j].playerOnly = true;
			} else if(cmap[i][j] == '#') {
				Tile_blockSight(&map[i][j]);	Tile_close(&map[i][j]);
			} else if(cmap[i][j] == 'g') {
				temp = (struct Object*) malloc(sizeof(struct Object));
				Object_init(temp, j, i, STOP, 'g', "goal", "goal1", CYAN, CYAN, 0, 0, 0);
				entityObjects[totalEntities++] = *temp;	free(temp);
				cmap[i][j] = '.';
				Tile_unblock(&map[i][j]);	map[i][j].playerOnly = true;
			} else if(cmap[i][j] == 's') {
				temp = (struct Object*) malloc(sizeof(struct Object));
				Object_init(temp, j, i, STOP, 4, "effect", "slow1", LIGHTBLUE, BLACK, 0, 0, 0);
				entityObjects[totalEntities++] = *temp;	free(temp);
				cmap[i][j] = '.';	Tile_unblock(&map[i][j]);
				//map[i][j].playerOnly = true;
			} else if(cmap[i][j] == 'f') {
				temp = (struct Object*) malloc(sizeof(struct Object));
				Object_init(temp, j, i, STOP, 4, "effect", "fast1", LIGHTBLUE, BLACK, 0, 0, 0);
				entityObjects[totalEntities++] = *temp;	free(temp);
				cmap[i][j] = '.';	Tile_unblock(&map[i][j]);
				//map[i][j].playerOnly = true;
			} else if(cmap[i][j] == 'x') {
				temp = (struct Object*) malloc(sizeof(struct Object));
				Object_init(temp, j, i, STOP, 4, "effect", "breaker1", LIGHTBLUE, BLACK, 0, 0, 0);
				entityObjects[totalEntities++] = *temp;	free(temp);
				cmap[i][j] = '.';	Tile_unblock(&map[i][j]);
				//map[i][j].playerOnly = true;
			} else if(cmap[i][j] == 'r') {
				temp = (struct Object*) malloc(sizeof(struct Object));
				Object_init(temp, j, i, STOP, 4, "effect", "repel1", LIGHTBLUE, BLACK, 0, 0, 0);
				entityObjects[totalEntities++] = *temp;	free(temp);
				cmap[i][j] = '.';	Tile_unblock(&map[i][j]);
				//map[i][j].playerOnly = true;
			} else if(cmap[i][j] == 'a') {
				temp = (struct Object*) malloc(sizeof(struct Object));
				Object_init(temp, j, i, STOP, 4, "effect", "attract1", LIGHTBLUE, BLACK, 0, 0, 0);
				entityObjects[totalEntities++] = *temp;	free(temp);
				cmap[i][j] = '.';	Tile_unblock(&map[i][j]);
				//map[i][j].playerOnly = true;
			} else if(cmap[i][j] == 'v') {
				temp = (struct Object*) malloc(sizeof(struct Object));
				Object_init(temp, j, i, STOP, 4, "effect", "fullvision1", LIGHTBLUE, BLACK, 0, 0, 0);
				entityObjects[totalEntities++] = *temp;	free(temp);
				cmap[i][j] = '.';	Tile_unblock(&map[i][j]);
				//map[i][j].playerOnly = true;
			} else if(cmap[i][j] == 'V') {
				cmap[i][j] = ' ';	Tile_unblock(&map[i][j]);	Tile_close(&map[i][j]);
			} else {
				Tile_unblock(&map[i][j]);
			}
		}
	}
	Level_Export(&this->data, "MZRUN/LEVELS/AREA1.lvl");
}

int main() {
	struct Creator creator;
	clrscr();
	Creator_create_lvl(&creator);
	/*creator.draw();
	while(!creator.end) {
		creator.input();
	}*/
	return 0;
}