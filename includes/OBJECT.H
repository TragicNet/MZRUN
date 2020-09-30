#ifndef OBJECT_H
#define OBJECT_H

//typedef void(*Scripts)(struct Engine *this);

struct Object {
	struct coord pos;
	enum eDirection dir;
	char symbol;
	char type[20];
	char name[20];
	int range;
	int speed;
	int color;
	int bgcolor;
	int weight;

	//Temporary Variables: Change Frequently
	unsigned int last;
} entityObjects[70], wall, air, blackair;
unsigned int totalEntities = 1;

void Object_reloc(struct Object *this, struct coord pos) {	this->pos = pos;	}
bool Object_isThere(struct Object *this, struct coord t) {	return (this->pos.x==t.x && this->pos.y == t.y) ? true : false;	}

void Object_init(struct Object *this, int x, int y, enum eDirection dir, char symbol, char type[], char name[], int color,  int bgcolor, int speed, int range, int weight) {
	coord_init(&this->pos, x, y);	this->dir = dir;	this->symbol = symbol;	
	strcpy(this->name, name);	strcpy(this->type, type);	this->range = range;
	this->speed = speed;	this->color = color;	this->bgcolor = bgcolor;	
	this->last = 0;	this->weight = weight;
}

void removeObject(struct Object objects[], int loc) {
	int i;
	Tile_unblock(&map[objects[loc].pos.y][objects[loc].pos.x]);
	if(totalEntities>0) {
        for(i = loc; i<totalEntities; i++) {
			objects[i] = objects[i+1];
        }
		totalEntities--;
    }
}

void Object_setColor(struct Object *this, int color,  int bgcolor) {
	this->color = color;	this->bgcolor = bgcolor;
}

int posCheck(struct coord pos, char *search, bool same) {
	int i;
	for(i = totalEntities-1; i >= 0; i--) {
		if(coord_cmp(entityObjects[i].pos, pos)) {
			if( strcmp(search, "") == 0 || strcmp(search, entityObjects[i].type) == 0 )
				return i;
		} else if( same == false && !strcmp(search, entityObjects[i].type) == 0 ) {
			return i;
		}
	}
	return i;
}

bool Object_move(struct Object *this, int posX, int posY) {
	bool ghost = false;
	if(strcmp(this->name, "ghost") == 0)	ghost = true;
	if(this->pos.x+posX<0 || this->pos.x+posX>fullmap.x || this->pos.y+posY<0 || this->pos.y+posY>fullmap.y)
		return false;
	else if(Tile_isBlocked(&map[this->pos.y+posY][this->pos.x+posX]) && !ghost)
		return false;
	if(strcmp(this->type, "player")!=0) {
		if(map[this->pos.y+posY][this->pos.x+posX].playerOnly == true)	return false;
		map[this->pos.y][this->pos.x].playerOnly = false;
	}
	this->pos.x+=posX;	this->pos.y+=posY;
	if(strcmp(this->type, "player")!=0)	map[this->pos.y][this->pos.x].playerOnly = true;
	return true;
}


void Object_route(struct Object *this, unsigned int last) {
    int spdform, rngform, spdmod = 0, rngmod = 0, i, j, xdiff = 0, ydiff = 0, ch = 0, r;
	bool tried = false, viewBlocked = false, invisible = false;
	if(strcmp(currentMode, "Hard+") == 0) {
		spdmod = 3;	rngmod = 3;
	} else if(strcmp(currentMode, "Hard") == 0) {
		spdmod = 2;	rngmod = 2;
	} else if(strcmp(currentMode, "Normal+") == 0) {
		spdmod = 1;	rngmod = 1;
	} else {
		spdmod = 0;	rngmod = 0;
	}
	//CheckStates
	for(i = 0; i<totStates; i++) {
		if(strcmp(states[i].name, "SLW") == 0)	spdmod-=states[i].val;
		else if(strcmp(states[i].name, "FST") == 0)	spdmod+=states[i].val;
		else if(strcmp(states[i].name, "ATR") == 0)	rngmod-=states[i].val;
		else if(strcmp(states[i].name, "RPL") == 0)	rngmod+=states[i].val;
		else if(strcmp(states[i].name, "INV") == 0)	invisible = true;
		else if(strcmp(states[i].name, "STP") == 0)	return;
	}
	rngform = this->range + rngmod;
	spdform = MaxSpeed - this->speed - spdmod;
	if(spdform <= 0)	spdform = 1 + spdmod;
	if(this->speed == 0 || this->last - last < (spdform*5))
		return;
    do {
		ch++;
		xdiff = entityObjects[0].pos.x - this->pos.x;
		ydiff = entityObjects[0].pos.y - this->pos.y;
		if(abs(ydiff)<=rngform && abs(xdiff)<=rngform) {
			i = this->pos.y;
			if(this->range!=fullmap.x && strcmp(this->name, "ghost") != 0) {
				//Don't Follow if Vision Blocked
				do {
					if(i>0 && i<fullmap.y)
						if(ydiff > 0) i++;	else if(ydiff < 0)	i--;
					j = this->pos.x;
					do {
						if(j>0 && j<fullmap.x)
							if(xdiff > 0) j++;	else if(xdiff < 0)	j--;
						if(Tile_isOpaque(&map[i][j]) && i>=0 && j>=0) {
							viewBlocked = true;	break;
						}
					} while(j != entityObjects[0].pos.x);
					if(viewBlocked == true)	break;
				} while(i != entityObjects[0].pos.y);
			}
			
			if(viewBlocked == true || invisible == true) {
				r = 1 + random(4);
			} else if(tried == true) {
				switch(r) {
					case 1:
					case 2:	r = ( distance(xdiff, ydiff-1) > distance(xdiff, ydiff+1)) ? 3 : 4;
							break;
					case 3:
					case 4:	r = ( distance(xdiff-1, ydiff) > distance(xdiff+1, ydiff)) ? 1 : 2;
							break;
				};
			} else if(abs(ydiff) > abs(xdiff)) {
				r = ( ydiff > 0 ) ? DOWN : UP;
			} else {
				r = ( xdiff > 0 ) ? RIGHT : LEFT;
			}
		} else {
			r = 1 + random(4);
		}

		this->dir = (enum eDirection) r;
		switch(r) {
		case 1:		xdiff = -1;	ydiff = +0;	break;
		case 2:		xdiff = +1;	ydiff = +0; break;
		case 3:		xdiff = +0;	ydiff = -1; break;
		case 4:		xdiff = +0;	ydiff = +1; break;
		};
		tried = true;
	} while(!Object_move(this, xdiff, ydiff) && ch < rngform);
	this->last = last;
}

void Object_draw(struct Object *this) {
	textcolor(this->color);
	textbackground(this->bgcolor);
	cprintf("%c", this->symbol);
	textcolor(WHITE);
	textbackground(BLACK);
}

int Object_torchRadius(struct coord this, struct coord t) {	
	return distance( this.x - t.x, (this.y - t.y) * 2 );
}

#endif