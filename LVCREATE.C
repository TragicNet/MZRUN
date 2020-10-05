/*
	Map maker
	load map from txt file and put in lvl file with map data and objects

*/

char fileName[50] = "AREA4";

#include "MZRUN/INCLUDES/GLOBALS.H"

struct Creator {
	struct Level data;
	struct Tile map[MaxY][MaxX];
	int cmap[MaxY][MaxX];
	struct coord pos;
	char paths[50][50];
	bool end;
	int totFiles;
};
char src[50], trg[50];

void Creator_init(struct Creator *this) {
	int i, j;
	for(i = 0; i < fullmap.y; i++) {
		for(j = 0; j < fullmap.x; j++) {
			Tile_init(&this->map[i][j], false, false, false, true);
		}
	}
	this->pos.x = this->pos.y = 1;	this->end = false;	this->totFiles = 0;
}

/*void Creator_draw(struct Creator *this) {
}

void Creator_create_object(struct Creator *this, char c) {
}

void Creator_make_map(struct Creator *this)  {
}

void Creator_input(struct Creator *this) {
}

bool Creator_move(struct Creator *this, int PosX, int PosY) {
}*/

int Creator_getPaths(struct Creator *this) {
	struct dirent *de;
	int i, cnt=0, s;
	char ext[] = ".TXT", tmp[50];
	DIR *dr = opendir("MZRUN/files/TXTFILES");
	if(dr == NULL) return 0;
	while((de = readdir(dr)) != NULL)  {
		s = strlen(de->d_name);
		for(i = s-4; i < s; i++) {
			if(de->d_name[i] != ext[i-(s-4)])	break;
		}
		if(i==s) {
			for(i = 0; i<s-4; i++) {
				tmp[i] = de->d_name[i];
			}
			tmp[i] = '\0';
			strcpy(this->paths[cnt++], tmp);
			//printf("\n%s", paths[cnt-1]);
		}
	}
	closedir(dr);
	return cnt;
}

