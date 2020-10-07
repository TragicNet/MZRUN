#ifndef TILE_H
#define TILE_H

// Structure each tile
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

#endif