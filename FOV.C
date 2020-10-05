#include <stdio.h>
#include <conio.h>
#include <math.h>

#define bool int
#define true 1
#define false 0

int range = 7;
int nx[] = { -1, 1, 0, 0 };
int ny[] = { 0, 0, -1, 1 };

struct coord {
	int x, y;
} p, m;

bool coord_cmp(struct coord a, struct coord b) {	return (a.x == b.x && a.y == b.y) ? true : false;	}

struct coord coord_sub(struct coord a, struct coord b) {
    a.x-=b.x;	a.y-=b.y;
    return a;
}

int distance(int xdiff, int ydiff) {
	int val = abs(xdiff * xdiff) + abs(ydiff * ydiff);
	val = (unsigned)((val<0) ? -val : val);
	if(val < 0) {
		clrscr();	printf("xdiff: %d\nydiff: %d\nval: ", xdiff, ydiff, val);	getch();	exit(0);
	}
	return (val <= 0) ? 0 : val;//sqrt(val);
}

int view(struct coord this, struct coord t) {
	return distance( (this.x - t.x), (this.y - t.y) * 2 );
}

char map[10][40] = {
	"########################################",
	"#                                      #",
	"#            #####                     #",
	"#                #     m#              #",
	"#            #   #         p #         #",
	"#            ## ##                     #",
	"#                                      #",
	"#                                      #",
	"#                                      #",
	"########################################"
};

void getPos() {
	int i, j;
	for(j = 0; j < 10; j++) {
		for(i = 0; i < 40; i++) {
			if(map[j][i] == 'm') {	m.x = i;	m.y = j;	}
			else if(map[j][i] == 'p') {	p.x = i;	p.y = j;	}
		}
	}

}

bool visible(struct coord start, struct coord end) {
	int xi, yi, error;
	struct coord current, errorCorrect, delta;
	if(view(start, end) >= (range*range))	return false;
	xi = (end.x > start.x) ? 1 : -1;
	yi = (end.y > start.y) ? 1 : -1;
	current.x = start.x;	current.y = start.y;
	delta.x = abs(start.x-end.x);	delta.y = abs(start.y-end.y);
	error = delta.x - delta.y;
	errorCorrect.x = delta.x*2, errorCorrect.y = delta.y*2;
	while(1) {
		if(current.x > fullmap.x || current.y > fullmap.y || current.x < 1 || current.y < 1 )	break;
		if(coord_cmp(current, end))	break;
		else if(map[current.y][current.x] == '#')	return false;
		else if(current.x - end.x == xi || current.y == yi)	break;
		if(error > 0) {	current.x+=xi;	error-=errorCorrect.y;	}
		else if(error < 0) {	current.y+=yi;	error+=errorCorrect.x;	}
		else {	current.x+=xi;	current.y+=yi;	}
	}
	return true;
}

void draw() {
	int i, j;
	struct coord t;
	gotoxy(1, 1);
	for(j = 0; j < 10; j++) {
		for(i = 0; i < 40; i++) {
			t.x = i, t.y = j;
			if(view(t, p) >= range * range) {
				cprintf(".", map[j][i]);
			} else if(visible(p, t)) {
				cprintf("%c", map[j][i]);
			} else  cprintf(".");
			textattr(WHITE + (BLACK<<4));
		}
		printf("\n");
	}
	gotoxy(p.x + 1, p.y + 1);	cprintf("p");
	gotoxy(m.x + 1, m.y + 1);	cprintf("m");
	gotoxy(1, 11);
	printf("p: %d, %d", p.x, p.y);
	printf("\tm: %d, %d", m.x, m.y);
}

void main() {
    clrscr();
    getPos();
	draw();
	printf("\nVisible: %s", (visible(m, p) ? "true" : "false"));
	getch();
}