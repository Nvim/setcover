#include "../include/header.h"
#include <stdio.h>

/***** FONCTIONS COMMUNES AUX 2 ALGORITHMES ******/

void afficher_tab(int *tab, int taille) {
  for (int i = 0; i < taille; i++) {
    printf("%d, ", tab[i]);
    if (i == 19) {
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

// retourne 1 si l'individu est une solution, 0 sinon
int check_solution_mieux(int *individu) {

  int *tab = calloc(TAILLE_SET, sizeof(int));
  for (int i = 0; i < NB_SOUS_ENSEMBLES; i++) {
    if (individu[i] == 1) {
      for (int j = 0; j < TAILLE_SET; j++) {
        if (sous_ensembles[i][j] == 1) {
          tab[j] = 1;
        }
      }
    }
  }
  for (int i = 0; i < TAILLE_SET; i++) {
    if (tab[i] == 0) {
      return 0;
    }
  }
  return 1;
}

void free_2d(int **tab, int x) {
  if (tab == NULL) {
    return;
  }
  for (int i = 0; i < x; i++) {
    if (tab[i] != NULL) {
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
void init_from_file(const char *file) {
  FILE *fp = fopen(file, "r");
  if (fp == NULL) {
    fprintf(stderr, "\n erreur fichier");
    return;
  }

  char buffer[128];

  if (fgets(buffer, sizeof(buffer), fp) == NULL) {
    fprintf(stderr, "\nerreur lecture fichier");
    return;
  }
  TAILLE_SET = atoi(buffer);
  if (fgets(buffer, sizeof(buffer), fp) == NULL) {
    fprintf(stderr, "\nerreur lecture fichier");
    return;
  }
  NB_SOUS_ENSEMBLES = atoi(buffer);
  printf("\nTaille Set: %d, Nb sous_ensembles: %d\n", TAILLE_SET,
         NB_SOUS_ENSEMBLES);

  if (TAILLE_SET > sizeof(buffer)) {
    fprintf(stderr, "\n Erreur: L'ensemble U est trop grand\n");
    return;
  }

  // allocation matrice:
  sous_ensembles = alloc_tab_2d(NB_SOUS_ENSEMBLES, TAILLE_SET);
  if (sous_ensembles == NULL) {
    printf("\nerr\n");
    return;
  }

  char c = ' ';
  int j = 0;
  // lecture et remplissage de la matrice
  for (int i = 0; i < NB_SOUS_ENSEMBLES; i++) {

    while (j < TAILLE_SET) {
      fread(&c, 1, 1, fp);
      sous_ensembles[i][j] = atoi(&c);
      j++;
    }
    j = 0;
    fseek(fp, SEEK_CUR, 1);
  }
}
