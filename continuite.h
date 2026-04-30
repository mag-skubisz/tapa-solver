#ifndef CONTINUITE_H
#define CONTINUITE_H


#include "grille.h"
#include "dimacs.h"

int index_distance(int v, int d, int nb_cases);
int continuite_to_dimacs(FILE *fichier, Grille *G, int* nb_variables);

#endif