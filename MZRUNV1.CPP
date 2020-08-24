#include "MZRUN/GLOBALS.H"

void main() {
	int gd = DETECT, gm;
	initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");

	clrscr();
	_setcursortype(_NOCURSOR);
	Engine game;
	game.HUD();
	while(!game.getState()) {
		game.drawEngine();
		game.inputEngine();
		game.handleEngine();
	}
	closegraph();
}
