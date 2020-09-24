#include "MZRUN/GLOBALS.H"
//Hello
void main() {
	clrscr();
	_setcursortype(_NOCURSOR);
	Game_init();
	while(Game_Start()) {}
}