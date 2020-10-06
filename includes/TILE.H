#ifndef TILE_H
#define TILE_H

struct Tile {
    bool blocked;
    bool sightBlocked;
    bool playerOnly;
    bool floor;
};

bool Tile_isBlocked(struct Tile *this)      {   return this->blocked;       }
void Tile_block(struct Tile *this)          {   this->blocked = true;       }
void Tile_unblock(struct Tile *this)        {   this->blocked = false;      }
bool Tile_isOpaque(struct Tile *this)       {   return this->sightBlocked;  }
void Tile_blockSight(struct Tile *this)     {   this->sightBlocked = true;  }
void Tile_unblockSight(struct Tile *this)   {   this->sightBlocked = false; }
bool Tile_isOpen(struct Tile *this)         {   return this->floor;         }
void Tile_open(struct Tile *this)           {   this->floor = true;         }
void Tile_close(struct Tile *this)          {   this->floor = false;        }

void Tile_init(struct Tile *this, bool blocked, bool sightBlocked, bool playerOnly, bool floor) {
    this->blocked = blocked;
    this->sightBlocked = sightBlocked;
    this->playerOnly = playerOnly;
    this->floor = floor;
}

int Tile_view(struct coord this, struct coord t) {	
	return distance( (this.x - t.x), (this.y - t.y) * 2 );
}

bool Tile_visible(struct coord start, struct coord end, int range, struct Tile map[][90]) {
    int xi, yi, error;
	struct coord current, errorCorrect, delta;
	//if(Tile_view(start, end) >= (range*range))	return false;
	xi = (end.x > start.x) ? 1 : -1;
	yi = (end.y > start.y) ? 1 : -1;
	current.x = start.x;	current.y = start.y;
	delta.x = abs(start.x-end.x);	delta.y = abs(start.y-end.y);
	error = delta.x - delta.y;
	errorCorrect.x = delta.x*2, errorCorrect.y = delta.y*2;
	while(1) {
		if(coord_cmp(current, end))	break;
        else if(current.x - end.x == xi || current.y - end.y == yi)	break;
        else if(current.x < 0 || current.y < 0 || current.x > fullmap.x || current.y > fullmap.y)  break;
		else if(Tile_isOpaque(&map[current.y][current.x]))	return false;
		
		if(error > 0) {	current.x+=xi;	error-=errorCorrect.y;	}
		else if(error < 0) {	current.y+=yi;	error+=errorCorrect.x;	}
		else {	current.x+=xi;	current.y+=yi;	}
	}
	return true;
}

#endif