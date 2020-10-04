#include <stdio.h>
#include <conio.h>
#include <math.h>

#define bool int
#define true 1
#define false 0

int range = 5;
int nx[] = { -1, 1, 0, 0 };
int ny[] = { 0, 0, -1, 1 };

struct coord {
	int x, y;
}p, m;

bool coord_cmp(struct coord a, struct coord b) {	return (a.x == b.x && a.y == b.y) ? true : false;	}

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
	"#                #                     #",
	"#            #   #         p           #",
	"#            ## ##             m       #",
	"#                          #           #",
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

bool visible(struct coord src, struct coord trg) {
	int i, j, dx = src.x - trg.x, dy = src.y - trg.y, xi = dx, yi = dy;
	struct coord t;
	xi = dx;	if(xi > 0)  xi = 1;  else if(xi < 0) xi = -1;
	yi = dy;	if(yi > 0)  yi = 1;  else if(yi < 0) yi = -1;
	if(coord_cmp(trg, m)) {
		printf("\nsrc: %2d, %2d, trg: %2d, %2d", src.x, src.y, trg.x, trg.y);
		printf("\ndiff = %2d, %2d", abs(dx), abs(dy));
		printf("\nincr: %2d, %2d", xi, yi);
	}
	//if(view(src, trg) > range*range)	return false;
	/*for(i = src.x; i != trg.x + xi; ) {
        if(i>0 && i<40) i+=xi;
		for(j = src.y; j != trg.y + yi; ) {
            if(j>0 && j<10) j+=yi;
            if(map[j][i] == '#')    return false;
        }
    }*/
	return true;
}

void draw() {
	int i, j;
	struct coord t;
	gotoxy(1, 1);
	for(j = 0; j < 10; j++) {
		for(i = 0; i < 40; i++) {
			t.x = i, t.y = j;
			if(view(p, t) > range * range) {
				cprintf("%c", map[j][i]);
			} else if(!visible(t, p))
				cprintf("/");
			else
				cprintf(".");
			textattr(WHITE + (BLACK<<4));
		}
		printf("\n");
	}
	gotoxy(p.x + 1, p.y + 1);	cprintf("p");
	gotoxy(m.x + 1, m.y + 1);	cprintf("m");
	gotoxy(1, 11);
}

void main() {
    clrscr();
    getPos();
	draw();
	printf("\nVisible: %s", (visible(p, m) ? "true" : "false"));
    getch();
}