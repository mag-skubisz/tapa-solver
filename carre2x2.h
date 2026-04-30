#ifndef CARRE2X2_H
#define CARRE2X2_H

#include "grille.h"
#include "dimacs.h"

// Prend la grille et un fichier en paramètre, écrit les clauses pour les carrés 2x2 dans le fichier au format DIMACS
//retourne le nombre de clauses écrites
int carre2x2_to_dimacs(Grille *G, FILE *fichier);

#endif