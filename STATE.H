#ifndef STATES_H
#define STATES_H
struct State {
    char name[20];
    int duration;
    int val;
    int last;
} states[20];
int totStates = 0;
void State_init(struct State *this, char name[], int val, int duration, int last) {
    strcpy(this->name, name);   this->val = val;    this->duration = duration;  this->last = last;
}
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
    int i, loc, stateVal, stateDuration;
    bool found = false;
    char stateName[20];
    if(strcmp(name, "immunity1") == 0) {
        strcpy(stateName, "IMM");   stateVal = 0;   stateDuration = 5;  //kill monsters
    } else if(strcmp(name, "breaker1") == 0) {
        strcpy(stateName, "BRK");   stateVal = 0;   stateDuration = 10; //kill monsters and remove state
    } else if(strcmp(name, "fullvision1") == 0) {
        strcpy(stateName, "VIS");   stateVal = 0;   stateDuration = 2;  //give vision of full map
    } else if(strcmp(name, "slow1") == 0) {
        strcpy(stateName, "SLW");   stateVal = 1;   stateDuration = 5;  //decrease speed of time
    } else if(strcmp(name, "fast1") == 0) {
        strcpy(stateName, "FST");   stateVal = 1;   stateDuration = 5;  //increase speed of time
    } else if(strcmp(name, "repel1") == 0) {
        strcpy(stateName, "RPL");   stateVal = 1;   stateDuration = 5;  //decrease range
    } else if(strcmp(name, "attract1") == 0) {
        strcpy(stateName, "ATR");   stateVal = 1;   stateDuration = 5;  //increase range
    } else if(strcmp(name, "invisible1") == 0) {
        strcpy(stateName, "INV");   stateVal = 0;   stateDuration = 5;  //invisible player
    } else if(strcmp(name, "hinder1") == 0) {
        strcpy(stateName, "HND");   stateVal = 0;   stateDuration = 10;  //hinder player input 50% chance
    } else if(strcmp(name, "paralyze1") == 0) {
        strcpy(stateName, "PAR");   stateVal = 0;   stateDuration = 2;  //paralyze player
    } else if(strcmp(name, "stop1") == 0) {
        strcpy(stateName, "STP");   stateVal = 0;   stateDuration = 2;  //stop monsters
    } else return false;
    for(i = 0; i < totStates; i++) {
        if(strcmp(name, stateName) == 0) {
            if(strcmp(states[i].name, stateName) == 0)   break;
        } 
    }
    if(i != totStates)  found = true;
    if(totStates<10 || found == true) {
        if(found == false) {
            totStates++;    loc = totStates-1;
        } else  loc = i;
        State_init(&states[loc], stateName, stateVal, stateDuration, time);
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