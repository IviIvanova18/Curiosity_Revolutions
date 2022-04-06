#include "observateur.h"
#include <stdio.h>
#include <stdlib.h>

void transition_etat(Alphabet alphabet,State *state){
    switch(*state){
        case Unknown:
            switch (alphabet)
            {
            case M:
                *state = Mesured;
                break;
            case D:
            case G:
                *state=  Unknown;
               
                break;
            case A:
                *state= Error;
                break;
            default:
                printf("Unknown command!\n");
                break;
            }
            break;
        case Mesured:
            switch (alphabet)
            {
            case M:
                *state= Mesured;
                break;
            case A:
            case D:
            case G:
                *state= Unknown;
                break;
            default:
                printf("Unknown command!\n");
                break;
            }
            break;
        case Error:
            *state= Error;
            break;
        default:
            printf("Unknown command!\n");
            break;
    }
}

void print_if_error(State state){
    if(state == Error){
        printf("ERROR THE ROBOT DIDN'T MESURE BEFORE MOVING FORWARD!\n");
    }
}

void print_curr_state(State state){
    switch (state)
    {
    case Error:
            printf("Error state \n");
        break;
    case Mesured:
            printf("Mesure\n");
        break;
    case Unknown:
            printf("Unknown\n");
        break;
    default:
        break;
    }
}

// void print_alpha(Alphabet alpha){
//     switch (alpha)
//     {
//     case A:
//         printf("A\n");
//         break;
//     case D:
//         printf("D\n");
//         break;
//     case G:
//         printf("G\n");
//         break;
//     case M:
//         printf("M\n");
//         break;
//     default:
//         break;
//     }
// }
