#include "../include/header.h"
#include <stdlib.h>

int *init_individu() {
  int *individu = alloc_tab(NB_SOUS_ENSEMBLES);
  for (int i = 0; i < NB_SOUS_ENSEMBLES; i++) {
    individu[i] = rand() % 2;
  }
  return individu;
}

// parcours chaque bit, et le modifie avec une chance de 0.2
void muter(int *individu) {
  double random;
  for (int i = 0; i < NB_SOUS_ENSEMBLES; i++) {
    random = (double)rand() /  RAND_MAX; // Nombre aléatoire entre 0 et 1
    if (random < PNBM) {   // une chance sur PNBM
      if (individu[i] == 1) {
        individu[i] = 0;
      } else {
        individu[i] = 1;
      }
    }
  }
}

// retourne un nouvel individu basé sur papa et maman
int *reproduction(int *papa, int *maman, int delim) {
  int *bebe = alloc_tab(NB_SOUS_ENSEMBLES);
  int i;
  for (i = 0; i < delim; i++) {
    bebe[i] = papa[i];
  }
  for (i = delim; i < NB_SOUS_ENSEMBLES; i++) {
    bebe[i] = maman[i];
  }
  return bebe;
}

// retourne un tableau de taille 2*TAILLE_POPULATION.
// la premiere moitié est remplie avec des individus aléatoires.
int **init_population() {
  int **population = alloc_tab_2d(2 * TAILLE_POPULATION, NB_SOUS_ENSEMBLES);
  for (int i = 0; i < TAILLE_POPULATION; i++) {
    population[i] = init_individu();
  }
  return population;
}

// genere un couple aléatoirement parmi les TAILLE_POPULATION 1ers membres de la
// population.
int **couple_aleatoire(int **population) {
  int **couple = alloc_tab_2d(2, NB_SOUS_ENSEMBLES);
  int papa, maman;

  couple[0] = alloc_tab(NB_SOUS_ENSEMBLES);
  couple[1] = alloc_tab(NB_SOUS_ENSEMBLES);

  papa = rand() % TAILLE_POPULATION;
  couple[0] = population[papa];

  do {
    maman = rand() % TAILLE_POPULATION;
  } while (papa == maman);
  couple[1] = population[maman];

  return couple;
}

// remplit la derniere moitié de population avec les enfants de la premiere
void nouvelle_generation(int **population) {
  int **couple;

  for (int i = TAILLE_POPULATION; i < 2 * TAILLE_POPULATION; i += 2) {
    couple = couple_aleatoire(population);
    population[i] = reproduction(couple[0], couple[1], 3);
    population[i + 1] = reproduction(couple[1], couple[0], 3);
  }
}

// applique des mutations aux enfants (2eme moitié de la population)
void appliquer_mutations(int **population) {
  double m;
  for (int i = TAILLE_POPULATION; i < 2 * TAILLE_POPULATION; i++) {
    m = (double)rand() / RAND_MAX;
    if (m < PM) {
      muter(population[i]);
    }
  }
}

// retourne un tableau de vecteurs correspondant à l'individu.
// taille du tableau = nombre de '1' dans l'individu.
int **vectorize_individu(int *individu) {
  int nb_sous_ensembles = count_ones(individu, NB_SOUS_ENSEMBLES);

  int **tab = alloc_tab_2d(nb_sous_ensembles, TAILLE_SET);
  int tab_index = 0;

  // parcours des valeurs de l'individu
  for (int i = 0; i < NB_SOUS_ENSEMBLES; i++) {
    if (individu[i] == 1) {
      tab[tab_index] = sous_ensembles[i];
      tab_index++;
    }
  }
  return tab;
}

// retourne un tableau de tous les scores d'une double population.
// score = count_zeros()+1 si solution, 0 sinon.
// => 0 <= score <= SOUS_ENSEMBLES
int *get_scores(int **double_population) {
  int *scores = (int *)malloc(sizeof(int) * 2 * TAILLE_POPULATION);

  for (int i = 0; i < 2 * TAILLE_POPULATION; i++) {
    if (check_solution(double_population[i])) {
      scores[i] = count_zeros(double_population[i], NB_SOUS_ENSEMBLES)+1;
    } else {
      scores[i] = 0;
    }
  }
  return scores;
}

void bubble_sort(int ** population, int *scores){
  int i, j, swapped;
  for (i = 0; i < 2*TAILLE_POPULATION - 1; i++) {
    swapped = 0;
    for (j = 0; j < 2*TAILLE_POPULATION - i - 1; j++) {
      if (scores[j] < scores[j + 1]) {
        swap(&scores[j], &scores[j + 1]);
        int *temp = population[j];
        population[j] = population[j+1];
        population[j+1] = temp;
        swapped = 1;
      }
    }
    if(swapped == 0){
      break;
    }
  }
}

void afficher_res_format(int **population) {
  printf("   -  A  B  C  D  E  F  -\n");
  for (int i = 0; i < TAILLE_POPULATION; i++) {
    printf("* %02d: ", i);
    for (int j = 0; j < NB_SOUS_ENSEMBLES; j++) {
      if (population[i][j] == 1) {
        printf("X  ");
      } else {
        printf("   ");
      }
    }
    printf("\n");
  }
}
