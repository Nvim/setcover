#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define TAILLE_POPULATION 10
#define NB_GEN 5
#define PM 0.3
#define PNBM 0.2

extern int TAILLE_SET;
extern int NB_SOUS_ENSEMBLES;

extern int **sous_ensembles;
extern int recursions;

void afficher_tab(int *tab, int taille);

void afficher_tab_2d(int **tab, int x, int y);

int count_ones(int *tab, int taille);

int count_zeros(int *tab, int taille);

void swap(int *a, int *b);

int *alloc_tab(int taille);

int **alloc_tab_2d(int taille_tab, int taille_sous_tab);

// input:;1, 4, 7}
// output:;1,0,0,1,0,0,1}
int *vectorize(int *sous_ensemble, int taille_sous_ensemble);

// parcours chaque bit, et le modifie avec une chance de 0.2
void muter(int *individu);

// retourne un nouvel individu basé sur papa et maman
int *reproduction(int *papa, int *maman, int delim);

// retourne un tableau de taille 2*TAILLE_POPULATION.
// la premiere moitié est remplie avec des individus aléatoires.
int **init_population();

// genere un couple aléatoirement parmi les TAILLE_POPULATION 1ers membres de la
// population.
int **couple_aleatoire(int **population);

// remplit la derniere moitié de population avec les enfants de la premiere
void nouvelle_generation(int **population);

// applique des mutations aux enfants (2eme moitié de la population)
void appliquer_mutations(int **population);

// retourne un tableau de vecteurs correspondant à l'individu.
// taille du tableau = nombre de '1' dans l'individu.
int **vectorize_individu(int *individu);

// retounre un tableau correspondant à la fusion de tous les vecteurs
int *merege_vectors(int **vects, int vects_taille);

// retourne 1 si l'individu est une solution, 0 sinon
int check_solution(int *individu);

// retourne un tableau de tous les scores d'une double population.
// score = count_zeros()+1 si solution, 0 sinon.
// => 0 <= score <= SOUS_ENSEMBLES
int *get_scores(int **double_population);

void bubble_sort(int ** population, int *scores);

void afficher_res_format(int **population);

void free_2d(int **tab, int x);

// Lecture fichier matrice.
// 1ere ligne = nombre d'éléments de U. 
// 2 eme ligne = nombre d'ensembles de S
void init_from_file(const char * file);

void jsp(int *current);
void branchement(int *current, int index);
