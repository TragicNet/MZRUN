#ifndef HELP_H
#define HELP_H

void help_page1();
void help_page2();
void help_page3();

void help() {
	int key, page = 1;
	bool pressed = true;
	textbackground(BLACK);
	while(1) {
		if(pressed) {
			textcolor(YELLOW);	clrscr();	gotoxy(37,2);	cprintf("<Help>");
			textcolor(WHITE);
			switch (page)
			{
			case 1:	help_page1();	break;
			case 2:	help_page2();	break;
			case 3:	help_page3();	break;
			}
			pressed = false;
		}
		key = getch();
		switch (key) {
		case 'a':
			if(page>1)	page--;	pressed = true;	break;
		case 'd':
			if(page<3)	page++;	pressed = true;	break;
		case 27:
			textcolor(WHITE);	textbackground(BLACK);	return;
		}
	}
}

void help_page1() {
	gotoxy(5, 4);	cprintf("Find your way through the maze, to reach the goal ");
	gotoxy(5, 6);	cprintf("Stay away from monsters, they will follow you and kill you");
	gotoxy(5, 8);	cprintf("You can pick up items, some give you an advantage while some...");

	textcolor(YELLOW);
	gotoxy(20, 24);	cprintf("  a: LEFT     ");	cprintf("  d: RIGHT    ");	cprintf("esc: EXIT    ");
	//gotoxy(5, 24);	cprintf("<");
	gotoxy(75, 24);	cprintf(">");
}

void help_page2() {
	struct Object goal, item, monster, player;
	Object_init(&goal, 0, 0, STOP, 'g', "goal", "goal1", CYAN, CYAN, 0, 0, 0);
	Object_init(&item, 0, 0, STOP, 4, "effect", "fullvision1", LIGHTBLUE, BLACK, 0, 0, 0);
	Object_init(&monster, 0, 0, STOP, '*', "monster", "normal", RED, BLACK, 5, 4, 1);
	Object_init(&player, 0, 0, RIGHT, '@', "player", "player", GREEN, BLACK, 0, 0, 1);

	textcolor(YELLOW);	gotoxy(38, 4);	cprintf("KEY");	textcolor(WHITE);
	gotoxy(5, 6);	Object_draw(&player);	cprintf(" Player");
	gotoxy(25, 6);	Object_draw(&wall);		cprintf(" Wall");
	gotoxy(45, 6);	Object_draw(&air);		cprintf(" Air/Floor");
	gotoxy(65, 6);	Object_draw(&item);		cprintf(" Item");
	gotoxy(5, 8);	Object_draw(&monster);	cprintf(" Normal Monster");
	gotoxy(25, 8);	Object_draw(&goal);		cprintf(" Goal");
	gotoxy(45, 8);	Object_draw(&blackair);	cprintf(" No Vision/Pit");
	
	textcolor(YELLOW);
	gotoxy(20, 24);	cprintf("  a: LEFT     ");	cprintf("  d: RIGHT    ");	cprintf("esc: EXIT    ");
	gotoxy(5, 24);	cprintf("<");
	gotoxy(75, 24);	cprintf(">");
}

void help_page3() {
	textcolor(YELLOW);	gotoxy(36, 4);	cprintf("EFFECTS");	textcolor(WHITE);
	window(5, 1, 75, 25);
	gotoxy(1, 6);	cprintf("Effects are status effects that buff or debuff different entities");
	gotoxy(1, 7);	cprintf("Effects have different duration and values that they affect");
	window(3, 1, 77, 25);
	gotoxy(1, 9);	cprintf("IMM: Immunity, The player can't die for the duration");
	gotoxy(1, 10);	cprintf("BRK: Breaker, The player can't die, the state is removed when triggered");
	gotoxy(1, 11);	cprintf("VIS: Vision, Grants vision");
	gotoxy(1, 12);	cprintf("SLW: Slow, Decreases speed of monsters");
	gotoxy(1, 13);	cprintf("FST: Fast, Increases speed of monsters");
	gotoxy(1, 14);	cprintf("RPL: Repel, Decreases range of monsters");
	gotoxy(1, 15);	cprintf("ATR: Attract, Increases range of monsters");
	gotoxy(1, 16);	cprintf("INV: Invisibility, Player is invisible to monsters");
	gotoxy(1, 11);	cprintf("HND: Hinder, 50% chance to ignore player movement");
	gotoxy(1, 11);	cprintf("PAR: Paralysis, Stops Player");
	gotoxy(1, 11);	cprintf("STP: Stop, Stops monsters");

	window(1, 1, 80, 25);
	textcolor(YELLOW);
	gotoxy(20, 24);	cprintf("  a: LEFT     ");	cprintf("  d: RIGHT    ");	cprintf("esc: EXIT    ");
	gotoxy(5, 24);	cprintf("<");

	//gotoxy(75, 24);	cprintf(">");
}

#endif