#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAILLE_SET 7
#define SOUS_ENSEMBLES 6
#define TAILLE_POPULATION 20
#define NB_GEN 40
#define PM 0.3
#define PNBM 0.2

int **sous_ensembles = NULL;

typedef struct solution {
  int *tab;
  int taille;
} solution;

int A[] = {1, 4, 7};
int B[] = {1, 4};
int C[] = {4, 5, 7};
int D[] = {3, 5, 6};
int E[] = {2, 3, 6, 7};
int F[] = {2, 7};

void afficher_tab(int *tab, int taille) {
  for (int i = 0; i < taille; i++) {
    printf("%d, ", tab[i]);
  }
  printf("\n");
}

void afficher_tab_2d(int **tab, int x, int y) {
  for (int i = 0; i < x; i++) {
    printf("* %d: ", i);
    afficher_tab(tab[i], y);
  }
}

int count_ones(int *tab, int taille) {
  int count = 0;
  for (int i = 0; i < taille; i++) {
    if (tab[i] == 1) {
      count++;
    }
  }
  return count;
}

int count_zeros(int *tab, int taille) {
  int count = 0;
  for (int i = 0; i < taille; i++) {
    if (tab[i] == 0) {
      count++;
    }
  }
  return count;
}

void swap(int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

int *alloc_tab(int taille) {
  int *tab = (int *)malloc(sizeof(int) * taille);
  return tab;
}

int **alloc_tab_2d(int taille_tab, int taille_sous_tab) {
  int **tab = (int **)malloc(sizeof(int *) * taille_tab);
  for (int i = 0; i < taille_tab; i++) {
    tab[i] = (int *)malloc(sizeof(int) * taille_sous_tab);
  }
  return tab;
}

// input: {1, 4, 7}
// output: {1,0,0,1,0,0,1}
int *vectorize(int *sous_ensemble, int taille_sous_ensemble) {
  int *vector = calloc(TAILLE_SET, sizeof(int));

  for (int i = 0; i < taille_sous_ensemble; i++) {
    vector[sous_ensemble[i] - 1] = 1;
  }
  return vector;
}

int *init_individu() {
  int *individu = alloc_tab(SOUS_ENSEMBLES);
  for (int i = 0; i < SOUS_ENSEMBLES; i++) {
    individu[i] = rand() % 2;
  }
  return individu;
}

// parcours chaque bit, et le modifie avec une chance de 0.2
void muter(int *individu) {
  int bool;
  for (int i = 0; i < SOUS_ENSEMBLES; i++) {
    bool = rand() % 5; // 0 1 2 3 4
    if (bool == 4) {   // une chance sur 5 du coup
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
  int *bebe = alloc_tab(SOUS_ENSEMBLES);
  int i;
  for (i = 0; i < delim; i++) {
    bebe[i] = papa[i];
  }
  for (i = delim; i < SOUS_ENSEMBLES; i++) {
    bebe[i] = maman[i];
  }
  return bebe;
}

// retourne un tableau de taille 2*TAILLE_POPULATION.
// la premiere moitié est remplie avec des individus aléatoires.
int **init_population() {
  int **population = alloc_tab_2d(2 * TAILLE_POPULATION, SOUS_ENSEMBLES);
  for (int i = 0; i < TAILLE_POPULATION; i++) {
    population[i] = init_individu();
  }
  return population;
}

// genere un couple aléatoirement parmi les TAILLE_POPULATION 1ers membres de la
// population.
int **couple_aleatoire(int **population) {
  int **couple = alloc_tab_2d(2, SOUS_ENSEMBLES);
  int papa, maman;

  couple[0] = alloc_tab(SOUS_ENSEMBLES);
  couple[1] = alloc_tab(SOUS_ENSEMBLES);

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
  int m;
  for (int i = TAILLE_POPULATION; i < 2 * TAILLE_POPULATION; i++) {
    m = rand() % 10;
    if (m < 3) {
      printf("\n\t- MUTATION: individu %d -\n", i);
      printf("Avant: ");
      afficher_tab(population[i], SOUS_ENSEMBLES);
      muter(population[i]);
      printf("\nApres: ");
      afficher_tab(population[i], SOUS_ENSEMBLES);
    }
  }
}

// retourne un tableau de vecteurs correspondant à l'individu.
// taille du tableau = nombre de '1' dans l'individu.
int **vectorize_individu(int *individu) {
  int nb_sous_ensembles = count_ones(individu, SOUS_ENSEMBLES);

  int **tab = alloc_tab_2d(nb_sous_ensembles, TAILLE_SET);
  int tab_index = 0;

  // parcours des valeurs de l'individu
  for (int i = 0; i < SOUS_ENSEMBLES; i++) {
    if (individu[i] == 1) {
      tab[tab_index] = sous_ensembles[i];
      tab_index++;
    }
  }
  return tab;
}

// retounre un tableau correspondant à la fusion de tous les vecteurs
int *merege_vectors(int **vects, int vects_taille) {
  int *merged = (int *)calloc(TAILLE_SET, sizeof(int));

  for (int i = 0; i < vects_taille; i++) {
    for (int j = 0; j < TAILLE_SET; j++) {
      if (vects[i][j] == 1) {
        merged[j] = 1;
      }
    }
  }
  return merged;
}

// retourne 1 si l'individu est une solution, 0 sinon
int check_solution(int *individu) {

  int taille_vects = count_ones(individu, SOUS_ENSEMBLES);
  int **vects = vectorize_individu(individu);

  int *merged = merege_vectors(vects, taille_vects);

  for (int i = 0; i < TAILLE_SET; i++) {
    if (merged[i] == 0) {
      return 0;
    }
  }
  return 1;
}

// retourne un tableau de tous les scores d'une double population.
// score = count_zeros()+1 si solution, 0 sinon.
// => 0 <= score <= SOUS_ENSEMBLES
int *get_scores(int **double_population) {
  int *scores = (int *)malloc(sizeof(int) * 2 * TAILLE_POPULATION);

  for (int i = 0; i < 2 * TAILLE_POPULATION; i++) {
    if (check_solution(double_population[i])) {
      scores[i] = count_zeros(double_population[i], SOUS_ENSEMBLES)+1;
    } else {
      scores[i] = 0;
    }
  }
  return scores;
}

void bubble_sort(int ** population){
  int *scores = get_scores(population);
  int i, j;
  for (i = 0; i < 2*TAILLE_POPULATION - 1; i++) {
    for (j = 0; j < 2*TAILLE_POPULATION - i - 1; j++) {
      if (scores[j] < scores[j + 1]) {
        swap(&scores[j], &scores[j + 1]);
        int *temp = population[j];
        population[j] = population[j+1];
        population[j+1] = temp;
      }
    }
  }
}

void afficher_res_format(int **population) {
  printf("   -  A  B  C  D  E  F  -\n");
  for (int i = 0; i < TAILLE_POPULATION; i++) {
    printf("* %02d: ", i);
    for (int j = 0; j < SOUS_ENSEMBLES; j++) {
      if (population[i][j] == 1) {
        printf("X  ");
      } else {
        printf("   ");
      }
    }
    printf("\n");
  }
}

int main() {

  srand(time(NULL));
  int i;
  // 6 tableaux de 7 entiers
  sous_ensembles = alloc_tab_2d(SOUS_ENSEMBLES, TAILLE_SET);
  if (sous_ensembles == NULL) {
    printf("\nerr\n");
    return 1;
  }
  sous_ensembles[0] = vectorize(A, 3);
  sous_ensembles[1] = vectorize(B, 2);
  sous_ensembles[2] = vectorize(C, 3);
  sous_ensembles[3] = vectorize(D, 3);
  sous_ensembles[4] = vectorize(E, 5);
  sous_ensembles[5] = vectorize(F, 2);

  int ** population = init_population();
  int * scores;
  for(int i = 0; i<NB_GEN; i++){
    nouvelle_generation(population);
    appliquer_mutations(population);
    
    scores = get_scores(population);
    bubble_sort(population);
  }

  printf("\n --- Résultats ---\n\n");
  afficher_res_format(population);

  printf("\n --- Scores ---\n\n");
  afficher_tab(scores, 2*TAILLE_POPULATION);

}
