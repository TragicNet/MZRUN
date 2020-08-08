#ifndef OBJECT_H
#define OBJECT_H

//#include "MZRUN/TILE.H"

class Object {
protected:
	int x, y;
	char obj_symbol;
	int obj_color;
	int obj_bgcolor;
	eDirection dir;
	char type[20];

public:
	Object(){};
	Object(int y, int x, eDirection dir, char symbol, char type[20], int color, int bgcolor = BLACK) {
		this->x = x;	this->y = y;	
		this->dir = dir;	this->obj_symbol = symbol;
		this->obj_color = color;	this->obj_bgcolor = bgcolor;
		strcpy(this->type, type);
	}
	~Object() {}
	inline int getX() {	return this->x;	}
	inline int getY() {	return this->y;	}
	inline char getSymbol() {	return this->obj_symbol;	}
	inline char* getType() {	return this->type;	}
	inline int getColor() {	return this->obj_color;	}
	inline int getBgColor() {	return this->obj_bgcolor;	}
	inline eDirection getDirection() {	return this->dir;	}
	bool moveObject(int posX, int posY);
	void route();
	void draw();
	void death();
	inline bool isThere(int i, int j) {	return (this->x==i && this->y == j) ? true : false;	}
	inline float distance(int y, int x) {	return pow( pow(this->x-x, 2)/*/2.5*/ + pow(this->y*2-y*2, 2)/**1.5*/ , 0.5 );	}
};
unsigned int totalEntities = 1;
Object *entityObjects = (Object *) malloc(50 * sizeof(Object));
Object *keys = (Object *) malloc(20 * sizeof(Object));
Object *doors = (Object *) malloc(20 * sizeof(Object));
Object wall(0, 0, STOP, '#', "wall", WHITE, LIGHTGRAY);
//Object wallblock(0, 0, STOP, 'X', "wallblock", YELLOW, RED);
Object air(0, 0, STOP, '.', "air", LIGHTGRAY);
Object blackair(0, 0, STOP, '.', "air", BLACK);

int posCheck(int x, int y, char *search = "", bool same = true) {
	for(int i = totalEntities-1; i >= 0; i--)
		if(entityObjects[i].isThere(x, y)) {
			if( strcmp(search, "") || ( strcmp(search, entityObjects[i].getType()) && same == true ) )
				return i;
		}
	return -1;
}

bool Object :: moveObject(int posX, int posY) {
	if(this->x+posX<0 || this->x+posX>MaxX || this->y+posY<0 || this->y+posY>MaxY)
		return false;
	else if(map[this->y+posY][this->x+posX].isBlocked()) {
		gotoxy(this->x+posX+1, this->y+posY+1);
		//wallblock.draw();
		return false;
	} else if(posCheck(posX, posY) != -1) {
		return false;
	}
	this->x+=posX;	this->y+=posY;
	return true;
}

void Object :: route() {
    int tx, ty;
	int dist = 4;
    eDirection r;
    do {
	    r = (eDirection) (1 + random(1000)/250);
		tx = entityObjects[0].getX() - this->x;
		ty = entityObjects[0].getY() - this->y;
        if(abs(tx) <= dist && abs(ty) <= dist) {
			if(ty == 0 || tx > ty) {
				if( tx >= -dist && tx < 0) {
					r = LEFT;
				} else if( tx <= dist && tx > 0) {
					r = RIGHT;
				}
			} else if(tx == 0 || ty > tx) {
				if( ty >= -dist && ty < 0) {
					r = UP;
				} else if( ty <= dist && ty > 0) {
					r = DOWN;
				}	
			} else if(tx == ty  || tx < ty) {
				eDirection xr = (eDirection) (1 + random(1000)/500);
				switch(xr) {
					case 1:	if( ty >= -dist && ty < 0) {
								r = UP;
							} else if( ty <= dist && ty > 0) {
								r = DOWN;
							}
					case 2:	if( tx >= -dist && tx < 0) {
								r = LEFT;
							} else if( tx <= dist && tx > 0) {
								r = RIGHT;
							}
				}
			}
		}
		this->dir = r;
		switch(r) {
            case 1:   tx = -1;  ty = +0; break;
            case 2:   tx = +1;  ty = +0; break;
            case 3:   tx = +0;  ty = -1; break;
            case 4:   tx = +0;  ty = +1; break;
        };
	}while(!this->moveObject(tx, ty));
}

void Object :: draw() {
	textcolor(obj_color);
	textbackground(obj_bgcolor);
	cprintf("%c", obj_symbol);
	textcolor(WHITE);
	textbackground(BLACK);
}

void Object::death() {
	sound(120);	halt(100);	nosound();
	halt(50);
	sound(120);	halt(100);	nosound();
}

#endif