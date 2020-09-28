#ifndef LEVEL_H
#define LEVEL_H

struct Level {
	struct coord fullmap;
	int totalEntities;
	struct Object entities[70];
	struct Tile map[MaxY][MaxX];
};

void Level_Export(struct Level *this, char path[]) {
	int i, j;
	FILE *file;
	this->fullmap = fullmap;
	this->totalEntities = totalEntities;
	//this->goal = goal;
	for(i=0; i<this->totalEntities; i++)
		this->entities[i] = entityObjects[i];
	for(i=0; i<MaxY; i++) {
		for(j=0; j<MaxX; j++) {
			this->map[i][j] = map[i][j];
		}
	}

	file = fopen(path, "wb" );
	fwrite((char*) this, sizeof(struct Level), 1, file);
	fclose(file);
}

void Level_Import(struct Level *this, char path[]) {
	int i, j;
	FILE *file;

	file = fopen(path, "rb" );
	fread((char*) this, sizeof(struct Level), 1, file);
	fclose(file);

	fullmap = this->fullmap;
	totalEntities = this->totalEntities;
	//Object_reloc(&goal, this->goal.pos);
	for(i=0; i<totalEntities; i++)
		entityObjects[i] = this->entities[i];
	for(i=0; i<MaxY; i++) {
		for(j=0; j<MaxX; j++) {
			map[i][j] = this->map[i][j];
		}
	}
}

#endif