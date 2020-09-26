#include "MZRUN/GLOBALS.H"
//Hello
void main() {
	struct Game game;
	clrscr();
	_setcursortype(_NOCURSOR);
	Game_init(&game);
	while(Game_Start(&game)) {}
}