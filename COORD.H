#ifndef COORD_H
#define COORD_H

class coord {
public:
    int x, y;
    coord() {   x = 0;  y = 0;  }
    coord(int x, int y) {
        this->x = x;    this->y = y;
    }
    coord(coord &t) {
        this->x = t.x;    this->y = t.y;
    }
    coord operator=(coord const &b) {
        return coord(b.x, b.y);
    }
}screen(51, 17), fullmap(MaxX, MaxY);

coord operator-(coord const &a, coord const &b) {
    return coord(a.x - b.x, a.y - b.y);
}
coord operator+(coord const &a, coord const &b) {
    return coord(a.x + b.x, a.y + b.y);
}
bool operator==(coord const &a, coord const &b) {
    return (a.x == b.x && a.y == b.y) ? true : false;
}

coord view(coord *p, coord s, coord m) {
    coord hs = coord(s.x/2, s.y/2);
    coord c = *p - hs;
    if(c.x < 0)    c.x = 0;
    else if(c.x + s.x > m.x)    c.x = m.x - s.x;
    if(c.y < 0)    c.y = 0;
    else if(c.y + s.y > m.y)    c.y = m.y - s.y;
    return c;
}

#endif