#ifndef OBJECT_H
#define OBJECT_H

class Object {
protected:
	coord pos;
	eDirection dir;
	char symbol;
	char type[20];
	int range;
	int speed;
	int color;
	int bgcolor;

	//Temporary Variables: Change Frequently
	unsigned int last;

public:
	Object(){};
	Object(int x, int y, eDirection dir, char symbol, char type[20], int color,  int bgcolor = BLACK, int speed = 0, int range = 0) {
		this->pos = coord(x, y);	this->dir = dir;
		this->symbol = symbol;	strcpy(this->type, type);
		this->range = range;	this->speed = speed;
		this->color = color;	this->bgcolor = bgcolor;	
		this->last = 0;
	}
	~Object() {}
	inline coord getPos()	{	return this->pos;	}
	inline char getSymbol() {	return this->symbol;	}
	inline char* getType() {	return this->type;	}
	inline void setColor(int color = NULL,  int bgcolor = NULL)	{
		this->color = (color == NULL) ? this->color : color;
		this->bgcolor = (bgcolor == NULL) ? this->bgcolor : bgcolor;
	}
	inline int getSpeed() {	return this->speed;	}
	inline unsigned int getLast() {	return this->last;	}
	inline int getColor() {	return this->color;	}
	inline int getBgColor() {	return this->bgcolor;	}
	inline eDirection getDirection() {	return this->dir;	}
	bool moveObject(int posX, int posY);
	inline void reloc(coord pos) {	this->pos = pos;	}
	void route(unsigned int last);
	void draw();
	inline bool isThere(coord t) {	return (this->pos.x==t.x && this->pos.y == t.y) ? true : false;	}
	inline float distance(coord t) {	return pow( pow(this->pos.x-t.x, 2)/*/2.5*/ + pow(this->pos.y*2-t.y*2, 2)/**1.5*/ , 0.5 );	}
};
unsigned int totalEntities = 1;
Object entityObjects[50], keys[20], doors[20];
Object goal(0, 0, STOP, 'g', "goal", CYAN, CYAN);
Object wall(0, 0, STOP, '#', "wall", WHITE, LIGHTGRAY);
Object air(0, 0, STOP, '.', "air", LIGHTGRAY);
Object blackair(0, 0, STOP, '.', "air", BLACK);

int posCheck(coord pos, char search[] = NULL, bool same = true) {
	for(int i = totalEntities-1; i >= 0; i--)
		if(entityObjects[i].isThere(pos)) {
			if( search == NULL || ( strcmp(search, entityObjects[i].getType()) && same == true ) )
				return i;
		}
	return -1;
}

bool Object :: moveObject(int posX, int posY) {
	if(this->pos.x+posX<0 || this->pos.x+posX>fullmap->x || this->pos.y+posY<0 || this->pos.y+posY>fullmap->y)
		return false;
	else if(map[this->pos.y+posY][this->pos.x+posX].isBlocked())
		return false;
	if(strcmp(this->type, "player"))
		map[this->pos.y][this->pos.x].unblock();
	this->pos.x+=posX;	this->pos.y+=posY;
	if(strcmp(this->type, "player"))
		map[this->pos.y][this->pos.x].block();
	return true;
}


void Object :: route(unsigned int last) {
    if(this->speed == 0 || this->last - last < ((MaxSpeed + 1 - this->speed)*10))
		return;
	int tx = 0, ty = 0, ch = 0, r;
	bool tried = false;
    do {
		ch++;
		tx = entityObjects[0].getPos().x - this->pos.x;
		ty = entityObjects[0].getPos().y - this->pos.y;
		if(abs(ty)<=this->range && abs(tx)<=this->range) {
			if(tried == true) {
				switch(r) {
					case 1:
					case 2:	r = (pow( pow(tx, 2) + pow(ty-1, 2), 0.5 ) > pow( pow(tx, 2) + pow(ty+1, 2), 0.5 )) ? 3 : 4;
							break;
					case 3:
					case 4:	r = (pow( pow(tx-1, 2) + pow(ty, 2), 0.5 ) > pow( pow(tx+1, 2) + pow(ty, 2), 0.5 )) ? 1 : 2;
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
	} while(!this->moveObject(tx, ty) && ch < this->range);
	this->last = last;
}

void Object :: draw() {
	textcolor(color);
	textbackground(bgcolor);
	cprintf("%c", symbol);
	textcolor(WHITE);
	textbackground(BLACK);
}

#endif