#ifndef STATES_H
#define STATES_H

struct State {
    char fullName[20];
    char name[20];
    int duration;
    int val;
    int last;
} states[20];
int totStates = 0;
void printState(struct State *this) {
    printf("\n%s, %s, %d, %d, %d\n", this->fullName, this->name, this->val, this->duration, this->last);
}
void State_init(struct State *this, const char fullName[], const char name[], const int val, const int duration, const int last) {
	strcpy(this->fullName, fullName);	strcpy(this->name, name);   this->val = val;    this->duration = duration;  this->last = last;
}
void sortStates(struct State states[]) {
	int i, j;
	struct State temp;
	for(i = 0; i<totStates; i++)
		for(j = 0; j<totStates; j++)
			if(states[i].duration > states[j].duration) {
				temp = states[i];
				states[i] = states[j];
				states[j] = temp;
			}
}
bool getState(struct State *this, const char name[]) {
	FILE *file;
	int i = 0;
	bool found = false;
	char *ch, str[50], vals[5][20];
	file = fopen("MZRUN/files/STATES.DAT", "r" );
	if(file==NULL) {
		clrscr();   printf("STATES.DAT missing");  exit(0);
	}
	while (fgets(str, 100, file) != NULL) {
		ch = strtok(str, ", \t");
		if(strcmp(name, ch) != 0)   continue;
		while (ch != NULL) {
			strcpy(vals[i++], ch);
			ch = strtok(NULL, ", \t");
		}
		State_init(this, vals[0], vals[1], atoi(vals[2]), atoi(vals[3]), atoi(vals[4]));
		found = true;    break;
    }
	fclose(file);
    return found;
}
bool addState(struct State states[], const char name[], const int time) {
	int i, loc;
	bool found = false;
    struct State temp;
    if(!getState(&temp, name)) return false;
	for(i = 0; i < totStates; i++) {
		if(strcmp(states[i].name, temp.name) == 0)   break;
	}
	if(i != totStates)  found = true;
	if(totStates<20 || found == true) {
		if(found == false) {
			totStates++;    loc = totStates-1;
		} else  loc = i;
		states[loc] = temp;
        states[loc].last = time;
	}
	sortStates(states);
    return true;
}
void removeState(struct State states[], const int loc) {
    int i;
    if(totStates>0) {
        for(i = loc; i<totStates; i++) {
            states[i] = states[i+1];
        }
        totStates--;
    }
}

#endif