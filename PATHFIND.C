//include <MZRUN/includes/GLOBALS.H>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <dos.h>
#include <bios.h>

#define bool int
#define true 1
#define false 0

char map[10][40] = {
	"########################################",
	"#         #  #                         #",
	"#         #  ##### ###                 #",
	"#                # #                   #",
	"#        *#  #  ##@#                   #",
	"#         #  ## ###                    #",
	"#         #          #                 #",
	"#         ####  #### #                 #",
	"#            #       #                 #",
	"########################################"
};

/*char map[10][40] = {
	"########################################",
	"#                                      #",
	"#            #####                     #",
	"#                #                     #",
	"#            #   #@                    #",
	"#           *## ##                     #",
	"#                                      #",
	"#                                      #",
	"#                                      #",
	"########################################"
};*/

void draw() {
	int i, j;
	gotoxy(1, 1);
	for(j = 0; j < 10; j++) {
		for(i = 0; i < 40; i++) {
			if(map[j][i] == '#')	textattr(WHITE + (LIGHTGRAY<<4));
			else if(map[j][i] == '*')	textattr(RED + (BLACK<<4));
			else if(map[j][i] == '@')	textattr(GREEN + (BLACK<<4));
			cprintf("%c", map[j][i]);
			textattr(WHITE + (BLACK<<4));
		}
		printf("\n");
	}
}

bool reached = false;

enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
int nx[] = { -1, 1, 0, 0 };
int ny[] = { 0, 0, -1, 1 };


void halt(int x) {
	delay(x);
	while(1)
	{	if(kbhit())	getch();
		else	break;
	}
}

void noise(int f, int d) {
	sound(f);	halt(d);	nosound();
}

struct coord {
	int x, y;
};

bool coord_cmp(struct coord a, struct coord b) {	return (a.x == b.x && a.y == b.y) ? true : false;	}

int heuristic(const struct coord a, const struct coord b) {
	return abs(a.x-b.x) +  abs(a.y-b.y);
}

int distance(const struct coord a, const struct coord b) {
	return abs((a.x-b.x) * (a.x-b.x)) + abs((a.y-b.y) * (a.y-b.y));
}

struct block {
	struct coord pos;
	int f, g, h;
	struct block *next;
	struct block *prev;
} player, monster;

bool block_Equal(struct block a, struct block b) {
	return coord_cmp(a.pos, b.pos);
}

void block_Init(struct block *this, int x, int y) {
	this->pos.x = x;    this->pos.y = y;
	this->f = this->g = this->h = 0;
	this->prev = NULL;	this->next = NULL;
}

void block_Print(struct block *this) {
	struct block *p;
	p = this;
	while(p != NULL) {
		printf(" > :%d: %2d, %2d", p->f, p->pos.x, p->pos.y);
		p = p->next;
	}
}

