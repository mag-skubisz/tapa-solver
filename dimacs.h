#ifndef DIMACS_H
#define DIMACS_H

#include "grille.h"

int index_case(int ligne, int colonne, int largeur);
int initialiserDimacs(Grille *G, FILE *Fichier);
int modifierClausesDimacs(int nb_clauses, int nb_variables, FILE *fichier);

#endif