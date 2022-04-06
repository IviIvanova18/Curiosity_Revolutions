#ifndef _OBSERVATEUR_H_
#define _OBSERVATEUR_H_

typedef enum{
    A,
    G,
    D,
    M,
}Alphabet;

typedef enum{
    Unknown,
    Mesured,
    Error,
}State;

// think what is taking
void lire_etat(State state);

void print_if_error(State state);

//like last year switch case 
void transition_etat(Alphabet alphabet, State *state);

void print_curr_state(State state);

void print_alpha(Alphabet alpha);

#endif