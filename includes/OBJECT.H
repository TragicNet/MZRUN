#ifndef OBJECT_H
#define OBJECT_H

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
} entityObjects[50], wall, air, blackair, outVision, pit;
unsigned int totalEntities = 1;

void Object_reloc(struct Object *this, struct coord pos) {	this->pos = pos;	}
bool Object_isThere(struct Object *this, struct coord t) {	return (this->pos.x==t.x && this->pos.y == t.y) ? true : false;	}

void Object_init(struct Object *this, int x, int y, enum eDirection dir, char symbol, char type[], char name[], int color,  int bgcolor, int speed, int range, int weight) {
	coord_init(&this->pos, x, y);	this->dir = dir;	this->symbol = symbol;	
	strcpy(this->name, name);	strcpy(this->type, type);	this->range = range;
	this->speed = speed;	this->color = color;	this->bgcolor = bgcolor;	
	this->last = 0;	this->weight = weight;
}

void removeObject(struct Object objects[], int loc, struct Tile map[][90]) {
	int i;
	Tile_unblock(&map[objects[loc].pos.y][objects[loc].pos.x]);
	map[objects[loc].pos.y][objects[loc].pos.x].playerOnly = false;
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

bool Object_move(struct Object *this, int posX, int posY, struct Tile map[][90]) {
	bool ghost = false;
	if(strcmp(this->name, "ghost") == 0)	ghost = true;
	if(this->pos.x+posX<0 || this->pos.x+posX>fullmap.x || this->pos.y+posY<0 || this->pos.y+posY>fullmap.y)
		return false;
	else if(Tile_isBlocked(&map[this->pos.y+posY][this->pos.x+posX]) && !ghost)
		return false;
	if(strcmp(this->type, "player")!=0 && !ghost) {
		if(map[this->pos.y+posY][this->pos.x+posX].playerOnly == true)	return false;
		map[this->pos.y][this->pos.x].playerOnly = false;
	}
	this->pos.x+=posX;	this->pos.y+=posY;
	if(strcmp(this->type, "player")!=0 && !ghost)	map[this->pos.y][this->pos.x].playerOnly = true;
	return true;
}

int view(struct coord this, struct coord t) {	
	return distance( (this.x - t.x), (this.y - t.y) * 2 );
}

bool visible(struct coord start, struct coord end, int range, struct Tile map[][90]) {
	int xi, yi, error;
	struct coord current, errorCorrect, delta;
	//if(view(start, end) >= (range*range))	return false;
	xi = (end.x > start.x) ? 1 : -1;
	yi = (end.y > start.y) ? 1 : -1;
	current.x = start.x;	current.y = start.y;
	delta.x = abs(start.x-end.x);	delta.y = abs(start.y-end.y);
	error = delta.x - delta.y;
	errorCorrect.x = delta.x*2, errorCorrect.y = delta.y*2;
	while(1) {
		if(coord_cmp(current, end))	break;
		else if(Tile_isOpaque(&map[current.y][current.x]))	return false;
		else if(current.x - end.x == xi || current.y == yi)	break;
		if(error > 0) {	current.x+=xi;	error-=errorCorrect.y;	}
		else if(error < 0) {	current.y+=yi;	error+=errorCorrect.x;	}
		else {	current.x+=xi;	current.y+=yi;	}
	}
	return true;
}

void Object_route(struct Object *this, unsigned int last, struct Tile map[][90]) {
    int spdform, rngform, spdmod = 0, rngmod = 0, i, j, xdiff = 0, ydiff = 0, ch = 0, r;
	int xi, yi;
	bool tried = false, viewBlocked = false, invisible = false, ghost = true, reaper = true;
	if(strcmp(this->name, "ghost") == 0)	ghost = true;
	else if(strcmp(this->name, "reaper") == 0)	reaper = true;
	if(strcmp(currentMode, "Hard+") == 0) {
		spdmod = 3;	rngmod = 3;
	} else if(strcmp(currentMode, "Hard") == 0) {
		spdmod = 1;	rngmod = 2;
	} else if(strcmp(currentMode, "Normal+") == 0) {
		spdmod = -1;	rngmod = 1;
	} else {
		spdmod = -3;	rngmod = 0;
	}
	//CheckStates
	for(i = 0; i<totStates; i++) {
		if(strcmp(states[i].name, "SLW") == 0)	spdmod-=states[i].val;
		else if(strcmp(states[i].name, "FST") == 0)	spdmod+=states[i].val;
		else if(strcmp(states[i].name, "ATR") == 0)	rngmod-=states[i].val;
		else if(strcmp(states[i].name, "RPL") == 0)	rngmod+=states[i].val;
		else if(strcmp(states[i].name, "INV") == 0)	invisible = true;
		else if(strcmp(states[i].name, "STP") == 0)	{ this->last = last; return; }
	}
	rngform = this->range + rngmod;
	spdform = MaxSpeed - this->speed - spdmod;
	if(spdform <= 0)	spdform = 1 + spdmod;
	if(this->speed == 0 || this->last - last < (spdform*5))
		return;
	/*if(strcmp(this->name, "reaper") == 0) {
		r = 1 + random(4);
		Object_move(this, nx[r], ny[r]);
		this->last = last;
		return;
	}*/
    do {
		ch++;
		xdiff = entityObjects[0].pos.x - this->pos.x;
		ydiff = entityObjects[0].pos.y - this->pos.y;
		if(view(this->pos, entityObjects[0].pos) < (rngform * rngform) && !invisible) {
			if(this->range!=fullmap.x && !ghost && !reaper) {
				//Don't Follow if Vision Blocked
				/*xi = xdiff;	if(xi > 0)  xi = 1;  else if(xi < 0) xi = -1;
    			yi = ydiff;	if(yi > 0)  yi = 1;  else if(yi < 0) yi = -1;
				for(i = this->pos.y; i != entityObjects[0].pos.y + ((xi != yi) ? yi : 0); ) {
					if(i>0 && i<fullmap.y)	i+=yi;
					for(j = this->pos.x; j != entityObjects[0].pos.x + ((xi != yi) ? xi : 0); ) {
						if(j>0 && j<fullmap.x)	j+=xi;
						if(Tile_isOpaque(&map[i][j])) {	viewBlocked = true;	break;	}
					}
					if(viewBlocked == true)	break;
				}*/
				viewBlocked = !visible(this->pos, entityObjects[0].pos, rngform, map);
			}
			
			if(viewBlocked == true) {
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
			}/*else {
				r = astar(this->pos, entityObjects[0].pos, this->type);
			}*/
		} else {
			r = 1 + random(4);
			//break;	stop all for debugging
		}

		this->dir = (enum eDirection) r;
		tried = true;
	} while(!Object_move(this, nx[r-1], ny[r-1], map) && ch < rngform);
	this->last = last;
}

void Object_draw(struct Object *this) {
	textattr(this->color + ( this->bgcolor << 4));
	cprintf("%c", this->symbol);
	textattr(WHITE + (BLACK<<4));
}

#endif