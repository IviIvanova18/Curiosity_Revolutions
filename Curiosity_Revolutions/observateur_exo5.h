#ifndef _OBSERVATEUR_H_
#define _OBSERVATEUR_H_

int flag;

typedef enum{
    A,
    G,
    D,
    M1,
    M3,
    M7
}Alphabet_new;

typedef enum{
    Unknown,
    MesuredInFront,
    MesuredRight,
    MesuredLeft,
    MesuredFrontAndRight,
    MesuredFrontAndLeft,
    MesuredRightAndLeft,
    MesuredAll,
    Error,
}State_new;

// think what is taking
void lire_etat(State_new state);

void print_if_error(State_new state);

//like last year switch case 
void transition_etat(Alphabet_new alphabet, State_new *state);

void print_curr_state(State_new state);

void print_if_water(int waterFlag);


#endif