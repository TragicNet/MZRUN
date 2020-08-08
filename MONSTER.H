#ifndef MONSTER_H
#define MONSTER_H

//#include "MZRUN/OBJECT.H"

class Monster : public Object {
public:
	Monster(){};
	void route();
	void operator=(Object src) {
		this->x = src.getX();
		this->y = src.getY();
		this->obj_symbol = src.getSymbol();
		this->obj_color =  src.getColor();
		this->obj_bgcolor = src.getBgColor();
		this->dir = src.getDirection();
    } 
};

void Monster :: route() {
    int tx, ty;
    eDirection r;
    do {
	    r = (eDirection) (1 + random(1000)/250);
        this->dir = r;
        switch(r) {
            case 1:   tx = -1;  ty = +0; break;
            case 2:   tx = +1;  ty = +0; break;
            case 3:   tx = +0;  ty = -1; break;
            case 4:   tx = +0;  ty = +1; break;
        }
	}while(!this->moveObject(tx, ty));
}

#endif