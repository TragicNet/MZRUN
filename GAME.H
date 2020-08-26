#ifndef GAME_H
#define GAME_H

void GameLoop() {
	Engine game;
	game.HUD();
	while(!game.getState()) {
		game.drawEngine();
		game.inputEngine();
		game.handleEngine();
	}
}

void Start() {
	int i, x = 35, y = 12, key;
	bool pressed = false;
	clrscr();
	gotoxy(35,9);
	textcolor(YELLOW);	cprintf("MAZE RUNNER");
	textcolor(WHITE);
	gotoxy(37,12);	cprintf(" Start ");
	gotoxy(37,13);	cprintf("Options");
	gotoxy(37,14);	cprintf(" Exit  ");
	textcolor(GREEN);
	for(i = 11; i <= 15; i++) {	//Vertical
		gotoxy(33, i);	cprintf("%c", border[1]);	gotoxy(47, i);	cprintf("%c", border[1]);
	}
	for(i = 33; i <= 47; i++) {	//Horizontal
		gotoxy(i, 11);	cprintf("%c", border[0]);	gotoxy(i, 15);	cprintf("%c", border[0]);
	}
	gotoxy(33, 11);	cprintf("%c", border[2]);	gotoxy(47, 11);	cprintf("%c", border[3]);
	gotoxy(33, 15);	cprintf("%c", border[4]);	gotoxy(47, 15);	cprintf("%c", border[5]);
	while(1) {
		gotoxy(x,y);	cprintf("<");	gotoxy(x+10,y);	cprintf(">");
		key = getch();
		gotoxy(x,y);	cprintf(" ");	gotoxy(x+10,y);	cprintf(" ");
		switch(key) {
		case 'w':	y = (y==12) ? 12 : y-1;	break;
		case 's':	y = (y==14) ? 14 : y+1;	break;
		case 13:	pressed = true;	break;
		case 27:	return;
		}
		if(pressed)	break;
	}
	switch(y-12) {
	case 0:	GameLoop();	break;
	case 1:	break;
	case 2: exit(0);	break;
	}
}

#endif