void block_Push(struct block **this, struct block obj) {
	struct block *l, *n = (struct block*) malloc(sizeof(struct block));

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

void block_Delete(struct block **this, struct block obj) {
	struct block *p, *del = NULL;
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
	if (block_Equal(**this, *del))
		*this = del->next;
	if (del->next != NULL)
		del->next->prev = del->prev;
	if (del->prev != NULL)
		del->prev->next = del->next;
	free(del);
}

bool block_Search(struct block *this, struct block obj) {
	struct block *p;
	p = this;
	while(p != NULL) {
		if(p->pos.x == obj.pos.x && p->pos.y == obj.pos.y)
			return true;
		p = p->next;
	}
	return false;
}

enum eDirection astar(struct block start_node, struct block end_node) {
	// Initialize both open and closed list
	struct block *open_list = NULL, *closed_list = NULL, *children = NULL;
	struct block current_node, item, temp, *p, *closed_child = NULL, *open_node = NULL, *child = NULL;
	struct coord r;
	int k;

	// Add the start node
	block_Push(&open_list, start_node);

	// Return if already there
	if(coord_cmp(start_node.pos, end_node.pos))	return STOP;

	// Loop until you find the end
	while(open_list != NULL) {
		// Get the current node
		current_node = *open_list;

		for(p = open_list; p != NULL; p = p->next)
			if(p->f < current_node.f)
				current_node = *p;

		// Pop current off open list, add to closed list
		block_Delete(&open_list, current_node);
		//path_Push(&path_list, current_node.pos);
		block_Push(&closed_list, current_node);

		/*getch();
		map[current_node.pos.y][current_node.pos.x] = 'o';
		clrscr();
		draw();
		printf("\n\ncurrent: %2d, %2d", current_node.pos.x, current_node.pos.y);
		//r = current_node.parent;
		//printf("\tParent: %2d, %2d", r.x, r.y);
		printf("open: ");	block_Print(open_list);
		printf("\n\nclosed: ");	block_Print(closed_list);*/

		//Display Whole
		//if(!coord_cmp(current_node.pos, start_node.pos) && !coord_cmp(current_node.pos, end_node.pos)) {	gotoxy(1+current_node.pos.x, 1 + current_node.pos.y);	printf(".");	delay(50); }

		// Found the goal
		if(block_Equal(current_node, end_node)) {
			r = current_node.pos;
			for(closed_child = closed_list; closed_child->next != NULL; closed_child = closed_child->next) {}
			for(; closed_child != NULL; closed_child = closed_child->prev) {
				if(distance(r, closed_child->pos) == 1 && !coord_cmp(closed_child->pos, start_node.pos))
					r = closed_child->pos;	//printf(" > %2d, %2d", r.x, r.y);
			}

			/*printf("\nReached");
			printf("\n\nCurrent: ");	block_Print(&current_node);
			printf("\nnew: %2d, %2d", r.x, r.y);
			getch();	exit(0);*/

			for(k = 0; k < 4; k++) {
				if(r.x - start_node.pos.x == nx[k] && r.y - start_node.pos.y == ny[k])
					return k + 1;
				//{	printf("\ndir: %d", k+1);	getch();	exit(0);	}
			}
		}
		block_Delete(&children, *children);
		block_Delete(&children, *children);
		block_Delete(&children, *children);
		block_Delete(&children, *children);
		children = NULL;
		// Generate children
		for(k = 0; k<4; k++) {
			// Get node position
			r.x = current_node.pos.x + nx[k], r.y = current_node.pos.y + ny[k];
			
			// Make sure within range
			if(r.x > 40-1 || r.y > 10-1 || r.x < 1 || r.y < 0)
				continue;
			
			// Make sure walkable terrain
			if(map[r.y][r.x] == '#')
				continue;
			
			for(closed_child = closed_list; closed_child != NULL; closed_child = closed_child->next)
				if(coord_cmp(r, closed_child->pos))
					break;
			if(closed_child != NULL)	continue;
			
			block_Init(&temp, r.x, r.y);
			block_Push(&children, temp);
		}
		
		for(child = children; child != NULL; child = child->next) {
			
			// Child is on the closed list
			for(closed_child = closed_list; closed_child != NULL; closed_child = closed_child->next)
				if(coord_cmp(child->pos, closed_child->pos))
					break;
			if(closed_child == NULL) {
				// Create the f, g, and h values
				child->g = heuristic(child->pos, start_node.pos);
				//Not Finding best route
				//child->g = current_node.g + 1;
				child->h = heuristic(child->pos, end_node.pos);
				child->f = child->g + child->h;

				// Child is already in the open list
				for(open_node = open_list; open_node != NULL; open_node = open_node->next)
					if( block_Equal(*child, *open_node) && child->g >= open_node->g )
						break;
				if(open_node == NULL) {
					// Add the child to the open list
					block_Push(&open_list, *child);
				}
			}
		}
	}
	return rand() % 4;
}

void ppos() {
	int i, j, px, py, mx, my;
	for(j = 0; j < 10; j++) {
		for(i = 0; i < 40; i++) {
			if(map[j][i] == '*') {	mx = i;	my = j;	}
			else if(map[j][i] == '@') {	px = i;	py = j;	}
		}
	}
	block_Init(&player, px, py);
	printf("\n p: %2d, %2d", player.pos.x, player.pos.y);
	block_Init(&monster, mx, my);
	printf("\n m: %2d, %2d", monster.pos.x, monster.pos.y);
}

void tuturu() {
	int i;
	noise(300, 100);	halt(100);
	noise(250, 100);		halt(50);
	noise(300, 500);
	halt(500);
	for(i = 20; i < 160; i+=20) {
		noise(i, 100);
	}
	noise(i, 400);	noise(i-30, 400);	halt(200);

	for(i = 40; i < 180; i+=20) {
		noise(i, 100);
	}
	noise(i, 400);	noise(i-30, 400);	halt(200);

	for(i = 60; i < 200; i+=20) {
		noise(i, 100);
	}
	noise(220, 400);	halt(25);
	noise(220, 70);	halt(100);
	noise(220, 70);	halt(100);
	noise(220, 70);	halt(100);
	noise(250, 1000);
}

void main() {
	struct block *t = NULL, o;
	enum eDirection d = STOP;
	int i;
	_setcursortype(_NOCURSOR);
	textcolor(WHITE);
	clrscr();
	ppos();
	//Checking DLL operations
	/*block_Push(&t, &monster);
	block_Push(&t, &player);
	for(i = 1; i < 3; i++) {
		block_Init(&o, i, i, NULL);
		block_Push(&t, &o);
	}
	printf("\nt: ");	block_Print(t);
	printf("\nfound: %d", block_Search(t, player));
	block_Delete(&t, player);
	printf("\tfound: %d", block_Search(t, player));
	printf("\nt: ");	block_Print(t);*/
	
	//Checking Single Runs uncomment astar code
	//printf("\nDirection: %d", astar(monster, player));
	
	//Complete Run
	draw();
	getch();
	while(1) {
		draw();
		d = astar(monster, player);
		if(d == STOP)	break;
		map[monster.pos.y][monster.pos.x] = ' ';
		monster.pos.x += nx[d-1], monster.pos.y += ny[d-1];
		map[monster.pos.y][monster.pos.x] = '*';
		halt(300);
	}
	//tuturu();
	getch();
	_setcursortype(_NORMALCURSOR);
}