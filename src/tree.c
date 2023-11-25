#include "../include/header.h"

// retourne 1 si il reste des solutions en disant non à la courante
int can_say_no(int *current) {
  int i = 0;
  char passed = 0;

  int cur_cpy[NB_SOUS_ENSEMBLES];
  memcpy(cur_cpy, current, sizeof(int) * NB_SOUS_ENSEMBLES);

  // mettre l'element actuel à 0:
  while (i < NB_SOUS_ENSEMBLES) {
    if (cur_cpy[i] == -1) {
      passed = 1;
      cur_cpy[i] = 0;
      i++;
      continue;
    }
    if (passed == 1) {
      cur_cpy[i] = 1;
    }
    i++;
  }

  if (check_solution(current)) {
    return 1;
  } else
    return 0;
}

// Retourne 1 si il est toujours possible d'obtenir une solution
int can_continue(int *current, int index) {

  int cur_cpy[NB_SOUS_ENSEMBLES];
  memcpy(cur_cpy, current, sizeof(int) * NB_SOUS_ENSEMBLES);

  for (int i = index; i < NB_SOUS_ENSEMBLES; i++) {
    cur_cpy[i] = 1;
  }
  if (check_solution(cur_cpy)) {
    return 1;
  }
  return 0;
}

// bloqué car on aura i > NB_SOUS_ENSEMBLES après un premier passage sur
// l'arbre. on a potentiellement bcp plus de 6 appels récursif pour tout
// parcourir, mais on doit trouver moyen de s'arreter à la fin de current.
void jsp(int *current) {
  static int i = 0;

  if (i == NB_SOUS_ENSEMBLES) {
    return;
  }
  current[i] = -1;

  if (can_say_no(current)) {
    current[i] = 0;
    i++;
    jsp(current);
  } else {
    current[i] = 1;
    i++;
    jsp(current);
  }

  printf("\n\t---Solution Minimale---\n");
  afficher_tab(current, NB_SOUS_ENSEMBLES);
}

// best = NB_SOUS_ENSEMBLES au 1er appel
int branchement(int *current, int index, int best) {
  recursions++;
  // + de 1s que best ?
  if (count_ones(current, NB_SOUS_ENSEMBLES) > best) {
    return best;
  }
  // Déjà solution ?
  if (check_solution(current)) {
    printf("\n\t---Solution Minimale---\n");
    printf("**%d**\t", count_ones(current, NB_SOUS_ENSEMBLES));
    afficher_tab(current, NB_SOUS_ENSEMBLES);
    return count_ones(current, NB_SOUS_ENSEMBLES);
  }
  // Out of Range ?
  if (index >= NB_SOUS_ENSEMBLES) {
    return best;
  }
  // OUI
  current[index] = 1;
  // if (can_continue(current, index + 1)) {
    best = branchement(current, index + 1, best);
  // }
  // NON
  current[index] = 0;
  if (can_continue(current, index + 1)) {
    best = branchement(current, index + 1, best);
  }
  return best;
}
