#ifndef TILE_H
#define TILE_H

//#include "MZRUN/GLOBALS.H"

class Tile {
private:
    bool blocked;
    bool block_sight;

public:
    Tile(bool blocked = false, bool block_sight = false);
    ~Tile() {};
    inline bool isBlocked()     {   return this->blocked;       }
    inline void block()         {   this->blocked = true;       }
    inline void unblock()       {   this->blocked = false;      }
    inline bool isOpaque()      {   return this->block_sight;   }
    inline void blockSight()    {   this->block_sight = true;   }
    inline void unblockSight()  {   this->block_sight = false;  }
};
Tile map[MaxY][MaxX];

Tile :: Tile(bool blocked, bool block_sight) {
    this->blocked = blocked;
    this->block_sight = block_sight;
}

#endif