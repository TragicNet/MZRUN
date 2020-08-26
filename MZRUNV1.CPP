#include "MZRUN/GLOBALS.H"

void main() {
	int gd = EGA, gm = EGAHI;
	clrscr();
	initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");

	_setcursortype(_NOCURSOR);
	Engine game;
	game.HUD();
	while(!game.getState()) {
		game.drawEngine();
		game.inputEngine();
		game.handleEngine();
	}
	/*game.drawEngine();
	getch();*/
	closegraph();
}
