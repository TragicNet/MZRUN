#ifndef OBJECT_H
#define OBJECT_H

//#include "MZRUN/TILE.H"

class Object {
protected:
	//int x, y;
	coord *pos;
	char obj_symbol;
	int obj_color;
	int obj_bgcolor;
	eDirection dir;
	char type[20];

public:
	Object(){};
	Object(int x, int y, eDirection dir, char symbol, char type[20], int color, int bgcolor = BLACK) {
		//this->x = x;	this->y = y;
		this->pos = new coord(x, y);
		this->dir = dir;	this->obj_symbol = symbol;
		this->obj_color = color;	this->obj_bgcolor = bgcolor;
		strcpy(this->type, type);
	}
	~Object() {}
	inline coord* getPos()	{	return this->pos;	}
	inline char getSymbol() {	return this->obj_symbol;	}
	inline char* getType() {	return this->type;	}
	inline int getColor() {	return this->obj_color;	}
	inline int getBgColor() {	return this->obj_bgcolor;	}
	inline eDirection getDirection() {	return this->dir;	}
	bool moveObject(int posX, int posY);
	void route();
	void draw();
	void death();
	inline bool isThere(coord t) {	return (this->pos->x==t.x && this->pos->y == t.y) ? true : false;	}
	inline float distance(coord t) {	return pow( pow(this->pos->x-t.x, 2)/*/2.5*/ + pow(this->pos->y*2-t.y*2, 2)/**1.5*/ , 0.5 );	}
};
unsigned int totalEntities = 1;
Object *entityObjects = (Object *) malloc(50 * sizeof(Object));
Object *keys = (Object *) malloc(20 * sizeof(Object));
Object *doors = (Object *) malloc(20 * sizeof(Object));
Object wall(0, 0, STOP, '#', "wall", WHITE, LIGHTGRAY);
//Object wallblock(0, 0, STOP, 'X', "wallblock", YELLOW, RED);
Object air(0, 0, STOP, '.', "air", LIGHTGRAY);
Object blackair(0, 0, STOP, '.', "air", BLACK);

int posCheck(coord pos, char *search = "", bool same = true) {
	for(int i = totalEntities-1; i >= 0; i--)
		if(entityObjects[i].isThere(pos)) {
			if( strcmp(search, "") || ( strcmp(search, entityObjects[i].getType()) && same == true ) )
				return i;
		}
	return -1;
}

bool Object :: moveObject(int posX, int posY) {
	if(this->pos->x+posX<0 || this->pos->x+posX>MaxX || this->pos->y+posY<0 || this->pos->y+posY>MaxY)
		return false;
	else if(map[this->pos->y+posY][this->pos->x+posX].isBlocked()) {
		gotoxy(this->pos->x+posX+1, this->pos->y+posY+1);
		//wallblock.draw();
		return false;
	}
	if(strcmp(this->type, "player"))
		map[this->pos->y][this->pos->x].unblock();
	this->pos->x+=posX;	this->pos->y+=posY;
	if(strcmp(this->type, "player"))
		map[this->pos->y][this->pos->x].block();
	return true;
}


void Object :: route() {
    int tx = 0, ty = 0, r;
	int ch = 0, dist = 4; // 4
	bool tried = false;
    do {
		ch++;
		tx = entityObjects[0].getPos()->x - this->pos->x;
		ty = entityObjects[0].getPos()->y - this->pos->y;
		if(abs(ty)<=dist && abs(tx)<=dist) {
			if(tried == true) {
				//r = (r==4) ? 1 : (r + 1);
				switch(r) {
					case 1:
					case 2:	//r = 3 + random(2);
							r = (pow( pow(tx, 2) + pow(ty-1, 2), 0.5 ) > pow( pow(tx, 2) + pow(ty+1, 2), 0.5 )) ? 3 : 4;
							break;
					case 3:
					case 4:	//r = 1 + random(2);
							r = (pow( pow(tx-1, 2) + pow(ty, 2), 0.5 ) > pow( pow(tx+1, 2) + pow(ty, 2), 0.5 )) ? 1 : 2;
							break;
				};
			} else if(abs(ty) > abs(tx)) {
				r = ( ty > 0 ) ? DOWN : UP;
			} else {
				r = ( tx > 0 ) ? RIGHT : LEFT;
			}
		} else {
			r = 1 + random(4);
		}

		this->dir = (eDirection) r;
		switch(r) {
			case 1:   tx = -1;  ty = +0; break;
			case 2:   tx = +1;  ty = +0; break;
			case 3:   tx = +0;  ty = -1; break;
			case 4:   tx = +0;  ty = +1; break;
		};
		tried = true;
	} while(!this->moveObject(tx, ty) && ch < dist);
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