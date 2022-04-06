#include "observateur_exo5.h"
#include <stdio.h>
#include <stdlib.h>

void transition_etat(Alphabet_new alphabet,State_new *state){
    switch(*state){
        case Unknown:
            switch (alphabet)
            {
            case M1:
                *state = MesuredInFront;
                break;
            case M3:
                *state = MesuredRight;
                break;
            case M7:
                *state = MesuredLeft;
                break;
            case D:
            case G:
            case A:
                *state= Error;
                break;
            default:
                printf("Unknown command!\n");
                break;
            }
            break;
        case MesuredInFront:
            switch (alphabet)
            {
            case M1:
                *state= MesuredInFront;
                break;
            case M3:
                *state= MesuredFrontAndRight;
                break;
            case M7:
                *state= MesuredFrontAndLeft;
                break;
            case A:
                *state= Unknown;
                break;
            case D:
            case G:
                *state= Error;
                break;
            default:
                printf("Unknown command!\n");
                break;
            }
            break;
        case MesuredLeft:
            switch (alphabet)
            {
            case M1:
                *state= MesuredFrontAndLeft;
                break;
            case M3:
                *state= MesuredRightAndLeft;
                break;
            case M7:
                *state= MesuredLeft;
                break;
            case A:
            case D:
                *state = Error;
                break;
            case G:
                *state= Unknown;
                break;
            default:
                printf("Unknown command!\n");
                break;
            }
            break;
        case MesuredRight:
            switch (alphabet)
            {
            case M1:
                *state= MesuredFrontAndRight;
                break;
            case M3:
                *state= MesuredRight;
                break;
            case M7:
                *state= MesuredRightAndLeft;
                break;
            case A:
            case G:
                *state = Error;
                break;
            case D:
                *state= Unknown;
                break;
            default:
                printf("Unknown command!\n");
                break;
            }
            break;
        case MesuredFrontAndRight:
            switch (alphabet)
            {
            case A:
            case D:
                *state = Unknown;
                break;
            case M7:
                *state = MesuredAll;
                break;
            case G:
                *state = Error;
                break;
            case M1:
            case M3:
                *state = MesuredFrontAndRight;
                break;
            default:
                printf("Unknown command!\n");
                break;
            }
            break;
        case MesuredFrontAndLeft:
            switch (alphabet)
            {
            case A:
            case G:
                *state = Unknown;
                break;
            case M3:
                *state = MesuredAll;
                break;
            case D:
                *state = Error;
                break;
            case M1:
            case M7:
                *state = MesuredFrontAndRight;
                break;
            default:
                printf("Unknown command!\n");
                break;
            }
            break;
        case MesuredRightAndLeft:
            switch (alphabet)
            {
            case G:
            case D:
                *state = Unknown;
                break;
            case M1:
                *state = MesuredAll;
                break;
            case A:
                *state = Error;
                break;
            case M7:
            case M3:
                *state = MesuredFrontAndRight;
                break;
            default:
                printf("Unknown command!\n");
                break;
            }
            break;
        case MesuredAll:
            switch (alphabet)
            {
            case A:
            case D:
            case G:
                *state = Unknown;
                break;
            case M1:
            case M3:
            case M7:
                *state = MesuredFrontAndRight;
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

void print_if_error(State_new state){
    if(state == Error){
        printf("ERROR THE ROBOT DIDN'T MESURE BEFORE DOING A MOVE!\n");
    }else{
        printf("OK!\n");

    }
}

void print_curr_state(State_new state){
    switch (state)
    {
    case Error:
            printf("Error state \n");
        break;
    case MesuredRight:
            printf("Mesure right\n");
        break;
    case MesuredLeft:
            printf("Mesure left\n");
        break;
    case MesuredInFront:
            printf("Mesure in front\n");
        break;
    case Unknown:
            printf("Unknown\n");
        break;
    default:
        break;
    }
}

void print_if_water(int flag){
    if(flag == 1){
        printf("Attention robot passes near water!\n");
    }
}

