#ifndef HELP_H
#define HELP_H

void help() {
	textbackground(BLUE);	textcolor(YELLOW);
	clrscr();
	gotoxy(37,2);	cprintf("<Help>");
	gotoxy(5, 3);	cprintf("EFFECTS");
	getch();
	textcolor(WHITE);	textbackground(BLACK);
}

#endif