#ifndef COORD_H
#define COORD_H

struct coord {
	int x, y;
} board, screen, fullmap, tmp;

void coord_init(struct coord *trg, int x, int y) {   trg->x = x;  trg->y = y;  }
void coord_cpy(struct coord *trg, struct coord src) {   trg->x = src.x;  trg->y = src.y;  }

struct coord coord_sub(struct coord a, struct coord b) {
    coord_init(&tmp, a.x - b.x, a.y - b.y);
    return tmp;
}

struct coord coord_add(struct coord a, struct coord b) {
    coord_init(&tmp, a.x + b.x, a.y + b.y);
    return tmp;
}
bool coord_cmp(struct coord a, struct coord b) {
    return (a.x == b.x && a.y == b.y) ? true : false;
}

int coord_distance(const struct coord a, const struct coord b) {
	return abs((a.x-b.x) * (a.x-b.x)) + abs((a.y-b.y) * (a.y-b.y));
}

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