/*
	Map maker
	load map from txt file and put in lvl file with map data and objects

*/

char fileName[50] = "AREA3";

#include "MZRUN/INCLUDES/GLOBALS.H"

struct Creator {
	struct Level data;
	char cmap[MaxY][MaxX];
	struct coord pos;
	bool end;
};
char src[50], trg[50];

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
}

bool Creator_move(struct Creator *this, int PosX, int PosY) {
	return true;
}

int Creator_getPaths(char paths[][50]) {
	struct dirent *de;
	int i, cnt=0, s;
	char ext[] = ".TXT";
	DIR *dr = opendir("MZRUN/files/TXTFILES");
	if(dr == NULL) return 0;
	while((de = readdir(dr)) != NULL)  {
		s = strlen(de->d_name);
		for(i = s-4; i < s; i++) {
			if(de->d_name[i] != ext[i-(s-4)])	break;
		}
		if(i==s) {
			strcpy(paths[cnt++], de->d_name);
		}
	}
	closedir(dr);
	return cnt;
}

void Creator_create_lvl(struct Creator *this, char paths[][50]) {
	int i = 0, j, s, cnt = 0, totFiles;
	struct Object *temp;
	FILE *file;
	char fileName[50] = "MZRUN/files/TXTFILES/";
	char ext[] = ".LVL";
	coord_init(&fullmap, 90, 30);
	temp = (struct Object*) malloc(sizeof(struct Object));
	totFiles = Creator_getPaths(paths);
	//while(cnt < totFiles) {
		//Create Map file
		strcpy(fileName, src);
		//strcat(fileName, paths[cnt]);
		//printf("%d: %s %s\t", totFiles, fileName, paths[cnt]);	getch();
		s = strlen(fileName);
		file = fopen(fileName, "r");
		if( file == NULL) {
			clrscr();	printf("\n! ERROR: %s missing !", fileName);	getch();	exit(0);
		}
		while(!feof(file)) {
			fscanf(file, "%s", &this->cmap[i++]);
		}
		fclose(file);
		totalEntities = 1;
		for(i = 0; i<fullmap.y; i++) {
			for(j = 0; j<fullmap.x; j++) {
				Tile_init(&map[i][j], true, false, false, true);
				//coord_init(&tmp, j, i);
				if(this->cmap[i][j]=='@') {
					//temp = (struct Object*) malloc(sizeof(struct Object));
					Object_init(temp, j, i, RIGHT, '@', "player", "player", GREEN, BLACK, 0, 0, 1);
					this->cmap[i][j] = '.';	entityObjects[0] = *temp;	//free(temp);
					Tile_unblock(&map[i][j]);
				} else if(this->cmap[i][j] == '*') {
					//temp = (struct Object*) malloc(sizeof(struct Object));
					Object_init(temp, j, i, STOP, '*', "monster", "normal", RED, BLACK, 5, 4, 1); //RED
					this->cmap[i][j] = '.';	entityObjects[totalEntities++] = *temp;	//free(temp);
					Tile_unblock(&map[i][j]);	map[i][j].playerOnly = true;
				} else if(this->cmap[i][j] == '#') {
					Tile_blockSight(&map[i][j]);	Tile_close(&map[i][j]);
				} else if(this->cmap[i][j] == 'g') {
					//temp = (struct Object*) malloc(sizeof(struct Object));
					Object_init(temp, j, i, STOP, 'g', "goal", "goal1", CYAN, CYAN, 0, 0, 0);
					entityObjects[totalEntities++] = *temp;	//free(temp);
					this->cmap[i][j] = '.';
					Tile_unblock(&map[i][j]);	map[i][j].playerOnly = true;
				} else if(this->cmap[i][j] == 's') {
					//temp = (struct Object*) malloc(sizeof(struct Object));
					Object_init(temp, j, i, STOP, 4, "effect", "slow1", LIGHTBLUE, BLACK, 0, 0, 0);
					entityObjects[totalEntities++] = *temp;	//free(temp);
					this->cmap[i][j] = '.';	Tile_unblock(&map[i][j]);
					//map[i][j].playerOnly = true;
				} else if(this->cmap[i][j] == 'f') {
					//temp = (struct Object*) malloc(sizeof(struct Object));
					Object_init(temp, j, i, STOP, 4, "effect", "fast1", LIGHTBLUE, BLACK, 0, 0, 0);
					entityObjects[totalEntities++] = *temp;	//free(temp);
					this->cmap[i][j] = '.';	Tile_unblock(&map[i][j]);
					//map[i][j].playerOnly = true;
				} else if(this->cmap[i][j] == 'x') {
					//temp = (struct Object*) malloc(sizeof(struct Object));
					Object_init(temp, j, i, STOP, 4, "effect", "breaker1", LIGHTBLUE, BLACK, 0, 0, 0);
					entityObjects[totalEntities++] = *temp;	//free(temp);
					this->cmap[i][j] = '.';	Tile_unblock(&map[i][j]);
					//map[i][j].playerOnly = true;
				} else if(this->cmap[i][j] == 'r') {
					//temp = (struct Object*) malloc(sizeof(struct Object));
					Object_init(temp, j, i, STOP, 4, "effect", "repel1", LIGHTBLUE, BLACK, 0, 0, 0);
					entityObjects[totalEntities++] = *temp;	//free(temp);
					this->cmap[i][j] = '.';	Tile_unblock(&map[i][j]);
					//map[i][j].playerOnly = true;
				} else if(this->cmap[i][j] == 'a') {
					//temp = (struct Object*) malloc(sizeof(struct Object));
					Object_init(temp, j, i, STOP, 4, "effect", "attract1", LIGHTBLUE, BLACK, 0, 0, 0);
					entityObjects[totalEntities++] = *temp;	//free(temp);
					this->cmap[i][j] = '.';	Tile_unblock(&map[i][j]);
					//map[i][j].playerOnly = true;
				} else if(this->cmap[i][j] == 'v') {
					//temp = (struct Object*) malloc(sizeof(struct Object));
					Object_init(temp, j, i, STOP, 4, "effect", "fullvision1", LIGHTBLUE, BLACK, 0, 0, 0);
					entityObjects[totalEntities++] = *temp;	//free(temp);
					this->cmap[i][j] = '.';	Tile_unblock(&map[i][j]);
					//map[i][j].playerOnly = true;
				} else if(this->cmap[i][j] == 'V') {
					this->cmap[i][j] = '.';	Tile_unblock(&map[i][j]);	Tile_close(&map[i][j]);	map[i][j].playerOnly = true;
				} else {
					Tile_unblock(&map[i][j]);
				}
			}
		}
		strcpy(fileName, trg);
		/*strcat(fileName, paths[cnt]);
		s = strlen(fileName);
		for(i = s-4; i < s; i++) {
			fileName[i] = ext[i-(s-4)];
		}*/
		//printf("%s\n%s\n", src, trg);	getch();
		Level_Export(&(this->data), fileName);
		cnt++;
	//}
	free(temp);
}

void main() {
	struct Creator creator;
	clrscr();
	printf("Enter File Name: ");
	scanf("%s", &fileName);
	sprintf(src, "MZRUN/files/TXTFILES/%s.TXT", fileName);
	sprintf(trg, "MZRUN/files/LEVELS/%s.LVL", fileName);
	printf("\nCreating: %s.LVL ...", fileName);
	Creator_create_lvl(&creator, paths);
	printf("\n\nDone");	getch();
}