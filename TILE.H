#ifndef TILE_H
#define TILE_H

//#include "MZRUN/GLOBALS.H"

class Tile {
private:
    bool blocked;

public:
    Tile(bool blocked = false);
    ~Tile() {};
    inline bool isBlocked() {   return this->blocked;   }
    inline void block() {   this->blocked = true;   }
    inline void unblock()   { this->blocked = false;    }
};
Tile map[MaxY][MaxX];

Tile :: Tile(bool blocked) {
    this->blocked = blocked;
}

#endif