void Creator_create_lvl(struct Creator *this) {
	int i = 0, j = 0, cnt = 0;
	char ch;
	struct Object temp;
	FILE *file;
	char fileName[50] = "MZRUN/files/TXTFILES/";
	coord_init(&fullmap, 90, 30);
		//Create Map file
		strcpy(fileName, src);
		file = fopen(fileName, "r");
		//file = fopen("MZRUN/files/TXTFILES/area4.txt", "r");
		if( file == NULL) {
			clrscr();	printf("\n! ERROR: %s missing !", fileName);	getch();	exit(0);
		}
		/*while(!feof(file)) {
			fscanf(file, "%s", &this->cmap[i++]);
		}*/
		while((ch = fgetc(file)) != EOF ) {
			if(ch == '\n') {	i = 0;	j++;	continue;	}
			this->cmap[j][i] = ch;
			i++;
		}
		fclose(file);
		totalEntities = 1;
		/*gotoxy(1, 1);
		for(i = 0; i<20; i++) {
			for(j = 0; j<60; j++) {
				printf("%c", this->cmap[i][j]);
			}
			printf("\n");
		}
		getch();	exit(0);*/
		for(i = 0; i<fullmap.y; i++) {
			for(j = 0; j<fullmap.x; j++) {
				Tile_init(&this->map[i][j], true, false, false, true);
				if(this->cmap[i][j]=='@') {
					Object_init(&temp, j, i, RIGHT, '@', "player", "player", GREEN, BLACK, 0, 0, 1);
					this->cmap[i][j] = '.';	entityObjects[0] = temp;
					Tile_unblock(&this->map[i][j]);
				} else if(this->cmap[i][j] == '1') {
					Object_init(&temp, j, i, STOP, '*', "monster", "normal", RED, BLACK, 5, 5, 1);
					this->cmap[i][j] = '.';	entityObjects[totalEntities++] = temp;
					Tile_unblock(&this->map[i][j]);	this->map[i][j].playerOnly = true;
				} else if(this->cmap[i][j] == '2') {
					Object_init(&temp, j, i, STOP, '*', "monster", "ranger", RED, BLACK, 6, 8, 1);
					this->cmap[i][j] = '.';	entityObjects[totalEntities++] = temp;
					Tile_unblock(&this->map[i][j]);	this->map[i][j].playerOnly = true;
				} else if(this->cmap[i][j] == '3') {
					Object_init(&temp, j, i, STOP, '*', "monster", "sprinter", RED, BLACK, 8, 6, 1);
					this->cmap[i][j] = '.';	entityObjects[totalEntities++] = temp;
					Tile_unblock(&this->map[i][j]);	this->map[i][j].playerOnly = true;
				} else if(this->cmap[i][j] == '4') {
					Object_init(&temp, j, i, STOP, '*', "monster", "reaper", BROWN, BLACK, 15, fullmap.x, 1);
					this->cmap[i][j] = '.';	entityObjects[totalEntities++] = temp;
					Tile_unblock(&this->map[i][j]);	this->map[i][j].playerOnly = true;
				} else if(this->cmap[i][j] == '5') {
					Object_init(&temp, j, i, STOP, '*', "monster", "vampire", CYAN, BLACK, 10, 10, 1);
					this->cmap[i][j] = '.';	entityObjects[totalEntities++] = temp;
					Tile_unblock(&this->map[i][j]);	this->map[i][j].playerOnly = true;
				} else if(this->cmap[i][j] == '6') {
					Object_init(&temp, j, i, STOP, '*', "monster", "elite", YELLOW, BLACK, 8, 8, 1);
					this->cmap[i][j] = '.';	entityObjects[totalEntities++] = temp;
					Tile_unblock(&this->map[i][j]);	this->map[i][j].playerOnly = true;
				} else if(this->cmap[i][j] == '7') {
					Object_init(&temp, j, i, STOP, '*', "monster", "ghost", WHITE, BLACK, 8, 8, 1);
					this->cmap[i][j] = '.';	entityObjects[totalEntities++] = temp;
					Tile_unblock(&this->map[i][j]);
				} else if(this->cmap[i][j] == '#') {
					Tile_blockSight(&this->map[i][j]);	Tile_close(&this->map[i][j]);
				} else if(this->cmap[i][j] == 'g') {
					Object_init(&temp, j, i, STOP, 'g', "goal", "goal1", CYAN, CYAN, 0, 0, 0);
					entityObjects[totalEntities++] = temp;
					this->cmap[i][j] = '.';
					Tile_unblock(&this->map[i][j]);	this->map[i][j].playerOnly = true;
				} else if(this->cmap[i][j] == 's') {
					Object_init(&temp, j, i, STOP, 4, "effect", "slow1", LIGHTBLUE, BLACK, 0, 0, 0);
					entityObjects[totalEntities++] = temp;
					this->cmap[i][j] = '.';	Tile_unblock(&this->map[i][j]);
					//this->map[i][j].playerOnly = true;
				} else if(this->cmap[i][j] == 'f') {
					Object_init(&temp, j, i, STOP, 4, "effect", "fast1", LIGHTBLUE, BLACK, 0, 0, 0);
					entityObjects[totalEntities++] = temp;
					this->cmap[i][j] = '.';	Tile_unblock(&this->map[i][j]);
					//this->map[i][j].playerOnly = true;
				} else if(this->cmap[i][j] == 'x') {
					Object_init(&temp, j, i, STOP, 4, "effect", "breaker1", LIGHTBLUE, BLACK, 0, 0, 0);
					entityObjects[totalEntities++] = temp;
					this->cmap[i][j] = '.';	Tile_unblock(&this->map[i][j]);
					//this->map[i][j].playerOnly = true;
				} else if(this->cmap[i][j] == 'r') {
					Object_init(&temp, j, i, STOP, 4, "effect", "repel1", LIGHTBLUE, BLACK, 0, 0, 0);
					entityObjects[totalEntities++] = temp;
					this->cmap[i][j] = '.';	Tile_unblock(&this->map[i][j]);
					//this->map[i][j].playerOnly = true;
				} else if(this->cmap[i][j] == 'a') {
					Object_init(&temp, j, i, STOP, 4, "effect", "attract1", LIGHTBLUE, BLACK, 0, 0, 0);
					entityObjects[totalEntities++] = temp;
					this->cmap[i][j] = '.';	Tile_unblock(&this->map[i][j]);
					//this->map[i][j].playerOnly = true;
				} else if(this->cmap[i][j] == 'v') {
					Object_init(&temp, j, i, STOP, 4, "effect", "fullvision1", LIGHTBLUE, BLACK, 0, 0, 0);
					entityObjects[totalEntities++] = temp;
					this->cmap[i][j] = '.';	Tile_unblock(&this->map[i][j]);
					//this->map[i][j].playerOnly = true;
				} else if(this->cmap[i][j] == ' ') {
					this->cmap[i][j] = '.';	Tile_unblock(&this->map[i][j]);	Tile_close(&this->map[i][j]);	this->map[i][j].playerOnly = true;
				} else {
					Tile_unblock(&this->map[i][j]);
				}
			}
		}
		strcpy(fileName, trg);
		Level_Export(&(this->data), fileName, this->map);
		cnt++;
}

void main() {
	struct Creator creator;
	int i, j;
	clrscr();
	textcolor(WHITE);
	/*printf("Enter File Name: ");
	scanf("%s", &fileName);*/
	creator.totFiles = Creator_getPaths(&creator);
	for(i = 0; i<creator.totFiles; i++) {
		strcpy(fileName, creator.paths[i]);
		//sprintf(src, "MZRUN/files/TXTFILES/%s.TXT", fileName);
		sprintf(src, "MZRUN/files/TXTFILES/%s.TXT", fileName);
		sprintf(trg, "MZRUN/files/LEVELS/%s.LVL", fileName);
		printf("\nCreating: %s.LVL ...", fileName);
		Creator_create_lvl(&creator);
	}
	printf("\n\nDone");	getch();
}