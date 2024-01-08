#include "../include/header.h"

int TAILLE_SET;
int NB_SOUS_ENSEMBLES;

int **sous_ensembles = NULL;

char alphabet[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
                   'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
                   'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

void main_genetique() {
  int **population = init_population();
  int *scores;

  clock_t debut = clock();
  for (int i = 0; i < NB_GEN; i++) {
    nouvelle_generation(population);
    appliquer_mutations(population);

    scores = get_scores(population);
    // bubble_sort(population, scores);
    selection_sort(population, scores);

    printf("\n --- Gen %d ---\n", i);
    afficher_tab(scores, 2 * TAILLE_POPULATION);
  }
  clock_t fin = clock();

  printf("\n --- Résultats ---\n\n");
  afficher_res_format(population);

  printf("\n --- Scores ---\n\n");
  afficher_tab(scores, 2 * TAILLE_POPULATION);

  double temps = (double)(fin - debut) / CLOCKS_PER_SEC;
  printf("Temps d'exécution : %f secondes\n", temps);
  free_2d(population, 2 * TAILLE_POPULATION);
  free_2d(sous_ensembles, NB_SOUS_ENSEMBLES);
  free(scores);
}

int recursions = 0;
void main_branchement() {
  int *current = calloc(NB_SOUS_ENSEMBLES, sizeof(int));
  int min;

  clock_t debut = clock();
  // branchement(current, 0, NB_SOUS_ENSEMBLES);
  min = branchement(current, 0, NB_SOUS_ENSEMBLES);
  clock_t fin = clock();

  double temps = (double)(fin - debut) / CLOCKS_PER_SEC;
  printf("Temps d'exécution : %f secondes\n", temps);
  printf("Récursions: %d\n", recursions);
  printf("Min: %d\n", min);

  free_2d(sous_ensembles, NB_SOUS_ENSEMBLES);
  free(current);
}

int main() {
  srand(time(NULL));
  char filename[] = "datasets/test.txt";
  init_from_file(filename);

  main_genetique();
  // main_branchement();
  return 0;
}
