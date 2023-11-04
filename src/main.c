#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAILLE_POPULATION 10
#define NB_GEN 5
#define PM 0.3
#define PNBM 0.2

int TAILLE_SET;
int NB_SOUS_ENSEMBLES;

int **sous_ensembles = NULL;

void afficher_tab(int *tab, int taille) {
  for (int i = 0; i < taille; i++) {
    printf("%d, ", tab[i]);
    if(i==19){
      printf("\n");
    }
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
  int *individu = alloc_tab(NB_SOUS_ENSEMBLES);
  for (int i = 0; i < NB_SOUS_ENSEMBLES; i++) {
    individu[i] = rand() % 2;
  }
  return individu;
}

// parcours chaque bit, et le modifie avec une chance de 0.2
void muter(int *individu) {
  int bool;
  for (int i = 0; i < NB_SOUS_ENSEMBLES; i++) {
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
  int m;
  for (int i = TAILLE_POPULATION; i < 2 * TAILLE_POPULATION; i++) {
    m = rand() % 10;
    if (m < 3) {
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

  int taille_vects = count_ones(individu, NB_SOUS_ENSEMBLES);
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

void free_2d(int **tab, int x){
  if(tab == NULL){
    return;
  }
  for(int i = 0; i<x; i++){
    if(tab[i] != NULL){
      free(tab[i]);
      tab[i] = NULL;
    }
  }
  free(tab);
  tab = NULL;
}

// Lecture fichier matrice.
// 1ere ligne = nombre d'éléments de U. 
// 2 eme ligne = nombre d'ensembles de S
void init_from_file(const char * file){
  FILE *fp = fopen(file, "r");
  if(fp == NULL){
    fprintf(stderr, "\n erreur fichier");
    return ;
  }

  char buffer[128];

  if(fgets(buffer, sizeof(buffer), fp) == NULL){
    fprintf(stderr, "\nerreur lecture fichier");
    return ;
  }
  TAILLE_SET = atoi(buffer);
  if(fgets(buffer, sizeof(buffer), fp) == NULL){
    fprintf(stderr, "\nerreur lecture fichier");
    return ;
  }
  NB_SOUS_ENSEMBLES = atoi(buffer);
  printf("\nTaille Set: %d, Nb sous_ensembles: %d\n", TAILLE_SET, NB_SOUS_ENSEMBLES);

  if(TAILLE_SET > sizeof(buffer)){
    fprintf(stderr, "\n Erreur: L'ensemble U est trop grand\n");
    return ;
  }

  // allocation matrice:
  sous_ensembles = alloc_tab_2d(NB_SOUS_ENSEMBLES, TAILLE_SET);
  if (sous_ensembles == NULL) {
    printf("\nerr\n");
    return ;
  }

  char c = ' ';
  int j = 0;
  // lecture et remplissage de la matrice
  for(int i = 0; i<NB_SOUS_ENSEMBLES; i++){

    while(j < TAILLE_SET){
      fread(&c, 1, 1, fp);
      sous_ensembles[i][j] = atoi(&c);
      j++;
    }
    j = 0;
    fseek(fp, SEEK_CUR, 1);
  }
}


int main() {

  srand(time(NULL));

  int i;
  char filename[] = "src/test.txt";
  init_from_file(filename);

  int ** population = init_population();
  int * scores;
  for(i = 0; i<NB_GEN; i++){
    nouvelle_generation(population);
    appliquer_mutations(population);
    
    scores = get_scores(population);
    bubble_sort(population, scores);
    printf("\n --- Gen %d ---\n", i);
    afficher_tab(scores, 2*TAILLE_POPULATION);
  }

  printf("\n --- Résultats ---\n\n");
  afficher_res_format(population);

  printf("\n --- Scores ---\n\n");
  afficher_tab(scores, 2*TAILLE_POPULATION);

  free_2d(sous_ensembles, NB_SOUS_ENSEMBLES);
  free_2d(population, 2*TAILLE_POPULATION);
  free(scores);

  return 0;
}

// int main() {
//   srand(time(NULL));
//
//   init_from_file("src/test.txt");
//   int **population = init_population();
//   nouvelle_generation(population);
//   printf("\n -- Population Initiale --\n");
//   afficher_res_format(population);
//
//   printf("\n -- Scores --\n");
//   int * scores = get_scores( population);
//   afficher_tab(scores, 2*TAILLE_POPULATION);
//
//   printf("\n -- Scores Triés --\n");
//   debug( population);
//
//   free(scores);
//   free_2d(population, 2*TAILLE_POPULATION);
//   free_2d(sous_ensembles, SOUS_ENSEMBLES);
//   return 0;
// }
