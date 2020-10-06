#ifndef SCRIPT_H
#define SCRIPT_H

void script_death(struct Engine *this) {
	int i, j, cx = 40, cy = 13;
	int height = 3, width = 12;
	this->gameOver = true;
	halt(225);
	Engine_draw(this);
	cx-=width/2;	cy-=height/2;
	textcolor(LIGHTRED);
	gotoxy(cx, cy++);	cprintf("%c", border[2]);	for(i = 0; i<width-2; i++)	cprintf("%c", border[0]);	cprintf("%c", border[3]);
	gotoxy(cx, cy++);	cprintf("%c GAMEOVER %c", border[1], border[1]);
	gotoxy(cx, cy++);	cprintf("%c", border[4]);	for(i = 0; i<width-2; i++)	cprintf("%c", border[0]);	cprintf("%c", border[5]);

	noise(100, 100);	halt(15);	noise(100, 100);
	getch();
}

void script_fall(struct Engine *this) {
	Engine_draw(this);	halt(200);
	this->gameOver = true;
	//Halt for visible player Death
	noise(30, 50);
	Engine_draw(this);	halt(50);
	noise(25, 75);
	script_death(this);
}

void script_kill() {
	int i;
	for(i=100; i<200; i+=50) {
		noise(i, i-50);
	}
}

void script_eaten(struct Engine *this) {
	int i;
	Engine_draw(this);
	this->gameOver = true;
	for(i=150; i>50; i-=10) {
		noise(i, i-50);
	}
	script_death(this);
}

void script_cleared(struct Engine *this) {
	int i, j, cx = 40, cy = 13;
	int height = 3, width = 11;
	Engine_draw(this);
	cx-=width/2;	cy-=height/2;
	this->gameOver = true;
	this->cleared = true;
	textcolor(LIGHTGREEN);
	gotoxy(cx, cy++);	cprintf("%c", border[2]);	for(i = 0; i<width-2; i++)	cprintf("%c", border[0]);	cprintf("%c", border[3]);
	gotoxy(cx, cy++);	cprintf("%c CLEARED %c", border[1], border[1]);
	gotoxy(cx, cy++);	cprintf("%c", border[4]);	for(i = 0; i<width-2; i++)	cprintf("%c", border[0]);	cprintf("%c", border[5]);

	noise(100, 100);	halt(5);
	noise(250, 100);	halt(5);
	noise(150, 100);	halt(5);
	noise(300, 100);	halt(5);
	getch();
}

#endif