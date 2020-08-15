#include <stdio.h>
#include <conio.h>
#include <string.h>

char border[] = { 196, 179, 218, 191, 192, 217 };

class coord {
public:
	int x, y;
	coord() {}
	coord(int x, int y) {
		this->x = x;	this->y = y;
	}
};

void create_window() {
	int i, j, cx = 40, cy = 13;
	char text[] = "You Died!";
	int len = strlen(text);
	for(j=1; j<1; j++) {
		gotoxy(cx, cy+j);
		for(i=1; i<=len+2;i++) {
			if(i==1 || i==len+2)	cprintf("%c", border[1]);
			else	cprintf("%c", ' ');
		}
	}
	for(i=1; i<=len+2;i++) {
		if(i==1) {
			gotoxy(cx, cy);		cprintf("%c", border[2]);
			gotoxy(cx, cy+j);	cprintf("%c", border[4]);
		}
		else if(i==len+2) {
			gotoxy(cx, cy);		cprintf("%c", border[3]);
			gotoxy(cx, cy+j);	cprintf("%c", border[5]);
		}
		else {
			gotoxy(cx, cy);		cprintf("%c", border[0]);
			gotoxy(cx, cy+j);	cprintf("%c", border[0]);
		}
	}
}

void temp() {
	int i, j, cx = 40, cy = 13;
	int width = 12;
	
	gotoxy(cx, cy++);	cprintf("%c", border[2]);
	for(i = 0; i<width-2; i++)	cprintf("%c", border[0]);	cprintf("%c", border[3]);
	
	gotoxy(cx, cy++);	cprintf("%c You Died %c", border[1]);
	
	gotoxy(cx, cy++);	cprintf("%c", border[2]);
	for(i = 0; i<width-2; i++)	cprintf("%c", border[0]);	cprintf("%c", border[3]);
}

void main() {
	clrscr();
	//create_window();
	temp();
	getch();
}