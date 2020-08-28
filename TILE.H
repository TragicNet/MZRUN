#ifndef TILE_H
#define TILE_H

struct Tile {
    bool blocked;
    bool block_sight;
    bool player_only;
} map[MaxY][MaxX];

bool Tile_isBlocked(struct Tile *this)     {   return this->blocked;       }
void Tile_block(struct Tile *this)         {   this->blocked = true;       }
void Tile_unblock(struct Tile *this)       {   this->blocked = false;      }
bool Tile_isOpaque(struct Tile *this)      {   return this->block_sight;   }
void Tile_blockSight(struct Tile *this)    {   this->block_sight = true;   }
void Tile_unblockSight(struct Tile *this)  {   this->block_sight = false;  }

void Tile_init(struct Tile *this, bool blocked, bool block_sight, bool player_only) {
    this->blocked = blocked;
    this->block_sight = block_sight;
    this->player_only = player_only;
}

#endif