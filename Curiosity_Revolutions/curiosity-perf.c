#include "generation_terrains.h"
#include "environnement.h"
#include "interprete.h"
#include "programme.h"
#include "terrain.h"
#include "robot.h"
#include "observateur_exo5.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


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



int main(int argc, char **argv){
    int N, i, l, h;
    float dObst;
    FILE *resFile;
    Terrain T;
    long graine;
    int nb_max_steps;
    erreur_programme errp;
    Programme prog;
    Robot r;
    Environnement envt;
    etat_inter etat;
    resultat_inter res;
    float rand_dens;
    int end;
    int steps=1;
    int nb_in_water = 0;
    int nb_crashed=0;
    int nb_blocked = 0;
    int sum_all_steps = 0;
    int valid =0;
    int bad;
    State_new state;
    state = Unknown;
    int waterFlag;

    if (argc < 9) {
    printf(
        "Usage: %s <fichier_programme> <N> <largeur> <hauteur> <densite_obstacle> <graine> <nb_max_steps> <fichier_res> \n",
        argv[0]);
    return 1;
    }

    errp = lire_programme(&prog, argv[1]);
    gestion_erreur_programme(errp);

    N = strtol(argv[2], NULL, 10);
    l = strtol(argv[3], NULL, 10);
    h = strtol(argv[4], NULL, 10);
    dObst = strtof(argv[5], NULL);
    graine = strtol(argv[6], NULL, 10);  
    nb_max_steps = strtol(argv[7], NULL, 10);
    

    if (l > DIM_MAX || l % 2 == 0) {
        printf("Largeur incorrecte : doit être impaire et <= %d\n", DIM_MAX);
        return 1;
    }
    if (h > DIM_MAX || h % 2 == 0) {
      printf("Hauteur incorrecte : doit être impaire et <= %d\n", DIM_MAX);
      return 1;
    }
    if ((dObst > 1) || (dObst < 0)) {
      printf("Densité incorrecte : doit être comprise entre 0 et 1\n");
      return 1;
    }
    if (nb_max_steps <= 0) {
      printf("Nombre de pas maximum incorrect : doit être plus grand que 0\n");
      return 1;
    }

    // Ouverture du fichier résultat
    resFile = fopen(argv[8], "w");
    // Écriture du nombre de terrains
    fprintf(resFile, "%d\n", N);

    srand(time(&graine));

    init_robot(&r, l/2, h/2, Est);

    for(i=0;i<N;i++){
      
      rand_dens = ((float)rand()/(float)(RAND_MAX))*dObst;

      generation_aleatoire(&T,l,h,rand_dens);
      // printf("%f\n",rand_dens);
      // afficher_terrain(&T);
      
      
      if(existe_chemin_vers_sortie(T)==1){
        envt.r = r;
        envt.t=T;
        init_etat(&etat);
        
        res = exec_pas(&prog, &envt, &etat,&state,&waterFlag);
        for (steps = 2 ; steps < nb_max_steps && res == OK_ROBOT ; steps++) {
            res = exec_pas(&prog, &envt, &etat,&state,&waterFlag);          
        }

        switch (res) {
            case OK_ROBOT:
              end = -1;
              nb_blocked++;
              fprintf(resFile,"ROBOT is still blocked in the maze. Max number of steps is reached : %d\n",steps);
              break;
            case SORTIE_ROBOT:
              fprintf(resFile,"Le robot est sorti :-) With number of executed steps :%d\n",steps);
              
              sum_all_steps+=steps;
              valid++;
              break;
            case ARRET_ROBOT:
                end = -1;
                fprintf(resFile,"%d\n",end);
                fprintf(resFile,"ROBOT stopped there is no more steps to be executed! With number of executed steps :%d\n",steps);

                nb_blocked++;
                break;
            case PLOUF_ROBOT:
                end = -2;
                fprintf(resFile,"%d\n",end);
                fprintf(resFile,"ROBOT fell in water! With number of executed steps :%d\n",steps);
                nb_in_water++;
                break;
            case CRASH_ROBOT:
                end = -3;
                fprintf(resFile,"%d\n",end);
                fprintf(resFile,"ROBOT crashed! With number of executed steps :%d\n",steps);
                nb_crashed++;
                break;
            case ERREUR_PILE_VIDE:
              fprintf(resFile,"%s","ERREUR : pile vide\n");
              bad++;
              break;
            case ERREUR_ADRESSAGE:
              bad++;
              fprintf(resFile,"%s","ERREUR : erreur d'adressage\n");
              break;
            case ERREUR_DIVISION_PAR_ZERO:
              bad++;
              fprintf(resFile,"%s","ERREUR : division par 0\n");
              break;
          }
       
        
        }else{
          i--;
        }
        print_if_error(state);
    }

    fprintf(resFile,"\n");
    fprintf(resFile,"---------------------------------------------\n");
    fprintf(resFile,"  Event     |     Times    |    Percentage     \n");
    fprintf(resFile,"---------------------------------------------\n");
    fprintf(resFile," In water   |       %d      |      %f      \n",nb_in_water,(((float)nb_in_water/(float)N)*100));
    fprintf(resFile,"---------------------------------------------\n");
    fprintf(resFile," Crashed    |       %d      |      %f   \n",nb_crashed,((float)nb_crashed/(float)N)*100);
    fprintf(resFile,"---------------------------------------------\n");
    fprintf(resFile," Blocked    |       %d      |      %f    \n",nb_blocked,((float)nb_blocked/(float)N)*100);
    fprintf(resFile,"---------------------------------------------\n");
    fprintf(resFile," Exit maze  |       %d     |       %f    \n",valid,(float)valid/(float)N*100);
    fprintf(resFile,"---------------------------------------------\n");
    fprintf(resFile,"Average steps executed : %f\n",(float)sum_all_steps/(float)valid);
    fprintf(resFile,"---------------------------------------------\n");


    fprintf(resFile,"\n");
    fprintf(resFile,"Robot was in water %d times ",nb_in_water);
    fprintf(resFile,"which is %f percent of all landscapes\n",(((float)nb_in_water/(float)N)*100));
    fprintf(resFile,"Robot crashed %d times which is %f percent of all landscapes\n",nb_crashed,((float)nb_crashed/(float)N)*100);
    
    fprintf(resFile,"Robot is still in the maze %d times which is %f percent of all landscapes\n",nb_blocked,((float)nb_blocked/(float)N)*100);
    
    if (valid != 0){
      fprintf(resFile,"All valid landscapes are %f percent of all generated\n",(float)valid/(float)N*100);
      fprintf(resFile,"All valid landscapes are %d and average steps was %f\n",valid,(float)sum_all_steps/(float)valid);
    }
}

// void print_stats(){
//   fprintf(resFile)
// }