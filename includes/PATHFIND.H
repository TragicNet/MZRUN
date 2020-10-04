#ifndef PATHFINDER_H
#define PATHFINDER_H

int heuristic(const struct coord a, const struct coord b) {
	return abs(a.x-b.x) +  abs(a.y-b.y);
}

struct Block {
	struct coord pos;
	int f, g, h;
	struct Block *next;
	struct Block *prev;
};

bool Block_equal(struct Block a, struct Block b) {
	return coord_cmp(a.pos, b.pos);
}

void Block_init(struct Block *this, struct coord loc) {
	this->pos = loc;
	this->f = this->g = this->h = 0;
	this->prev = NULL;	this->next = NULL;
}

void Block_push(struct Block **this, struct Block obj) {
	struct Block *l, *n = (struct Block*) malloc(sizeof(struct Block));

	n->pos.x = obj.pos.x;	n->pos.y = obj.pos.y;
	n->f = obj.f;	n->g = obj.g;	n->h = obj.h;
	n->next = NULL;	n->prev = NULL;

	if(*this == NULL) {
		*this = n;	return;
	}
	l = *this;
	while(l->next != NULL)	l = l->next;
	l->next = n;
	n->prev = l;
}

void Block_delete(struct Block **this, struct Block obj) {
	struct Block *p, *del = NULL;
	p = *this;
	while(p != NULL) {
		if(p->pos.x == obj.pos.x && p->pos.y == obj.pos.y) {
			del = p;	break;
		}
		p = p->next;
	}
	if(*this == NULL || del == NULL) {
		return;
	}
	if (Block_equal(**this, *del))
		*this = del->next;
	if (del->next != NULL)
		del->next->prev = del->prev;
	if (del->prev != NULL)
		del->prev->next = del->next;
	free(del);
}

void Block_deleteAll(struct Block **this) {
	struct Block *p = *this, *q;
	while(p != NULL) {
		q = p->next;
		Block_delete(this, *p);
		p = q;
	}
}

enum eDirection astar(struct coord s, struct coord e) {
	// initialize both open and closed list
	struct Block *open_list = NULL, *closed_list = NULL, *children = NULL;
	struct Block start_node, end_node, current_node, item, temp, *p, *closed_child = NULL, *open_node = NULL, *child = NULL;
	struct coord r;
	int k;
	Block_init(&start_node, s);	Block_init(&end_node, e);

	// Add the start node
	Block_push(&open_list, start_node);

	// Return if already there
	if(coord_cmp(start_node.pos, end_node.pos))	return 1 + random(4);

	// Loop until you find the end
	while(open_list != NULL) {
		// Get the current node
		current_node = *open_list;

		for(p = open_list; p != NULL; p = p->next)
			if(p->f < current_node.f)
				current_node = *p;

		// Pop current off open list, add to closed list
		Block_delete(&open_list, current_node);
		//path_push(&path_list, current_node.pos);
		Block_push(&closed_list, current_node);
		
		// Found the goal
		if(Block_equal(current_node, end_node)) {
			r = current_node.pos;
			for(closed_child = closed_list; closed_child->next != NULL; closed_child = closed_child->next) {}
			for(; closed_child != NULL; closed_child = closed_child->prev) {
				if(coord_distance(r, closed_child->pos) == 1 && !coord_cmp(closed_child->pos, start_node.pos))
					r = closed_child->pos;
			}

			for(k = 0; k < 4; k++) {
				if(r.x - start_node.pos.x == nx[k] && r.y - start_node.pos.y == ny[k]) {
					Block_deleteAll(&open_list);	Block_deleteAll(&closed_child);
					p = NULL, open_node = NULL, closed_child = NULL, child = NULL;
					free(p);	free(open_node);	free(closed_child);	free(child);
					//gotoxy(1, 1);	printf("dir: %d : Found: %d, %d : %d, %d", k, r.x, r.y, start_node.pos.x, end_node.pos.y);	getch();
					return k;
				}
			}
		}
		Block_deleteAll(&children);
		children = NULL;
		// Generate children
		for(k = 0; k<4; k++) {
			// Get node position
			r.x = current_node.pos.x + nx[k], r.y = current_node.pos.y + ny[k];
			
			// Make sure within range
			if(r.x > fullmap.x || r.y > fullmap.y || r.x < 1 || r.y < 1)
				continue;
			
			// Make sure walkable terrain
			if(!Tile_isOpen(&map[r.y][r.x]) && Tile_isBlocked(&map[r.y][r.x]) && map[r.y][r.x].playerOnly)
				continue;
			/*if(Tile_isBlocked(&map[r.y][r.x]))
				continue;*/
			
			for(closed_child = closed_list; closed_child != NULL; closed_child = closed_child->next)
				if(coord_cmp(r, closed_child->pos))
					break;
			if(closed_child != NULL)	continue;
			
			Block_init(&temp, r);
			Block_push(&children, temp);
		}
		
		for(child = children; child != NULL; child = child->next) {
			
			// Child is on the closed list
			for(closed_child = closed_list; closed_child != NULL; closed_child = closed_child->next)
				if(coord_cmp(child->pos, closed_child->pos))
					break;
			if(closed_child == NULL) {
				// Create the f, g, and h values
				child->g = current_node.g + 1;
				child->h = heuristic(child->pos, end_node.pos);
				child->f = child->g + child->h;

				// Child is already in the open list
				for(open_node = open_list; open_node != NULL; open_node = open_node->next)
					if( Block_equal(*child, *open_node) && child->g >= open_node->g )
						break;
				if(open_node == NULL) {
					// Add the child to the open list
					Block_push(&open_list, *child);
				}
			}
		}
	}
	Block_deleteAll(&open_list);	Block_deleteAll(&closed_child);
	p = NULL, open_node = NULL, closed_child = NULL, child = NULL;
	//free(p);	free(open_node);	free(closed_child);	free(child);
	return 1 + random(4);
}

#endif