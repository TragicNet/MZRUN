#ifndef LEVEL_H
#define LEVEL_H

// Structure for level data
struct Level {
	struct coord fullmap;
	int totalEntities;
	struct coord player_pos[4];
	struct Object entities[50];
	struct Tile map[MaxY][MaxX];
};

// Export level data to file
void Level_Export(struct Level *this, char path[]) {
	int i, j;
	FILE *file;
	this->fullmap = fullmap;
	this->totalEntities = totalEntities;
	for(i=0; i<this->totalEntities; i++)
		this->entities[i] = entityObjects[i];

	file = fopen(path, "wb" );
	fwrite((char*) this, sizeof(struct Level), 1, file);
	fclose(file);
}

// Export level data from file
void Level_Import(struct Level *this, char path[]) {
	int i, j;
	FILE *file;

	file = fopen(path, "rb" );
	fread((char*) this, sizeof(struct Level), 1, file);
	fclose(file);

	fullmap = this->fullmap;
	totalEntities = this->totalEntities;
	for(i=0; i<totalEntities; i++)
		entityObjects[i] = this->entities[i];
	
}

#endif