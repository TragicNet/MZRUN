#ifndef OBJECT_H
#define OBJECT_H

//typedef void(*Scripts)(struct Engine *this);

struct Object {
	struct coord pos;
	enum eDirection dir;
	char symbol;
	char type[20];
	int range;
	int speed;
	int color;
	int bgcolor;
	int weight;

	//Temporary Variables: Change Frequently
	unsigned int last;
}entityObjects[50], keys[20], doors[20], wall, air, blackair;
unsigned int totalEntities = 1;

void Object_reloc(struct Object *this, struct coord pos) {	this->pos = pos;	}
bool Object_isThere(struct Object *this, struct coord t) {	return (this->pos.x==t.x && this->pos.y == t.y) ? true : false;	}
void Engine_death(struct Engine *this);
void Object_init(struct Object *this, int x, int y, enum eDirection dir, char symbol, char type[20], int color,  int bgcolor, int speed, int range, int weight) {
	coord_init(&this->pos, x, y);	this->dir = dir;
	this->symbol = symbol;	strcpy(this->type, type);
	this->range = range;	this->speed = speed;
	this->color = color;	this->bgcolor = bgcolor;	
	this->last = 0;	this->weight = weight;
}

void Object_setColor(struct Object *this, int color,  int bgcolor)	{
	this->color = color;	this->bgcolor = bgcolor;
}

int posCheck(struct coord pos, char *search, bool same) {
	int i;
	for(i = totalEntities-1; i >= 0; i--) {
		if(coord_cmp(entityObjects[i].pos, pos)) {
			if( strcmp(search, "") || strcmp(search, entityObjects[i].type) )
				return i;
		} else if( same == false && !strcmp(search, entityObjects[i].type) ) {
			return i;
		}
	}
	return i;
}

bool Object_move(struct Object *this, int posX, int posY) {
	if(this->pos.x+posX<0 || this->pos.x+posX>fullmap.x || this->pos.y+posY<0 || this->pos.y+posY>fullmap.y)
		return false;
	else if(Tile_isBlocked(&map[this->pos.y+posY][this->pos.x+posX]))
		return false;
	
	if(strcmp(this->type, "player"))
		Tile_unblock(&map[this->pos.y][this->pos.x]);
	if(strcmp(this->type, "player")!=0 && map[this->pos.y+posY][this->pos.x+posX].player_only == true)
		return false;
		
	this->pos.x+=posX;	this->pos.y+=posY;
	if(strcmp(this->type, "player"))
		Tile_block(&map[this->pos.y][this->pos.x]);
	return true;
}


void Object_route(struct Object *this, unsigned int last) {
    int i, j, xdiff = 0, ydiff = 0, ch = 0, r;
	bool tried = false, viewBlocked = false;
	if(this->speed == 0 || this->last - last < ((MaxSpeed + 1 - this->speed)*20))
		return;
    do {
		ch++;
		xdiff = entityObjects[0].pos.x - this->pos.x;
		ydiff = entityObjects[0].pos.y - this->pos.y;
		if(abs(ydiff)<=this->range && abs(xdiff)<=this->range) {
			//Don't Follow if Vision Blocked
			i = this->pos.y;
			if(this->range!=MaxX) {
				do {
					if(ydiff > 0) i++;	else if(ydiff < 0)	i--;
					j = this->pos.x;
					do {
						if(xdiff > 0) j++;	else if(xdiff < 0)	j--;
						if(Tile_isOpaque(&map[i][j]) && i>=0 && j>=0) {
							viewBlocked = true;	break;
						}
					} while(j != entityObjects[0].pos.x);
					if(viewBlocked == true)	break;
				} while(i != entityObjects[0].pos.y);
			}
			
			if(viewBlocked == true) {
				r = 1 + random(4);
			} else if(tried == true) {
				switch(r) {
					case 1:
					case 2:	r = (pow( pow(abs(xdiff), 2) + pow(abs(ydiff-1), 2), 0.5 ) > pow( pow(abs(xdiff), 2) + pow(abs(ydiff+1), 2), 0.5 )) ? 3 : 4;
							break;
					case 3:
					case 4:	r = (pow( pow(abs(xdiff-1), 2) + pow(abs(ydiff), 2), 0.5 ) > pow( pow(abs(xdiff+1), 2) + pow(abs(ydiff), 2), 0.5 )) ? 1 : 2;
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
		case 1:   xdiff = -1;  ydiff = +0; break;
		case 2:   xdiff = +1;  ydiff = +0; break;
		case 3:   xdiff = +0;  ydiff = -1; break;
		case 4:   xdiff = +0;  ydiff = +1; break;
		};
		tried = true;
	} while(!Object_move(this, xdiff, ydiff) && ch < this->range);
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
	float xp, yp, d;
	xp = (this.x-t.x == 0) ? 0 : pow(abs(this.x-t.x), 2);
	yp = (this.y-t.y == 0) ? 0 : pow(abs(this.y*2-t.y*2), 2);
	d = xp + yp;
	if(xp == 0 && yp == 0)
		return 0;
	if(xp  < 0 || yp < 0) {
		gotoxy(1, 1);	printf("%f, %f", xp, yp);
		gotoxy(1, 2);	printf("%d, %d", this.x, this.y);
		gotoxy(1, 3);	printf("%d, %d", t.x, t.y);
		getch();	exit(0);
	}
	return pow( d, 0.5);
	//return torchRange;
}

#endif