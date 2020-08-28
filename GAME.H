#ifndef GAME_H
#define GAME_H

void init() {
	int i, j;
	coord_init(&board, 5, 5);
	coord_init(&screen, 39, 13);
	for(i = 0; i < MaxY; i++) {
		for(j = 0; j < MaxX; j++) {
			Tile_init(&map[i][j], false, false, false);
		}
	}
	Object_init(&wall, 0, 0, STOP, '#', "wall", WHITE, LIGHTGRAY, 0, 0, 1);
	Object_init(&air, 0, 0, STOP, '.', "air", LIGHTGRAY, BLACK, 0, 0, 0);
	Object_init(&blackair, 0, 0, STOP, '.', "air", BLACK, BLACK, 0, 0, 0);
}

void GameLoop() {
	struct Engine game;	Engine_init(&game);
	Engine_HUD(&game);
	while(!Engine_getState(&game)) {
		Engine_draw(&game);
		Engine_input(&game);
		Engine_handle(&game);
	}
}

void Start() {
	int i, x = 35, y = 12, key;
	bool pressed = false;
	init();
	clrscr();
    /*int t = CYAN;
    textcolor(t); lowvideo();   cprintf("Hello");
    textcolor(t); highvideo();  cprintf("Hello");
    textcolor(t); normvideo();  cprintf("Hello");*/
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