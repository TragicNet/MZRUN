///include <MZRUN/includes/GLOBALS.H>
#include <stdio.h>
#include <conio.h>
#include <math.h>

#define bool int
#define true 1
#define false 0

char map[10][40] = {
	"////////////////////////////////////////",
	"/                                      /",
	"/                                      /",
	"/                #                     /",
	"/             *  #                     /",
	"/                #                     /",
	"/             @  #                     /",
	"/                                      /",
	"/                                      /",
	"////////////////////////////////////////"
};

struct block {
	int x, y;
	bool visited;
	int f, g, h;
	struct block *parent;
	struct block *neighbour[4];
    struct block *next;
} *temp, *start, *end, player, monster, *open, *closed;

void push(struct block *n) {
	if(start == NULL)	start = end = n;
	else {	end->next = n;	end = n;	}
}

void pop() {
	if(start != NULL) {
		temp = start;
		start = start->next;
		free(temp);
    }
}

void Block_init(struct block *this, int x, int y, bool visited, struct block *parent ) {
	int i;
	this->x = x;    this->y = y;    this->visited = visited;   this->parent = parent;
	this->f = this->g = this->h = 0;
    for(i=0; i<4; i++) {
		this->neighbour[i] = NULL;
	}
    this->next = NULL;
}

enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
int nx[] = { -1, 1, 0, 0 };
int ny[] = { 0, 0, -1, 1 };

float distance(int xdiff, int ydiff) {
	int val = abs(xdiff * xdiff) + abs(ydiff * ydiff);
	val = (unsigned)((val<0) ? -val : val);
	if(val < 0) {
		clrscr();	printf("xdiff: %d\nydiff: %d\nval: ", xdiff, ydiff, val);	getch();	exit(0);
	}
	return (val <= 0) ? 0 : sqrt(val);
}

enum eDirection find(struct block src) {
	int i, j, k, g, h, gxdiff, gydiff, hxdiff, hydiff;
    int min = 0;
    int f[] = { 0, 0, 0, 0 };
    int t[] = { 0, 0, 0, 0 };
	int order[] = { 1, 2, 3, 4 };
    enum eDirection dir;
    gxdiff = src.x - start->x, gydiff = src.y - start->y;
    hxdiff = src.x - player.x, hydiff = src.y - player.y;
    /*if(abs(xdiff) > abs(ydiff))
        dir = ( distance(xdiff-1, ydiff) > distance(xdiff+1, ydiff)) ? 1 : 2;
    else
        dir = ( distance(xdiff, ydiff-1) > distance(xdiff, ydiff+1)) ? 3 : 4;
    */
	if(hxdiff == 0 && hydiff == 0) {
        printf("same");
        return false;
    }
    if(src.visited) return false;

    for(i = 0; i < 4; i++) {
        g = distance(gxdiff + nx[i], gydiff + ny[i]);
        h = distance(hxdiff + nx[i], hydiff + ny[i]);
        f[i] += (g + h);
        t[i] = f[i];
    }
    /*printf("\nt: ");    for(i = 0; i < 4; i++) printf("%d ", t[i]);
    printf("\nf: ");    for(i = 0; i < 4; i++) printf("%d ", f[i]);
    printf("\norder: ");    for(i = 0; i < 4; i++) printf("%d ", order[i]);*/
    for(i = 0; i < 4; i++) {
        for(j = i; j < 4; j++)
            if(f[i] > f[j]) {
                f[i] += f[j];   f[j] = f[i] - f[j]; f[i] -= f[j];
                order[i] += order[j];   order[j] = order[i] - order[j]; order[i] -= order[j];
                /*printf("\n%d -> %d", i, j);
                printf("\tf: ");    for(k = 0; k < 4; k++) printf("%d ", f[k]);
                printf("\torder: ");    for(k = 0; k < 4; k++) printf("%d ", order[k]);*/
            }
    }
    /*printf("\nt: ");    for(i = 0; i < 4; i++) printf("%d ", t[i]);
    printf("\nf: ");    for(i = 0; i < 4; i++) printf("%d ", f[i]);
    printf("\norder: ");    for(i = 0; i < 4; i++) printf("%d ", order[i]);*/

    return (enum eDirection)order[0];
}

void draw() {
	int i, j, k, px, py, mx, my, wx[4], wy[4];
	gotoxy(1, 1);
	for(j = 0, k = 0; j < 10; j++) {
		for(i = 0; i < 40; i++) {
			printf("%c", map[j][i]);
			if(map[j][i] == '@') {	px = i;	py = j;	}
			else if(map[j][i] == '*') {	mx = i;	my = j;	}
			else if(map[j][i] == '#') {	wx[k] = i;	wy[k] = j;  k++;	}
            /*else    continue;
            getch();*/
		}
		printf("\n");
	}
	//temp = malloc(sizeof(struct node));
	Block_init(&player, px, py, false, NULL); //push(&temp);
	//temp = malloc(sizeof(struct node));
	Block_init(&monster, mx, my, false, NULL); push(&monster);
	i = monster.x - player.x;
	j = monster.y - player.y;
	//totDistance = distance(i, j);
	printf("0: STOP, 1: LEFT, 2: RIGHT, 3: UP, 4: DOWN");
	printf("\n p: %2d, %2d", player.x, player.y);
	printf("\t m: %2d, %2d", monster.x, monster.y);
	//printf("\n d: %2d, %2d = %f", i, j, totDistance);
	printf("\nw1: %2d, %2d", wx[0], wy[0]);
	printf("\tw2: %2d, %2d", wx[1], wy[1]);
	printf("\tw3: %2d, %2d", wx[2], wy[2]);
}

void main() {
	int i, j;
	enum eDirection d;
	start = end = NULL;
	clrscr();
    draw();
    printf("\nDirection: %d", find(monster));
    getch();
}