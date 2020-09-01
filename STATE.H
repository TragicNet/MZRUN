#ifndef STATES_H
#define STATES_H
struct State {
    char name[20];
    int duration;
    int val;
    int last;
} states[10];
int totStates = 0;
void State_init(struct State *this, char name[], int val, int duration, int last) {
    strcpy(this->name, name);   this->val = val;    this->duration = duration;  this->last = last;
}
/*void State_cpy(struct State *this, struct State trg) {
    strcpy(this->name, trg.name);   this->val = trg.val;    this->duration = trg.duration;  this->last = trg.last;
}*/
void State_sort(struct State states[]) {
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
bool State_add(struct State states[], char name[], int time) {
    int i, loc;
    bool found = false;
    for(i = 0; i < totStates; i++) {
        if(strcmp(name, "immunity1") == 0) {
            if(strcmp(states[i].name, "IMM") == 0)   break;
        } else if(strcmp(name, "fullvision1") == 0) {
            if(strcmp(states[i].name, "VIS") == 0)   break;
        } else if(strcmp(name, "slow1") == 0) {
            if(strcmp(states[i].name, "SLW") == 0)   break;
        } else if(strcmp(name, "fast1") == 0) {
            if(strcmp(states[i].name, "FST") == 0)   break;
        } else if(strcmp(name, "repel1") == 0) {
            if(strcmp(states[i].name, "RPL") == 0)   break;
        } else if(strcmp(name, "attract1") == 0) {
            if(strcmp(states[i].name, "ATR") == 0)   break;
        } 
    }
    if(i != totStates)  found = true;
    if(totStates<10 || found == true) {
        if(found == false) {
            totStates++;    loc = totStates-1;
        } else  loc = i;
        if(strcmp(name, "immunity1") == 0) {
            State_init(&states[loc], "IMM", 0, 5, time);
        } else if(strcmp(name, "fullvision1") == 0) {
            State_init(&states[loc], "VIS", 0, 10, time);
        } else if(strcmp(name, "slow1") == 0) {
            State_init(&states[loc], "SLW", 1, 5, time);
        } else if(strcmp(name, "fast1") == 0) {
            State_init(&states[loc], "FST", 1, 5, time);
        } else if(strcmp(name, "repel1") == 0) {
            State_init(&states[loc], "RPL", 1, 5, time);
        } else if(strcmp(name, "attract1") == 0) {
            State_init(&states[loc], "ATR", 1, 5, time);
        } else return false;
    }
    State_sort(states);
    return true;
}
void State_remove(struct State states[], int loc) {
    int i;
    if(totStates>0) {
        for(i = loc; i<totStates; i++) {
            states[i] = states[i+1];
        }
        totStates--;
    }
}
#endif