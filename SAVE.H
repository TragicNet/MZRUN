#ifndef SAVE_H
#define SAVE_H

class Save {
private:
    coord save_fullmap;
    int save_totalEntities;
    Object save_goal;
    Object save_monsters[50];
    char save_cmap[MaxY][MaxX];
    Tile save_map[MaxY][MaxX];
public:
    void Export(char path[]);
    void Import(char path[]);
};

void Save :: Export(char path[]) {
    int i, j;
    this->save_fullmap = fullmap;
    this->save_totalEntities = totalEntities;
    this->save_goal = goal;
    for(i=0; i<this->save_totalEntities; i++)
        this->save_monsters[i] = entityObjects[i];
    for(i=0; i<MaxY; i++) {
        for(j=0; j<MaxX; j++) {
            this->save_cmap[i][j] = cmap[i][j];
            this->save_map[i][j] = map[i][j];
        }
    }
	ofstream file(path, ios::in | ios::binary );
	file.write((char*) this, sizeof(Save));
	file.close();
}

void Save :: Import(char path[]) {
	ifstream file(path, ios::in | ios::binary );
	file.read((char*) this, sizeof(Save));
	file.close();

    int i, j;
    //delete fullmap; fullmap = new coord(this->save_fullmap.x, this->save_fullmap.y);
    fullmap = save_fullmap;
    totalEntities = this->save_totalEntities;
    goal.reloc(save_goal.getPos());
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