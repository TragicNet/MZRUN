#include <iostream.h>
#include <conio.h>
#include <stdio.h>
#include <bios.h>
#include <process.h>

#define bool int
#define true 1
#define false 0

int gameOver = false;
char map[20][60] = {
    "############################################################",
    "#...#...............#.#....................................#",
    "#...#...#.#........#..#...............*....................#",
    "###.#...#.#..##...#...#.....*..#...........................#",
    "#....####.####*###....#.......###..........................#",
    "#.....................#.......#.......*....................#",
    "#...........**........#......###...........................#",
    "#.....*.......................#............................#",
    "#.................*...........#..*......*..................#",
    "#......*...#...#............###............................#",
    "#####.....#....#......#....##.###.....*....................#",
    "#....#...#..*..##...#.#.........#..........................#",
    "#....#*.#............#.........#...........................#",
    "#.....##..........*...........#..*......*..................#",
    "#......*.....#.......#.......#.............................#",
    "#..........##.#......#..*...#.#....*.......................#",
    "#........*#.#..*....#..........#...........................#",
    "#........##.....................#....*.....................#",
    "#*.........#................*....#.........................#",
    "############################################################"
};
class pos {
public:
    int x, y;
    pos(int x, int y) {
        this->x = x;    this->y = y;
    }
    pos(pos &t) {
        this->x = t.x;    this->y = t.y;
    }
    pos operator=(pos const &b) {
        return pos(b.x, b.y);
    }
} player(1, 1), screen(11, 5), fullmap(60, 20), origin(0, 0);

pos operator-(pos const &a, pos const &b) {
    return pos(a.x - b.x, a.y - b.y);
}
pos operator+(pos const &a, pos const &b) {
    return pos(a.x + b.x, a.y + b.y);
}
int operator==(pos const &a, pos const &b) {
    return (a.x == b.x && a.y == b.y) ? 1 : 0;
}

pos scroll(pos p, pos s, pos m) {
    pos hs = pos(s.x/2, s.y/2);
    pos c = p - hs;
    if(c.x < 0)    c.x = 0;
    else if(c.x + s.x > m.x)    c.x = m.x - s.x;
    if(c.y < 0)    c.y = 0;
    else if(c.y + s.y > m.y)    c.y = m.y - s.y;
    return c;
}
void draw() {
    gotoxy(1, 1);
    pos camera = scroll(player, screen, fullmap);
    for(int i = 0; i<screen.y; i++) {
        for(int j = 0; j<screen.x; j++) {
            if(player == camera + pos(j, i))
                cout<<'@';
            else
                cout<<map[camera.y + i][camera.x + j];
        }
        cout<<endl;
    }
    printf("\ncamera\nx: %2d y: %2d", camera.x, camera.y);
    printf("\nplayer\nx: %2d y: %2d", player.x, player.y);
}
void input() {
	if(kbhit()) {
		switch(getch()) {
			case 27:	gameOver = true;
						break;
			case 'w':	player.y--;
						break;
			case 's':	player.y++;
						break;
			case 'a':	player.x--;
						break;
			case 'd':	player.x++;
						break;
		}
	}
}

void main() {
    clrscr();
	_setcursortype(_NOCURSOR);
	while(!gameOver) {
		draw();
		input();
	}
}