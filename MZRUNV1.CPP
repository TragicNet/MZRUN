#include "MZRUN/GLOBALS.H"

void main() {
	clrscr();
	_setcursortype(_NOCURSOR);
	Engine game;
	game.HUD();
	while(!game.getState()) {
		game.drawEngine();
		game.inputEngine();
		game.handleEngine();
	}
}
