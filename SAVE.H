#ifndef SAVE_H
#define SAVE_H

class Save {
public:
    int stotalEntities;
    Object smonsters[50];
    char scmap[MaxY][MaxX];
    Tile smap[MaxY][MaxX];
    Save() {}
    Save(int k) {
        int i, j;
        this->stotalEntities = totalEntities;
        for(i=0; i<stotalEntities; i++)
            this->smonsters[i] = entityObjects[i];
        for(i=0; i<MaxY; i++) {
            for(i=0; i<MaxX; i++) {
                this->scmap[i][j] = cmap[i][j];
                this->smap[i][j] = map[i][j];
            }
        }
    }
};

void Export(Engine &e) {
	Save savefile(1);
	FILE *file;
	file = fopen("./MZRUN/LEVELS/area1.lvl","wb");
	fwrite(&savefile, sizeof(Save), 1, file);
	fclose(file);
    cout<<"Done";	getch();	exit(0);
}

void Import() {
    Save f;
	FILE *file;
	file = fopen("./MZRUN/LEVELS/area1.lvl","rb");
	fwrite(&f, sizeof(Save), 1, file);
	fclose(file);
    int i, j;
    totalEntities = f.stotalEntities;
    for(i=0; i<totalEntities; i++)
        entityObjects[i] = f.smonsters[i];
    for(i=0; i<MaxY; i++) {
        for(i=0; i<MaxX; i++) {
            cmap[i][j] = f.scmap[i][j];
            map[i][j] = f.smap[i][j];
        }
    }
    cout<<totalEntities<<"HEllo"; getch();    exit(0);
}


#endif