#ifndef COORD_H
#define COORD_H

// Structure for coordinates
struct coord {
	int x, y;
} board, screen, fullmap;

// Initialize coordinates
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

#endif