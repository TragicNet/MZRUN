#ifndef SAVE_H
#define SAVE_H

struct Save {
	struct coord save_fullmap;
	int save_totalEntities;
	//struct Object save_goal;
	struct Object save_monsters[50];
	char save_cmap[MaxY][MaxX];
	struct Tile save_map[MaxY][MaxX];
};

void Save_Export(struct Save *this, char path[]) {
	int i, j;
	FILE *file;
	this->save_fullmap = fullmap;
	this->save_totalEntities = totalEntities;
	//this->save_goal = goal;
	for(i=0; i<this->save_totalEntities; i++)
		this->save_monsters[i] = entityObjects[i];
	for(i=0; i<MaxY; i++) {
		for(j=0; j<MaxX; j++) {
			this->save_cmap[i][j] = cmap[i][j];
			this->save_map[i][j] = map[i][j];
		}
	}

	file = fopen(path, "wb" );
	fwrite((char*) this, sizeof(struct Save), 1, file);
	fclose(file);
}

void Save_Import(struct Save *this, char path[]) {
	int i, j;
	FILE *file;

	file = fopen(path, "rb" );
	fread((char*) this, sizeof(struct Save), 1, file);
	fclose(file);

	fullmap = this->save_fullmap;
	totalEntities = this->save_totalEntities;
	//Object_reloc(&goal, this->save_goal.pos);
	for(i=0; i<totalEntities; i++)
		entityObjects[i] = this->save_monsters[i];
	for(i=0; i<MaxY; i++) {
		for(j=0; j<MaxX; j++) {
			cmap[i][j] = this->save_cmap[i][j];
			map[i][j] = this->save_map[i][j];
		}
	}
}

#endif