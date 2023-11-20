#include "../include/header.h"

int TAILLE_SET;
int NB_SOUS_ENSEMBLES;

int **sous_ensembles = NULL;

// int main() {
//
//   srand(time(NULL));
//
//   int i;
//   char filename[] = "src/test.txt";
//   init_from_file(filename);
//
//   int ** population = init_population();
//   int * scores;
//   for(i = 0; i<NB_GEN; i++){
//     nouvelle_generation(population);
//     appliquer_mutations(population);
//     
//     scores = get_scores(population);
//     bubble_sort(population, scores);
//     printf("\n --- Gen %d ---\n", i);
//     afficher_tab(scores, 2*TAILLE_POPULATION);
//   }
//
//   printf("\n --- Résultats ---\n\n");
//   afficher_res_format(population);
//
//   printf("\n --- Scores ---\n\n");
//   afficher_tab(scores, 2*TAILLE_POPULATION);
//
//   free_2d(sous_ensembles, NB_SOUS_ENSEMBLES);
//   free_2d(population, 2*TAILLE_POPULATION);
//   free(scores);
//
//   return 0;
// }

int recursions = 0;
int main(){
  srand(time(NULL));
  char filename[] = "src/test.txt";
  init_from_file(filename);

  int i = 0;
  int * current = calloc(NB_SOUS_ENSEMBLES, sizeof(int));

  clock_t debut = clock();
  branchement(current, 0, NB_SOUS_ENSEMBLES);
  clock_t fin = clock();

  double temps = (double)(fin - debut) / CLOCKS_PER_SEC;
  printf("Temps d'exécution : %f secondes\n", temps);
  printf("Récursions: %d\n", recursions);
  return 0;
}
