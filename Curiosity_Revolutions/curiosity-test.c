#include "environnement.h"
#include "interprete.h"
#include "programme.h"
#include "observateur_exo5.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void gestion_erreur_terrain(erreur_terrain e) {
  switch (e) {
  case OK:
    //printf("EVERITHING IS OK\n");
    break;
  case ERREUR_FICHIER:
    printf("Erreur lecture du terrain : erreur d'ouverture du fichier\n");
    exit(1);
  case ERREUR_LECTURE_LARGEUR:
    printf("Erreur lecture du terrain : erreur de lecture de la largeur\n");
    exit(1);
  case ERREUR_LECTURE_HAUTEUR:
    printf("Erreur lecture du terrain : erreur de lecture de la hauteur\n");
    exit(1);
  case ERREUR_LARGEUR_INCORRECTE:
    printf("Erreur lecture du terrain : largeur incorrecte\n");
    exit(1);
  case ERREUR_HAUTEUR_INCORRECTE:
    printf("Erreur lecture du terrain : hauteur incorrecte\n");
    exit(1);
  case ERREUR_CARACTERE_INCORRECT:
    printf("Erreur lecture du terrain : caractère incorrect\n");
    exit(1);
  case ERREUR_LIGNE_TROP_LONGUE:
    printf("Erreur lecture du terrain : ligne trop longue\n");
    exit(1);
  case ERREUR_LIGNE_TROP_COURTE:
    printf("Erreur lecture du terrain : ligne trop courte\n");
    exit(1);
  case ERREUR_LIGNES_MANQUANTES:
    printf("Erreur lecture du terrain : lignes manquantes\n");
    exit(1);
  case ERREUR_POSITION_ROBOT_MANQUANTE:
    printf(
        "Erreur lecture du terrain : position initiale du robot manquante\n");
    exit(1);
  }
}
void affichage_position_programme(erreur_programme e) {
  int i;
  printf("Ligne %d, colonne %d :\n", e.num_ligne, e.num_colonne);
  printf("%s\n", e.ligne);
  /* Impression de e.num_colonne-1 espaces */
  for (i = 1; i < e.num_colonne; i++) {
    printf(" ");
  }
  /* Impression d'un curseur de position */
  printf("^\n");
}
void gestion_erreur_programme(erreur_programme e) {
  switch (e.type_err) {
  case OK_PROGRAMME:
    //printf("EVERITHING IS OK\n");
    break;
  case ERREUR_FICHIER_PROGRAMME:
    printf("Erreur lecture du programme : erreur d'ouverture du fichier\n");
    exit(2);
  case ERREUR_BLOC_NON_FERME:
    printf("Erreur lecture du programme : bloc non fermé\n");
    exit(2);
  case ERREUR_FERMETURE_BLOC_EXCEDENTAIRE:
    printf("Erreur lecture du programme : fermeture de bloc excédentaire\n");
    affichage_position_programme(e);
    exit(2);
  case ERREUR_COMMANDE_INCORRECTE:
    printf("Erreur lecture du programme : commande incorrecte\n");
    affichage_position_programme(e);
    exit(2);
  }
}

typedef enum {N, S,F,O,P} events;

int main(int argc, char **argv){
    erreur_terrain errt;
    Environnement envt;
    Programme prog;
    erreur_programme errp;
    etat_inter etat;
    resultat_inter res;
    Robot r;
    Terrain t;
    Orientation robot_o; 
    char terrain_file[100];
    char programme_file[100];
    FILE *f;
    int max_steps;
    char expected_event[sizeof(char)];
    int expected_x; int expected_y;
    int robot_x; int robot_y;
    char expected_oriantation[sizeof(char)];
    char end[sizeof(char)];
    int steps;
    State_new state;
    state = Unknown;
    int waterFlag;


    if (argc < 2) {
    printf("Usage: %s <fichier> \n", argv[0]);
    return 1;
    }
    
    f = fopen(argv[1], "r");
    fscanf(f,"%s",terrain_file);
    printf("Terrain name is: %s\n",terrain_file);
    fscanf(f,"%s",programme_file);
    printf("Terrain name is: %s\n",programme_file);
    fscanf(f,"%d",&max_steps);
    printf("Max steps is: %d\n",max_steps);
    int expected_max_steps = max_steps;
    fscanf(f,"%s",expected_event);
    printf("Event is: %s\n",expected_event);
    fscanf(f,"%d %d",&expected_x,&expected_y);
    printf("X and Y are: %d %d\n",expected_x,expected_y);
    fscanf(f,"%s",expected_oriantation);
    printf("Oriantation is: %c\n",expected_oriantation[0]);

    

    errt = initialise_environnement(&envt, terrain_file,&state,&waterFlag);
    gestion_erreur_terrain(errt);
    errp = lire_programme(&prog, programme_file);
    gestion_erreur_programme(errp);

  
    init_etat(&etat);
    do {
        res = exec_pas(&prog, &envt, &etat,&state,&waterFlag);
        steps++;
        /* Affichage du terrain et du robot */
        //afficher_envt(&envt);
    } while (res == OK_ROBOT);
    switch (res){
        case OK_ROBOT:
            end[0] = 'N';
            break;
        case ARRET_ROBOT:
            end[0] = 'F';
            break;
        case SORTIE_ROBOT:
            end[0] = 'S';
            break;
        case CRASH_ROBOT:
            end[0] = 'O';
            break;
        case PLOUF_ROBOT:
            end[0] = 'P';
            break;
        default:
            printf("Unrecocnised char!\n");
            break;
    }
    if(steps!=expected_max_steps){
        printf("NOT the same number of steps\n");
        exit(1);
    }

    if(end[0] != expected_event[0]){
            printf("Error not the same event\n");
            exit(1);
    }

    if(end[0] == 'N'|| end[0] =='F'){
      r= envt.r;
    t=envt.t;
    position(&r,&robot_x,&robot_y);
    if(robot_x != expected_x || robot_y != expected_y){
        printf("Error not the same position\n");
        exit(1);
    }
    robot_o = orient(&r);
    char robot_char_o[sizeof(char)];
    switch (robot_o)
        {
        case Nord:
            robot_char_o[0] = 'N';
            break;
        case Est:
            robot_char_o[0] = 'E';
            break;
        case Sud:
            robot_char_o[0] = 'S';
            break;
        case Ouest:
            robot_char_o[0] = 'O';
            break;
        default:
            break;
        }
        if(robot_char_o[0]!= expected_oriantation[0]){
            printf("Error not the same opriantation\n");
            exit(1);
        }
    }
    

    printf("OK\n");

    fclose(f);
}