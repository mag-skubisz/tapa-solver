#ifndef INDICES_H
#define INDICES_H
#include "grille.h"


// Prend la grille et un fichier en paramètre, écrit les clauses pour les indices dans le fichier au format DIMACS
// ignore_numbers indique si les chiffres des indices doivent être ignorés ou non (utile pour les règles de base)
int indices_to_dimacs(Grille *G, FILE *f, int ignore_numbers);
#endif 