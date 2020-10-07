#ifndef COORD_H
#define COORD_H

// Structure for coordinates
struct coord {
	int x, y;
} board, screen, fullmap;

void coord_init(struct coord *trg, int x, int y) {
    trg->x = x;  trg->y = y;
}

// Copy coordinates
void coord_cpy(struct coord *trg, struct coord src) {
    trg->x = src.x;  trg->y = src.y;
}

// Subtract coordinates
struct coord coord_sub(struct coord a, struct coord b) {
    a.x -= b.x, a.y -= b.y;
    return a;
}

// Add coordinates
struct coord coord_add(struct coord a, struct coord b) {
	a.x += b.x, a.y += b.y;
    return a;
}

// Compare coordinates
bool coord_cmp(struct coord a, struct coord b) {
    return (a.x == b.x && a.y == b.y) ? true : false;
}


// Calc distance between coordinates
int coord_distance(const struct coord a, const struct coord b) {
	return abs((a.x-b.x) * (a.x-b.x)) + abs((a.y-b.y) * (a.y-b.y));
}

// Calculate display coordinates
struct coord display(struct coord p, struct coord s, struct coord m) {
    struct coord hs, c;
    coord_init(&hs, s.x/2, s.y/2);
    c = coord_sub(p, hs);
    if(c.x < 0)    c.x = 0;
    else if(c.x + s.x > m.x)    c.x = m.x - s.x;
    if(c.y < 0)    c.y = 0;
    else if(c.y + s.y > m.y)    c.y = m.y - s.y;
    return c;
}

// Calculate visiblility range
int coord_view(struct coord this, struct coord t) {	
	return distance( (this.x - t.x), (this.y - t.y) * 2 );
}

/*bool coord_visible(struct coord start, struct coord end, int range, struct Tile map[][90]) {
    int xi, yi, error;
	struct coord current, errorCorrect, delta;
	//if(coord_view(start, end) >= (range*range))	return false;
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
}*/

#endif